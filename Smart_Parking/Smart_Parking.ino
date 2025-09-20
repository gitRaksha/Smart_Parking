/*******************************************          Includes         *******************************************************/
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

/*******************************************          Defines          *******************************************************/
#define TRIGGER_PIN         18
#define ECHO_PIN            19
#define GREEN_LED           32
#define BLUE_LED            26
#define RED_LED             25
#define PHOTORESISTOR_PIN   34
#define LIGHTS_PIN1         12
#define LIGHTS_PIN2         14
#define SERVO_MOTOR_PIN     13
#define BUTTON_PIN          16
#define MAX_NUMBER_OF_CARS  5


/*******************************************          Classes           *******************************************************/
WiFiClient espClient;
PubSubClient client(espClient);
Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

/*******************************************     Wifi & MQTT Server     *******************************************************/
const char* ssid = "Your Wifi SSID";                        /* Your Wifi SSID */
const char* password = "Your Wifi Password";                /* Your Wifi Password */
const char* mqtt_server = "broker.hivemq.com"; /* i used hivemq because ---test.mosquitto.org--- was down at the moment, you can change it and don't forget to change it in node-red*/


/*******************************************         Variables          *******************************************************/
uint8_t num_of_cars = 0;
uint8_t num_of_blocked_cars = 0;
char buffer[3];
uint16_t distance;
uint16_t sensorValue;
uint8_t lightIntensity;

/*******************************************   Ultrasonic Measurement   *******************************************************/
uint16_t readUltrasonicDistance()
{
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout after 30ms
    if (duration == 0) return 9999; // No valid reading
    uint16_t distance = duration * 0.034 / 2;

    // Ignore out-of-range readings
    if (distance > 500 || distance < 10) return 9999;
    return distance;
}


/*******************************************    Setup Wifi Connection   *******************************************************/
void setup_wifi()
{ 
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) 
    { 
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

/*******************************************    Reconnect To MQTT      *******************************************************/
void reconnect() 
{ 
    while(!client.connected()) 
    {
        Serial.println("Attempting MQTT connection...");

        if(client.connect("ESPClient")) 
        {
            Serial.println("Connected");
            client.subscribe("Ehab/Parking/NumNoPlaceCars");
            client.subscribe("Ehab/Parking/CarsInParking");
        } 
        else 
        {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

/*******************************************           Setup            *******************************************************/
void setup()
{
    Serial.begin(115200);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(PHOTORESISTOR_PIN, INPUT);
    pinMode(LIGHTS_PIN1, OUTPUT);
    pinMode(LIGHTS_PIN2, OUTPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);

    //myservo.attach(SERVO_MOTOR_PIN, 500, 2400);
    myservo.attach(SERVO_MOTOR_PIN);
    myservo.write(0);

    setup_wifi(); 
    client.setServer(mqtt_server, 1883);

    digitalWrite(RED_LED, HIGH);
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("Welcome");
}

/*******************************************           Loop            *******************************************************/
void loop()
{
    /* Reconnect if disconnection happens */
    if(!client.connected()) { reconnect(); }
  lcd.begin();
  lcd.setCursor(3, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print(String(MAX_NUMBER_OF_CARS - num_of_cars) + " places left");

    /* PhotoResistor part */
    sensorValue = analogRead(PHOTORESISTOR_PIN); // Read the value from the photoresistor

    if(sensorValue < 500)
    {
        digitalWrite(LIGHTS_PIN1, HIGH);
        digitalWrite(LIGHTS_PIN2, HIGH);
    }
    else
    {
        digitalWrite(LIGHTS_PIN1, LOW);
        digitalWrite(LIGHTS_PIN2, LOW);
    }

    /* Exit mechanism */
    if(HIGH == digitalRead(BUTTON_PIN) && num_of_cars > 0)
    {
        while(digitalRead(BUTTON_PIN));
        num_of_cars--;
        dtostrf(num_of_cars, 3, 0, buffer);
        client.publish("Ehab/Parking/CarsInParking", buffer, false);
        myservo.write(90);
        delay(3000);
        myservo.write(0);
    }
    else { /* Nothing */ }

    /* Reading Distance */
    distance = readUltrasonicDistance();
    Serial.print("Distance (cm): ");
    Serial.println(distance);
    /* Enter mechanism */
    if(distance < 20)
    {
        if(num_of_cars == MAX_NUMBER_OF_CARS)
        {
            char buff[4];
            num_of_blocked_cars++;
            dtostrf(num_of_blocked_cars, 4, 0, buff);
            client.publish("Ehab/Parking/NumNoPlaceCars", buff, false);
            for(uint8_t i = 0; i < 6; i++)
            {
                digitalWrite(RED_LED, !digitalRead(RED_LED));
                delay(500);
            }
        }
        else
        {
            num_of_cars++;
            dtostrf(num_of_cars, 3, 0, buffer);
            client.publish("Ehab/Parking/CarsInParking", buffer, false);
            digitalWrite(BLUE_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            delay(1500);
            myservo.write(90);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(BLUE_LED, LOW);
            delay(3000);
            myservo.write(0);
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
        }
    }
    else { /* Nothing */ }
    delay(100);
}

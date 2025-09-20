# 🚗 Smart Parking IoT System

This project implements a **Smart Parking System** using an **ESP32 microcontroller** with real-time monitoring via **Node-RED**.  
It detects cars entering/exiting, controls a barrier with a servo motor, manages parking lights, and sends parking data to an MQTT broker for visualization.

---

## 📸 Features
- 🚘 Car detection with **ultrasonic sensor**
- 🚦 **RGB LED** + **LCD I2C display** for parking status
- 🎚️ **Servo motor** controls the entry barrier
- 🌙 **Photoresistor (LDR)** for automatic night lighting
- 📡 **MQTT integration** with HiveMQ broker
- 📊 **Node-RED dashboard** for real-time monitoring:
  - Available spaces
  - Number of blocked cars
  - Parking activity chart

---

## 📂 Project Structure

```text
.
├── image1.jpg
├── image2.jpg
├── LICENSE
├── node_red
│   └── node_red_flow.json
├── Smart_Parking
│   └── Smart_Parking.ino
└── video.mp4

 ```

---

## 🛠️ Hardware Used
- ESP32 DevKit v1
- Ultrasonic sensor (HC-SR04)
- Servo motor (SG90)
- RGB LED
- LCD 16x2 with I2C module
- LDR (photoresistor)
- 2 x LEDs for parking lights
- Push button (exit simulation)

---

## 🧩 Software & Dependencies
- **Arduino IDE** (ESP32 board support installed)
- Libraries:
  - `WiFi.h`
  - `PubSubClient.h` (MQTT)
  - `ESP32Servo.h`
  - `LiquidCrystal_I2C.h`
- **Node-RED** (for dashboard + MQTT subscription) [Install Node RED](https://nodered.org/docs/getting-started/)
- MQTT Broker: [HiveMQ Public Broker](https://www.hivemq.com/public-mqtt-broker/)

---

## ⚙️ Setup
1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/smart-parking-esp32.git

2. Open in Arduino IDE and install required libraries.
3. Update Wi-Fi SSID and password in the code:

   ```bash
    const char* ssid = "YOUR_WIFI";
    const char* password = "YOUR_PASSWORD";

4. Upload the code to ESP32.
5. Import the provided Node-RED flow JSON into your Node-RED editor.
6. Connect to the MQTT broker and launch the dashboard.

---

## 📊 Node-RED Dashboard

- Displays the number of available parking slots
- Shows blocked cars when parking is full
- Plots a chart of parking activity over time
- (Dashboard JSON is included in the repo.)

---

## 📜 License

This project is open-source under the MIT License.
Feel free to use and adapt it for learning or development purposes.

---

## 👤 Author

- **Marwen Mekni**

---

⭐ If you like this project, feel free to fork, star, or reuse it for your own IoT projects!








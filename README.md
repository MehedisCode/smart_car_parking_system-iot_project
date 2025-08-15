<img src="https://github.com/user-attachments/assets/e26a8275-b105-41af-b908-88ac2aaae93b" width="350"/>
<img src="https://github.com/user-attachments/assets/3059765d-7c90-461c-af32-55b977a3daf5" width="490"/>

# 🚗 ESP32 Smart Parking System with Web Monitoring

This is an **ESP32-based Smart Parking Management System** with **IR Sensors, Ultrasonic Sensors, LCD Display, Servo Gate Control, Buzzer Alerts, and Web Monitoring** via Wi-Fi.

The system detects parking slot occupancy, controls entry/exit gates automatically, notifies when parking is full, and serves a **live web page** showing slot availability.

---

## 📌 Features

- **Parking Slot Detection**:
  - Slot 1 → IR Sensor
  - Slot 2 → Ultrasonic Distance Sensor
  - Slot 3 → IR Sensor
- **Automatic Gate Control**
  - Opens when a vehicle is detected at the entry/exit
  - Works only if parking space is available
- **LCD Display (16x2 I2C)** for real-time slot and gate status
- **Buzzer Alert** when car is too close to wall
- **Wi-Fi Web Server**
  - Live parking status viewable in browser
  - Auto-refresh every second
- **Parking Full Detection** to block entry
- **Serial Monitor Logs** for debugging

---

## 🛠️ Hardware Requirements

| Component                  | Quantity |
|----------------------------|----------|
| ESP32 Development Board    | 1        |
| IR Obstacle Sensor         | 2        |
| HC-SR04 Ultrasonic Sensor  | 3        |
| Servo Motor (SG90/MG90S)   | 1        |
| Active Buzzer              | 1        |
| I2C LCD Display (16x2)     | 1        |
| Jumper Wires               | —        |
| Breadboard / PCB           | —        |
| 5V Power Supply            | 1        |

---

## 🔌 Pin Configuration

### Slot Sensors
| Slot | Sensor Type | Pin (ESP32) |
|------|-------------|-------------|
| 1    | IR Sensor   | GPIO 32     |
| 2    | Ultrasonic  | Trig: 26 / Echo: 25 |
| 3    | IR Sensor   | GPIO 27     |

### Entry Gate Sensor
| Sensor Type | Pin (ESP32) |
|-------------|-------------|
| Ultrasonic  | Trig: 33 / Echo: 14 |

### Exit Gate Sensor
| Sensor Type | Pin (ESP32) |
|-------------|-------------|
| Ultrasonic  | Trig: 19 / Echo: 18 |

### Others
| Device       | Pin |
|--------------|-----|
| Servo Motor  | 13  |
| Buzzer       | 12  |
| I2C LCD      | SDA/SCL (default ESP32 pins) |

---

## 📡 Wi-Fi Configuration
Edit these lines inside `setup()` to match your Wi-Fi network:
```
const char* ssid = "YourSSID"
const char* password = "YourPassword"
```

---

## 🔧 Installation & Setup

1. Install **Arduino IDE** and add the ESP32 board to the board manager.
2. Install required libraries:
   - `WiFi.h` (Built-in ESP32)
   - `WebServer.h` (Built-in ESP32)
   - `ESP32Servo.h`
   - `LiquidCrystal_I2C.h`
3. Connect your hardware as per the pin configuration.
4. Upload the sketch to your ESP32.
5. Open Serial Monitor (115200) to see:
   - Wi-Fi connection logs
   - Assigned IP address
6. Access the Web Page:
   - Enter the ESP32 IP in a web browser from the same network.
   - The page auto-refreshes every second showing slot status.

---

## 🖥️ Web UI Preview

**LCD Output**

<img src="https://github.com/user-attachments/assets/2ba50887-577c-4d67-b5ec-d60dfe948a9c" width="350"/><br>

**Browser View**

<img src="https://github.com/user-attachments/assets/ab697fd4-0789-4006-b48f-f40ab23bdd48" width="350"/>

---

## ⚙️ System Workflow

1. **Check Slot Sensors** → Determine occupancy.
2. **Full Parking?**
   - **Yes** → Show "Full" on LCD, block gate opening.
   - **No** → Allow gate to open on car detection.
3. **Car Entry/Exit Detection** → Control servo position.
4. **Buzzer** → Trigger if slot 2 car is too close to wall.
5. **LCD + Web Server** → Continuous real-time updates.

---

## 🚀 Future Improvements
- Add mobile app integration
- Implement cloud logging
- Integrate license plate recognition
- Add security camera streaming

---

## 📜 License
This project is open-source under the **MIT License**.






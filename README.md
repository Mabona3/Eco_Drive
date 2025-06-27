# ECO-DRIVE

**Graduation Project – Computer Science & Engineering – Minia University**

**Eco-Drive** is an intelligent automotive system designed to promote environmentally conscious driving and enhance vehicle diagnostics. 
It can be installed in any car to monitor engine performance, driving behavior, and emission levels.
The system provides real-time feedback to help drivers improve fuel efficiency and reduce harmful emissions.
Additionally, it offers remote monitoring through both a mobile app and a web interface.

---

## 🔧 System Overview

Eco-Drive consists of the following components:

### 1. OBD-II Interface
A standard device that connects to the vehicle's OBD-II port (available in cars manufactured after 1996). It collects real-time diagnostic and performance data from the car’s ECU (Engine Control Unit).

### 2. Main Control Unit (ESP32)
The core microcontroller of the system:
- Receives and processes data from the OBD-II interface.
- Evaluates vehicle performance, driver behavior, and emission data.
- Sends alerts and diagnostic reports to the driver.
- Interfaces with GPS and RFID modules for location tracking and user authentication.

### 3. Mobile App
A smartphone application that:
- Displays driving behavior and vehicle status.
- Receives alerts (e.g., high emissions, driving issues).
- Provides remote access and control features.

### 4. Web Interface
A responsive web dashboard that mirrors the functionality of the mobile app for broader accessibility:
- Remote access to vehicle data and analytics.
- Driver performance history.
- Maintenance notifications and system alerts.

---

## 📡 Connection Diagram

### GPS Module Connection

| GPS Pin | ESP32 Pin |
|--------:|:----------:|
| VCC     | 5V         |
| GND     | GND        |
| RX      | GPIO 16    |
| TX      | GPIO 17    |

### RFID Module Connection

| RFID Pin | ESP32 Pin |
|:--------:|:-----------:|
| VCC      | 3.3V        |
| GND      | GND         |
| SDA      | GPIO 5      |
| SCK      | GPIO 18     |
| MOSI     | GPIO 23     |
| MISO     | GPIO 19     |
| RST      | GPIO 22     |
| IRQ      | Not used    |

---

## 💻 Firmware Design (Arduino Style)

The firmware is implemented in C-style using modular programming rather than object-oriented design. 
Each module handles a specific task and communicates via message queues and synchronization primitives (e.g., semaphores).

### 🛰 GPS Thread

- Periodically reads data from the GPS module.
- Pushes location data into a fixed-size queue for use by other system components (e.g., logging, backend sync).

### 🛂 RFID Thread

- Periodically reads data from the tag.
- Sends user data to the backend to identify or switch the active user.

---

This system aims to bridge the gap between sustainable driving and modern automotive technology, empowering drivers to make smarter, greener decisions.


# ECO-DRIVE
This is a graduation project in CSE Minia University. This project focuses on the automotives and the environment. 
It is a system that can be installed in any car to monitor the car's performance and the driver's behavior.
The system will give the driver feedback on how to drive more efficiently and will also monitor the car's 
performance and alert the driver if there is any problem with the car.
The system will also monitor the car's emissions and alert the driver if the emissions are too high. 
The system will also have a mobile app that will allow the driver to monitor the car's performance and the driver's behavior remotely.
The system will also have a web interface that will allow the driver to monitor the car's performance and the driver's behavior remotely.

## System Components
The system will consist of the following components:
1. OBD-II Interface: This is a device that will be installed in the car and will read data from the car's OBD-II port. The OBD-II port is a standard port that is present in all cars manufactured after 1996. The OBD-II interface will read data from the car's engine control unit (ECU) and will send this data to the system's main unit.
2. Main Unit: This is the main unit of the system and will be installed in the car. It will receive data from the OBD-II interface and will process this data to monitor the car's performance and the driver's behavior. It will also send alerts to the driver if there is any problem with the car or if the emissions are too high.
3. Mobile App: This is a mobile app that will allow the driver to monitor the car's performance and the driver's behavior remotely. The app will also allow the driver to receive alerts from the system and to control the system remotely.
4. Web Interface: This is a web interface that will allow the driver to monitor the car's performance and the driver's behavior remotely. The web interface will also allow the driver to receive alerts from the system and to control the system remotely.

## Connection Diagram
The connections is as follows in each component and the microcontroller:
### GPS

| GPS | ESP32  |
|:---:|:------:|
| VCC |   5V   |
| GND |  GND   |
| RX  | pin 16 |
| TX  | pin 17 |


### RFID


| RFID |  ESP32  |
|:----:|:-------:|
| VCC  |   3.3V  |
| GND  |   GND   |
|  SDA | pin 5   |
|  SCK | pin 18  |
| MOSI | pin 23  |
| MISO | pin 19  |
|  RST | pin 22  |
|  IRQ | pin -   |


## Arduino's Implementation
The Implementation of the Arduino is made in C-style in term of modules not classes and structures to handle the data sent between threads.
The Implemented threads till now are the RFID thread, the GPS thread.

### GPS
The GPS thread is quite simple just read from the GPS every *specified* time and push the data in a queue that has the size that is *specified*.

### RFID
The RFID is running through interrupts it is closed by a semaphore that is triggered using the RFID key that open the semaphore, and it sends to the backend to
switch the user to the RFID requested user and then close the semaphore.


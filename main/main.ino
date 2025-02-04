#define ESP32
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include "constant.h"
#include "wifinet.h"
#include "firebase.h"
#include "gps.h"
#include "rfid.h"
#include "carStatus.h"

void setup() {
  Serial.begin(115200);
  WiFi_init();
  Firebase_init();
  GPS_init();
  RFID_init();
  xTaskCreate(
    GPS_read,
    "Reading GPS", 
    10000,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    RPMSensor_read,
    "Reading GPS", 
    10000,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    FuelSensor_read,
    "Reading GPS", 
    10000,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    TempSensor_read,
    "Reading GPS", 
    10000,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    RFID_read,
    "Reading RFID", 
    10000,
    NULL,
    1,
    NULL
  );
}

void loop(){}

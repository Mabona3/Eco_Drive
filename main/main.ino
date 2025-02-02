#define ESP32
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include "constant.h"
#include "wifinet.h"
#include "firebase.h"
#include "gps.h"
#include "carStatus.h"

void setup() {
  Serial.begin(115200);
  WiFiInit();
  FirebaseInit();
  GPSInit();
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
}

void loop(){}

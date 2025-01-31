#define ESP32
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include "constant.h"
#include "wifinet.h"
#include "firebase.h"
#include "gps.h"

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
    GPS_read,
    "Reading GPS", 
    10000,
    NULL,
    1,
    NULL
  );
}

void loop(){}

#define ESP32
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include "constant.h"
#include "wifinet.h"
#include "gps.h"
#include "rfid.h"
#include "carStatus.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  WiFi_init();
  GPS_init();
  RFID_init();
  MQTT_init();
  xTaskCreate(
    GPS_read,
    "Reading GPS",
    2048,
    NULL,
    1,
    NULL);
  xTaskCreate(
    RFID_read,
    "Reading RFID",
    2048,
    NULL,
    1,
    NULL);
  xTaskCreate(
    MQTT_task,
    "Reading RFID",
    2048,
    NULL,
    1,
    NULL);
  // xTaskCreate(
  //   FuelSensor_read,
  //   "Reading RFID",
  //   2048,
  //   NULL,
  //   1,
  //   NULL);
}

void loop() {
}

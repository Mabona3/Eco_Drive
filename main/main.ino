#define ESP32
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include "constant.h"
#include "wifinet.h"
#include "gps.h"
#include "rfid.h"
#include "carStatus.h"
#include "mqtt.h"
#include "queue_prepare.h"

void InitAll();

void setup() {
  Serial.begin(115200);
  InitAll();
  xTaskCreate(
    QueuePrepare_task,
    "Data Prepare Task",
    2048,
    NULL,
    1,
    NULL);
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
    1024,
    NULL,
    2,
    NULL);
  xTaskCreate(
    MQTT_task,
    "Upload MQTT",
    8192,
    NULL,
    1,
    NULL);
  xTaskCreate(
    CarStatus_task,
    "Car Status",
    2048,
    NULL,
    1,
    NULL);
}

void loop() {
}

void InitAll() {
  WiFi_init();
  GPS_init();
  RFID_init();
  MQTT_init();
  QueuePrepare_init();
  CarStatus_init();
}

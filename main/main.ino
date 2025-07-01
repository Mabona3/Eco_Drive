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
  if (xTaskCreate(MQTT_task, "Upload MQTT", 5120, NULL, 1, NULL) != pdPASS) {
    Serial.println("mqtt Creation Failed");
  }
  if (xTaskCreate(CarStatus_task, "OBD Task", 8192, NULL, 1, NULL) != pdPASS) {
    Serial.println("car Creation Failed");
    Serial.println(esp_get_free_heap_size());
  }
  if (xTaskCreate(QueuePrepare_task, "Data Prepare Task", 3072, NULL, 1, NULL) != pdPASS) {
    Serial.println("data pre Creation Failed");
    Serial.println(esp_get_free_heap_size());
  }
  if (xTaskCreate(GPS_read, "Reading GPS", 1536, NULL, 1, NULL) != pdPASS) {
    Serial.println("gps Creation Failed");
    Serial.println(esp_get_free_heap_size());
  }
  if (xTaskCreate(RFID_read, "Reading RFID", 1536, NULL, 1, NULL) != pdPASS) {
    Serial.println("rfid Creation Failed");
    Serial.println(esp_get_free_heap_size());
  }
}

void loop() {
  vTaskDelete(NULL);
}

void InitAll() {
  WiFinet_init();
  GPS_init();
  RFID_init();
  MQTT_init();
  QueuePrepare_init();
  CarStatus_init();
}

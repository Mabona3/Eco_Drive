#include "carStatus.h"
#include "HardwareSerial.h"
#include "mqtt.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

void FuelSensor_read(void *pvParameters) {
  while (1) {
    mqttData data;
    Serial.println("Fuel Sensor Reading");
    strcpy(data.data, "Fuel Sensor Reading");
    if (mqttQueue == NULL) {
      Serial.println("Queue is not initiallized");
    } else {
      xQueueSend(mqttQueue, &data, 0);
    }

    vTaskDelay(1000);
  }
}

void TempSensor_read(void *pvParameters) {
  while (1) {
    Serial.println("Temp Sensor Reading");
    vTaskDelay(1000);
  }
}

void RPMSensor_read(void *pvParameters) {
  while (1) {
    Serial.println("RPM Sensor Reading");
    vTaskDelay(1000);
  }
}

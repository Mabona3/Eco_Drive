#include "carStatus.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

void FuelSensor_read(void *pvParameters) {
  while (1) {
    Serial.println("Fuel Sensor Reading");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TempSensor_read(void *pvParameters) {
  while (1) {
    Serial.println("Temp Sensor Reading");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void RPMSensor_read(void *pvParameters) {
  while (1) {
    Serial.println("RPM Sensor Reading");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}



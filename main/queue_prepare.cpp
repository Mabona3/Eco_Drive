#include "queue_prepare.h"
#include "esp32-hal.h"
#include "freertos/projdefs.h"

QueueHandle_t mqttQueue;
EcoDriveData ecoDriveData;
SemaphoreHandle_t EcoDriveMutex;

void QueuePrepare_init() {
  mqttQueue = xQueueCreate(QUEUE_MAX, (UBaseType_t)sizeof(queueData));
  EcoDriveMutex = xSemaphoreCreateMutex();
}

void QueuePrepare_task(void *pvParameters) {
  EcoDriveData snapshot;
  queueData data;
  for (;;) {
    if (xSemaphoreTake(EcoDriveMutex, 0) == pdTRUE) {
      snapshot = ecoDriveData;
      xSemaphoreGive(EcoDriveMutex);
      if (snapshot.gps_valid || snapshot.obd_valid) {
        snprintf(data.data, MQTT_MSG_MAX_LEN,
                 "{\"rpm\": %f, \"speed\": %d, \"coolant_temp\": %d, "
                 "\"load\": %d, \"fuel_level\": %d, \"throttle\": %d, \""
                 "intake_temp\": %d, \"maf\": %f \"lng\": %lf, \"lat"
                 "\": %lf \"time\": \"%d/%d/%d %d:%d:%d\"}",
                 snapshot.obd_data.engineRPM, snapshot.obd_data.vehicleSpeed,
                 snapshot.obd_data.coolantTemp, snapshot.obd_data.engineLoad,
                 snapshot.obd_data.fuelLevel, snapshot.obd_data.throttlePos,
                 snapshot.obd_data.intakeTemp, snapshot.obd_data.mafRate,
                 snapshot.gps_lng, snapshot.gps_lat, snapshot.date.day,
                 snapshot.date.month, snapshot.date.year, snapshot.time.hour,
                 snapshot.time.minute, snapshot.time.second);

        if (xQueueSendToBack(mqttQueue, &data, 0) != pdPASS) {
          queueData dummy;
          xQueueReceive(mqttQueue, &dummy, 0);
          xQueueSendToBack(mqttQueue, &data, 0);
        }
      } else {
        Serial.println("No Valid Data");
      }
    } else {
      Serial.println("Prepare is waiting for the mutex");
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

#include "queue_prepare.h"
#include "esp32-hal.h"

static QueueHandle_t mqttQueue;
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
    if (xSemaphoreTake(EcoDriveMutex, 0)) {
      snapshot = ecoDriveData;
      xSemaphoreGive(EcoDriveMutex);
      if (snapshot.gps_valid || snapshot.obd_valid) {
        snprintf(data.data, MQTT_MSG_MAX_LEN, "{}");
        xQueueSend(mqttQueue, &data, 100);
      } else {
        Serial.println("No Valid Data");
      }
    } else {
      Serial.println("Prepare is waiting for the mutex");
    }
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

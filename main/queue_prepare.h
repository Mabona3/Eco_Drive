#ifndef PREPARE_QUEUE_H
#define PREPARE_QUEUE_H

#include <esp32-hal.h>

#include "carStatus.h"
#include "constant.h"

// "{"rpm": 6667.00, "speed": 300, "coolant_temp": 185, "load": 628,
// "fuel_level": 100, "throttle": 114, "intake_temp": 157, "maf": 7.00
// "lng": 90.123456, "lat": 90.123456}"

// A struct to hold data to be uploaded to the queueHandle.
typedef struct {
  char data[MQTT_MSG_MAX_LEN];
} queueData;

typedef struct {
  // gps data
  double gps_lng, gps_lat;
  bool gps_valid;

  // OBD Data
  OBDData obd_data;
  // Valid if connected only can't validate the car data it will be.
  bool obd_valid;

} EcoDriveData;

extern EcoDriveData ecoDriveData;
extern SemaphoreHandle_t EcoDriveMutex;

void QueuePrepare_init();
void QueuePrepare_task(void *pvParameters);

#endif // !PREPARE_QUEUE_H

#ifndef PREPARE_QUEUE_H
#define PREPARE_QUEUE_H

#include <esp32-hal.h>

#include "TinyGPS++.h"
#include "carStatus.h"
#include "constant.h"

// "{"rpm": 6667.00, "speed": 300, "coolant_temp": 185, "load": 628,
// "fuel_level": 100, "throttle": 114, "intake_temp": 157, "maf": 7.00
// "lng": 90.123456, "lat": 90.123456 "time": "10/10/2010 12:59:59"}"
//

struct Date {
  int year;
  int month;
  int day;
};

struct Time {
  int hour;
  int minute;
  int second;
};

// A struct to hold data to be uploaded to the queueHandle.
typedef struct {
  char data[MQTT_MSG_MAX_LEN];
} queueData;

typedef struct {
  // gps data
  double gps_lng, gps_lat;
  bool gps_valid;

  struct Date date;
  struct Time time;

  // OBD Data
  OBDData obd_data;
  // Valid if connected only can't validate the car data it will be.
  bool obd_valid;

} EcoDriveData;

extern EcoDriveData ecoDriveData;
extern SemaphoreHandle_t EcoDriveMutex;
extern QueueHandle_t mqttQueue;

void QueuePrepare_init();
void QueuePrepare_task(void *pvParameters);

#endif // !PREPARE_QUEUE_H

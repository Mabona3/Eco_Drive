#ifndef CARSTATUS_H
#define CARSTATUS_H

#include "BLEAdvertisedDevice.h"
#include "BLEDevice.h"
#include "BLEScan.h"
#include "BLEUtils.h"

// Connection status
extern bool deviceConnected = false;

// OBD data storage
typedef struct {
  float engineRPM;
  int vehicleSpeed;
  int coolantTemp;
  int engineLoad;
  int fuelLevel;
  int throttlePos;
  int intakeTemp;
  float mafRate;
  unsigned long lastUpdateTime;
} OBDData;

void CarStatus_init();
void CarStatus_task(void *);

#endif // CARSTATUS_H

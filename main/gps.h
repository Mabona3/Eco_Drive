#ifndef GPS_H
#define GPS_H

#include "constant.h"
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

static HardwareSerial GPS_Serial(1);
static TinyGPSPlus gps;

void GPS_init();
void GPS_read(void *pvParameters);
void processData();

#endif

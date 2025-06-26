#ifndef GPS_H
#define GPS_H

#include "constant.h"
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

void GPS_init();
void GPS_read(void *pvParameters);
void processData();

#endif

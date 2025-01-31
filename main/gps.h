#ifndef GPS_H
#define GPS_H

#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include "firebase.h"
#include "constant.h"

typedef struct {
  int month;
  int day;
  int year;
} GPSdate;

typedef struct {
  int hour;
  int minute;
  int second;
  int centisecond; 
} GPStime;

typedef struct {
  double lat;
  double lng;
  GPSdate gps_date;
  GPStime gps_time;
} GPSdata;

static HardwareSerial GPS_Serial(1);
static TinyGPSPlus gps;
static GPSdata gps_data;

void GPSInit();

void GPS_read(void *pvParameters);

void displayInfo();

#endif

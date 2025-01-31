#include "gps.h"
#include "firebase.h"

void GPSInit() {
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_PIN_RX, GPS_PIN_TX);
}

void GPS_read(void *pvParameters) {
  while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read());
  }
  displayInfo();
  delay(1000);
}

void displayInfo() {
  gps_data.lat = gps.location.lat();
  gps_data.lng = gps.location.lng();
  gps_data.gps_date.month = gps.date.month();
  gps_data.gps_date.day = gps.date.day();
  gps_data.gps_date.year = gps.date.year();
  gps_data.gps_time.hour = gps.time.hour();
  gps_data.gps_time.minute = gps.time.minute();
  gps_data.gps_time.second = gps.time.second();
  gps_data.gps_time.centisecond = gps.time.centisecond();

  if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid() && gps.date.day() != 0)
    FirebaseGPS();
  else
    Serial.println("GPS data is not sent");

  Serial.print(F("Location: ")); 
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}

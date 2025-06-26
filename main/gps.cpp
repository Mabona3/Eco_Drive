#include "gps.h"
#include "HardwareSerial.h"
#include <freertos/FreeRTOS.h>

void GPS_init() { GPS_Serial.begin(9600, SERIAL_8N1, GPS_PIN_RX, GPS_PIN_TX); }

void GPS_read(void *pvParameters) {
  for (;;) {
    Serial.println("Searching GPS...");
    while (GPS_Serial.available()) {
      gps.encode(GPS_Serial.read());
    }
    processData();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void displayInfo();
void uploadData() {}

void processData() {
  displayInfo();
  uploadData();
}

void displayInfo() {
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
    if (gps.time.hour() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}

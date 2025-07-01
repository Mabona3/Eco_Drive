#include "gps.h"
#include "mqtt.h"
#include "queue_prepare.h"
#include <HardwareSerial.h>
#include <esp32-hal.h>
#include <freertos/FreeRTOS.h>

static HardwareSerial GPS_Serial(1);
static TinyGPSPlus gps;

// processing data.
void processData();
void displayInfo();

void GPS_init() { GPS_Serial.begin(9600, SERIAL_8N1, GPS_PIN_RX, GPS_PIN_TX); }

void GPS_read(void *pvParameters) {
  for (;;) {
    while (GPS_Serial.available()) {
      gps.encode(GPS_Serial.read());
    }
    processData();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void uploadData() {
  if (gps.location.isValid()) {
  }
  if (xSemaphoreTake(EcoDriveMutex, 100)) {
    ecoDriveData.gps_valid = gps.location.isValid();
    ecoDriveData.gps_lng = gps.location.lng();
    ecoDriveData.gps_lat = gps.location.lat();
    xSemaphoreGive(EcoDriveMutex);
  }
}

void processData() {
  uploadData();
  if (Serial)
    displayInfo();
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

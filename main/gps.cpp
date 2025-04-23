#include <freertos/FreeRTOS.h>
#include "gps.h"
#include "firebase.h"

void GPS_init() {
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_PIN_RX, GPS_PIN_TX);
}

void GPS_read(void *pvParameters) {
  for (;;) {
    // This is also non blocking function it just a wrapper
    // around uart_get_buff_len which get the length of 
    // uart buffered data.
    if (GPS_Serial.available()) {
      char c = GPS_Serial.read();
      if (gps.encode(c)) {
        processData();
      }
    }
    vTaskDelay(1000);
  }
}

void processData() {
  bool valid = gps.location.isValid();
  displayInfo();

  if (valid && gps.date.day() != 0) {
    Serial.println("Sending data to the firebase.");
    FirebaseJson locationData;
    locationData.add("lat", gps.location.lat());
    locationData.add("lng", gps.location.lng());

    FirebaseJson timeData;
    timeData.add("hour", gps.time.hour());
    timeData.add("minute", gps.time.minute());
    timeData.add("second", gps.time.second());
    timeData.add("centisecond", gps.time.centisecond());

    FirebaseJson dateData;
    dateData.add("month", gps.date.month());
    dateData.add("day", gps.date.day());
    dateData.add("year", gps.date.year());
    FirebaseJson json;
    json.add("location", locationData);
    json.add("time", timeData);
    json.add("date", dateData);
    message msg = {&json, "/gps"};
    xQueueSend(queue, &msg, 1000);
  }
  else {
    Serial.println("GPS data is not sent");
  }
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

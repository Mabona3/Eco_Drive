#ifndef WIFINET_H
#define WIFINET_H

#include <WiFi.h>
#include "constant.h"

void WiFiInit() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println("Connected");

}

#endif // WIFINET_H

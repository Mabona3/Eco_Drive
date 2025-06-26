#ifndef MQTT_H
#define MQTT_H

#include "constant.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

static WiFiClientSecure esp_client;
static PubSubClient mqtt_client(esp_client);

void MQTT_init();
void MQTT_close();

#endif // MQTT_H

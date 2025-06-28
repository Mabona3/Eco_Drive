#ifndef MQTT_H
#define MQTT_H

#include "constant.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

void MQTT_init();
void MQTT_task(void *pvParameters);

#endif // MQTT_H

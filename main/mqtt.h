#ifndef MQTT_H
#define MQTT_H

#include "constant.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

typedef struct {
  char data[64];
} mqttData;

extern QueueHandle_t mqttQueue;

void MQTT_init();
void MQTT_close();
void MQTT_task(void *pvParams);
void MQTT_changeTopic();

#endif // MQTT_H

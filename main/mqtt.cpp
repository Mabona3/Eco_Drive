#include <HardwareSerial.h>
#include <cstdio>
#include <esp32-hal.h>
#include <esp_random.h>
#include <freertos/FreeRTOS.h>

#include "mqtt.h"
#include "queue_prepare.h"
#include "rfid.h"
#include "wifinet.h"

static WiFiClientSecure esp_client;
static PubSubClient mqtt_client;
static char mqtt_topic[24];

// private function.
void _MQTT_connect();

void MQTT_reconnect() {
  if (RFID_changed) {
    int uidLength = rfid.uid.size;
    char id[21];
    id[(uidLength << 1)] = '\0';
    for (int i = 0; i < uidLength; i++) {
      sprintf(&id[i << 1], "%02X", rfid.uid.uidByte[i]);
    }
    snprintf(mqtt_topic, 24, "client-%s", id);
  } else {
    snprintf(mqtt_topic, 24, "anonymous-%lX", esp_random());
  }
  Serial.print("The topic is ");
  Serial.println(mqtt_topic);
  if (mqtt_client.connected()) {
    mqtt_client.disconnect();
    esp_client.stop();
    delay(200);
  }

  _MQTT_connect();
}

void MQTT_init() { MQTT_reconnect(); }

void _MQTT_connect() {
  esp_client.setInsecure();
  mqtt_client.setServer(MQTT_URL, MQTT_PORT);
  mqtt_client.setClient(esp_client);
  char topicStatus[45];
  snprintf(topicStatus, 45, "vehicles/%s/status", mqtt_topic);
  while (!mqtt_client.connected()) {
    Serial.printf("Connecting to MQTT Broker as %s, with topicStatus: %s...\n",
                  mqtt_topic, topicStatus);

    if (mqtt_client.connect(mqtt_topic, MQTT_USERNAME, MQTT_PASSWORD,
                            topicStatus, 0, 0, "{\"status\": \"offline\"}")) {

      Serial.println("Connected to MQTT broker");
      mqtt_client.publish(topicStatus, "{\"status\": \"online\"}");
    } else {
      Serial.printf(
          "Failed to connect to MQTT broker, rc=%d Retrying in 1 sec.\n",
          mqtt_client.state());
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}

// MQTT Task collect the data in a queue and then publish it.
void MQTT_task(void *pvParameters) {
  char topic_telemetry[45];
  snprintf(topic_telemetry, sizeof(topic_telemetry), "vehicles/%s/telementry",
           mqtt_topic);
  Serial.println("Created");
  queueData msg;
  for (;;) {
    if (RFID_changed) {
      MQTT_reconnect();
      snprintf(topic_telemetry, sizeof(topic_telemetry),
               "vehicles/%s/telementry", mqtt_topic);
    }
    if (mqtt_client.connected()) {
      Serial.println(topic_telemetry);
      if (xQueueReceive(mqttQueue, &msg, 3000 / portTICK_PERIOD_MS)) {

        mqtt_client.publish(String(topic_telemetry).c_str(), msg.data);
      } else {
        Serial.println("No Messages");
      }
    }
  }
}

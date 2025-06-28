#include <HardwareSerial.h>
#include <esp32-hal.h>
#include <esp_random.h>
#include <freertos/FreeRTOS.h>

#include "mqtt.h"
#include "queue_prepare.h"
#include "rfid.h"

static WiFiClientSecure esp_client;
static PubSubClient mqtt_client;
static String mqtt_topic;
QueueHandle_t mqttQueue = NULL;

// private function.
void _MQTT_connect();

void MQTT_reconnect() {
  if (rfid.uid.size != 0) {
    String id;
    for (int i = 0; i < rfid.uid.size; i++) {
      id += String(rfid.uid.uidByte[i], HEX);
    }
    mqtt_topic = "client-" + id;
  } else {
    mqtt_topic = "anonymous-" + String(esp_random(), HEX);
  }
  Serial.print("The topic is ");
  Serial.println(mqtt_topic);
  if (mqtt_client.connected()) {
    mqtt_client.disconnect();
    delay(200);
  }

  _MQTT_connect();
}

void MQTT_init() { MQTT_reconnect(); }

void _MQTT_connect() {
  esp_client.setInsecure();
  mqtt_client.setServer(MQTT_URL, MQTT_PORT);
  mqtt_client.setKeepAlive(60);
  mqtt_client.setClient(esp_client);
  String topicStatus = String("vehicles/" + mqtt_topic + "/status");
  while (!mqtt_client.connected()) {
    Serial.printf("Connecting to MQTT Broker as %s, with topicStatus: %s...\n",
                  mqtt_topic.c_str(), topicStatus.c_str());

    if (mqtt_client.connect(mqtt_topic.c_str(), MQTT_USERNAME, MQTT_PASSWORD,
                            topicStatus.c_str(), 0, 0,
                            "{\"status\": \"offline\"}")) {

      Serial.println("Connected to MQTT broker");
      mqtt_client.publish(topicStatus.c_str(), "{\"status\": \"online\"}");
    } else {
      Serial.printf(
          "Failed to connect to MQTT broker, rc=%d Retrying in 1 sec.",
          mqtt_client.state());
      delay(1000);
    }
  }
}

// MQTT Task collect the data in a queue and then publish it.
void MQTT_task(void *pvParameters) {
  queueData msg;
  for (;;) {
    if (RFID_changed) {
      RFID_changed = false;
      MQTT_reconnect();
    }
    if (mqtt_client.connected()) {
      if (xQueueReceive(mqttQueue, &msg, 200 / portTICK_PERIOD_MS)) {
        mqtt_client.publish(
            String("vehicles/" + mqtt_topic + "/telementry").c_str(), msg.data);
        mqtt_client.loop();
      } else {
        Serial.println("No Messages");
      }
    }
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

#include "mqtt.h"
#include "esp32-hal.h"
#include "rfid.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <esp_random.h>
#include <freertos/FreeRTOS.h>

static WiFiClientSecure esp_client;
static PubSubClient mqtt_client(esp_client);
static String mqtt_topic;
QueueHandle_t mqttQueue = NULL;

void MQTT_changeTopic() {
  if (RFID_anonymous) {
    String id;
    for (int i = 0; i < rfid.uid.size; i++) {
      id += String(rfid.uid.uidByte[i], HEX);
    }
    mqtt_topic = "vehicles/client-" + id + "/status";
  } else {
    mqtt_topic = "vehicles/anonymous-" + String(esp_random(), HEX) + "/status";
  }
  Serial.print("The topic is ");
  Serial.println(mqtt_topic);
}

void MQTT_init() {
  esp_client.setInsecure();
  mqtt_client.setServer(MQTT_URL, MQTT_PORT);
  mqtt_client.setKeepAlive(60);
  mqttQueue = xQueueCreate(QUEUE_MAX, sizeof(mqttData));
  MQTT_changeTopic();

  String clientId = "esp-client-" + String(esp_random(), HEX);
  while (!mqtt_client.connected()) {
    Serial.printf("Connecting to MQTT Broker as %s...\n", clientId.c_str());

    if (mqtt_client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(mqtt_client.state());
      Serial.println("Retrying in 1 seconds.");
      delay(1000);
    }
  }
}

// MQTT Task collect the data in a queue and then publish it.
void MQTT_task(void *pvParams) {
  mqttData msg;
  for (;;) {
    if (mqtt_client.connected()) {
      if (xQueueReceive(mqttQueue, &msg, 0)) {
        Serial.print(mqtt_topic);
        Serial.print(": ");
        Serial.println(msg.data);
        // mqtt_client.publish(topic, msg.data);
      }
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

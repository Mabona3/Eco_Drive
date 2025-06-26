#include "mqtt.h"
#include "rfid.h"
#include <HardwareSerial.h>
#include <esp_random.h>

void MQTT_init() {
  esp_client.setInsecure();
  mqtt_client.setServer(MQTT_URL, MQTT_PORT);
  mqtt_client.setKeepAlive(60);

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

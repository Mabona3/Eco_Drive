#include "firebase.h"
#include "Arduino.h"
#include "constant.h"
#include "gps.h"
#include <FirebaseESP32.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>

int Firebase_init() {
  /* Assigning Credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  Firebase.reconnectNetwork(true);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  queue = xQueueCreate(QUEUE_MAX, sizeof(message));

  if (queue == NULL) {
    Serial.println("Failed to create queue");
    return 1;
  }
}

void Firebase_task(void *pvParameters) {
  message msg;
  for (;;) {
    if (xQueueReceive(queue, &msg, 0)) {
      Serial.printf("Received message: %s\n", msg.url);
      // Checked it only returns the checking of the authentication of the
      // firebase It is actually a boolean value in a struct that returns auth
      // readiness The docs says: `This function should be called repeatedly
      // to handle authentication tasks.` Which means this function should be
      // in a loop i.e. no blocking
      if (Firebase.ready()) {
        Firebase.pushJSON(fbdo, msg.url, *msg.json);
        Serial.println("Message sent");
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

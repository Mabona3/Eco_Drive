#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "gps.h"
#include "firebase.h"
#include "constant.h"

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
    if (xQueueReceive(queue, &msg, 1000)) {
      Serial.printf("Received message: %s\n", msg.url);
      if (Firebase.ready()) {
        Firebase.pushJSON(fbdo, msg.url, *msg.json);
        Serial.println("Message sent");
      }
    }
  }
}

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "gps.h"
#include "firebase.h"

int FirebaseInit() {
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
}

void FirebaseGPS() {
  if (Firebase.ready()) {
    FirebaseJson locationData;
    locationData.add("lat", gps_data.lat);
    locationData.add("lng", gps_data.lng);


    FirebaseJson timeData;
    timeData.add("hour", gps_data.gps_time.hour);
    timeData.add("minute", gps_data.gps_time.minute);
    timeData.add("second", gps_data.gps_time.second);
    timeData.add("centisecond", gps_data.gps_time.centisecond);

    FirebaseJson dateData;
    dateData.add("month", gps_data.gps_date.month);
    dateData.add("day", gps_data.gps_date.day);
    dateData.add("year", gps_data.gps_date.year);

    FirebaseJson json;
    json.add("location", locationData);
    json.add("time", timeData);
    json.add("date", dateData);

    Firebase.pushJSON(fbdo, "/gps", json);

  }
}

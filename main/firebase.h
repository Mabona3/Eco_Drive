#ifndef FIREBASE_H
#define FIREBASE_H

#include <FirebaseESP32.h>

#include "constant.h"

static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;
static QueueHandle_t queue;


typedef struct {
  FirebaseJson *json;
  const char *url;
} message;

int Firebase_init();
void Firebase_task(void *pvParameters);

#endif // FIREBASE_H

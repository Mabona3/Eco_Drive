#ifndef FIREBASE_H
#define FIREBASE_H

#include <FirebaseESP32.h>

#include "constant.h"

static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;

int Firebase_init();
void FirebaseGPS();

#endif // FIREBASE_H

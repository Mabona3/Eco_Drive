#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include "constant.h"

static MFRC522DriverPinSimple ssPin(RFID_PIN_SS);
static MFRC522DriverSPI spiDriver{ssPin};
static MFRC522 rfid{spiDriver};

void RFID_init();
void RFID_read(void *pvParameters);

#endif // RFID_H

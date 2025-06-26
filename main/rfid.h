#ifndef RFID_H
#define RFID_H

#include "constant.h"
#include <Arduino.h>
#include <MFRC522Debug.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522v2.h>
#include <SPI.h>

static MFRC522DriverPinSimple ssPin(RFID_PIN_SS);
static SPIClass &spiClass = SPI;
static const SPISettings spiSettings =
    SPISettings(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0);
static MFRC522DriverSPI spiDriver{ssPin, spiClass, spiSettings};
static MFRC522 rfid{spiDriver};

void RFID_init();
void RFID_read(void *pvParameters);

#endif // RFID_H

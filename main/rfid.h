#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <freertos/semphr.h>
#include "constant.h"

static MFRC522DriverPinSimple ssPin(RFID_PIN_SS);
static MFRC522DriverSPI spiDriver{ssPin};
static MFRC522 rfid{spiDriver};
static volatile bool RFID_detected = false;
static SemaphoreHandle_t xSemaphore = NULL;


void ARDUINO_ISR_ATTR RFID_isr(void);
void RFID_init();
void RFID_read(void *pvParameters);

#endif // RFID_H

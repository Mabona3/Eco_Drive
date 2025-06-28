#ifndef RFID_H
#define RFID_H

#include "constant.h"
#include <MFRC522Debug.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522v2.h>
#include <SPI.h>

extern MFRC522 rfid;
extern bool RFID_changed;

void RFID_init();
void RFID_read(void *pvParameters);

#endif // RFID_H

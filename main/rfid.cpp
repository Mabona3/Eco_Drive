#include "rfid.h"

/* Private functions */
void printHex(byte *buffer, byte bufferSize);

/* Public functions */
void RFID_init() {
  SPI.begin();
  rfid.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(rfid, Serial);	// Show details of PCD - MFRC522 Card Reader details.
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void RFID_read(void *pvParameters) {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards.
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called.
  MFRC522Debug::PICC_DumpToSerial(rfid, Serial, &(rfid.uid));

  delay(2000);
}

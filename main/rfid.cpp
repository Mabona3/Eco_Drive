#include "rfid.h"

void RFID_init() {
  SPI.begin();
  rfid.PCD_Init();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  xSemaphore = xSemaphoreCreateBinary();
}

void RFID_read(void *pvParameters) {
  while (1) {
    Serial.println("Waiting for RFID");
    if (xSemaphoreTake(xSemaphore, 1000) == pdTRUE) {
      Serial.println("RFID detected");
      RFID_detected = false;

      if (rfid.PICC_IsNewCardPresent() || rfid.PICC_ReadCardSerial()) {
        Serial.print("Card UID: ");
        for (byte i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
      }
    }
  }
}

void ARDUINO_ISR_ATTR RFID_isr(void) {
  RFID_detected = true;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

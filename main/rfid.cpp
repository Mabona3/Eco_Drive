#include "rfid.h"

/* Public functions */
void RFID_init() {
  SPI.begin();
  rfid.PCD_Init();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  xSemaphore = xSemaphoreCreateBinary();
}

void RFID_read(void *pvParameters) {
  while (1) {
    // Take the semaphore signal from the isr
    Serial.println("Waiting for RFID");
    if (xSemaphoreTake(xSemaphore, 1000) == pdTRUE) {
      Serial.println("RFID detected");
      RFID_detected = false;

      // check if new Card is put
      if (rfid.PICC_IsNewCardPresent() || rfid.PICC_ReadCardSerial()) {
        Serial.print("Card UID: ");
        for (byte i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        // TODO: What will happen with the RFID data
        // The data will be sent to the backend or the firebase
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

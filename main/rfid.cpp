#include "rfid.h"
#include "HardwareSerial.h"

bool rfid_connected = false;

void RFID_init() {
  SPI.begin();
  rfid.PCD_Init();
}

void RFID_read(void *pvParameters) {
  for (;;) {
    // Removed the interrupts and semaphore polling this is the simple way of
    // doing this however this way sends and recieves via SPI which makes a
    // delay of 1ms~2ms
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      Serial.println("RFID detected");
      Serial.print("Card UID: ");
      Serial.println(rfid.uid.size);
      Serial.print("\"");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
      }
      Serial.println("\"");
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

#include "rfid.h"
#include "mqtt.h"
#include <HardwareSerial.h>

static MFRC522DriverPinSimple ssPin(RFID_PIN_SS);
static SPIClass &spiClass = SPI;
static const SPISettings spiSettings =
    SPISettings(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0);
static MFRC522DriverSPI spiDriver{ssPin, spiClass, spiSettings};

MFRC522 rfid{spiDriver};

bool RFID_anonymous = false;

void RFID_init() {
  SPI.begin();
  rfid.PCD_Init();
}

void RFID_read(void *pvParameters) {
  for (;;) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      Serial.println("RFID detected");
      RFID_anonymous = true;
      MQTT_changeTopic();
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

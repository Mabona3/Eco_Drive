#include "carStatus.h"
#include "NimBLEScan.h"
#include "queue_prepare.h"

#include <NimBLEDevice.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

const char *ELM_ADDRESS = "00:10:cc:4f:36:03";
const char *ELM_NAME = "ELM327";

static NimBLEScan *pBLEScan;
static NimBLEClient *pClient;
static NimBLERemoteService *pRemoteService;
static NimBLERemoteCharacteristic *pRemoteCharacteristic;
static NimBLEAdvertisedDevice *myDevice = nullptr;

static volatile bool deviceConnected = false;
static volatile bool doConnect = false;

#define RESPONSE_BUF_SIZE 128
static char responseData[RESPONSE_BUF_SIZE];
static volatile bool responseReady = false;
static bool expectingResponse = false;
static SemaphoreHandle_t xResponseSemaphore = NULL;
static SemaphoreHandle_t xResponseMutex = NULL;

const char *PID_LIST[] = {"010C", "010D", "0105", "0104",
                          "012F", "0111", "010F", "0110"};
const int NUM_PIDS = sizeof(PID_LIST) / sizeof(PID_LIST[0]);

OBDData obdData;

const int DATA_REQUEST_INTERVAL_MS = 100;
int currentPidIndex = 0;

int hexToInt(const char *hex);
bool sendPIDRequest(const char *pid);
void obd_module_setup();
bool connectToELM327();
bool sendCommand(const char *command, uint32_t timeout = 1000);
void processPIDResponse(const char *pid, const char *response);
static void notifyCallback(NimBLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData, size_t length, bool isNotify);

class MyAdvertisedDeviceCallbacks : public NimBLEScanCallbacks {
  void onResult(NimBLEAdvertisedDevice *advertisedDevice) {
    if (advertisedDevice->getAddress().toString() == ELM_ADDRESS ||
        advertisedDevice->getName() == ELM_NAME) {
      if (pBLEScan->isScanning())
        pBLEScan->stop();

      if (myDevice) {
        delete myDevice;
        myDevice = nullptr;
      }

      myDevice = new NimBLEAdvertisedDevice(*advertisedDevice);
      doConnect = true;
    }
  }
};

class MyClientCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient *pclient) { deviceConnected = true; }

  void onDisconnect(NimBLEClient *pclient) { deviceConnected = false; }
};

static void notifyCallback(NimBLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData, size_t length, bool isNotify) {
  if (length == 0 || length >= RESPONSE_BUF_SIZE)
    return;

  if (xSemaphoreTake(xResponseMutex, portMAX_DELAY) == pdTRUE) {
    if (expectingResponse) {
      memcpy(responseData, pData, length);
      responseData[length] = '\0';

      char *end = responseData + length;
      while (end > responseData &&
             (*end == '\0' || *end == '>' || isspace(*end))) {
        *end-- = '\0';
      }

      responseReady = true;
      expectingResponse = false;
      xSemaphoreGive(xResponseSemaphore);
    }
    xSemaphoreGive(xResponseMutex);
  }
}

void CarStatus_init() {
  BLEDevice::init("ESP32_ELM_Scanner");
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  pBLEScan = BLEDevice::getScan();
  pBLEScan->setScanCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(0, true);
}

void CarStatus_task(void *pvParameters) {
  for (;;) {
    if (doConnect) {
      connectToELM327();
      doConnect = false;
    }

    if (deviceConnected) {
      for (int i = 0; i < NUM_PIDS; i++) {
        sendPIDRequest(PID_LIST[currentPidIndex]);
      }
    } else {
      doConnect = true;
    }
    if (xSemaphoreTake(EcoDriveMutex, portMAX_DELAY)) {
      ecoDriveData.obd_data = obdData;
      xSemaphoreGive(EcoDriveMutex);
      vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

bool connectToELM327() {
  if (!myDevice || !pClient->connect(myDevice)) {
    return false;
  }

  static const NimBLEUUID serviceUUIDs[] = {
      NimBLEUUID("0000fff0-0000-1000-8000-00805f9b34fb"),
      NimBLEUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e")};

  for (const auto &uuid : serviceUUIDs) {
    if ((pRemoteService = pClient->getService(uuid)))
      break;
  }
  if (!pRemoteService)
    return false;

  static const NimBLEUUID charUUIDs[] = {
      NimBLEUUID("0000fff1-0000-1000-8000-00805f9b34fb"),
      NimBLEUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e")};

  for (const auto &uuid : charUUIDs) {
    if ((pRemoteCharacteristic = pRemoteService->getCharacteristic(uuid)))
      break;
  }
  if (!pRemoteCharacteristic)
    return false;

  if (!pRemoteCharacteristic->subscribe(true, notifyCallback)) {
    return false;
  }

  return sendCommand("ATZ", 3000) && sendCommand("ATE0") &&
         sendCommand("ATL0") && sendCommand("ATSP0");
}

bool sendCommand(const char *command, uint32_t timeout) {
  if (!deviceConnected || !pRemoteCharacteristic)
    return false;

  if (xSemaphoreTake(xResponseMutex, portMAX_DELAY) == pdTRUE) {
    expectingResponse = true;
    responseReady = false;
    responseData[0] = '\0';
    xSemaphoreGive(xResponseMutex);
  }

  String cmdWithCR = String(command) + "\r";
  pRemoteCharacteristic->writeValue((uint8_t *)cmdWithCR.c_str(),
                                    cmdWithCR.length(), false);

  bool success = false;
  if (xSemaphoreTake(xResponseSemaphore, pdMS_TO_TICKS(timeout))) {
    if (xSemaphoreTake(xResponseMutex, portMAX_DELAY) == pdTRUE) {
      success = responseReady;
      xSemaphoreGive(xResponseMutex);
    }
  }

  if (!success && xSemaphoreTake(xResponseMutex, portMAX_DELAY) == pdTRUE) {
    expectingResponse = false;
    xSemaphoreGive(xResponseMutex);
  }

  return success;
}

bool sendPIDRequest(const char *pid) {
  if (sendCommand(pid, 200)) {
    processPIDResponse(pid, responseData);
    return true;
  }
  return false;
}

void processPIDResponse(const char *pid, const char *response) {
  char expectedHeader[7];
  snprintf(expectedHeader, sizeof(expectedHeader), "41 %c%c", pid[0], pid[1]);
  const char *p = strstr(response, expectedHeader);
  if (!p)
    return;
  p += 5;
  while (*p == ' ')
    p++;
  size_t remaining = strlen(p);
  if (strcmp(pid, "010C") == 0) { // Engine RPM
    if (remaining >= 5) {         // Need 2 data bytes (4 chars + separator)
      int a = hexToInt(p);
      int b = hexToInt(p + 3);
      obdData.engineRPM = (a * 256.0 + b) / 4.0;
    }
  } else if (strcmp(pid, "010D") == 0) { // Vehicle Speed
    if (remaining >= 2) {
      obdData.vehicleSpeed = hexToInt(p);
    }
  } else if (strcmp(pid, "0105") == 0) { // Coolant Temperature
    if (remaining >= 2) {
      obdData.coolantTemp = hexToInt(p) - 40;
    }
  } else if (strcmp(pid, "0104") == 0) { // Engine Load
    if (remaining >= 2) {
      int value = hexToInt(p);
      obdData.engineLoad = (value * 100) / 255;
    }
  } else if (strcmp(pid, "012F") == 0) { // Fuel Level
    if (remaining >= 2) {
      int value = hexToInt(p);
      obdData.fuelLevel = (value * 100) / 255;
    }
  } else if (strcmp(pid, "0111") == 0) { // Throttle Position
    if (remaining >= 2) {
      int value = hexToInt(p);
      obdData.throttlePos = (value * 100) / 255;
    }
  } else if (strcmp(pid, "010F") == 0) { // Intake Air Temperature
    if (remaining >= 2) {
      obdData.intakeTemp = hexToInt(p) - 40;
    }
  } else if (strcmp(pid, "0110") == 0) { // MAF Rate
    if (remaining >= 5) { // Need 2 data bytes (4 chars + separator)
      int a = hexToInt(p);
      int b = hexToInt(p + 3);
      obdData.mafRate = (a * 256.0 + b) / 100.0;
    }
  }
}

int hexToInt(const char *hex) {
  char buf[3] = {hex[0], hex[1], '\0'};
  return (int)strtol(buf, NULL, 16);
}

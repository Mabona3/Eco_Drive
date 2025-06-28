#include "carStatus.h"
#include "queue_prepare.h"

BLEClient *pClient;
BLEAdvertisedDevice *myDevice;

void startScanning();
bool connectToELM327();
void requestNextPID();
bool sendCommand(String command);
void processResponse(const char *pid, String response);

// ELM327 device settings
#define ELM_ADDRESS "00:10:cc:4f:36:03" // Your ELM327 BLE MAC address
#define ELM_NAME "ELM327"               // Alternative: scan by name

// Connection status
bool deviceConnected = false;
bool doConnect = false;
bool doScan = true;

// Response handling
String responseData = "";
bool responseReady = false;

// OBD-II PIDs
const char *PID_LIST[] = {
    "010C", // Engine RPM
    "010D", // Vehicle speed
    "0105", // Engine coolant temperature
    "0104", // Calculated engine load
    "012F", // Fuel level input
    "0111", // Throttle position
    "010F", // Intake air temperature
    "0110"  // MAF air flow rate
};
const int NUM_PIDS = 8;
int currentPidIndex = 0;

OBDData obdData;

// Callbacks
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().toString() == ELM_ADDRESS ||
        advertisedDevice.getName() == ELM_NAME) {
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = false;
      advertisedDevice.getScan()->stop();
    }
  }
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) { deviceConnected = true; }

  void onDisconnect(BLEClient *pclient) {
    deviceConnected = false;
    doScan = true;
  }
};

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                           uint8_t *pData, size_t length, bool isNotify) {
  for (int i = 0; i < length; i++) {
    responseData += (char)pData[i];
  }
  if (responseData.indexOf('>') >= 0) {
    responseReady = true;
  }
}

void CarStatus_init() {
  BLEDevice::init("ESP32_OBD_Reader");
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
  startScanning();
}

void CarStatus_task(void *pvParameters) {
  // Handle connection
  for (;;) {
    if (doConnect) {
      if (connectToELM327()) {
        doConnect = false;
      }
    }

    // Auto-reconnect if disconnected
    if (!deviceConnected && doScan) {
      startScanning();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    for (int i = 0; i < NUM_PIDS; i++) {
      if (deviceConnected)
        requestNextPID();
      else
        break;
    }

    if (xSemaphoreTake(ecoDriveMutex, 100)) {
      ecoDriveData.obd_data = obdData;
      xSemaphoreGive(ecoDriveMutex);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void startScanning() {
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5);
}

bool connectToELM327() {
  if (!pClient->connect(myDevice))
    return false;

  BLERemoteService *pRemoteService =
      pClient->getService(BLEUUID("0000fff0-0000-1000-8000-00805f9b34fb"));
  if (!pRemoteService)
    return false;

  BLERemoteCharacteristic *pRemoteCharacteristic =
      pRemoteService->getCharacteristic(
          BLEUUID("0000fff1-0000-1000-8000-00805f9b34fb"));
  if (!pRemoteCharacteristic)
    return false;

  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  } else {
    return false;
  }

  // Initialize ELM327
  sendCommand("ATZ");
  delay(1000);
  sendCommand("ATE0");
  sendCommand("ATL0");
  sendCommand("ATSP0");

  return true;
}

bool sendCommand(String command) {
  responseData = "";
  responseReady = false;

  BLERemoteService *pRemoteService =
      pClient->getService(BLEUUID("0000fff0-0000-1000-8000-00805f9b34fb"));
  BLERemoteCharacteristic *pRemoteCharacteristic =
      pRemoteService->getCharacteristic(
          BLEUUID("0000fff1-0000-1000-8000-00805f9b34fb"));

  String cmdWithCR = command + "\r";
  pRemoteCharacteristic->writeValue(cmdWithCR.c_str(), cmdWithCR.length());

  unsigned long startTime = millis();
  while (!responseReady && (millis() - startTime < 2000)) {
    delay(10);
  }
  return responseReady;
}

void requestNextPID() {
  const char *pid = PID_LIST[currentPidIndex];
  if (sendCommand(pid)) {
    processResponse(pid, responseData);
  }
  currentPidIndex = (currentPidIndex + 1) % NUM_PIDS;
}

void processResponse(const char *pid, String response) {
  obdData.lastUpdateTime = millis();
  int index = response.indexOf("41 " + String(pid).substring(2));
  if (index < 0)
    return;

  String hexValue = response.substring(index + 6, index + 8);
  int value = (int)strtol(hexValue.c_str(), NULL, 16);

  if (strcmp(pid, "010C") == 0) { // RPM
    obdData.engineRPM = value * 25.5;
  } else if (strcmp(pid, "010D") == 0) { // Speed
    obdData.vehicleSpeed = value;
  } else if (strcmp(pid, "0105") == 0) { // Coolant temp
    obdData.coolantTemp = value - 40;
  } else if (strcmp(pid, "0104") == 0) { // Engine load
    obdData.engineLoad = (value * 100) / 255;
  } else if (strcmp(pid, "012F") == 0) { // Fuel level
    obdData.fuelLevel = (value * 100) / 255;
  } else if (strcmp(pid, "0111") == 0) { // Throttle position
    obdData.throttlePos = (value * 100) / 255;
  } else if (strcmp(pid, "010F") == 0) { // Intake temp
    obdData.intakeTemp = value - 40;
  } else if (strcmp(pid, "0110") == 0) { // MAF rate
    obdData.mafRate = value / 100.0;
  }
}

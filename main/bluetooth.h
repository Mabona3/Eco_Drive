#pragma once

#include <BluetoothSerial.h>

static BluetoothSerial SerialBT;

void Bluetooth_init() {
  if (!SerialBT.begin("ESP32_Bluetooth",true)) {
    Serial.println("Bluetooth Has Failed");  
    abort();
  }
  Serial.println("Starting discoverAsync...");
  BTScanResults *btDeviceList = SerialBT.getScanResults();  // maybe accessing from different threads!
  if (SerialBT.discoverAsync([](BTAdvertisedDevice *pDevice) {
        // BTAdvertisedDeviceSet*set = reinterpret_cast<BTAdvertisedDeviceSet*>(pDevice);
        // btDeviceList[pDevice->getAddress()] = * set;
        Serial.printf(">>>>>>>>>>>Found a new device asynchronously: %s\n", pDevice->toString().c_str());
      })) {
    delay(BT_DISCOVER_TIME);
    Serial.print("Stopping discoverAsync... ");
    SerialBT.discoverAsyncStop();
    Serial.println("discoverAsync stopped");
    delay(5000);
    if (btDeviceList->getCount() > 0) {
      BTAddress addr;
      int channel = 0;
      Serial.println("Found devices:");
      for (int i = 0; i < btDeviceList->getCount(); i++) {
        BTAdvertisedDevice *device = btDeviceList->getDevice(i);
        Serial.printf(" ----- %s  %s %d\n", device->getAddress().toString().c_str(), device->getName().c_str(), device->getRSSI());
        std::map<int, std::string> channels = SerialBT.getChannels(device->getAddress());
        Serial.printf("scanned for services, found %d\n", channels.size());
        for (auto const &entry : channels) {
          Serial.printf("     channel %d (%s)\n", entry.first, entry.second.c_str());
        }
        if (channels.size() > 0) {
          addr = device->getAddress();
          channel = channels.begin()->first;
        }
      }
      if (addr) {
        Serial.printf("connecting to %s - %d\n", addr.toString().c_str(), channel);
        SerialBT.connect(addr, channel, sec_mask, role);
      }
    } else {
      Serial.println("Didn't find any devices");
    }
  } else {
    Serial.println("Error on discoverAsync f.e. not working after a \"connect\"");
  }
}

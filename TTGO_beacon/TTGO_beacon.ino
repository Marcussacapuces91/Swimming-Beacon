/**
 *   Copyright 2022 Marc SIBERT
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#define DEBUG true
#define __FILENAME__ ( __builtin_strrchr("\\" __FILE__, '\\') + 1 )
#define LOG(args) { Serial.print(__FILENAME__); Serial.print('@'); Serial.print(__LINE__); Serial.print(": "); Serial.println(args); }
#define LOG2(args,fmt) { Serial.print(__FILENAME__); Serial.print('@'); Serial.print(__LINE__); Serial.print(": "); Serial.println(args,fmt); }

#include <TFT_eSPI.h>
#define FSS9 &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
#define FSS18 &FreeSans18pt7b
#define FSS24 &FreeSans24pt7b

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

auto tft = TFT_eSPI();

enum {
  WIDTH = TFT_WIDTH,
  HEIGHT = TFT_HEIGHT
};

static const char* TAG = __FILENAME__;

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) ;
  Serial.print(0x12);
  LOG(F("Copyright 2022 (c) par Marc SIBERT"));
  LOG(F("Debug printout..."));

  ESP_LOGV(TAG, "Starting logging");
#endif

  // Set up the display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setFreeFont(FSS12);
  tft.println();
  tft.print(F("Test Bluetooth BLE"));
  tft.setFreeFont(FSS9);
  tft.println();
  tft.print(F("Copyright (c) 2022 - SIBERT"));

  BLEDevice::init("");
  LOG(F("Scanning..."));
  auto *const pBLEScan = BLEDevice::getScan();
  if (!pBLEScan) {
    LOG(F("Erreur d'instanciation du BLE Scan"));
    return;
  }

  const auto scanTime = 5; // sec
  auto scanResults = pBLEScan->start(scanTime, false);
//  LOG(String(F("Devices ")) + String(scanResults.getCount()));
  for (auto i = scanResults.getCount(); i > 0; --i) {
    auto device = scanResults.getDevice(i - 1);
    LOG(String(F("Device ")) + String(i));
    if (device.haveName()) LOG(String(F("Name: ")) + device.getName().c_str());
    if (device.haveServiceUUID()) LOG(String(F("Serv. UUID: ")) + device.getServiceUUID().toString().c_str());
    if (device.haveManufacturerData()) {
      const auto md = device.getManufacturerData();
      LOG(String(F("Manuf. Data: (")) + String(md.length()) + ")");
      for (auto j = 0; j < md.length(); ++j) {
        LOG2(byte(md.c_str()[j]), HEX);
      }
    }
    if (device.havePayload()) {
      const auto *pPayLoad = device.getPayLoad();
      
      LOG(String(F("Payload: (")) + String(md.length()) + ")");
      for (auto j = 0; j < md.length(); ++j) {
        LOG2(byte(md.c_str()[j]), HEX);
      }
    }
  }

  pBLEScan->clearResults();


}

void loop() {
}

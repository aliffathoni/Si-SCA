#include "BLE.h"

BLEServer* pServer = NULL;
BLECharacteristic* HeartRateCharacteristic = NULL;
BLECharacteristic* BattMonCharacteristic = NULL;
BLECharacteristic* AccelCharacteristic = NULL;

static BLEUUID service_HR_UUID(BLEUUID((uint16_t)0x180D));
static BLEUUID char_HR_UUID(BLEUUID((uint16_t)0x2A37));
BLEDescriptor descriptor_HR_UUID(BLEUUID((uint16_t)0x2901));

static BLEUUID service_BATT_UUID(BLEUUID((uint16_t)0x180F));
static BLEUUID char_BATT_UUID(BLEUUID((uint16_t)0x2A19));
BLEDescriptor descriptor_BATT_UUID(BLEUUID((uint16_t)0x2901));

static BLEUUID service_ACL_UUID(BLEUUID((uint16_t)0x1819));
static BLEUUID char_ACL_UUID(BLEUUID((uint16_t)0x290C));
BLEDescriptor descriptor_ACL_UUID(BLEUUID((uint16_t)0x2713));

bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

BLE::BLE(String deviceName)
{
  _deviceName = deviceName;
}

void BLE::begin(void)
{
  BLEDevice::init(_deviceName.c_str());

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *HeartRateService = pServer->createService(service_HR_UUID);

  // Create a BLE Characteristic
  HeartRateCharacteristic = HeartRateService->createCharacteristic(
                      char_HR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  HeartRateCharacteristic->addDescriptor(&descriptor_HR_UUID);

  BLEService *BattMonService = pServer->createService(service_BATT_UUID);

  BattMonCharacteristic = BattMonService->createCharacteristic(
                      char_BATT_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BattMonCharacteristic->addDescriptor(&descriptor_BATT_UUID);

  BLEService *AccelService = pServer->createService(service_ACL_UUID);

  AccelCharacteristic = AccelService->createCharacteristic(
                      char_ACL_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  AccelCharacteristic->addDescriptor(&descriptor_ACL_UUID);

  // Start the service
  HeartRateService->start();
  BattMonService->start();
  AccelService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(service_HR_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

bool BLE::checkConnection(void)
{
  if (deviceConnected) {
    Serial.print("Device Connected!");
    return true;
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    pServer->startAdvertising(); // restart advertising
    oldDeviceConnected = deviceConnected;
    // Serial.println("Device not Connected!")
    return false;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}

void BLE::send_HR_Data(int hr)
{
  _hr = hr;
  String hrString = String(_hr);
  HeartRateCharacteristic->setValue(hrString.c_str());
  HeartRateCharacteristic->notify();
}

void BLE::send_ECG_Data(int ecg)
{
  _ecg = ecg;
  String ecgString = String(_ecg);
  HeartRateCharacteristic->setValue(ecgString.c_str());
  HeartRateCharacteristic->notify();
}

void BLE::send_User_Pos(String pos)
{
  _pos = pos;
  AccelCharacteristic->setValue(_pos.c_str());
  AccelCharacteristic->notify();
}

void BLE::send_Batt_Percentage(int batt)
{
  _batt = batt;
  String battString = String(_batt);
  BattMonCharacteristic->setValue(battString.c_str());
  BattMonCharacteristic->notify();
}
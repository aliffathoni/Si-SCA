#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

String DeviceName = "ESP32-1234";

BLEServer* pServer = NULL;
BLECharacteristic* HeartRateCharacteristic = NULL;
BLECharacteristic* BattMonCharacteristic = NULL;
BLECharacteristic* AccelCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
static BLEUUID service_HR_UUID(BLEUUID((uint16_t)0x180D));
static BLEUUID char_HR_UUID(BLEUUID((uint16_t)0x2A37));
BLEDescriptor descriptor_HR_UUID(BLEUUID((uint16_t)0x2901));

static BLEUUID service_BATT_UUID(BLEUUID((uint16_t)0x180F));
static BLEUUID char_BATT_UUID(BLEUUID((uint16_t)0x2A19));
BLEDescriptor descriptor_BATT_UUID(BLEUUID((uint16_t)0x2901));

static BLEUUID service_ACL_UUID(BLEUUID((uint16_t)0x1819));
static BLEUUID char_ACL_UUID(BLEUUID((uint16_t)0x290C));
BLEDescriptor descriptor_ACL_UUID(BLEUUID((uint16_t)0x2713));

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println(".");
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  
  // Create the BLE Device
  BLEDevice::init(DeviceName.c_str());

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
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {
        BattMonCharacteristic->setValue((uint8_t*)&value, 2);
        BattMonCharacteristic->notify();
        HeartRateCharacteristic->setValue(String(value).c_str());
        HeartRateCharacteristic->notify();
        AccelCharacteristic->setValue((uint8_t*)&value, 2);
        AccelCharacteristic->notify();
        value++;
        digitalWrite(2, value%2);
        Serial.println(value);
        if(value>100)value=0;
        delay(500); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* HRMonCharacteristic = NULL;
BLECharacteristic* BattMonCharacteristic = NULL;
BLECharacteristic* ECGCharacteristic = NULL;
BLECharacteristic* CondCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

unsigned long lastScan;
unsigned long lastTrans;

int adc;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// #define HR_SERVICE_UUID        "0000180d-0000-1000-8000-00805f9b34fb"
// #define HR_CHARACTERISTIC_UUID "00002a37-0000-1000-8000-00805f9b34fb"

static BLEUUID HR_SERVICE_UUID(BLEUUID((uint16_t)0x180D));
static BLEUUID HR_CHARACTERISTIC_UUID(BLEUUID((uint16_t)0x2A37));

// #define BATT_UUID              "0000180f-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a19-0000-1000-8000-00805f9b34fb"

static BLEUUID BATT_UUID(BLEUUID((uint16_t)0x180F));
static BLEUUID BATT_C_UUID(BLEUUID((uint16_t)0x2A19));

// #define BATT_UUID              "0000183e-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a58-0000-1000-8000-00805f9b34fb"

static BLEUUID ECG_UUID(BLEUUID((uint16_t)0x183E));
static BLEUUID ECG_C_UUID(BLEUUID((uint16_t)0x2A58));

// #define BATT_UUID              "0000181a-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "0000290c-0000-1000-8000-00805f9b34fb"

static BLEUUID COND_UUID(BLEUUID((uint16_t)0x181A));
static BLEUUID COND_C_UUID(BLEUUID((uint16_t)0x290C));

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void sendTask(void *pvParameters)
{
  while(1){
    if (deviceConnected) {
        int adc = random(55, 75);
        String x = String(adc);
        HRMonCharacteristic->setValue(x.c_str());
        HRMonCharacteristic->notify();
        vTaskDelay(750);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        pServer->startAdvertising(); // restart advertising
        // Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  }
}

void sendTask2(void *pvParameters)
{
  while(1){
    if (deviceConnected) {
        int x = random(1,99);
        String aa = String(x);
        BattMonCharacteristic->setValue(aa.c_str());
        BattMonCharacteristic->notify();
        digitalWrite(2, x%2);
        vTaskDelay(1000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        pServer->startAdvertising(); // restart advertising
        // Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  }
}
void sendTask3(void *pvParameters)
{
  while(1){
    if (deviceConnected) {
        int x = random(1,4095);
        ECGCharacteristic->setValue(x);
        ECGCharacteristic->notify();
        digitalWrite(2, x%2);
        vTaskDelay(10);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        pServer->startAdvertising(); // restart advertising
        // Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  }
}

void sendTask4(void *pvParameters)
{
  while(1){
    if (deviceConnected) {
        String condition[3] = {"FALL", "SLEEP", "STAND"};
        int x = random(0,2);
        String aa = condition[x];
        CondCharacteristic->setValue(aa.c_str());
        CondCharacteristic->notify();
        digitalWrite(2, x%2);
        vTaskDelay(5000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        pServer->startAdvertising(); // restart advertising
        // Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  // Create the BLE Device
  BLEDevice::init("Si-SCA");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *HRMonService = pServer->createService(HR_SERVICE_UUID);

  // Create a BLE Characteristic
  HRMonCharacteristic = HRMonService->createCharacteristic(
                      HR_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  HRMonCharacteristic->addDescriptor(new BLE2902());

  BLEService *BattMonService = pServer->createService(BATT_UUID);

  BattMonCharacteristic = BattMonService->createCharacteristic(
                      BATT_C_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BattMonCharacteristic->addDescriptor(new BLE2902());

  BLEService *CondService = pServer->createService(COND_UUID);

  CondCharacteristic = CondService->createCharacteristic(
                      COND_C_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  CondCharacteristic->addDescriptor(new BLE2902());

  BLEService *ECGService = pServer->createService(ECG_UUID);

  ECGCharacteristic = ECGService->createCharacteristic(
                      ECG_C_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  ECGCharacteristic->addDescriptor(new BLE2902());
  
  // Start the service
  HRMonService->start();
  BattMonService->start();
  ECGService->start();
  CondService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(HR_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");

  xTaskCreate(sendTask, "Send HR", 2048, NULL, 1, NULL);
  xTaskCreate(sendTask2, "Send Batt", 2048, NULL, 1, NULL);
  xTaskCreate(sendTask3, "Send ECG", 2048, NULL, 1, NULL);
  xTaskCreate(sendTask4, "Send Condition", 2048, NULL, 1, NULL);
}

void loop() {}

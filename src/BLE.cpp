#include "BLE.h"

String _deviceName = "Si-SCA "+String((uint8_t)ESP.getEfuseMac(), HEX);

BLEServer* pServer = NULL;
BLECharacteristic* HeartRateCharacteristic = NULL;
BLECharacteristic* BattMonCharacteristic = NULL;
BLECharacteristic* ECGCharacteristic = NULL;
BLECharacteristic* ECG2Characteristic = NULL;
BLECharacteristic* CondCharacteristic = NULL;

// #define HR_UUID                "0000180d-0000-1000-8000-00805f9b34fb"
// #define HR_C_UUID              "00002a37-0000-1000-8000-00805f9b34fb"

static BLEUUID HR_UUID(BLEUUID((uint16_t)0x180D));
static BLEUUID HR_C_UUID(BLEUUID((uint16_t)0x2A37));

// #define BATT_UUID              "0000180f-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a19-0000-1000-8000-00805f9b34fb"

static BLEUUID BATT_UUID(BLEUUID((uint16_t)0x180F));
static BLEUUID BATT_C_UUID(BLEUUID((uint16_t)0x2A19));

// #define ECG_UUID              "0000183e-0000-1000-8000-00805f9b34fb"
// #define ECG_C_UUID            "00002a58-0000-1000-8000-00805f9b34fb"

static BLEUUID ECG_UUID(BLEUUID((uint16_t)0x183E));
static BLEUUID ECG_C_UUID(BLEUUID((uint16_t)0x2A58));

// #define COND_UUID              "0000181a-0000-1000-8000-00805f9b34fb"
// #define COND_C_UUID            "0000290c-0000-1000-8000-00805f9b34fb"

static BLEUUID COND_UUID(BLEUUID((uint16_t)0x181A));
static BLEUUID COND_C_UUID(BLEUUID((uint16_t)0x290C));

// #define ECG2_UUID              "00001813-0000-1000-8000-00805f9b34fb"
// #define ECG2_C_UUID            "00002a58-0000-1000-8000-00805f9b34fb"

static BLEUUID ECG2_UUID(BLEUUID((uint16_t)0x1813));
static BLEUUID ECG2_C_UUID(BLEUUID((uint16_t)0x2A58));

bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      pServer->startAdvertising();
    }
};

void bleBegin(String deviceName)
{
  if(deviceName!="") _deviceName = deviceName;
  BLEDevice::init(_deviceName.c_str());

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *HeartRateService = pServer->createService(HR_UUID);

  // Create a BLE Characteristic
  HeartRateCharacteristic = HeartRateService->createCharacteristic(
                      HR_C_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  HeartRateCharacteristic->addDescriptor(new BLE2902());
  
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

  ECG2Characteristic = ECGService->createCharacteristic(
                      ECG2_C_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  ECG2Characteristic->addDescriptor(new BLE2902());

  // Start the service
  HeartRateService->start();
  BattMonService->start();
  ECGService->start();
  CondService->start();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(HR_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

bool checkConnection(void)
{
  if (deviceConnected) return true;
  else return false;
}

void send_HR_Data(int hr)
{
  String hrString = String(hr);
  HeartRateCharacteristic->setValue(hrString.c_str());
  HeartRateCharacteristic->notify();
}

void send_ECG_Data(int ecg)
{
  if(checkConnection()){
    String ecgString = String(ecg);
    HeartRateCharacteristic->setValue(ecgString.c_str());
    HeartRateCharacteristic->notify();
  }
}

void send_User_Activity(String pos)
{
  if(checkConnection()){
    CondCharacteristic->setValue(pos.c_str());
    CondCharacteristic->notify();
  }
}

void send_Batt_Percentage(int batt)
{
  if(checkConnection()){
    String battString = String(batt);
    BattMonCharacteristic->setValue(battString.c_str());
    BattMonCharacteristic->notify();
  }
}

void send_Diagnose(String quickResult)
{
  //create characteristic and send notify
  if(checkConnection()){
    //send diagnose
  }
}
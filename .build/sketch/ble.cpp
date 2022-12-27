#line 1 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\ble.cpp"
#include "ble.h"

BLEServer* pServer = NULL;
BLECharacteristic* HeartRateCharacteristic = NULL;
BLECharacteristic* BattMonCharacteristic = NULL;
BLECharacteristic* ECGCharacteristic = NULL;
BLECharacteristic* AICharacteristic = NULL;
BLECharacteristic* CondCharacteristic = NULL;

// #define HR_SERVICE_UUID        "0000180d-0000-1000-8000-00805f9b34fb"
// #define HR_CHARACTERISTIC_UUID "00002a37-0000-1000-8000-00805f9b34fb"

static BLEUUID HR_SERVICE_UUID(BLEUUID((uint16_t)0x180D));
static BLEUUID HR_CHARACTERISTIC_UUID(BLEUUID((uint16_t)0x2A37));

// #define BATT_UUID              "0000180f-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a19-0000-1000-8000-00805f9b34fb"

static BLEUUID BATT_UUID(BLEUUID((uint16_t)0x180F));
static BLEUUID BATT_C_UUID(BLEUUID((uint16_t)0x2A19));

// #define BATT_UUID              "0000181a-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a58-0000-1000-8000-00805f9b34fb"

static BLEUUID ECG_UUID(BLEUUID((uint16_t)0x181A));
static BLEUUID ECG_C_UUID(BLEUUID((uint16_t)0x2A58));

// #define BATT_UUID              "0000183e-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "0000290c-0000-1000-8000-00805f9b34fb"

static BLEUUID COND_UUID(BLEUUID((uint16_t)0x183E));
static BLEUUID COND_C_UUID(BLEUUID((uint16_t)0x290C));

// #define AI_UUID              "0000183e-0000-1000-8000-00805f9b34fb"
// #define AI_C_UUID            "00002b3b-0000-1000-8000-00805f9b34fb"

static BLEUUID AI_UUID(BLEUUID((uint16_t)0x183E));
static BLEUUID AI_C_UUID(BLEUUID((uint16_t)0x2B3B));

bool deviceConnected = false;
bool oldDeviceConnected = false;

bluetooth::bluetooth(){}

bluetooth::~bluetooth(){}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device Connected!");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device Disconnected!");
        pServer->startAdvertising();
    }
};

void bluetooth::init()
{
    BLEDevice::init("Sisca ver 0.8");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *HeartRateService = pServer->createService(HR_SERVICE_UUID);
    HeartRateCharacteristic = HeartRateService->createCharacteristic(
                        HR_CHARACTERISTIC_UUID,
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

    AICharacteristic = CondService->createCharacteristic(
                        AI_C_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    AICharacteristic->addDescriptor(new BLE2902());

    HeartRateService->start();
    BattMonService->start();
    ECGService->start();
    CondService->start();
    
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(HR_SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Bluetooth Created!");
}

void bluetooth::send_bpm(int bpm_value)
{
    String bpm = String(bpm_value);
    HeartRateCharacteristic->setValue(bpm.c_str());
    HeartRateCharacteristic->notify();
}

void bluetooth::send_ecg(int ecg_value)
{
    String ecg = String(ecg_value);
    ECGCharacteristic->setValue(ecg.c_str());
    ECGCharacteristic->notify();
}

void bluetooth::send_position(String position_value)
{
    CondCharacteristic->setValue(position_value.c_str());
    CondCharacteristic->notify();
}

void bluetooth::send_battery(int batt_value)
{
    String batt = String(batt_value);
    BattMonCharacteristic->setValue(batt.c_str());
    BattMonCharacteristic->notify();
}

void bluetooth::send_ai(String ai_value)
{
    AICharacteristic->setValue(ai_value.c_str());
    AICharacteristic->notify();
}

bool bluetooth::status()
{
    if(deviceConnected) return true;
    else return false;
}
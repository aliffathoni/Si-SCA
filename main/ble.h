#ifndef BLE_H_
#define BLE_H_

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

String device_name = "SiSCA-"+String((uint8_t)ESP.getEfuseMac(), HEX);

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

#endif
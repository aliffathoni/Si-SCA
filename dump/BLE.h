#ifndef BLE_h
#define BLE_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

bool checkConnection(void);
void bleBegin(String deviceName);
void send_HR_Data(int hr);
void send_ECG_Data(int ecg);
void send_User_Activity(String pos);
void send_Batt_Percentage(int batt);
void send_Diagnose(String quickResult);

#endif
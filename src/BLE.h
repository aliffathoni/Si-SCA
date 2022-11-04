#ifndef BLE_h
#define BLE_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BLE
{
  public:
    BLE(String deviceName);
    bool checkConnection(void);
    void begin(void);
    void send_HR_Data(int hr);
    void send_ECG_Data(int ecg);
    void send_User_Pos(String pos);
    void send_Batt_Percentage(int batt);
    void send_Diagnose(String quickResult)
  private:
    String _deviceName = "Si-SCA "+String((uint8_t)ESP.getEfuseMac(), HEX);
};

#endif
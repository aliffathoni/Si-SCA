#ifndef BLE_h
#define BLE_h

#include <Arduino.h>

class BLE
{
  public:
    BLE(void);
    void begin(String deviceName);
    void sendData(void);
  private:  
    String _deviceName = "Si-SCA 1";//+String((uint32_t)ESP.getEfuseMac(), HEX);
};

#endif
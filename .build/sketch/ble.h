#line 1 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\ble.h"
#ifndef BLE_H_
#define BLE_H_

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


class bluetooth
{
    public:
        bluetooth(/* args */);
        ~bluetooth();
        void init();
        void send_bpm(int bpm_value);
        void send_position(String position_value);
        void send_battery(int batt_value);
        void send_ecg(int ecg_value);
        void send_ai(String ai_value);
        bool status();    
};

#endif
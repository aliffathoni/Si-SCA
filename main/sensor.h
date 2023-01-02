#ifndef SENSOR_H_
#define SENSOR_H_

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


class sensor
{
    private:
        #define mpu_address 0x69

    public:
        sensor(/* args */);
        ~sensor();
        void init();
        int get_bpm(int ecgValue);
        String get_position();
};

#endif
#ifndef MPU6050_h
#define MPU6050_h

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

void mpuBegin(void);
String getPosition(void);

#endif
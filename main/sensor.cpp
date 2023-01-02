#include "sensor.h"

Adafruit_MPU6050 mpu;

sensor::sensor(){}

sensor::~sensor(){}

void sensor::init()
{
  mpu.begin(mpu_address);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

int sensor::get_bpm(int ecgValue)
{
  int rand = ecgValue;
  return rand;
}

String sensor::get_position()
{
  float position;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Z axis is gravity measurement value of the sensor
  position = a.acceleration.z;

  // Standard gravity is 9.8m/s^2
  if(position > 8.0)
    return "Sleep";
  else if((position <= 8.0) && (position >= -6.0))
    return "Stand";
  else
    return "Fall";
}
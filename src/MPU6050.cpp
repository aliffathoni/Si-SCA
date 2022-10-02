#include "MPU6050.h"

Adafruit_MPU6050 mpu;

void MPU6050::begin(void)
{
  mpu.begin();

  //default Adafruit MPU6050 Setting
  //Set MPU Accrelorometer Range
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  // mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  // mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  
  //Set MPU Gyro Range
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  // mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  // mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  
  //Set MPU Filter Bandwith
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  
}

float MPU6050::getPosition(void)
{
  float position;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  position = a.acceleration.z;
  return position;
}
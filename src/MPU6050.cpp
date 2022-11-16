#include "MPU6050.h"

Adafruit_MPU6050 mpu;

/*!
 *    @brief  Initiate and start MPU6050 sensor.
 */
void mpuBegin(void)
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

/*!
 *    @brief  Get User Activity Position.
 *
 *    @return String value of current user position.
 */
String getPosition(void)
{
  float position;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Z axis is gravity measurement value of the sensor
  position = a.acceleration.z;

  // Standard gravity is 9.8m/s^2
  if(position > 8.0)
    return "Sleep";
  else if((position <= 8.0) && (position >= 0.0))
    return "Stand";
  else
    return "Fall";
}
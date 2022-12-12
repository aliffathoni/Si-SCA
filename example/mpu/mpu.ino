// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
#define mpu_i2c 0x69
void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin(mpu_i2c)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

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

void loop() {
  Serial.print("User Position : ");
  Serial.println(getPosition());

  // Serial.println("");
  delay(100);
}
#ifndef CONFIG_h
#define CONFIG_h

#include <Arduino.h>
#include <TaskScheduler.h>

#include "MPU6050.h"
#include "AD8232.h"
#include "indicator.h"
#include "BLE.h"

#define LED_PIN     15
#define BUZZER_PIN  5
#define adp         16
#define adn         17
#define ana         34

LED led(LED_PIN);
AD8232 ad(adp, adn, ana);
MPU6050 mp;
BLE ble("sisca");
BUZZER buzz(BUZZER_PIN);

class SiSCA
{ 
  public:
    SiSCA(void);
    void begin(void);
    void startSystem(void);
};

SiSCA::SiSCA(void){}

void SiSCA::begin()
{
  led.begin();
  ble.begin();
  buzz.begin();
  // ad.begin();
  // mp.begin();
}

void SiSCA::startSystem()
{
  // ad.getDataECG();
  // ad.getDataHR();
  // Serial.println(mp.getPosition());
  // if(ble.checkConnection()){
  //   int r = random(1, 99);
  //   ble.send_HR_Data(r);
  // }
  // buzz.set(1);
  led.on("Green", 64);
  delay(500);
  led.off();
  delay(500);
}

#endif
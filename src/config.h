#ifndef CONFIG_h
#define CONFIG_h

#include <Arduino.h>

#include "MPU6050.h"
#include "AD8232.h"
#include "indicator.h"
#include "BLE.h"

#define LED_PIN 2
#define BUZZER  5
#define adp     16
#define adn     17
#define ana     34

LED led(LED_PIN);
AD8232 ad(adp, adn, ana);
MPU6050 mp;

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
  ad.begin();
  mp.begin();
}

void SiSCA::startSystem()
{
  ad.getDataECG();
  ad.getDataHR();
  Serial.println(mp.getPosition());
  led.on();
  delay(100);
  led.off();
  delay(100);
}

#endif
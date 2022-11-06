#include "config.h"
#include <TaskScheduler.h>

SiSCA::SiSCA(String bluetoothName, uint8_t ledPin, uint8_t buzzerPin, uint8_t batteryPin, uint8_t adcPin)
{
  _bluetoothName = bluetoothName;
  _ledPin = ledPin;
  _buzzerPin = buzzerPin;
  _batteryPin = batteryPin;
  _adcPin = adcPin;
}

void SiSCA::begin(void)
{
  ledBegin(_ledPin);
  bleBegin(_bluetoothName);
  buzzerBegin(_buzzerPin);
  adBegin(_adcPin);
  mpuBegin();
}

void SiSCA::startSystem(void)
{
  ledOn("Green");
  delay(500);
  ledOff();
  delay(500);
}
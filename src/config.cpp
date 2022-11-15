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

int SiSCA::getHR(void)
{
  int hr = getDataHR();
  return hr;
}

int SiSCA::getECG(void)
{
  int ecg = getDataECG();
  return ecg;
}

String SiSCA::getDiagnose(void)
{
  String diagnose = quickDetect();
  return diagnose;
}

int SiSCA::getBattery(void)
{
  int battery = getDataBattery();
  return battery;
}

String SiSCA::getActivity(void)
{
  String activity = getPosition();
  return activity;
}

void SiSCA::sendData(void)
{
  send_HR_Data(getDataHR);
  send_Diagnose(getDiagnose);
  send_Batt_Percentage(getBattery);
  send_User_Activity(getActivity);
}

void SiSCA::sendECGData(void)
{
  send_ECG_Data(getECG);
}
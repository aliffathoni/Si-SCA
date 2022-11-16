#include "config.h"
#include <TaskScheduler.h>

/*!
 *    @brief  Initiate class name.
 *
 *    @return Initiated device name and wiring connection.
 */
SiSCA::SiSCA(String bluetoothName, uint8_t ledPin, uint8_t buzzerPin, uint8_t batteryPin, uint8_t adcPin)
{
  _bluetoothName = bluetoothName;
  _ledPin = ledPin;
  _buzzerPin = buzzerPin;
  _batteryPin = batteryPin;
  _adcPin = adcPin;
}

/*!
 *    @brief  start all necessary sensors and output
 */
void SiSCA::begin(void)
{
  ledBegin(_ledPin);
  bleBegin(_bluetoothName);
  buzzerBegin(_buzzerPin);
  adBegin(_adcPin);
  mpuBegin();
}

/*
 *  @brief start SiSCA system
*/
void SiSCA::startSystem(void)
{
  ledOn("Green");
  delay(500);
  ledOff();
  delay(500);
}

/*!
 *    @brief  Get heart rate count data from sensor.
 *
 *    @return Integer value of Heart Rate measurement.
 */
int SiSCA::getHR(void)
{
  int hr = getDataHR();
  return hr;
}

/*!
 *    @brief  Get ECG data from sensor.
 *
 *    @return Integer value of ECG analog input value.
 */
int SiSCA::getECG(void)
{
  int ecg = getDataECG();
  return ecg;
}

/*!
 *    @brief  Get quick diagnostic data based on ECG data.
 *
 *    @return String of quick diagnose result.
 */
String SiSCA::getDiagnose(void)
{
  String diagnose = quickDetect();
  return diagnose;
}

/*!
 *    @brief  Get device battery percentage.
 *
 *    @return Integer value of battery percentage.
 */
int SiSCA::getBattery(void)
{
  int battery = getDataBattery();
  return battery;
}

/*!
 *    @brief  Get current user activity.
 *
 *    @return String of current activity.
 */
String SiSCA::getActivity(void)
{
  String activity = getPosition();
  return activity;
}

/*!
 *    @brief  Send Heart Rate, Quick Diagnostic, Battery percentage
 *            and User Activity data to client.
 */
void SiSCA::sendData(void)
{
  send_HR_Data(getDataHR);
  send_Diagnose(getDiagnose);
  send_Batt_Percentage(getBattery);
  send_User_Activity(getActivity);
}

/*!
 *    @brief  Send specific ECG data to client.
 */
void SiSCA::sendECGData(void)
{
  send_ECG_Data(getECG);
}
#include "indicator.h"

Adafruit_NeoPixel pixels(1, 15, NEO_GRB + NEO_KHZ800);

/*!
 *    @brief  Initiate Adafruit_NeoPixel Class and
 *            set connected pin.
 * 
 *    @param  ledPin is pin connected to LED Data Input
 */
void ledBegin(int ledPin)
{
  pixels.begin();
  pixels.setPin(ledPin);
}

/*!
 *    @brief  Turn on LED.
 *    
 *    @param  color is color to show on LED.
 */
void ledOn(String color)
{
  if(color == "Red") pixels.setPixelColor(0, 255, 0, 0);
  else if(color == "Green") pixels.setPixelColor(0, 0, 255, 0);
  else if(color == "Blue") pixels.setPixelColor(0, 0, 0, 255);
  else pixels.setPixelColor(0, 127, 127, 127);

  pixels.show();
}

/*!
 *    @brief  Turn off LED.
 */
void ledOff(void)
{
  pixels.clear();
  pixels.show();
}

uint8_t _buzzerPin;

/*!
 *    @brief  Initiate buzzer and
 *            set connected pin.
 * 
 *    @param  buzzerPin is pin connected to Buzzer.
 */
void buzzerBegin(uint8_t buzzerPin)
{
  _buzzerPin = buzzerPin;
  pinMode(_buzzerPin, OUTPUT);
}

/*!
 *    @brief  Turn on buzzer.
 */
void buzzerOn(void)
{
  digitalWrite(_buzzerPin, HIGH);
}

/*!
 *    @brief  Turn off buzzer.
 */
void buzzerOff(void)
{
  digitalWrite(_buzzerPin, LOW);
}

uint8_t _batteryPin;

/*!
 *    @brief  Initiate battery voltage sensor and
 *            set connected pin.
 * 
 *    @param  batteryPin is pin connected to battery sens.
 */
void batteryBegin(uint8_t batteryPin)
{
  _batteryPin = batteryPin;
  pinMode(_batteryPin, INPUT);
}

/*!
 *    @brief  Get battery data.
 * 
 *    @return Integer of battery percentage.
 */
int getDataBattery(void)
{
  int analogValue = analogRead(_batteryPin);
  int percentage = 0;

  percentage = (analogValue/4095)*100;
  return percentage;
}
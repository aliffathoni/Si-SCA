#include "indicator.h"

Adafruit_NeoPixel pixels(1, 15, NEO_GRB + NEO_KHZ800);

void ledBegin(int ledPin)
{
  pixels.begin();
  pixels.setPin(ledPin);
}

void ledOn(String color)
{
  if(color == "Red") pixels.setPixelColor(0, 255, 0, 0);
  else if(color == "Green") pixels.setPixelColor(0, 0, 255, 0);
  else if(color == "Blue") pixels.setPixelColor(0, 0, 0, 255);
  else pixels.setPixelColor(0, 127, 127, 127);

  pixels.show();
}

void ledOff(void)
{
  pixels.clear();
  pixels.show();
}

uint8_t _buzzerPin;

void buzzerBegin(uint8_t buzzerPin)
{
  _buzzerPin = buzzerPin;
  pinMode(_buzzerPin, OUTPUT);
}

void buzzerOn(void)
{
  digitalWrite(_buzzerPin, HIGH);
}

void buzzerOff(void)
{
  digitalWrite(_buzzerPin, LOW);
}

uint8_t _batteryPin;

void batteryBegin(uint8_t batteryPin)
{
  _batteryPin = batteryPin;
  pinMode(_batteryPin, INPUT);
}

int getDataBattery(void)
{
  int analogValue = analogRead(_batteryPin);
  int percentage = 0;

  percentage = (analogValue/4095)*100;
  return percentage;
}
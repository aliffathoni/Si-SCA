#include "indicator.h"
#include <FastLED.h>

CRGB leds[1];

LED::LED(uint8_t pin)
{
  _pin = pin;
}

BUZZER::BUZZER(unsigned char pin)
{
  _pin = pin;
}

BATTERY::BATTERY(uint8_t pin)
{
  _pin = pin;
}

void LED::begin(void)
{
  pinMode(_pin, OUTPUT);
  FastLED.addLeds<WS2812, _pin, GRB>(leds, 1);
}

void LED::on(String color, int brightness)
{ 
  if(color == "Red") leds[0] = CRGB::Red;
  else if(color == "Green") leds[0] = CRGB::Green;
  else if(color == "Blue") leds[0] = CRGB::Blue;
  else leds[0] = CRGB::Red;

  FastLED.setBrightness(brightness);
  FastLED.show();
}

void LED::off(void)
{
  FastLED.setBrightness(0);
  FastLED.show();
}

void BUZZER::begin(void)
{
  pinMode(_pin, OUTPUT);
}

void BUZZER::set(bool _buzzerState)
{
  digitalWrite(_pin, _buzzerState);
}

double BATTERY::getBattery(void)
{
  int analogValue = analogRead(_pin);
  int percentage = 0;

  percentage = (analogValue/4095)*100;
  return percentage;
}
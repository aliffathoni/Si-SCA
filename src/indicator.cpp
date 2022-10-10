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

void LED::begin(void)
{
  pinMode(_pin, OUTPUT);
  FastLED.addLeds<WS2812, 15>(leds, 1);
}

void LED::on(char color)
{ 
  _color = color;
  switch (color) {
    case 'R':
      leds[0] = CRGB::Red;
      break;
    case 'G':
      leds[0] = CRGB::Green;
      break;
    case 'B':
      leds[0] = CRGB::Blue;
      break;
    default :
      leds[0] = CRGB::White;
      break;
  }
  FastLED.setBrightness(4);
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

void BUZZER::on(void)
{
  digitalWrite(_pin, HIGH);
}

void BUZZER::off(void)
{
  digitalWrite(_pin, LOW);
}
#include "indicator.h"

void LED::begin(uint8_t pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void BUZZER::begin(uint8_t pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}
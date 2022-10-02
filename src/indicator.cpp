#include "indicator.h"

LED::LED(uint8_t pin)
{
  _pin = pin;
}

BUZZER::BUZZER(uint8_t pin)
{
  _pin = pin;
}

void LED::begin(void)
{
  pinMode(_pin, OUTPUT);
}

void LED::on(void)
{
  digitalWrite(_pin, HIGH);
}

void LED::off(void)
{
  digitalWrite(_pin, LOW);
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
#ifndef INDICATOR_h
#define INDICATOR_h

#include <Arduino.h>

class LED
{
  public:
    void begin(uint8_t pin);
    void on();
    void off();
  private:
    uint8_t _pin;
};

class BUZZER
{
  public:
    void begin(uint8_t pin);
    void on();
    void off();
  private:
    uint8_t _pin;
};

#endif
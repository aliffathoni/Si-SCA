#ifndef INDICATOR_h
#define INDICATOR_h

#include <Arduino.h>

class LED
{
  public:
    LED(uint8_t pin);
    void begin(void);
    void on(void);
    void off(void);
  private:
    uint8_t _pin;
};

class BUZZER
{
  public:
    BUZZER(uint8_t pin);
    void begin(void);
    void on(void);
    void off(void);
  private:
    uint8_t _pin;
};

#endif
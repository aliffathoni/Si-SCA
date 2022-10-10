#ifndef INDICATOR_h
#define INDICATOR_h

#include <Arduino.h>

class LED
{
  public:
    LED(uint8_t pin);
    void begin(void);
    void on(char color);
    void off(void);
  private:
    unsigned char _pin;
    char _color;
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
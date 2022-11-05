#ifndef INDICATOR_h
#define INDICATOR_h

#include <Arduino.h>

class LED
{
  public:
    LED(int pin);
    void begin(void);
    void on(String color, int brightness);
    void off(void);
  private:
    int _pin;
};

class BUZZER
{
  public:
    BUZZER(uint8_t pin);
    void begin(void);
    void set(bool _buzzerState);
  private:
    uint8_t _pin;
};

class BATTERY
{
  public:
    BATTERY(uint8_t pin);
    double getBattery(void);
  private:
    uint8_t _pin;
};

#endif
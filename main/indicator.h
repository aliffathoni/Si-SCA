#ifndef INDICATOR_H_
#define INDICATOR_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class indicator
{
    private:
        uint8_t _pinRGB;
        uint8_t _pinBuzzer;
        uint8_t _pinLED;
        
    public:
        indicator(uint8_t pinRGB, uint8_t pinBuzzer, uint8_t pinLED);
        ~indicator();
        void init();
        void initRGB();
        void set_led(int duty_cycle);
        void sleep();
        void warning();
        void disconnected();
        void connected();
        void disconnected_indicator();
        void connected_indicator();
        void setup();
};

#endif
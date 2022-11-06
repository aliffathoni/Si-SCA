#ifndef INDICATOR_h
#define INDICATOR_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

void ledBegin(int ledPin);
void ledOn(String color);
void ledOff(void);

void buzzerBegin(uint8_t buzzerPin);
void buzzerOn(void);
void buzzerOff(void);

void batteryBegin(uint8_t batteryPin);
int getBattery(void);

#endif
#ifndef AD8232_h
#define AD8232_h

#include <Arduino.h>

int getDataECG(void);
int getDataHR(void);
void adBegin(uint8_t pin);
void setTresshold(int Upper, int Lower);
String quickDetect(void);

#endif
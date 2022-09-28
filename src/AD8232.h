#ifndef AD8232_h
#define AD8232_h

#include <Arduino.h>

class AD8232
{
  public:
    int getDataECG(uint8_t pin);
    int getDataHR(uint8_t pin);
    void setLatency(long latency);
    void setTresshold(int bpm);
  private:
    uint8_t _pin;
    uint8_t _latency = 20;
    uint8_t _bpm;
    uint8_t lastRead;
};

#endif
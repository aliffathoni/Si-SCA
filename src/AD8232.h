#ifndef AD8232_h
#define AD8232_h

#include <Arduino.h>

class AD8232
{
  public:
    AD8232(uint8_t pin, uint8_t pPin, uint8_t nPin);
    int getDataECG(void);
    int getDataHR(void);
    void begin(void);
    void setLatency(long latency);
    void setTresshold(int Upper, int Lower);
    String quickDetect(void);
  private:
    uint8_t _pin;
    long _latency = 20;
    long lastRead;
    int _bpm;
    uint8_t _pPin, _nPin;
    int _adc;
    int BPM;
    int UpperTresshold;
    int LowerTresshold;
    bool IgnoreReading = false;
    bool FirstPulseDetected = false;
    unsigned long FirstPulseTime = 0;
    unsigned long SecondPulseTime = 0;
    unsigned long PulseInterval = 0;
    unsigned long lastScan = 0;
    unsigned long lastPrint = 0;
    String quickDetectValue;
};

#endif
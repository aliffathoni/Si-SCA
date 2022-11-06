#include "AD8232.h"

uint8_t _pin;
long _latency = 20;
long lastRead;
int _bpm;
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

void adBegin(uint8_t pin)
{
  _pin = pin;
  
  pinMode(_pin, INPUT);
}

void setLatency(long latency)
{
  _latency = latency;
}

void setTresshold(int Upper, int Lower)
{
  UpperTresshold = Upper;
  LowerTresshold = Lower;
}

int getDataECG(void)
{
  int ecg;
  if(millis() - lastRead > _latency){
    lastRead = millis();
    ecg = analogRead(_pin);
    // Serial.println(ecg);
    _adc = ecg;
    return ecg;
  } else{
    return 0;
  }
}

int getDataHR(void)
{
  int hr;
  if(millis() - lastRead > _latency){
    lastRead = millis();
    hr = analogRead(_pin);
    if(hr > UpperTresshold && IgnoreReading == false){
      if(FirstPulseDetected == false){
        FirstPulseTime = millis();
        FirstPulseDetected = true;
      }
      else{
        SecondPulseTime = millis();
        PulseInterval = SecondPulseTime - FirstPulseTime;
        FirstPulseTime = SecondPulseTime;
      }
      IgnoreReading = true;
    }

    // Heart beat trailing edge detected.
    if(hr < LowerTresshold && IgnoreReading == true){
      IgnoreReading = false;
    }  

    // Calculate Beats Per Minute.
    BPM = (1.0/PulseInterval) * 60.0 * 1000;

    return BPM;
  } else{
    return 0;
  }
}

String quickDetect(void)
{
  if(_adc /*gelombang normal*/){
    quickDetectValue = "Normal";
    return "Normal";
  }
  if(_adc /*gelombang aneh*/){
    if(_adc/*takar*/){
      quickDetectValue = "Takar";
      return "Takar";
    } else{
      quickDetectValue = "Fibri";
      return "Vibri";
    }
  }
}
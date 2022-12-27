#include "AD8232.h"

uint8_t _pin;
long lastRead;
int _bpm;
int _adc;
int BPM;
int UpperTresshold = 2000;
int LowerTresshold = 1000;
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

void setTresshold(int Upper, int Lower)
{
  UpperTresshold = Upper;
  LowerTresshold = Lower;
}

int getDataECG(void)
{
  int ecg;
  ecg = analogRead(_pin);
  return ecg;
}

int getDataHR(void)
{
  int hr;
  hr = getDataECG();
  if(hr > UpperTresshold && IgnoreReading == false){
    if(FirstPulseDetected == false){
      LowerTresshold = hr - 700;
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
}

String quickDetect(void)
{
  if(getDataECG()/*gelombang normal*/){
    quickDetectValue = "Normal";
    return "Normal";
  }
  if(getDataECG()/*gelombang aneh*/){
    if(getDataECG()/*takar*/){
      quickDetectValue = "Takar";
      return "Takar";
    } else{
      quickDetectValue = "Fibri";
      return "Vibri";
    }
  }
}
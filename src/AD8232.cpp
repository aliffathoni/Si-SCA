#include "AD8232.h"

AD8232::AD8232(uint8_t pin, uint8_t pPin, uint8_t nPin)
{
  _pin = pin;
  _pPin = pPin;
  _nPin = nPin;
}

void AD8232::begin()
{
  pinMode(_pPin, INPUT);
  pinMode(_nPin, INPUT);
}

void AD8232::setLatency(long latency)
{
  _latency = latency;
}

void AD8232::setTresshold(int Upper, int Lower)
{
  UpperTresshold = Upper;
  LowerTresshold = Lower;
}

int AD8232::getDataECG(void)
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

int AD8232::getDataHR(void)
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

String AD8232::quickDetect(void)
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
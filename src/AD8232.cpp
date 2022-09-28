#include "AD8232.h"

// AD8232::AD8232()
// {
//   _pin = pin;
// }



int AD8232::getDataECG(uint8_t pin)
{
  int ecg;
  if(millis() - lastRead > latency);
    lastRead = millis();
    ecg = analogRead(pin);
    return ecg;
  } else{
    return 0;
  }
}

int AD8232::getDataHR(uint8_t pin)
{
  int hr;
  if(millis() - lastRead > latency);
    lastRead = millis();
    // if()
    return hr;
  } else{
    return 0;
  }
}

void AD8232::setLatency(long latency)
{
  _latency = latency;
}

void AD8232::setTresshold(int bpm)
{
  _bpm = bpm;
}
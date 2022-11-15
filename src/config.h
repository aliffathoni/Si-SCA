#ifndef CONFIG_h
#define CONFIG_h

#include <Arduino.h>

#include "MPU6050.h"
#include "AD8232.h"
#include "indicator.h"
#include "BLE.h"

#define LED_PIN     15
#define BUZZER_PIN  5
#define adp         16
#define adn         17
#define ana         34


/*!
 *  @brief initiate SiSCA system class
*/
class SiSCA
{ 
  public:
    SiSCA(String bluetoothName,
          uint8_t ledPin,
          uint8_t buzzerPin,
          uint8_t batteryPin,
          uint8_t adcPin);

    void begin(void);

    void startSystem(void);

    int getHR(void);

    int getECG(void);

    String getDiagnose(void);

    int getBattery(void);

    String getActivity(void);

    void getData(void);

    void setBuzzer(void);

    void setLed(void);

    void startBluetooth(void);

    void stopBluetooth(void);
    
    void sendData(void);

    void sendECGData(void);

  private:
    String _bluetoothName;
    uint8_t _ledPin,
            _buzzerPin,
            _batteryPin,
            _adcPin;
};

#endif
#include <Arduino.h>
#line 1 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
#include "driver/rtc_io.h"
#include "indicator.h"
#include "sensor.h"
#include "ble.h"

bool deviceConnected = false;

#define RGB_PIN     26
#define BUZZER_PIN  14
#define LED_PIN     23
#define BUTTON_PIN  GPIO_NUM_27

indicator cond(RGB_PIN, BUZZER_PIN, LED_PIN);
sensor sens;

TaskHandle_t led_task = NULL;
TaskHandle_t mpu_task = NULL;
TaskHandle_t send_mpu = NULL;
TaskHandle_t send_ecg = NULL;
TaskHandle_t send_rest = NULL;
TaskHandle_t sleep_task = NULL;

int bpm_value = 0;
int ecg_value = 0;
String lastPosition;

unsigned long sleepTimer;

#line 29 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void wakeup_reason();
#line 53 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void led(void *pvParameter);
#line 65 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void mpu(void *pvParameter);
#line 103 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void send_ecg_data(void *pvParameter);
#line 118 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void send_mpu_data(void *pvParameter);
#line 132 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void send_rest_data(void *pvParameter);
#line 155 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void sleep_check(void *pvParameter);
#line 171 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void setup();
#line 259 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void loop();
#line 29 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\main\\main.ino"
void wakeup_reason()
{
    rtc_gpio_pullup_en(BUTTON_PIN);
    esp_sleep_enable_ext0_wakeup(BUTTON_PIN, 0);
    esp_sleep_enable_timer_wakeup(15 * 1000000);
  
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 :
            Serial.println("Wakeup caused by external signal using RTC_IO");
            break;
        case ESP_SLEEP_WAKEUP_TIMER :
            Serial.println("Wakeup caused by timer");
            cond.sleep();
            esp_deep_sleep_start();
            break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}

void led (void *pvParameter)
{
    while(1){
        ecg_value = analogRead(39);
        bpm_value = sens.get_bpm(ecg_value);
        int duty = map(ecg_value, 0, 4095, 0, 1023);
        cond.set_led(duty);

        vTaskDelay(1);
    }
}

void mpu (void *pvParameter)
{
    while(1){
        if(sens.get_position() != lastPosition){
            lastPosition = sens.get_position();
        }
        if(lastPosition == "Fall"){
            cond.warning();
            vTaskDelay(2500);
        }
        vTaskDelay(100);
    }
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        cond.connected_indicator();
        Serial.println("Device Connected!");
        vTaskResume(mpu_task);
        vTaskResume(led_task);
        vTaskResume(send_ecg);
        vTaskResume(send_mpu);
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        cond.disconnected_indicator();
        Serial.println("Device Disconnected!");
        pServer->startAdvertising();
        sleepTimer = millis();
        vTaskSuspend(mpu_task);
        vTaskSuspend(led_task);
        vTaskSuspend(send_ecg);
        vTaskSuspend(send_mpu);
    }
};

void send_ecg_data (void *pvParameter)
{
  while (1)
  {
    if(deviceConnected){
      String a = String(ecg_value);
      ECGCharacteristic->setValue(a.c_str());
      ECGCharacteristic->notify();
      Serial.println(a);
    }
    
    vTaskDelay(20);
  }
}

void send_mpu_data (void *pvParameter)
{
  while(1)
  {
    if(deviceConnected){
      String condition = lastPosition;
      CondCharacteristic->setValue(condition.c_str());
      CondCharacteristic->notify();
    }

    vTaskDelay(100);
  }
}

void send_rest_data (void *pvParameter)
{
  while(1)
  {
    if(deviceConnected){
      String condition = "Normal";
      AICharacteristic->setValue(condition.c_str());
      AICharacteristic->notify();

      int x = 100;
      String a = String(x);
      BattMonCharacteristic->setValue(a.c_str());
      BattMonCharacteristic->notify();

      String bpm = String(bpm_value);
      HeartRateCharacteristic->setValue(bpm.c_str());
      HeartRateCharacteristic->notify();
    }

    vTaskDelay(1000);
  }
}

void sleep_check(void *pvParameter)
{
  for(;;){
    if(!deviceConnected){
        if(millis() - sleepTimer > 60000){
            esp_deep_sleep_start();
        }
        cond.disconnected();
    }   else{
        cond.connected();
    }

    vTaskDelay(2500 / portTICK_PERIOD_MS);
  }
}

void setup()
{
    Serial.begin(115200);
    cond.initRGB();

    wakeup_reason();

    cond.init();
    sens.init();

    BLEDevice::init(device_name.c_str());

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *HeartRateService = pServer->createService(HR_SERVICE_UUID);
    HeartRateCharacteristic = HeartRateService->createCharacteristic(
                        HR_CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    HeartRateCharacteristic->addDescriptor(new BLE2902());
    
    BLEService *BattMonService = pServer->createService(BATT_UUID);
    BattMonCharacteristic = BattMonService->createCharacteristic(
                        BATT_C_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    BattMonCharacteristic->addDescriptor(new BLE2902());

    BLEService *CondService = pServer->createService(COND_UUID);
    CondCharacteristic = CondService->createCharacteristic(
                        COND_C_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    CondCharacteristic->addDescriptor(new BLE2902());

    BLEService *ECGService = pServer->createService(ECG_UUID);
    ECGCharacteristic = ECGService->createCharacteristic(
                        ECG_C_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    ECGCharacteristic->addDescriptor(new BLE2902());

    AICharacteristic = CondService->createCharacteristic(
                        AI_C_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    AICharacteristic->addDescriptor(new BLE2902());

    HeartRateService->start();
    BattMonService->start();
    ECGService->start();
    CondService->start();
    
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(HR_SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
    Serial.println("Bluetooth Created!");

    xTaskCreate(mpu, "MPU Task", 5000, NULL, 3, &mpu_task);
    xTaskCreate(led, "LED PWM Task", 5000, NULL, 3, &led_task);
    xTaskCreate(sleep_check, "Sleep Timer", 5000, NULL, 3, &sleep_task);
    xTaskCreate(send_ecg_data, "Send ECG Data", 5000, NULL, 3, &send_ecg);
    xTaskCreate(send_mpu_data, "Send MPU Data", 5000, NULL, 3, &send_mpu);
    xTaskCreate(send_rest_data, "Send Battery, BPM and AI Calculation Data", 5000, NULL, 3, &send_rest);

    cond.setup();
    
    vTaskDelete(NULL);
}

void loop(){}

#include <Arduino.h>
#line 1 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
#include "driver/rtc_io.h"
#include "indicator.h"
#include "sensor.h"
#include "ble.h"

#define ANALOG_PIN  39
#define RGB_PIN     26
#define BUZZER_PIN  16
#define LED_PIN     23
#define BUTTON_PIN  GPIO_NUM_27

TaskHandle_t ecg_task = NULL;
TaskHandle_t mpu_task = NULL;
TaskHandle_t bt_task = NULL;
TaskHandle_t sleep_check_task = NULL;

unsigned long sleep_threshold = 15000;
unsigned long lastConnect = 0;

indicator cond(RGB_PIN, BUZZER_PIN, LED_PIN);
bluetooth bt;
sensor sens;

#line 24 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void ecg_routine(void *pvParameter);
#line 36 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void mpu_routine(void *pvParameter);
#line 45 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void bt_routine(void *pvParameter);
#line 63 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void sleep_check_routine(void *pvParameter);
#line 74 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void wakeup_reason();
#line 98 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void setup_bt(void *pvParameter);
#line 108 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void setup_peripherals(void *pvParameter);
#line 116 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void setup();
#line 134 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void loop();
#line 24 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
void ecg_routine (void *pvParameter)
{
    while(1){
        int ecgValue = analogRead(ANALOG_PIN);
        cond.set_led(ecgValue);
        bt.send_ecg(ecgValue);
        bt.send_bpm(sens.get_bpm(ecgValue));

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

void mpu_routine (void *pvParameter)
{
    while(1){        
        bt.send_position(sens.get_position(123.456));

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void bt_routine (void *pvParameter)
{
    while(1){
        if(bt.status()){
            vTaskResume(ecg_task);
            vTaskResume(mpu_task);
            vTaskSuspend(sleep_check_task);
        }   else{
            lastConnect = millis();
            vTaskSuspend(ecg_task);
            vTaskSuspend(mpu_task);
            vTaskResume(sleep_check_task);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sleep_check_routine (void *pvParameter)
{
    while(1){
        if(millis() - lastConnect > sleep_threshold){
            esp_deep_sleep_start();
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void wakeup_reason()
{
    rtc_gpio_pulldown_en(BUTTON_PIN);
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

void setup_bt (void *pvParameter)
{
    vTaskSuspendAll();
    {
        bt.init();
    }
    xTaskResumeAll();
    vTaskDelete(NULL);
}

void setup_peripherals (void *pvParameter)
{
    cond.init();
    vTaskDelay(10);
    cond.setup();
    vTaskDelete(NULL);
}

void setup()
{
    Serial.begin(115200);

    xTaskCreate(setup_peripherals, "Setup RGB LED, Buzzer and PWM LED", 2000, NULL, 2, NULL);

    wakeup_reason();

    xTaskCreate(setup_bt, "Setup BLE Connection", 5000, NULL, 4, NULL);

    xTaskCreate(bt_routine, "Check Bluetooth Connection", 5000, NULL, 3, &bt_task);
    xTaskCreate(ecg_routine, "Send ECG, BPM and Ai Calculation Data", 10000, NULL, 4, &ecg_task);
    xTaskCreate(mpu_routine, "Send MPU Position Data", 5000, NULL, 3, &mpu_task);
    xTaskCreate(sleep_check_routine, "Check Sleep Condition", 5000, NULL, 2, &sleep_check_task);

    vTaskDelete(NULL);
}

void loop(){}

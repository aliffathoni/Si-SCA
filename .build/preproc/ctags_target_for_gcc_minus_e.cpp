# 1 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
# 2 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 2
# 3 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 2
# 4 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 2
# 5 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 2







TaskHandle_t ecg_task = 
# 12 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                       __null
# 12 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                           ;
TaskHandle_t mpu_task = 
# 13 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                       __null
# 13 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                           ;
TaskHandle_t bt_task = 
# 14 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                      __null
# 14 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                          ;
TaskHandle_t sleep_check_task = 
# 15 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                               __null
# 15 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                   ;

unsigned long sleep_threshold = 15000;
unsigned long lastConnect = 0;

indicator cond(26, 16, 23);
bluetooth bt;
sensor sens;

void ecg_routine (void *pvParameter)
{
    while(1){
        int ecgValue = analogRead(39);
        cond.set_led(ecgValue);
        bt.send_ecg(ecgValue);
        bt.send_bpm(sens.get_bpm(ecgValue));

        vTaskDelay(20 / ( ( TickType_t ) 1000 / ( 
# 32 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                       1000 
# 32 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                       ) ));
    }
}

void mpu_routine (void *pvParameter)
{
    while(1){
        bt.send_position(sens.get_position(123.456));

        vTaskDelay(200 / ( ( TickType_t ) 1000 / ( 
# 41 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                        1000 
# 41 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                        ) ));
    }
}

void bt_routine (void *pvParameter)
{
    while(1){
        if(bt.status()){
            vTaskResume(ecg_task);
            vTaskResume(mpu_task);
            vTaskSuspend(sleep_check_task);
        } else{
            lastConnect = millis();
            vTaskSuspend(ecg_task);
            vTaskSuspend(mpu_task);
            vTaskResume(sleep_check_task);
        }

        vTaskDelay(1000 / ( ( TickType_t ) 1000 / ( 
# 59 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                         1000 
# 59 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                         ) ));
    }
}

void sleep_check_routine (void *pvParameter)
{
    while(1){
        if(millis() - lastConnect > sleep_threshold){
            esp_deep_sleep_start();
        }

        vTaskDelay(1000 / ( ( TickType_t ) 1000 / ( 
# 70 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                         1000 
# 70 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                         ) ));
    }
}

void wakeup_reason()
{
    rtc_gpio_pulldown_en(GPIO_NUM_27);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
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
    vTaskDelete(
# 105 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
               __null
# 105 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                   );
}

void setup_peripherals (void *pvParameter)
{
    cond.init();
    vTaskDelay(10);
    cond.setup();
    vTaskDelete(
# 113 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
               __null
# 113 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                   );
}

void setup()
{
    Serial.begin(115200);

    xTaskCreate(setup_peripherals, "Setup RGB LED, Buzzer and PWM LED", 2000, 
# 120 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                                             __null
# 120 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                                 , 2, 
# 120 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                                                      __null
# 120 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                                          );

    wakeup_reason();

    xTaskCreate(setup_bt, "Setup BLE Connection", 5000, 
# 124 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                       __null
# 124 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                           , 4, 
# 124 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                                __null
# 124 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                    );

    xTaskCreate(bt_routine, "Check Bluetooth Connection", 5000, 
# 126 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                               __null
# 126 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                   , 3, &bt_task);
    xTaskCreate(ecg_routine, "Send ECG, BPM and Ai Calculation Data", 10000, 
# 127 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                                            __null
# 127 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                                , 4, &ecg_task);
    xTaskCreate(mpu_routine, "Send MPU Position Data", 5000, 
# 128 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                            __null
# 128 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                , 3, &mpu_task);
    xTaskCreate(sleep_check_routine, "Check Sleep Condition", 5000, 
# 129 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
                                                                   __null
# 129 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                                                                       , 2, &sleep_check_task);

    vTaskDelete(
# 131 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino" 3 4
               __null
# 131 "c:\\Users\\AkaY\\Desktop\\Si-SCA\\src\\src.ino"
                   );
}

void loop(){}

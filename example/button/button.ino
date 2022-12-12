#include "driver/rtc_io.h"

void setup(){
    Serial.begin(115200);
    rtc_gpio_pulldown_dis(GPIO_NUM_12);
    rtc_gpio_pullup_en(GPIO_NUM_12);
    
    Serial.println("st");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 0);
    delay(1000);
    esp_deep_sleep_start();
}

void loop(){
  Serial.println(rtc_gpio_get_level(GPIO_NUM_12));
  delay(100);
}
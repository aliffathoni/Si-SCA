#include "indicator.h"

Adafruit_NeoPixel pixels(1, 26, NEO_GRB + NEO_KHZ800);

indicator::indicator(uint8_t pinRGB, uint8_t pinBuzzer, uint8_t pinLED)
{
    _pinRGB = pinRGB;
    _pinBuzzer = pinBuzzer;
    _pinLED = pinLED;
}

indicator::~indicator(){}

void indicator::init()
{
    ledcSetup(0, 5000, 12);
    ledcAttachPin(_pinBuzzer, 0);
    ledcSetup(1, 12000, 10);
    ledcAttachPin(_pinLED, 1);
    
    pixels.begin();
    pixels.setBrightness(128);
    pixels.setPixelColor(0, pixels.Color(0, 120, 0));
    pixels.show();
}

void indicator::set_led(int duty_cycle)
{
    int y = map(duty_cycle, 0, 4095, 0, 1023);
    ledcWrite(1, duty_cycle);
}

void indicator::sleep()
{
    pixels.setPixelColor(0, 0, 0, 128);
    pixels.show();
    vTaskDelay(100);
    pixels.clear();
    pixels.show();
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 128, 0);
    pixels.show();
    vTaskDelay(150);
    pixels.clear();
    pixels.show();
}

void indicator::warning()
{
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(200);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(200);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(200);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(200);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(400);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(400);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(400);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(200);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(200);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(200);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 128, 0, 0);
    pixels.show();
    ledcWriteTone(0, 2048);
    vTaskDelay(200);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
}

void indicator::disconnected()
{
    pixels.setPixelColor(0, 0, 128, 0);
    pixels.show();
    vTaskDelay(100);
    
    pixels.clear();
    pixels.show();
    vTaskDelay(50);

    pixels.setPixelColor(0, 0, 128, 0);
    pixels.show();
    vTaskDelay(100);
    
    pixels.clear();
    pixels.show();
    vTaskDelay(50);
}

void indicator::connected()
{
    pixels.setPixelColor(0, 0, 0, 128);
    pixels.show();
    vTaskDelay(100);
    
    pixels.clear();
    pixels.show();
    vTaskDelay(50);

    pixels.setPixelColor(0, 0, 0, 128);
    pixels.show();
    vTaskDelay(100);
    
    pixels.clear();
    pixels.show();
    vTaskDelay(50);
}

void indicator::connected_indicator()
{
    ledcWriteTone(0, 512);
    vTaskDelay(100);
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 0, 128);
    pixels.show();    
    ledcWriteTone(0, 3072);
    vTaskDelay(100);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 128, 0);
    pixels.show();    
    ledcWriteTone(0, 4096);
    vTaskDelay(150);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
}

void indicator::disconnected_indicator()
{
    ledcWriteTone(0, 4096);
    vTaskDelay(100);
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 128, 0);
    pixels.show();    
    ledcWriteTone(0, 1024);
    vTaskDelay(100);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 0, 128);
    pixels.show();
    ledcWriteTone(0, 512);
    vTaskDelay(150);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
}

void indicator::setup()
{
    ledcWriteTone(0, 512);
    vTaskDelay(100);
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 0, 128);
    pixels.show();    
    ledcWriteTone(0, 3072);
    vTaskDelay(100);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, 0, 128, 0);
    pixels.show();    
    ledcWriteTone(0, 1024);
    vTaskDelay(150);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
    vTaskDelay(50);
    pixels.setPixelColor(0, pixels.Color(227, 120, 3));
    pixels.show();    
    ledcWriteTone(0, 4096);
    vTaskDelay(150);
    pixels.clear();
    pixels.show();
    ledcWriteTone(0, 0);
}
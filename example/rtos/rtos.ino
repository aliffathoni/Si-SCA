#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <FastLED.h>

#include <TaskScheduler.h>

TaskHandle_t xHandle = NULL;

BLEServer* pServer = NULL;
BLECharacteristic* HeartRateCharacteristic = NULL;
BLECharacteristic* BattMonCharacteristic = NULL;
BLECharacteristic* ECGCharacteristic = NULL;
BLECharacteristic* ECG2Characteristic = NULL;
BLECharacteristic* CondCharacteristic = NULL;

// #define HR_SERVICE_UUID        "0000180d-0000-1000-8000-00805f9b34fb"
// #define HR_CHARACTERISTIC_UUID "00002a37-0000-1000-8000-00805f9b34fb"

static BLEUUID HR_SERVICE_UUID(BLEUUID((uint16_t)0x180D));
static BLEUUID HR_CHARACTERISTIC_UUID(BLEUUID((uint16_t)0x2A37));

// #define BATT_UUID              "0000180f-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a19-0000-1000-8000-00805f9b34fb"

static BLEUUID BATT_UUID(BLEUUID((uint16_t)0x180F));
static BLEUUID BATT_C_UUID(BLEUUID((uint16_t)0x2A19));

// #define BATT_UUID              "0000183e-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "00002a58-0000-1000-8000-00805f9b34fb"

static BLEUUID ECG_UUID(BLEUUID((uint16_t)0x183E));
static BLEUUID ECG_C_UUID(BLEUUID((uint16_t)0x2A58));

// #define BATT_UUID              "0000181a-0000-1000-8000-00805f9b34fb"
// #define BATT_C_UUID            "0000290c-0000-1000-8000-00805f9b34fb"

static BLEUUID COND_UUID(BLEUUID((uint16_t)0x181A));
static BLEUUID COND_C_UUID(BLEUUID((uint16_t)0x290C));

// #define ECG2_UUID              "00001813-0000-1000-8000-00805f9b34fb"
// #define ECG2_C_UUID            "00002a58-0000-1000-8000-00805f9b34fb"

static BLEUUID ECG2_UUID(BLEUUID((uint16_t)0x1813));
static BLEUUID ECG2_C_UUID(BLEUUID((uint16_t)0x2A58));

bool deviceConnected = false;
bool oldDeviceConnected = false;

unsigned long checkPoint;

CRGB leds[1];

void blinkBlue(){
  leds[0]=CRGB::Blue;
  FastLED.setBrightness(64);
  FastLED.show();
}

void blinkGreen(){
  leds[0]=CRGB::Green;
  FastLED.setBrightness(64);
  FastLED.show();
}

void blinkRed(){
  leds[0]=CRGB::Red;
  FastLED.setBrightness(64);
  FastLED.show();
}

void blinkYellow(){
  leds[0]=CRGB::Yellow;
  FastLED.setBrightness(4);
  FastLED.show();
}

void blinkNothing(){
  leds[0]=CRGB::White;
  FastLED.setBrightness(0);
  FastLED.show();
}

void blink(void *pvParameter)
{
  for(;;){
    if(deviceConnected){
      blinkBlue();
      vTaskDelay(50 / portTICK_PERIOD_MS);
      blinkNothing();
      vTaskDelay(50 / portTICK_PERIOD_MS);
      blinkGreen();
      vTaskDelay(50 / portTICK_PERIOD_MS);
      blinkNothing();
    } else{
      blinkBlue();
      vTaskDelay(250 / portTICK_PERIOD_MS);
      blinkNothing();
    }
      vTaskDelay(2500 / portTICK_PERIOD_MS);
  }
}

void sendHR(){
  if (deviceConnected) {
    int x = random(50, 150);
    String a = String(x);
    HeartRateCharacteristic->setValue(a.c_str());
    HeartRateCharacteristic->notify();
    Serial.println(x);
  }
}

void sendECG(){
  if (deviceConnected) {
    int x = random(50, 150);
    String a = String(x);
    ECGCharacteristic->setValue(a.c_str());
    ECGCharacteristic->notify();
    ECG2Characteristic->setValue(a.c_str());
    ECG2Characteristic->notify();
    Serial.println(x);
  }
}

void sendCond(){
  if (deviceConnected) {
    String condition[3] = {"FALL", "SLEEP", "STAND"};
    int m = random(0,2);
    String cc = condition[m];
    CondCharacteristic->setValue(cc.c_str());
    CondCharacteristic->notify();
  }
}

void sendBatt(){
  if (deviceConnected) {
    int x = random(1, 99);
    String a = String(x);
    BattMonCharacteristic->setValue(a.c_str());
    BattMonCharacteristic->notify();
    Serial.print("Battery : ");
    Serial.println(x);
  }
}

void sleepCheck();

Task send_ecg(10, TASK_FOREVER, &sendECG);
Task send_batt(1000, TASK_FOREVER, &sendBatt);
Task send_cond(1000, TASK_FOREVER, &sendCond);
Task send_hr(1000, TASK_FOREVER, &sendHR);
Task sleep_check(100, TASK_FOREVER, &sleepCheck);

Scheduler sch;

void sleepCheck(){
  if(millis() - checkPoint > 10000){
    sleep_check.disable();
    sleepPrep();
    esp_sleep_enable_touchpad_wakeup();
    esp_deep_sleep_start();
  }
}

void sleepPrep(){
  send_ecg.disable();
  send_batt.disable();
  send_cond.disable();
  send_hr.disable();
  blinkYellow();
}

void wake(){
  send_ecg.enable();
  send_batt.enable();
  send_cond.enable();
  send_hr.enable();
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device Connected!");
      sleep_check.disable();
    }

    void onDisconnect(BLEServer* pServer) {
      checkPoint = millis();
      deviceConnected = false;
      Serial.println("Device Disconnected!");
      pServer->startAdvertising();
      sleep_check.enable();
    }
};

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812, 15, GRB>(leds, 1);
  blinkRed();
    
  BLEDevice::init("Sisca");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *HeartRateService = pServer->createService(HR_SERVICE_UUID);

  // Create a BLE Characteristic
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

  ECG2Characteristic = ECGService->createCharacteristic(
                      ECG2_C_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  ECG2Characteristic->addDescriptor(new BLE2902());

  // Start the service
  HeartRateService->start();
  BattMonService->start();
  ECGService->start();
  CondService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(HR_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();

  blinkNothing();

  xTaskCreate(blink,"Blink Task",1024,NULL,2,&xHandle);

  touchAttachInterrupt(T2, wake, 40);

  sch.init();
  sch.addTask(send_ecg);
  send_ecg.enable();
  sch.addTask(send_batt);
  send_batt.enable();
  sch.addTask(send_cond);
  send_cond.enable();
  sch.addTask(send_hr);
  send_hr.enable();
  sch.addTask(sleep_check);
  checkPoint = millis();
  sleep_check.enable();
}

void loop() {sch.execute();}
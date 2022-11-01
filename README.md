
# Si-SCA Project

[![UPDATE](https://img.shields.io/github/last-commit/aliffathoni/Si-SCA)](https://github.com/aliffathoni/Si-SCA)
[![ARDUINO](https://img.shields.io/badge/ArduinoIDE-blue)](https://www.arduino.cc/en/software/)
[![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)

Si-SCA is a project to monitor heart rate and gyro status using Bluetooth Low Energy devices based on ESP32. This repository contain firmware for the devices to be installed.

## Library Used

 - [Adafruit MPU 6050](https://github.com/adafruit/Adafruit_MPU6050)
 - [NimBLE](https://github.com/h2zero/esp-nimble-cpp)
 - [RGB LED](https://github.com/FastLED/FastLED)
 - [FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
 - [TaskScheduler](https://github.com/arkhipenko/TaskScheduler)

## To-Do-List

- [X] ~~Establish Bluetooth Connection~~
- [X] ~~AD8232 HR Sensor Research~~
- [X] ~~Validate and Calibrate HR data~~
- [X] ~~MPU6050 get fixed Gyro data~~
- [X] ~~Validate and Calibrate Gyro data~~
- [X] ~~Get Battery Percentage Level~~
- [X] ~~Update BLE using NimBLE~~
- [X] ~~Add RGB Indicator~~
- [X] ~~Add Buzzer Indicator~~
- [X] ~~Configure Sleep Mode~~
- [X] ~~Import RTOS for better timing management~~
- [ ] Add AI for ECG signal detector

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details.

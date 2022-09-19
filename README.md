
# Si-SCA Project

[![UPDATE](https://img.shields.io/github/last-commit/AkagamiYozora/Si-SCA)](https://github.com/AkagamiYozora/Si-SCA)

[![ARDUINO](https://img.shields.io/badge/ArduinoIDE-blue)](https://www.arduino.cc/en/software/)
[![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)

Si-SCA is a project to monitor heart rate and gyro status using Bluetooth Low Energy devices based on ESP32. This repository contain software for the devices to be installed.

## Library Used

 - [Adafruit MPU 6050](https://github.com/adafruit/Adafruit_MPU6050)
 - [MAX30003 HR Sensor](https://github.com/Protocentral/protocentral_max30003/)
 - [NimBLE](https://github.com/h2zero/esp-nimble-cpp)

## To-Do-List

- [X] Establish Bluetooth Connection
- [X] MAX30003 get HR data
- [ ] Validate and Calibrate HR data
- [X] MPU6050 get fixed Gyro data
- [ ] Validate and Calibrate Gyro data
- [ ] Get Battery Percentage Level
- [ ] Update BLE using NimBLE
- [ ] Add RGB Indicator
- [ ] Add Charging Control
- [X] Add Buzzer Indicator

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details.

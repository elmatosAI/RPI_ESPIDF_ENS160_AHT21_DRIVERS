﻿# Air Quality and Temperature Monitoring with Raspberry Pi, WiP

This project is a professional demonstration of interfacing an ESP32 microcontroller with environmental sensors to monitor air quality and climate conditions. The application provides insights into real-time data acquisition using the I2C communication protocol and highlights skills in embedded systems development and hardware integration.

## Table of Contents

1. [Project Summary](#project-summary)
2. [Key Contributions](#key-contributions)
3. [Features](#features)
4. [Technical Skills Demonstrated](#technical-skills-demonstrated)
5. [Hardware Requirements](#hardware-requirements)
6. [Software Requirements](#software-requirements)
7. [Installation and Setup](#installation-and-setup)
8. [Code Overview](#code-overview)
9. [Additional Information](#additional-information)

---

## Project Summary

This project showcases the ability to design and implement an embedded system capable of monitoring environmental metrics such as:
- Air quality (eCO2, TVOC, AQI)
- Temperature
- Relative Humidity

The system leverages the ESP32 development board and demonstrates advanced use of the I2C protocol for sensor communication.

## Key Contributions

- Designed and implemented a fully functional embedded system for environmental monitoring.
- Developed custom drivers for ENS160 and AHT21 sensors.
- Optimized sensor integration using shared I2C communication lines.
- Demonstrated proficiency in FreeRTOS task scheduling for real-time data processing.
- Created a robust and modular codebase suitable for future extensions.

## Features

- **Real-time monitoring**: Logs air quality, temperature, and humidity data in real-time.
- **Accurate sensor interfacing**: Interfaced with ENS160 and AHT21 sensors using I2C.
- **Embedded systems expertise**: Designed software for ESP32 using the ESP-IDF framework.

## Technical Skills Demonstrated

- **Embedded C programming**: Implemented modular, well-documented drivers for environmental sensors.
- **Microcontroller expertise**: Configured and utilized ESP32 for low-level I2C communication.
- **Hardware-software integration**: Integrated multiple sensors on shared communication lines.
- **Debugging and optimization**: Used serial monitoring tools to debug and optimize system performance.
- **Project organization**: Structured codebase and provided comprehensive documentation.

## Hardware Requirements

- ESP32 Development Board
- ENS160 Air Quality Sensor
- AHT21 Temperature and Humidity Sensor
- Pull-up resistors (4.7kΩ for I2C lines)
- Breadboard and connecting wires

### Pin Connections
| ENS160 Pin  | ESP32 Pin |
|-------------|-----------|
| SDA         | GPIO8     |
| SCL         | GPIO9     |

| AHT21 Pin   | ESP32 Pin |
|-------------|-----------|
| SDA         | GPIO8     |
| SCL         | GPIO9     |

## Software Requirements

- [ESP-IDF](https://github.com/espressif/esp-idf) (ESP32 development framework)
- Serial monitor tools (e.g., `minicom`, `putty`, or the ESP-IDF monitor tool)

## Installation and Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/PWawrow/ESP32_ESPIDF_ENS160_AHT21_DRIVERS.git
   cd esp32-air-quality-monitor
   ```
2. Configure the ESP-IDF environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```
3. Build the project:
   ```bash
   idf.py build
   ```
4. Flash the firmware to your ESP32:
   ```bash
   idf.py -p /dev/ttyUSB0 flash
   ```
5. Monitor the serial output:
   ```bash
   idf.py monitor
   ```

## Code Overview

### Key Files

- **`main.c`**: Initializes the I2C bus, configures sensors, and manages the FreeRTOS task for data acquisition.
- **`ens160.c` / `ens160.h`**: Driver for the ENS160 sensor, including functions for eCO2, TVOC, and AQI readings.
- **`aht21.c` / `aht21.h`**: Driver for the AHT21 sensor, providing temperature and humidity measurements.

### Task Workflow

The main FreeRTOS task operates as follows:
1. Initiates measurements from the AHT21 sensor.
2. Reads temperature and relative humidity values.
3. Gathers air quality data (eCO2, TVOC, AQI) from the ENS160 sensor.
4. Logs data in a structured format via the serial console.

## Additional Information

### Potential Applications

- Indoor air quality monitoring for smart homes or offices.
- Educational projects demonstrating sensor integration and embedded systems design.
- Development base for IoT-enabled air quality monitoring systems.


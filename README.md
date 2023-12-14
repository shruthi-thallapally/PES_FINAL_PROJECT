# PES_FINAL_PROJECT
# FRDM-KL25Z BMP280 Sensor Interface

## Project Overview
This project demonstrates how to interface the GY-BMP280 sensor with the FRDM-KL25Z microcontroller to read atmospheric pressure and temperature data using the I2C communication protocol.

## Features
- Read temperature and pressure data from the BMP280 sensor.
- Process and calibrate raw data from the sensor for accurate readings.
- Implement I2C communication protocol on FRDM-KL25Z.

## Setup and Configuration
- Connect the BMP280 sensor to the FRDM-KL25Z I2C pins.
- Ensure appropriate pull-up resistors are in place for I2C communication.

## Usage
1. Initialize the I2C communication.
2. Configure the BMP280 sensor.
3. Read and process the temperature and pressure data.

## Code Structure
- `i2c.h` & `i2c.c`: Contains I2C communication setup and functions.
- `bmp280.h` & `bmp280.c`: Contains BMP280 specific configurations and functions.
- `main.c`: Main program demonstrating sensor data reading.
- `cbfifo.c` & `cbfifo.h`: Circular Buffer setup and functions for UART implementation
- `cmd_process.h` & `cmd_process.c`: User Interface to read temperature when user enters “TEMP” and pressure when user enters “PRESSURE”
- `sysclock.h` & `sysclock.c`: Initializes the system clock.
- `uart0.h` & `uart0.c`: Contains UART specific configurations and functions to print values on the terminal.

## Acknowledgements
Took reference from the BMP280 datasheet for calibration and measurement of temperature and pressure. Referred to the Dean’s code for I2C protocol implementation.

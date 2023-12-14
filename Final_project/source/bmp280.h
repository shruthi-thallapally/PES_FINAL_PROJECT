/*
 * pressure.h
 *
 *  Created on: 12-Dec-2023
 *      Author: Shruthi Thallapally
 *   Reference: Referrred the BMP280 datasheet for calibration and measurement
 */

#ifndef BMP280_H_
#define BMP280_H_

extern signed long temperature_raw, pressure_raw;
extern unsigned short dig_T1, dig_P1;
extern signed short dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
extern float temperature, pressure, altitude, init_height;

/**
 * @brief Initializes the BMP280 sensor.
 *        Configures the BMP280 sensor's control and config registers and reads its calibration data.
 * @param None
 * @return void
 */
void BMP280_init(void);

/**
 * @brief Reads and calculates temperature and pressure values from BMP280.
 *        Gathers raw temperature and pressure data from the BMP280 sensor and computes
 *        the actual values using calibration data. It then prints these values to the console.
 * @param None
 * @return void
 */
void BMP280_values();



#endif /* BMP280_H_ */

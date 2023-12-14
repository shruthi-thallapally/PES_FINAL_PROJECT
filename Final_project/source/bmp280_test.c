/*
 * bmp280_test.c
 *
 *  Created on: 13-Dec-2023
 *      Author: Shruthi Thallapally
 */
#include "bmp280_test.h"
#include "bmp280.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"

#define ACC_TEMP_DEVIATION 5.00
#define ACC_PRESS_DEVIATION 10


void test_data_accuracy_temp() {
    // Known reference values for temperature and pressure
    float known_temp = 22.01;


    // Read sensor values
    float sensor_temp = temperature;


    // Compare sensor values with known values
    if (abs(sensor_temp - known_temp) <= ACC_TEMP_DEVIATION) {
        printf("\n\rTemperature within acceptable range\n\r");
    } else {
        printf("\n\rTemperature deviation too high\n\r");
    }

}

void test_data_accuracy_press() {
    // Known reference values for temperature and pressure

    float known_pressure = 99345.33;

    // Read sensor values

    float sensor_pressure = pressure;

    // Compare sensor values with known values

    if (abs(sensor_pressure - known_pressure) <=ACC_PRESS_DEVIATION) {
        printf("\n\rPressure within acceptable range\n\r");
    } else {
        printf("\n\rPressure deviation too high\n\r");
    }
}

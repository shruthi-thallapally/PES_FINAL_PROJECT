/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    Final_project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "bmp280.h"
#include "i2c.h"
#include "uart0.h"
#include "sysclock.h"
#include "bmp280.h"
#include "bmp280_test.h"
#include "cmd_process.h"


/**
 * @brief Main program to initialize and run BMP280 sensor data reading.
 *
 * Initializes system clock, UART communication, and sets up UART interrupt handling.
 * Continuously executes commands and reads sensor data in a loop.
 *
 * @return int Returns 0 upon completion.
 */
int main(void) {

	// Initialize the system clock
	sysclock_init();

	//initialize UART communication
	init_UART0();

	//set interrupt priority for UART, clear all pending interrupts and enable interrupt request
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Initial message over UART
	printf("Final project:GY-BMP280 \r\n");
	BMP280_init();
	// Initialize I2C communication
    i2c_init();

    // Main loop
    while(1) {
    	BMP280_values();
    	command_execution();




    }
    return 0 ;
}

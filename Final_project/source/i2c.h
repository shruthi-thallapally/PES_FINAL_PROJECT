/*
 * i2c.h
 *
 *  Created on: 12-Dec-2023
 *      Author: Shruthi Thallapally
 *   Reference: Referred the dean's code
 */

#ifndef I2C_H_
#define I2C_H_
#include "stdint.h"

/**
 * @brief Initializes the I2C1 interface.
 *        Sets up the I2C1 interface for communication, including configuring the SDA and SCL pins,
 *        setting the baud rate, enabling the I2C module, and selecting the high drive mode.
 * @param None
 * @return void
 */
void i2c_init();
/**
 * @brief Reads a byte of data from a specified register of an I2C device.
 * @param device_adr The I2C address of the device.
 * @param internal_adr The register address within the device to read.
 * @return The byte of data read from the device.
 */
uint8_t I2C_Read_Register(uint8_t device_adr, uint8_t internal_adr);
/**
 * @brief Writes a byte of data to a specified register of an I2C device.
 * @param device_adr The I2C address of the device.
 * @param internal_adr The register address within the device to write.
 * @param data The byte of data to write to the device.
 * @return void
 */
void I2C_Write_Register(uint8_t device_adr, uint8_t internal_adr, uint8_t data);

#endif /* I2C_H_ */

/*
* i2c.c
*
* Created on: 12-Dec-2023
*     Author: Shruthi Thallapally
*  Reference: Referred the dean's code
*/
#include "i2c.h"
#include "MKL25Z4.h"

/**
 * @brief Initializes the I2C1 interface.
 *        Sets up the I2C1 interface for communication, including configuring the SDA and SCL pins,
 *        setting the baud rate, enabling the I2C module, and selecting the high drive mode.
 * @param None
 * @return void
 */
void i2c_init() {
	// Enable clock for I2C1 and Port E
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	// Configure I2C1 SDA and SCL pins
	PORTE->PCR[0] |= PORT_PCR_MUX(6); // PTE0 as I2C1 SDA
	PORTE->PCR[1] |= PORT_PCR_MUX(6); // PTE1 as I2C1 SCL
	// Set I2C1 to master mode and set baud rate
	// Setting the frequency divider register.
	// I2C1->F = I2C_F_ICR(0x11) | //The SCL divider multiplied by multiplier factor (mul) determines the I2C baud rate.
	// I2C_F_MULT(0); // Multiplier factor = 1
	I2C1->F = (I2C_F_ICR(0x00) | I2C_F_MULT(2));
	// Clearing bits of C1 register
	I2C1->C1 = 0;
	I2C1->C1 |= I2C_C1_IICEN_MASK; // Enable I2C1 module
	I2C1->C1 |= I2C_C1_IICIE_MASK;
	//selecting High drive mode
	I2C1->C2 |= (I2C_C2_HDRS_MASK);
}

/**
 * @brief Reads a byte of data from a specified register of an I2C device.
 * @param device_adr The I2C address of the device.
 * @param internal_adr The register address within the device to read.
 * @return The byte of data read from the device.
 */
uint8_t I2C_Read_Register(uint8_t device_adr, uint8_t internal_adr) {
	// Start I2C transmission in write mode
	I2C1->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;
	// Write device address
	I2C1->D = device_adr;
	while (!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	// Write internal register address
	I2C1->D = internal_adr;
	while (!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	// Repeated start in read mode
	I2C1->C1 |= I2C_C1_RSTA_MASK;
	I2C1->D = device_adr | 0x01;
	while (!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	// Switch to receiver mode and perform a dummy read
	I2C1->C1 &= ~I2C_C1_TX_MASK;
	uint8_t data = I2C1->D;
	while (!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	// Send NACK and read the byte
	I2C1->C1 |= I2C_C1_TXAK_MASK;
	data = I2C1->D;
	// Stop I2C transmission
	I2C1->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK);
	return data;
}

/**
 * @brief Writes a byte of data to a specified register of an I2C device.
 * @param device_adr The I2C address of the device.
 * @param internal_adr The register address within the device to write.
 * @param data The byte of data to write to the device.
 * @return void
 */
void I2C_Write_Register(uint8_t device_adr, uint8_t internal_adr, uint8_t data) {
	// Start I2C transmission in write mode
	I2C1->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;
	// Write device address
	I2C1->D = device_adr;
	while ((I2C1->S & I2C_S_IICIF_MASK)==0);
	I2C1->S |= I2C_S_IICIF_MASK;
	// Write internal register address
	I2C1->D = internal_adr;
	while ((I2C1->S & I2C_S_IICIF_MASK)==0);
	// while(((I2C1->S & I2C_S_IICIF_MASK)==0) || ((I2C1->S & I2C_S_TCF_MASK) == 0));
	I2C1->S |= I2C_S_IICIF_MASK;
	// Write data
	I2C1->D = data;
	while (!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	// Stop I2C transmission
	I2C1->C1 &= ~I2C_C1_MST_MASK;
}

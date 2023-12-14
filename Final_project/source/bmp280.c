/*
 * bmp280.c
 *
 *  Created on: 12-Dec-2023
 *      Author: Shruthi Thallapally
 *  References: Referrred the BMP280 datasheet for calibration and measurement
 */
#include "stdint.h"
#include "stdio.h"
#include "bmp280.h"
#include "i2c.h"

#define BMP280_CTRL_MEAS_REG 0xF4
#define BMP280_CONFIG_REG 0xF5
#define BMP280_CALIB_DATA_REG 0x88
#define BMP280_dev_address 0x76
#define BMP280_READ ((0x76<<1)|0X01)
#define BMP280_WRITE (0x76<<1)

signed long temperature_raw, pressure_raw;
unsigned short dig_T1, dig_P1;
signed short dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
float temperature, pressure;

/**
 * @brief Initializes the BMP280 sensor.
 *        Configures the BMP280 sensor's control and config registers and reads its calibration data.
 * @param None
 * @return void
 */
void BMP280_init(void) {
    // Write to the control register to set the temperature and pressure measurement modes
	I2C_Write_Register(BMP280_WRITE, 0xF4, 0x3F); // osrs_t 010 x2, osrs_p 16 101, mode normal 11

    // Write to the config register to set the standby time, filter, and SPI mode of BMP280
	I2C_Write_Register(BMP280_WRITE, BMP280_CONFIG_REG, 0x90); // standby time 500ms 100, filter 16 100, SPI DIS 00

    // Read calibration data from BMP280
    uint8_t calib[24];
    for (int i = 0; i < 24; i++) {
        calib[i] = I2C_Read_Register(BMP280_READ, BMP280_CALIB_DATA_REG + i);
    }

    dig_T1=(calib[0])+(calib[1]<<8);
    dig_T2=(calib[2])+(calib[3]<<8);
    dig_T3=(calib[4])+(calib[5]<<8);
    dig_P1=(calib[6])+(calib[7]<<8);
    dig_P2=(calib[8])+(calib[9]<<8);
    dig_P3=(calib[10])+(calib[11]<<8);
    dig_P4=(calib[12])+(calib[13]<<8);
    dig_P5=(calib[14])+(calib[15]<<8);
    dig_P6=(calib[16])+(calib[17]<<8);
    dig_P7=(calib[18])+(calib[19]<<8);
    dig_P8=(calib[20])+(calib[21]<<8);
    dig_P9=(calib[22])+(calib[23]<<8);
}

/**
 * @brief Reads and calculates temperature and pressure values from BMP280.
 *        Gathers raw temperature and pressure data from the BMP280 sensor and computes
 *        the actual values using calibration data. It then prints these values to the console.
 * @param None
 * @return void
 */
void BMP280_values()
{
	uint8_t status, buff[6];
	// Wait for data to be ready
	do
	{
		status=I2C_Read_Register(BMP280_READ, 0xF3);
	} while(((status&0b00001000)==8)||((status&0b00000001)==1));

	 // Read the raw pressure and temperature data
	for (int i = 0; i < 6; i++) {
		 buff[i] = I2C_Read_Register(BMP280_READ, 0xF7 + i);
	}

	 // Obtaining the raw temperature data
	volatile uint32_t temp[3];
	temp[2]=buff[3];
	temp[1]=buff[4];
	temp[0]=buff[5];
	temperature_raw=(temp[2]<<12)+(temp[1]<<4)+(temp[0]>>4);

	// Obtaining the raw pressure data
	temp[2]=buff[0];
	temp[1]=buff[1];
	temp[0]=buff[2];
	pressure_raw=(temp[2]<<12)+(temp[1]<<4)+(temp[0]>>4);

	// Calculate the actual temperature and pressure
	double var1, var2;
	var1=(((double)temperature_raw)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
	var2=((((double)temperature_raw)/131072.0-((double)dig_T1)/8192.0)*(((double)temperature_raw)/131072.0-((double)dig_T1)/8192.0))*((double)dig_T3);
	double t_fine = (int32_t)(var1+var2);
	volatile	float T = (var1+var2)/5120.0;

	 // More calculations for pressure
	var1=((double)t_fine/2.0)-64000.0;
	var2=var1*var1*((double)dig_P6)/32768.0;
	var2=var2+var1*((double)dig_P5)*2.0;
	var2=(var2/4.0)+(((double)dig_P4)*65536.0);
	var1=(((double)dig_P3)*var1*var1/524288.0+((double)dig_P2)*var1)/524288.0;
	var1=(1.0+var1/32768.0)*((double)dig_P1);
	volatile	double p=1048576.0-(double)pressure_raw;
	p=(p-(var2/4096.0))*6250.0/var1;
	var1=((double)dig_P9)*p*p/2147483648.0;
	var2=p*((double)dig_P8)/32768.0;
	p=p+(var1+var2+((double)dig_P7))/16.0;

	// Update global variables with calculated values
	temperature=T;
	pressure=p;
	printf("\n\rTemperature=%d\n\r",T);
	printf("\n\rPressure=%d\n\r",p);
}

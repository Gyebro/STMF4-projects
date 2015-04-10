/**
 * MMA845X Utilities for GY-45 breakout board
 * Author: Gergely Gyebroszki
 * Dependencies: I2C drivers (STD variant) with TM I2C library
 */

#ifndef MMA845X_UTILS
#define MMA845X_UTILS

#include "tm_stm32f4_i2c.h"

/* MMA address is 0x1D if SA0 is high, 0x1C if low
   Here the 0x1C must be shifted left, so it will be 0x38 */
#define MMA_ADDRESS			0x38

/* Define a few of the MMA registers */
#define MMA_OUT_X_MSB		0x01
#define MMA_XYZ_DATA_CFG	0x0E
#define MMA_WHO_AM_I   		0x0D
#define MMA_CTRL_REG1  		0x2A

/* Value of WHO_AM_I */
#define MMA_I_AM			0x2A

/* Status and error codes */
#define MMA_OK						0
#define MMA_ERROR_NOT_CONNECTED		1
#define MMA_ERROR_NOT_RECOGNIZED	2

/* Acceleration full scale ranges */
#define MMA_RANGE_2G	0x00
#define MMA_RANGE_4G	0x01
#define MMA_RANGE_8G	0x10

/* Initialize the MMA845X on I2C1 SCL:PB6 and SDA:PB7 */
uint8_t MMA845X_Initialize(uint8_t MMA_RANGE);

/* Read the raw acceleration register values*/
void MMA845X_ReadRawData(uint8_t* rawData);

/* Read the acceleration values and put them into the destination buffer */
void MMA845X_ReadAcceleration(int * destination);

#endif /* MMA845X_UTILS */

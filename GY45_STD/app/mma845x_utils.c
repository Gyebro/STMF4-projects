#include "mma845x_utils.h"

/* Implementation of public functions */

uint8_t MMA845X_Initialize(uint8_t MMA_RANGE) {
	/* Init I2C1 -- SCL: PB6 and SDA: PB7 */
    TM_I2C_Init(I2C1, TM_I2C_PinsPack_1, 100000);
    /* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(I2C1, MMA_ADDRESS)) {
		return MMA_ERROR_NOT_CONNECTED;
	}
	/* Check who I am */
	if (TM_I2C_Read(I2C1, MMA_ADDRESS, MMA_WHO_AM_I) != MMA_I_AM) {
		return MMA_ERROR_NOT_RECOGNIZED;
	}
	/* Standby: Get ctrl register value */
	uint8_t ctrl = TM_I2C_Read(I2C1, MMA_ADDRESS, MMA_CTRL_REG1);
	/* Clear the active bit to go into standby */
	TM_I2C_Write(I2C1, MMA_ADDRESS, MMA_CTRL_REG1, ctrl & ~(0x01));

	/* Write full scale range to DATA_CFG register */
	TM_I2C_Write(I2C1, MMA_ADDRESS, MMA_XYZ_DATA_CFG, MMA_RANGE);

	/* Activate: Get ctrl register again */
	ctrl = TM_I2C_Read(I2C1, MMA_ADDRESS, MMA_CTRL_REG1);
	/* Set the active bit to start measurement */
	TM_I2C_Write(I2C1, MMA_ADDRESS, MMA_CTRL_REG1, ctrl | 0x01);

	return MMA_OK;
}

void MMA845X_ReadAcceleration(int* destination) {
	// Raw x,y,z register data
	uint8_t rawData[6];
	// Read from MMA845X
	TM_I2C_ReadMulti(I2C1, MMA_ADDRESS, MMA_OUT_X_MSB, rawData, 6);

	// Loop to calculate 12-bit ADC and G value for each axis
	volatile int i;
	for(i = 0; i < 3 ; i++) {
		// Combine the two 8 bit registers into one 12-bit number
		int gCount = (rawData[i*2] << 8) | rawData[(i*2)+1];
		// The registers are left aligned, right align the 12-bit integer
		gCount >>= 4;
		// If the number is negative, we have to make it so manually (no 12-bit data type)
		if (rawData[i*2] > 0x7F) {
			gCount = ~gCount + 1;
			gCount *= -1;  // Transform into negative 2's complement #
		}
		destination[i] = gCount; // Store in destination
	}
}



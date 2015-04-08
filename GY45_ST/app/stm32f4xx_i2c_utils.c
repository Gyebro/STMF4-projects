#include "stm32f4xx_i2c_utils.h"

#include "stm32f4xx_gpio_utils.h"
#include "stm32f4xx_hal_i2c.h"

#define CONFIG_I2C_TIMEOUT 1000
#define CONFIG_I2C_TRIALS 200

I2C_HandleTypeDef I2C_Initialize(I2C_TypeDef* I2CX, uint32_t CLOCKSPEED) {
	switch ((int)I2CX) {
	case (int)I2C1:
		// Enable the I2Cx interface clock
		__I2C1_CLK_ENABLE();
		// Enable alternating config for pin pack 1: PB6 (SCL) and PB7 (SDA), Open-Drain, PullUp
		GPIO_Alt_Initialize(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_MEDIUM, GPIO_AF4_I2C1);
	break;
	}
	// Configure handle
	I2C_HandleTypeDef I2C_handle;
	I2C_handle.Instance = I2CX;
	I2C_handle.Init.ClockSpeed = CLOCKSPEED;
	I2C_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C_handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C_handle.Init.OwnAddress1 = 0x00;
	HAL_I2C_Init(&I2C_handle);
	return I2C_handle;
}

uint8_t I2C_Read(I2C_HandleTypeDef* HI2C, uint8_t ADDR, uint8_t REG) {
	uint8_t REC = 0;

	// Transmit the register address
	HAL_I2C_Master_Transmit(HI2C, ADDR, &REG, 1, CONFIG_I2C_TIMEOUT);
	if(HAL_OK == HAL_I2C_IsDeviceReady(HI2C, ADDR, CONFIG_I2C_TRIALS, CONFIG_I2C_TIMEOUT)) {
		// Receive the value
		HAL_I2C_Master_Receive(HI2C, ADDR, &REC, 1, CONFIG_I2C_TIMEOUT);
	}

	return REC;
}

/* PRIVATE FUNCTIONS */

void I2C_Start() {

}

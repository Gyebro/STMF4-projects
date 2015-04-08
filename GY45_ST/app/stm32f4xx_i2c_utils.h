#ifndef STM32F4XX_I2C_UTILS
#define STM32F4XX_I2C_UTILS

/**
 * STM32F4XX I2C (Inter-Integrated Circuit) Utilities
 * Author: Gergely Gyebrószki
 */

#include "stm32f4xx.h"

I2C_HandleTypeDef I2C_Initialize(I2C_TypeDef* I2CX, uint32_t CLOCKSPEED);

uint8_t I2C_Read(I2C_HandleTypeDef* HI2C, uint8_t ADDR, uint8_t REG);

void I2C_Read_Multi(I2C_HandleTypeDef* HI2C, uint8_t ADDR, uint8_t REG, uint8_t *DATA, uint16_t COUNT);

void I2C_Write(I2C_HandleTypeDef* HI2C, uint8_t ADDR, uint8_t REG, uint8_t DATA);

#endif

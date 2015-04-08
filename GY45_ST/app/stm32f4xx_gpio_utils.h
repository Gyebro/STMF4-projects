#ifndef STM32F4XX_GPIO_UTILS
#define STM32F4XX_GPIO_UTILS

/**
 * STM32F4XX GPIO (General Purpose Input/Output) Utilities
 * Author: Gergely Gyebrószki
 */

#include "stm32f4xx.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

void GPIO_Initialize(GPIO_TypeDef * GPIOX, uint32_t PIN, uint32_t MODE, uint32_t PULL, uint32_t SPEED);

void GPIO_Alt_Initialize(GPIO_TypeDef * GPIOX, uint32_t PIN, uint32_t MODE, uint32_t PULL, uint32_t SPEED, uint32_t ALTERNATE);

#endif

#ifndef STM32F4XX_USART_UTILS
#define STM32F4XX_USART_UTILS

/**
 * STM32F4XX USART (Universal synchronous/asynchronous receiver/transmitter) Utilities
 * Author: Gergely Gyebrószki
 */

#include "stm32f4xx.h"

USART_HandleTypeDef USART_Initialize(USART_TypeDef* USARTX, uint32_t BAUDRATE);

HAL_StatusTypeDef USART_PutChar(USART_HandleTypeDef* HUSART, char c);

HAL_StatusTypeDef USART_PutString(USART_HandleTypeDef* HUSART, char* str);

#endif

#ifndef STM32F4XX_USART_UTILS
#define STM32F4XX_USART_UTILS

/**
 * STM32F4XX USART (Universal synchronous/asynchronous receiver/transmitter) Utilities
 * Author: Gergely Gyebrószki
 */

#include "stm32f4xx.h"

USART_HandleTypeDef USART_Initialize(USART_TypeDef* USART_T, uint32_t BAUDRATE);

void USART_PutChar(USART_HandleTypeDef* HUSART, char c);

#endif

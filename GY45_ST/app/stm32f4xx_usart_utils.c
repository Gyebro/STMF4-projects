#include "stm32f4xx_usart_utils.h"

#include "stm32f4xx_gpio_utils.h"
#include "stm32f4xx_hal_usart.h"

#define CONFIG_USART_TIMEOUT 1000

// TODO: Only implemented for USART1
USART_HandleTypeDef USART_Initialize(USART_TypeDef* USARTX, uint32_t BAUDRATE) {
	switch ((int)USARTX) {
	case (int)USART1:
		// Enable alternating config for pin pack 1: PA9 and PA10
		GPIO_Alt_Initialize(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FAST, GPIO_AF7_USART1);
		// Enable USART1 interface clock
		__USART1_CLK_ENABLE();
		// Enable NVIC for USART1 (IRQ handle, and priority)
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	break;
	}
	// Configure handle
	USART_HandleTypeDef USART_Handle;
	USART_Handle.Instance = USARTX;
	USART_Handle.Init.BaudRate = BAUDRATE;
	USART_Handle.Init.Mode = USART_MODE_TX_RX;
	USART_Handle.Init.Parity = USART_PARITY_NONE;
	USART_Handle.Init.StopBits = USART_STOPBITS_1;
	USART_Handle.Init.WordLength = USART_WORDLENGTH_8B;
	// Initialize USART registers
	HAL_USART_Init(&USART_Handle);
	return USART_Handle;
}

HAL_StatusTypeDef USART_PutChar(USART_HandleTypeDef* HUSART, char c) {
	// Send data in blocking mode
	return HAL_USART_Transmit(HUSART,&c,1,CONFIG_USART_TIMEOUT);
}

HAL_StatusTypeDef USART_PutString(USART_HandleTypeDef* HUSART, char* str) {
	// Send data in blocking mode
	HAL_StatusTypeDef status;
	while (*str) {
		status = HAL_USART_Transmit(HUSART,str++,1,CONFIG_USART_TIMEOUT);
	}
	return status;
}


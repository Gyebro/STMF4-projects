#include "stm32f4xx_usart_utils.h"

#include "stm32f4xx_gpio_utils.h"
#include "stm32f4xx_hal_usart.h"

// TODO: Only implemented for USART1
USART_HandleTypeDef USART_Initialize(USART_TypeDef* USART_T, uint32_t BAUDRATE) {
	switch ((int)USART_T) {
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
	USART_Handle.Instance = USART_T;
	USART_Handle.Init.BaudRate = BAUDRATE;
	USART_Handle.Init.Mode = USART_MODE_TX_RX;
	USART_Handle.Init.Parity = USART_PARITY_NONE;
	USART_Handle.Init.StopBits = USART_STOPBITS_1;
	USART_Handle.Init.WordLength = USART_WORDLENGTH_8B;
	// Initialize USART registers
	HAL_USART_Init(&USART_Handle);
	// Enable RX Interrupt
	__USART_ENABLE_IT(&USART_Handle, USART_IT_RXNE);
	return USART_Handle;
}

void USART_PutChar(USART_HandleTypeDef* HUSART, char c) {
	// Send data in non-blocking mode
	HAL_USART_Transmit_IT(HUSART,&c,1);
}


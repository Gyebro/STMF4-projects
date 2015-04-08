#include "stm32f4xx_gpio_utils.h"

void GPIO_Initialize(GPIO_TypeDef * GPIOX, uint32_t PIN, uint32_t MODE, uint32_t PULL, uint32_t SPEED) {
	switch ((int)GPIOX) {
	case (int)GPIOA:
		__GPIOA_CLK_ENABLE();
		break;
	case (int)GPIOB:
		__GPIOB_CLK_ENABLE();
		break;
	case (int)GPIOC:
		__GPIOC_CLK_ENABLE();
		break;
	case (int)GPIOD:
		__GPIOD_CLK_ENABLE();
		break;
	case (int)GPIOE:
		__GPIOE_CLK_ENABLE();
		break;
	case (int)GPIOF:
		__GPIOF_CLK_ENABLE();
		break;
	case (int)GPIOG:
		__GPIOG_CLK_ENABLE();
		break;
	}
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = PIN;
	GPIO_Init.Mode = MODE;
	GPIO_Init.Pull = PULL;
	GPIO_Init.Speed = SPEED;
	HAL_GPIO_Init(GPIOX, &GPIO_Init);
}

void GPIO_Alt_Initialize(GPIO_TypeDef * GPIOX, uint32_t PIN, uint32_t MODE, uint32_t PULL, uint32_t SPEED, uint32_t ALTERNATE) {
	switch ((int)GPIOX) {
	case (int)GPIOA:
		__GPIOA_CLK_ENABLE();
		break;
	case (int)GPIOB:
		__GPIOB_CLK_ENABLE();
		break;
	case (int)GPIOC:
		__GPIOC_CLK_ENABLE();
		break;
	case (int)GPIOD:
		__GPIOD_CLK_ENABLE();
		break;
	case (int)GPIOE:
		__GPIOE_CLK_ENABLE();
		break;
	case (int)GPIOF:
		__GPIOF_CLK_ENABLE();
		break;
	case (int)GPIOG:
		__GPIOG_CLK_ENABLE();
		break;
	}
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = PIN;
	GPIO_Init.Mode = MODE;
	GPIO_Init.Pull = PULL;
	GPIO_Init.Speed = SPEED;
	GPIO_Init.Alternate = ALTERNATE;
	HAL_GPIO_Init(GPIOX, &GPIO_Init);
}

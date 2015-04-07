/**
 *    GY45 (MMA845X breakout board) test project
 *
 *    @author  Gergely Gyebroszki
 */
#include "stm32f4xx.h"

// This project uses the new HAL driveris
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

int main(void) {
	// Enable clock for GPIO G
	__GPIOG_CLK_ENABLE();

	// Configure pin
    GPIO_InitTypeDef GPIO_InitDef;
    GPIO_InitDef.Pin = GPIO_PIN_13 | GPIO_PIN_14; // Pins 13 AND 14
    GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP; // Output, Push-Pull
    GPIO_InitDef.Pull = GPIO_NOPULL; // No Pull-up or Pull-down
    GPIO_InitDef.Speed = GPIO_SPEED_FAST; // Fast speed

    //Initialize pins
    HAL_GPIO_Init(GPIOG, &GPIO_InitDef);

    volatile int i;
    // Toggle PIN13 only
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
    while (1) {
        // Toggle PINS 14 and 13
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14);
		// Sleep
		for (i = 0; i < 400000; i++);
    }
}

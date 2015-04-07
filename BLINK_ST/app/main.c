/**
 *    Blinky project
 *
 *    @author  Gergely Gyebroszki
 */
#include "stm32f4xx.h"

/**
 * This project uses the new HAL driver, see
 *  Docs/DM00105879 Description of STM32F4xx HAL drivers.pdf
 * Old code left in comments to see difference
 */

//#include "stm32f4xx_rcc.h"
#include "stm32f4xx_hal_rcc.h"
//#include "stm32f4xx_gpio.h"
#include "stm32f4xx_hal_gpio.h"

int main(void) {
	// Enable clock for GPIO G
	__GPIOG_CLK_ENABLE();
    /*RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);*/

	// Configure pin
    GPIO_InitTypeDef GPIO_InitDef;
    GPIO_InitDef.Pin = GPIO_PIN_13 | GPIO_PIN_14; // Pins 13 AND 14
    GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP; // Output, Push-Pull
    GPIO_InitDef.Pull = GPIO_NOPULL; // No Pull-up or Pull-down
    GPIO_InitDef.Speed = GPIO_SPEED_FAST; // Fast speed
    /*GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;*/

    //Initialize pins
    HAL_GPIO_Init(GPIOG, &GPIO_InitDef);
    //GPIO_Init(GPIOG, &GPIO_InitDef);


    volatile int i;
    while (1) {
        // Toggle leds
    	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14);
        /*GPIO_ToggleBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);*/

        // Waste some time
        for (i = 0; i < 200000; i++);
    }
}

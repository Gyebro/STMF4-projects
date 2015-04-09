/**
 * GY45 (MMA845X breakout board) example
 * Wiring
 *
 * GY45 ----------------- STM32F429-DISCO
 * V_IN (red) ----------- 3V (right)
 * GND (brown) ---------- GND
 * SCL (orange) --------- PB6
 * SDA (yellow) --------- PB7
 *
 * CP210X ----- STM32F429-DISCO
 * RX --------- PA9
 * TX --------- PA10
 *
 */


/* Core modules */
#include "stm32f4xx.h"

/* TM libraries (http://stm32f4-discovery.com) */
#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_delay.h"

/* STD C libs */
//#include <stdio.h>

/* MMA845X utility */
#include "mma845x_utils.h"

int main(void) {
    //char str[120];
    int accelData[3];

    /* Initialize system */
    SystemInit();

    /* Initialize delay */
    TM_DELAY_Init();

    /* Initialize PG13 (GREEN LED) and PG14 (RED LED) */
    TM_GPIO_Init(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);
    TM_GPIO_SetPinValue(GPIOG, GPIO_PIN_14, 1); // Red: ON

    /* Initialize USART1 at 115200 baud, TX: PA10, RX: PA9 */
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);

    /* Initialize MMA845X */
    uint8_t mma_status = MMA845X_Initialize(MMA_RANGE_4G);
    if (mma_status == MMA_OK) {
    	TM_USART_Puts(USART1, "MMA initialized\n");
    	TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_14 | GPIO_PIN_13); // Red: OFF, Gr: ON
    } else {
    	TM_USART_Puts(USART1, "MMA initialization failed, error code: ");
    	// Add 48 to the byte value to have character representation, (48 = '0')
    	TM_USART_Putc(USART1, mma_status+48);
    	TM_USART_Putc(USART1, '\n');
    }

    /* Format data */
	//sprintf(str, "1. Accelerometer\t- X:%d\t- Y:%d\t- Z:%d\n",100,51,10);
    MMA845X_ReadAcceleration(accelData);

	// Main loop
    while (1) {
    	MMA845X_ReadAcceleration(accelData);
    	if (TM_DELAY_Time() >= 100) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			// Read acceleration data
			//MMA845X_ReadAcceleration(accelData);
			// Send bytes over USART
			//TM_USART_Putc(USART1, accelData[0] >> 8);
			//TM_USART_Putc(USART1, accelData[0]);
			// Toggle Green led
			TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_13);
    	}

    }
}

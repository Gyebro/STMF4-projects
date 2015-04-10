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
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_fonts.h"

/* MMA845X utility */
#include "mma845x_utils.h"

/* Other utilities */
#include "mini-printf.h"

/* Print acceleration value to USART1 */
static char str[120];
void printAccelUSART1(int* accelData) {
	mini_snprintf(str,100,"%d %d %d\n",accelData[0],accelData[1],accelData[2]);
	TM_USART_Puts(USART1, str);
}
void printRawAccelUSART1(uint8_t* raw) {
	mini_snprintf(str,100,"X:%x %x\tY:%x %x\tZ:%x %x\n",raw[0],raw[1],raw[2],raw[3],raw[4],raw[5]);
	TM_USART_Puts(USART1, str);
}
/* Print acceleration value to LCD */
static char lcdstr[120];
void printAccelLCD(int* accelData) {
	mini_snprintf(lcdstr,100,"X:%d___\nY:%d___\nZ:%d___\n",accelData[0],accelData[1],accelData[2]);
	TM_ILI9341_Puts(30, 60, lcdstr, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
}

int main(void) {

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
    uint8_t mma_status = MMA845X_Initialize(MMA_RANGE_2G);
    if (mma_status == MMA_OK) {
    	TM_USART_Puts(USART1, "MMA initialized\n");
    	TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_14 | GPIO_PIN_13); // Red: OFF, Gr: ON
    } else {
    	TM_USART_Puts(USART1, "MMA initialization failed, error code: ");
    	// Add 48 to the byte value to have character representation, (48 = '0')
    	TM_USART_Putc(USART1, mma_status+48);
    	TM_USART_Putc(USART1, '\n');
    }

    /* Initialize Display */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_1);
	TM_ILI9341_SetLayer1();
	TM_ILI9341_Fill(ILI9341_COLOR_BLACK); /* Fill data on layer 1 */
	// Greeting text
	TM_ILI9341_Puts(30, 30, "MMA845X demo", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

	/* MAIN LOOP */
    while (1) {

    	// Read and print acceleration data
		MMA845X_ReadAcceleration(accelData);
		printAccelUSART1(accelData);
		printAccelLCD(accelData);

		// Toggle Green led
		TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_13);


    }
}

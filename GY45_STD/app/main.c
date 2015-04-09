
/* Core modules */
#include "stm32f4xx.h"
/* TM libraries (http://stm32f4-discovery.com) */
#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"

/* MMA address is 0x1D if SA0 is high, 0x1C if low */
// Here the 0x1C must be shifted left, so it will be 0x38
#define MMA_ADDRESS			0x38
// Define a few of the MMA registers
#define MMA_OUT_X_MSB		0x01
#define MMA_XYZ_DATA_CFG	0x0E
#define MMA_WHO_AM_I   		0x0D
#define MMA_I_AM			0x2A
#define MMA_CTRL_REG1  		0x2A

int main(void) {
    volatile int i;

    /* Initialize system */
    SystemInit();

    /* Initialize delay */
    TM_DELAY_Init();

    /* Initialize PG13 (GREEN LED) and PG14 (RED LED) */
    TM_GPIO_Init(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);
    TM_GPIO_SetPinValue(GPIOG, GPIO_PIN_14, 1); // Red: ON

    /* Initialize USART1 at 115200 baud, TX: PA10, RX: PA9 */
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);

    /* Init I2C1 SCL: PB6 and SDA: PB7 */
    TM_I2C_Init(I2C1, TM_I2C_PinsPack_1, 100000);

    /* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(I2C1, MMA_ADDRESS)) {
		/* Return error */
		TM_USART_Puts(USART1, "I2C device is NOT connected!\n");
	} else {
		TM_USART_Puts(USART1, "I2C device is connected\n");
	}

	/* Check who I am */
	uint8_t whoiam = TM_I2C_Read(I2C1, MMA_ADDRESS, MMA_WHO_AM_I);
	if (whoiam != MMA_I_AM) {
		/* Return error */
		TM_USART_Puts(USART1, "I2C device is Unknown!\n");
		TM_USART_Putc(USART1, whoiam);
	} else {
		TM_USART_Puts(USART1, "I2C device is an MMA845x\n");
	}

    /* Put string to USART */
    TM_USART_Puts(USART1, "Initialization complete\n");
    TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_14 | GPIO_PIN_13); // Red: OFF, Gr: ON

	// Main loop
    while (1) {
       for(i=0;i<500000;i++) {}
       TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_13);
    }
}

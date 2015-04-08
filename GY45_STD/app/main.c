
/* Core modules */
#include "stm32f4xx.h"
/* TM libraries (http://stm32f4-discovery.com) */
#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_i2c.h"

/* MMA address is 0x1D if SA0 is high, 0x1C if low */
// Here the 0x1C must be shifted left, so it will be 0x38
#define MMA_ADDRESS		0x1C
// Define a few of the MMA registers
#define MMA_OUT_X_MSB		0x01
#define MMA_XYZ_DATA_CFG	0x0E
#define MMA_WHO_AM_I   		0x0D
#define MMA_CTRL_REG1  		0x2A

int main(void) {
    volatile int i;

    /* Initialize system */
    SystemInit();

    /* Initializa PG13 (GREEN LED) and PG14 (RED LED) */
    TM_GPIO_Init(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);
    TM_GPIO_SetPinValue(GPIOG, GPIO_PIN_14, 1); // Red: ON

    /* Initialize USART1 at 115200 baud, TX: PA10, RX: PA9 */
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);

    /* Init I2C1 SCL: PB6 and SDA: PB7 */
    TM_I2C_Init(I2C1, TM_I2C_PinsPack_1, 10000);

    /* Put string to USART */
    TM_USART_Puts(USART1, "Initialization complete\n");

    TM_GPIO_SetPinValue(GPIOG, GPIO_PIN_14, 0); // Red: OFF

    uint8_t data;
    data = TM_I2C_Read(I2C1, (MMA_ADDRESS << 1), MMA_WHO_AM_I);
    TM_USART_Putc(USART1, data);

    uint8_t ctrl = TM_I2C_Read(I2C1, (MMA_ADDRESS << 1), MMA_CTRL_REG1);
    TM_USART_Putc(USART1, ctrl);
    TM_I2C_Write(I2C1, (MMA_ADDRESS << 1), MMA_CTRL_REG1,  ctrl | 0x01); //Set the active bit to begin detection


    volatile uint8_t reg;
    for (reg=0; reg<0xFF; reg++) {
    	data = TM_I2C_Read(I2C1, (MMA_ADDRESS << 1), reg);
    	TM_USART_Putc(USART1, data);
    }

	// Main loop
    while (1) {
       for(i=0;i<500000;i++) {}
       TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_13);
    }
}

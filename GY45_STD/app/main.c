
/* Core modules */
#include "stm32f4xx.h"
/* TM libraries (http://stm32f4-discovery.com) */
#include "defines.h"
#include "tm_stm32f4_usart.h"

int main(void) {
    uint8_t c;

    /* Initialize system */
    SystemInit();

    /* Initialize USART1 at 115200 baud, TX: PA10, RX: PA9 */
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);

    /* Put string to USART */
    TM_USART_Puts(USART1, "TM Library test\n\r");

    while (1) {
        /* Get character from internal buffer */
        c = TM_USART_Getc(USART1);
        if (c) {
            /* If anything received, put it back to terminal */
            TM_USART_Putc(USART1, c);
        }
    }
}

/**
 * GY45 (MMA845X breakout board) example
 * Wiring
 *
 * GY45 ----------------- STM32F429-DISCO
 * V_IN (red) ----------- 3V (right)
 * GND (brown) ---------- GND
 * SCL (orange) --------- PA8
 * SDA (yellow) --------- PC9
 *
 * CP210X ----- STM32F429-DISCO
 * RX --------- PA9
 * TX --------- PA10
 *
 * Half-bridge
 * (RED)	5V
 * (BRO)	GND
 * (WHITE)	PA0
 *
 * Motor control
 * IN1 (GREEN)	PE2
 * IN2 (BLUE)	PE3
 * ENA (ORANG)	PE5
 *
 */


/* Core modules */
#include "stm32f4xx.h"

/* TM libraries (http://stm32f4-discovery.com) */
#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_adc.h"
//#include "tm_stm32f4_dac.h"
#include "tm_stm32f4_dac_signal.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_fonts.h"

#include "tm_stm32f4_usb_vcp.h"

/* MMA845X utility */
#include "mma845x_utils.h"

/* Other utilities */
#include "mini-printf.h"

/* Comment or Uncomment the following lines to enable/disable UART/VCP */
//#define ENABLE_USART
//#define ENABLE_VCP
//#define DISABLE_ACCELEROMETER

#define CURRENT_ADC ADC1
#define CURRENT_CH ADC_Channel_0

/* Send message to PC */
void SendString(char* message) {
#ifdef ENABLE_USART
	TM_USART_Puts(USART1, message);
#endif
#ifdef ENABLE_VCP
	TM_USB_VCP_Puts(message);
#endif
}
void SendChar(char message) {
#ifdef ENABLE_USART
	TM_USART_Putc(USART1, message);
#endif
#ifdef ENABLE_VCP
	TM_USB_VCP_Putc(message);
#endif
}

/* Print acceleration value to COMM */
static char str[120];
void printAccel(int* accelData) {
	mini_snprintf(str,100,"%d %d %d\n",accelData[0],accelData[1],accelData[2]);
	SendString(str);
}

/* Print acceleration value to LCD */
static char lcdstr[120];
void printAccelLCD(int* accelData) {
	mini_snprintf(lcdstr,100,"X:%d___\nY:%d___\nZ:%d___\n",accelData[0],accelData[1],accelData[2]);
	TM_ILI9341_Puts(30, 60, lcdstr, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
}

size_t xpix = 1;
int axprev = 0, ayprev = 0, azprev = 0;
int strainprev = 0;
void printGraphsLCD(int* accelData, int analogIn, int analogAvg) {
	// Only draw if it is time to do so
	//if (TM_DELAY_Time() > 1) {
		mini_snprintf(lcdstr,100,"A:%d___",analogAvg);
		TM_ILI9341_Puts(30, 60, lcdstr, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		// Draw 3 accel graphs on top half of the screen
		int ax = 80 + accelData[0]*40/4096;
		int ay = 80 + accelData[1]*40/4096;
		int az = 80 + accelData[2]*40/4096;
		TM_ILI9341_DrawLine(xpix,80,xpix+1,80,ILI9341_COLOR_GRAY);
		TM_ILI9341_DrawLine(xpix,axprev,xpix+1,ax,ILI9341_COLOR_RED);
		TM_ILI9341_DrawLine(xpix,ayprev,xpix+1,ay,ILI9341_COLOR_GREEN);
		TM_ILI9341_DrawLine(xpix,azprev,xpix+1,az,ILI9341_COLOR_BLUE);
		axprev = ax;
		ayprev = ay;
		azprev = az;
		// Draw vertical line
		TM_ILI9341_DrawLine(1,160,240,160,ILI9341_COLOR_WHITE);
		// Draw strain graph on bottom half
		int strain = 180 + analogIn*140/4096;
		TM_ILI9341_DrawLine(xpix,strainprev,xpix+1,strain,ILI9341_COLOR_CYAN);
		strainprev = strain;
		// Move x coord
		xpix++;
		if(xpix > 239) {
			xpix = 1;
			// Clear whole screen
			TM_ILI9341_Fill(ILI9341_COLOR_BLACK);
		}
		// Reset timer
	//	TM_DELAY_SetTime(0);
	//}
}

#define BUFFER 200

int main(void) {

    int accelData[3];
    int analogData[BUFFER];
    int a = 0;
    int analogIn = 0;

    /* Initialize system */
    SystemInit();

    /* Initialize delay */
    TM_DELAY_Init();

    /* Initialize PG13 (GREEN LED) and PG14 (RED LED) */
    TM_GPIO_Init(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);
    TM_GPIO_SetPinValue(GPIOG, GPIO_PIN_14, 1); // Red: ON

#ifdef ENABLE_USART
    /* Initialize USART1 at 115200 baud, TX: PA10, RX: PA9 */
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);
#endif

#ifdef ENABLE_VCP
    /* Initialize USB Virtual Comm Port */

    TM_USB_VCP_Result status = TM_USB_VCP_NOT_CONNECTED;
    while (TM_USB_VCP_GetStatus() != TM_USB_VCP_CONNECTED) {
    	TM_USB_VCP_Init();
    	TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_14);
    	Delay(500000);
    }
    SendString("USB VCP initialized and connected\n");
    TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_14 | GPIO_PIN_13); // Red: OFF, Gr: ON

#endif

#ifdef ENABLE_MMA

    /* Initialize MMA845X */
    uint8_t mma_status = MMA845X_Initialize(MMA_RANGE_4G);
    if (mma_status == MMA_OK) {
    	SendString("MMA initialized\n");
    } else {
    	SendString("MMA initialization failed, error code: ");
    	// Add 48 to the byte value to have character representation, (48 = '0')
    	SendChar('0'+mma_status);
    	SendChar('\n');
    }

#endif

    /* Initialize Display */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_1);
	TM_ILI9341_SetLayer1();
	TM_ILI9341_Fill(ILI9341_COLOR_BLACK); /* Fill data on layer 1 */

	/* Initialize ADC1 */
	TM_ADC_Init(CURRENT_ADC, CURRENT_CH);

	/* Initialize PE2 and PE3 for digital output (Motor direction) */
    TM_GPIO_Init(GPIOE, GPIO_PIN_2 | GPIO_PIN_3, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);
    // Set them to HIGH/LOW
    TM_GPIO_SetPinHigh(GPIOE, GPIO_PIN_3);
    TM_GPIO_SetPinLow(GPIOE, GPIO_PIN_2);

    /* Set up PE5 (in front of PE4) for PWM (TIM9 CH1 PP2) (Motor speed control) */
    TM_PWM_TIM_t TIM9_Data;
    // Set PWM to 1kHz frequency on timer TIM4, 1 kHz = 1ms = 1000us
	TM_PWM_InitTimer(TIM9, &TIM9_Data, 1000);
	// Initialize PWM on TIM9, Channel 1 and PinsPack 2 = PE5
	TM_PWM_InitChannel(&TIM9_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	// Set channel 1 value, 50% duty cycle
	TM_PWM_SetChannelPercent(&TIM9_Data, TM_PWM_Channel_1, 50);

	/* Initialize DAC channel 2, pin PA5 (Shaker control) */
	//TM_DAC_Init(TM_DAC2);
	/* Set 12bit analog value of 2047/4096 * 3.3V */
	//TM_DAC_SetValue(TM_DAC2, 4096);

	// DAC PIN PA5
	/* Initialize DAC1, use TIM4 for signal generation */
	TM_DAC_SIGNAL_Init(TM_DAC2, TIM4);
	/* Output predefined triangle signal with frequency of 5kHz */
	TM_DAC_SIGNAL_SetSignal(TM_DAC2, TM_DAC_SIGNAL_Signal_Sinus, 50);

	int i=0;

	/* MAIN LOOP */
    while (1) {

    	// Read acceleration data
#ifdef ENABLE_MMA
		MMA845X_ReadAcceleration(accelData);
#endif

		// Read analog input
		analogData[a] = TM_ADC_Read(CURRENT_ADC, CURRENT_CH);
		a++;
		if(a==BUFFER) {a=0;}

		// Analog average
		analogIn=0;
		for(i=0;i<BUFFER;i++){analogIn+=analogData[i];}
		analogIn/=BUFFER;

		// Print graphs
		printGraphsLCD(accelData, analogData[a], analogIn);

		// Toggle Green led
		TM_GPIO_TogglePinValue(GPIOG, GPIO_PIN_13);

    }
}

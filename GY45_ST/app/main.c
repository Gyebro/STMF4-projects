/**
 *    GY45 (MMA845X breakout board) test project
 *
 *    @author  Gergely Gyebroszki
 *
 *    Hardware setup:
 *    MMA845X Breakout ------------ STM32F429I-DISCO
 *    VIN ----------------------- 5V (Red)
 *    SDA ----------------------- PC9 (Yellow, I2C data)
 *    SCL ----------------------- PA8 (Orange, I2C clock)
 *    GND ----------------------- GND (Black)
 */
#include "stm32f4xx.h"

// This project uses the new HAL driveris
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"



/*
 MMA8452Q Basic Example Code
 Nathan Seidle
 SparkFun Electronics
 November 5, 2012

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example code shows how to read the X/Y/Z accelerations and basic functions of the MMA5842. It leaves out
 all the neat features this IC is capable of (tap, orientation, and inerrupts) and just displays X/Y/Z. See
 the advanced example code to see more features.



 */


#define MMA8452_ADDRESS 0x1C  // 0x1D if SA0 is high, 0x1C if low

//Define a few of the registers that we will be accessing on the MMA8452
#define OUT_X_MSB    0x01
#define XYZ_DATA_CFG 0x0E
#define WHO_AM_I     0x0D
#define CTRL_REG1    0x2A
#define GSCALE       2		// Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.

void readAccelData(int *destination) {
/*
  byte rawData[6];  // x/y/z accel register data stored here
  readRegisters(OUT_X_MSB, 6, rawData);  // Read the six raw data registers into data array
  // Loop to calculate 12-bit ADC and g value for each axis
  for(int i = 0; i < 3 ; i++)
  {
    int gCount = (rawData[i*2] << 8) | rawData[(i*2)+1];  //Combine the two 8 bit registers into one 12-bit number
    gCount >>= 4; //The registers are left align, here we right align the 12-bit integer
    // If the number is negative, we have to make it so manually (no 12-bit data type)
    if (rawData[i*2] > 0x7F)
    {
      gCount = ~gCount + 1;
      gCount *= -1;  // Transform into negative 2's complement #
    }
    destination[i] = gCount; //Record this gCount into the 3 int array
  }
  */
}

// Initialize the MMA8452 registers
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8452Q
void initMMA8452() {
/*
  byte c = readRegister(WHO_AM_I);  // Read WHO_AM_I register
  if (c == 0x2A) // WHO_AM_I should always be 0x2A
  {
    Serial.println("MMA8452Q is online...");
  }
  else
  {
    Serial.print("Could not connect to MMA8452Q: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
  }
  MMA8452Standby();  // Must be in standby to change registers
  // Set up the full scale range to 2, 4, or 8g.
  byte fsr = GSCALE;
  if(fsr > 8) fsr = 8; // Easy error check
  fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
  writeRegister(XYZ_DATA_CFG, fsr);
  // The default data rate is 800Hz and we don't modify it in this example code
  MMA8452Active();  // Set to active to start reading
  */
}

// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby() {
/*
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
  */
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active() {
/*
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c | 0x01); //Set the active bit to begin detection
  */
}

// Read bytesToRead sequentially, starting at addressToRead into the dest byte array
void readRegisters(short addressToRead, int bytesToRead, short * dest) {
/*
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active
  Wire.requestFrom(MMA8452_ADDRESS, bytesToRead); //Ask for bytes, once done, bus is released by default
  while(Wire.available() < bytesToRead); //Hang out until we get the # of bytes we expect
  for(int x = 0 ; x < bytesToRead ; x++)
    dest[x] = Wire.read();
    */
}

// Read a single byte from addressToRead and return it as a byte
short readRegister(short addressToRead) {
/*
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active
  Wire.requestFrom(MMA8452_ADDRESS, 1); //Ask for 1 byte, once done, bus is released by default
  while(!Wire.available()) ; //Wait for the data to come back
  return Wire.read(); //Return this one byte
  */
  return 0;
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister(short addressToWrite, short dataToWrite) {
/*
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToWrite);
  Wire.write(dataToWrite);
  Wire.endTransmission(); //Stop transmitting
  */
}


int main(void) {
	// Enable clock for GPIO G
	__GPIOG_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();

	// Configure pin for LEDS
    GPIO_InitTypeDef GPIO_InitDef;
    GPIO_InitDef.Pin = GPIO_PIN_13 | GPIO_PIN_14; // Pins 13 AND 14
    GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP; // Output, Push-Pull
    GPIO_InitDef.Pull = GPIO_NOPULL; // No Pull-up or Pull-down
    GPIO_InitDef.Speed = GPIO_SPEED_FAST; // Fast speed

    // Configure PA0 for Button
    GPIO_InitTypeDef GPIO_ButtonDef;
    GPIO_ButtonDef.Pin = GPIO_PIN_0;
    GPIO_ButtonDef.Mode = GPIO_MODE_INPUT;
    GPIO_ButtonDef.Pull = GPIO_PULLDOWN;
    GPIO_ButtonDef.Speed = GPIO_SPEED_FAST;

    //Initialize pins
    HAL_GPIO_Init(GPIOG, &GPIO_InitDef);
    HAL_GPIO_Init(GPIOA, &GPIO_ButtonDef);

    //Wire.begin(); // Join the bus as a master
    initMMA8452(); // Test and initialize the MMA8452


    volatile int i;
    while (1) {

    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
    		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
    		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
    	} else {
    		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
    		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
    	}

		// Read accelero data
		int accelCount[3];  // Stores the 12-bit signed value
		readAccelData(accelCount);  // Read the x/y/z adc values
		// Now we'll calculate the accleration value into actual g's
		float accelG[3];  // Stores the real accel value in g's
		//for (int i = 0 ; i < 3 ; i++) { accelG[i] = (float) accelCount[i] / ((1<<12)/(2*GSCALE)); }
    }
}

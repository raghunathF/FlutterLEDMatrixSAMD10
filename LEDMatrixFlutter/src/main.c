/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "LEDMatrixSPIControl.h"
#include "LEDMatrixSPIMaster.h"
#include "LEDMatrixUtilities.h"
#include "LEDMatrixI2CSlave.h"
#include "LEDMatrixReadDial.h"
#include "LEDMatrixControl.h"

#define DATA_LENGTH_LEDS	  55
uint16_t useDialLValue = 0 ;
uint16_t useDialRValue = 0;

volatile bool sensorsUpdate		= false;
volatile bool I2CWriteComplete  = false;

uint8_t	 LEDMatrixMode = 0;
volatile bool		 transferDone = false;
volatile uint8_t	 DMASourceRegister[13];
volatile uint8_t     RMatrix[4][8];
volatile uint8_t     GMatrix[4][8];
volatile uint8_t     BMatrix[4][8];
volatile uint8_t     dialOutputs[2];

uint8_t I2CReadBuffer[DATA_LENGTH_LEDS];

volatile bool I2CReadComplete = false;

void test()
{
	int8_t i,j,k =0; 
	volatile uint8_t SPISend[13];
	
	for(j=0;j<8;j++)
	{
		SPISend[0] = 0x80 >> j;
		for(i=0;i<4;i++)
		{		
			SPISend[i*3 + 1] = ~BMatrix[3-i][j];
			SPISend[i*3 + 2] = ~GMatrix[3-i][j];
			SPISend[i*3 + 3] = ~RMatrix[3-i][j];
			/*
			SPISend[i*3 + 1] = ~0x3F;
			SPISend[i*3 + 2] = ~0xFC;
			SPISend[i*3 + 3] = ~0x3F;
			*/
		}
		LEDMatrixSend(SPISend);
		delay_cycles_us(500);
	}

}

int main (void)
{	
	uint8_t i,j =0;

	system_init();
	delay_init();
	configDials();
	
	configure_i2c_slave();
	configure_i2c_slave_callbacks();
	testFunction();
	
	LEDMatrixTimerInit();
	testPinsInit();
	LEDMatrixInit();
	SPIMasterInit();
	
	
	LEDMatrixMode = 1;
	while(1)
	{
		readDials();
		LEDMatrixControl();
		//delay_ms(20);
		/*
		for(j=2;j<5;j++)
		{
			for(i=0;i<25;i++)
			{
				testFunctionBBTLogo(j);
				test();
			}
		}
		*/
	}
}

/*
 * LEDMatrixBluetoothMode.c
 *
 * Created: 11/29/2018 10:32:28 AM
 *  Author: raghu
 */ 
#include <asf.h>
#include "LEDMatrixBluetoothMode.h"
#include "LEDMatrixUtilities.h"

#define DATA_LENGTH_LEDS	  55

extern volatile bool I2CReadComplete;
extern uint8_t I2CReadBuffer[DATA_LENGTH_LEDS];

void updateBluetoothMode()
{
	uint8_t i =0;
	if(I2CReadComplete == true)
	{
		I2CReadComplete = false;
		for(i=1;i<4;i++)
		{
			convertMatrix2Shift(I2CReadBuffer+1+18*(i-1),i);
		}
	}
}
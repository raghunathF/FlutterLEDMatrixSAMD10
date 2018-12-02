/*
 * LEDMatrixMultiplyMode.c
 *
 * Created: 11/24/2018 12:14:56 PM
 *  Author: Raghu
 */ 
#include <asf.h>
#include "LEDMatrixMultiplyMode.h"
#include "LEDMatrixUtilities.h"

#define  UPDATE_THRESH_ROW		1	
#define  UPDATE_THRESH_COLUMN	2	

volatile uint8_t minRThresh[13];
volatile uint8_t maxRThresh[13];

volatile uint8_t minCThresh[13];
volatile uint8_t maxCThresh[13];

static uint8_t LEDArray[18];

extern  volatile uint8_t dialOutputs[2];



#define THRESHOLD2_MIN_0	0
#define THRESHOLD2_MAX_0	25

#define THRESHOLD2_MIN_12	211
#define THRESHOLD2_MAX_12	255


uint8_t dialConvertValueRow(uint8_t dialValue)
{
	uint8_t i = 0;
	uint8_t dialValueConvert = 0;
	static uint8_t prevRowValue = 0; 
	for(i=0;i<=12;i++)
	{
		if((dialValue >= minRThresh[i]) && (dialValue < maxRThresh[i]) )
		{
			dialValueConvert = i;
			break;
		}
	}
	if(prevRowValue != dialValueConvert)
	{
		for(i=0;i<=12;i++)
		{
			minRThresh[i] = 18*i + 7;
			maxRThresh[i] = 18*i +  11;
		}
		if(( dialValueConvert != 0 ) || (dialValueConvert != 12))
		{
			minRThresh[dialValueConvert] = 18*(dialValueConvert-1)+11;
			maxRThresh[dialValueConvert] = 18*(dialValueConvert+1)+ 7;
		}
		else if(dialValueConvert == 0)
		{
			minRThresh[dialValueConvert] = THRESHOLD2_MIN_0;
			maxRThresh[dialValueConvert] = THRESHOLD2_MAX_0;
		}
		else if(dialValueConvert == 12)
		{
			minRThresh[dialValueConvert] = THRESHOLD2_MIN_12;
			maxRThresh[dialValueConvert] = THRESHOLD2_MAX_12;
		}
		prevRowValue = dialValueConvert;
	}
	return dialValueConvert;
}

uint8_t dialConvertValueColumn(uint8_t dialValue)
{
	uint8_t i = 0;
	uint8_t dialValueConvert = 0;
	static uint8_t prevColumnValue = 0;
	for(i=0;i<=12;i++)
	{
		if((dialValue >= minCThresh[i]) && (dialValue < maxCThresh[i]))
		{
			dialValueConvert = i;
			break;
		}
	}
	if(prevColumnValue != dialValueConvert)
	{
		for(i=0;i<=12;i++)
		{
			minCThresh[i] = 18*i + 7;
			maxCThresh[i] = 18*i +  11;
		}
		if(( dialValueConvert != 0 ) || (dialValueConvert != 12))
		{
			minCThresh[dialValueConvert] = 18*(dialValueConvert-1)+11;
			maxCThresh[dialValueConvert] = 18*(dialValueConvert+1)+ 7;
		}
		else if(dialValueConvert == 0)
		{
			minCThresh[dialValueConvert] = THRESHOLD2_MIN_0;
			maxCThresh[dialValueConvert] = THRESHOLD2_MAX_0;
		}
		else if(dialValueConvert == 12)
		{
			minCThresh[dialValueConvert] = THRESHOLD2_MIN_12;
			maxCThresh[dialValueConvert] = THRESHOLD2_MAX_12;
		}
		prevColumnValue = dialValueConvert;
	}
	return dialValueConvert;
}

uint8_t filterDialMulMode(uint8_t dialValue , uint8_t RC )
{
	uint8_t dialValueConvert = 0;
	uint8_t i =0;
	static uint8_t prevColumnValue = 0;
	switch(RC)
	{
		case UPDATE_THRESH_ROW:
			dialValueConvert = dialConvertValueRow(dialValue);
			break;
		case UPDATE_THRESH_COLUMN:
			dialValueConvert = dialConvertValueColumn(dialValue);
			break;
		default:
			break;
	}

	return dialValueConvert;
}

void convertDialMulMode()
{
	filterDialMulMode(dialOutputs[0] , UPDATE_THRESH_ROW);
	filterDialMulMode(dialOutputs[1] , UPDATE_THRESH_COLUMN);
}


void refreshLEDArray()
{
	uint8_t i = 0;
	for(i=0;i<18;i++)
	{
		LEDArray[i] = 0;
	}
}

void convertDialInfo2LEDMatrix(uint8_t row , uint8_t column)
{
	uint8_t i ,j = 0;
	uint8_t activeLEDS[144];
	uint8_t activeLEDCount = 0;
	uint8_t LEDArrayNumber      = 0;
	uint8_t LEDNumber			= 0;
	
	for(i=1;i<=row;i++)
	{
		for(j=1;j<=column;j++)
		{
			activeLEDS[activeLEDCount] = (i-1)*12 + j ;
			activeLEDCount++;
		}
	}
	refreshLEDArray();
	for(i=0;i<activeLEDCount;i++)
	{
		LEDArrayNumber				  =		17 - (activeLEDS[i] / 8);
		LEDNumber					  =		(activeLEDS[i] % 8);
		LEDArray[LEDArrayNumber]     |=		0x01 << LEDNumber;
	}
	
}

void update2ShiftResgisterFormat()
{
	//updating all three different colors with same array
	convertMatrix2Shift(LEDArray ,1);
	convertMatrix2Shift(LEDArray ,2);
	convertMatrix2Shift(LEDArray ,3);
}

void	updateMatrixMultiplyMode()
{
	static uint8_t prevRow		= 0;
	static uint8_t prevColumn	= 0;
	static uint8_t row    = 0;
	static uint8_t column = 0;
	convertDialMulMode();
	if((prevRow != row ) && (prevColumn == column ))
	{
		convertDialInfo2LEDMatrix(row , column);	
		update2ShiftResgisterFormat();		
	}
	
}


void setupThresholds()
{
	uint8_t i = 0;
	uint8_t j = 0;
	for(i=0;i<=12;i++)
	{
		minCThresh[i] = 18*i +  7;
		maxCThresh[i] = 18*i +  11;
		
		minRThresh[i] = 18*i +  7;
		maxRThresh[i] = 18*i +  11;
	}
}

void initMatrixMultiplyMode()
{
	setupThresholds();
}
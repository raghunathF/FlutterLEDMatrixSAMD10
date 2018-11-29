/*
 * LEDMatrixUtilities.c
 *
 * Created: 11/13/2018 12:52:18 PM
 *  Author: Raghunathreddy Jangam
 */ 
#include <asf.h>

#define RED		1
#define GREEN   2
#define BLUE    3

extern  volatile uint8_t     RMatrix[4][8];
extern  volatile uint8_t     GMatrix[4][8];
extern  volatile uint8_t     BMatrix[4][8];

uint8_t reverseByte(volatile uint8_t input) 
{
	input = (input & 0xF0) >> 4 | (input & 0x0F) << 4;
	input = (input & 0xCC) >> 2 | (input & 0x33) << 2;
	input = (input & 0xAA) >> 1 | (input & 0x55) << 1;
	return input;
}


void convertMatrix2Shift(uint8_t* matrixData ,uint8_t matrixColor)
{
	uint8_t				i,j,k			= 0;
	uint32_t			temp			= 0;
	uint32_t			temp_1			= 0;
	uint8_t				carryBits		= 0;
	uint32_t            mask6Bits		= 0xFC000000;
	switch(matrixColor)
	{
		case RED:
			for(j=0;j<3;j++)
			{
				temp =0;
				for(k=0;k<3;k++)
				{
					temp_1     =  matrixData[j*3+k]  ;
					temp      |=  (temp_1 << (24 - 8*k));
				}
				for(i=0; i<2 ; i++)
				{
					RMatrix[1][2+i+(2*j)]       =		reverseByte((uint8_t)((temp &  (mask6Bits>>6*2*i)) >> (26-6*2*i))) ;
					RMatrix[2][2+i+(2*j)]       =		reverseByte(((uint8_t)((temp &  (mask6Bits>>6+6*2*i))	>> (20-6*2*i)<<2))) ;
				}
			}
			
			for(j=0;j<3;j++)
			{
				temp =0;
				for(k=0;k<3;k++)
				{
					temp_1    =  matrixData[(3+j)*3+k] ;
					temp      |=  temp_1 << 24 - 8*k;
				}
				for(i=0; i<2 ; i++)
				{
					RMatrix[0][0+i+ (2*j)]       =		reverseByte((uint8_t)((temp & (mask6Bits>>6*2*i)) >> (26-6*2*i)));
					RMatrix[3][0+i+ (2*j)]       =		reverseByte((uint8_t)((temp & (mask6Bits>>6+6*2*i)) >> (20-6*2*i) <<2));
				}
			}
			break;
		
		case GREEN:
			
			for(j=0;j<3;j++)
			{
				temp =0;
				for(k=0;k<3;k++)
				{
					temp_1     =  matrixData[j*3+k] ;
					temp    |=  (temp_1 << (24 - 8*k));
				}
				for(i=0; i<2 ; i++)
				{
					GMatrix[1][2+i+(2*j)]       =		(uint8_t)(((temp &  (mask6Bits>>6*2*i)) >> (26-6*2*i))) ;
					GMatrix[2][2+i+(2*j)]       =		(uint8_t)(((temp  &  (mask6Bits>>6+6*2*i)) >> (20-6*2*i))<<2);
				}
			}
			
			for(j=0;j<3;j++)
			{
				temp =0;
				for(k=0;k<3;k++)
				{
					temp_1    =  matrixData[(3+j)*3+k] ;
					temp      |=  temp_1 << 24 - 8*k;
				}
				for(i=0; i<2 ; i++)
				{
					GMatrix[0][0+i+ (2*j)]       =		(uint8_t)(((temp & (mask6Bits>>6*2*i)) >> (26-6*2*i)));
					GMatrix[3][0+i+ (2*j)]       =		(uint8_t)(((temp & (mask6Bits>>6+6*2*i)) >> (20-6*2*i))<<2);
				}
			}
			break;
			
		case BLUE:
			
			for(j=0;j<3;j++)
			{
				temp =0;
				for(k=0;k<3;k++)
				{
					temp_1   =  matrixData[j*3+k] ;
					temp    |=  (temp_1 << (24 - 8*k));
				}
				for(i=0; i<2 ; i++)
				{
					BMatrix[1][2+i+(2*j)]       =		reverseByte((uint8_t)((temp &  (mask6Bits>>6*2*i)) >> (26-6*2*i)));
					BMatrix[2][2+i+(2*j)]       =		reverseByte((uint8_t)(((temp &  (mask6Bits>>6+6*2*i))	>> (20-6*2*i))<<2));
				}
			}
			
			for(j=0;j<3;j++)
			{
				temp =0;
				for(k=0;k<3;k++)
				{
					temp_1    =  matrixData[(3+j)*3+k] ;
					temp      |=  temp_1 << 24 - 8*k;
				}
				for(i=0; i<2 ; i++)
				{
					BMatrix[0][0+i+ (2*j)]       =		reverseByte((uint8_t)((temp & (mask6Bits>>6*2*i)) >> (26-6*2*i)));
					BMatrix[3][0+i+ (2*j)]       =		reverseByte((uint8_t)(((temp & (mask6Bits>>6+6*2*i)) >> (20-6*2*i))<<2));
				}
			}
			break;
		default:
			break;
	}
}

void testFunction()
{
	static uint8_t matrixData[18]	= {0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66};
	uint8_t matrixColor		= 0;
	
	matrixColor = RED;
	convertMatrix2Shift(matrixData ,matrixColor);
	
	matrixColor = BLUE;
	convertMatrix2Shift(matrixData ,matrixColor);
	
	matrixColor = GREEN;
	convertMatrix2Shift(matrixData ,matrixColor);
}

void BBTLogo(uint8_t frame)
{
	static uint8_t  matrixDataRFrame1[18]			= {0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66};
	static uint8_t  matrixDataGFrame1[18]			= {0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66};
	static uint8_t  matrixDataBFrame1[18]			= {0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66};
	
	static uint8_t  matrixDataRFrame2[18]			= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	static uint8_t  matrixDataGFrame2[18]			= {0x80,0x74,0x0F,0x39,0xF3,0x7E,0x23,0xC1,0x98,0x09,0x00,0x88,0x04,0x80,0x38,0x00,0x40,0x04};
	static uint8_t  matrixDataBFrame2[18]			= {0x80,0x74,0x0F,0x39,0xF3,0x7E,0x23,0xC1,0x98,0x09,0x00,0x88,0x04,0x80,0x38,0x00,0x40,0x04};
		
	static uint8_t  matrixDataRFrame3[18]			= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	static uint8_t  matrixDataGFrame3[18]			= {0x80,0x74,0x0F,0x39,0xF3,0x7E,0x23,0xC1,0x98,0x09,0x00,0x88,0x04,0x80,0x38,0x00,0x40,0x02};
	static uint8_t  matrixDataBFrame3[18]			= {0x80,0x74,0x0F,0x39,0xF3,0x7E,0x23,0xC1,0x98,0x09,0x00,0x88,0x04,0x80,0x38,0x00,0x40,0x02};
	
	static uint8_t  matrixDataRFrame4[18]			= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	static uint8_t  matrixDataGFrame4[18]			= {0x80,0x74,0x0F,0x39,0xF3,0x7E,0x23,0xC1,0x98,0x09,0x00,0x88,0x04,0x80,0x38,0x00,0x40,0x02};
	static uint8_t  matrixDataBFrame4[18]			= {0x80,0x74,0x0F,0x39,0xF3,0x7E,0x23,0xC1,0x98,0x09,0x00,0x88,0x04,0x80,0x38,0x00,0x40,0x02};
	
	uint8_t			matrixColor				= 0;
	switch(frame)
	{
		case	1:
			matrixColor = RED;
			convertMatrix2Shift(matrixDataRFrame1 ,matrixColor);
			
			matrixColor = BLUE;
			convertMatrix2Shift(matrixDataGFrame1 ,matrixColor);
			
			matrixColor = GREEN;
			convertMatrix2Shift(matrixDataBFrame1 ,matrixColor);
			break;
			
		case	2:
			matrixColor = RED;
			convertMatrix2Shift(matrixDataRFrame2 ,matrixColor);
			
			matrixColor = BLUE;
			convertMatrix2Shift(matrixDataGFrame2 ,matrixColor);
			
			matrixColor = GREEN;
			convertMatrix2Shift(matrixDataBFrame2 ,matrixColor);
			break;
			
		case	3:
			matrixColor = RED;
			convertMatrix2Shift(matrixDataRFrame3 ,matrixColor);
			
			matrixColor = BLUE;
			convertMatrix2Shift(matrixDataGFrame3 ,matrixColor);
			
			matrixColor = GREEN;
			convertMatrix2Shift(matrixDataBFrame3 ,matrixColor);
			break;
		
		case	4:
			matrixColor = RED;
			convertMatrix2Shift(matrixDataRFrame4 ,matrixColor);
		
			matrixColor = BLUE;
			convertMatrix2Shift(matrixDataGFrame4 ,matrixColor);
		
			matrixColor = GREEN;
			convertMatrix2Shift(matrixDataBFrame4 ,matrixColor);
			break;
		
		default:
			break;
			
	}
	
}
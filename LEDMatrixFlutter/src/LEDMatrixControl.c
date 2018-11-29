/*
 * LEDMatrixControl.c
 *
 * Created: 11/26/2018 2:08:52 PM
 *  Author: raghu
 */ 

#include "asf.h"
#include "LEDMatrixControl.h"
#include "LEDMatrixMultiplyMode.h"
#include "LEDMatrixUtilities.h"
#include "LEDMatrixSPIMaster.h"
#include "LEDMatrixSPIControl.h"
#include "LEDMatrixBluetoothMode.h"

extern uint8_t LEDMatrixMode;

#define     STANDBY_MODE		0
#define		MUL_MODE			1
#define		BLUETOOTH_MODE		2

#define     LED_MATRIX_TIMER    TC1

extern  volatile uint8_t     RMatrix[4][8];
extern  volatile uint8_t     GMatrix[4][8];
extern  volatile uint8_t     BMatrix[4][8];

struct  tc_module    tcLEDMatrix;
static  bool		 updateLEDRows = false;

void testPinsInit()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(TP1, &config_port_pin);
}

void updateSPIValues()
{
	int8_t i,j,k =0; 
	static uint8_t SPISend[13];
	
	for(j=0;j<8;j++)
	{
		SPISend[0] = 0x80 >> j;
		for(i=0;i<4;i++)
		{		
			SPISend[i*3 + 1] = ~BMatrix[3-i][j];
			SPISend[i*3 + 2] = ~GMatrix[3-i][j];
			SPISend[i*3 + 3] = ~RMatrix[3-i][j];
		}
		LEDMatrixSend(SPISend);
		delay_cycles_us(400);
	}
}


//Every 340us
void initLEDMatrixTimer()
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	struct tc_events config_events;
	config_tc.enable_capture_on_channel[0]		=		true;
	config_tc.counter_size						=		TC_COUNTER_SIZE_8BIT; //8
	config_tc.clock_source						=		GCLK_GENERATOR_3 ;
	config_tc.clock_prescaler					=		TC_CLOCK_PRESCALER_DIV64; //1024
	config_tc.counter_8_bit.period				=		0xFF;
	tc_init(&tcLEDMatrix, LED_MATRIX_TIMER , &config_tc);
	tc_enable(&tcLEDMatrix);
}

void updateLEDMatrix()
{
	
	updateLEDRows = true;
	port_pin_toggle_output_level(TP1);
	//updateSPIValues();
}

void initLEDMatrixTimerCallbacks()
{
	tc_register_callback(&tcLEDMatrix, updateLEDMatrix,TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tcLEDMatrix, TC_CALLBACK_OVERFLOW);
}

void initMatrixStandbyMode()
{
	BBTLogo(3);
}

void updateLEDMatrixArray(uint8_t currentLEDMatrixMode)
{
	uint8_t i = 0;
	switch(currentLEDMatrixMode)
	{
		case MUL_MODE:
			updateMatrixMultiplyMode();
			break;
		case BLUETOOTH_MODE:
			updateBluetoothMode();
			break;
		default:
			break;
	}
}

void LEDMatrixControl()
{
	static uint8_t prevLEDMatrixMode	= 255; 
	static uint8_t currentLEDMatrixMode = 0;
	currentLEDMatrixMode  =  2;
	if(prevLEDMatrixMode != currentLEDMatrixMode)
	{
		switch(LEDMatrixMode)
		{
			case STANDBY_MODE:
				initMatrixStandbyMode();
				break;
			case MUL_MODE:
				initMatrixMultiplyMode();
				break;
			case BLUETOOTH_MODE:
				break;
			default:
				break;
		}
		prevLEDMatrixMode = currentLEDMatrixMode;
	}
	if(updateLEDRows == true)
	{
		updateLEDRows = false;
		updateSPIValues();
	}
	
	updateLEDMatrixArray(currentLEDMatrixMode);
}

void LEDMatrixTimerInit()
{
	initLEDMatrixTimer();
	initLEDMatrixTimerCallbacks();
	
}
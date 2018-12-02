/*
 * LEDMatrixReadDial.c
 *
 * Created: 11/23/2018 2:35:01 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "LEDMatrixReadDial.h"
#include "LEDMatrixI2CSlave.h"

#define  DIAL_THRESHOLD				2
#define  NUMBER_COUNT_THRESHOLD		3
#define  DIAL_L_ANALOG_CHANNEL		4
#define  DIAL_R_ANALOG_CHANNEL		5


uint16_t* ADCResult = NULL;

//uint16_t testLogDialL[500];
//uint16_t testLogDialR[500];

static uint16_t useDialLValue ;
static uint16_t useDialRValue ;

extern volatile bool sensorsUpdate;
extern  uint8_t dialOutputs[2];

struct adc_module adc_instance;

void configADC()
{
	struct adc_config conf_adc;
	ADCResult = malloc(sizeof(uint16_t));
	
	adc_get_config_defaults(&conf_adc);

	conf_adc.reference			=  ADC_REFCTRL_REFSEL_INTVCC0_Val;
	conf_adc.clock_prescaler	=  ADC_CLOCK_PRESCALER_DIV32;
	conf_adc.positive_input		=  DIAL_L_ANALOG_CHANNEL;
	conf_adc.negative_input		=  ADC_NEGATIVE_INPUT_GND;
	conf_adc.resolution			=  ADC_RESOLUTION_8BIT;

	adc_init(&adc_instance, ADC, &conf_adc);
	adc_enable(&adc_instance);
}

void configDials()
{
	configADC();
}


uint16_t adc_start_read_result(const enum adc_positive_input analogPin)
{
	uint16_t temp = 0;
	adc_set_positive_input(&adc_instance, analogPin );
	adc_start_conversion(&adc_instance);
	while((adc_get_status(&adc_instance) & ADC_STATUS_RESULT_READY) != 1);
	adc_read(&adc_instance, ADCResult);
	temp = *ADCResult;
	return temp;
}

void filterDialValues(uint8_t dialLValue , uint8_t dialRValue)
{
	static uint16_t dialLValueInf   = 0 ;
	static uint16_t dialRValueInf	= 0 ;
	static uint16_t countThresholdL  = 0;
	static uint16_t countThresholdR  = 0;
	static bool      firstTime      = true;
	
	if(firstTime == true)
	{
		firstTime     = false;
		dialLValueInf = dialLValue;
		dialRValueInf = dialRValue;
		useDialLValue = dialLValue;
		useDialRValue = dialRValue;
	}
	else
	{
		dialLValueInf = (dialLValue + dialLValueInf*9)/10;
		dialRValueInf = (dialRValue + dialRValueInf*9)/10;
	}
	//useDialLValue = dialLValue;
	//useDialRValue = dialRValue;
	
	if(abs(dialLValueInf - useDialLValue) > DIAL_THRESHOLD)
	{
		countThresholdL++;
		if(countThresholdL > NUMBER_COUNT_THRESHOLD)
		{
			countThresholdL = 0;
			useDialLValue = (uint8_t)dialLValueInf;
			
		}
	}
	else
	{
		useDialLValue = (uint8_t)dialLValueInf;
		countThresholdL = 0;
	}
	
	if(abs(dialRValueInf - useDialRValue) > DIAL_THRESHOLD)
	{
		countThresholdR++;
		if(countThresholdR > NUMBER_COUNT_THRESHOLD)
		{
			countThresholdR = 0;
			useDialLValue = (uint8_t)dialRValueInf;
			
		}
	}
	else
	{
		useDialRValue = (uint8_t)dialRValueInf;
		countThresholdR = 0;
	}
	
	
	
}
void readDials()
{
	static uint16_t testCount = 0;
	
	uint16_t dialLValue = 0;
	uint16_t dialRValue = 0;
	static uint8_t tempCount = 0;
	tempCount++;
	if(tempCount >200)
	{
		tempCount = 0;
	}
	dialLValue		= adc_start_read_result(DIAL_L_ANALOG_CHANNEL);
	dialRValue		= adc_start_read_result(DIAL_R_ANALOG_CHANNEL);
	
	sensorsUpdate  = true;
	filterDialValues(dialLValue,dialRValue);
	dialOutputs[0]  = (useDialLValue & 0xF0) >>4;
	dialOutputs[1]  = (useDialLValue & 0x0F);
	dialOutputs[2]  = (useDialRValue & 0xF0) >>4;
	dialOutputs[3]  = (useDialRValue & 0x0F);
	port_pin_set_output_level(TP1 , true);
	//dialOutputs[0]  = tempCount;
	//dialOutputs[1]  = tempCount+1;
	i2c_update_sensors_values();
	port_pin_set_output_level(TP1 , false);
	//dialUpdate  = true;
	//dialUpdate  = false;
	//testLogDialL[testCount] = useDialRValue;
	sensorsUpdate = false;
	//testLogDialR[testCount] = useDialRValue;
	testCount++;
	
	if(testCount == 500)
	{
		testCount = 0;
	}
	
}
/*
 * LEDMatrixSPIControl.c
 *
 * Created: 11/8/2018 4:46:24 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "LEDMatrixSPIControl.h"
#include "LEDMatrixSPIMaster.h"

extern volatile bool transferDone;


void LEDMatrixSend(uint8_t* receiveBuffer);
void LEDMatrixInit();
void LEDMatrixPinInit();

void resetShiftData()
{
	//MR pin low
	port_pin_set_output_level(RESET_PIN,false);
	//Delay
	delay_cycles_us(3);
	//MR pin high
	port_pin_set_output_level(RESET_PIN,true);
}

//Send Matrix Send
void LEDMatrixSend(uint8_t* receiveBuffer)
{
	setUpDMAData(receiveBuffer);
	resetShiftData();
	
	//SS Enable Low
	port_pin_set_output_level(SS_PIN,false);
	delay_cycles_us(1);
	//Send the serial data
	startTransferDMA();
	//SS Enable High 
	delay_cycles_us(1);
	port_pin_set_output_level(SS_PIN,true);
}

void LEDMatrixInit()
{
	LEDMatrixPinInit();
	port_pin_set_output_level(OUTPUT_ENABLE_PIN,true);
	resetShiftData();
	port_pin_set_output_level(OUTPUT_ENABLE_PIN,false);
}


void LEDMatrixPinInit()
{
	//MR Pin
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(RESET_PIN, &config_port_pin);
	
	
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(SS_PIN, &config_port_pin);
	
	
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(OUTPUT_ENABLE_PIN, &config_port_pin);

}


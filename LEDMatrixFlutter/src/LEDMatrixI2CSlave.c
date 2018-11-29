/*
 * I2CSlave.c
 *
 * Created: 11/19/2018 10:28:07 AM
 *  Author: raghu
 */ 

#include <asf.h>
#include "LEDMatrixI2CSlave.h"

void i2c_read_request_callback(struct i2c_slave_module *const module);
void i2c_write_request_callback(struct i2c_slave_module *const module);
void configure_i2c_slave(void);
void configure_i2c_slave_callbacks(void);


static struct i2c_slave_packet packet;


#define SLAVE_ADDRESS		  0x11
#define DATA_LENGTH_SENSORS   2
#define DATA_LENGTH_LEDS	  55

#define CONF_I2C_SLAVE_MODULE     SERCOM2

static uint8_t write_buffer[DATA_LENGTH_SENSORS] = {0x00, 0x00};
extern volatile uint8_t tempDialOutputs[2];
static uint8_t read_buffer [DATA_LENGTH_LEDS];
extern uint8_t I2CReadBuffer [DATA_LENGTH_LEDS];
extern volatile bool sensorsUpdate;

struct i2c_slave_module i2c_slave_instance;
extern uint8_t volatile useDialLValue;
extern uint8_t volatile useDialRValue;
extern volatile bool I2CWriteComplete;
extern volatile bool I2CReadComplete;

void i2c_read_request_callback(struct i2c_slave_module *const module)
{
	static uint8_t tempDialOutputs[2];
	static uint8_t count = 0;
	if(sensorsUpdate == false)
	{
		tempDialOutputs[0]	   = useDialLValue;
		tempDialOutputs[1]	   = useDialRValue;
	}
	packet.data_length = DATA_LENGTH_SENSORS;
	packet.data        = tempDialOutputs;
	

	i2c_slave_write_packet_job(module, &packet);
}


void i2c_write_request_callback(struct i2c_slave_module *const module)
{
	packet.data_length = DATA_LENGTH_LEDS;
	packet.data        = read_buffer;
	if (i2c_slave_read_packet_job(module, &packet) != STATUS_OK) {
	}
}

void i2c_write_complete_callback()
{
	I2CWriteComplete = true;
}

void i2c_read_complete_callback()
{
	uint8_t i =0;
	I2CReadComplete = true;
	for(i=0;i<DATA_LENGTH_LEDS;i++)
	{
		I2CReadBuffer[i] = read_buffer[i];
	}
	/*
	if(I2CReadBuffer[1] == 'M')
	*/
}

//! [initialize_i2c]
void configure_i2c_slave(void)
{
	struct i2c_slave_config config_i2c_slave;
	i2c_slave_get_config_defaults(&config_i2c_slave);

	config_i2c_slave.address      = SLAVE_ADDRESS;
	config_i2c_slave.address_mode = I2C_SLAVE_ADDRESS_MODE_MASK;
	
	config_i2c_slave.pinmux_pad0 =  PINMUX_PA22D_SERCOM2_PAD0;
	config_i2c_slave.pinmux_pad1 =  PINMUX_PA23D_SERCOM2_PAD1;
	
	i2c_slave_init(&i2c_slave_instance, CONF_I2C_SLAVE_MODULE , &config_i2c_slave);
	i2c_slave_enable(&i2c_slave_instance);
}


void configure_i2c_slave_callbacks(void)
{
	
	i2c_slave_register_callback(&i2c_slave_instance, i2c_read_request_callback,I2C_SLAVE_CALLBACK_READ_REQUEST);
	i2c_slave_enable_callback(&i2c_slave_instance,I2C_SLAVE_CALLBACK_READ_REQUEST);


	i2c_slave_register_callback(&i2c_slave_instance, i2c_write_request_callback,I2C_SLAVE_CALLBACK_WRITE_REQUEST);
	i2c_slave_enable_callback(&i2c_slave_instance,I2C_SLAVE_CALLBACK_WRITE_REQUEST);
	
	i2c_slave_register_callback(&i2c_slave_instance, i2c_write_complete_callback,I2C_SLAVE_CALLBACK_WRITE_COMPLETE);
	i2c_slave_enable_callback(&i2c_slave_instance,I2C_SLAVE_CALLBACK_WRITE_COMPLETE);
	
	i2c_slave_register_callback(&i2c_slave_instance, i2c_read_complete_callback,I2C_SLAVE_CALLBACK_READ_COMPLETE);
	i2c_slave_enable_callback(&i2c_slave_instance,I2C_SLAVE_CALLBACK_READ_COMPLETE);

}




/*
 * LEDMatrixI2CSlave.h
 *
 * Created: 11/19/2018 10:29:29 AM
 *  Author: raghu
 */ 


#ifndef LEDMATRIXI2CSLAVE_H_
#define LEDMATRIXI2CSLAVE_H_


void configure_i2c_slave(void);
void configure_i2c_slave_callbacks(void);
void i2c_update_sensors_values();




#endif /* LEDMATRIXI2CSLAVE_H_ */
#ifndef __I2C_H__
#define __I2C_H__
#include "MKL25Z4.h"
#include "stdint.h"

typedef enum{
	I2C_NACK = 0,
	I2C_ACK = 1
}i2c_ack_t;

typedef enum{
	I2C_WRITE = 0,
	I2C_READ = 1
}i2c_operation_t;

void init_i2c();
void I2C_START();
void I2C_STOP();
void I2C_RSTART();
void I2C_WAIT_IICIF();
i2c_ack_t I2C_RXAK();
void I2C_TX_ACK();
void I2C_TX_NACK();
void I2C_TRANSMIT_MODE();
void I2C_RECEIVE_MODE();
void I2C_SEND_BYTE(uint8_t byte);
uint8_t I2C_GET_ADDRESS(uint8_t addr, i2c_operation_t operation);
#endif

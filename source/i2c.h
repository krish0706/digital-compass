#ifndef __I2C_H__
#define __I2C_H__
#include "MKL25Z4.h"
#include "stdint.h"

void init_i2c();
typedef enum{
	I2C_NACK = 0,
	I2C_ACK = 1
}i2c_ack_t;

typedef enum{
	I2C_WRITE = 0,
	I2C_READ = 1
}i2c_operation_t;

static inline void I2C_START(){
	I2C1->C1 |= I2C_C1_MST_MASK;
}

static inline void I2C_STOP(){
	I2C1->C1 &= ~I2C_C1_MST_MASK;
}

static inline void I2C_RSTART(){
	I2C1->C1 |= I2C_C1_RSTA_MASK;
}

static inline void I2C_WAIT_TCF(){
	while((I2C1->S & I2C_S_TCF_MASK) == 0);
}

static inline void I2C_WAIT_IICIF(){
	while((I2C1->S & I2C_S_IICIF_MASK) == 0);
	I2C1->S |= I2C_S_IICIF_MASK;
}

static inline i2c_ack_t I2C_RXAK(){
	if(I2C1->S & I2C_S_RXAK_MASK){
		//no ack was received
		return I2C_NACK;
	}else{
		return I2C_ACK;
	}
}

static inline void I2C_TX_ACK(){
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;
}

static inline void I2C_TX_NACK(){
	I2C1->C1 |= I2C_C1_TXAK_MASK;
}

static inline void I2C_TRANSMIT_MODE(){
	I2C1->C1 |= I2C_C1_TX_MASK;
}

static inline void I2C_RECEIVE_MODE(){
	I2C1->C1 &= ~I2C_C1_TX_MASK;
}

static inline void I2C_SEND_BYTE(uint8_t byte){
	I2C1->D = byte;
}

static inline uint8_t I2C_GET_ADDRESS(uint8_t addr, i2c_operation_t operation){
	return (addr<<1 | operation);
}
#endif

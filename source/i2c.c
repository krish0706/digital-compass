#include "MKL25Z4.h"
#include "i2c.h"
#include "stdint.h"
#include "systick.h"

void I2C_START(){
	I2C1->C1 |= I2C_C1_MST_MASK;
}

void I2C_STOP(){
	I2C1->C1 &= ~I2C_C1_MST_MASK;
}

void I2C_RSTART(){
	I2C1->C1 |= I2C_C1_RSTA_MASK;
}

void I2C_WAIT_IICIF(){
	while((I2C1->S & I2C_S_IICIF_MASK) == 0);
	I2C1->S |= I2C_S_IICIF_MASK;
}

i2c_ack_t I2C_RXAK(){
	if(I2C1->S & I2C_S_RXAK_MASK){
		//no ack was received
		return I2C_NACK;
	}else{
		return I2C_ACK;
	}
}

void I2C_TX_ACK(){
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;
}

void I2C_TX_NACK(){
	I2C1->C1 |= I2C_C1_TXAK_MASK;
}

void I2C_TRANSMIT_MODE(){
	I2C1->C1 |= I2C_C1_TX_MASK;
}

void I2C_RECEIVE_MODE(){
	I2C1->C1 &= ~I2C_C1_TX_MASK;
}

void I2C_SEND_BYTE(uint8_t byte){
	I2C1->D = byte;
}

uint8_t I2C_GET_ADDRESS(uint8_t addr, i2c_operation_t operation){
	return (addr<<1 | operation);
}

void init_i2c(){
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[0] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_SRE_MASK);
	PORTE->PCR[0] = PORT_PCR_MUX(6);

	PORTE->PCR[1] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_SRE_MASK);
	PORTE->PCR[1] = PORT_PCR_MUX(6);

	I2C1->F &= ~(I2C_F_ICR_MASK | I2C_F_MULT_MASK);
	I2C1->F |= I2C_F_ICR(0x27);//psc of 480

	I2C1->C1 |= I2C_C1_IICEN_MASK;
}


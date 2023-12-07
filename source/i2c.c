#include "MKL25Z4.h"
#include "i2c.h"
#include "stdint.h"
#include "systick.h"

typedef enum{
	I2C_NACK = 0,
	I2C_ACK = 1
}i2c_ack_t;

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
	reset_tick();
	while((I2C1->S & I2C_S_IICIF_MASK) == 0){
		if(now()>=10){
			break;
		}
	}
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

void blocking_delay(uint8_t s){
	reset_tick();
	while(now()<(s<<1));
}
void I2C_WRITE_BYTE(uint8_t byte);
uint8_t I2C_READ_BYTE(uint8_t location);
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

//	while(1){
//		I2C_TRANSMIT_MODE();
//
//		do{
//			I2C_START();
//			I2C_SEND_BYTE(0xa0);
//			I2C_WAIT_IICIF();
//
//			I2C1->C1 |= I2C_C1_TXAK_MASK;
//			I2C_SEND_BYTE(0x33);
//			I2C_WAIT_TCF();
//
//		}while(!(I2C_RXAK()));
//		I2C_STOP();
//
//	}
	I2C_READ_BYTE(0x00);
	blocking_delay(2);
	I2C_WRITE_BYTE(0x55);

}

//void I2C_WRITE_BYTE(uint8_t byte){
//
//
//start:
//	I2C_TRANSMIT_MODE();
//	I2C_START();
//	I2C_SEND_BYTE(0xa0);//10100000 for eeprom write operation
//	I2C_WAIT_IICIF();
//
//	if(I2C_RXAK() == I2C_NACK){
//		goto start;
//	}
//
//	I2C_SEND_BYTE(0x00);//address to write to in eeprom
//	I2C_WAIT_IICIF();
//
//	if(I2C_RXAK() == I2C_NACK){
//		goto start;
//	}
//
//	I2C_SEND_BYTE(byte);//data to write to in eeprom
//	I2C_WAIT_IICIF();
//
//	if(I2C_RXAK() == I2C_NACK){
//		goto start;
//	}
//
//	I2C_STOP();
//
//}
//
//uint8_t I2C_READ_BYTE(uint8_t location){
//	uint8_t data = 0;
//start:
//	I2C_TRANSMIT_MODE();
//	I2C_START();
//	I2C_SEND_BYTE(0xa0);//10100000 for eeprom write operation to set initial location
//	I2C_WAIT_IICIF();
//
//	if(I2C_RXAK() == I2C_NACK){
//		goto start;
//	}
//
//	I2C_SEND_BYTE(location);//address to write to in eeprom
//	I2C_WAIT_IICIF();
//	if(I2C_RXAK() == I2C_NACK){
//		goto start;
//	}
//
//	I2C_RSTART();
//	I2C_SEND_BYTE(0xa1);//10100001 for eeprom read operation
//	I2C_WAIT_IICIF();
//	if(I2C_RXAK() == I2C_NACK){
//		goto start;
//	}
//
//	I2C_RECEIVE_MODE();
//	I2C_TX_NACK();
//
//	data = I2C1->D;//to start receive action of i2c
//	I2C_WAIT_IICIF();
//
//	I2C_STOP();
//	data = I2C1->D;//data will be in here now
//
//	return data;
//
//
//
//}

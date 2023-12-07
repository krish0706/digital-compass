#include "MKL25Z4.h"
#include "i2c.h"
#include "stdint.h"
#include "systick.h"

void blocking_delay(uint8_t s){
	reset_tick();
	while(now()<(s<<1));
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

////	while(1){
////		I2C_TRANSMIT_MODE();
////
////		do{
////			I2C_START();
////			I2C_SEND_BYTE(0xa0);
////			I2C_WAIT_IICIF();
////
////			I2C1->C1 |= I2C_C1_TXAK_MASK;
////			I2C_SEND_BYTE(0x33);
////			I2C_WAIT_TCF();
////
////		}while(!(I2C_RXAK()));
////		I2C_STOP();
////
////	}
//	I2C_READ_BYTE(0x00);
//	blocking_delay(2);
//	I2C_WRITE_BYTE(0x55);

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

/*******************************************************************************
 * Copyright (C) 2023 by Krish Shah
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Krish Shah and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    i2c.c
 * @brief   I2C Module driver code for FRDMKL25Z I2C Peripheral. It uses the I2C1 module.
 * 			PTE0 <--> SDA
 * 			PTE1 <--> SCL
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#include "MKL25Z4.h"
#include "i2c.h"
#include "stdint.h"
#include "systick.h"

#define ICR_PSC_480	0x27
#define I2C_PIN_ALT_FUNC_NUM 6
#define PTE_SDA_PIN_NUM 0
#define PTE_SCL_PIN_NUM 1

/*
 * Sends a start condition on the I2C line
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_START(){
	I2C1->C1 |= I2C_C1_MST_MASK;
}

/*
 * Sends a stop condition on the I2C line
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_STOP(){
	I2C1->C1 &= ~I2C_C1_MST_MASK;
}

/*
 * Sends a Restart condition on the on the I2C line
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_RSTART(){
	I2C1->C1 |= I2C_C1_RSTA_MASK;
}

/*
 * Blocking delay call to wait for event on I2C Lines
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_WAIT_IICIF(){
	while((I2C1->S & I2C_S_IICIF_MASK) == 0);
	I2C1->S |= I2C_S_IICIF_MASK;
}

/*
 * Function to check if an ack or nack was received after the previous transaction
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 for ack, 0 for nack
 */
i2c_ack_t I2C_RXAK(){
	if(I2C1->S & I2C_S_RXAK_MASK){
		//no ack was received
		return I2C_NACK;
	}else{
		return I2C_ACK;
	}
}

/*
 * Send a ack on the i2c lines
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_TX_ACK(){
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;
}

/*
 * Send a nack on the i2c lines
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_TX_NACK(){
	I2C1->C1 |= I2C_C1_TXAK_MASK;
}

/*
 * Set I2C peripheral into transmit mode
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_TRANSMIT_MODE(){
	I2C1->C1 |= I2C_C1_TX_MASK;
}

/*
 * Set I2C peripheral into receive mode
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_RECEIVE_MODE(){
	I2C1->C1 &= ~I2C_C1_TX_MASK;
}

/*
 * Send one byte of data on the I2C lines
 *
 * Parameters:
 *  byte the byte of data on the I2C lines
 *
 * Returns:
 *  none
 */
void I2C_SEND_BYTE(uint8_t byte){
	I2C1->D = byte;
}

/*
 * Function to calculate the i2c address based on read or write mode
 *
 * Parameters:
 *  addr 7 bit i2c address
 *  operation read/write operation
 *
 * Returns:
 *  8 bit i2c address
 */
uint8_t I2C_GET_ADDRESS(uint8_t addr, i2c_operation_t operation){
	return (addr<<1 | operation);
}

/*
 * Function to initialize the I2C1 peripheral on the FRDMKL25Z, and its corresponding pins.
 * 			PTE0 <--> SDA
 * 			PTE1 <--> SCL
 *
 * Core clock in this project is 48MHz. The mul is left to default value of 1(to avoid restart error as mentioned in the
 * errata sheet), and prescaler is set to 480 by using an ICR value of 0x27.
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void init_i2c(){
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[PTE_SDA_PIN_NUM] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_SRE_MASK);
	PORTE->PCR[PTE_SDA_PIN_NUM] = PORT_PCR_MUX(I2C_PIN_ALT_FUNC_NUM);

	PORTE->PCR[PTE_SCL_PIN_NUM] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_SRE_MASK);
	PORTE->PCR[PTE_SCL_PIN_NUM] = PORT_PCR_MUX(I2C_PIN_ALT_FUNC_NUM);

	I2C1->F &= ~(I2C_F_ICR_MASK | I2C_F_MULT_MASK);
	I2C1->F |= I2C_F_ICR(ICR_PSC_480);

	I2C1->C1 |= I2C_C1_IICEN_MASK;
}


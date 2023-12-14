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
 * @file    i2c.h
 * @brief   Header for I2C Module driver code for FRDMKL25Z I2C Peripheral. It uses the I2C1 module.
 * 			PTE0 <--> SDA
 * 			PTE1 <--> SCL
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#ifndef __I2C_H__
#define __I2C_H__
#include "stdint.h"

typedef enum{
	I2C_NACK = 0,
	I2C_ACK = 1
}i2c_ack_t;

typedef enum{
	I2C_WRITE = 0,
	I2C_READ = 1
}i2c_operation_t;

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
void init_i2c();

/*
 * Sends a start condition on the I2C line
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_START();

/*
 * Sends a stop condition on the I2C line
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_STOP();

/*
 * Sends a Restart condition on the on the I2C line
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_RSTART();

/*
 * Blocking delay call to wait for event on I2C Lines
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_WAIT_IICIF();

/*
 * Function to check if an ack or nack was received after the previous transaction
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 for ack, 0 for nack
 */
i2c_ack_t I2C_RXAK();

/*
 * Send a ack on the i2c lines
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_TX_ACK();

/*
 * Send a nack on the i2c lines
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_TX_NACK();

/*
 * Set I2C peripheral into transmit mode
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_TRANSMIT_MODE();

/*
 * Set I2C peripheral into receive mode
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void I2C_RECEIVE_MODE();

/*
 * Send one byte of data on the I2C lines
 *
 * Parameters:
 *  byte the byte of data on the I2C lines
 *
 * Returns:
 *  none
 */
void I2C_SEND_BYTE(uint8_t byte);

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
uint8_t I2C_GET_ADDRESS(uint8_t addr, i2c_operation_t operation);
#endif

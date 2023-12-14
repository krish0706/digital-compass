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
 * @file    ssd1306.c
 * @brief   Driver code for SSD1306 based 128x64 OLED Display
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#include "ssd1306.h"
#include "i2c.h"
#include "systick.h"
#include "stdint.h"
#include "font.h"
#include "string.h"
#include "stdio.h"

#define LSH_MUL_128 7
#define TEST_STATE_TIME 1000
#define DISPLAY_BUFFFER_LEN 1024
static uint8_t DISPLAY_BUFFER[DISPLAY_BUFFFER_LEN] = {0};




/*
 * Function to send one command to SSD1306 IC
 *
 * Parameters:
 *  cmd the byte value of the command to be sent
 *
 * Returns:
 *  1 on success
 *  0 on failure
 */
ssd1306_error_t ssd1306_send_one_cmd(uint8_t cmd){
	I2C_TRANSMIT_MODE();
	I2C_START();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(SSD1306_DEVICE_ADDR, I2C_WRITE));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return SSD1306_NACK_ERROR;
	}

	I2C_SEND_BYTE(SSD1306_CMD_BYTE_SEND_MULTIPLE_COMMANDS);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return SSD1306_NACK_ERROR;
	}

	I2C_SEND_BYTE(cmd);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return SSD1306_NACK_ERROR;
	}
	I2C_STOP();
	b_delay(10);//i2c lockup
	return SSD1306_OK;
}

/*
 * Function to initialise SSD1306 Display.
 * Follows the initilasation sequence provided in the IC Datasheet
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on Success
 *  0 on Failure
 */
ssd1306_error_t init_ssd1306(){
	ssd1306_send_one_cmd(SSD1306_SET_MULTIPLEX);
	ssd1306_send_one_cmd(SSD1306_MULTIPLEX_VALUE);

	ssd1306_send_one_cmd(SSD1306_SET_STARTLINE);

	ssd1306_send_one_cmd(SSD1306_SET_SEGMENT_REMAP_SEG0_COL127);

	ssd1306_send_one_cmd(SSD1306_INV_COM_SCAN);

	ssd1306_send_one_cmd(SSD1306_SET_COMPINS);
	ssd1306_send_one_cmd(SSD1306_COMPINS_VALUE);

	ssd1306_send_one_cmd(SSD1306_SET_CONTRAST_CONTROL);
	ssd1306_send_one_cmd(SSD1306_CONTRAST_CONTROL_VALUE);

	ssd1306_send_one_cmd(SSD1306_SET_DISPLAY_ON_RAM);
	ssd1306_send_one_cmd(SSD1306_SET_POSITIVE_DISPLAY);

	ssd1306_send_one_cmd(SSD1306_SET_CLOCK_DIV);
	ssd1306_send_one_cmd(SSD1306_CLOCK_DIV_VALUE);
	ssd1306_send_one_cmd(SSD1306_SET_CHARGE_PUMP);
	ssd1306_send_one_cmd(SSD1306_CHARGE_PUMP_VALUE);

	ssd1306_send_one_cmd(SSD1306_SET_DISPLAY_ON);

	ssd1306_clear_buffer();
	ssd1306_update_display();
	return SSD1306_OK;
}

/*
 * Function to update the entire display screen at once with new values present in the
 * buffer
 *
 * It first sets the page address and col address ranges, which also resets the respective pointer.
 * After that 1024 Bytes of buffer are transmitted to the display in one I2C Transaction
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on Success
 *  0 on Failure
 */
ssd1306_error_t ssd1306_update_display(){
	ssd1306_send_one_cmd(SSD1306_SET_MEMORY_ADDR_MODE);
	ssd1306_send_one_cmd(SSD1306_MEMORY_ADDR_MODE_HORI);

	ssd1306_send_one_cmd(SSD1306_SET_PAGE_ADDR);
	ssd1306_send_one_cmd(SSD1306_PAGE_START_ADDR);
	ssd1306_send_one_cmd(SSD1306_PAGE_END_ADDR);

	ssd1306_send_one_cmd(SSD1306_SET_COL_ADDR);
	ssd1306_send_one_cmd(SSD1306_COL_ADDR_START_ADDR);
	ssd1306_send_one_cmd(SSD1306_COL_ADDR_END_ADDR);

	I2C_TRANSMIT_MODE();
	I2C_START();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(SSD1306_DEVICE_ADDR, I2C_WRITE));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return SSD1306_NACK_ERROR;
	}

	I2C_SEND_BYTE(SSD1306_CMD_BYTE_SEND_MULTIPLE_DATA);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return SSD1306_NACK_ERROR;
	}

	for(int i = 0; i < DISPLAY_BUFFFER_LEN; i++){
		I2C_SEND_BYTE(DISPLAY_BUFFER[i]);
		I2C_WAIT_IICIF();
		if(I2C_RXAK() == I2C_NACK){
			return SSD1306_NACK_ERROR;
		}
	}
	I2C_STOP();
	b_delay(10);
	return SSD1306_OK;
}

/*
 * Function to clear the framebuffer by filling it with 0
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on successs
 *  0 on failure
 */
ssd1306_error_t ssd1306_clear_buffer(){
	memset(DISPLAY_BUFFER,0,DISPLAY_BUFFFER_LEN);
	return SSD1306_OK;
}
/*
 * Function to write a given string in the display buffer at a given
 * page and column location.
 *
 * Parameters:
 *  page the page at which the string should start
 *  column the coloumn at which the string should start
 *
 * Returns:
 *  1 on successs
 *  0 on failure
 */
ssd1306_error_t ssd1306_write_string_in_buffer(uint8_t page,uint8_t column,char *buf,uint8_t buf_len){
	if(page > SSD1306_PAGE_END_ADDR || column > SSD1306_COL_ADDR_END_ADDR){
		return SSD1306_BUFFER_ERROR;
	}
	uint16_t j = 0;
	uint16_t column_offset = 0, page_offset = 0;
	column_offset = column;//provides the x displacment of the string start from the top left of the screen
	page_offset = page<<LSH_MUL_128;//multiply by 128, this is used to control the y displacement of the string start from top left of screen
								    //it is multiplied by 128 becuase the first column on each page is a multiple of 128(0..128..256....)so
									//multiplication provides this offset
	while(buf_len--){
		for(int i = 0; i < FONT_SIZE; i++){
			DISPLAY_BUFFER[i+j+page_offset+column_offset] = FONT[(*buf)-FONT_LOOKUP_OFFSET][i];//offset from array index of font array
																							   //to value of character in ascii table
		}
		buf++;
		j+= FONT_SPACING;
	}
	return SSD1306_OK;
}

/*
 * Function to turn the display into a negative image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_set_negative_display(){
	ssd1306_send_one_cmd(SSD1306_SET_NEGATIVE_DISPLAY);
	return SSD1306_OK;
}

/*
 * Function to turn the display into a positive image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_set_positive_display(){
	ssd1306_send_one_cmd(SSD1306_SET_POSITIVE_DISPLAY);
	return SSD1306_OK;
}

/*
 * Function to turn the display into a forward image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_mirror_display_forward(){
	ssd1306_send_one_cmd(SSD1306_SET_SEGMENT_REMAP_SEG0_COL127);
	return SSD1306_OK;
}

/*
 * Function to turn the display into a mirror image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_mirror_display_reverse(){
	ssd1306_send_one_cmd(SSD1306_SET_SEGMENT_REMAP_SEG0_COL0);
	return SSD1306_OK;
}

/*
 * Function to turn the display back into a flipped upside down image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_flip_display_upward(){
	ssd1306_send_one_cmd(SSD1306_INV_COM_SCAN);
	ssd1306_mirror_display_forward();
	return SSD1306_OK;
}

/*
 * Function to turn the display into a straight image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_flip_display_downward(){
	ssd1306_send_one_cmd(SSD1306_NORMAL_COM_SCAN);
	ssd1306_mirror_display_reverse();
	return SSD1306_OK;
}

/*
 * Function to test the screen
 * It first prints all the characters from the font table
 * Then it changes the display to negative
 * Then it changed back to positive
 * After that the mirror and flip modes of the display are tested
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void test_ssd(){
	ssd1306_clear_buffer();
	char buf[FONT_TABLE_LEN] = {0};
	for(int i = 0; i < FONT_TABLE_LEN; i++){
		buf[i] = i + FONT_LOOKUP_OFFSET;
	}
	ssd1306_write_string_in_buffer(0, 0, buf, FONT_TABLE_LEN);
	ssd1306_set_negative_display();
	ssd1306_update_display();
	b_delay(TEST_STATE_TIME);
	ssd1306_set_positive_display();
	b_delay(TEST_STATE_TIME);
	ssd1306_mirror_display_reverse();
	b_delay(TEST_STATE_TIME);
	ssd1306_mirror_display_forward();
	b_delay(TEST_STATE_TIME);
	ssd1306_flip_display_downward();
	b_delay(TEST_STATE_TIME);
	ssd1306_flip_display_upward();
}

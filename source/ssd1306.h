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
 * @file    ssd1306.h
 * @brief   Header file for Driver code for SSD1306 based 128x64 OLED Display
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#ifndef __SSD1306_H__
#define __SSD1306_H__
#include "stdint.h"

#define SSD1306_DEVICE_ADDR 					(0x3CU)

#define SSD1306_CMD_BYTE_SEND_ONE_COMMAND 		(0x80U)
#define SSD1306_CMD_BYTE_SEND_MULTIPLE_COMMANDS (0x00U)
#define SSD1306_CMD_BYTE_SEND_MULTIPLE_DATA		(0x40U)
#define SSD1306_CMD_BYTE_SEND_ONE_DATA			(0xC0U)

#define SSD1306_SET_MEMORY_ADDR_MODE			(0x20U)
#define SSD1306_SET_COL_ADDR 					(0x21U)
#define SSD1306_SET_PAGE_ADDR					(0x22U)

#define SSD1306_MEMORY_ADDR_MODE_HORI			(0x00U)
#define SSD1306_COL_ADDR_START_ADDR				(0x00U)
#define SSD1306_COL_ADDR_END_ADDR				(0x7FU)
#define SSD1306_PAGE_START_ADDR					(0x00U)
#define SSD1306_PAGE_END_ADDR					(0x07U)

#define SSD1306_SET_CLOCK_DIV					(0xD5U)
#define SSD1306_CLOCK_DIV_VALUE					(0x80U)
#define SSD1306_SET_MULTIPLEX					(0xA8U)
#define SSD1306_MULTIPLEX_VALUE					(0x3FU)
#define SSD1306_SET_DISPLAY_OFFSET 				(0xD3U)
#define SSD1306_DISPLAY_OFFSET_VALUE			(0x00U)

#define SSD1306_SET_STARTLINE					(0x40U)
#define SSD1306_SET_COMPINS						(0xDAU)
#define SSD1306_COMPINS_VALUE					(0x12U)

#define SSD1306_SET_DISPLAY_ON_RAM				(0xA4U)
#define SSD1306_SET_DISPLAY_ON_IGNORE_RAM		(0xA5U)

#define SSD1306_SET_POSITIVE_DISPLAY			(0xA6U)
#define SSD1306_SET_NEGATIVE_DISPLAY			(0xA7U)
#define SSD1306_SET_DISPLAY_OFF					(0xAEU)
#define SSD1306_SET_DISPLAY_ON					(0xAFU)

#define SSD1306_SET_SEGMENT_REMAP_SEG0_COL0		(0xA0U)
#define SSD1306_SET_SEGMENT_REMAP_SEG0_COL127   (0xA1U)

#define SSD1306_NORMAL_COM_SCAN					(0xC0U)
#define SSD1306_INV_COM_SCAN					(0xC8U)

#define SSD1306_SET_CONTRAST_CONTROL			(0x81U)
#define SSD1306_CONTRAST_CONTROL_VALUE			(0x7FU)

#define SSD1306_SET_CHARGE_PUMP					(0x8DU)
#define SSD1306_CHARGE_PUMP_VALUE				(0x14U)

typedef enum{
	SSD1306_NACK_ERROR = 0,
	SSD1306_BUFFER_ERROR = 0,
	SSD1306_OK,
}ssd1306_error_t;

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
ssd1306_error_t init_ssd1306();

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
ssd1306_error_t ssd1306_update_display();

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
ssd1306_error_t ssd1306_clear_buffer();

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
ssd1306_error_t ssd1306_send_one_cmd(uint8_t cmd);

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
ssd1306_error_t ssd1306_write_string_in_buffer(uint8_t page,uint8_t column,char *buf,uint8_t buf_len);

/*
 * Function to turn the display into a negative image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_set_negative_display();

/*
 * Function to turn the display into a positive image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_set_positive_display();

/*
 * Function to turn the display back into a flipped upside down image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_flip_display_upward();

/*
 * Function to turn the display into a straight image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_flip_display_downward();

/*
 * Function to turn the display into a forward image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_mirror_display_forward();

/*
 * Function to turn the display into a mirror image
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  1 on success
 */
ssd1306_error_t ssd1306_mirror_display_reverse();

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
void test_ssd();
#endif

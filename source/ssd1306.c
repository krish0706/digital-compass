#include "ssd1306.h"
#include "i2c.h"
#include "systick.h"
#include "stdint.h"
#include "font.h"
#include "string.h"
#include "stdio.h"

#define DISPLAY_BUFFFER_LEN 1024

static uint8_t DISPLAY_BUFFER[DISPLAY_BUFFFER_LEN] = {0};


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

void init_ssd1306(){
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
}

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

void ssd1306_clear_buffer(){
	for(int i = 0; i < DISPLAY_BUFFFER_LEN; i++){
		DISPLAY_BUFFER[i] = 0x00;
	}
}

void ssd1306_write_string_in_buffer(uint8_t page,uint8_t column,char *buf,uint8_t buf_len){
	if(page > 7 || column > 127){
		return;
	}
	uint16_t j = 0;
	uint16_t column_offset = 0, page_offset = 0;
	column_offset = column;
	page_offset = page<<7;//multiply by 128
	while(buf_len--){
		for(int i = 0; i < FONT_SIZE; i++){
			DISPLAY_BUFFER[i+j+page_offset+column_offset] = FONT[(*buf)-FONT_LOOKUP_OFFSET][i];
		}
		buf++;
		j+= FONT_SPACING;
	}
}

void ssd1306_set_negative_display(){
	ssd1306_send_one_cmd(SSD1306_SET_NEGATIVE_DISPLAY);
}

void ssd1306_set_positive_display(){
	ssd1306_send_one_cmd(SSD1306_SET_POSITIVE_DISPLAY);
}

void ssd1306_mirror_display_forward(){
	ssd1306_send_one_cmd(SSD1306_SET_SEGMENT_REMAP_SEG0_COL127);
}

void ssd1306_mirror_display_reverse(){
	ssd1306_send_one_cmd(SSD1306_SET_SEGMENT_REMAP_SEG0_COL0);
}
//meaning of up down will depend on user
void ssd1306_flip_display_upward(){
	ssd1306_send_one_cmd(SSD1306_INV_COM_SCAN);
	ssd1306_mirror_display_forward();
}

void ssd1306_flip_display_downward(){
	ssd1306_send_one_cmd(SSD1306_NORMAL_COM_SCAN);
	ssd1306_mirror_display_reverse();
}

void test_ssd(){
	ssd1306_clear_buffer();
	char buf[FONT_TABLE_LEN] = {0};
	for(int i = 0; i < FONT_TABLE_LEN; i++){
		buf[i] = i + FONT_LOOKUP_OFFSET;
	}
	ssd1306_write_string_in_buffer(0, 0, buf, FONT_TABLE_LEN);
	ssd1306_set_negative_display();
	ssd1306_update_display();
	b_delay(1000);
	ssd1306_set_positive_display();
	b_delay(1000);
	ssd1306_mirror_display_reverse();
	b_delay(1000);
	ssd1306_mirror_display_forward();
	b_delay(1000);
	ssd1306_flip_display_downward();
	b_delay(1000);
	ssd1306_flip_display_upward();
}




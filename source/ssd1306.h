#ifndef __SSD1306_H__
#define __SSD1306_H__
#include "stdint.h"

#define SSD1306_DEVICE_ADDR (0x3CU)

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
	SSD1306_NACK_ERROR,
	SSD1306_OK,
}ssd1306_error_t;

void init_ssd1306();
void ssd1306_clear_buffer();
ssd1306_error_t ssd1306_send_one_cmd(uint8_t cmd);
void ssd1306_write_string_in_buffer(uint8_t page,uint8_t column,char *buf,uint8_t buf_len);
void test_ssd();
ssd1306_error_t ssd1306_update_display();
void ssd1306_set_negative_display();
void ssd1306_set_positive_display();
void ssd1306_flip_display_upward();
void ssd1306_flip_display_downward();
void ssd1306_mirror_display_forward();
void ssd1306_mirror_display_reverse();
#endif

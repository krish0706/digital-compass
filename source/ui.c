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
 * @file    ui.c
 * @brief   UI code for the Digital Compass Project. Used to render 2 screens:
 * 			1] Raw Data Reading
 * 			2] Calculated Compass Azimuth
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#include "ssd1306.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "systick.h"

/*
 * Function to calculate frame rate of the display. It measures the time from which it was previously called
 * to the current time, to find a delta value. 1s (1000ms) divided by this delta is the number of frames per
 * second
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  value of calculated frame rate
 */
uint32_t get_frame_rate(){
	static ticktime_t previous_frame_tick = 0;
	uint32_t time_diff = now() - previous_frame_tick;
	uint32_t frame_rate = (1000/time_diff);
	previous_frame_tick = now();
	return frame_rate;
}

/*
 * Function to render the raw reading screen on the display, based on provided x, y and z values
 *
 * Parameters:
 *  x integer value of raw x reading
 *  y integer value of raw y reading
 *  z integer value of raw z reading
 *
 * Returns:
 *  none
 */
void display_raw_reading_display(int16_t x, int16_t y,int16_t z){
	char buf[100];

	ssd1306_clear_buffer();

	sprintf(buf,"Raw Sensor Values:");
	ssd1306_write_string_in_buffer(0, 0, buf, strlen(buf));

	sprintf(buf,"X:%d",x);
	ssd1306_write_string_in_buffer(1, 0, buf, strlen(buf));

	sprintf(buf,"Y:%d",y);
	ssd1306_write_string_in_buffer(2, 0, buf, strlen(buf));

	sprintf(buf,"Z:%d",z);
	ssd1306_write_string_in_buffer(3, 0, buf, strlen(buf));

	sprintf(buf,"Frame Rate:%d",get_frame_rate());
	ssd1306_write_string_in_buffer(7, 0, buf, strlen(buf));


	ssd1306_update_display();
}

/*
 * Function to render the calculated azimuth value on the display
 *
 * Parameters:
 *  direction value of the azimuth to display on the screen
 *
 * Returns:
 *  none
 */
void display_direction_display(int16_t direction){
	char buf[100];

	ssd1306_clear_buffer();

	sprintf(buf,"Direction:");
	ssd1306_write_string_in_buffer(0, 0, buf, strlen(buf));

	sprintf(buf,"%d Degrees",direction);
	ssd1306_write_string_in_buffer(1, 0, buf, strlen(buf));

	ssd1306_update_display();
}

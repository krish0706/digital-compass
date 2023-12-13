#include "ssd1306.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "systick.h"

uint32_t get_frame_rate(){
	static ticktime_t previous_frame_tick = 0;
	uint32_t time_diff = now() - previous_frame_tick;
	uint32_t frame_rate = (1000/time_diff);
	previous_frame_tick = now();
	return frame_rate;
}

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

void display_direction_display(int16_t direction){
	char buf[100];

	ssd1306_clear_buffer();

	sprintf(buf,"Direction:");
	ssd1306_write_string_in_buffer(0, 0, buf, strlen(buf));

	sprintf(buf,"%d Degrees",direction);
	ssd1306_write_string_in_buffer(1, 0, buf, strlen(buf));

	ssd1306_update_display();
}

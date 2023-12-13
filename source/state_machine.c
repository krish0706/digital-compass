#include "stdint.h"
#include "systick.h"
#include "ssd1306.h"
#include "QMC5883L.h"
#include "fsl_debug_console.h"
#include "ui.h"
#include "math.h"

#define PI 3.14159

#define TEST_DISPLAY_DURATION 	   2500
#define RAW_DISPLAY_DURATION  	   5000
#define DIRECTION_DISPLAY_DURATION 5000

typedef enum{
	TEST_DISPLAY,
	RAW_DISPLAY,
	DIRECTION_DISPLAY
}state_t;

typedef struct{
	state_t current_state;
	ticktime_t state_start_time;
	uint32_t step;
	int timer_elapsed_event_flag;
}state_info_t;

typedef void (*callback_t)(state_info_t *state_machine);
void test_display_callback(state_info_t *state_machine);
void raw_display_callback(state_info_t *state_machine);
void direction_display_callback(state_info_t *state_machine);


typedef struct{
	callback_t action_transition_in;
	state_t TIMER_ELAPSED_next_state;
}state_table_entry_t;

state_table_entry_t state_table[] = {
		{test_display_callback,RAW_DISPLAY},
		{raw_display_callback,DIRECTION_DISPLAY},
		{direction_display_callback,RAW_DISPLAY}
};

void test_display_callback(state_info_t *state_machine){
	test_ssd();
	if(now() - state_machine->state_start_time > TEST_DISPLAY_DURATION){
		state_machine->timer_elapsed_event_flag = 1;
	}
}
void raw_display_callback(state_info_t *state_machine){
	int16_t result[3];
	qmc_get_nex_raw_sample(result);
	display_raw_reading_display(result[0], result[1], result[2]);
	if(now() - state_machine->state_start_time > RAW_DISPLAY_DURATION){
		state_machine->timer_elapsed_event_flag = 1;
	}
}
void direction_display_callback(state_info_t *state_machine){
	int16_t result[3];
	double direction = 0;
	qmc_get_nex_raw_sample(result);
	qmc_calibrate_data(result);
	direction = atan2(result[1],result[0]);
	if(direction < 0){
		direction = (direction)*180/PI + 360;
	}else{
		direction = (direction)*180/PI;
	}
	display_direction_display(direction);
	if(now() - state_machine->state_start_time > DIRECTION_DISPLAY_DURATION){
		state_machine->timer_elapsed_event_flag = 1;
	}
}

void run_state_machine(){
	state_info_t state_machine;
	state_machine.current_state = TEST_DISPLAY;
	state_machine.timer_elapsed_event_flag = 0;
	state_machine.step = 0;
	state_machine.state_start_time = now();

	while(1){
		if(state_machine.timer_elapsed_event_flag){
			state_machine.timer_elapsed_event_flag = 0;
			state_machine.current_state = state_table[state_machine.current_state].TIMER_ELAPSED_next_state;
			state_machine.state_start_time = now();
			PRINTF("ENTERING STATE %d at %d\r\n",state_machine.current_state,now());
		}

		state_table[state_machine.current_state].action_transition_in(&state_machine);
	}
}

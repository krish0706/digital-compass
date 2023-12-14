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
 * @file    state_machine.c
 * @brief   State machine to switch the display between 3 modes of operations:
 * 			1] Testing State
 * 			2] Raw Data Display State
 * 			3] Direction Display State
 *
 * 			The State transitions follow this pattern:
 *
 * 			T->R
 * 			R->D
 * 			D->R
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
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
#define PI_RADIAN_IN_DEGREES	    180
#define TWO_PI_RADIAN_IN_DEGREES	360

typedef enum{
	TEST_DISPLAY,
	RAW_DISPLAY,
	DIRECTION_DISPLAY
}state_t;

typedef struct{
	state_t current_state;
	ticktime_t state_start_time;
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

/*
 * Callback function which runs on entering the testing state
 *
 * Parameters:
 *  state_machine pointer to structure holding state variables
 *
 * Returns:
 *  none
 */
void test_display_callback(state_info_t *state_machine)
{
	test_ssd();
	if(now() - state_machine->state_start_time > TEST_DISPLAY_DURATION)
	{
		state_machine->timer_elapsed_event_flag = 1;
	}
}

/*
 * Callback function which runs on entering the raw data display state
 *
 * Parameters:
 *  state_machine pointer to structure holding state variables
 *
 * Returns:
 *  none
 */
void raw_display_callback(state_info_t *state_machine)
{
	int16_t result[3];
	qmc_get_nex_raw_sample(result);
	display_raw_reading_display(result[AXIS_X], result[AXIS_Y], result[AXIS_Z]);
	if(now() - state_machine->state_start_time > RAW_DISPLAY_DURATION)
	{
		state_machine->timer_elapsed_event_flag = 1;
	}
}

/*
 * Callback function which runs on entering the direction display state
 *
 * Parameters:
 *  state_machine pointer to structure holding state variables
 *
 * Returns:
 *  none
 */
void direction_display_callback(state_info_t *state_machine)
{
	int16_t result[3];
	double direction = 0;
	qmc_get_nex_raw_sample(result);
	qmc_calibrate_data(result);
	direction = atan2(result[AXIS_Y],result[AXIS_X]);//atan2 is discontinuous at 180 degrees thus 360*
													 //should be added to it if value is less than 0
													 //during the radian to degree conversion
	if(direction < 0)
	{
		direction = ((direction)*PI_RADIAN_IN_DEGREES/PI) + TWO_PI_RADIAN_IN_DEGREES;
	}else{
		direction = (direction)*PI_RADIAN_IN_DEGREES/PI;
	}
	display_direction_display(direction);
	if(now() - state_machine->state_start_time > DIRECTION_DISPLAY_DURATION){
		state_machine->timer_elapsed_event_flag = 1;
	}
}
/*
 * Function to run the state machine. Each state transition happens when the timer_elapsed_event_flag is set
 * Runs forever.
 * The State transitions follow this pattern:
 *
 * 			T->R
 * 			R->D
 * 			D->R
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void run_state_machine()
{
	state_info_t state_machine;
	state_machine.current_state = TEST_DISPLAY;
	state_machine.timer_elapsed_event_flag = 0;
	state_machine.state_start_time = now();

	while(1)
	{
		if(state_machine.timer_elapsed_event_flag)
		{
			state_machine.timer_elapsed_event_flag = 0;
			state_machine.current_state = state_table[state_machine.current_state].TIMER_ELAPSED_next_state;
			state_machine.state_start_time = now();
			PRINTF("ENTERING STATE %d at %d\r\n",state_machine.current_state,now());
		}

		state_table[state_machine.current_state].action_transition_in(&state_machine);
	}
}

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
 * @file    systick.c
 * @brief   Systick driver code. Here, the systick is configured
 * 			to generate a tick every 1ms
 *
 *
 * @author  Krish Shah
 * @date    December 1 2023
 *
 */
#include "systick.h"
#include "board.h"
#include "core_cm0plus.h"


#define SYSTICK_LOAD_VALUE 3000
#define MUX_GPIO 1

ticktime_t tick = 0;
ticktime_t clock_tick = 0;
/*
 * Initializes the Systick timer. It is configured to generate an interrupt every 1ms which is used to
 * increment the tick variable. If CLKSOURCE bit is 0, core-clock/16 is the systick clock
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void init_systick()
{
	SysTick->LOAD = SYSTICK_LOAD_VALUE;
	SysTick->VAL = 0;//to clear SYST_CVR, since startup value is unknown
	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;//to configure systick clock to 3MHz
																	    //and to enable the systick timer
}

/*
 * A function to get the current number of ticks elapsed in the system since startup
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  ticktime_t returns the current number of ticks elapsed
 */
ticktime_t now()
{
	return tick;
}

/*
 * A function reset the tick counter
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void reset_tick()
{
	tick = 0;
}

/*
 * A function reset the clock tick counter
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void reset_clock_tick(){
	clock_tick = tick;
}

/*
 * A function get the ticks elapsed since last call to reset_clock_tick()
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  ticks elapsed since last call to reset_clock_tick()
 */
ticktime_t get_clock_tick(){
	return (tick-clock_tick);
}

/*
 * A function for blocking delay call in mseconds
 *
 * Parameters:
 *  seconds interger value of number of ms to delay
 *
 * Returns:
 *  none
 */
void b_delay(int ms){
	reset_clock_tick();
	while(get_clock_tick()<(ms));
}

/*
 * Systick Interrupt Handler is used to increment the value of the ticks variable .
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void SysTick_Handler()
{
	tick++;
}

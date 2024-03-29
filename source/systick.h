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
 * @file    systick.h
 * @brief   Header file for Systick Driver.Here, the systick is configured
 * 			to generate a tick every 1ms
 *
 *
 * @author  Krish Shah
 * @date    December 1 2023
 *
 */
#ifndef __SYSTICK_H__
#define __SYSTICK_H__
#include "board.h"

typedef volatile uint32_t ticktime_t;

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
void init_systick();

/*
 * A function to get the current number of ticks elapsed in the system since startup
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  ticktime_t returns the current number of ticks elapsed
 */
ticktime_t now();

/*
 * A function reset the tick counter
 *
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 */
void reset_tick();

/*
 * A function for blocking delay call in mseconds
 *
 * Parameters:
 *  seconds interger value of number of mseconds to delay
 *
 * Returns:
 *  none
 */
void b_delay(int ms);
#endif

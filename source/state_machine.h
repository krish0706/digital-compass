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
 * @file    state_machine.h
 * @brief   Header file for State machine code to switch the display between 3 modes of operations:
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
#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

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
void run_state_machine();
#endif

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
 * @file    ui.h
 * @brief   Header file for UI code for the Digital Compass Project. Used to render 2 screens:
 * 			1] Raw Data Reading
 * 			2] Calculated Compass Azimuth
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#ifndef __UI_H__
#define __UI_H__
#include "stdint.h"

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
void display_raw_reading_display(int16_t x, int16_t y,int16_t z);

/*
 * Function to render the calculated azimuth value on the display
 *
 * Parameters:
 *  direction value of the azimuth to display on the screen
 *
 * Returns:
 *  none
 */
void display_direction_display(int16_t direction);
#endif

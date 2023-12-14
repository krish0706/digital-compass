/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
 * @file    main.c
 * @brief   main program file for digital compass project.
 * 			Completed for ECEN5813 Final Project.
 * 			Initializes modules and then runs the state machine.
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "board.h"
#include "pin_mux.h"
#include "QMC5883L.h"
#include "ssd1306.h"
#include "state_machine.h"
#include "i2c.h"
#include "systick.h"

#undef CALIBRATION_MODE//change to #define to dump calibration data on the terminal and to #undef to run state machine.

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Init Modules. */
    init_systick();
    init_i2c();
    init_ssd1306();

	qmc_config_t config;
	config.int_enb = INT_ENB_DISABLE;
	config.rol_pnt = ROL_PNT_DISABLE;
	config.soft_rst = SOFT_RST_DISABLE;
	config.osr = OSR_OPTION_512;
	config.rng = RNG_OPTION_8G;
	config.odr = ODR_OPTION_200HZ;
	config.mode = MODE_OPTION_CONTINUOUS;
	init_qmc(&config);
#ifdef CALIBRATION_MODE
	qmc_dump_calibration_data(1024);
	while(1);//block after return from dump calibration
#else
	run_state_machine();
#endif
}

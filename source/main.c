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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "board.h"

#include "pin_mux.h"
#include "i2c.h"
#include "systick.h"
#include "QMC5883L.h"
#include "ssd1306.h"
#include "math.h"
#include "stdint.h"
#include "ui.h"
#define PI 3.14159
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    init_systick();

    PRINTF("hello world.\r\n");
    init_i2c();
    init_ssd1306();
    test_ssd();

    ssd1306_set_negative_display();
    ssd1306_set_positive_display();
    ssd1306_flip_display_downward();
    ssd1306_flip_display_upward();
    ssd1306_mirror_display_reverse();
    ssd1306_mirror_display_forward();

    display_raw_reading_screen(0, 100, 1000);
    display_raw_reading_screen(-0, -100, -1000);
    display_direction_screen(0);
    display_direction_screen(90);

    while(1);

//    while(1);

//    qmc_config_t config;
//    config.int_enb = INT_ENB_DISABLE;
//    config.rol_pnt = ROL_PNT_DISABLE;
//    config.soft_rst = SOFT_RST_DISABLE;
//    config.osr = OSR_OPTION_512;
//    config.rng = RNG_OPTION_8G;
//    config.odr = ODR_OPTION_200HZ;
//    config.mode = MODE_OPTION_CONTINUOUS;
//    init_qmc(&config);
////    float bias[3];
////    int16_t raw_result[3];
////    qmc_dump_calibration_data(300);
////    qmc_run_calibration(100, bias);
////    PRINTF("CALIBRATION DONE\r\n");
////    while (1)
////    {
//////    	qmc_get_nex_raw_sample(raw_result);
//////		for(int i = 0; i < 3;i++){
//////			if(raw_result[i] < 0){
//////				PRINTF("Axis:%d Data:-%i\r\n",i,raw_result[i]);
//////			}else{
//////				PRINTF("Axis:%d Data:%i\r\n",i,raw_result[i]);
//////			}
//////		}
////    }
//    int16_t result[3] = {0};
//        qmc_config_t config;
//        config.int_enb = INT_ENB_DISABLE;
//        config.rol_pnt = ROL_PNT_DISABLE;
//        config.soft_rst = SOFT_RST_DISABLE;
//        config.osr = OSR_OPTION_512;
//        config.rng = RNG_OPTION_8G;
//        config.odr = ODR_OPTION_200HZ;
//        config.mode = MODE_OPTION_CONTINUOUS;
//        init_qmc(&config);
//    while(1){
//    	qmc_get_nex_raw_sample(result);
//    	qmc_calibrate_data(result);
////		for(int i = 0; i < 3;i++){
////			if(result[i] < 0){
////				PRINTF("Axis:%d Data:-%i\r\n",i,result[i]);
////			}else{
////				PRINTF("Axis:%d Data:%i\r\n",i,result[i]);
////			}
////		}
//    	double angle = 0;
//    	angle = atan2(result[1],result[0]);
//    	if(angle < 0){
//    		angle = (angle)*180/PI + 360;
//    		PRINTF("%d Degrees\r\n",(int)angle);
//    	}else{
//    		angle = (angle)*180/PI;
//    		PRINTF("%d Degrees\r\n",(int)angle);
//    	}
//    }
}

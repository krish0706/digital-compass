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
 * @file    qmc5883l.c
 * @brief   Driver Code for QMC5883L Magnetometer IC.
 *
 * @author  Krish Shah
 * @date    December 13 2023
 *
 */
#include "i2c.h"
#include "QMC5883L.h"
#include "fsl_debug_console.h"
#include "systick.h"

#define BYTE_SHIFT 8
#define NUM_DOUT_BUFFER 6

qmc_calibration_data_t calibration_data = {
		.offset_x = OFFSET_X,
		.offset_y = OFFSET_Y,
		.offset_z = OFFSET_Z,
		.scale_x = SCALE_X,
		.scale_y = SCALE_Y,
		.scale_z = SCALE_Z,
};

/*
 * Function to write specified data into the specified register on
 * the qmc5883l
 *
 * Parameters:
 *  reg the address of the register where data has to be written
 *  data the data that has to written
 *
 * Returns:
 *  1 for success
 *  0 for failure
 */
qmc_error_t qmc_i2c_write_reg(uint8_t reg,uint8_t data){
	I2C_TRANSMIT_MODE();
	I2C_START();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(QMC_DEVICE_ADDR, I2C_WRITE));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_SEND_BYTE(reg);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_SEND_BYTE(data);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_STOP();


	return QMC_OK;
}

/*
 * Function to read data from a specific register on the qmc5883l
 *
 * Parameters:
 *  reg the address of the register from which data has to read
 *	data(out) pointer to byte to store the data that was read
 *
 * Returns:
 *  1 for success
 *  0 for failure
 */
qmc_error_t qmc_i2c_read_reg(uint8_t reg,uint8_t* data){
	I2C_TRANSMIT_MODE();
	I2C_START();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(QMC_DEVICE_ADDR, I2C_WRITE));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_SEND_BYTE(reg);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_RSTART();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(QMC_DEVICE_ADDR,I2C_READ));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}
	I2C_RECEIVE_MODE();
	I2C_TX_NACK();

	*data = I2C1->D;//to start receive action of i2c
	I2C_WAIT_IICIF();

	I2C_STOP();
	*data = I2C1->D;//data will be available now

	return QMC_OK;
}

/*
 * Function to read multiple bytes of data, starting from a specific register
 * on the qmc5883l. It uses the internal pointer increment feature of the qmc5883l
 * which increments the pointer to next register when we read one register, allowing
 * us to chain reads into a single operation, without having to specify the
 * address again and again
 *
 * Parameters:
 *  reg the address of the register from which data read starts
 *	buf(out) pointer to byte array to store the data that was read
 *	buf_len	number of bytes to read from the device, must be equal to len(buf)
 *
 * Returns:
 *  1 for success
 *  0 for failure
 */
qmc_error_t qmc_i2c_read_regs(uint8_t reg,uint8_t buf[],uint8_t buf_len){
	I2C_TRANSMIT_MODE();
	I2C_START();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(QMC_DEVICE_ADDR, I2C_WRITE));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_SEND_BYTE(reg);
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}

	I2C_RSTART();
	I2C_SEND_BYTE(I2C_GET_ADDRESS(QMC_DEVICE_ADDR,I2C_READ));
	I2C_WAIT_IICIF();
	if(I2C_RXAK() == I2C_NACK){
		return QMC_NACK_ERROR;
	}
	I2C_RECEIVE_MODE();

	int i = 0;
	while(i < buf_len){
		if(i == buf_len-1){//if on last read, master transmits nack to stop reading
			I2C_TX_NACK();
		}else{
			I2C_TX_ACK();
		}
		buf[i] = I2C1->D;//to start receive action of i2c
		I2C_WAIT_IICIF();
		if(i == buf_len - 1){
			I2C_STOP();//if not stopped here, reading d will start next fetch
		}
		buf[i] = I2C1->D;//data will be available now
		i++;
	}
	return QMC_OK;
}

/*
 * Function to set the OSR bit in CR1
 *
 * Parameters:
 *  option the choice of osr setting
 *  CR1(out) pointer to byte containing CR1 value
 *
 * Returns:
 *  none
 */
void setOSR(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_OSR_MASK;
	*CR1 |= option<<CR1_OSR_SHIFT;
}

/*
 * Function to set the RNG bit in CR1
 *
 * Parameters:
 *  option the choice of rng setting
 *  CR1(out) pointer to byte containing CR1 value
 *
 * Returns:
 *  none
 */
void setRNG(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_RNG_MASK;
	*CR1 |= option<<CR1_RNG_SHIFT;
}

/*
 * Function to set the odr bit in CR1
 *
 * Parameters:
 *  option the choice of odr setting
 *  CR1(out) pointer to byte containing CR1 value
 *
 * Returns:
 *  none
 */
void setODR(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_ODR_MASK;
	*CR1 |= option<<CR1_ODR_SHIFT;
}

/*
 * Function to set the mode bit in CR1
 *
 * Parameters:
 *  option the choice of mode setting
 *  CR1(out) pointer to byte containing CR1 value
 *
 * Returns:
 *  none
 */
void setMODE(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_MODE_MASK;
	*CR1 |= option<<CR1_MODE_SHIFT;
}

/*
 * Function to set the soft_rst bit in CR2
 *
 * Parameters:
 *  option the choice of soft_rst setting
 *  CR2(out) pointer to byte containing CR2 value
 *
 * Returns:
 *  none
 */
void setSOFT_RST(qmc_cr2_soft_rst_options_t option,uint8_t* CR2){
	*CR2 &= ~CR2_SOFT_RST_MASK;
	*CR2 |= option<<CR2_SOFT_RST_SHIFT;
}

/*
 * Function to set the rol_pnt bit in CR2
 *
 * Parameters:
 *  option the choice of rol_pnt setting
 *  CR2(out) pointer to byte containing CR2 value
 *
 * Returns:
 *  none
 */
void setROL_PNT(qmc_cr2_rol_pnt_options_t option,uint8_t* CR2){
	*CR2 &= ~CR2_ROL_PNT_MASK;
	*CR2 |= option<<CR2_ROL_PNT_SHIFT;
}

/*
 * Function to set the int_enb bit in CR2
 *
 * Parameters:
 *  option the choice of int_enb setting
 *  CR2(out) pointer to byte containing CR2 value
 *
 * Returns:
 *  none
 */
void setINT_ENB(qmc_cr2_int_enb_options_t option,uint8_t* CR2){
	*CR2 &= ~CR2_INT_ENB_MASK;
	*CR2 |= option<<CR2_INT_ENB_SHIFT;
}

/*
 * Function to get osr value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR1 register
 *
 * Returns:
 *  OSR value
 */
uint8_t getOSR(uint8_t reg_value){
	return ((reg_value & CR1_OSR_MASK)>>CR1_OSR_SHIFT);
}

/*
 * Function to get rng value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR1 register
 *
 * Returns:
 *  rng value
 */
uint8_t getRNG(uint8_t reg_value){
	return ((reg_value & CR1_RNG_MASK)>>CR1_RNG_SHIFT);
}

/*
 * Function to get odr value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR1 register
 *
 * Returns:
 *  ODR value
 */
uint8_t getODR(uint8_t reg_value){
	return ((reg_value & CR1_ODR_MASK)>>CR1_ODR_SHIFT);
}

/*
 * Function to get mode value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR1 register
 *
 * Returns:
 *  MODE value
 */
uint8_t getMODE(uint8_t reg_value){
	return ((reg_value & CR1_MODE_MASK)>>CR1_MODE_SHIFT);
}

/*
 * Function to get DOR value
 *
 * Parameters:
 *  reg_value	the byte containing value of the SR register
 *
 * Returns:
 *  DOR value
 */
uint8_t getDOR(uint8_t reg_value){
	return (reg_value & SR_DOR_MASK)>>SR_DOR_SHIFT;
}

/*
 * Function to get ovl value
 *
 * Parameters:
 *  reg_value	the byte containing value of the SR register
 *
 * Returns:
 *  OVL value
 */
uint8_t getOVL(uint8_t reg_value){
	return (reg_value & SR_OVL_MASK)>>SR_OVL_SHIFT;
}

/*
 * Function to get DRDY value
 *
 * Parameters:
 *  reg_value	the byte containing value of the SR register
 *
 * Returns:
 *  DRDY value
 */
uint8_t getDRDY(uint8_t reg_value){
	return (reg_value & SR_DRDY_MASK)>>SR_DRDY_SHIFT;
}

/*
 * Function to get soft_rst value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR2 register
 *
 * Returns:
 *  soft_rst value
 */
uint8_t getSOFT_RST(uint8_t reg_value){
	return (reg_value & CR2_SOFT_RST_MASK)>>CR2_SOFT_RST_SHIFT;
}

/*
 * Function to get rol_pnt value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR2 register
 *
 * Returns:
 *  rol_pnt value
 */
uint8_t getROL_PNT(uint8_t reg_value){
	return (reg_value & CR2_ROL_PNT_MASK)>>CR2_ROL_PNT_SHIFT;
}

/*
 * Function to get int_enb value
 *
 * Parameters:
 *  reg_value	the byte containing value of the CR2 register
 *
 * Returns:
 *  int_enb value
 */
uint8_t getINT_ENB(uint8_t reg_value){
	return (reg_value & CR2_INT_ENB_MASK)>>CR2_INT_ENB_SHIFT;
}

/*
 * Function to contatenate the 8 bit MSB and LSB into one 16 Bit
 * signed variable
 *
 * Parameters:
 *  MSB byte value of most significant byte
 *  LSB	byte value of least significant byte
 *
 * Returns:
 *  16-bit signed variable formed by contatenating MSB and LSB
 */
static inline int16_t concatenate_bytes(uint8_t MSB, uint8_t LSB){
	return (int16_t)((MSB<<BYTE_SHIFT) | LSB);
}

/*
 *	Function to process raw data from the registers of the IC to form 16-bit signed
 *	result. Register 0 is X_LSB and 1 is X_MSB, put together they form a 16-bit signed
 *	variable representing the reading at the x-axis
 *
 * Parameters:
 * 	data(in) pointer to raw byte data of 6 register(MSB and LSB register for each axis: x,y,z)
 * 	result(out) pointer to signed 16-bit result
 *
 * Returns:
 *  none
 */
void process_raw_data(uint8_t data[],int16_t result[]){
	uint8_t MSB, LSB;
	for(int i = AXIS_X; i <= AXIS_Z; i++){
		MSB = data[2*i+1];//odd number elements are MSB(1..3..5)
		LSB = data[2*i];//even number elements as LSB(0..2..4)
		result[i] = concatenate_bytes(MSB, LSB);
	}
}

/*
 * Function to calibrate data according to the calculated value of scale and bias
 *
 * calibrated value for an axis = (scale*(axis_value - offset))
 *
 * Parameters:
 *  data(in/out) pointer to data array which is processed and calibrated
 *
 * Returns:
 *  none
 */
void qmc_calibrate_data(int16_t data[]){
	data[0] = (calibration_data.scale_x*(data[AXIS_X] - calibration_data.offset_x));
	data[1] = (calibration_data.scale_y*(data[AXIS_Y] - calibration_data.offset_y));
	data[2] = (calibration_data.scale_z*(data[AXIS_Z] - calibration_data.offset_z));
}

/*
 * Function to inialise the QMC module accoring to the config provided
 *
 * Parameters:
 *  config pointer to config structure containing the config for the device
 *
 * Returns:
 *  none
 */
void init_qmc(qmc_config_t *config){
	uint8_t cr1 = 0, cr2 = 0;

	setMODE(config->mode, &cr1);
	setODR(config->odr,&cr1);
	setRNG(config->rng,&cr1);
	setOSR(config->osr,&cr1);

	setSOFT_RST(config->soft_rst, &cr2);
	setROL_PNT(config->rol_pnt, &cr2);
	setINT_ENB(config->int_enb, &cr2);

	//write default value into srs period register
	while(qmc_i2c_write_reg(QMC_SRS_PERIOD_ADDR,QMC_SRS_PERIOD_DEFAULT_VALUE)!= QMC_OK);
	b_delay(10);//to prevent I2C on KL25Z from locking up

	//write cr1 register
	while(qmc_i2c_write_reg(QMC_CR1_ADDR,cr1)!= QMC_OK);
	b_delay(10);//to prevent I2C on KL25Z from locking up

	//write cr2 register
	while(qmc_i2c_write_reg(QMC_CR2_ADDR,cr2)!= QMC_OK);
	b_delay(10);//to prevent I2C on KL25Z from locking up
}

/*
 * Function to get next raw sample from QMC5883L IC
 *
 * Parameters:
 *  result(out) pointer to 16-bit integer array to collect the raw sample values
 *
 * Returns:
 *  1 on success
 *  0 on failure
 */
qmc_error_t qmc_get_nex_raw_sample(int16_t result[]){
	qmc_error_t ret;
	uint8_t sr = 0;
	uint8_t dout_buffer[NUM_DOUT_BUFFER];
	while(1){
		ret = qmc_i2c_read_reg(QMC_SR_ADDR,&sr);
		b_delay(10);//to prevent I2C on KL25Z from locking up
		if(ret == QMC_OK){
			if(getDOR(sr)){
				ret = QMC_ERROR_DOR;
			}
			if(getOVL(sr)){
				ret = QMC_ERROR_OVL;
			}
			if(getDRDY(sr)){
				while(qmc_i2c_read_regs(QMC_DATA_X_LSB_ADDR,dout_buffer,NUM_DOUT_BUFFER) != QMC_OK);
				process_raw_data(dout_buffer, result);
				break;
			}
		}
		b_delay(10);//to prevent I2C on KL25Z from locking up
	}
	return ret;
}

/*
 * Function to run a calibration routine based on:
 * 	https://github.com/kriswiner/MPU6050/wiki/Simple-and-Effective-Magnetometer-Calibration
 *
 * It collects max and min values in each axis for the number of samples specified. After that it
 * uses the following formula to find the offset(bias) in each axis:
 *
 * 		offset = (max - min)/2
 *
 * Parameters:
 *  num_samples the number of samples for which the calibration should run
 *
 * Returns:
 *  none
 */
void qmc_run_calibration(uint16_t num_samples){
	uint16_t i = 0;
	int16_t max_value[3] = {-32767,-32767,-32767};
	int16_t min_value[3] = {32767,32767,32767};
	int16_t raw_sample_value[3] = {0};
	float bias[3] = {0};
	while(i < num_samples){
		qmc_get_nex_raw_sample(raw_sample_value);
		for(int i = AXIS_X; i <= AXIS_Z; i++){
			if(raw_sample_value[i] < min_value[i]){
				min_value[i] = raw_sample_value[i];
			}
			if(raw_sample_value[i] > max_value[i]){
				max_value[i] = raw_sample_value[i];
			}
		}
		b_delay(1);
		i++;
	}
	for(int i = AXIS_X; i <= AXIS_Z; i++){
		bias[i] = (((float)(max_value[i] + min_value[i]))/2.0f);
	}
	calibration_data.offset_x = bias[AXIS_X];
	calibration_data.offset_y = bias[AXIS_Y];
	calibration_data.offset_z = bias[AXIS_Z];
}

/*
 * Function to dump raw sensor values on the terminal to run a python based calibration routine
 * based on:
 * 	https://github.com/kriswiner/MPU6050/wiki/Simple-and-Effective-Magnetometer-Calibration
 *
 * The python calibration routine calculates the scale factor for each axis in addition to the
 * bias. It can be found under /calibration-py-file
 *
 * It dumps the axis data in the following format
 *
 * 	 <x> <y> <z>\n
 *
 * Parameters:
 *  num_samples the number of samples for which the data should be dumped
 *
 * Returns:
 *  none
 */
void qmc_dump_calibration_data(uint16_t num_samples_to_dump){
	int16_t raw_result[3];
	uint16_t i = 0;

	while(i < num_samples_to_dump){

		qmc_get_nex_raw_sample(raw_result);

		for(int i = AXIS_X; i <= AXIS_Z;i++){
			if(raw_result[i] < 0){
				PRINTF("-%i ",raw_result[i]);
			}else{
				PRINTF("%i ",raw_result[i]);
			}
		}
		PRINTF("\r\n");
		i++;
	}
}



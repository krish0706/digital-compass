#include "i2c.h"
#include "QMC5883L.h"
#include "fsl_debug_console.h"//for debugging
#include "systick.h"


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
	*data = I2C1->D;//data will be in here now

	return QMC_OK;
}



void init_qmc(){
	uint8_t data;
	while(qmc_i2c_write_reg(QMC_CR1_ADDR,0b00010101) != QMC_OK);
	b_delay(1);
	while(qmc_i2c_read_reg(QMC_CR1_ADDR, &data) != QMC_OK);
	PRINTF("DATA: 0x%x\r\n",data);
}



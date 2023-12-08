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
		if(i == buf_len-1){
			I2C_TX_NACK();
		}else{
			I2C_TX_ACK();
		}
		buf[i] = I2C1->D;//to start receive action of i2c
		I2C_WAIT_IICIF();
		if(i == buf_len - 1){
			I2C_STOP();//if not stopped here, reading d will start next fetch
		}
		buf[i] = I2C1->D;//data will be in here now
		i++;
	}
	return QMC_OK;
}

void setOSR(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_OSR_MASK;
	*CR1 |= option<<CR1_OSR_SHIFT;
}

void setRNG(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_RNG_MASK;
	*CR1 |= option<<CR1_RNG_SHIFT;
}

void setODR(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_ODR_MASK;
	*CR1 |= option<<CR1_ODR_SHIFT;
}

void setMODE(qmc_cr1_osr_options_t option, uint8_t* CR1){
	*CR1 &= ~CR1_MODE_MASK;
	*CR1 |= option<<CR1_MODE_SHIFT;
}

void setSOFT_RST(uint8_t* CR2,uint8_t val){
	*CR2 &= ~CR2_SOFT_RST_MASK;
	*CR2 |= val<<CR2_SOFT_RST_SHIFT;
}

void setROL_PNT(uint8_t* CR2,uint8_t val){
	*CR2 &= ~CR2_ROL_PNT_MASK;
	*CR2 |= val<<CR2_ROL_PNT_SHIFT;
}

void setINT_ENB(uint8_t* CR2,uint8_t val){
	*CR2 &= ~CR2_INT_ENB_MASK;
	*CR2 |= val<<CR2_INT_ENB_SHIFT;
}

uint8_t getOSR(uint8_t reg_value){
	return ((reg_value & CR1_OSR_MASK)>>CR1_OSR_SHIFT);
}

uint8_t getRNG(uint8_t reg_value){
	return ((reg_value & CR1_RNG_MASK)>>CR1_RNG_SHIFT);
}

uint8_t getODR(uint8_t reg_value){
	return ((reg_value & CR1_ODR_MASK)>>CR1_ODR_SHIFT);
}

uint8_t getMODE(uint8_t reg_value){
	return ((reg_value & CR1_MODE_MASK)>>CR1_MODE_SHIFT);
}

uint8_t getDOR(uint8_t reg_value){
	return (reg_value & SR_DOR_MASK)>>SR_DOR_SHIFT;
}

uint8_t getOVL(uint8_t reg_value){
	return (reg_value & SR_OVL_MASK)>>SR_OVL_SHIFT;
}

uint8_t getDRDY(uint8_t reg_value){
	return (reg_value & SR_DRDY_MASK)>>SR_DRDY_SHIFT;
}

uint8_t getSOFT_RST(uint8_t reg_value){
	return (reg_value & CR2_SOFT_RST_MASK)>>CR2_SOFT_RST_SHIFT;
}

uint8_t getROL_PNT(uint8_t reg_value){
	return (reg_value & CR2_ROL_PNT_MASK)>>CR2_ROL_PNT_SHIFT;
}

uint8_t getINT_ENB(uint8_t reg_value){
	return (reg_value & CR2_INT_ENB_MASK)>>CR2_INT_ENB_SHIFT;
}

#define BYTE_SHIFT 8
static inline uint16_t concatenate_bytes(uint8_t MSB, uint8_t LSB){
	return (MSB<<BYTE_SHIFT) | LSB;
}

//result len must be 3 in calling func
void process_raw_data(uint8_t data[],int16_t result[]){
	uint8_t MSB, LSB;
	for(int i = 0; i < 3; i++){
		MSB = data[2*i+1];
		LSB = data[2*i];
		result[i] = concatenate_bytes(MSB, LSB);
	}
}

void init_qmc(){
//	uint8_t data;
//	while(qmc_i2c_write_reg(QMC_CR1_ADDR,0b00010101) != QMC_OK);
//	b_delay(1);
//	while(qmc_i2c_read_reg(QMC_CR1_ADDR, &data) != QMC_OK);
//	PRINTF("DATA: 0x%x\r\n",data);
//#define reg_buf_len 4
//	uint8_t reg_buf[reg_buf_len] = {0};
//	while(qmc_i2c_read_regs(QMC_TOUT_LSB_ADDR,reg_buf,reg_buf_len) != QMC_OK);
//	for(int i = 0; i < reg_buf_len; i++){
//		PRINTF("%d %d\r\n",reg_buf[reg_buf_len]);
//	}
	while(qmc_i2c_write_reg(QMC_SRS_PERIOD_ADDR,QMC_SRS_PERIOD_DEFAULT_VALUE)!= QMC_OK);
	b_delay(1);
	while(qmc_i2c_write_reg(QMC_CR1_ADDR,0x1D) != QMC_OK);
	b_delay(1);
	uint8_t sr = 0;
	qmc_error_t ret;
#define reg_buf_len 6
	uint8_t reg_buf[reg_buf_len] = {0};
	int16_t result_buf[3];
	while(1){
		ret = qmc_i2c_read_reg(QMC_SR_ADDR,&sr);
		if(ret == QMC_OK){
			if(getDRDY(sr)){
				b_delay(1);
				while(qmc_i2c_read_regs(QMC_DATA_X_LSB_ADDR,reg_buf,reg_buf_len) != QMC_OK);
				for(int i = 0; i < reg_buf_len; i++){
					PRINTF("%d %d\r\n",i,reg_buf[i]);
				}
				PRINTF("DATA READY\r\n");
				process_raw_data(reg_buf, result_buf);
				for(int i = 0; i < 3;i++){
					PRINTF("Axis:%d Data:%d\r\n",i,result_buf[i]);
				}
				PRINTF("DATA PROCESSED\r\n");
			}
		}
		b_delay(1);
	}
}



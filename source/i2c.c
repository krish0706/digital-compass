#include "MKL25Z4.h"
#include "i2c.h"
#include "stdint.h"
#include "systick.h"

void init_i2c(){
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[0] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_SRE_MASK);
	PORTE->PCR[0] = PORT_PCR_MUX(6);

	PORTE->PCR[1] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_SRE_MASK);
	PORTE->PCR[1] = PORT_PCR_MUX(6);

	I2C1->F &= ~(I2C_F_ICR_MASK | I2C_F_MULT_MASK);
	I2C1->F |= I2C_F_ICR(0x27);//psc of 480

	I2C1->C1 |= I2C_C1_IICEN_MASK;
}


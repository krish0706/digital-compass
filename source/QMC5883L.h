#ifndef __QMC5883L_H__
#define __QMC5883L_H__

#define QMC_DEVICE_ADDR 0x0D

#define QMC_CR1_ADDR 0x09

typedef enum{
	QMC_NACK_ERROR = 0,
	QMC_OK = 1
}qmc_error_t;

void init_qmc();

#endif

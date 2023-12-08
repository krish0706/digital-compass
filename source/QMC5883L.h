#ifndef __QMC5883L_H__
#define __QMC5883L_H__

#define QMC_DEVICE_ADDR (0x0DU)

#define QMC_DATA_X_LSB_ADDR (0x00U)
#define QMC_DATA_X_MSB_ADDR	(0x01U)
#define QMC_DATA_Y_LSB_ADDR	(0x02U)
#define QMC_DATA_Y_MSB_ADDR	(0x03U)
#define QMC_DATA_Z_LSB_ADDR	(0x04U)
#define QMC_DATA_Z_MSB_ADDR	(0x05U)
#define QMC_SR_ADDR         (0x06U)
#define QMC_TOUT_LSB_ADDR   (0x07U)
#define QMC_TOUT_MSB_ADDR   (0x08U)
#define QMC_CR1_ADDR   		(0x09U)
#define QMC_CR2_ADDR   		(0x0AU)
#define QMC_SRS_PERIOD_ADDR (0x0BU)

#define SR_DOR_MASK			(0x04U)
#define SR_DOR_SHIFT		(2)
#define SR_OVL_MASK			(0x02U)
#define SR_OVL_SHIFT		(1)
#define SR_DRDY_MASK		(0x01U)
#define SR_DRDY_SHIFT		(0)

#define CR1_OSR_MASK   		(0xC0U)
#define CR1_OSR_SHIFT  		(6U)
#define CR1_RNG_MASK   		(0x30U)
#define CR1_RNG_SHIFT  		(4U)
#define CR1_ODR_MASK   		(0x0CU)
#define CR1_ODR_SHIFT  		(2U)
#define CR1_MODE_MASK  		(0x03U)
#define CR1_MODE_SHIFT 		(0U)

#define CR2_SOFT_RST_MASK	(0x08U)
#define CR2_SOFT_RST_SHIFT	(7U)
#define CR2_ROL_PNT_MASK	(0x40U)
#define CR2_ROL_PNT_SHIFT	(6U)
#define CR2_INT_ENB_MASK	(0x01U)
#define CR2_INT_ENB_SHIFT	(0U)

#define QMC_SRS_PERIOD_DEFAULT_VALUE (0x01U)

typedef enum{
	QMC_NACK_ERROR = 0,
	QMC_OK = 1
}qmc_error_t;

typedef enum{
	MODE_OPTION_STANDBY,
	MODE_OPTION_CONTINUOUS
}qmc_cr1_mode_options_t;

typedef enum{
	ODR_OPTION_10HZ,
	ODR_OPTION_50HZ,
	ODR_OPTION_100HZ,
	ODR_OPTION_200HZ
}qmc_cr1_odr_options_t;

typedef enum{
	RNG_OPTION_2G,
	RNG_OPTION_8G
}qmc_cr1_rng_options_t;

typedef enum{
	OSR_OPTION_512,
	OSR_OPTION_256,
	OSR_OPTION_128,
	OSR_OPTION_64,
}qmc_cr1_osr_options_t;

typedef enum{
	SOFT_RST_DISABLE,
	SOFT_RST_ENABLE
}qmc_cr2_soft_rst_options_t;

typedef enum{
	ROL_PNT_DISABLE,
	ROL_PNT_ENABLE
}qmc_cr2_rol_pnt_options_t;

typedef enum{
	INT_ENB_ENABLE,
	INT_ENB_DISABLE
}qmc_cr2_int_enb_options_t;

typedef struct{
	qmc_cr1_mode_options_t mode;
	qmc_cr1_odr_options_t odr;
	qmc_cr1_rng_options_t rng;
	qmc_cr1_osr_options_t osr;
	qmc_cr2_soft_rst_options_t soft_rst;
	qmc_cr2_rol_pnt_options_t rol_pnt;
	qmc_cr2_int_enb_options_t int_enb;
}qmc_config_t;

void init_qmc(qmc_config_t *config);
void qmc_get_nex_raw_sample(int16_t result[]);

#endif


/**
  ******************************************************************************
  * @file    ina233_hal.h
  * @brief   This file contains the headers of the Texas Instruments INA233
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 Politecnico di Milano.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
 ******************************************************************************
  */

#ifndef __TI_INA233_HAL_H
#define __TI_INA233_HAL_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32g0xx_hal.h"
#include "ina233_hal_conf.h"


#define INA233_CURRENT_LSB  (CONF_MAX_EXPECTED_CURRENT / 2^15)
#define INA233_CAL          (0.00512 / (INA233_CURRENT_LSB * CONF_SHUNT_RESISTOR_VALUE)) // = 8258


// HAL API will take care of adding the R/W bit
#define INA233_SLAVE_1 (0b1000000 << 1)
#define INA233_SLAVE_2 (0b1000100 << 1)
#define INA233_SLAVE_3 (0b1001000 << 1)
#define INA233_SLAVE_4 (0b1001100 << 1)

#define INA233_MFR_ID        0x99
#define INA233_MFR_MODEL     0x9A
#define INA233_MFR_REVISION  0x9B

#define INA233_VIN_CODE      0x88
#define INA233_STATUS_WORD   0x79
#define COEFFICIENT          (0.01 / 8)
#define toVoltage(x) (x*(0.01/8))

/**
 * @brief Device manufacturing information
 */
typedef struct {
	uint8_t man_id[2];      /*!< Manufacturer ID (in ASCII) */
	uint8_t dev_model[6];   /*!< Device number (in ASCII) */
	uint8_t rev[2];         /*!< Device revision letter and number */
} INA233_ManufactureInfo;



#ifdef __cplusplus
 }
#endif

#endif // __TI_INA233_HAL_H

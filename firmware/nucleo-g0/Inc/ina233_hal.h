
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


#define INA233_CURRENT_LSB  (CONF_MAX_EXPECTED_CURRENT / (1 << 15))
#define INA233_CAL          (uint16_t)(0.00512 / (INA233_CURRENT_LSB * CONF_SHUNT_RESISTOR_VALUE)) // = 8258


// HAL API will take care of adding the R/W bit
#define INA233_SLAVE_1 (0b1000000 << 1)
#define INA233_SLAVE_2 (0b1000100 << 1)
#define INA233_SLAVE_3 (0b1001000 << 1)
#define INA233_SLAVE_4 (0b1001100 << 1)

 // PMBus Commands
#define INA233_CLEAR_FAULTS         0x03
#define INA233_RESTORE_DEFAULT_ALL  0x12
#define INA233_CAPABILITY           0x19
#define INA233_IOUT_OC_WARN_LIMIT   0x4A
#define INA233_VIN_OV_WARN_LIMIT    0x57
#define INA233_VIN_UV_WARN_LIMIT    0x58
#define INA233_PIN_OP_WARN_LIMIT    0x6B
#define INA233_STATUS_BYTE          0x78
#define INA233_STATUS_WORD          0x79
#define INA233_STATUS_IOUT          0x7B
#define INA233_STATUS_INPUT         0x7C
#define INA233_STATUS_CML           0x7E
#define INA233_STATUS_MFR_SPECIFIC  0x80
#define INA233_READ_EIN             0x86
#define INA233_READ_VIN             0x88
#define INA233_READ_IN              0x89
#define INA233_READ_VOUT            0x8B
#define INA233_READ_IOUT            0x8C
#define INA233_READ_POUT            0x96
#define INA233_READ_PIN             0x97
#define INA233_MFR_ID               0x99
#define INA233_MFR_MODEL            0x9A
#define INA233_MFR_REVISION         0x9B
#define INA233_ADC_CONFIG           0xD0
#define INA233_READ_VSHUNT          0xD1
#define INA233_ALERT_MASK           0xD2
#define INA233_MFR_CALIBRATION      0xD4
#define INA233_DEVICE_CONFIG        0xD5
#define INA233_CLEAR_EIN            0xD4
#define INA233_TI_MFR_ID            0xE0
#define INA233_TI_MFR_MODEL         0xE1
#define INA233_TI_MFR_REVISION      0xE2


#define COEFFICIENT          (0.01 / 8)
#define toVoltage(x) (x*(0.01/8))

/**
 * @brief Device manufacturing information
 */
typedef struct {
	uint8_t producer[3];    /*!< Manufacturer ID (in ASCII) */
	uint8_t model[7];       /*!< Device number (in ASCII) */
	uint8_t rev[3];         /*!< Device revision letter and number */
} INA233_DeviceInfo;



#ifdef __cplusplus
 }
#endif

#endif // __TI_INA233_HAL_H

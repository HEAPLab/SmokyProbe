/**
  ******************************************************************************
  * @file    ina233_hal_conf.h
  * @brief   This file contains configuration parameters for TI INA233
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

#ifndef __TI_INA233_HAL_CONF_H
#define __TI_INA233_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

#define CONF_NUM_DEVICES            8
#define CONF_I2C_BUS_TIMEOUT        10
#define CONF_SHUNT_RESISTOR_VALUE   0.002  // Ohm
#define CONF_MAX_EXPECTED_CURRENT   10.0   // A

#ifdef __cplusplus
 }
#endif

#endif // __TI_INA233_HAL_CONF_H

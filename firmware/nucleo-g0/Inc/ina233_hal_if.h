/**
  ******************************************************************************
  * @file    ina233_hal_if.h
  * @brief   This file contains function signatures for TI INA233 HAL
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

#ifndef __TI_INA233_HAL_IF_H
#define __TI_INA233_HAL_IF_H

#include "ina233_hal.h"


#ifdef __cplusplus
 extern "C" {
#endif

HAL_StatusTypeDef INA233_Init(I2C_HandleTypeDef *, uint16_t);

HAL_StatusTypeDef INA233_GetDeviceInfo(I2C_HandleTypeDef *, uint16_t, INA233_DeviceInfo *);

HAL_StatusTypeDef INA233_StatusCheck(I2C_HandleTypeDef *, uint16_t);


#ifdef __cplusplus
 }
#endif

#endif // __TI_INA233_HAL_IF_H




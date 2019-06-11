/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : ina233_hal.c
  * @brief          : Source file for TI INA233 access functions
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
/* USER CODE END Header */

#include "ina233_hal.h"


HAL_StatusTypeDef INA233_StatusCheck(I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t code = INA233_STATUS_WORD_CODE;
	uint8_t recv_data[2];
	uint32_t timeout = 10;

	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &code, sizeof(uint8_t), timeout);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, recv_data, 2*sizeof(uint8_t), timeout);
	if (status == HAL_ERROR) {
		  asm("nop");
	}
	return status;
}

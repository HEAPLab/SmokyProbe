
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

#include "ina233_hal_if.h"


HAL_StatusTypeDef INA233_Init(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr)
{
	HAL_StatusTypeDef status = HAL_OK;

	// Status check
	status = INA233_StatusCheck(i2c_handle, slave_addr);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	// Device info
	INA233_DeviceInfo info = {{ 0 }};
	status = INA233_GetDeviceInfo(i2c_handle, slave_addr, &info);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	// Calibration
	uint8_t calib_cmd_data[] = { INA233_MFR_CALIBRATION, INA233_CAL & 255, INA233_CAL >> 8 };

	status = HAL_I2C_Master_Transmit(
			i2c_handle, slave_addr, calib_cmd_data, sizeof(calib_cmd_data), CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
	}

	// Voltage/Current limits setting
	// TODO: Set in SmokyProbe v0.2

	// Set reading mode...

	return status;
}


HAL_StatusTypeDef INA233_GetDeviceInfo(
		I2C_HandleTypeDef * i2c_handle,
		uint16_t slave_addr,
		INA233_DeviceInfo * info)
{
	HAL_StatusTypeDef status = HAL_OK;

	uint8_t cmd = INA233_MFR_ID;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, info->producer, sizeof(info->producer), CONF_I2C_BUS_TIMEOUT);
	if (status != HAL_OK) return status;

	cmd = INA233_MFR_MODEL;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, info->model, sizeof(info->model), CONF_I2C_BUS_TIMEOUT);
	if (status != HAL_OK) return status;

	cmd = INA233_MFR_REVISION;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, info->rev, sizeof(info->rev), CONF_I2C_BUS_TIMEOUT);

	return status;
}


HAL_StatusTypeDef INA233_StatusCheck(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t cmd = INA233_STATUS_WORD;
	uint16_t recv_data;  // default value = 1000h = 4096d

	// Status
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, 2, CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	// Voltage/current/power warning thresholds
	uint8_t cmds[] = {
			INA233_IOUT_OC_WARN_LIMIT,
			INA233_VIN_OV_WARN_LIMIT,
			INA233_VIN_UV_WARN_LIMIT,
			INA233_PIN_OP_WARN_LIMIT
	};

	int i = 0;
	for (; i < 4; ++i) {
		status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmds[i], 1, CONF_I2C_BUS_TIMEOUT);
		status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, 2, CONF_I2C_BUS_TIMEOUT);
		if (status == HAL_ERROR) {
			asm("nop");
		}
	}

	return status;
}


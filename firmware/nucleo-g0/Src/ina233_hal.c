
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
#include <math.h>


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

	// Restore default
	uint8_t cmd_cfg[] = { INA233_RESTORE_DEFAULT_ALL };
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd_cfg, sizeof(cmd_cfg), CONF_I2C_BUS_TIMEOUT);
	uint8_t cmd_and_data[3];

	// Device configuration
	cmd_and_data[0] = INA233_DEVICE_CONFIG;
	uint8_t dev_cfg;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd_and_data, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, &dev_cfg, sizeof(dev_cfg), CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
	}

	// ADC Configuration
	cmd_and_data[0] = INA233_ADC_CONFIG;
	uint16_t adc_cfg;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd_and_data, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &adc_cfg, sizeof(adc_cfg), CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
	}

	// Calibration
	cmd_and_data[0] = INA233_MFR_CALIBRATION;
	cmd_and_data[1] = INA233_CAL & 255;
	cmd_and_data[2] = INA233_CAL >> 8;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd_and_data, 3, CONF_I2C_BUS_TIMEOUT);
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

	uint8_t cmd[] = { INA233_MFR_ID };
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, info->producer, sizeof(info->producer), CONF_I2C_BUS_TIMEOUT);
	if (status != HAL_OK) return status;

	cmd[0] = INA233_MFR_MODEL;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, info->model, sizeof(info->model), CONF_I2C_BUS_TIMEOUT);
	if (status != HAL_OK) return status;

	cmd[0] = INA233_MFR_REVISION;
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, info->rev, sizeof(info->rev), CONF_I2C_BUS_TIMEOUT);

	return status;
}


HAL_StatusTypeDef INA233_StatusCheck(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t cmd[] = { INA233_STATUS_WORD };
	uint16_t recv_data;  // default value = 1000h = 4096d

	// Status
	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd, 1, CONF_I2C_BUS_TIMEOUT);
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


float INA233_To_RealValue(int16_t reg_value, float m, int16_t R, uint16_t b)
{
	return (1.0 / m) * (reg_value * pow(10.0, -R) - b);
}


HAL_StatusTypeDef INA233_ReadInputCurrent(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, float * value)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t cmd = INA233_READ_IN;
	uint16_t recv_data;

	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, 2, CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	float m = (1.0 / INA233_CURRENT_LSB);
	*value = INA233_To_RealValue(recv_data, m, 0, 0);

	return status;
}

HAL_StatusTypeDef INA233_ReadInputVoltage(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, float * value)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t cmd = INA233_READ_VIN;
	uint16_t recv_data;

	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, 2, CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	*value = INA233_To_RealValue(recv_data, 8, 2, 0);

	return status;
}


HAL_StatusTypeDef INA233_ReadInputPower(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, float * value)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t cmd = INA233_READ_PIN;
	uint16_t recv_data;

	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, 2, CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	float m = (1.0 / (25 * INA233_CURRENT_LSB));
	*value = INA233_To_RealValue(recv_data, m, 0, 0);

	return status;
}


HAL_StatusTypeDef INA233_ReadShuntVoltage(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, float * value)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t cmd = INA233_READ_VSHUNT;
	uint16_t recv_data;

	status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, &cmd, 1, CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, 2, CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	*value = INA233_To_RealValue(recv_data, 4, 5, 0);

	return status;
}


HAL_StatusTypeDef INA233_ReadInputPowerSamples(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, INA233_PowerSampling * power_sampling)
{
	uint8_t cmd[] = { INA233_READ_EIN };
	uint8_t recv_data[7];

	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd, sizeof(cmd), CONF_I2C_BUS_TIMEOUT);
	status = HAL_I2C_Master_Receive(i2c_handle, slave_addr, (uint8_t *) &recv_data, sizeof(recv_data), CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	power_sampling->accumulator    = (recv_data[2] << 8) | recv_data[1];
	power_sampling->rollover_count =  recv_data[3];
	power_sampling->sample_count   = (recv_data[6] << 16) | (recv_data[5] << 8) | recv_data[4];
	power_sampling->tot_power = (power_sampling->rollover_count * (1 << 16)) + power_sampling->accumulator;

	return status;
}


HAL_StatusTypeDef INA233_StartEnergySampling(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, TIM_HandleTypeDef * htim, INA233_PowerSampling * power_sampling)
{
	// Clear power sampling
	INA233_ClearInputEnergy(i2c_handle, slave_addr);

	// Get power samples
	HAL_StatusTypeDef status = INA233_ReadInputPowerSamples(i2c_handle, slave_addr, power_sampling);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	// Start timing
	status = HAL_TIM_Base_Start(htim);

	return status;
}


HAL_StatusTypeDef INA233_StopEnergySampling(
		I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr, TIM_HandleTypeDef * htim,
		INA233_PowerSampling * prev_sampling, float * energy)
{
	INA233_PowerSampling curr_sampling;

	// Get power samples
	HAL_StatusTypeDef status = INA233_ReadInputPowerSamples(i2c_handle, slave_addr, &curr_sampling);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	// Stop timing
	status = HAL_TIM_Base_Stop(htim);
	uint32_t elapsed_time = __HAL_TIM_GET_COUNTER(htim);

	uint32_t power_acc    = curr_sampling.accumulator - prev_sampling->accumulator;
	uint32_t sample_count = curr_sampling.sample_count - prev_sampling->sample_count;
	float power_avg = (float) power_acc / sample_count;

	*energy = power_avg * (elapsed_time * pow(10, -6));
	return status;
}


HAL_StatusTypeDef INA233_ClearInputEnergy(I2C_HandleTypeDef * i2c_handle, uint16_t slave_addr)
{
	uint8_t cmd[] = { INA233_CLEAR_EIN };

	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(i2c_handle, slave_addr, cmd, 1, CONF_I2C_BUS_TIMEOUT);
	if (status == HAL_ERROR) {
		  asm("nop");
		  return status;
	}

	return HAL_OK;
}

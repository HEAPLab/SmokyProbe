/**
  ******************************************************************************
  * @file           : hostctrl.h
  * @brief          : Definitions of data structures for host-side control.
  *                   This file contains the common defines of the application.
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

#ifndef __HOSTCTRL_H
#define __HOSTCTRL_H

#ifdef __cplusplus
 extern "C" {
#endif


typedef enum {
	NOP = 0,
	RESET_DEVICE,
	GET_DEVICE_INFO,
	GET_CURRENT,
	GET_VOLTAGE,
	GET_VOLTAGE_SHUNT,
	GET_POWER,
	START_ENERGY_SAMPLING,
	STOP_ENERGY_SAMPLING
} HostSideRequest;


#ifdef __cplusplus
 }
#endif

#endif // __HOSTCTRL_H

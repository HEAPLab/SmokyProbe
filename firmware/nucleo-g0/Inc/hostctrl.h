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

#ifndef __SMOKYPROBE_HOSTCTRL_H
#define __SMOKYPROBE_HOSTCTRL_H

#ifdef __cplusplus
 extern "C" {
#endif

#define PKT_REPLY_DATA_MAX_LENGTH  100
#define PKT_REPLY_HEADER_LENGTH    3

#define PKT_CHANNEL_ID      0
#define PKT_REQUEST_CODE    1
#define PKT_REPLY_CODE      PKT_REQUEST_CODE
#define PKT_DATA_LENGTH     2
//#define PKT_REPLY_DATA_START 3

#define PKT_REQUEST_DATA_START  2


typedef struct {
	uint8_t dev_id;
	uint8_t req_code;
	uint8_t data;
}  RequestMessage;

typedef enum {
	NOP = 0,
	RESET_DEVICE,
	GET_DEVICE_INFO,
	GET_CURRENT,
	GET_VOLTAGE,
	GET_VOLTAGE_SHUNT,
	GET_POWER,
	GET_SAMPLES,
	START_ENERGY_SAMPLING,
	STOP_ENERGY_SAMPLING
} HostSideRequest;

typedef enum {
	VALID_REQUEST = 0,
	INVALID_CHANNEL_ID,
	INVALID_REQUEST_CODE
} ErrorCode;



#ifdef __cplusplus
 }
#endif

#endif // __SMOKYPROBE_HOSTCTRL_H

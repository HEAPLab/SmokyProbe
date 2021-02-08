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

#define MSG_REQUEST_LEN         3
#define MSG_REPLY_HEADER_LEN    4
#define MSG_REPLY_DATA_LEN_MAX  100

#define MSG_POS_CHANNEL_ID      0
#define MSG_POS_REQUEST_CODE    1
#define MSG_POS_DATA            2

#define MSG_POS_REPLY_CODE      MSG_POS_REQUEST_CODE
#define MSG_POS_REPLY_STATUS    2
#define MSG_POS_REPLY_DATA_LEN  3

/*
typedef enum {
	NOP = 0,
	CHECK_DEVICE,
	GET_DEVICE_INFO,
	GET_CURRENT,
	GET_VOLTAGE,
	GET_VOLTAGE_SHUNT,
	GET_POWER,
	GET_SAMPLES,
	START_ENERGY_SAMPLING,
	STOP_ENERGY_SAMPLING
} HostRequestCode;
*/

typedef char HostRequestCode;

#define NOP                    'x'
#define CHECK_DEVICE           'C'
#define GET_DEVICE_INFO        'D'
#define GET_CURRENT            'I'
#define GET_VOLTAGE            'V'
#define GET_VOLTAGE_SHUNT      'v'
#define GET_POWER              'P'
#define GET_SAMPLES            'S'
#define START_ENERGY_SAMPLING  'E'
#define STOP_ENERGY_SAMPLING   'e'

#define itoch(x) x + 48
#define chtoi(x) x - 48

typedef enum {
	REQUEST_OK = 100,
	CHANNEL_NOT_VALID,
	REQUEST_NOT_VALID,
	HW_ERROR
} ErrorCode;


#ifdef __cplusplus
 }
#endif

#endif // __SMOKYPROBE_HOSTCTRL_H

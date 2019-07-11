/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "ina233_hal_if.h"
#include "hostctrl.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef hlpuart1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
static void Test_Time(void);
static void Run_Interactive(void);
static void Run_Batch(void);
static void Test_UART_Echo(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

HAL_StatusTypeDef status = HAL_OK;

static uint16_t dev_addrs[] = {
		INA233_SLAVE_1, INA233_SLAVE_2, INA233_SLAVE_3, INA233_SLAVE_4
};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  // INA233 devices initialization
  unsigned int i = 0;
  for (; i < CONF_NUM_DEVICES; ++i) {
	  INA233_Init(&hi2c1, dev_addrs[i]);
  }

  status = HAL_TIM_Base_Start(&htim2);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  Test_Time();
	  //Test_UART_Echo();
	  //Run_Interactive();
	  //Run_Batch();
	  HAL_Delay(1000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */
	__LPUART1_CLK_ENABLE();

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

void Test_Time()
{
	uint32_t count = __HAL_TIM_GET_COUNTER(&htim2);
	uint8_t outstr[20];
	sprintf(outstr, "t=%u \n\r", count);
	HAL_UART_Transmit(&hlpuart1, outstr, strlen(outstr), HAL_MAX_DELAY);
}

void Test_UART_Echo()
{
  uint8_t inbyte;
  HAL_UART_Receive(&hlpuart1, &inbyte, 1, HAL_MAX_DELAY);
  HAL_UART_Transmit(&hlpuart1, &inbyte, 1, HAL_MAX_DELAY);
}


void Run_Interactive( )
{
  // Request/reply
  float current, power, voltage, vshunt, energy;
  INA233_DeviceInfo dev_info;
  INA233_PowerSampling power_sampling;

  uint8_t request[MSG_REQUEST_LEN];
  uint8_t ch_id = 0;
  uint8_t host_data = 255;
  HostRequestCode reqcode;

  uint8_t reply_header[MSG_REPLY_HEADER_LEN];
  char reply_data[MSG_REPLY_DATA_LEN_MAX];
  unsigned short int data_to_send;

  // Get the host-side request
  status = HAL_UART_Receive(&hlpuart1, request, MSG_REQUEST_LEN, HAL_MAX_DELAY);
  ch_id     = request[MSG_POS_CHANNEL_ID];
  reqcode   = (HostRequestCode) request[MSG_POS_REQUEST_CODE];
  host_data = request[MSG_POS_DATA];

  if (ch_id >= INA233_NR_SLAVES) {
	reply_header[MSG_POS_REPLY_STATUS] = CHANNEL_NOT_VALID;
	reply_header[MSG_POS_REPLY_DATA_LEN] = 0;
	reqcode = NOP;
  }

  // Request handling
  switch(reqcode) {

	case NOP:
		data_to_send = 0;
		break;

	case GET_DEVICE_INFO:
		status = INA233_GetDeviceInfo(&hi2c1, dev_addrs[ch_id], &dev_info);
		if (status == HAL_OK) {
		  sprintf(reply_data, "%s-%s-%s",
				  dev_info.producer, dev_info.model, dev_info.rev);
		  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
		  reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
		  data_to_send = 1;
		}
		break;

	case GET_CURRENT:
		status = INA233_ReadInputCurrent(&hi2c1, dev_addrs[ch_id], &current);
		if (status == HAL_OK) {
		  sprintf(reply_data, "%2.6f", current);
		  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
		  reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
		  data_to_send = 1;
		}
		break;

	case GET_VOLTAGE:
		status = INA233_ReadInputVoltage(&hi2c1, dev_addrs[ch_id], &voltage);
		if (status == HAL_OK) {
			sprintf(reply_data, "%2.3f", voltage);
			reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
			reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
			data_to_send = 1;
		}
		break;

	case GET_VOLTAGE_SHUNT:
		status = INA233_ReadShuntVoltage(&hi2c1, dev_addrs[ch_id], &vshunt);
		if (status == HAL_OK) {
			sprintf(reply_data, "%2.3f", vshunt);
			reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
			reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
			data_to_send = 1;
		}
		break;

	case GET_POWER:
	  status = INA233_ReadInputPower(&hi2c1, dev_addrs[ch_id], &power);
	  if (status == HAL_OK) {
		  sprintf(reply_data, "%4.6f", power);
		  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
		  reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
		  data_to_send = 1;
	  }
	  break;

	case GET_SAMPLES:
	  status = INA233_ReadInputCurrent(&hi2c1, dev_addrs[ch_id], &current);
	  status |= INA233_ReadInputVoltage(&hi2c1, dev_addrs[ch_id], &voltage);
	  status |= INA233_ReadShuntVoltage(&hi2c1, dev_addrs[ch_id], &vshunt);
	  status |= INA233_ReadInputPower(&hi2c1, dev_addrs[ch_id], &power);
	  if (status == HAL_OK) {
		  sprintf(reply_data, "%2.6f %2.3f %2.4f %4.6f",
				  current, voltage, power, vshunt);
		  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
		  reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
		  data_to_send = 1;
	  }
	  break;

	case START_ENERGY_SAMPLING:
	  status = INA233_StartEnergySampling(
			  &hi2c1, dev_addrs[ch_id], &htim2, &power_sampling);
	  if (status == HAL_OK) {
		  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
		  reply_header[MSG_POS_REPLY_DATA_LEN] = 0;
		  data_to_send = 0;
	  }
	  break;

	case STOP_ENERGY_SAMPLING:
	  status = INA233_StopEnergySampling(
			  &hi2c1, dev_addrs[ch_id], &htim2, &power_sampling, &energy);
	  if (status == HAL_OK) {
		  sprintf(reply_data, "%4.6f", energy);
		  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_OK;
		  reply_header[MSG_POS_REPLY_DATA_LEN] = strlen(reply_data);
		  data_to_send = 1;
	  }
	  break;

	default:
	  reply_header[MSG_POS_REPLY_STATUS] = REQUEST_NOT_VALID;
	  reply_header[MSG_POS_REPLY_DATA_LEN] = 0;
	  data_to_send = 0;
	  break;
  }

  // Send header
  if (status != HAL_OK) {
	reply_header[MSG_POS_REPLY_STATUS] = HW_ERROR;
	reply_header[MSG_POS_REPLY_DATA_LEN] = 0;
	data_to_send = 0;
  }
  reply_header[MSG_POS_CHANNEL_ID] = ch_id;
  reply_header[MSG_POS_REPLY_CODE] = (uint8_t) reqcode;
  status = HAL_UART_Transmit(&hlpuart1,
	  (uint8_t *) reply_header, MSG_REPLY_HEADER_LEN, HAL_MAX_DELAY);

  // Send reply
  if (data_to_send) {
	status = HAL_UART_Transmit(&hlpuart1,
		  (uint8_t *) reply_data, reply_header[MSG_POS_REPLY_DATA_LEN], HAL_MAX_DELAY);
	memset(reply_data, 0, MSG_REPLY_DATA_LEN_MAX);
	reply_header[MSG_POS_REPLY_DATA_LEN] = 0;
	data_to_send = 0;
  }
}


void Run_Batch() {
	float current, power, voltage, vshunt;
	uint8_t reply_data[MSG_REPLY_DATA_LEN_MAX];

	uint8_t ch_id = 0;
	for (; ch_id < 4; ++ch_id) {
	  status = INA233_ReadInputCurrent(&hi2c1, dev_addrs[ch_id], &current);
	  status |= INA233_ReadInputVoltage(&hi2c1, dev_addrs[ch_id], &voltage);
	  status |= INA233_ReadShuntVoltage(&hi2c1, dev_addrs[ch_id], &vshunt);
	  status |= INA233_ReadInputPower(&hi2c1, dev_addrs[ch_id], &power);
	  if (status == HAL_OK) {
		  sprintf(reply_data, "[%d] %2.6f %2.3f %2.4f %4.6f \n\r",
				  ch_id, current, voltage, power, vshunt);
		  size_t len = strlen((char *)reply_data);
		  status = HAL_UART_Transmit(&hlpuart1,
				  (uint8_t *) reply_data, len, HAL_MAX_DELAY);
		  memset(reply_data, 0, MSG_REPLY_DATA_LEN_MAX);
	  }
	}
}




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

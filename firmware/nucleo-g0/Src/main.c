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
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "ina233_hal_if.h"
#include "hostctrl.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
static void TIM2_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

HAL_StatusTypeDef status = HAL_OK;

static uint16_t dev_addrs[] = { INA233_SLAVE_1, INA233_SLAVE_2, INA233_SLAVE_3, INA233_SLAVE_4 };

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
  TIM2_Init();

  /* USER CODE BEGIN 2 */

  // INA233 devices initialization
  unsigned int i = 0;
  for (; i < CONF_NUM_DEVICES; ++i) {
	  INA233_Init(&hi2c1, dev_addrs[i]);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
/*
  // Welcome message
  unsigned char * welcome_msg = "SmokyProbe: Connected!"; //INA233_STR_NR_SLAVES " channels available"
  uint8_t init_msg[100];
  init_msg[PKT_CHANNEL_ID]  = 255;
  init_msg[PKT_REPLY_CODE]  = 255;
  init_msg[PKT_DATA_LENGTH] = strlen(welcome_msg);
  HAL_UART_Transmit(&hlpuart1, (uint8_t*) init_msg, PKT_REPLY_HEADER_LENGTH, 0xFFFF);
  HAL_UART_Transmit(&hlpuart1, welcome_msg, strlen(welcome_msg), 0xFFFF);

  */
  // Request/reply
  float current, power, voltage, vshunt, energy;
  INA233_DeviceInfo dev_info;
  INA233_PowerSampling power_sampling;

  uint8_t request[5];
  uint8_t ch_id = 0;
  uint8_t host_data = 255;
  HostSideRequest reqcode;

  uint8_t reply_header[3];
  char reply_data[PKT_REPLY_DATA_MAX_LENGTH];
  unsigned short int data_to_send;

  while (1)
  {
	  // Get the host-side request
	  status = HAL_UART_Receive(&hlpuart1, request, sizeof(RequestMessage), HAL_MAX_DELAY);
	  ch_id     = request[PKT_CHANNEL_ID];
	  reqcode   = (HostSideRequest) request[PKT_REQUEST_CODE];
	  host_data = request[PKT_REQUEST_DATA_START];

	  if (ch_id >= INA233_NR_SLAVES) {
		  reqcode = NOP;
		  reply_header[PKT_DATA_LENGTH] = 1;
		  snprintf(reply_data, 1, "%d", INVALID_CHANNEL_ID);
	  }

	  // Prepare reply...
	  reply_header[PKT_CHANNEL_ID] = ch_id;
	  reply_header[PKT_REPLY_CODE] = (uint8_t) reqcode;

	  // Request handling
	  switch(reqcode) {
	  case NOP:
		  data_to_send = 1;
		  break;
	  case GET_DEVICE_INFO:
		  status = INA233_GetDeviceInfo(&hi2c1, dev_addrs[ch_id], &dev_info);
		  snprintf(reply_data, PKT_REPLY_DATA_MAX_LENGTH, "%s %s %s \n\r",
				  &dev_info.producer[1],
				  &dev_info.model[1],
				  &dev_info.rev[1]);
		  data_to_send = 1;
		  break;

	  case GET_CURRENT:
		  status = INA233_ReadInputCurrent(&hi2c1, dev_addrs[ch_id], &current);
		  snprintf(reply_data, 12, "%f", current);
		  data_to_send = 1;
		  break;
	  case GET_VOLTAGE:
		  status = INA233_ReadInputVoltage(&hi2c1, dev_addrs[ch_id], &voltage);
		  snprintf(reply_data, 12, "%f", voltage);
		  data_to_send = 1;
		  break;
	  case GET_VOLTAGE_SHUNT:
		  status = INA233_ReadShuntVoltage(&hi2c1, dev_addrs[ch_id], &vshunt);
		  snprintf(reply_data, 12, "%f", vshunt);
		  data_to_send = 1;
		  break;
	  case GET_POWER:
		  status = INA233_ReadInputPower(&hi2c1, dev_addrs[ch_id], &power);
		  snprintf(reply_data, 12, "%f", power);
		  data_to_send = 1;
		  break;
	  case GET_SAMPLES:
		  status = INA233_ReadInputCurrent(&hi2c1, dev_addrs[ch_id], &current);
		  status = INA233_ReadInputVoltage(&hi2c1, dev_addrs[ch_id], &voltage);
		  status = INA233_ReadShuntVoltage(&hi2c1, dev_addrs[ch_id], &vshunt);
		  status = INA233_ReadInputPower(&hi2c1, dev_addrs[ch_id], &power);
		  snprintf(reply_data, 100, "%.3f %.3f %.4f %.4f", current, voltage, power, vshunt);
		  data_to_send = 1;
		  break;

	  case START_ENERGY_SAMPLING:
		  status = INA233_StartEnergySampling(
				  &hi2c1, dev_addrs[ch_id], &htim2, &power_sampling);
		  break;
	  case STOP_ENERGY_SAMPLING:
		  status = INA233_StopEnergySampling(
				  &hi2c1, dev_addrs[ch_id], &htim2, &power_sampling, &energy);
		  break;

	  default:
		  reply_header[PKT_DATA_LENGTH] = 1;
		  snprintf(reply_data, 1, "%d", INVALID_REQUEST_CODE);
		  data_to_send = 1;
		  break;
	  }

	  // Send reply
	  if (data_to_send) {
		  reply_header[PKT_DATA_LENGTH] = strlen(reply_data);
		  status = HAL_UART_Transmit(&hlpuart1, (uint8_t *) reply_header, 3, HAL_MAX_DELAY);
		  status = HAL_UART_Transmit(&hlpuart1, (uint8_t *) reply_data, reply_header[PKT_DATA_LENGTH], HAL_MAX_DELAY);
		  data_to_send = 0;
		  memset(reply_data, 0, PKT_REPLY_DATA_MAX_LENGTH);
	  }

	  HAL_Delay(500);

  /* USER CODE END WHILE */
  }

  return 0;
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
  PeriphClkInit.Lpuart1ClockSelection = RCC_SYSCLKSOURCE_HSI; //RCC_LPUART1CLKSOURCE_PCLK1;
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

static void TIM2_Init(void)
{
  __TIM2_CLK_ENABLE();
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 6400;  // 64 MHz clock => 1ms/cycle
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  //lptim1.RepetitionCounter = ;
  //lptim1.AutoReloadPreload = ;

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
}



/* USER CODE BEGIN 4 */

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

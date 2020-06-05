/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "portmacro.h"
#include "ili9320.h"
#include "uart.h"
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
UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_FS;

osThreadId_t defaultTaskHandle;
osThreadId_t myTask02_lowHandle;
osThreadId_t myTask03_highHandle;
osThreadId_t myTask04_realtiHandle;
osMessageQueueId_t myQueue01Handle;
osMessageQueueId_t myQueue02Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USB_PCD_Init(void);
void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);

/* USER CODE BEGIN PFP */
int _write(int fd, char *ptr, int len)
{
    int i = 0;
    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2) { return -1; }
    
    HAL_UART_Transmit(&huart1, *ptr, 1000, 1000);
    //while (*ptr && (i < len))
    {
      // USART_SendData(USART1, *ptr);    
      // while (USART_GetFlagStatus(USART1, USART_FLAG_TC)  == RESET) {}
      //     i++;
      //     ptr++;
      // 
    }
    return i;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */
  printf("Printf test.");

  ili9320_Initializtion();
  // LED1_GPIO_Port->BRR = LED1_Pin;     // Clear
  // for (uint16_t i = 0; i < 5000; i++) ;
  // LED1_GPIO_Port->BSRR = LED1_Pin;    // set
  // for (uint16_t i = 0; i < 5000; i++) ;
  // LED1_GPIO_Port->BRR = LED1_Pin;     // Clear
	ili9320_Clear(Blue);        // ~ 50 ms
  // LED1_GPIO_Port->BSRR = LED1_Pin;    // set
  // for (uint16_t i = 0; i < 5000; i++) ;
  // LED1_GPIO_Port->BRR = LED1_Pin;     // Clear
  // for (uint16_t i = 0; i < 5000; i++) ;
  // LED1_GPIO_Port->BSRR = LED1_Pin;    // set

	ili9320_ShowString_16x24(10, 10, 320, 240, "ili9320 LCD DISPLAY 16x24.");
	ili9320_ShowString_8x16(10, 80, 320, 240, "ili9320 LCD DISPLAY 8 x 16.");
	ili9320_ShowString_16x24(20, 120, 320, 240, "Loop-");
  /* USER CODE END 2 */

  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  const osMessageQueueAttr_t myQueue01_attributes = {
    .name = "myQueue01"
  };
  myQueue01Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue01_attributes);

  /* definition and creation of myQueue02 */
  const osMessageQueueAttr_t myQueue02_attributes = {
    .name = "myQueue02"
  };
  myQueue02Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue02_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* definition and creation of myTask02_low */
  const osThreadAttr_t myTask02_low_attributes = {
    .name = "myTask02_low",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  myTask02_lowHandle = osThreadNew(StartTask02, NULL, &myTask02_low_attributes);

  /* definition and creation of myTask03_high */
  const osThreadAttr_t myTask03_high_attributes = {
    .name = "myTask03_high",
    .priority = (osPriority_t) osPriorityHigh,
    .stack_size = 128
  };
  myTask03_highHandle = osThreadNew(StartTask03, NULL, &myTask03_high_attributes);

  /* definition and creation of myTask04_realti */
  const osThreadAttr_t myTask04_realti_attributes = {
    .name = "myTask04_realti",
    .priority = (osPriority_t) osPriorityRealtime,
    .stack_size = 256
  };
  myTask04_realtiHandle = osThreadNew(StartTask04, NULL, &myTask04_realti_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE); //
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED3_Pin|LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED3_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    //osDelay(1);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    HAL_UART_Transmit(&huart1, "Task01-default is running ...\r\n", 31, 1000);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    //osDelay(1);
    xTaskNotifyGive(myTask02_lowHandle);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02_low thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    //osDelay(1);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    HAL_UART_Transmit(&huart1, "Task02 is running ...\r\n", 23, 1000);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    //HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    //osDelay(110);
    //HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    //osDelay(1);
    HAL_UART_Transmit(&huart1, "Task02 give notify to Task03 ...\r\n", 55, 1000);
    xTaskNotifyGive(myTask03_highHandle);

  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03_high thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
    //osDelay(1);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    HAL_UART_Transmit(&huart1, "Task03 is running ...\r\n", 23, 1000);
    HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    //osDelay(1);
    HAL_UART_Transmit(&huart1, "Task03 give notify to Task04 .\r\n", 55, 1000);
    xTaskNotifyGive(myTask04_realtiHandle);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04_realti thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */
  //static uint8_t *pdata = "HAL_UART_Transmit";
  uint16_t i = 50000;
  //uint16_t xpos = 0;
  char loop[] = "000.00\0";
  //uint8_t tickDelay = 1;
  //char delay[] = "00\r\n";

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);

    if(USART_RX_STA & 0x8000)
		{
			uint16_t len = USART_RX_STA & 0x3fff; // Rx len
			HAL_UART_Transmit(&huart1, "\r\n您发送的消息为: \r\n", 200, 1000);
			HAL_UART_Transmit(&huart1, (uint8_t*)USART_RX_BUF, len, 1000);	//发送接收到的数据
			HAL_UART_Transmit(&huart1, "\r\n", 20, 1000);
			USART_RX_STA=0;
		}

    //portTICK_TYPE_ENTER_CRITICAL();
    {
      //if (--tickDelay == 0) tickDelay = 9;
      //delay[1] = '0' + tickDelay;
      //HAL_UART_Transmit(&huart1, delay, 17, 1000);

      if (i % 50 == 0) 
      {
        HAL_UART_Transmit(&huart1, "Task04 is running ...\r\n", 23, 1000);
      }

      i++;
      loop[0] = '0' + (i / 10000);
      loop[1] = '0' + ((i % 10000) / 1000);
      loop[2] = '0' + ((i % 1000) / 100);
      loop[4] = '0' + ((i % 100) / 10);
      loop[5] = '0' + (i % 10);
      //USART1_printf(loop);
      //USART1_printf("\r\n");

      ili9320_ShowString_8x16(100, 124, 320, 240, loop);
      // ili9320_PutChar(140, 124, loop[5], Red, Blue);
      // if (i % 10 == 0)
      // {
      //   ili9320_PutChar(132, 124, loop[4], Red, Blue);
      //   if (i % 100 == 0)
      //   {
      //     ili9320_PutChar(124, 124, loop[3], Red, Blue);
      //     ili9320_PutChar(116, 124, loop[2], Red, Blue);
      //   }
      //   if (i % 1000 == 0)
      //   {
      //     ili9320_PutChar(108, 124, loop[1], Red, Blue);
      //   }
      //   if (i % 10000 == 0)
      //   {
      //     ili9320_PutChar(100, 124, loop[0], Red, Blue);
      //   }
      // }
    }

    if (i % 100 == 0)
    {
        xTaskNotifyGive(defaultTaskHandle);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    //portTICK_TYPE_EXIT_CRITICAL();

    //ili9320_PutChar(100+ (xpos * 8), 124, loop[xpos], Red, Blue);
    //if (++xpos > 5) xpos = 0;
		//ili9320_ShowString_8x16(100, 124, 320, 240, loop);
  }
  /* USER CODE END StartTask04 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

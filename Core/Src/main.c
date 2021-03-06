/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "iwdg.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_custom_hid_if.h"
#include "ppm.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_USB_DEVICE_Init();
  MX_IWDG_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  // Setup PPM module
  const uint32_t prescaler = LL_TIM_GetPrescaler(TIM1);
  const uint32_t clock = 48000000 / (prescaler + 1);
  PPM_Init(clock);

  // Start the timer
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableIT_CC1(TIM1);
  LL_TIM_EnableIT_UPDATE(TIM1);
  LL_TIM_EnableCounter(TIM1);

  // Current report contains X, Y, Throttle, Rudder as 8-bit outputs,
  // 4 buttons as 1-bit outputs and 4 constant 1-bit outputs for alignment
  uint8_t report_buffer[5] = {0};
  bool report_changed = false;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    LL_IWDG_ReloadCounter(IWDG);

#if 0 // TEST BUTTON
    const bool button_pressed = LL_GPIO_IsInputPinSet(BUT_GPIO_Port, BUT_Pin);
    if (button_pressed)
    {
      LL_GPIO_SetOutputPin(D1_GPIO_Port, D1_Pin);
    }
    else
    {
      LL_GPIO_ResetOutputPin(D1_GPIO_Port, D1_Pin);
    }
#endif

    if (PPM_NEW_DATA)
    {
      PPM_NEW_DATA = false;

      for (uint8_t i = 0; i < 8; ++i)
      {
        const uint8_t channel_value = PPM_GetChannel(i);

#if 0 // TEST CHANNEL
        if (i == 2 && channel_value > 127)
        {
          LL_GPIO_SetOutputPin(D1_GPIO_Port, D1_Pin);
        }
        else if (i == 2 && channel_value <= 127)
        {
          LL_GPIO_ResetOutputPin(D1_GPIO_Port, D1_Pin);
        }
#endif

        // For the last 4 channels (8-bit -> 1-bit)
        const uint8_t but_report_index = 4;
        if (i >= but_report_index)
        {
          const uint8_t but_mask_index = i - but_report_index;
          const uint8_t bit_mask = (1 << but_mask_index);

          // Get i-th bit
          const uint8_t current_value = report_buffer[but_report_index] & bit_mask;

          // Compute the new i-th bit
          const uint8_t but_pressed = (channel_value < 127);
          const uint8_t but_value = but_pressed << but_mask_index;

          // Compare current vs. new
          if (current_value != but_value)
          {
            // set/reset only the correct bit
            report_buffer[but_report_index] &= ~bit_mask;
            report_buffer[but_report_index] |= but_value;
            report_changed = true;
          }
        }
        // For the first 4 channels (8-bit each)
        else if (report_buffer[i] != channel_value)
        {
          report_buffer[i] = channel_value;
          report_changed = true;
        }
      }

      if (report_changed)
      {
        const uint8_t status = USBD_CUSTOM_HID_SendReport_FS(report_buffer, sizeof(report_buffer));
        if (status == USBD_OK)
        {
          report_changed = false;
        }
      }
    }

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_6);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(48000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL);
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

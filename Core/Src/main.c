/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "timer.h"
#include "uart.h"
#include "functions.h"
#include "tests.h"
#include <string.h>
#include <stdio.h>   // For printf and snprintf

//#define UART_BUFFER_SIZE 128  // Adjust the size as needed


#define NUM_LIGHTS 4
#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 0
#define DELAY_1SEC HAL_Delay(1000) // 1-second delay macro

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void LightControlLoop(void);



int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART3_UART_Init();
  MX_CAN_Init();
  Start_PWM_All_Channels();




  uint8_t rxBuffer[25]; // Larger buffer
  uint8_t txBuffer[100]; // For echoed response
  // Start UART reception in interrupt mode
  HAL_UART_Receive_IT(&huart3, tempBuffer, 1);
  /* Infinite loop */
  while (1)
  {

	  processData(&huart3);

      system_initialisation();
      HAL_Delay(5000);

	  controlMotor5("open");
	  HAL_Delay(5000);
      controlMotor5("close");
	  HAL_Delay(1000);

	  rotatePhone(90);
	  HAL_Delay(500);
	  rotatePhone(180);
      HAL_Delay(500);
      rotatePhone(270);
      HAL_Delay(500);
	  rotatePhone(360);
      HAL_Delay(500);

      multitouch_screen_test(70, 40, 70, 140);
      HAL_Delay(500);

      screen_test(23, 5, 93, 195, 23);
      HAL_Delay(500);

      speaker_test(70, 1, 70, 150);
      HAL_Delay(500);

      Return_xy();
      HAL_Delay(500);

      SetLightBrightness(3 , 0);
      HAL_Delay(500);
      SetLightBrightness(4 , 0);
      HAL_Delay(500);
      camera_test(160);
      HAL_Delay(500);
      SetLightBrightness(3 , 100);
      HAL_Delay(500);
      SetLightBrightness(4 , 100);
      HAL_Delay(1000);
      


  }
}




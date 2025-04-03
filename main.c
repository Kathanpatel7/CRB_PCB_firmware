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



/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);


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
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM17_Init();
  MX_USART3_UART_Init();
  MX_CAN_Init();
  Start_PWM_All_Channels();
/*
  // Send homing command to all motors
  sendHomingCommand(0x001);  // Homing Motor 1

  while (!motor1Homed) {
    // Continuously check homing status for each motor
	processCANMessage();
    HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
  }

  sendHomingCommand(0x002);  // Homing Motor 2
  sendHomingCommand(0x003);  // Phone Rotation Motor
  sendHomingCommand(0x004);  // Image Mount Motor
  sendHomingCommand(0x005);  // Drawer Motor
  setServoPosition(&htim2, TIM_CHANNEL_1, 0);
  setServoPosition(&htim2, TIM_CHANNEL_2, 70);

  while (!drawerMotorHomed || !phoneRotationMotorHomed || !imageMountMotorHomed || !motor2Homed) {
    // Continuously check homing status for each motor
	processCANMessage();
    HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
  }
  */

  /* Example message to send over UART3 */
  uint8_t message[] = "Hello from UART3!\r\n";



  /* Infinite loop */
  while (1)
  {
    /* Transmit message over UART3 */
	Send_UART(message);

/*

	             HAL_Delay(3000);  // Delay for 1 second after processing
	             controlMotor5("open");


	             HAL_Delay(40000);  // Delay for 1 second after processing
	             controlMotor5("close"); // To close and move Motor 5 to 0mm
	             HAL_Delay(7000);

	             sendAbsoluteAxisCommand(0x003, 1500,100, 0);  // Move Motor 1 to x

	             HAL_Delay(3000);


	             sendAbsoluteAxisCommand(0x003, 1500, 100, 4100);  // Move Motor 1 to x

	             HAL_Delay(3000);

	             sendAbsoluteAxisCommand(0x003, 1500, 100, 8200);  // Move Motor 1 to x

	             HAL_Delay(3000);

	             sendAbsoluteAxisCommand(0x003, 1500, 100, 12300);  // Move Motor 1 to x

	             HAL_Delay(3000);

	             sendAbsoluteAxisCommand(0x003, 1500, 100, 16400);  // Move Motor 1 to x

	             HAL_Delay(3000);


	             screen_test();
	             speaker_test();
	             camera_test();

	             //myservo2.write(10);


	             HAL_Delay(3000);

	             processCANMessage();



    //screen_test();     */
    HAL_Delay(1000);
  }
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
  __HAL_RCC_CAN1_CLK_ENABLE();  // Enable CAN clock

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}



/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure GPIO pin for PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up or pull-down
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed for blinking
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);     // Initialize the pin
}

// GPIO Initialization (MX_GPIO_Init)


/* DMA Initialization */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */


  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);  // Example, replace with actual channel if different
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);  // Example, replace with actual channel if different
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */

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
static void LightControlLoop(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Function to process data


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
  MX_TIM16_Init();
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
  setServoPosition(TIM_CHANNEL_1, 0);
  setServoPosition(TIM_CHANNEL_2, 70);

  while (!drawerMotorHomed || !phoneRotationMotorHomed || !imageMountMotorHomed || !motor2Homed) {
    // Continuously check homing status for each motor
	processCANMessage();
    HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
  }
  */

  /* Example message to send over UART3 */


  uint8_t rxBuffer[25]; // Larger buffer
  uint8_t txBuffer[100]; // For echoed response
  // Start UART reception in interrupt mode
  HAL_UART_Receive_IT(&huart3, tempBuffer, 1);
  /* Infinite loop */
  while (1)
  {

	  processData(&huart3);








      // all loop
//      system_initialisation();
//      HAL_Delay(5000);
//
//	  controlMotor5("open");
//	  HAL_Delay(5000);
//      controlMotor5("close");
//	  HAL_Delay(1000);
//
//	  rotatePhone(90);
//	  HAL_Delay(500);
//	  rotatePhone(180);
//      HAL_Delay(500);
//      rotatePhone(270);
//      HAL_Delay(500);
//	  rotatePhone(360);
//      HAL_Delay(500);
//
//      multitouch_screen_test(70, 40, 70, 140);
//      HAL_Delay(500);
//
//      screen_test(23, 5, 93, 195, 23);
//      HAL_Delay(500);
//
//      speaker_test(70, 1, 70, 150);
//      HAL_Delay(500);
//
//      Return_xy();
//      HAL_Delay(500);
//
//      SetLightBrightness(3 , 0);
//      HAL_Delay(500);
//      SetLightBrightness(4 , 0);
//      HAL_Delay(500);
//      camera_test(160);
//      HAL_Delay(500);
//      SetLightBrightness(3 , 100);
//      HAL_Delay(500);
//      SetLightBrightness(4 , 100);
//      HAL_Delay(1000);








	    // Attempt to receive up to 64 bytes, with a timeout of 500 ms
/*

	  if (HAL_UART_Receive(&huart3, rxBuffer, sizeof(rxBuffer), 500) == HAL_OK)
	  {
	      // Find the first and last double-quote character
	      char *start = strchr((char *)rxBuffer, '!'); // Find the first "
	      char *end = strrchr((char *)rxBuffer, '\n');  // Find the last "

	      // Ensure both quotes are found and are in proper order
	      if (start != NULL && end != NULL && start < end)
	      {
	          // Extract the substring between quotes
	          size_t length = end - start - 1; // Calculate the length of the string between quotes
	          strncpy((char *)txBuffer, start + 1, length);
	          txBuffer[length] = '\n'; // Append newline character
	          txBuffer[length + 1] = '\0'; // Null-terminate the string

	          // Echo the extracted string
	          HAL_UART_Transmit(&huart3, txBuffer, strlen((char *)txBuffer), HAL_MAX_DELAY);
	      }

	      // Clear the buffers
	      memset(rxBuffer, 0, sizeof(rxBuffer));
	      memset(txBuffer, 0, sizeof(txBuffer));
	  }
*/

	  //LightControlLoop();
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
    //HAL_Delay(1000);
  }
}
void LightControlLoop(void) {


    while (1) {

    		SetLightBrightness(1, 100);
		SetLightBrightness(2, 0);
		SetLightBrightness(3, 0);
		SetLightBrightness(4, 0);
		HAL_Delay(1000);
		SetLightBrightness(1, 0);
		SetLightBrightness(3, 0);
		SetLightBrightness(4, 0);
		SetLightBrightness(2, 100);
		HAL_Delay(1000);
		SetLightBrightness(2, 0);
		SetLightBrightness(1, 0);
		SetLightBrightness(4, 0);
		SetLightBrightness(3, 100);
		HAL_Delay(1000);
		SetLightBrightness(4, 100);
		SetLightBrightness(2, 0);
		SetLightBrightness(3, 0);
		SetLightBrightness(1, 0);
		HAL_Delay(1000);
        // Step 1: Turn on each light one by one for 1 second
        for (uint8_t light = 1; light <= NUM_LIGHTS; light++) {
            SetLightBrightness(light, MIN_BRIGHTNESS); // Turn on light at brightness 0
            DELAY_1SEC; // Wait for 1 second
        }

        // Step 2: Gradually increase brightness from 0 to 100 for all lights together
        for (uint8_t brightness = MIN_BRIGHTNESS; brightness <= MAX_BRIGHTNESS; brightness++) {
            for (uint8_t light = 1; light <= NUM_LIGHTS; light++) {
                SetLightBrightness(light, brightness); // Set brightness for each light
            }
            HAL_Delay(50); // Adjust the delay for gradual increase speed
        }

        // Step 3: Gradually decrease brightness from 100 to 0 for all lights together
        for (uint8_t brightness = MAX_BRIGHTNESS; brightness >= MIN_BRIGHTNESS; brightness--) {
            for (uint8_t light = 1; light <= NUM_LIGHTS; light++) {
                SetLightBrightness(light, brightness); // Set brightness for each light
            }
            HAL_Delay(50); // Adjust the delay for gradual decrease speed
        }
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

/*
 * uart.c
 *
 *  Created on: Nov 11, 2024
 *      Author: kathanpatel
 */

#include "main.h"
#include "uart.h"
#include "tests.h"
#include "commands.h"
#include <string.h>  // For strlen and memset

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t rxdata[25];
#define RX_BUFFER_SIZE 256

// UART buffers and pointers
uint8_t rxBuffer[RX_BUFFER_SIZE];  // Buffer for UART reception
uint8_t tempBuffer[1];             // Temporary buffer for single-byte reception
volatile uint16_t writeIndex = 0;  // Write pointer for the ring buffer
volatile uint16_t readIndex = 0;   // Read pointer for the ring buffer

// UART callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {  // Update as per your USART instance
        // Store the received byte in the ring buffer
        rxBuffer[writeIndex++] = tempBuffer[0];
        writeIndex %= RX_BUFFER_SIZE; // Wrap around if buffer is full

        // Restart UART reception
        HAL_UART_Receive_IT(huart, tempBuffer, 1);
    }
}

// Function to process received data
void processData(UART_HandleTypeDef *huart) {
    static uint8_t parsingBuffer[RX_BUFFER_SIZE]; // Buffer to store incoming data
    static uint16_t parsingIndex = 0;            // Index for parsing buffer

    // Read the data byte by byte from the ring buffer
    while (readIndex != writeIndex) {
        uint8_t data = rxBuffer[readIndex++];  // Read a byte from the ring buffer
        readIndex %= RX_BUFFER_SIZE;          // Wrap around if needed

        // Store received data in the parsing buffer
        parsingBuffer[parsingIndex++] = data;

        // Check for newline character indicating end of message
        if (data == '\n') {
            parsingBuffer[parsingIndex] = '\0'; // Null-terminate the string

            // Extract data between '!' and '\n'
            char *start = strchr((char *)parsingBuffer, '!'); // Find the start delimiter
            char *end = strrchr((char *)parsingBuffer, '\n'); // Find the end delimiter
            if (start != NULL && end != NULL && start < end) {
                size_t length = end - start - 1;  // Calculate the length of the message
                uint8_t txBuffer[length + 1];    // Buffer for parsed data
                strncpy((char *)txBuffer, start + 1, length); // Copy the message
                txBuffer[length] = '\0';        // Null-terminate the string

                // Send an acknowledgment message before parsing
                sendUARTMessage("On it\n");
                sendUARTMessage(txBuffer);
                // Pass the parsed data to the parseCommand function
                parseCommand((char *)txBuffer);


            }

            // Reset the parsing buffer for the next message
            parsingIndex = 0;
        }
    }
}




void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/*
 * uart.h
 *
 *  Created on: Nov 11, 2024
 *      Author: kathanpatel
 */

#ifndef UART_H
#define UART_H

#include "main.h"  // Include the main HAL library

// Macro Definitions
#define UART_RX_BUFFER_SIZE 25

// Global Variables
extern UART_HandleTypeDef huart3;  // UART handle for USART3
extern uint8_t rxdata[UART_RX_BUFFER_SIZE];  // Buffer for received data
extern uint8_t tempBuffer[1]; // Temporary buffer for single-byte reception

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void processData(UART_HandleTypeDef *huart);
void sendUARTMessage(const char *message);

// Function Prototypes
void MX_USART3_UART_Init(void);  // Function to initialize USART3

#endif /* UART_H */

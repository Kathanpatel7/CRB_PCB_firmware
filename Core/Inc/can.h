/*
 * can.h
 *
 *  Created on: Nov 11, 2024
 *      Author: kathanpatel
 */

#ifndef CAN_H
#define CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

// External CAN handle and filter configurations
extern CAN_HandleTypeDef hcan;
extern CAN_FilterTypeDef canfilterconfig;
extern CAN_RxHeaderTypeDef g_rx_header;  // Global variable for CAN message header
extern uint8_t g_rx_data[8];

// Function prototypes
void MX_CAN_Init(void);
void CAN_RX0_IRQHandler(void);

// Corrected declaration for the CAN interrupt callback function
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void processCANMessage();

#ifdef __cplusplus
}
#endif

#endif /* CAN_H */

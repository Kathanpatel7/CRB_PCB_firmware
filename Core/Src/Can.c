/*
 * Can.c
 *
 *  Created on: Nov 11, 2024
 *      Author: kathanpatel
 */

#include "can.h"
#include "main.h"
#include "functions.h"


CAN_HandleTypeDef hcan;
CAN_FilterTypeDef canfilterconfig;
CAN_RxHeaderTypeDef g_rx_header;  // Global variable for CAN message header
uint8_t g_rx_data[8];             // Global array for CAN data
            // CAN message data buffer

void MX_CAN_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

void MX_CAN_Init(void)
{
    hcan.Instance = CAN;
    hcan.Init.Prescaler = 6;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = ENABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    // Ensure the CAN peripheral is properly reset
    if (HAL_CAN_DeInit(&hcan) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_CAN_Init(&hcan) != HAL_OK) {
        Error_Handler();
    }

    // Configure CAN Filter to accept all messages
    CAN_FilterTypeDef canfilterconfig = {0};
    canfilterconfig.FilterActivation = ENABLE;
    canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilterconfig.FilterScale = CAN_FILTERSCALE_16BIT;
    canfilterconfig.FilterBank = 0;
    canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    canfilterconfig.FilterIdHigh = 0x0000;
    canfilterconfig.FilterIdLow = 0x0000;
    canfilterconfig.FilterMaskIdHigh = 0x0000;
    canfilterconfig.FilterMaskIdLow = 0x0000;

    if (HAL_CAN_ConfigFilter(&hcan, &canfilterconfig) != HAL_OK) {
        Error_Handler();
    }

    // Activate CAN RX interrupt notification for FIFO0 message pending
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }

    // Start CAN
    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        Error_Handler();
    }
}

// In the CAN interrupt callback or handler function:
// Global variables to hold received CAN message information


// Callback function called when a CAN message is received in FIFO 0
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    // Receive the CAN message and store it in global variables
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &g_rx_header, g_rx_data) != HAL_OK) {
        Error_Handler();  // Error handling
    }

    // Indicate a message has been received
    /*
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_Delay(500);
    */
    // Call the function to process the received CAN message
    processCANMessage();
}

void sendCANMessage(uint32_t id, bool extended, uint8_t data[], uint8_t len) {
    CAN_TxHeaderTypeDef tx_header;
    uint32_t tx_mailbox;

    // Set the properties for the CAN header
    tx_header.DLC = len;  // Length of the data to send
    tx_header.StdId = id;  // Standard ID (or use ExtId if extended)
    tx_header.ExtId = id;  // Extended ID if needed
    tx_header.RTR = CAN_RTR_DATA;  // Data frame type
    tx_header.IDE = extended ? CAN_ID_EXT : CAN_ID_STD;  // Standard or Extended frame
    tx_header.TransmitGlobalTime = DISABLE;  // Disable global time

    // Wait for a free mailbox to send the message
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        HAL_Delay(1);  // Wait until a mailbox is free
    }

    // Send the CAN message
    if (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &tx_mailbox) != HAL_OK) {
        Error_Handler();  // Call error handler in case of failure
    }

    // Wait for the message to be transmitted (make sure it's done)
    while (HAL_CAN_IsTxMessagePending(&hcan, tx_mailbox)) {
        HAL_Delay(1);  // Wait for transmission to complete
    }

    HAL_Delay(100);  // Small delay before the next operation
}

// Function to process the CAN message based on message ID and data
void processCANMessage() {
    switch (g_rx_header.StdId) {
        case 0x001:  // Motor 1
            if (g_rx_data[0] == 0x91 && g_rx_data[1] == 2) {
                motor1Homed = true;
                //printf("Motor 1 homing successful.\n");
            }
            if (g_rx_data[0] == 0xF5 && g_rx_data[1] == 2) {  // Motion complete
                motor1MotionComplete = true;
                //printf("Motor 1 motion complete.\n");
            }
            break;

        case 0x002:  // Motor 2
            if (g_rx_data[0] == 0x91 && g_rx_data[1] == 2) {
                motor2Homed = true;
                //printf("Motor 2 homing successful.\n");
            }
            if (g_rx_data[0] == 0xF5 && g_rx_data[1] == 2) {  // Motion complete
                motor2MotionComplete = true;
                //printf("Motor 2 motion complete.\n");
            }
            break;

        case 0x003:  // Phone Rotation Motor
            if (g_rx_data[0] == 0x91 && g_rx_data[1] == 2) {
                phoneRotationMotorHomed = true;
                //printf("Phone Rotation Motor homing successful.\n");
            }
            if (g_rx_data[0] == 0xF5 && g_rx_data[1] == 2) {  // Motion complete
                phoneRotationMotorComplete = true;
                //printf("Phone Rotation Motor motion complete.\n");
            }
            break;

        case 0x004:  // Image Mount Motor
            if (g_rx_data[0] == 0x91 && g_rx_data[1] == 2) {
                imageMountMotorHomed = true;
                //printf("Image Mount Motor homing successful.\n");
            }
            if (g_rx_data[0] == 0xF5 && g_rx_data[1] == 2) {  // Motion complete
                imageMountMotorComplete = true;
                //printf("Image Mount Motor motion complete.\n");
            }
            break;

        case 0x005:  // Drawer Motor
            if (g_rx_data[0] == 0x91 && g_rx_data[1] == 2) {
                drawerMotorHomed = true;
                //printf("Drawer Motor homing successful.\n");
            }
            if (g_rx_data[0] == 0xF5 && g_rx_data[1] == 2) {  // Motion complete
                drawerMotorComplete = true;
                //printf("Drawer Motor motion complete.\n");
            }
            break;

        default:
            //printf("Unknown CAN ID received.\n");
            break;
    }
}
void CAN_RX0_IRQHandler(void)
{
    /* Call the CAN Rx FIFO0 message pending callback */
    HAL_CAN_IRQHandler(&hcan);
}

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PULSES_PER_MM_MOTOR2 3200.0  // Calibration: 16000 pulses for 10 mm (Motor 2)
#define PULSES_PER_MM_MOTOR1 266.67   // Calibration: -4000 pulses for 60 mm (Motor 1)

#include "stm32f0xx_hal.h"   // Include the STM32 HAL header
#include "can.h"
#include "main.h"
#include "uart.h"
#include <stdbool.h>  // Include this to define 'bool' in your code
// Define global variables
extern CAN_HandleTypeDef hcan;  // CAN handle for communication
extern UART_HandleTypeDef huart3; // UART handle for USART2

extern bool waitingForMotor2;
extern bool commandToMotor2Sent;
extern bool waitingForMotor1;
extern bool commandToMotor1Sent;
extern bool motor1Homed;
extern bool motor2Homed;
extern bool phoneRotationMotorHomed;
extern bool imageMountMotorHomed;
extern bool drawerMotorHomed;

// Global variables for motor motion completion status
extern bool motor1MotionComplete;
extern bool motor2MotionComplete;
extern bool phoneRotationMotorComplete;
extern bool imageMountMotorComplete;
extern bool drawerMotorComplete;

// Function declarations
void sendHomingParameters(uint32_t id, uint8_t homeTrig, uint8_t homeDir, uint16_t homeSpeed, uint8_t endLimit);
void sendHomingCommand(uint32_t id);
void system_initialisation();
void sendUARTMessage(const char *message);
void sendCANCommandWithCRC(uint32_t id, bool extended, uint8_t data[], uint8_t len);
void sendCANMessage(uint32_t id, bool extended, uint8_t data[], uint8_t len);
uint8_t calculateCRC(uint32_t id, uint8_t data[], uint8_t len);
void readCANMessages();
void sendAbsoluteAxisCommand(uint32_t id, uint16_t speed, uint8_t acceleration, int32_t absAxis);
void moveMotor2ByMM(float mm, uint16_t speed, uint8_t acceleration);
void moveMotor1ByMM(float mm, uint16_t speed, uint8_t acceleration);
void readCANFeedback();
//void moveToCoordinates(float x, float y);
void moveToCoordinates(float x, float y, float speed);

void scanUART();
void calibrateMotorEncoder(uint32_t motorID);
void controlMotor5(char *command);
void rotatePhone(float degree);
void Return_xy();

#endif

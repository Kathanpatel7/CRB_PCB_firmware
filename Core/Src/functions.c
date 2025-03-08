#include "functions.h"
#include "stm32f0xx_hal.h"
#include "main.h"
#include "can.h"
#include "uart.h"
#include <stdbool.h>
#include <string.h>

// Flags for motor motion completion
bool waitingForMotor2 = false;
bool commandToMotor2Sent = false;
bool waitingForMotor1 = false;
bool commandToMotor1Sent = false;
bool motor1Homed = false;
bool motor2Homed = false;
bool phoneRotationMotorHomed = false;
bool imageMountMotorHomed = false;
bool drawerMotorHomed = false;

bool motor1MotionComplete = false;
bool motor2MotionComplete = false;
bool phoneRotationMotorComplete = false;
bool imageMountMotorComplete = false;
bool drawerMotorComplete = false;

/**
 * @brief  Send homing parameters to the motor
 * @param  id: Motor ID
 * @param  homeTrig: Homing trigger
 * @param  homeDir: Homing direction
 * @param  homeSpeed: Homing speed
 * @param  endLimit: End limit
 * @retval None
 */
void sendHomingParameters(uint32_t id, uint8_t homeTrig, uint8_t homeDir, uint16_t homeSpeed, uint8_t endLimit)
{
    uint8_t data[8] = {0};
    data[0] = homeTrig;
    data[1] = homeDir;
    data[2] = (homeSpeed >> 8) & 0xFF;
    data[3] = homeSpeed & 0xFF;
    data[4] = endLimit;

    sendCANCommandWithCRC(id, false, data, sizeof(data));
}

void sendUARTMessage(const char *message) {
    HAL_UART_Transmit(&huart3, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
}

void system_initialisation() {
    // Send homing command to all motors
    sendHomingCommand(0x001);  // Homing Motor 1

    // Wait for Motor 1 to home
    while (!motor1Homed) {
        processCANMessage();
        HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
    }

    sendHomingCommand(0x002);  // Homing Motor 2

    // Homing other motors sequentially
    sendHomingCommand(0x003);  // Phone Rotation Motor
    sendHomingCommand(0x004);  // Image Mount Motor
    sendHomingCommand(0x005);  // Drawer Motor

    // Set servo positions (example positions)
    setServoPosition(TIM_CHANNEL_1, 0);   // Set Servo 1 position
    setServoPosition(TIM_CHANNEL_2, 70);  // Set Servo 2 position

    // Wait for all motors to home
    while (!drawerMotorHomed || !phoneRotationMotorHomed || !imageMountMotorHomed || !motor2Homed) {
        processCANMessage();
        HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
    }

}

void Return_xy() {
    // Send homing command to all motors
    sendHomingCommand(0x001);  // Homing Motor 1

    // Wait for Motor 1 to home
    while (!motor1Homed) {
        processCANMessage();
        HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
    }

    sendHomingCommand(0x002);  // Homing Motor 2

    // Set servo positions (example positions)
    setServoPosition(TIM_CHANNEL_1, 0);   // Set Servo 1 position
    setServoPosition(TIM_CHANNEL_2, 70);  // Set Servo 2 position

    // Wait for all motors to home
    while (!motor2Homed) {
        processCANMessage();
        HAL_Delay(100);  // Small delay to avoid spamming the CAN bus
    }

}


/**
 * @brief  Send a homing command to the motor
 * @param  id: Motor ID
 * @retval None
 */
void sendHomingCommand(uint32_t id) {
    uint8_t homingCommand[1] = {0x91};  // Homing command (0x91)
    sendCANCommandWithCRC(id, false, homingCommand, sizeof(homingCommand));

    // Mark variables as unused to avoid warnings


    // Optional: Toggle an LED to indicate command sent
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
    HAL_Delay(100);  // Brief delay to make the toggle visible
}

/**
 * @brief  Send a CAN message with CRC for integrity
 * @param  id: Motor ID
 * @param  extended: Use extended frame
 * @param  data: Data to send
 * @param  len: Length of data
 * @retval None
 */
void sendCANCommandWithCRC(uint32_t id, bool extended, uint8_t data[], uint8_t len) {
  uint8_t crc = calculateCRC(id, data, len);  // Calculate CRC
  uint8_t dataWithCRC[len + 1];  // New array to hold data and CRC

  // Copy the original data into the new array
  for (int i = 0; i < len; i++) {
    dataWithCRC[i] = data[i];
  }

  // Append the CRC to the new array
  dataWithCRC[len] = crc;

  // Send the data array with CRC
  sendCANMessage(id, extended, dataWithCRC, len + 1);
}
/**
 * @brief  Calculate CRC for the CAN message
 * @param  id: Motor ID
 * @param  data: Data to calculate CRC on
 * @param  len: Length of data
 * @retval Calculated CRC value
 */
// Function to calculate 8-bit CRC (simple summation and masking)
uint8_t calculateCRC(uint32_t id, uint8_t data[], uint8_t len) {
  uint8_t crc = (uint8_t)(id & 0xFF);  // Start with the lowest byte of CAN ID
  for (int i = 0; i < len; i++) {
    crc += data[i];  // Add each data byte to the CRC
  }
  crc &= 0xFF;  // Mask to ensure only lower 8 bits are used
  return crc;    // Return the final CRC value
}


/**
 * @brief  Send an absolute axis movement command
 * @param  id: Motor ID
 * @param  speed: Speed for movement
 * @param  acceleration: Acceleration for movement
 * @param  absAxis: Absolute position for movement
 * @retval None
 */
void sendAbsoluteAxisCommand(uint32_t id, uint16_t speed, uint8_t acceleration, int32_t absAxis) {
    uint8_t commandData[7];

    // Fill commandData with the required bytes
    commandData[0] = 0xF5;  // Position mode 4 (absolute motion)
    commandData[1] = (uint8_t)(speed >> 8);      // Speed high byte
    commandData[2] = (uint8_t)(speed & 0xFF);    // Speed low byte
    commandData[3] = acceleration;               // Acceleration byte
    commandData[4] = (uint8_t)((absAxis >> 16) & 0xFF);  // Absolute axis high byte
    commandData[5] = (uint8_t)((absAxis >> 8) & 0xFF);   // Absolute axis middle byte
    commandData[6] = (uint8_t)(absAxis & 0xFF);          // Absolute axis low byte

    // Send the CAN command with CRC
    sendCANCommandWithCRC(id, false, commandData, sizeof(commandData));


}


/**
 * @brief  Move to specific coordinates (X, Y)
 * @param  x: X coordinate
 * @param  y: Y coordinate
 * @retval None
 */
void moveToCoordinates(float x_axis, float y_axis, float speed) {
    // Define motion parameters
    const int X_MIN = 0;
    const int X_MAX = 100;
    const int Y_MIN = 0;
    const int Y_MAX = 250;
    const uint32_t timeout = 2000;  // 2-second timeout

    // Validate x and y limits
    if (x_axis < X_MIN || x_axis > X_MAX) {
        return;
    }
    if (y_axis < Y_MIN || y_axis > Y_MAX) {
        return;
    }

    // Map speed to rpm and acc
    int rpm = (int)(speed * 3000 / 100);  // Map speed (0-100) to rpm (0-3000)
    int acc = (int)(speed * 255 / 100);  // Map speed (0-100) to acc (0-255)

    // Convert coordinates to pulses
    int32_t pulseXaxis = (int32_t)(x_axis * PULSES_PER_MM_MOTOR1);
    int32_t pulseY = (int32_t)(y_axis * PULSES_PER_MM_MOTOR2);

    // Log the target coordinates
    // Example logging can be added here if needed for debugging

    // Send absolute position commands for both motors
    sendAbsoluteAxisCommand(0x001, rpm, acc, -pulseXaxis);  // Move Motor 1 to x
    sendAbsoluteAxisCommand(0x002, rpm, acc, -pulseY);      // Move Motor 2 to y

    // Start timeout timer
    uint32_t startTime = HAL_GetTick();

    // Initial feedback check
    processCANMessage();

    // Wait until both motors complete movement or timeout occurs
    while ((!motor1MotionComplete || !motor2MotionComplete) &&
           (HAL_GetTick() - startTime) < timeout) {
        HAL_Delay(10);  // Small delay to avoid spamming CAN bus
        processCANMessage();
    }

    // Check if timeout occurred
    if (HAL_GetTick() - startTime >= timeout) {
        // Handle timeout if necessary
    } else {
        // Handle successful completion
    }

    // Reset motion complete flags for next use
    motor1MotionComplete = 0;
    motor2MotionComplete = 0;
    x_axis = 0;
    y_axis = 0;
}


/**
 * @brief  Scan UART data
 * @retval None
 */


/**
 * @brief  Calibrate motor encoder
 * @param  motorID: Motor ID
 * @retval None
 */
void calibrateMotorEncoder(uint32_t motorID)
{
    // Example function to calibrate motor encoder
    // Send homing command and parameters
    sendHomingCommand(motorID);
}

/**
 * @brief  Control Motor 5 based on command string
 * @param  command: Command for controlling motor
 * @retval None
 */
void controlMotor5(char *command) {
    int rpm = 2000;
    int acc = 200;
    int32_t pulsePosition;

    // Determine the action based on the command
    if (strcmp(command, "open") == 0) {
        pulsePosition = 215 * PULSES_PER_MM_MOTOR2;  // Target position for "START" (e.g., 215mm)
        sendAbsoluteAxisCommand(0x005, rpm, acc, -pulsePosition);  // Command Motor 5 to move
    }
    else if (strcmp(command, "close") == 0) {
        pulsePosition = 0;  // Position to hold when stopped (could also be set to current position)
        sendAbsoluteAxisCommand(0x005, rpm, acc, -pulsePosition);  // Command Motor 5 to stop at position
    }
    else {
        return;
    }

    drawerMotorComplete = false;  // Reset completion flag before the move
    uint32_t startTime = HAL_GetTick();  // Start timeout timer
    uint32_t timeout = 5000;  // Timeout of 2 seconds

    HAL_Delay(100);  // Delay to ensure motor motion has time to initiate

    // Monitor motor motion until completion or timeout
    while (!drawerMotorComplete && (HAL_GetTick() - startTime) < timeout) {
        HAL_Delay(10);  // Small delay to avoid spamming CAN bus
        processCANMessage();  // Continuously check motion status

        // Optional: Debug print to check feedback reception
        // printf("Checking motor 5 motion status...\n");
    }

    if ((HAL_GetTick() - startTime) >= timeout) {
        // Handle timeout
        // printf("Error: Motion timeout.\n");
    } else {
        // Motion complete
        // printf("Motor 5 motion complete.\n");
    }

    drawerMotorComplete = false;  // Reset completion flag
}

void rotatePhone(float degree) {
    // Constants
    const int32_t PULSES_PER_ROTATION = 16400;  // Pulses for a full 360-degree rotation
    const int RPM = 500;                       // Rotation speed
    const int ACC = 100;                        // Acceleration

    // Calculate the number of pulses required for the given degree
    int32_t pulses = (int32_t)((degree / 360.0f) * PULSES_PER_ROTATION);

    // Send the command to rotate the phone
    sendAbsoluteAxisCommand(0x003, RPM, ACC, pulses);

    phoneRotationMotorComplete = false;  // Reset completion flag before the move
    uint32_t startTime = HAL_GetTick();  // Start timeout timer
    uint32_t timeout = 3000;  // Timeout of 2 seconds

    while (!phoneRotationMotorComplete && (HAL_GetTick() - startTime) < timeout) {
            HAL_Delay(10);  // Small delay to avoid spamming CAN bus
            processCANMessage();  // Continuously check motion status

            // Optional: Debug print to check feedback reception
            // printf("Checking motor 5 motion status...\n");
        }
}


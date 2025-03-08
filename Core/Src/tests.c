#include "tests.h"
#include "main.h"
#include "can.h"
#include "timer.h"
#include "uart.h"
#include "functions.h"

// Define the width and length for the zigzag pattern
#define WIDTH 70
#define LENGTH 165
int start_X = -93;
int start_Y = 210;

// Define servo positions
#define SERVO1_ACTUATE_POSITION 80
#define SERVO1_RETRACT_POSITION 0
#define SERVO2_ACTUATE_POSITION 0
#define SERVO2_RETRACT_POSITION 65

// Function prototypes
void moveToCoordinates(float x, float y, float speed);
void readCANFeedback(void);
void setServoPosition(uint32_t channel, uint16_t position);
void delay_ms(uint32_t ms);

void screen_test(int px1, int py1, int px2, int py2, int num_steps, int speed) {
    //float speed = 50.0; // Set default speed for the test
    float stepsize = (float)(py2 - py1) / num_steps;

    // Move to initial position
    moveToCoordinates(px1, py1, speed);
    delay_ms(100);
    setServoPosition(TIM_CHANNEL_1, SERVO1_ACTUATE_POSITION); // Activate servo
    delay_ms(100);

    for (int m = 0; m < num_steps; m++) {
        processCANMessage();

        // Determine the direction of the horizontal movement
        if (m % 2 == 0) {
            moveToCoordinates(px1, py1 + (m * stepsize), speed);
            processCANMessage();
            moveToCoordinates(px2, py1 + (m * stepsize), speed);
            processCANMessage();
        } else {
            moveToCoordinates(px2, py1 + (m * stepsize), speed);
            processCANMessage();
            moveToCoordinates(px1, py1 + (m * stepsize), speed);
            processCANMessage();
        }
    }

    // Retract servo after completing zigzag
    setServoPosition(TIM_CHANNEL_1, SERVO1_RETRACT_POSITION);
    delay_ms(100);

    // Move back to original positions after completing the test
    moveToCoordinates(0, 40, speed);
    processCANMessage();
    moveToCoordinates(0, 10, speed);
    processCANMessage();
}

void speaker_test(int px1, int py1, int px2, int py2, int speed) {
    //float speed = 60.0; // Set default speed for the test

    moveToCoordinates(px1, py1, speed);
    HAL_Delay(3000);
    moveToCoordinates(px2, py2, speed);
    HAL_Delay(3000);
    moveToCoordinates(px1, py1, speed);
}

void camera_test(int mm, int speed) {
    int32_t pulseXaxis = (int32_t)(mm * PULSES_PER_MM_MOTOR1);
    sendAbsoluteAxisCommand(0x004, 2000, 240, -pulseXaxis);
    HAL_Delay(4000);
    sendAbsoluteAxisCommand(0x004, 2000, 240, 0);
    HAL_Delay(1000);
}

void multitouch_screen_test(int px1, int py1, int px2, int py2, int speed) {
   // float speed = 70.0; // Set default speed for the test

    moveToCoordinates(px1, py1, speed);
    HAL_Delay(3000);
    moveToCoordinates(px2, py2, speed);
    HAL_Delay(3000);

    setServoPosition(TIM_CHANNEL_1, SERVO1_ACTUATE_POSITION);
    setServoPosition(TIM_CHANNEL_2, SERVO2_ACTUATE_POSITION);

    HAL_Delay(3000);

    moveToCoordinates(px1, py1, speed);
    HAL_Delay(3000);

    setServoPosition(TIM_CHANNEL_1, SERVO1_RETRACT_POSITION);
    setServoPosition(TIM_CHANNEL_2, SERVO2_RETRACT_POSITION);
}

void setServoPosition(uint32_t channel, uint16_t position) {
    TIM_HandleTypeDef *htim = &htim2;
    uint32_t min_pulse = (htim->Init.Period + 1) * 5 / 100;
    uint32_t max_pulse = (htim->Init.Period + 1) * 10 / 100;

    uint32_t pulse = min_pulse + ((max_pulse - min_pulse) * position) / 180;
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}

void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

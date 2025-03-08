#ifndef TESTS_H
#define TESTS_H

#include "main.h"
#include "can.h"
#include "timer.h"
#include "uart.h"

// Define the width and length for the zigzag pattern
#define WIDTH 70
#define LENGTH 165

// Define servo positions
#define SERVO1_ACTUATE_POSITION 80
#define SERVO1_RETRACT_POSITION 0
#define SERVO2_ACTUATE_POSITION 0
#define SERVO2_RETRACT_POSITION 65

// Function prototypes
//void screen_test(int px1, int py1, int px2, int py2, int num_steps);
//void camera_test(int mm);
//void speaker_test(int px1, int py1, int px2, int py2) ;
//void multitouch_screen_test(int px1, int py1, int px2, int py2);
//void moveToCoordinates(int x, int y);
void moveToCoordinates(float x, float y, float speed);
void multitouch_screen_test(int px1, int py1, int px2, int py2, int speed); // Add 'speed'
void screen_test(int px1, int py1, int px2, int py2, int num_steps, int speed); // Add 'speed'
void speaker_test(int px1, int py1, int px2, int py2, int speed); // Add 'speed'
void camera_test(int mm, int speed); // Add 'speed'

void readCANFeedback(void);
void setServoPosition(uint32_t channel, uint16_t position);
void delay_ms(uint32_t ms);

#endif // TESTS_H

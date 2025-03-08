/*
 * timer.h
 *
 * Created on: Nov 11, 2024
 * Author: kathanpatel
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

/* Timer Handles */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim17;

/* Function Prototypes */
void Start_PWM_All_Channels(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM16_Init(void);
void MX_TIM17_Init(void);
void SetLightBrightness(uint8_t light, uint8_t brightness);

#endif /* INC_TIMER_H_ */

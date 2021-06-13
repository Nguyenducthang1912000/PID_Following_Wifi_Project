/*
 * MotorControl_HAL.c
 *
 *  Created on: Apr 29, 2021
 *      Author: Duc Thang
 */
#include "HAL_MOTOR_CONTROL.h"
void MotorL_EnablePWM(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}
/**
 * @brief Start left engine
 * @param using HAL LIBRARY
 */
void MotorL_DisablePWM(void)
{
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}
void MotorR_EnablePWM(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}
void MotorR_DisablePWM(void)
{
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
}
void MotorL_SetPWM(int32_t PWMVal)
{
	if(PWMVal >= 7200)
	{
		PWMVal = 7200;
	}
	else if(PWMVal <= -7200)
	{
		PWMVal = -7200;
	}
	if(PWMVal >= 0)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,PWMVal);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,7200+PWMVal);
	}
}
void MotorR_SetPWM(int32_t PWMVal)
{
	if(PWMVal >= 7200)
	{
		PWMVal = 7200;
	}
	else if(PWMVal <= -7200)
	{
		PWMVal = -7200;
	}
	if(PWMVal >= 0)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,PWMVal);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,7200+PWMVal);
	}
}




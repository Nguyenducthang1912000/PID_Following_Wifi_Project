/*
 * MotorControl_HAL.h
 *
 *  Created on: Apr 29, 2021
 *      Author: Duc Thang
 */
#pragma once
#include <main.h>
#ifndef MOTORCONTROL_HAL_H_
#define MOTORCONTROL_HAL_H_
void MotorL_EnablePWM(void);
void MotorL_DisablePWM(void);
void MotorR_EnablePWM(void);
void MotorR_DisablePWM(void);
void MotorR_SetPWM(int32_t PWMVal);
void MotorL_SetPWM(int32_t PWMVal);
#endif /* MOTORCONTROL_HAL_H_ */

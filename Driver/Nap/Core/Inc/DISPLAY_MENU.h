/*
 * Menu.h
 *
 *  Created on: May 6, 2021
 *      Author: DucThang
 */
#pragma once
#include "main.h"
#include "HAL_I2C_LCD.h"
#include "stdio.h"
#include "string.h"
void executeAction(uint8_t line);
void Menu_system_control(uint8_t Menu_type,uint8_t line);
#ifndef SRC_MENU_H_
#define SRC_MENU_H_
/* Value modify flag --------------------------------------------------------*/
extern uint8_t Kp_modify_flag, Ki_modify_flag, Kd_modify_flag;
extern uint8_t Left_modify_flag, Right_modify_flag;
extern uint8_t First_point_modify_flag, Last_point_modify_flag;
/* Exit menu showing process ------------------------------------------------*/
extern uint8_t cancel_menu;
extern uint8_t cancel_reading_card;

/* Exit running process -----------------------------------------------------*/
extern uint8_t cancel_running;

/* System private value -----------------------------------------------------*/
extern float Kp, Ki, Kd;
extern int8_t First_point,Last_point;
extern int16_t Left, Right;
extern uint16_t Sensor_Threshold[6];
extern uint16_t Sensor_ADC_Value[6];

/* Step for running --------------------------------------------------------*/
#endif /* SRC_MENU_H_ */

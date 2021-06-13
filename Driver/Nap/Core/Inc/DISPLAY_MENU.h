/*
 * Menu.h
 *
 *  Created on: May 6, 2021
 *      Author: DucThang
 */
#include "main.h"
#include "HAL_I2C_LCD.h"
#include "stdio.h"
#include "string.h"
void Mainmenu(uint8_t line);
void PID_menu(uint8_t line);
void Speed_menu(uint8_t line);
void Color_Studying_process(void);
void LineDetect_show(void);
void Wifi_Connect_establish(void);
void Running(void);
void executeAction(uint8_t line);
void Menu_system_control(uint8_t Menu_type,uint8_t line);
#ifndef SRC_MENU_H_
#define SRC_MENU_H_





#endif /* SRC_MENU_H_ */

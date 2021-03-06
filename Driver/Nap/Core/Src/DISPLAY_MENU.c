/*
 * Menu.c
 *
 *  Created on: May 6, 2021
 *      Author: DucThang
 */
#include "main.h"
/* Menu Types definition -----------------------------------------------------*/
#define Running_Process 				0
#define Main_menu 						1
#define Color_Processing 				2
#define PID_Menu 						3
#define Engine_menu 					4
#define LineDetect_Show 				5
#define Wifi_connect 					6
#define Saving_process 					7
#define Path_solver_menu				8
#define Path_show_menu					9

#define ADC_Sample_Times				50000
#define Number_of_Sensors			 	6

/* System buffer ------------------------------------------------------------*/
char kp_val[6], ki_val[6], kd_val[6];
char kp_str[20], ki_str[20], kd_str[20];
char string[12];
char Left_str[20], Right_str[20];
char Left_val[5], Right_val[5];
char First_str[20],Last_str[20];
/* Menu control flag --------------------------------------------------------*/
uint8_t menu_display = 1;
uint8_t Menu_type = 1;
uint8_t Run_flag = 0;
uint8_t Color_Read = 1;
uint8_t Wifi_Connecting = 1;
uint8_t line = 1;

/* Function prototype -------------------------------------------------------*/
static void Mainmenu(uint8_t line);
static void Color_Studying_process(void);
static void PID_menu(uint8_t line);
static void Speed_menu(uint8_t line);
static void LineDetect_show(void);
static void Wifi_Connect_establish(void);
static void Path_Solver(uint8_t line_no);
static void Path_show(void);
void Saving_Process(void);
uint8_t MFRC522_Check();
void MFRC522_Reset();
void Running(void);

/* Function declaration ----------------------------------------------------*/
void Menu_system_control(uint8_t Menu_type, uint8_t line) {
	switch (Menu_type) {
	case Running_Process:
		Running();
		break;
	case Main_menu:
		Mainmenu(line);
		break;
	case Color_Processing:
		Color_Studying_process();
		break;
	case PID_Menu:
		PID_menu(line);
		break;
	case Engine_menu:
		Speed_menu(line);
		break;
	case LineDetect_Show:
		LineDetect_show();
		break;
	case Wifi_connect:
		Wifi_Connect_establish();
		break;
	case Saving_process:
		Saving_Process();
		break;
	case Path_solver_menu:
		Path_Solver(line);
		break;
	case Path_show_menu:
		Path_show();
		break;
	}
}

static void Mainmenu(uint8_t line) {
	switch (line) {
	case 1:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(">Car Run            ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(" Color studying     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" PID value modify   ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Max speed config   ");
		break;
	case 2:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(" Car Run            ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(">Color studying     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" PID value modify   ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Max speed config   ");
		break;
	case 3:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(" Car Run            ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(" Color studying     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(">PID value modify   ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Max speed config   ");
		break;
	case 4:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(" Car Run            ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(" Color studying     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" PID value modify   ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(">Max speed config   ");
		break;
	case 5:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(">Line Detect show   ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(" Read RFID Card     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" Wifi Connect       ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Path Solver        ");
		break;
	case 6:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(" Line Detect show   ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(">Read RFID Card     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" Wifi Connect       ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Path Solver        ");
		break;
	case 7:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(" Line Detect show   ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(" Read RFID Card     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(">Wifi Connect       ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Path Solver        ");
		break;
	case 8:
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(" Line Detect show   ");
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(" Read RFID Card     ");
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" Wifi Connect       ");
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(">Path Solver        ");
		break;
	}
}

static void PID_menu(uint8_t line) {

	switch (line) {
	case 1:
		sprintf(kp_str, ">Kp = %.2f         ", Kp);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(kp_str);
		sprintf(ki_str, " Ki = %.2f         ", Ki);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(ki_str);
		sprintf(kd_str, " Kd = %.2f         ", Kd);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(kd_str);
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Return to main menu");
		break;
	case 2:
		sprintf(kp_str, " Kp = %.2f         ", Kp);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(kp_str);
		sprintf(ki_str, ">Ki = %.2f         ", Ki);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(ki_str);
		sprintf(kd_str, " Kd = %.2f         ", Kd);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(kd_str);
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Return to main menu");
		break;
	case 3:
		sprintf(kp_str, " Kp = %.2f         ", Kp);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(kp_str);
		sprintf(ki_str, " Ki = %.2f         ", Ki);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(ki_str);
		sprintf(kd_str, ">Kd = %.2f         ", Kd);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(kd_str);
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(" Return to main menu");
		break;
	case 4:
		sprintf(kp_str, " Kp = %.2f         ", Kp);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(kp_str);
		sprintf(ki_str, " Ki = %.2f         ", Ki);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(ki_str);
		sprintf(kd_str, " Kd = %.2f         ", Kd);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(kd_str);
		lcd_send_cmd(0x80 | 0x54);
		lcd_send_string(">Return to main menu");
		break;
	}
}
static void Speed_menu(uint8_t line) {
	switch (line) {
	case 1:
		sprintf(Left_str, ">Left Eng = %d", Left);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(Left_str);
		sprintf(Right_str, " Right Eng = %d", Right);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(Right_str);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" Return to main menu");
		break;
	case 2:
		sprintf(Left_str, " Left Eng = %d", Left);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(Left_str);
		sprintf(Right_str, ">Right Eng = %d", Right);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(Right_str);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(" Return to main menu");
		break;
	case 3:
		sprintf(Left_str, " Left Eng = %d", Left);
		lcd_send_cmd(0x80 | 0x00);
		lcd_send_string(Left_str);
		sprintf(Right_str, " Right Eng = %d", Right);
		lcd_send_cmd(0x80 | 0x40);
		lcd_send_string(Right_str);
		lcd_send_cmd(0x80 | 0x14);
		lcd_send_string(">Return to main menu");
		break;
	}
}

static void Color_Studying_process(void) {
#ifdef	READ_WHITE_LINE
	uint16_t WhiteLine[] = {0 ,0 ,0 ,0, 0, 0};
#endif
#ifdef READ_BLACK_LINE
	uint16_t BlackLine[] = {0, 0, 0, 0, 0, 0};
#endif
	Color_Read = 1;
	lcd_send_cmd(0x80 | 0x00);
	lcd_send_string("Out line read      ");
	lcd_send_cmd(0x80 | 0x40);
	lcd_send_string("Press B to begin   ");
	while(Color_Read){
		if (HAL_GPIO_ReadPin(ButtonB_GPIO_Port, ButtonB_Pin) == 0)
		{
			while (HAL_GPIO_ReadPin(ButtonB_GPIO_Port, ButtonB_Pin) == 0)
			{
				HAL_Delay(50);
			}
			for(int i=0;i<ADC_Sample_Times;i++)
			{
				for(int i=0;i<Number_of_Sensors;i++)
				{
#ifdef READ_WHITE_LINE
					if(Sensor_ADC_Value[i] > WhiteLine[i])
					{
						WhiteLine[i] = Sensor_ADC_Value[i];
					}
#endif
#ifdef READ_BLACK_LINE
					if(Sensor_ADC_Value[i] < WhiteLine[i])
					{
						BlackLine[i] = Sensor_ADC_Value[i];
					}
#endif
				}
			}
#ifdef READ_WHITE_LINE
			for(int i=0;i<Number_of_Sensors;i++)
			{
				Sensor_Threshold[i]=WhiteLine[i] - 10;
			}
#endif
#ifdef READ_BLACK_LINE
			for(int i=0;i<Number_of_Sensors;i++)
			{
				if(i == 0)
				{
					Sensor_Threshold[i]=BlackLine[i];
				}
				else
					Sensor_Threshold[i]=BlackLine[i];
			}
#endif
			Color_Read = 0;
		}
	}
	lcd_clear();
	lcd_send_cmd(0x80 | 0x00);
	lcd_send_string("Done               ");
	HAL_Delay(200);
	Menu_type = Main_menu;
	lcd_clear();
}

static void LineDetect_show(void) {
	lcd_send_cmd(0x80 | 0x00);
	lcd_send_string("Line Detect        ");
	lcd_send_cmd(0x80 | 0x40);
	lcd_send_string("Press C for cancel ");
	while (cancel_menu) {
		for (int8_t i = 0; i < 6; i++) {
#ifdef READ_WHITE_LINE
			if (Sensor_ADC_Value[0] < Sensor_Threshold[0]) {
				lcd_send_cmd(0x80 | 0x16);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[1] < Sensor_Threshold[1]) {
				lcd_send_cmd(0x80 | 0x18);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[2] < Sensor_Threshold[2]) {
				lcd_send_cmd(0x80 | 0x1A);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[3] < Sensor_Threshold[3]) {
				lcd_send_cmd(0x80 | 0x1C);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[4] < Sensor_Threshold[4]) {
				lcd_send_cmd(0x80 | 0x1E);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[5] < Sensor_Threshold[5]) {
				lcd_send_cmd(0x80 | 0x20);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[0] >= Sensor_Threshold[0]) {
				lcd_send_cmd(0x80 | 0x16);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[1] >= Sensor_Threshold[1]) {
				lcd_send_cmd(0x80 | 0x18);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[2] >= Sensor_Threshold[2]) {
				lcd_send_cmd(0x80 | 0x1A);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[3] >= Sensor_Threshold[3]) {
				lcd_send_cmd(0x80 | 0x1C);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[4] >= Sensor_Threshold[4]) {
				lcd_send_cmd(0x80 | 0x1E);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[5] >= Sensor_Threshold[5]) {
				lcd_send_cmd(0x80 | 0x20);
				lcd_send_string(" ");
			}
#endif
#ifdef READ_BLACK_LINE
			if (Sensor_ADC_Value[0] <= Sensor_Threshold[0]) {
				lcd_send_cmd(0x80 | 0x16);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[1] <= Sensor_Threshold[1]) {
				lcd_send_cmd(0x80 | 0x18);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[2] <= Sensor_Threshold[2]) {
				lcd_send_cmd(0x80 | 0x1A);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[3] <= Sensor_Threshold[3]) {
				lcd_send_cmd(0x80 | 0x1C);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[4] <= Sensor_Threshold[4]) {
				lcd_send_cmd(0x80 | 0x1E);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[5] <= Sensor_Threshold[5]) {
				lcd_send_cmd(0x80 | 0x20);
				lcd_send_string("1");
			}
			if (Sensor_ADC_Value[0] > Sensor_Threshold[0]) {
				lcd_send_cmd(0x80 | 0x16);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[1] > Sensor_Threshold[1]) {
				lcd_send_cmd(0x80 | 0x18);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[2] > Sensor_Threshold[2]) {
				lcd_send_cmd(0x80 | 0x1A);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[3] > Sensor_Threshold[3]) {
				lcd_send_cmd(0x80 | 0x1C);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[4] > Sensor_Threshold[4]) {
				lcd_send_cmd(0x80 | 0x1E);
				lcd_send_string(" ");
			}
			if (Sensor_ADC_Value[5] > Sensor_Threshold[5]) {
				lcd_send_cmd(0x80 | 0x20);
				lcd_send_string(" ");
			}
#endif
		}
	}
	lcd_clear();
}

void Saving_Process(void)
{
	unsigned char Card_ID[5];
	char Card_Buffer_Str[4];
	char concat_Buff[20];
	cancel_reading_card = 1;
	memset(concat_Buff,0,sizeof(concat_Buff));
	memset(Card_Buffer_Str,0,sizeof(Card_Buffer_Str));
	MFRC522_Init();
	lcd_send_cmd(0x80 | 0x00);
	lcd_send_string("RFID Card Read     ");
	lcd_send_cmd(0x80 | 0x40);
	lcd_send_string("Press C for cancel ");
	while(cancel_reading_card)
	{
		if(MFRC522_Check(Card_ID) == MI_OK)
		{
			sprintf(Card_Buffer_Str,"%02x-",Card_ID[0]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x-",Card_ID[1]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x-",Card_ID[2]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x-",Card_ID[3]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x ",Card_ID[4]);
			strcat(concat_Buff,Card_Buffer_Str);
			if(concat_Buff == "d1-b0-af-21-ef ")
			{
				lcd_send_cmd(0x80 | 0x14);
				lcd_send_string("83-ee-85-1a-f2");
			}
			else
			{
				lcd_send_cmd(0x80 | 0x14);
				lcd_send_string(concat_Buff);
			}
			MFRC522_Reset();
		}
	}
	lcd_clear();
}

static void Wifi_Connect_establish(void)
{

		Menu_type = Main_menu;
		lcd_clear();
}
static void Path_Solver(uint8_t line)
{
	switch (line) {
		case 1:
			sprintf(First_str, ">First Point: %2d  ", First_point);
			lcd_send_cmd(0x80 | 0x00);
			lcd_send_string(First_str);
			sprintf(Last_str, " Last Point: %2d    ", Last_point);
			lcd_send_cmd(0x80 | 0x40);
			lcd_send_string(Last_str);
			lcd_send_cmd(0x80 | 0x14);
			lcd_send_string(" Submit             ");
			lcd_send_cmd(0x80 | 0x54);
			lcd_send_string(" Return to main menu");
			break;
		case 2:
			sprintf(First_str, " First Point: %2d  ", First_point);
			lcd_send_cmd(0x80 | 0x00);
			lcd_send_string(First_str);
			sprintf(Last_str, ">Last Point: %2d    ", Last_point);
			lcd_send_cmd(0x80 | 0x40);
			lcd_send_string(Last_str);
			lcd_send_cmd(0x80 | 0x14);
			lcd_send_string(" Submit             ");
			lcd_send_cmd(0x80 | 0x54);
			lcd_send_string(" Return to main menu");
			break;
		case 3:
			sprintf(First_str, " First Point: %2d  ", First_point);
			lcd_send_cmd(0x80 | 0x00);
			lcd_send_string(First_str);
			sprintf(Last_str, " Last Point: %2d    ", Last_point);
			lcd_send_cmd(0x80 | 0x40);
			lcd_send_string(Last_str);
			lcd_send_cmd(0x80 | 0x14);
			lcd_send_string(">Submit             ");
			lcd_send_cmd(0x80 | 0x54);
			lcd_send_string(" Return to main menu");
			break;
		case 4:
			sprintf(First_str, " First Point: %2d   ", First_point);
			lcd_send_cmd(0x80 | 0x00);
			lcd_send_string(First_str);
			sprintf(Last_str, " Last Point: %2d     ", Last_point);
			lcd_send_cmd(0x80 | 0x40);
			lcd_send_string(Last_str);
			lcd_send_cmd(0x80 | 0x14);
			lcd_send_string(" Submit             ");
			lcd_send_cmd(0x80 | 0x54);
			lcd_send_string(">Return to main menu");
			break;
	}
}
static void Path_show(void)
{
	char getPath_str[20];
	GetString(First_point, Last_point, getPath_str);
	lcd_send_cmd(0x80 | 0x00);
	lcd_send_string("Path direction      ");
	lcd_send_cmd(0x80 | 0x40);
	lcd_send_string(getPath_str);
	lcd_send_cmd(0x80 | 0x14);
	lcd_send_string("Press C to return   ");
}
void executeAction(uint8_t line) {
	switch (line) {
	case 1:
		switch (Menu_type) {
		case Main_menu:
			cancel_running = 1;
			Menu_type = Running_Process;
			break;
		case PID_Menu:
			if (Kp_modify_flag == 0) {
				Kp_modify_flag = 1;
				line = 1;
			} else {
				Kp_modify_flag = 0;
			}
			break;
		case Engine_menu:
			if (Left_modify_flag == 0) {
				Left_modify_flag = 1;
				line = 1;
			} else {
				Left_modify_flag = 0;
			}
			break;
		case Path_solver_menu:
			if (First_point_modify_flag == 0){
				First_point_modify_flag = 1;
				line = 1;
			}
			else
			{
				First_point_modify_flag = 0;
			}
			break;
		}

		lcd_clear();
		break;

	case 2:
		switch (Menu_type) {
		case Main_menu:
			Menu_type = Color_Processing;
			break;
		case PID_Menu:
			if (Ki_modify_flag == 0) {
				Ki_modify_flag = 1;
				line = 2;
			} else {
				Ki_modify_flag = 0;
			}
			break;
		case Engine_menu:
			if (Right_modify_flag == 0) {
				Right_modify_flag = 1;
				line = 2;
			} else {
				Right_modify_flag = 0;
			}
			break;
		case Path_solver_menu:
			if (Last_point_modify_flag == 0) {
				Last_point_modify_flag = 1;
				line = 2;
			} else {
				Last_point_modify_flag = 0;
			}
			break;
		}

		lcd_clear();
		break;

	case 3:
		switch (Menu_type) {
		case Main_menu:
			Menu_type = PID_Menu;
			break;
		case PID_Menu:
			if (Kd_modify_flag == 0) {
				Kd_modify_flag = 1;
				line = 3;
			} else {
				Kd_modify_flag = 0;
			}
			break;
		case Engine_menu:
			Menu_type = Main_menu;
			break;
		case Path_solver_menu:
			Menu_type = Path_show_menu;
			break;
		}

		lcd_clear();
		break;

	case 4:
		switch (Menu_type) {
		case Main_menu:
			Menu_type = Engine_menu;
			break;
		case PID_Menu:
			Menu_type = Main_menu;
			break;
		case Path_solver_menu:
			Menu_type = Main_menu;
			break;
		}
		lcd_clear();
		break;

	case 5:
		switch (Menu_type) {
		case Main_menu:
			cancel_menu = 1;
			Menu_type = LineDetect_Show;
			break;
		}
		lcd_clear();
		break;

	case 6:
		Menu_type = Saving_process;
		lcd_clear();
		break;

	case 7:
		Menu_type = Wifi_connect;
		lcd_clear();
		break;

	case 8:
		Menu_type = Path_solver_menu;
		lcd_clear();
		break;
	}
}

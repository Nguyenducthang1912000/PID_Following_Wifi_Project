/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Number_of_Menu_firstline 		1
#define Maximum_Menu_line 				8
#define Maximum_PID_line 				4
#define Maximum_Engine_line 			3
#define Maximum_Path_Solver_line		4
/* Menu Types definition -----------------------------------------------------*/
#define Running_menu 					0
#define Main_menu 						1
#define Color_Processing 				2
#define PID_Menu 						3
#define Engine_menu 					4
#define LineDetect_Show 				5
#define Wifi_Connect 					6
#define Saving_process 					7
#define Path_solver						8
#define Path_show_menu					9

/* Increase or Decrease amount for PID ---------------------------------------*/
#define Kp_amount 					 0.01f
#define Ki_amount 					 0.01f
#define Kd_amount 					 0.01f

/* Receive Buffer Size -------------------------------------------------------*/
#define RECEIVE_BUFF_SIZE				22U
#define ID_PID_MESS						1U
#define ID_STATUS_MESS					2U
#define ID_FIRST_LAST_MESS				3U
#define DATA_ERROR_REQ					4U
#define DATA_LEFT_REQ					5U
#define DATA_RIGHT_REQ					6U
#define DATA_PID_REQ					7U
#define DATA_BOOT_REQ					8U
#define DATA_PATH_REQ					9U
#define DATA_POINT_REQ					10U
#define DATA_RFID_REQ					11U

/* Car State -----------------------------------------------------------------*/
#define CAR_IS_RUNNING					1U
#define CAR_IS_PAUSE					0U

/* Transfer data flag --------------------------------------------------------*/
#define DATA_TRANS_DIS 					0U
#define DATA_TRANS_EN 					1U

/* Transfer PID DATA ---------------------------------------------------------*/
#define Send_Buffer_Size 				22
#define Kp_Buffer_Size 					7
#define Ki_Buffer_Size 					7
#define Kd_Buffer_Size 					7
/* Sensor position -----------------------------------------------------------*/

	/*		0			1			2			3			4			5		*/

/* Max Speed Constrains ------------------------------------------------------*/
#define MAXSPEED_RIGHT					3300
#define MAXSPEED_LEFT					3490

/* Default PID ---------------------------------------------------------------*/
#define DEFAULT_KP						36.42f
#define DEFAULT_KI						0.0f
#define DEFAULT_KD						2.3f

/* Line Color ----------------------------------------------------------------*/
#define LINE_HALF_BLACK					1U
#define LINE_FULL_BLACK					2U
#define LINE_MIDDLE						3U


/* Instruction define --------------------------------------------------------*/
#define TURN_SKIP_POINT					7U
#define TURN_SKIP 						0U
#define TURN_180DEG_STRAIGHT			1U
#define TURN_180DEG_RIGHT				2U
#define TURN_RIGHT						3U
#define TURN_LEFT						4U
#define TURN_180DEG_LEFT				5U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define sbi(Reg, Bit) (Reg |= (1<<Bit))					//set bit macro
#define cbi(Reg, Bit) (Reg &= ~(1<<Bit))				//clear bit macro
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

uint8_t LineDetect = 0;
volatile uint8_t stateBTNC = 1;
uint8_t cancel_menu = 1;
uint8_t cancel_running = 1;
uint8_t Kp_modify_flag = 0, Ki_modify_flag = 0, Kd_modify_flag = 0;
uint8_t Left_modify_flag = 0, Right_modify_flag = 0;
uint8_t First_point_modify_flag = 0, Last_point_modify_flag = 0;
uint8_t Status = CAR_IS_RUNNING;
uint8_t Trans_flag = DATA_TRANS_DIS;
uint8_t LCD_INIT_FLAG = 1;
uint8_t Previous_Line = LINE_MIDDLE;
uint8_t Flag_u8 = 0;
int8_t First_point = 0,Last_point = 0,Previous_point = 7;
int8_t Begin_solving = 0;
uint8_t count;
uint8_t STATION_ID = 0;
uint8_t cancel_reading_card = 1;
uint8_t RFID_ID_ready = 0;
int16_t Left = 3390, Right = 3200;
int16_t positionLeft = 0, positionRight = 0;
uint16_t Sensor_Threshold[] = { 3900, 3900, 3900, 4000, 4000, 4000 };
uint16_t Sensor_ADC_Value[6];
uint32_t counterLeft = 0,counterRight = 0;

volatile short countLeft = 0, countRight = 0;
volatile short countLeft_prv = 0, countRight_prv = 0;
volatile short rateLeft = 0, rateRight = 0;
volatile short rateLeft_prv = 0, rateRight_prv = 0;


const long interval = 70;
uint8_t ID;
int Motor_Speed_L,Motor_Speed_R;
int Speed_Left = 0, Speed_Right = 0;
int Check_line = 0;
int Error_Val;
int old_countLeft,old_countRight;
float Kp = DEFAULT_KP, Ki = DEFAULT_KI, Kd = DEFAULT_KD;
char string_2[1];
char PID_Rx[12];
char kp_val[10], ki_val[10], kd_val[10];
char Rx_Buffer[RECEIVE_BUFF_SIZE],Rx_Buffer_copied[RECEIVE_BUFF_SIZE];
char RFID_Str[20] = "2398801a21";
unsigned long previousMillis = 0;
unsigned long time;
unsigned char CardID[5];
PIDController Car = {0,0,0,0,-400,400};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM5_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */
static void SelectItem(void);
static void Sensor_Convert_A2D(void);
static void Sensor_Print_LineDetect();
static void MultifunctionButton(void);
static void ScrollUp(void);
static int Error_Return (uint8_t Sensor_Array);
static int Constraint (int Present_Value,int Min,int Max);
static void Go_Straight();
static void Left_Turn();
static void Right_Turn();
static void Turn_180_Deg();

// RC522
uint8_t MFRC522_Check(uint8_t* id);
uint8_t MFRC522_Compare(uint8_t* CardID, uint8_t* CompareID);
void MFRC522_WriteRegister(uint8_t addr, uint8_t val);
uint8_t MFRC522_ReadRegister(uint8_t addr);
void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t* TagType);
uint8_t MFRC522_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen);
uint8_t MFRC522_Anticoll(uint8_t* serNum);
void MFRC522_CalulateCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData);
uint8_t MFRC522_SelectTag(uint8_t* serNum);
uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum);
uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t* recvData);
uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t* writeData);
void MFRC522_Init(void);
void MFRC522_Reset(void);
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
void MFRC522_Halt(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART2 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, 0xFFFF);
	return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	strcpy(Rx_Buffer_copied,Rx_Buffer);
	char *ID_number  = strtok(Rx_Buffer_copied," ");
	ID = strtod(ID_number,NULL);
	if(ID == ID_PID_MESS)
	{
		char *Data1 = strtok(NULL," ");
		char *Data2 = strtok(NULL," ");
		char *Data3 = strtok(NULL,"");
		Kp = strtof(Data1,NULL);
		Ki = strtof(Data2,NULL);
		Kd = strtof(Data3,NULL);
	}
	else if(ID == ID_STATUS_MESS)
	{
		char *Data1 = strtok(NULL," ");
		Status = (uint8_t)atoi(Data1);
	}
	else if(ID == ID_FIRST_LAST_MESS)
	{
		char *Data1 = strtok(NULL," ");
		char *Data2 = strtok(NULL," ");
		First_point = atoi(Data1);
		Last_point = atoi(Data2);
		Begin_solving = 1;
	}
	else if(ID == DATA_ERROR_REQ && Trans_flag == DATA_TRANS_EN)
	{
		char Error_Str[4];
		sprintf(Error_Str,"%d",Error_Val);
		HAL_UART_Transmit(&huart6, Error_Str, sizeof(Error_Str), 5000);
	}
	else if(ID == DATA_LEFT_REQ && Trans_flag == DATA_TRANS_EN)
	{
		char Speed_Left_Str[5];
		sprintf(Speed_Left_Str,"%d",rateLeft);
		HAL_UART_Transmit(&huart6, Speed_Left_Str, sizeof(Speed_Left_Str),5000);
	}
	else if(ID == DATA_RIGHT_REQ && Trans_flag == DATA_TRANS_EN)
	{
		char Speed_Right_Str[5];
		sprintf(Speed_Right_Str,"%d",rateRight);
		HAL_UART_Transmit(&huart6, Speed_Right_Str, sizeof(Speed_Right_Str), 5000);
	}
	else if(ID == DATA_PID_REQ)
	{
	  char Kp_string[Kp_Buffer_Size+1],Ki_string[Ki_Buffer_Size+1],Kd_string[Kd_Buffer_Size+1],Final_string[Send_Buffer_Size];
	  memset(Final_string,0,sizeof(Final_string));
	  sprintf(Kp_string,"%06.2f ",Kp);
	  sprintf(Ki_string,"%06.2f ",Ki);
	  sprintf(Kd_string,"%06.2f\"",Kd);
	  strcat(Final_string,Kp_string);
	  strcat(Final_string,Ki_string);
	  strcat(Final_string,Kd_string);
	  HAL_UART_Transmit(&huart6, Final_string, sizeof(Final_string), 10000);
	}
	else if(ID == DATA_BOOT_REQ)
	{
		char Boot[1] = "1";
		HAL_UART_Transmit(&huart6, Boot, sizeof(Boot), 1000);
	}
	else if(ID == DATA_PATH_REQ)
	{
		char PATH_DIR[15];
		memset(PATH_DIR,0,sizeof(PATH_DIR));
		GetString_Transfer(First_point, Last_point, PATH_DIR);
		HAL_UART_Transmit(&huart6, PATH_DIR, sizeof(PATH_DIR), 5000);
	}
	else if(ID == DATA_POINT_REQ)
	{
		char STATION_ID_str[4];
		sprintf(STATION_ID_str,"%d",STATION_ID);
		HAL_UART_Transmit(&huart6, STATION_ID_str, sizeof(STATION_ID_str), 2000);
	}
	else if(ID == DATA_RFID_REQ)
	{
		if(RFID_Str == "d1b0af21ef")
		{
			strcpy(RFID_Str,"83ee851af2");
		}
		HAL_UART_Transmit(&huart6, RFID_Str,sizeof(RFID_Str),2000);
	}
	memset(Rx_Buffer_copied,0,sizeof(Rx_Buffer_copied));
	memset(Rx_Buffer,0,sizeof(Rx_Buffer));
	HAL_UART_Receive_IT(&huart6, Rx_Buffer, RECEIVE_BUFF_SIZE);

}

void Encoder_Read()
{
	countLeft = __HAL_TIM_GET_COUNTER(&htim2);
	countRight = __HAL_TIM_GET_COUNTER(&htim4);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART6_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_I2C3_Init();
  MX_TIM5_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();
  MotorL_EnablePWM();
  MotorR_EnablePWM();
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &Sensor_ADC_Value, 6);
  HAL_UART_Receive_IT(&huart6, Rx_Buffer, RECEIVE_BUFF_SIZE);
  /*Enable for encoder reading*/
  HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
  PIDController_Car_Init(&Car);
  MFRC522_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if(LCD_INIT_FLAG == 1)
		{
			for(int i = 0; i < 3; i++)
			{
				lcd_init();
				HAL_Delay(500);
				lcd_send_cmd(0);
			}
			LCD_INIT_FLAG = 0;
		}
		lcd_send_cmd(1);
		while (menu_display)
		{
			Car.Kp = Kp;
			Car.Kd = Kd;
			Menu_system_control(Menu_type, line);
			ScrollUp();
			SelectItem();
		}
//		MotorL_SetPWM(3000);
//		MotorR_SetPWM(3000);
//		printf("%d,%d\n",Speed_Left,Speed_Right);
//		HAL_Delay(100);
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 6;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 7199;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 60;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 59999;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 71;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 9999;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : ButtonC_Pin */
  GPIO_InitStruct.Pin = ButtonC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ButtonC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ButtonB_Pin ButtonA_Pin */
  GPIO_InitStruct.Pin = ButtonB_Pin|ButtonA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB10 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void Running(void) // Activate the car for running
{
	uint8_t Temp_Line;
	int8_t point_passed = -1;
	int Instruction[8];
	int Map_Point[4],Turn_Instruction[4];
	int8_t Max_point_temp,Max_point;
	int8_t Spin_once = 0;
	int8_t Path_guidance = 1;
	int8_t station_count = -1;
	uint8_t Read_RFID_Flag = 0;
	lcd_send_cmd(0x80 | 0x00);
	lcd_send_string("Car is Running!        ");
	lcd_send_cmd(0x80 | 0x40);
	lcd_send_string("Press C for cancel     ");
	Previous_Line = LINE_MIDDLE;
	while (cancel_running) {
		if(Begin_solving == 1)
		{
	/*======================== CACULATING SPEED EACH 300ms ============================================*/
			unsigned long currentMillis  = HAL_GetTick();
			Encoder_Read();
			if(currentMillis  - previousMillis  >= interval)
			{
				previousMillis  = previousMillis;
				rateLeft = (countLeft - countLeft_prv)*1345/374;
				rateRight = (countRight - countRight_prv)*1345/374;
				countLeft_prv = countLeft;
				countRight_prv = countRight;
			}
			if(Path_guidance == 1)
			{
				Max_point_temp = Solver(First_point, Last_point, Previous_point, Turn_Instruction, Map_Point, Instruction);
				Max_point = Max_point_temp + (Max_point_temp - 1) - 1;
				Path_guidance = 0;
			}
			Sensor_Convert_A2D();
			Go_Straight();
			Temp_Line = (LineDetect & 0b10000100);
			if(Temp_Line == 0b10000000 || Temp_Line == 0b00000100 || Temp_Line == 0b10000100)
			{
				if(Temp_Line == 0b10000000)
				{
					Error_Val = 3100;
				}
				else if(Temp_Line == 0b00000100)
				{
					Error_Val = 4300;
				}
				Sensor_Convert_A2D();
				Previous_Line = LINE_HALF_BLACK;
				if(point_passed == Max_point - 1)
				{
					station_count = station_count + 1;
					STATION_ID = Map_Point[station_count];
					First_point = Last_point;
					Previous_point = Map_Point[Max_point_temp - 2];
					Begin_solving = 0;
					Read_RFID_Flag = 1;
				}
				if(Spin_once == 0 && (Turn_Instruction[0] == TURN_180DEG_LEFT || Turn_Instruction[0] == TURN_180DEG_RIGHT || Turn_Instruction[0] == TURN_180DEG_STRAIGHT))
				{
					Turn_180_Deg();
					Previous_Line = LINE_HALF_BLACK;
					Spin_once = 1;
				}
			}

		/* Truong hop xe chay line giua ---------------------------------------*/
			else if (Temp_Line == 0b00000000)
			{
				if (Previous_Line == LINE_HALF_BLACK)
				{
					Sensor_Convert_A2D();
					Go_Straight();
					point_passed = point_passed + 1;

/*Step procesing BEGIN here */
					if(point_passed < Max_point)
					{
						switch(Instruction[point_passed])
						{
						case TURN_SKIP_POINT:
							station_count = station_count + 1;
							STATION_ID = Map_Point[station_count];
							Go_Straight();
							break;
						case TURN_LEFT:
							Left_Turn();
							break;
						case TURN_RIGHT:
							Right_Turn();
							break;
						case TURN_SKIP:
							Go_Straight();
							break;
						case TURN_180DEG_LEFT:
							Left_Turn();
							break;
						case TURN_180DEG_RIGHT:
							Right_Turn();
							break;
						case TURN_180DEG_STRAIGHT:
							Go_Straight();
							break;
;						}
					}

/*Step procesing END here */
					Previous_Line = LINE_MIDDLE;
				}

				else if(Previous_Line == LINE_MIDDLE)
				{
				Sensor_Convert_A2D();
				Error_Val = Error_Return(LineDetect);
				int16_t PID_Val = Line_Follower_PID(3500,Error_Val,&Car);
				Motor_Speed_R = (Right + PID_Val);
				Motor_Speed_L = (Left - PID_Val);
				Motor_Speed_R = Constraint(Motor_Speed_R, -MAXSPEED_RIGHT,MAXSPEED_RIGHT);
				Motor_Speed_L = Constraint(Motor_Speed_L, -MAXSPEED_LEFT,MAXSPEED_LEFT);
				MotorR_SetPWM(Motor_Speed_R);
				MotorL_SetPWM(Motor_Speed_L);
				}
			}

			if(Status == 0)
			{
				Trans_flag = DATA_TRANS_DIS;
				lcd_send_cmd(0x80 | 0x00);
				lcd_send_string("Car is Pause        ");
			}
			else
			{
				Trans_flag = DATA_TRANS_EN;
				lcd_send_cmd(0x80 | 0x00);
				lcd_send_string("Car is Running!        ");
			}
		}
		else if(Begin_solving == 0)
		{
			MotorL_SetPWM(0);
			MotorR_SetPWM(0);
		/*Reset value after done solving the matrix*/
			memset(Instruction,0,sizeof(Instruction));
			memset(Map_Point,0,sizeof(Map_Point));
			memset(Turn_Instruction,0,sizeof(Turn_Instruction));
			point_passed = -1;
			Max_point = 0;
			Max_point_temp = 0;
			Temp_Line = 0;
			Spin_once = 0;
			Path_guidance = 1;
			Max_point = 0;
			station_count = -1;
			if(Read_RFID_Flag == 1){
				Read_RFID_CARD();
				Read_RFID_Flag = 0;
			}
		}
	}
	MotorL_SetPWM(0);
	MotorR_SetPWM(0);
	lcd_clear();
}
void Read_RFID_CARD()
{
	uint8_t recover_position = 0;
	uint8_t enable = 1;
	char Card_Buffer_Str[4];
	char concat_Buff[20];
	memset(concat_Buff,0,sizeof(concat_Buff));
	memset(Card_Buffer_Str,0,sizeof(Card_Buffer_Str));
	MotorL_SetPWM(2500);
	MotorR_SetPWM(2500);
	MFRC522_Init();
	while(enable)
	{
		if (MFRC522_Check(CardID) == MI_OK)
		{
			MotorL_SetPWM(0);
			MotorR_SetPWM(0);
			sprintf(Card_Buffer_Str,"\"%02x",CardID[0]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x",CardID[1]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x",CardID[2]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x",CardID[3]);
			strcat(concat_Buff,Card_Buffer_Str);
			sprintf(Card_Buffer_Str,"%02x\"",CardID[4]);
			strcat(concat_Buff,Card_Buffer_Str);
			strcpy(RFID_Str,concat_Buff);
			RFID_ID_ready = 1;
			enable = 0;
			recover_position = 1;
			MFRC522_Reset();
		}
	}
	while(recover_position)
	{
		Sensor_Convert_A2D();
		MotorL_SetPWM(-2500);
		MotorR_SetPWM(-2500);
		if((LineDetect & 0b10000100) == 0b10000000 || (LineDetect & 0b10000100) == 0b00000100 || (LineDetect & 0b10000100) == 0b10000100)
		{
			HAL_Delay(250);
			recover_position = 0;
		}
	}
	MotorL_SetPWM(0);
	MotorR_SetPWM(0);
}

static void Left_Turn()
{
	MotorR_SetPWM(4900);
	MotorL_SetPWM(-3000);
	HAL_Delay(160);
	while((LineDetect & 0b11000000) <= 64)
	{
		Sensor_Convert_A2D();
	}
	while(LineDetect != 0b00010000)
	{
		Sensor_Convert_A2D();
	}
}
static void Right_Turn()
{
	MotorL_SetPWM(5400);
	MotorR_SetPWM(-3900);
	while((LineDetect & 0b00001100) <= 8)
	{
		Sensor_Convert_A2D();
	}
	while(LineDetect != 0b00010000)
	{
		Sensor_Convert_A2D();
	}
}
static void Turn_180_Deg()
{
	MotorL_SetPWM(6200);
	MotorR_SetPWM(-6200);
	while(LineDetect != 0b00000000)
	{
		Sensor_Convert_A2D();
	}
	while(LineDetect <= 3)
	{
		Sensor_Convert_A2D();
	}
	while(LineDetect != 0b00010000)
	{
		Sensor_Convert_A2D();
	}
}
static void Go_Straight()
{
	int16_t PID_Val = Line_Follower_PID(3500,3500,&Car);
	Motor_Speed_R = (Right + PID_Val);
	Motor_Speed_L = (Left - PID_Val);
	Motor_Speed_R = Constraint(Motor_Speed_R, -MAXSPEED_RIGHT,MAXSPEED_RIGHT);
	Motor_Speed_L = Constraint(Motor_Speed_L, -MAXSPEED_LEFT,MAXSPEED_LEFT);
	MotorR_SetPWM(Motor_Speed_R);
	MotorL_SetPWM(Motor_Speed_L);
}
static int Error_Return (uint8_t Sensor_Array){
	switch(Sensor_Array){
	case 0b00001000:
		return 4100;
		break;
	case 0b00011000:
		return 3900;
		break;

	case 0b00010000:
		return 3700;
		break;

	case 0b00000000:
		return 3500;
		break;

	case 0b00110000:
		return 3500;
		break;

	case 0b00100000:
		return 3300;
		break;

	case 0b01100000:
		return 3100;
		break;
	case 0b01000000:
		return 2900;
		break;
/*---------------------dont care------------------------*/
	default:
		return 3500;
		break;
	}
}
static int Constraint (int Present_Value,int Min,int Max){
	if(Present_Value > Max){
		return Present_Value = Max;
	}
	else if (Present_Value < Min ){
		return Present_Value = Min;
	}
	else
		return Present_Value;
}
static void Sensor_Convert_A2D()
{
	LineDetect = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (Sensor_ADC_Value[i] < Sensor_Threshold[i])
		{
			sbi(LineDetect, (7 - i));
//			printf("1 ");
		}
		else
		{
//			printf("0 ");
		}
	}
//	printf("\n");
//	HAL_Delay(100);
}
void Sensor_PrintValue(void)
{
	printf("Sensor Val: ");
	for (int i = 0; i < 6; ++i)
	{
		printf("%u ", Sensor_ADC_Value[i]);
	}
	printf("\n");

}
static void Sensor_Print_LineDetect()
{
		char buffer[6];
		itoa (LineDetect,buffer,2);
		printf ("binary: %s\n",buffer);
}
void ScrollUp(void)
{
	if (HAL_GPIO_ReadPin(ButtonA_GPIO_Port, ButtonA_Pin) == 0)
	{
		while (HAL_GPIO_ReadPin(ButtonA_GPIO_Port, ButtonA_Pin) == 0)
		{
			HAL_Delay(50);
		}
		switch (Menu_type)
		{
		case Main_menu:
			line--;
			if (line < Number_of_Menu_firstline)
			{
				line = Maximum_Menu_line;
			}
			break;
		case PID_Menu:
			line--;
			if (line < Number_of_Menu_firstline)
			{
				line = Maximum_PID_line;
			}
			if (Kp_modify_flag == 1)
			{
				Kp += Kp_amount;
				line = 1;
			}
			if (Ki_modify_flag == 1)
			{
				Ki += Ki_amount;
				line = 2;
			}
			if (Kd_modify_flag == 1)
			{
				Kd += Kd_amount;
				line = 3;
			}
			break;
		case Engine_menu:
			line--;
			if (line < Number_of_Menu_firstline)
			{
				line = Maximum_Engine_line;
			}
			if (Left_modify_flag == 1)
			{
				Left += 100;
				line = 1;
				if (Left >= 7200)
				{
					Left = 7200;
				}
			}
			if (Right_modify_flag == 1)
			{
				Right += 100;
				line = 2;
				if (Right >= 7200)
				{
					Right = 7200;
				}
			}
			break;
		case Path_solver:
			line--;
			if (line < Number_of_Menu_firstline)
			{
				line = Maximum_Path_Solver_line;
			}
			if (First_point_modify_flag == 1)
			{
				First_point += 1;
				line = 1;
				if(First_point>11)
				{
					First_point = 0;
				}
			}
			if (Last_point_modify_flag == 1)
			{
				Last_point += 1;
				line = 2;
				if(Last_point>11)
				{
					Last_point = 0;
				}
			}
			break;
		}
	}
}
void SelectItem(void)
{
	if (HAL_GPIO_ReadPin(ButtonB_GPIO_Port, ButtonB_Pin) == 0) //Select button
	{
		while (HAL_GPIO_ReadPin(ButtonB_GPIO_Port, ButtonB_Pin) == 0)
		{
			HAL_Delay(50);
		}
		executeAction(line);
		if (Kp_modify_flag == 1 || Ki_modify_flag == 1 || Kd_modify_flag == 1
				|| Right_modify_flag == 1 || Left_modify_flag == 1 || First_point_modify_flag == 1 || Last_point_modify_flag == 1)
		{
			__NOP();
		}
		else
		{
			line = 1;
		}
	}
}
void MultifunctionButton(void)
{
	switch (Menu_type)
	{
	case Running_menu:
		Menu_type = Main_menu;
		line = 1;
		cancel_running = 0;
		break;
	case Main_menu:
		line++;
		if (line > Maximum_Menu_line)
		{
			line = Number_of_Menu_firstline;
		}
		break;
	case PID_Menu:
		line++;
		if (line > Maximum_PID_line)
		{
			line = Number_of_Menu_firstline;
		}
		if (Kp_modify_flag == 1)
		{
			Kp -= Kp_amount;
			line = 1;
			if (Kp <= 0)
				{Kp = 0;}
		}
		if (Ki_modify_flag == 1)
		{
			Ki -= Ki_amount;
			line = 2;
			if (Ki <= 0)
				{Ki = 0;}
		}
		if (Kd_modify_flag == 1)
		{
			Kd -= Kd_amount;
			line = 3;
			if (Kd <= 0)
				{Kd = 0;}
		}
		break;
	case Engine_menu:
		line++;
		if (line > Maximum_Engine_line)
		{
			line = Number_of_Menu_firstline;
		}
		if (Left_modify_flag == 1)
		{
			Left -= 100;
			line = 1;
			if (Left <= -7200)
				{Left = -7200;}
		}
		if (Right_modify_flag == 1)
		{
			Right -= 100;
			line = 2;
			if (Right <= -7200)
				{Right = -7200;}
		}
		break;
	case LineDetect_Show:
		Menu_type = Main_menu;
		line = 1;
		cancel_menu = 0;
		break;
	case Saving_process:
		cancel_reading_card = 0;
		Menu_type = Main_menu;
		line = 1;
		break;
	case Color_Processing:
		Color_Read = 0;
		break;
	case Path_solver:
		line++;
		if (line > Maximum_Path_Solver_line)
		{
			line = Number_of_Menu_firstline;
		}
		if (First_point_modify_flag == 1)
		{
			First_point -= 1;
			line = 1;
			if(First_point < 0)
			{
				First_point = 11;
			}
		}
		if (Last_point_modify_flag == 1)
		{
			Last_point -= 1;
			line = 2;
			if(Last_point < 0)
			{
				Last_point = 11;
			}
		}
		break;
		case Path_show_menu:
			Menu_type = Path_solver;
			line = 1;
			break;
	}

}

//Deboucing button program
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ButtonC_Pin && stateBTNC == 1)
	{
		HAL_TIM_Base_Start_IT(&htim5);
		stateBTNC = 0;
	}

	else
	{
		__NOP();
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim5.Instance)
	{
		if (HAL_GPIO_ReadPin(ButtonC_GPIO_Port, ButtonC_Pin)
				== GPIO_PIN_RESET)
		{
			MultifunctionButton();
			menu_display = 1;
			stateBTNC = 1;
			HAL_TIM_Base_Stop_IT(&htim5);

		}
	}
}
//void ReadFlash(void)
//{
//	Flash_Read_Data(0x08020000, PID_Rx);
//	Convert_To_Str(PID_Rx, string_2);
//	char *KpinString = strtok(string_2," ");
//	char *KiinString = strtok(NULL," ");
//	char *KdinString = strtok(NULL," ");
//	Kp = strtod(KpinString, NULL);
//	Ki = strtod(KiinString, NULL);
//	Kd = strtod(KdinString, NULL);
//
//}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	char *KpinString = strtok(Rx_Buff," ");
//	char *KiinString = strtok(NULL," ");
//	char *KdinString = strtok(NULL," ");
//	Kp = strtod(KpinString, NULL);
//	Ki = strtod(KiinString, NULL);
//	Kd = strtod(KdinString, NULL);
//
//	HAL_UART_Receive_IT(&huart6, Rx_Buff, sizeof(Rx_Buff));
//	if(Kp != 0 && Ki != 0 && Kd != 0)
//	{
//		Wifi_Connecting = 0;
//
//	}
//
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

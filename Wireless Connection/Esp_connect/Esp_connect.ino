/*  Library include -----------------------------------------------*/
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>
#include <SoftwareSerial.h>
#include <string.h>

/*  Private definition --------------------------------------------*/
#define Send_Buffer_Size 22
#define Kp_Buffer_Size 7
#define Ki_Buffer_Size 7
#define Kd_Buffer_Size 6
#define ID_Buffer_Size 2
#define First_string_Size   3
#define Last_string_Size    3    

/*  Number of message ---------------------------------------------*/
#define REQ_STM_ERROR       0U
#define REQ_STM_LEFT        1U
#define REQ_STM_RIGHT       2U
#define REQ_STM_PID         3U
/*	Flags ---------------------------------------------------------*/
#define	DATA_SEND_REQ_DIS		0U
#define DATA_SEND_REQ_EN		1U
#define DATA_SEND_PID_DIS		0U
#define DATA_SEND_PID_EN		1U
/*  Formula for Buffer --------------------------------------------*/
// Cach tinh Send_Buffer_Size = ID_Buffer_Size + Kp_Buffer_Size + Ki_Buffer_Size + Kd_Buffer_Size - 4
// Cach tinh Buffer cho tung Kp,Ki,Kd => size thuc + gia tri \0 (+1)

/*  Message ID ----------------------------------------------------*/
#define PID_Message_ID 1
#define Car_State_Message_ID 2
#define First_Last_Message_ID 3

/*  STM32 Bootup --------------------------------------------------*/
#define STM32_BOOTUP        1U
#define STM32_SHUTDOWN      0U

/*  PID Bootup ----------------------------------------------------*/
#define PID_BOOTUP          1U
#define PID_SHUTDOWN        0U
/*  Library variable declaration ----------------------------------*/
SocketIoClient webSocket;
SoftwareSerial ss(14,5);

/*  Private variable declaration ----------------------------------*/
const char* Host_Socket1 = "192.168.1.31";
const char* Host_Socket2 = "192.168.1.234";
unsigned int Port_Socket = 3000;
float P,I,D;
uint8_t State,First,Last;
String Final_string_2;
uint8_t Number_of_data = REQ_STM_ERROR;
/*  Setup ---------------------------------------------------------*/
int beginTime = 0;
int endTime = 0;
uint8_t Read_Flag = DATA_SEND_REQ_DIS;
uint8_t Read_PID = DATA_SEND_PID_EN;
uint8_t Boot = STM32_SHUTDOWN;
uint8_t PID_Boot = STM32_SHUTDOWN;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ss.begin(9600);
  WiFi.begin("Nguyen Duc Tam ", "ductammdf1059");
  while(WiFi.status() != WL_CONNECTED)
  {
  delay(10);
  }
   webSocket.on("PID_value",PID_handle);
   webSocket.on("Car_State",Car_State_handle);
   webSocket.on("First_Last",Matrix_handle);
   webSocket.begin(Host_Socket1, Port_Socket, "/socket.io/?transport=websocket");
   pinMode(LED_BUILTIN, HIGH);
}

void loop() {
//   put your main code here, to run repeatedly
/*--------------------------------------------------------------- Request for STM32 Bootup signal when power up ---------------------------------*/
  if(Boot == STM32_SHUTDOWN)
  {
    if (endTime - beginTime > 1) {
      String data = "";
/*  Request for BOOTUP State from STM32 ------------------------------------------*/
      Send_Request_ID(8);
      beginTime = millis();
    }
    
    if (ss.available()>0){
      String data = "\"";
      int beginTimeSS, endTimeSS = millis();
      while (ss.available()>0){
        char temp = ss.read();
        data = data + temp;
        delay(10);
      }
      data = data + "\"";
      int len=1;
      while (data[len] != NULL){
        len++;
      }
      char str[len+1];
      data.toCharArray(str, len+1);
      webSocket.emit("STM32_BOOTUP", str); 
      /*  Send STM32 bootup state -----------------------------------------*/
      Boot = STM32_BOOTUP;
      PID_Boot = PID_BOOTUP;
    }
  }
/*-----------------------------------------------------------------  Request for PID parameter when bootup ---------------------------------*/
  if(PID_Boot == PID_BOOTUP)
  {
    if (endTime - beginTime > 5) {
      String data = "";
/*  Request for BOOTUP State from STM32 ------------------------------------------*/
      Send_Request_ID(7);
      beginTime = millis();
    }
    
    if (ss.available()>0){
      String data = "\"";
      int beginTimeSS, endTimeSS = millis();
      while (ss.available()>0){
        char temp = ss.read();
        data = data + temp;
        delay(10);
      }
      int len=1;
      while (data[len] != NULL){
        len++;
      }
      char str[len+1];
      data.toCharArray(str, len+1);
      webSocket.emit("PID_param", str);
      /*  Send STM32 bootup state -----------------------------------------*/
      Serial.print(data);
      PID_Boot = PID_SHUTDOWN;
      Read_Flag = DATA_SEND_REQ_EN;
      delay(15);
    }
  }

/*-----------------------------------------------------------------  Request for Information during run time -------------------------------*/
  if(Read_Flag == DATA_SEND_REQ_EN){
/*	Data being read every 50ms --------------------------------------------*/
    if (endTime - beginTime > 100) {
      String data = "";
      switch(Number_of_data){
/*  Request for ERROR from STM32 ------------------------------------------*/
        case REQ_STM_ERROR:
          Send_Request_ID(4);
          break;
/*  Requet for LEFT SPEED from STM32 --------------------------------------*/
        case REQ_STM_LEFT:
          Send_Request_ID(5);
          break;
/*  Request for RIGHT SPEED from STM32 ------------------------------------*/
        case REQ_STM_RIGHT:
          Send_Request_ID(6);
          break;
      }
      beginTime = millis();
    }
	
/*	Read DATA from STM32 --------------------------------------------------*/
    if (ss.available()>0)
    {
      String data = "";
      int beginTimeSS, endTimeSS = millis();
      while (ss.available()>0){
        char temp = ss.read();
        data = data + temp;
        delay(10);
      }
      int len=1;
      while (data[len] != NULL){
        len++;
      }
      char str[len+1];
      data.toCharArray(str, len+1);
      Serial.print(str);
      switch(Number_of_data){
/*  Read ERROR from STM32 ------------------------------------------------*/
        case REQ_STM_ERROR:
          webSocket.emit("Error_number", str);
          Number_of_data = REQ_STM_LEFT;
          break;
/*  Read LEFT SPEED from STM32 -------------------------------------------*/ 
        case REQ_STM_LEFT:
          webSocket.emit("Left_Eng", str);
          Number_of_data = REQ_STM_RIGHT;
          break;
/*  Read RIGHT SPEED from STM32 ------------------------------------------*/
        case REQ_STM_RIGHT:
          webSocket.emit("Right_Eng", str);
          Number_of_data = REQ_STM_ERROR;
          break;
      }
    }
  }
  endTime = millis();
  webSocket.loop();
}

/* Handle =======================================================*/
void PID_handle(const char* payload,size_t length){           //=
  pareJson_Data_PID(payload);                                 //=
}                                                             //=
void Car_State_handle(const char* payload,size_t length){     //=
  pareJson_Data_Car_State(payload);                           //=
}                                                             //=
void Matrix_handle(const char* payload,size_t length){        //=
  pareJson_Data_Matrix(payload);                              //=
}                                                             //=
                                                              //=
/*===============================================================*/

/*  Json handle =================================================*/
void pareJson_Data_PID(String strJson)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(strJson);
  if(!root.success()){
    return;
  }
  else{
    P = (root["P"].as<float>());
    I = (root["I"].as<float>());
    D = (root["D"].as<float>());
    PID_String_compression(P,I,D);
  }
}

void pareJson_Data_Car_State(String strJson)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(strJson);
  if(!root.success()){
    return;
  }
  else{
    State = (root["Car_state"].as<int>());
    State_String_compression(State);
  }
}

void pareJson_Data_Matrix(String strJson)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(strJson);
  if(!root.success()){
    return;
  }
  else{
    First = (root["First"].as<int>());
    Last = (root["Last"].as<int>());
    Matrix_String_compression(First,Last);
  }
}
/*=================================================================*/

/*  Compression handle ============================================*/
void PID_String_compression(float kp,float ki,float kd){
  char ID_string[ID_Buffer_Size+1],Kp_string[Kp_Buffer_Size+1],Ki_string[Ki_Buffer_Size+1],Kd_string[Kd_Buffer_Size+1],Final_string[Send_Buffer_Size];
  memset(Final_string,0,sizeof(Final_string));
  sprintf(ID_string,"%d ",PID_Message_ID);
  sprintf(Kp_string,"%06.2f ",kp);
  sprintf(Ki_string,"%06.2f ",ki);
  sprintf(Kd_string,"%06.2f",kd);

  for(int i = 0;i < ID_Buffer_Size ;i++)
  {
    Final_string[i] = ID_string[i];
  }
  for(int i = 0;i < Kp_Buffer_Size ;i++)
  {
    Final_string[i + ID_Buffer_Size] = Kp_string[i];
  }
  
  for(int i = 0;i < Ki_Buffer_Size ;i++)
  {
    Final_string[i + Kp_Buffer_Size + ID_Buffer_Size] = Ki_string[i];
  }
  
  for(int i = 0;i < Kd_Buffer_Size ;i++)
  {
    Final_string[i + Ki_Buffer_Size + Kp_Buffer_Size + ID_Buffer_Size] = Kd_string[i];
  }
  ss.write(Final_string,Send_Buffer_Size);
  
}
void State_String_compression(int State){
  char ID_string[ID_Buffer_Size+1],State_string[ID_Buffer_Size+1],Final_string[Send_Buffer_Size];
  memset(Final_string,0,sizeof(Final_string));
  sprintf(ID_string,"%d ",Car_State_Message_ID);
  sprintf(State_string,"%d ",State);
  for(int i = 0;i < ID_Buffer_Size ;i++)
  {
    Final_string[i] = ID_string[i]; 
  }
  for(int i = 0;i < ID_Buffer_Size ;i++)
  {
    Final_string[i + ID_Buffer_Size] = State_string[i];
  }
  for(int i = 0;i < (Send_Buffer_Size - ID_Buffer_Size - ID_Buffer_Size); i++)
  {
    Final_string[i + ID_Buffer_Size + ID_Buffer_Size] = '0';
  }
  Read_Flag = State;
  ss.write(Final_string,Send_Buffer_Size);
}
void Matrix_String_compression(int First,int Last){
  char ID_string[ID_Buffer_Size+1],First_string[First_string_Size+1],Last_string[Last_string_Size+1],Final_string[Send_Buffer_Size];
  memset(Final_string,0,sizeof(Final_string));
  sprintf(ID_string,"%d ",First_Last_Message_ID);
  sprintf(First_string,"%2d ",First);
  sprintf(Last_string,"%2d ",Last);
  for(int i = 0;i < ID_Buffer_Size; i++)
  {
     Final_string[i] = ID_string[i];   
  }
  for(int i = 0;i < First_string_Size; i++)
  {
     Final_string[i + ID_Buffer_Size] = First_string[i];
  }
  for(int i = 0;i < Last_string_Size; i++)
  {
     Final_string[i + ID_Buffer_Size + First_string_Size] = Last_string[i];
  }
  for(int i = 0;i < (Send_Buffer_Size - ID_Buffer_Size - First_string_Size - Last_string_Size); i++)
  {
     Final_string[i + ID_Buffer_Size + First_string_Size + Last_string_Size] = '0';
  }
  ss.write(Final_string,Send_Buffer_Size);
}
void Send_Request_ID (uint8_t ID)
{
    char string[22];
    string[0] = ID + '0';
    string[1] = ' ';
    for(int i=0; i<20; i++)
    {
      string[i+2] = '0';
    }
    ss.write(string,22);
    delay(20);
}

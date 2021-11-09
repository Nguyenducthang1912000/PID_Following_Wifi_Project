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
/*  Formula for Buffer --------------------------------------------*/
// Cach tinh Send_Buffer_Size = ID_Buffer_Size + Kp_Buffer_Size + Ki_Buffer_Size + Kd_Buffer_Size - 4
// Cach tinh Buffer cho tung Kp,Ki,Kd => size thuc + gia tri \0 (+1)

/*  Message ID ----------------------------------------------------*/
#define PID_Message_ID 1
#define Car_State_Message_ID 2
#define First_Last_Message_ID 3

/*  Library variable declaration ----------------------------------*/
SocketIoClient webSocket;
SoftwareSerial ss(14,5);

/*  Private variable declaration ----------------------------------*/
const char* Host_Socket = "192.168.137.1";
unsigned int Port_Socket = 3000;
float P,I,D;
int State,First,Last;
String Final_string_2;
/*  Setup ---------------------------------------------------------*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ss.begin(9600);
  WiFi.begin("thang_wifi", "thangpro113");
  while(WiFi.status() != WL_CONNECTED)
  {
  delay(10);
  }
   webSocket.on("PID_value",PID_handle);
   webSocket.on("Car_State",Car_State_handle);
   webSocket.on("First_Last",Matrix_handle);
   webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
   pinMode(LED_BUILTIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly
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
}
void Matrix_String_compression(int First,int Last){
  
}

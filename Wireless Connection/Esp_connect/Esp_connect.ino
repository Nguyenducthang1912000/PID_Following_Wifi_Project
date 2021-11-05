
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>
#include <SoftwareSerial.h>
#include <string.h>

#define Send_Buffer_Size 20
#define Kp_Buffer_Size 8
#define Ki_Buffer_Size 8
#define Kd_Buffer_Size 7

// Cach tinh Send_Buffer_Size = (Kp_Buffer_Size-1) + (Ki_Buffer_Size-1) + (Kd_Buffer_Size-1)
// Cach tinh Buffer cho tung Kp,Ki,Kd => size thuc + gia tri \0 (+1)

SocketIoClient webSocket;
SoftwareSerial ss(14,5);

const char* Host_Socket = "192.168.137.1";
unsigned int Port_Socket = 3000;
float P,I,D;
void Server_Data(const char* payload,size_t length){
  pareJson_Data(payload);
}
void Print_character(char *input, int sizeofstring)
{
    for (int i = 0; i < sizeofstring; i++)
    {
        if (i == sizeofstring - 1)
        {
            printf("%cend\n",input[i]);
        }
        else
            printf("%c,",input[i]);
    }
    
}
void PID_String_compression(float kp,float ki,float kd){
  char Kp_string[Kp_Buffer_Size],Ki_string[Ki_Buffer_Size],Kd_string[Kd_Buffer_Size],Final_string[Send_Buffer_Size];
  memset(Final_string,0,sizeof(Final_string));
  sprintf(Kp_string,"%06.2f ",kp);
  sprintf(Ki_string,"%06.2f ",ki);
  sprintf(Kd_string,"%06.2f",kd);
  for(int i = 0;i < (Kp_Buffer_Size - 1) ;i++)
  {
    Final_string[i] = Kp_string[i];
  }
  
  for(int i = 0;i < (Ki_Buffer_Size - 1) ;i++)
  {
    Final_string[i + (Ki_Buffer_Size - 1)] = Ki_string[i];
  }
  
  for(int i = 0;i < (Kd_Buffer_Size - 1) ;i++)
  {
    Final_string[i + (Ki_Buffer_Size - 1) + (Kd_Buffer_Size - 1) + 1] = Kd_string[i];
  }
  ss.write(Final_string,Send_Buffer_Size);
//  ss.printf("Size of Final_string : %d \n",sizeof(Final_string));
  
}
void pareJson_Data(String strJson)
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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ss.begin(115200);
  WiFi.begin("thang_wifi", "thangpro113");
  while(WiFi.status() != WL_CONNECTED)
  {
  delay(10);
  }
   webSocket.on("data-server-to-esp",Server_Data);
   webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
   pinMode(LED_BUILTIN, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly
  webSocket.loop();
}

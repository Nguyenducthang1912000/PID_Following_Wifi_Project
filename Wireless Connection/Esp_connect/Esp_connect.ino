
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>
#include <string.h>
SocketIoClient webSocket;

char host[] = "192.168.1.32";
unsigned int port = 3000;

const char* Host_Socket = "192.168.1.32";
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
void PID_String_compression(float kp,float ki,float kd)
{
  char Kp_string[7],Ki_string[7],Kd_string[6],Final_string[17];
  memset(Final_string,0,sizeof(Final_string));
  sprintf(Kp_string,"%05.2f ",kp);
  sprintf(Ki_string,"%05.2f ",ki);
  sprintf(Kd_string,"%05.2f",kd);
  for(int i = 0;i < 6 ;i++)
  {
    Final_string[i] = Kp_string[i];
  }
  for(int i = 0;i < 6 ;i++)
  {
    Final_string[i+6] = Ki_string[i];
  }
  for(int i = 0;i < 5 ;i++)
  {
    Final_string[i+12] = Kd_string[i];
  }
  Serial.write(Final_string,17);
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
  WiFi.begin("", "");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
   webSocket.on("data-server-to-esp",Server_Data);
   webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
}
void loop() {
  // put your main code here, to run repeatedly
  webSocket.loop();
}

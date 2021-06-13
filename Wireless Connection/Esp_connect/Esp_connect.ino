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
int TxFlag = 0;
String TxString;
char kp_val[5],ki_val[5],kd_val[5];
void Server_Data(const char* payload,size_t length){
//  Serial.print(payload);
  pareJson_Data(payload);
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
    sprintf(kp_val,"%05.2f ",P);
    TxString += kp_val;
    I = (root["I"].as<float>());
    sprintf(ki_val,"%05.2f ",I);
    TxString += ki_val;
    D = (root["D"].as<float>());
    sprintf(kd_val,"%05.2f ",D);
    TxString += kd_val;
//    int whitespace_amount = 18 - TxString.length();
//    for(int i = 0;i<whitespace_amount;i++)
//    {
//      TxString += " "; 
//    }
    int str_len = TxString.length()+1;
    char TxBuff[str_len];
    
    TxString.toCharArray(TxBuff,str_len);
    TxString = "";
    Serial.write(TxBuff,str_len);
//    Serial.print(sizeof(TxBuff));
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin("", "");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
//    Serial.print(".");
  }
//   Serial.println("Ip: ");
//   Serial.print(WiFi.localIP());
   webSocket.on("data-server-to-esp",Server_Data);
   webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
}
void loop() {
  // put your main code here, to run repeatedly
  while(!TxFlag){
  webSocket.loop();
  }
}

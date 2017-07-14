#include <ESP8266WiFi.h>
#include "FTPserver.h"

#define RESET_PIN 4


const char* ssid = "GDG Network";
const char* password = "its00001";

WiFiServer server(80);

FTPserver FTPserver(RESET_PIN);

void setup()
{
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  
  server.begin();
}

void loop() {
  
  WiFiClient client = server.available(); 
  
  if (client) {
    String line = client.readStringUntil('\r');
    int HTTPopt = FTPserver.OptionCall(line);
    String urlParam = FTPserver.URLparams(line);
    
    if(HTTPopt == FTPserver.optIndex) 
    {
      FTPserver.Index(&client);
    } 
    else if(HTTPopt == FTPserver.optDelete) 
    {
      FTPserver.DeleteFile(&client, urlParam);
    } 
    else if(HTTPopt == FTPserver.optFlash) 
    {
      FTPserver.FlashFile(&client, urlParam);
    } 
    else if(HTTPopt == FTPserver.optList) 
    {
      FTPserver.FileList(&client);
    } 
    else if(HTTPopt == FTPserver.optUpload) 
    {

      FTPserver.FileUpload(&client, urlParam);
    } 
    else 
    {
      FTPserver.Index(&client);
    }
    
    delay(1); 
    client.stop();
  }
}

void formatDevice() {

  SPIFFS.begin();
  SPIFFS.format();
  SPIFFS.end();
  
}















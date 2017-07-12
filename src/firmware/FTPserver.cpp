
#include <ESP8266WiFi.h>
#include "FS.h"
#include "Arduino.h"

#include "HEXparser.h"
#include "UploadProtocol.h"
#include "FTPserver.h"

FTPserver::FTPserver(int reset)
{
	_reset = reset;
}

void FTPserver::Index(WiFiClient* client)
{
  SPIFFS.begin();
  File file = SPIFFS.open("/index.html", "r"); // to change

  if(file) 
  {
    
    int fsize = file.size();
    byte buffer[1024];
    int i = 0;
    
    client->print(DefaultHeader(true)); // maybe
    while(fsize > 0) 
    {
      i = (fsize < 1024) ? fsize : 1024;
      file.read(buffer, i);
      client->write((const uint8_t*)buffer, i);
      fsize -= 1024;
    }
    client->print(DefaultFooter());   // maybe
    
  } 
  else 
  {
    String html = HttpSimplePage(F("spiffs error: index.htm not found.")); // modify 404.
    PrintPage(client, html);  
  }

  file.close();
  SPIFFS.end();
}

void FTPserver::FileList(WiFiClient* client) 
{

  String text = HttpRawText(GetDirList());
  PrintPage(client, text);
}

void FTPserver::DeleteFile(WiFiClient* client, String filename) 
{

  SPIFFS.begin();
  SPIFFS.remove(filename);
  SPIFFS.end();
  

  String text = HttpRawText(GetDirList());
  PrintPage(client, text);
}

void FTPserver::FlashFile(WiFiClient* client, String filename) 
{

  UploadProtocol avrdude(_resetPin);
  
  SPIFFS.begin();

  File file = SPIFFS.open(filename, "r");
  
  if(file) 
  {
    avrdude.setupDevice();
    HEXparser parse = HEXparser();
    

    while(file.available()) 
    {

      byte buff[50];
      String data = file.readStringUntil('\n');
      data.getBytes(buff, data.length());
      parse.ParseRecord(buff);
      
      if(parse.CheckReady())
      {
        byte* page = parse.GetMemoryPage();
        byte* address = parse.GetLoadAddress();
        avrdude.ProgramPage(address, page);
      }
    }
  }
  

  avrdude.closeProgMode();
  file.close();
  SPIFFS.end();
}

void FTPserver::FileUpload(WiFiClient* client, String filename) 
{

  int contentLen = 0;

  while (client->connected()) 
  {
    if (client->available()) 
    {
      String line = client->readStringUntil('\n');

      if(line.startsWith("Content-Length")) 
      {
        contentLen = line.substring(16, (line.length()-1)).toInt();
      }
      
      if(line.length() == 1 && line[0] == '\r') 
      {
        
        SPIFFS.begin();
        String path = "/hex/" + filename;
        File file = SPIFFS.open(path, "w+");

        if(file) 
        {
          int i = 0;
          while (i < contentLen) 
          {
            file.write(client->read());
            i++;
          }
          file.close();
        } 
        SPIFFS.end();
        
        delay(10);
        String html = HttpSimplePage("DONE");
        client->println(html);
        delay(10);
        
        break;
      } 
    }
  }
}
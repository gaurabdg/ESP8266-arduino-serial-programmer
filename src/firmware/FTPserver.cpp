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
  File file = SPIFFS.open(" ", "r"); // to do

  if(file) 
  {
    
    int fsize = file.size();
    byte buffer[1024];
    int i = 0;
    
    client->print(SendHeader()); 
    while(fsize > 0) 
    {
      i = (fsize < 1024) ? fsize : 1024;
      file.read(buffer, i);
      client->write((const uint8_t*)buffer, i);
      fsize -= 1024;
    }
    client->print(SendFooter());
    
  } 
  else 
  {
    // to-do 
  }

  file.close();
  SPIFFS.end();
}


void FTPserver::FileList(WiFiClient* client) 
{

  String text = ReadyToSendText(HexDirectory());
  PublishPage(client, text);
}

void FTPserver::DeleteFile(WiFiClient* client, String filename) 
{

  SPIFFS.begin();
  SPIFFS.remove(filename);
  SPIFFS.end();
  

  String text = ReadyToSendText(HexDirectory());
  PublishPage(client, text);
}

void FTPserver::FlashFile(WiFiClient* client, String filename) 
{

  UploadProtocol avrdude(_reset);    // ;)
  
  SPIFFS.begin();

  File file = SPIFFS.open(filename, "r");
  
  if(file) 
  {
    avrdude.DeviceSetup();
    HEXparser parse = HEXparser();
    

    while(file.available()) 
    {

      byte buffer[50];
      String data = file.readStringUntil('\n');
      data.getBytes(buffer, data.length());
      parse.ParseRecord(buffer);
      
      if(parse.CheckReady())
      {
        byte* page = parse.FetchRaw();
        byte* address = parse.FetchAddress();
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

  int contentLength = 0;

  while (client->connected()) 
  {
    if (client->available()) 
    {
      String line = client->readStringUntil('\n');

      if(line.startsWith("Content-Length")) 
      {
        contentLength = line.substring(16, (line.length()-1)).toInt();
      }
      
      if(line.length() == 1 && line[0] == '\r') 
      {
        
        SPIFFS.begin();
        String path = "/hex/" + filename;
        File file = SPIFFS.open(path, "w+");

        if(file) 
        {
          int i = 0;
          while (i < contentLength) 
          {
            file.write(client->read());
            i++;
          }
          file.close();
        } 
        SPIFFS.end();
        break;
      } 
    }
  }
}

String FTPserver::URLparams(String s) 
{

  String param = "";
  if(s.indexOf("&") > -1) 
  {
    int start = s.indexOf("&") +1;
    int end = s.indexOf(" ", start);
    param = s.substring(start, end);   
  }

  return param;
  
}

String FTPserver::SendHeader() 
{

  return String(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n"));
    
}

String FTPserver::SendFooter() 
{

  return String(F("\r\n"));
  
}

String FTPserver::ReadyToSendText(String text) 
{

  String html = SendHeader();
  html +=  text;
  html += SendFooter();  

  return html;
  
}

String FTPserver::HexDirectory() 
{

  String list = "";
  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/hex");
  while (dir.next()) 
  {
    list += dir.fileName() + ";";
    File f = dir.openFile("r");
    list += String(f.size()) + ";\n";
  }
  SPIFFS.end();
  return list;
}

void FTPserver::PublishPage(WiFiClient* client, String page) 
{
  while (client->connected()) 
  {
    if (client->available()) 
    {
      String line = client->readStringUntil('\r');
      if (line.length() == 1 && line[0] == '\n') 
      {
        client->println(page);
        break;
      }
    }
  }
}

int FTPserver::OptionCall(String s) 
{

  if(s.startsWith("GET /files"))
  {
    return optList;
  } 
  else if (s.startsWith("GET /delete")) 
  {
    return optDelete;
  } 
  else if (s.startsWith("GET /flash")) 
  {
    return optFlash;
  } 
  else if (s.startsWith("GET /delete")) 
  {
    return optDelete;
  } 
  else if (s.startsWith("POST /upload")) 
  {
    return optUpload;
  } 
  else 
  {
    return optIndex;
  }
  
}
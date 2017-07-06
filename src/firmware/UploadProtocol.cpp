#include "UploadProtocol.h"
#include "Arduino.h"

UploadProtocol::UploadProtocol(int reset)
{
	Serial.begin(115200);

	pinMode(reset, OUTPUT)
	int _reset = reset;
}

void UploadProtocol::DeviceSetup()
{
	reset();
	sync();
	setParams();
	setExtParams();
	startProgMode();
}

void UploadProtocol::reset() 
{
  
  digitalWrite(_reset, LOW);
  delay(1);
  digitalWrite(_reset, HIGH);
  delay(100);
  digitalWrite(_reset, LOW);
  delay(1);
  digitalWrite(_reset, HIGH);
  delay(100);

}

int UploadProtocol::sync()
{
	return SendCmmnd(0x30);
}

int UploadProtocol::startProgMode()
{
	return SendCmmnd(0x50);
}

void UploadProtocol::ProgramPage(byte* address, byte* data)
{
	byte init[] = { 0x64, 0x00, 0x80, 0x46 };
	
	setLoadAddress(address[1], address[0]);
	Serial.write(init, 4);

	for(int i = 0; i<128 ; i++)
	{
		Serial.write(data[i]);
	}

	Serial.write(0x20);      // SYNC_CRC_EOP                                   
}

void UploadProtocol::setLoadAddress(byte high, byte low)
{
	byte buffer[] = {high, low};
	return SendParams(0x55, buffer, sizeof(buffer));
}

byte UploadProtocol::SendCmmnd(byte cmmnd) 
{

  byte bytes[] = { cmmnd, 0x20 };
  return WriteBytes(bytes, 2);
}

byte UploadProtocol::SendParams(byte cmmnd, byte* params, int len) 
{

  byte bytes[32];
  bytes[0] = cmmnd;

  int i = 0;
  while (i < len) 
  {
    bytes[i + 1] = params[i];
    i++;
  }

  bytes[i + 1] = 0x20;

  return WriteBytes(bytes, i + 2);
}


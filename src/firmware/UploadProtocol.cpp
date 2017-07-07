#include "UploadProtocol.h"
#include "Arduino.h"

UploadProtocol::UploadProtocol(int reset)
{
	Serial.begin(115200);
	pinMode(reset, OUTPUT);
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

int UploadProtocol::setParams()
{
	byte params[] = { 0x86, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x04, 0x00, 0x00, 0x00, 0x80, 0x00}; //current support for optiboot/atmega328p
	return SendParams(0x42, params, sizeof(params));
}

int UploadProtocol::setExtParams()
{
	byte extparams[] = { 0x05, 0x04, 0xd7, 0xc2, 0x00};
	return SendParams(0x45, extparams, sizeof(extparams));
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

int UploadProtocol::WriteBytes(byte* bytes, int len)
{
	Serial.write(bytes, len);

	byte inSync = Serial.read();
	byte checkOK == Serial.read();
	if(inSync = 0x14 && checkOK = 0x10)
	{
		return 1;
	}
	return 0;
}

int UploadProtocol::closeProgMode()
{
	return SendCmmnd(0x51);
}
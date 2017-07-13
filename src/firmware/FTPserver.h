#ifndef FTPserver_h
#define FTPserver_h

#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "FS.h"

#include "HEXparser.h"
#include "UploadProtocol.h"
#include "FTPserver.h"

class FTPserver
{
	public:
		FTPserver(int res);
		int OptionCall(String x);
		String URLparams(String x);
		String ReadyToSendText(String x);

		void Index(WiFiClient* cl);
		void FileList(WiFiClient* cl);
		void DeleteFile(WiFiClient* cl);
		void FlashFile(WiFiClient* cl);
		void FileUpload(WiFiClient* cl);

		const int optIndex = 0;
		const int optFlash = 0;
		const int optUpload = 0;
		const int optDelete = 0;
		const int optList = 0;


	private:
		int _reset = 0;

		String HexDirectory();
		void PublishPage(WiFiClient* cl, String x);
		String SendHeader();
		String SendFooter();
}
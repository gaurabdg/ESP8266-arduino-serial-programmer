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
		void DeleteFile(WiFiClient* cl, String s);
		void FlashFile(WiFiClient* cl, String s);
		void FileUpload(WiFiClient* cl, String s);

		const int optIndex = 0;
		const int optFlash = 1;
		const int optUpload = 2;
		const int optDelete = 3;
		const int optList = 4;


	private:
		int _reset = 0;

		String HexDirectory();
		void PublishPage(WiFiClient* cl, String x);
		String SendHeader(bool y);
		String SendFooter();
};

#endif

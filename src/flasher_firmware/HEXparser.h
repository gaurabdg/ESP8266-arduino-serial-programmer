#ifndef HEXparser_h
#define HEXparser_h
#include "Arduino.h"

class HEXparse
{
	public:
		HEXparser();
		void ParseRecord(byte* data); // Import data from spiffs.open() in web server.
		byte* FetchRaw();
		byte* FetchAddress();

	private:
		int recType = 0; // Here only 0/1 is needed-data present/end of file.
		int length = 0;
		int index = 0;
		byte RawPage[128];
		byte address[2];

		/* Intel HEX file format info */
		int RecordLength(byte* record);
		void RecordAddress(byte* record);
		int RecordType(byte* record);

		//                             //
		byte* extractData(byte* record, int len);
		void FileEnd();
};

#endif
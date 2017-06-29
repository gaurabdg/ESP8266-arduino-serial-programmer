#ifndef HEXparser_h
#define HEXparser_h
#include "Arduino.h"

class HEXparse
{
	public:
		HEXparse();
		ParseRecord(byte* data); // Import data from spiffs.open() in web server.
		
	private:
		int recType = 0; // Here only 0/1 is needed-data present/end of file.
		int length = 0;

		/* Intel HEX file format info */
		int RecordType(byte* record);
		int RecordAddress(byte* record);
		int RecordLength(byte* record);
		//                             //
		byte* extractData(byte* record, int len);
		void FileEnd();
};

#endif
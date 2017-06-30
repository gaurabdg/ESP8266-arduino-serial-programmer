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
		int recType = 0;              // Here only 0/1 is needed-data present/end of file.
		int length = 0;  
		int index = 0;                // to keep track of the no. of bytes in each chunk
		byte RawPage[128];            // sending 128 bytes of 'chunk' each time
		byte address[2];
		bool firstTime = true;        // data address is to be updated only after the first chunk transfer

		/* Intel HEX file format info */
		int RecordLength(byte* record);
		void RecordAddress(byte* record);
		int RecordType(byte* record);
        //                             //
		void extractData(byte* record, int len);
		void FileEnd();
};

#endif
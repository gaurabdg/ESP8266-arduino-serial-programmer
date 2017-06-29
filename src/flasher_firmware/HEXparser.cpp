#include "HEXparser.h"
#include "Arduino.h"

void HEXparser::ParseRecord()
{
	recType = RecordType(record);

	if(recType == 0)
	{
		extractData(record, length);
	}

	if(recType == 1)
	{
		FileEnd();
	}
}

int HEXparser::RecordAddress(byte* record)
{
	char holder[5];

	holder[0] = record[3];
	holder[1] = record[4];
	holder[2] = record[5];
	holder[3] = record[6];
	holder[4] = '\0';

	return strtol(holder, 0, 16);
}

int HEXparser::RecordLength(byte* record)
{
	char holder[3];

	holder[0] = record[1];
	holder[1] = record[2];
	holder[2] = '\0';

	return strtol(holder, 0, 16);
}

int HEXparser::RecordType(byte* record)
{
	char holder[3];

	holder[0] = record[7];
	holder[1] = record[8];
	holder[2] = '\0';

	return strtol(holder, 0, 16);	
}

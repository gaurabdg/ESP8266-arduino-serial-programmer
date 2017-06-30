#include "HEXparser.h"
#include "Arduino.h"

//initiate address
HEXparser::HEXparser()
{
	address[0] = 0x00;
	address[1] = 0x00;
}


int HEXparser::RecordLength(byte* record)
{
	char holder[3];

	holder[0] = record[1];
	holder[1] = record[2];
	holder[2] = '\0';

	return strtol(holder, 0, 16);
}

void HEXparser::RecordAddress(byte* record)
{
	char holder[3];
	
	holder[2] = '\0';

	holder[0] = record[3];
	holder[1] = record[4];
	address[0] = strtol(buff, 0, 16);

	holder[0] = record[5];
	holder[1] = record[6];
	address[1] = strtol(buff, 0, 16); 
}



int HEXparser::RecordType(byte* record)
{
	char holder[3];

	holder[0] = record[7];
	holder[1] = record[8];
	holder[2] = '\0';

	return strtol(holder, 0, 16);	
}

byte* HEXparser::extractData(byte* record, int len)
{
	int begin = 9; 
	int end = (len*2) + begin; //data is in sets of 2
	char holder[3];
	holder[2] = '\0';

	for(int i=begin;begin<end;i=i+2)
	{
		holder[0] = record[i];
		holder[1] = record[i+1];

		RawPage[index] = strtol(holder, 0, 16);
		index++;
	}
}

void HEXparser::FileEnd()
{
	address[1] += 0x40;
	if(address[1] == 0)
	{
		address[0] += 0x1;
	}

	while(index<128)
	{
		RawPage[index] = 0xFF;
		index++ ;
	}
}

void HEXparser::ParseRecord(byte* record)
{
	recType = RecordType(record);

	if(recType == 0)
	{
		length = RecordLength(record);
		extractData(record, length);

		if(index == 128)
		{
			address[1] += 0x40;

			if(address[1] == 0)
			{
					address[0] += 0x1;
			}

			index = 0;
		}
	}

	if(recType == 1)
	{
		FileEnd();
	}
}

byte* HEXparser::FetchRaw()
{
	return RawPage;
}

byte* HEXparser::FetchAddress()
{
	return address;
}
#ifndef UploadProtocol_h
#define UploadProtocol_h
#include "Arduino.h"

class UploadProtocol
{
	public:
		UploadProtocol(int reset);
		void reset();
		void setup();
		int sync();
		int setParams();
		int setExtParams();
		int startProgMode();
		void ProgramPage(byte* address, byte* data);
		int setLoadAddress(byte high, byte low);
		int closeProgMode();

	private:
		byte SendCmmnd(byte cmmnd);
		byte SendParams(byte cmmnd, byte* params, int len);
		byte WriteBytes(byte* bytes, int len);

		int _reset;

}

#endif


#ifndef UploadProtocol_h
#define UploadProtocol_h
#include "Arduino.h"

class UploadProtocol
{
	public:
		UploadProtocol(int reset);
		void reset();
		int sync();
		int setParams();
		int setExtParams();
		int startProgMode();
		int setLoadAddress();

	private:
		byte SendCmmnd(byte cmmnd);
		byte SendParams(byte cmmnd, byte* params, int len);
		byte WriteBytes(byte* bytes, int len);

		int _reset;

}

#endif
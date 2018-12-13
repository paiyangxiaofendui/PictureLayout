// QRcodeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>

using namespace std;

#include "../../include/QRCode/qrcode/qrencode.h"


//#ifdef DEBUG 

#pragma comment(lib, "../../lib/QRCoded.lib")

#pragma  message(".......................................当前为debug版本")

// #else
// 
// #pragma comment(lib, "../../lib/QRCode.lib")
// 
// #pragma  message(".......................................当前为release版本")
// 
// #endif


int _tmain(int argc, _TCHAR* argv[])
{
	string str = "袁梓埠123yzb";

	QRcode*			pQRC;
	const char* pSrc = str.c_str();



	if (pQRC = QRcode_encodeString(pSrc, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
	{

		int unWidth = pQRC->width;

		int p_x = 0;
		int p_y = 0;

		unsigned char*	 pSourceData = pQRC->data;
		for (int c_i = 0;c_i < unWidth;c_i++)
		{
			for (int c_j = 0;c_j < unWidth;c_j++)
			{
				
				
				pSourceData++;
			}
		}

		QRcode_free(pQRC);
	}


	return 0;
}


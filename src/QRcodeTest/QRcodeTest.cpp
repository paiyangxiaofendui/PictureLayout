// QRcodeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <windows.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include <comdef.h>
#include <GdiPlus.h>
#include <wingdi.h>

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

void qr_code(std::string _src_str, std::string _dst_bmp_path)
{

	const char*  szSourceSring = _src_str.c_str();
	unsigned int unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char* pRGBData, *pSourceData, *pDestData;
	QRcode* pQRC;
	FILE*  f;


	if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * 8 * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * 8;



		// Allocate pixels buffer
		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			exit(-1);
		}



		// Preset to white
		memset(pRGBData, 0xff, unDataBytes);

		// Prepare bmp headers
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42; // "BM"
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER) +
			unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER);
		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * 8;
		kInfoHeader.biHeight = -((int)unWidth * 8);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;

		// Convert QrCode bits to bmp pixels
		pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * 8;
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < 8; l++)
					{
						for (n = 0; n < 8; n++)
						{


							*(pDestData + n * 3 + unWidthAdjusted * l) = 0x00;
								*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = 0x00;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = 0x00;
						}
					}
				}
				pDestData += 3 * 8;
				pSourceData++;
			}
		}

		// Output the bmp file
		if (!(fopen_s(&f, _dst_bmp_path.c_str(), "wb")))
		{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
			printf("qrcode has generated in %s\n", _dst_bmp_path.c_str());
			fclose(f);
		}
		else
		{
			printf("Unable to open file");
			exit(-1);
		}

		// Free data
		free(pRGBData);
		QRcode_free(pQRC);
	}
	else
	{
		printf("NULL returned");
		exit(-1);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	string str = "打印批号二维码测试123abc";

	qr_code(str, "test.bmp");

// 	QRcode*			pQRC;
// 	const char* pSrc = str.c_str();
// 
// 
// 
// 	if (pQRC = QRcode_encodeString(pSrc, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
// 	{
// 
// 		int unWidth = pQRC->width;
// 
// 		int p_x = 0;
// 		int p_y = 0;
// 
// 		unsigned char*	 pSourceData = pQRC->data;
// 		for (int c_i = 0;c_i < unWidth;c_i++)
// 		{
// 			for (int c_j = 0;c_j < unWidth;c_j++)
// 			{
// 				
// 				
// 				pSourceData++;
// 			}
// 		}
// 
// 		QRcode_free(pQRC);
// 	}


	return 0;
}


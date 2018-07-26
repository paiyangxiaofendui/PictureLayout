#include "stdafx.h"
#include <atlimage.h>
#include "Misc.h"
#include <math.h>
#include "HGTools.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../Work/WorkDef.h"

#define PI 3.14159

bool SaveBitmapToFile(CBitmap* bitmap, LPCTSTR lpFileName);

CString GetModulePath()
{
	TCHAR cStr[MAX_PATH+1];
	GetModuleFileName(/*GetModuleHandle(0)*/AfxGetModuleState()->m_hCurrentInstanceHandle, cStr, MAX_PATH+1);
	CString str(cStr);
	str = str.Left(str.ReverseFind('\\')+1);
	return str;
}

CString GetFloatString(float fNum, UCHAR uDecimals)
{
	CString strRet;
	CString strTmp;
	strTmp.Format(_T("%%.%df"), uDecimals);
	strRet.Format(strTmp, fNum);
	return strRet;
}

CString GetFloatStringTrimming(float fNum, UCHAR uDecimals)
{
	CString strRet = GetFloatString(fNum, uDecimals);

	int nPos = strRet.Find(_T("."));
	if(nPos != -1)
	{
		while(strRet.GetLength() > nPos)
		{
			if(strRet.GetAt(strRet.GetLength()-1) == _T('0') || strRet.GetAt(strRet.GetLength()-1) == _T('.'))
			{
				strRet = strRet.Left(strRet.GetLength()-1);
			}
			else
				break;
		}
	}

	return strRet;
}

CString GetIntegerString(int nNum, UCHAR uBitCount)
{
	CString strRet;
	if(uBitCount == 0)
		strRet.Format(_T("%d"), nNum);
	else
	{
		CString strTmp;
		strTmp.Format(_T("%%0%dd"), uBitCount);
		strRet.Format(strTmp, nNum);
	}

	return strRet;
}

bool IsFloatEqual(float f1, float f2, float fPrecision)
{
	if(fabs(f1 - f2) <= fPrecision)
		return TRUE;
	return FALSE;
}

float GetScale(float fPanelWidth, float fPanelHeight, float fDrawAreaWidth, float fDrawAreaHeight)
{
	float scale = 1;
	scale = (fDrawAreaWidth / fPanelWidth) > (fDrawAreaHeight / fPanelHeight) ? (fDrawAreaHeight / fPanelHeight) : (fDrawAreaWidth / fPanelWidth);//获取缩放系数
	return scale;
}

float GetPointDistance(float fX1, float fY1, float fX2, float fY2)
{
	return sqrt(pow(fX1-fX2, 2) + pow(fY1-fY2, 2));
}


Image* LoadPngImgFromRes(UINT uResID)
{
	HINSTANCE hInst = /*AfxGetResourceHandle()*/AfxGetModuleState()->m_hCurrentInstanceHandle;
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(uResID), _T("PNG")); // type
	if (!hRsrc)
		return NULL;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return NULL;

	// Allocate global memory on which to create stream
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pMem = (BYTE*)GlobalLock(hMem);
	memcpy(pMem,lpRsrc,len);
	IStream* pStm;
	CreateStreamOnHGlobal(hMem,FALSE,&pStm);

	Image* pImg=Gdiplus::Image::FromStream(pStm);

	// free/release stuff
	pStm->Release();
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	FreeResource(lpRsrc);

	return pImg;
}

CString GetFileNameInPath(CString strPath)
{
	CString strRet;
	int nPos = strPath.ReverseFind(_T('\\'));
	int nPos1 = strPath.ReverseFind(_T('.'));
	strRet = strPath.Mid(nPos+1, nPos1 - nPos - 1);
	return strRet;
}

CString GetFileNameInPath_WithSuffix(CString strPath)
{
	CString strRet;
	int nPos = strPath.ReverseFind(_T('\\'));
	strRet = strPath.Right(strPath.GetLength()-(nPos+1));
	return strRet;
}

bool MakeSureFolderExist(CString strPath)
{
	if(!PathFileExists(strPath))
	{
		return CreateMultipleDirectory(strPath);
	}
	return true;
}

bool IsKeyPressed(UINT nKeyID)
{
	//byte byteKeyState[256];
	//GetKeyboardState(byteKeyState);
	//return byteKeyState[nKeyID]&1;

	return 0x8000 & GetKeyState(nKeyID); 
}

float DistanceBetweenPoints(float fX1, float fY1, float fX2, float fY2)
{
	return sqrt((fX1 - fX2) * (fX1 - fX2) + (fY1 - fY2) * (fY1 - fY2));
}

float GetLineAngle(float fX1, float fY1, float fX2, float fY2)
{
	//两点的x、y值
	float x = fX2 - fX1;
	float y = fY2 - fY1;
	float fLen = DistanceBetweenPoints(fX1, fY1, fX2, fY2);

	if(fLen == 0.0)
		return 0;
	if(x == 0.0)
	{
		if(y > 0)
			return 90;
		else
			return -90;
	}

	//斜边长度
	float fCos = x/fLen;
	float fAcos = acos(fCos);
	//求出弧度
	float angle = 180.0/(PI/fAcos);
	//用弧度算出角度        
	if (y<0) {
		angle = -angle;
	} else if ((y == 0) && (x<0)) {
		angle = 180;
	}
	return angle;
}

bool GetArcMidPoint(float fX1, float fY1, float fX2, float fY2, float fR, int nDir, float& fXMid, float& fYMid)
{
	float fXCenter, fYCenter;
	float fSin, fCos;
	if(!GetCircleCenterByStartEndRD(fX1, fY1, fX2, fY2, fR, nDir, fXCenter, fYCenter, fSin, fCos))
		return false;

	float fClockwiseRotateAng = 0.0;
	if(nDir == DirectionType::CLOCKWISE)
	{
		fClockwiseRotateAng = (acos(fCos)*360/(PI*2))/2;
	}
	else
	{
		fClockwiseRotateAng = 360 - (acos(fCos)*360/(PI*2))/2;
	}
	RotatePoint(fX1, fY1, fXCenter, fYCenter, fClockwiseRotateAng, fXMid, fYMid);

	return true;
}

void NormalVector_ex(float x0,float y0,float & x,float & y)
{
	float len = sqrt(x0*x0 + y0*y0);

	x = x0/len;
	y = y0/len;

}

bool GetArcMidPoint_ex(float fX1, float fY1, float fX2, float fY2, float fR, int nDir, float& fXMid, float& fYMid)
{
	float r = fR;
	int sign = nDir;
	//GetCircleCenter(cur_point.x, end_pnt.x,cur_point.y, end_pnt.y, r, sign, center_x, center_y);
	float fXCenter, fYCenter, fSin, fCos;
	GetCircleCenterByStartEndRD(fX1, fY1, fX2, fY2, fR, nDir, fXCenter, fYCenter, fSin, fCos);

	// 求弦中点绝对坐标
	float mid_x = (fX2 - fX1)/2.0 + fX1;
	float mid_y = (fY2 - fY1)/2.0 + fY1;

	float vec_x = mid_x - fXCenter;
	float vec_y = mid_y - fYCenter;

	NormalVector_ex(vec_x, vec_y, vec_x, vec_y);
	vec_x *= r;
	vec_y *= r;

	// 求圆弧中点绝对坐标
	fXMid = fXCenter + vec_x;
	fYMid = fYCenter + vec_y;

	return true;
}

bool GetCircleCenterByStartEndRD(float fXStart, float fYStart, float fXEnd, float fYEnd, float fR, int nDir, float& fXCenter, float& fYCenter, float& fSin, float& fCos)
{
	float fXa = fXStart, fYa = fYStart, fXb = fXEnd, fYb = fYEnd/*, fR = fR*/;
	//strTmp.Format(L"fXa = %f, fYa = %f, fXb = %f, fYb = %f, fR = %f\n", fXa, fYa, fXb, fYb, fR);
	//OutputDebugString(strTmp);
	float fDx = fXa - fXb;
	//strTmp.Format(L"fDx = %f\n", fDx);
	//OutputDebugString(strTmp);
	float fDy = fYa - fYb;
	//strTmp.Format(L"fDy = %f\n", fDy);
	//OutputDebugString(strTmp);

	float fCenterX1;
	float fCenterX2;
	float fCenterY1;
	float fCenterY2;
	if(IsFloatEqual(fDy, 0, 0.1))
	{
		fCenterX1 = (fXa + fXb) / 2;
		fCenterX2 = (fXa + fXb) / 2;

		fCenterY1 = fYa - sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
		fCenterY2 = fYa + sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
	}
	else
	{
		float fD = fDx / fDy;
		//strTmp.Format(L"fD = %f\n", fD);
		//OutputDebugString(strTmp);
		float fa = 1 + fD*fD;
		float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
		float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
		//strTmp.Format(L"fa = %f, fb = %f, fc = %f\n", fa, fb, fc);
		//OutputDebugString(strTmp);
		fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
		fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
		fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
		fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
	}


	//strTmp.Format(L"fCenterX1 = %f, fCenterY1 = %f\n", fCenterX1, fCenterY1);
	//OutputDebugString(strTmp);
	//strTmp.Format(L"fCenterX2 = %f, fCenterY2 = %f\n", fCenterX2, fCenterY2);
	//OutputDebugString(strTmp);
	//SolidBrush brush(Color::Black);
	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX1*fScale), (REAL)(rcPanel.top +fCenterY1*fScale), (REAL)3, (REAL)3);
	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX2*fScale), (REAL)(rcPanel.top +fCenterY2*fScale), (REAL)3, (REAL)3);

	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXa*fScale), (REAL)(rcPanel.top +fYa*fScale), (REAL)3, (REAL)3);
	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXb*fScale), (REAL)(rcPanel.top +fYb*fScale), (REAL)3, (REAL)3);


	PointF vectorStart(fXa-fCenterX1, fYa-fCenterY1);
	PointF vectorEnd(fXb-fCenterX1, fYb-fCenterY1);
	float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
	float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
	fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
	fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);

	//strTmp.Format(L"fCos = %f, fSin = %f\n", fCos, fSin);
	//OutputDebugString(strTmp);
	//bool bAngleLessThan180 = true;
	//if(fSin < 0 || (IsFloatEqual(fSin, 0) && fCos < 0))
	//	bAngleLessThan180 = false;


	PointF ptTheCenter;
	if(((-fSin) >= 0 && nDir == 1) || ((-fSin) < 0 && nDir == 2))
	{
		ptTheCenter.X = fCenterX2;
		ptTheCenter.Y = fCenterY2;
	}
	else
	{
		ptTheCenter.X = fCenterX1;
		ptTheCenter.Y = fCenterY1;
	}

	fXCenter = ptTheCenter.X;
	fYCenter = ptTheCenter.Y;
	return true;
}

void GetCircleCenterByThreePoint(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3, float& fXCenter, float& fYCenter)
{
	float fU = (pow(fX1, 2) - pow(fX2, 2) + pow(fY1, 2) - pow(fY2, 2)) / (2*fX1 - 2*fX2);
	float fK1 = (fY1 - fY2) / (fX1 - fX2);
	float fV = (pow(fX1, 2) - pow(fX3, 2) + pow(fY1, 2) - pow(fY3, 2)) / (2*fX1 - 2*fX3);
	float fK2 = (fY1 - fY3) / (fX1 - fX3);

	fYCenter = (fU - fV) / (fK1 - fK2);
	fXCenter = fV - (fU - fV) * fK2 / (fK1 - fK2);
}

int GetArcDirByPoint(float fX1, float fY1, float fX2, float fY2, float fCenterX, float fCenterY)
{
	PointF vectorStart(fX1-fCenterX, fY1-fCenterY);
	PointF vectorEnd(fX2-fCenterX, fY2-fCenterY);
	float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
	float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
	//float fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
	float fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);

	if(fSin > 0)
		return DirectionType::ANTI_CLOCKWISE;
	else
		return DirectionType::CLOCKWISE;
}

void RotatePoint(float fOrgX, float fOrgY, float fCenterX, float fCenterY, float fClockwiseRotateAng, float& fRotatedX, float& fRotatedY)
{
	float fVectorX = fOrgX - fCenterX;
	float fVectorY = fOrgY - fCenterY;
	float fRotateArc = (/*360 - */-fClockwiseRotateAng) / 360 * (PI * 2);
	//float fVectorLen = sqrt(pow(fVectorX, 2) + pow(fVectorY, 2));

	float fRotatedVectorX = fVectorX * cos(fRotateArc) - fVectorY * sin(fRotateArc);
	float fRotatedVectorY = fVectorY * cos(fRotateArc) + fVectorX * sin(fRotateArc);
	fRotatedX = fCenterX + fRotatedVectorX;
	fRotatedY = fCenterY + fRotatedVectorY;
}

int GetRegularAngle(int nAngle)
{
	nAngle = nAngle % 360;
	return nAngle;
}

bool ConvertBmpToOneBitBmp(CString strFilePath)
{
	USES_CONVERSION;

	CString strTmpPath;
	int nPos = strFilePath.ReverseFind(_T('\\'));
	int nPos1 = strFilePath.ReverseFind(_T('.'));
	strTmpPath = strFilePath.Left(nPos+1) + strFilePath.Mid(nPos+1, nPos1-nPos-1) + _T("_") + strFilePath.Right(strFilePath.GetLength()-nPos1);

	unsigned char *pBmpBuf;//读入图像数据的指针

	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	RGBQUAD *pColorTable;//颜色表指针
	int biBitCount;//图像类型

	//二进制读方式打开指定的图像文件
	FILE *fp = fopen(strFilePath,"rb"); 
	if(fp==0)
		return false; 
	//跳过位图文件头结构BITMAPFILEHEADER  
	fseek(fp, sizeof(BITMAPFILEHEADER),0); 
	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER head;  
	fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
	//获取图像宽、高、每像素所占位数等信息
	bmpWidth = head.biWidth; 
	bmpHeight = head.biHeight; 
	biBitCount = head.biBitCount; 
	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	int lineByte=(bmpWidth * biBitCount/8+3)/4*4; 
	//灰度图像有颜色表，且颜色表表项为256 
	if(biBitCount==8)	
	{
		//申请颜色表所需要的空间，读颜色表进内存
		pColorTable=new RGBQUAD[256];  
		fread(pColorTable,sizeof(RGBQUAD),256,fp); 
	} 

	//申请位图数据所需要的空间，读位图数据进内存
	pBmpBuf=new unsigned char[lineByte * bmpHeight]; 
	fread(pBmpBuf,1,lineByte * bmpHeight,fp);  

	//关闭文件
	fclose(fp); 



	CFile file;
	if(!file.Open(strTmpPath, CFile::modeRead|CFile::modeWrite|CFile::modeCreate))
		return false;

	//BITMAP bp;
	//m_pPrim->GetBitmap(&bp);      // m_pPrim 是要转成8位位图的原始图

	// 构建BITMAPFILEHEADER结构
	BITMAPFILEHEADER bfHeader;
	int nWidthBytes   = ((bmpWidth + 31) & ~31) / 8 /*(bmpWidth + 3) / 4 * 4*/;       // 保存每行是4个字节的整数倍
	UINT size    = bmpHeight * nWidthBytes;
	bfHeader.bfType   = MAKEWORD('B', 'M');      // BM 
	bfHeader.bfOffBits  = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD);
	bfHeader.bfSize   = bfHeader.bfOffBits + size;
	bfHeader.bfReserved1 = 0;
	bfHeader.bfReserved2 = 0;

	// 构建BITMAPINFOHEADER结构
	BITMAPINFOHEADER biHeader;
	biHeader.biSize   = sizeof(BITMAPINFOHEADER);
	biHeader.biWidth  = bmpWidth;
	biHeader.biHeight  = bmpHeight;
	biHeader.biPlanes  = 1;
	biHeader.biBitCount  = 1;
	biHeader.biCompression = /*BI_RGB*/0;
	biHeader.biSizeImage = size;
	biHeader.biXPelsPerMeter= 0;
	biHeader.biYPelsPerMeter= 0;
	biHeader.biClrUsed  = 0;
	biHeader.biClrImportant = 0;

	//ExtractData();

	// 创建调色板
	RGBQUAD quad[2];
	quad[0].rgbBlue = quad[0].rgbGreen = quad[0].rgbRed = 0;
	//quad[0].rgbReserved = 0;
	quad[1].rgbBlue = quad[1].rgbGreen = quad[1].rgbRed = 255;

	PBYTE pData = new BYTE[size];
	ZeroMemory(pData,size);
	for(UINT h=0; h<bmpHeight; h++)
	{
		for(UINT w=0; w<bmpWidth; w++)
		{
			PBYTE pPixelStart = pBmpBuf + h*(lineByte/*/8*/) + w*(biBitCount/8);
			unsigned char b = *pPixelStart;
			unsigned char g = *(pPixelStart+1);
			unsigned char r = *(pPixelStart+2);
			unsigned int nTotal = r + g + b;


			unsigned int uBitPosOffset = (nWidthBytes*8)*h + w;
			unsigned char uBitValue = (nTotal/3 > 200) ? 0x01 : 0x00;
			*(pData + uBitPosOffset/8) = *(pData + uBitPosOffset/8) | (uBitValue << (7-uBitPosOffset%8));

			//*(pData + h*nWidthBytes + w) = nTotal/3/* > 126*3*//* ? 255 : 0*/;//*(m_pData + (bp.bmHeight - h - 1)*bp.bmWidth + w);
		}
	}

	// 位图写入文件
	file.Write(&bfHeader, sizeof(bfHeader));
	file.Write(&biHeader, sizeof(BITMAPINFOHEADER));
	file.Write(quad, 2 * sizeof(RGBQUAD));
	file.Write(pData, size);

	delete pData;
	pData = NULL;

	file.Close();

	DeleteFile(strFilePath);
	rename(strTmpPath, strFilePath);

	return true;
}

bool RescaleBmp(CString strFilePath, int nTargetWidth, int nTargetHeight)
{
	if(!PathFileExists(strFilePath))
		return false;

	CImage image;
	image.Load(strFilePath);

	CDC dcImage;
	dcImage.Attach(image.GetDC());

	CDC dcMem;
	CBitmap bmpMem;
	dcMem.CreateCompatibleDC(&dcImage);
	bmpMem.CreateCompatibleBitmap(&dcImage, nTargetWidth, nTargetHeight);
	dcMem.SelectObject(&bmpMem);


	float fScale = GetScale(image.GetWidth(), image.GetHeight(), nTargetWidth, nTargetHeight);
	//CRect rcPanelArea(param.m_nGap/2, param.m_nGap/2, rcDrawArea.Width()-param.m_nGap/2, rcDrawArea.Height()-param.m_nGap/2);
	CRect rcDrawArea;
	rcDrawArea.SetRect(0, 0, nTargetWidth, nTargetHeight);

	CRect rcPanel((rcDrawArea.Width()-image.GetWidth()*fScale)/2, \
		(rcDrawArea.Height()-image.GetHeight()*fScale)/2, \
		(rcDrawArea.Width()-image.GetWidth()*fScale)/2 + image.GetWidth()*fScale, \
		(rcDrawArea.Height()-image.GetHeight()*fScale)/2 + image.GetHeight()*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	//g.FillRectangle(&SolidBrush(Color(255, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());
	//dcMem.FillSolidRect(rcPanel, RGB(255, 0, 0));
	dcMem.FillSolidRect(rcDrawArea, RGB(255, 255, 255));
	dcMem.StretchBlt(rcPanel.left, rcPanel.top, rcPanel.Width(), rcPanel.Height(), &dcImage, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);


	dcImage.Detach();
	image.ReleaseDC();
	image.Destroy();

	//SaveBitmapToFile(&bmpMem, "C:\\Users\\Administrator\\Desktop\\Tmp.bmp");
	DeleteFile(strFilePath);
	SaveBitmapToFile(&bmpMem, strFilePath);

	dcMem.DeleteDC();
	bmpMem.DeleteObject();

	return true;
}

bool SaveBitmapToFile(CBitmap* bitmap, LPCTSTR lpFileName)  
{              
	HBITMAP hBitmap;    // 为刚才的屏幕位图句柄  
	HDC hDC; //设备描述表    
	int iBits; //当前显示分辨率下每个像素所占字节数    
	WORD wBitCount; //位图中每个像素所占字节数    
	DWORD dwPaletteSize = 0; //定义调色板大小  
	DWORD dwBmBitsSize;  //位图中像素字节大小     
	DWORD dwDIBSize;   //位图文件大小  
	DWORD dwWritten;  //写入文件字节数  
	BITMAP Bitmap; //位图属性结构    
	BITMAPFILEHEADER bmfHdr; //位图文件头结构    
	BITMAPINFOHEADER bi; //位图信息头结构      
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构    
	HANDLE fh;   //定义文件  
	HANDLE hDib;    //分配内存句柄  
	HANDLE hPal;   //调色板句柄  
	HANDLE hOldPal = NULL;  
		      
	//计算位图文件每个像素所占字节数    
	hBitmap = (HBITMAP)*bitmap;    
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);    
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);    
	DeleteDC(hDC);  
		      
	if (iBits <= 1)    
	    wBitCount = 1;    
	else if (iBits <= 4)    
	    wBitCount = 4;    
	else if (iBits <= 8)    
	    wBitCount = 8;    
	else if (iBits <= 24)    
	    wBitCount = 24;    
	else if (iBits <= 32)  
	    wBitCount = 32;  
		      
	//计算调色板大小    
	if (wBitCount <= 8)    
		dwPaletteSize = (1 << wBitCount) * sizeof (RGBQUAD);  
		      
	//设置位图信息头结构    
	GetObject(hBitmap, sizeof (BITMAP), (LPSTR)&Bitmap);  
	bi.biSize = sizeof (BITMAPINFOHEADER);    
	bi.biWidth = Bitmap.bmWidth;    
	bi.biHeight = Bitmap.bmHeight;    
	bi.biPlanes = 1;      
	bi.biBitCount = wBitCount;    
	bi.biCompression = BI_RGB;    
	bi.biSizeImage = 0;    
	bi.biXPelsPerMeter = 0;    
	bi.biYPelsPerMeter = 0;    
	bi.biClrUsed = 0;    
	bi.biClrImportant = 0;    
		      
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31) / 32) * 4 * Bitmap.bmHeight;    
	  
	//为位图内容分配内存    
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));    
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);    
	*lpbi = bi;  
	  
	// 处理调色板         
	hPal = GetStockObject(DEFAULT_PALETTE);    
	if (hPal)    
	{    
	    hDC = ::GetDC(NULL);      
	    hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);    
	    RealizePalette(hDC);  
	}    
	
	//   获取该调色板下新的像素值    
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,    \
	    (LPSTR)lpbi + sizeof (BITMAPINFOHEADER) + dwPaletteSize,  \
	   (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);    
	
	//恢复调色板       
	if (hOldPal)      
	{    
	    SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);    
	    RealizePalette(hDC);    
	    ::ReleaseDC(NULL, hDC);  
	 }    
	
	//创建位图文件              
		fh = CreateFile(lpFileName, GENERIC_WRITE,  \
		0, NULL, CREATE_ALWAYS,  \
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);    

	if (fh == INVALID_HANDLE_VALUE)  
		return FALSE;    

	//   设置位图文件头    
	bmfHdr.bfType = 0x4D42;     //   "BM"    
	dwDIBSize = sizeof (BITMAPFILEHEADER)       \
	        + sizeof (BITMAPINFOHEADER)    \
	        + dwPaletteSize + dwBmBitsSize;      
	bmfHdr.bfSize = dwDIBSize;    
	bmfHdr.bfReserved1 = 0;    
	bmfHdr.bfReserved2 = 0;    
	bmfHdr.bfOffBits = (DWORD)sizeof (BITMAPFILEHEADER)     \
	        + (DWORD)sizeof (BITMAPINFOHEADER)    \
	        + dwPaletteSize;      
	  
	//   写入位图文件头    
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);    
	
	//   写入位图文件其余内容    
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,  \
	    &dwWritten, NULL);  
	  
	//清除            
	GlobalUnlock(hDib);    
	GlobalFree(hDib);    
	CloseHandle(fh);    
	
	return TRUE;      
}    

BOOL CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	BOOL bSuccess = FALSE;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? TRUE : FALSE;    
	}

	return bSuccess;
}

BOOL DeleteFilesInDirectory(LPCTSTR szDirName, BOOL bDeleteRootDir) 
{ 
    CFileFind tempFind; 
	CString strFileFind;
	CString strDir = szDirName;
	if(strDir.GetAt(strDir.GetLength()-1) != _T('\\'))
		strDir += _T("\\");
	strFileFind.Format(_T("%s*.*"), strDir); 
    BOOL IsFinded = tempFind.FindFile(strFileFind); 
    while (IsFinded) 
    { 
        IsFinded = tempFind.FindNextFile(); 
        if (!tempFind.IsDots())
        { 

            if (tempFind.IsDirectory()) 
            { 
				CString strFindedDir = strDir + tempFind.GetFileName() + _T("\\");
                DeleteFilesInDirectory(strFindedDir, TRUE); 
            } 
            else
            { 
				CString strFindedFile = strDir + tempFind.GetFileName();
                DeleteFile(strFindedFile); 
            } 
        } 
    } 
    tempFind.Close(); 
    if(bDeleteRootDir) 
    { 
		RemoveDirectory(strDir);
    } 
    return TRUE; 
}

vector<CString> SplitStringByChar(CString& strToSplit, TCHAR cChar)
{
	//USES_CONVERSION;
	vector<CString> vRet;

	bool bFinish = false;
	int nPos = -1;
	int nPos1 = -1;
	CString strSplitted;

	while(!bFinish)
	{
		nPos1 = strToSplit.Find(cChar, nPos+1);
		if(nPos1 == -1)
			bFinish = true;
		else
		{
			strSplitted = strToSplit.Mid(nPos+1, nPos1-nPos-1);
			vRet.push_back(strSplitted);
			nPos = nPos1;
		}
	}

	strSplitted = strToSplit.Mid(nPos+1, strToSplit.GetLength()-(nPos+1));
	vRet.push_back(strSplitted);

	return vRet;
}

float GetPointAndLineSegmentDistance(float fPointX, float fPointY, float fLineStartX, float fLineStartY, float fLineEndX, float fLineEndY)
{
	float fNearPointX = 0.0, fNearPointY = 0.0;
	if(IsFloatEqual(fLineStartX, fLineEndX) && IsFloatEqual(fLineStartY, fLineEndY))
	{
		fNearPointX = fLineStartX;
		fNearPointY = fLineStartY;
	}
	else if(IsFloatEqual(fLineStartX, fLineEndX))
	{
		fNearPointX = fLineStartX;

		float fMinYInLineSegment = fLineStartY > fLineEndY ? fLineEndY : fLineStartY;
		float fMaxYInLineSegment = fLineStartY > fLineEndY ? fLineStartY : fLineEndY;
		if(fPointY > fMinYInLineSegment && fPointY < fMaxYInLineSegment)
			fNearPointY = fPointY;
		else
		{
			if(fabs(fPointY - fLineStartY) > fabs(fPointY - fLineEndY))
				fNearPointY = fLineEndY;
			else
				fNearPointY = fLineStartY;
		}
	}
	else if(IsFloatEqual(fLineStartY, fLineEndY))
	{
		fNearPointY = fLineStartY;

		float fMinXInLineSegment = fLineStartX > fLineEndX ? fLineEndX : fLineStartX;
		float fMaxXInLineSegment = fLineStartX > fLineEndX ? fLineStartX : fLineEndX;
		if(fPointX > fMinXInLineSegment && fPointX < fMaxXInLineSegment)
			fNearPointX = fPointX;
		else
		{
			if(fabs(fPointX - fLineStartX) > fabs(fPointX - fLineEndX))
				fNearPointX = fLineEndX;
			else
				fNearPointX = fLineStartX;
		}
	}
	else
	{
		float fK = (fLineEndY - fLineStartY) / (fLineEndX - fLineStartX);
		float fCrossPointX = (pow(fK, 2)*fLineStartX+fK*(fPointY-fLineStartY)+fPointX)/(pow(fK, 2)+1);
		float fCrossPointY = fK*(fCrossPointX-fLineStartX)+fLineStartY;

		float fLineSegmentLen = DistanceBetweenPoints(fLineStartX, fLineStartY, fLineEndX, fLineEndY);
		if(IsFloatEqual(DistanceBetweenPoints(fCrossPointX, fCrossPointY, fLineStartX, fLineStartY)+DistanceBetweenPoints(fCrossPointX, fCrossPointY, fLineEndX, fLineEndY), fLineSegmentLen))
		{
			fNearPointX = fCrossPointX;
			fNearPointY = fCrossPointY;
		}
		else
		{
			if(DistanceBetweenPoints(fPointX, fPointY, fLineStartX, fLineStartY) > DistanceBetweenPoints(fPointX, fPointY, fLineEndX, fLineEndY))
			{
				fNearPointX = fLineEndX;
				fNearPointY = fLineEndY;
			}
			else
			{
				fNearPointX = fLineStartX;
				fNearPointY = fLineStartY;
			}
		}
	}

	return DistanceBetweenPoints(fPointX, fPointY, fNearPointX, fNearPointY);
}

void ImageRotate180(CString strFileFullPath)
{
	USES_CONVERSION;

	if(!PathFileExists(strFileFullPath))
		return;

	Bitmap* pBmpSrc = Bitmap::FromFile(A2W(strFileFullPath));
	CImage imgSrc, imgDst;
	//imgSrc.Load(strFileFullPath);
	imgDst.Create(pBmpSrc->GetWidth(), pBmpSrc->GetHeight(), ILC_COLOR32);
	CDC dcImage;
	dcImage.Attach(imgDst.GetDC());
	Graphics g(dcImage.m_hDC);
	
	int nTmp = ((float)pBmpSrc->GetWidth())/2;
	int nTmp1 = ((float)pBmpSrc->GetHeight())/2;
	g.TranslateTransform(nTmp, nTmp1);
	g.RotateTransform(180);
	g.TranslateTransform(-nTmp, -nTmp1);
	g.DrawImage(pBmpSrc, RectF(0, 0, pBmpSrc->GetWidth(), pBmpSrc->GetHeight()), 0, 0, pBmpSrc->GetWidth(), pBmpSrc->GetHeight(), UnitPixel);
	g.ResetTransform();

	delete pBmpSrc;
	dcImage.Detach();
	imgDst.ReleaseDC();
	CString strFileFullPath_c;
	imgDst.Save(strFileFullPath);
	imgDst.Destroy();


}

CString GetTimeString()
{
	CString strRet;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strRet.Format(_T("%4d-%2d-%2d %2d-%2d-%2d"), sysTime.wYear,sysTime.wMonth,sysTime.wDay, sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	return strRet;
}

wstring UTF8ToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset((void*)pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring wstrReturn(pUnicode);
	delete [] pUnicode;
	return wstrReturn;
}

string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	string strReturn(pElementText);
	delete [] pElementText;
	return strReturn;
}

wstring AnsiToUnicode(const char* szStr)
{
	int nLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen+1];
	::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	wstring strReturn(pResult);
	delete[] pResult;
	return strReturn;
}

std::string UnicodeToAnsi(const wchar_t* lpwcszWString)
{
	int nLen = ::WideCharToMultiByte(CP_ACP, NULL, lpwcszWString, /*wcslen(lpwcszWString)*/-1, NULL, 0, NULL, NULL); 
	if (nLen == 0)
	{
		return NULL;
	}
	char* szAnsi = new char[nLen+1]; 
	::WideCharToMultiByte(CP_ACP, NULL, lpwcszWString, /*wcslen(wszString)*/-1, szAnsi, nLen, NULL, NULL); 
	//szAnsi[ansiLen] = '\0'; 
	string strReturn(szAnsi);
	delete[] szAnsi; 
	szAnsi =NULL; 
	return strReturn;
}


XmlHandler::XmlHandler()
	: m_eEncodingType(ENCODING_GB2312)
	, m_pDoc(NULL)
{

}

XmlHandler::~XmlHandler()
{
	delete m_pDoc;
}

XmlHandler::EncodingType XmlHandler::GenEncodingType()
{
	return m_eEncodingType;
}

TiXmlDocument* XmlHandler::GetDoc()
{
	return m_pDoc;
}

bool XmlHandler::LoadFile(LPCTSTR szFileDir)
{
	USES_CONVERSION;

	BOOL bDelete = FALSE;
	CString strXMLFile = szFileDir;
	CString strFirDir = szFileDir;

	if(strFirDir.Right(strFirDir.GetLength()-strFirDir.ReverseFind('.')-1).CompareNoCase("xml") != 0)
	{
		bDelete = TRUE;






		strFirDir.ReleaseBuffer();
		strXMLFile.ReleaseBuffer();
	}

	m_pDoc = new TiXmlDocument(strXMLFile.GetBuffer());
	strXMLFile.ReleaseBuffer();
	BOOL bLoadSuccess = m_pDoc->LoadFile();
	if(bDelete)
		DeleteFile(strXMLFile);
	if(!bLoadSuccess)
	{
		delete m_pDoc;
		m_pDoc = NULL;
		return false;
	}

	strcpy(m_szFileDir, strXMLFile.GetBuffer());
	strXMLFile.ReleaseBuffer();

	m_pDoc->SetCondenseWhiteSpace(false);

	TiXmlDeclaration* elmDecl = m_pDoc->FirstChild()->ToDeclaration();
	if(elmDecl)
	{
		if(strcmp(elmDecl->Encoding(), "gb2312") == 0 || strcmp(elmDecl->Encoding(), "GB2312") == 0)
			m_eEncodingType = ENCODING_GB2312;
		else if(strcmp(elmDecl->Encoding(), "utf-8") == 0 || strcmp(elmDecl->Encoding(), "UTF-8") == 0)
			m_eEncodingType = ENCODING_UTF8;
	}

	return true;
}

bool XmlHandler::SaveFile()
{
	if(m_pDoc)
		return m_pDoc->SaveFile(m_szFileDir);

	return false;
}

bool XmlHandler::GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, bool& bRet)
{
	bool bFind = false;
	if(elm != NULL)
	{
		CStringA strTmp;
		if(GetXmlAttribute_str(elm, szAttributeName, strTmp))
		{
			bRet = (atoi(strTmp) == 0) ? false : true;
			bFind = true;
		}
	}

	return bFind;
}

bool XmlHandler::GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, float& fRet)
{
	bool bFind = false;
	if(elm != NULL)
	{
		CStringA strTmp;
		if(GetXmlAttribute_str(elm, szAttributeName, strTmp))
		{
			fRet = atof(strTmp);
			bFind = true;
		}
	}

	return bFind;
}

bool XmlHandler::GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, int& nRet)
{
	bool bFind = false;
	if(elm != NULL)
	{
		CStringA strTmp;
		if(GetXmlAttribute_str(elm, szAttributeName, strTmp))
		{
			nRet = atoi(strTmp);
			bFind = true;
		}
	}

	return bFind;
}

bool XmlHandler::GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, CString& strRet)
{
	bool bFind = false;
	if(elm != NULL)
	{
		CStringA strTmp;
		if(GetXmlAttribute_str(elm, szAttributeName, strTmp))
		{
			if(m_eEncodingType == ENCODING_UTF8)
			{
				strRet = UnicodeToAnsi(UTF8ToWString(strTmp.GetBuffer()).c_str()).c_str();
				strTmp.ReleaseBuffer();
			}
			else
			{
				strRet = strTmp;
			}

			bFind = true;
		}
	}

	return bFind;
}

bool XmlHandler::GetXmlAttribute_str(TiXmlElement* elm, LPCSTR szAttributeName, CStringA& strRet)
{
	if(!elm)
		return false;

	bool bFind = false;
	TiXmlAttribute *attr = NULL;
	attr = elm->FirstAttribute();
	while(attr)
	{
		//CStringA strTmp = attr->Name();
		if(strcmp(attr->Name(), szAttributeName) == 0)
		{
			strRet = attr->Value();//attr->ValueStr();
			//strRet = attr->ValueStr().c_str();
			bFind = true;
			break;
		}
		attr = attr->Next();  
	}
	return bFind;
}

TiXmlElement* XmlHandler::GetIndicatedElement(TiXmlElement* elmRoot, char* szElementLabel, char* szAttribute, const char* szIndicatedAttributeValue)
{
	USES_CONVERSION;

	if(!elmRoot)
		return NULL;

	TiXmlElement* pElmRet = NULL;
	for(TiXmlElement* elm = elmRoot->FirstChildElement(szElementLabel); elm != NULL; elm = elm->NextSiblingElement(szElementLabel))
	{
		BOOL bSameString = FALSE;
		if(m_eEncodingType != ENCODING_UTF8)
		{
			string strTheComparingAttrValue = elm->Attribute(szAttribute);
			bSameString = (strcmp(elm->Attribute(szAttribute), szIndicatedAttributeValue) == 0) ? TRUE : FALSE;
		}
		else
		{
			//CStringA strA = elm->Attribute(szAttribute);
			//CString strW = UTF8ToWString(elm->Attribute(szAttribute)).c_str();
			wstring wsStr = UTF8ToWString(elm->Attribute(szAttribute)).c_str();
			bSameString = (strcmp(UnicodeToAnsi(wsStr.c_str()).c_str(), szIndicatedAttributeValue) == 0) ? TRUE : FALSE;
		}

		//CStringA strA = WStringToMBytes(/*UTF8ToWString(elm->Attribute(szAttribute)).c_str()*/strW.GetBuffer()).c_str();

		if(bSameString)
		{
			pElmRet = elm;
			break;
		}
	}
	return pElmRet;
}

bool XmlHandler::SetIndicatedElmAttr(TiXmlElement* elmRoot, char* szElementLabel, char* szAttributeToFind, const char* szValueToFind, const char* szAttrToSet, const char* szValueToSet)
{
	TiXmlElement* pElmRet = GetIndicatedElement(elmRoot, szElementLabel, szAttributeToFind, szValueToFind);
	if(!pElmRet)
	{
		pElmRet = new TiXmlElement(szElementLabel);
		elmRoot->LinkEndChild(pElmRet);
		pElmRet->SetAttribute(szAttributeToFind, szValueToFind);
	}
	pElmRet->SetAttribute(szAttrToSet, szValueToSet);

	return true;
}

bool XmlHandler::SetElmAttr(TiXmlElement* elmRoot, char* szElementLabel, const char* szAttrToSet, const char* szValueToSet)
{
	if(!elmRoot)
		return false;

	TiXmlElement* pElmRet = GetChildElm(elmRoot, szElementLabel);
	if(!pElmRet)
	{
		pElmRet = new TiXmlElement(szElementLabel);
		elmRoot->LinkEndChild(pElmRet);
	}
	pElmRet->SetAttribute(szAttrToSet, szValueToSet);

	return true;
}

TiXmlElement* XmlHandler::GetChildElm(TiXmlElement* elm, LPCSTR szChildElm)
{
	if(elm == NULL)
		return NULL;

	CString strChildElm = szChildElm;
	if(strChildElm.IsEmpty())
		return elm->FirstChildElement();
	else
		return elm->FirstChildElement(szChildElm);
}

bool XmlHandler::LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, bool& bRet)
{
	return GetXmlAttribute(GetChildElm(elm, szChildElm), szAttributeName, bRet);
}

bool XmlHandler::LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, float& fRet)
{
	return GetXmlAttribute(GetChildElm(elm, szChildElm), szAttributeName, fRet);
}

bool XmlHandler::LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, int& nRet)
{
	return GetXmlAttribute(GetChildElm(elm, szChildElm), szAttributeName, nRet);
}

bool XmlHandler::LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, CString& strRet)
{
	return GetXmlAttribute(GetChildElm(elm, szChildElm), szAttributeName, strRet);
}

vector<TiXmlElement*> XmlHandler::GetChildElms(TiXmlElement* elm, LPCSTR szChildElm)
{
	vector<TiXmlElement*> vRes;
	if(elm != NULL)
	{
		CStringA strChildElm;
		if(szChildElm != NULL)
			strChildElm = szChildElm;
		bool bGetAllChild = strChildElm.IsEmpty();
		for(TiXmlElement* elmChild = (bGetAllChild ? elm->FirstChildElement() : elm->FirstChildElement(szChildElm)); elmChild != NULL; elmChild = (bGetAllChild ? elmChild->NextSiblingElement() : elmChild->NextSiblingElement(szChildElm)))
			vRes.push_back(elmChild);
	}
	return vRes;
}

CoordinateHandler::CoordinateHandler(PositionLRType eOriginLRPos, PositionFBType eOriginFBPos, PositionTBType eOriginTBPos, \
	TCHAR chLR, TCHAR chFB, TCHAR chTB, \
	PlusMinusType eLRPlusMinusType, PlusMinusType eFBPlusMinusType, PlusMinusType eTBPlusMinusType)
	:m_eOriginLRPos(eOriginLRPos)
	,m_eOriginFBPos(eOriginFBPos)
	,m_eOriginTBPos(eOriginTBPos)
	,m_chLR(chLR)
	,m_chFB(chFB)
	,m_chTB(chTB)
	,m_eLRPlusMinusType(eLRPlusMinusType)
	,m_eFBPlusMinusType(eFBPlusMinusType)
	,m_eTBPlusMinusType(eTBPlusMinusType)
{

}

void CoordinateHandler::GetConvertedValue(float fPanelLRLen, float fPanelFBLen, float fPanelTBLen, float fSourceLRValue, float fSourceFBValue, float fSourceTBValue, \
	float* pfDesLRValue, float* pfDesFBValue, float* pfDesTBValue)
{
	if(pfDesLRValue != NULL)
	{
		float fTmp;
		if(m_eOriginLRPos == LEFT)
			fTmp = fSourceLRValue;
		else
			fTmp = fPanelLRLen - fSourceLRValue;

		if(m_eLRPlusMinusType == MINUS)
			fTmp = -fTmp;
		*pfDesLRValue = fTmp;
	}

	if(pfDesFBValue != NULL)
	{
		float fTmp;
		if(m_eOriginFBPos == FRONT)
			fTmp = fSourceFBValue;
		else
			fTmp = fPanelFBLen - fSourceFBValue;

		if(m_eFBPlusMinusType == MINUS)
			fTmp = -fTmp;
		*pfDesFBValue = fTmp;
	}

	if(pfDesTBValue != NULL)
	{
		float fTmp;
		if(m_eOriginTBPos == TOP)
			fTmp = fSourceTBValue;
		else
			fTmp = fPanelTBLen - fSourceTBValue;

		if(m_eTBPlusMinusType == MINUS)
			fTmp = -fTmp;
		*pfDesTBValue = IsFloatEqual(fTmp, 0) ? 0 : fTmp;
	}
}

TCHAR CoordinateHandler::GetPositionChar(PositionType ePositionType)
{
	TCHAR ch;
	switch(ePositionType)
	{
	case LEFT_RIGHT:
		ch = m_chLR;
		break;
	case FRONT_BACK:
		ch = m_chFB;
		break;
	case TOP_BOTTOM:
		ch = m_chTB;
		break;
	default:
		break;
	}

	return ch;
}

void CoordinateHandler::GetConvertedCoordinate(float fPanelLRLen, float fPanelFBLen, float fPanelTBLen, float fSourceLRValue, float fSourceFBValue, float fSourceTBValue, \
	float* pfDesXValue, float* pfDesYValue, float* pfDesZValue)
{
	float afDesValue[3];
	GetConvertedValue(fPanelLRLen, fPanelFBLen, fPanelTBLen, fSourceLRValue, fSourceFBValue, fSourceTBValue, \
		afDesValue, &afDesValue[1], &afDesValue[2]);

	float* apfDestXYZValue[3];
	apfDestXYZValue[0] = pfDesXValue;
	apfDestXYZValue[1] = pfDesYValue;
	apfDestXYZValue[2] = pfDesZValue;

	TCHAR acPosChar[3];
	acPosChar[0] = m_chLR;
	acPosChar[1] = m_chFB;
	acPosChar[2] = m_chTB;

	TCHAR acXYZChar[3] = {L'X', L'Y', L'Z'};
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(acPosChar[i] == acXYZChar[j])
			{
				if(apfDestXYZValue[j] != NULL)
					*(apfDestXYZValue[j]) = afDesValue[i];
			}
		}
	}
}

int CoordinateHandler::GetOriginPos(PositionType ePositionType)
{
	int nRet;
	switch(ePositionType)
	{
	case LEFT_RIGHT:
		nRet = m_eOriginLRPos;
		break;
	case FRONT_BACK:
		nRet = m_eOriginFBPos;
		break;
	case TOP_BOTTOM:
		nRet = m_eOriginTBPos;
		break;
	}

	return nRet;
}


Normalizer::Normalizer(int nValueCount, float* paValuePower)
{
	m_nValueCount = nValueCount;
	m_afValuePower = new float[m_nValueCount];
	memcpy(m_afValuePower, paValuePower, sizeof(float)*m_nValueCount);
}

Normalizer::~Normalizer()
{
	delete[] m_afValuePower;
	for(int i = 0; i < m_vItemValue.size(); i++)
	{
		float* aValue = m_vItemValue[i];
		delete[] aValue;
	}
}

void Normalizer::InputItem(float* paItemValue)
{
	float* aItamValue = new float[m_nValueCount];
	memcpy(aItamValue, paItemValue, sizeof(float)*m_nValueCount);
	m_vItemValue.push_back(aItamValue);
}

vector<float> Normalizer::GetNormalizeResult()
{
	vector<float> vRet;
	
	//for(int i = 0; i < m_vItemValue.size(); i++)
	//{
	//	CString strLine;
	//	for(int j = 0; j < pInput->m_nValueCount; j++)
	//	{
	//		float fValue = pInput->m_afItemsValues[i*pInput->m_nValueCount + j];
	//		CString str;
	//		str.Format("%f ", fValue);
	//		strLine += str;
	//	}
	//	strLine += "\n";
	//	//OutputDebugString(strLine);
	//}



	float* afItemsNmlValues = new float[m_vItemValue.size() * m_nValueCount];
	for(int i = 0; i < m_nValueCount; i++)
	{
		float fMaxValue = -FLT_MAX;
		for(int j = 0; j < m_vItemValue.size(); j++)
		{
			if(/*pInput->m_afItemsValues[j*pInput->m_nValueCount+i]*/m_vItemValue[j][i] > fMaxValue)
				fMaxValue = /*pInput->m_afItemsValues[j*pInput->m_nValueCount+i]*/m_vItemValue[j][i];
		}

		for(int j = 0; j < /*pInput->m_nItemCount*/m_vItemValue.size(); j++)
		{
			if(fMaxValue <= 0)
				/*afItemsNmlValues[j*pInput->m_nValueCount+i]*/afItemsNmlValues[j*m_nValueCount+i] = 1;
			else
				/*afItemsNmlValues[j*pInput->m_nValueCount+i]*/afItemsNmlValues[j*m_nValueCount+i] = /*pInput->m_afItemsValues[j*pInput->m_nValueCount+i]*/m_vItemValue[j][i] / fMaxValue;
		}
	}

	for(int i = 0; i < /*pInput->m_nItemCount*/ m_vItemValue.size(); i++)
	{
		CString strLine;

		float fTotal = 0.0;
		for(int j = 0; j < /*pInput->m_nValueCount*/m_nValueCount; j++)
		{
			fTotal += afItemsNmlValues[i*m_nValueCount+j] * m_afValuePower[j];
			CString strTmp;
			//strTmp.Format("%f ", afItemsNmlValues[i*pInput->m_nValueCount+j] * pInput->m_aValuePower[j]);
			strLine += strTmp;
		}
		vRet.push_back(fTotal * 100);
		//pRet[i] = fTotal * 100;

		strLine += "\n";
		//OutputDebugString(strLine);
	}


	delete afItemsNmlValues;

	return vRet;
}
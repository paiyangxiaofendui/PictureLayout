#pragma once

#include <string>
#include <vector>
#include "../../../include/TinyXml/tinyxml.h"

using namespace TinyXml;
using namespace std;

CString GetModulePath();

CString GetFloatString(float fNum, UCHAR uDecimals);

CString GetFloatStringTrimming(float fNum, UCHAR uDecimals);

CString GetIntegerString(int nNum, UCHAR uBitCount = 0);

bool IsFloatEqual(float f1, float f2, float fPrecision = 0.01);

float GetScale(float fPanelWidth, float fPanelHeight, float fDrawAreaWidth, float fDrawAreaHeight);

float GetPointDistance(float fX1, float fY1, float fX2, float fY2);

Image* LoadPngImgFromRes(UINT uResID);

CString GetFileNameInPath(CString strPath);

CString GetFileNameInPath_WithSuffix(CString strPath);

bool MakeSureFolderExist(CString strPath);

bool IsKeyPressed(UINT nKeyID);

float DistanceBetweenPoints(float fX1, float fY1, float fX2, float fY2);

float GetLineAngle(float fX1, float fY1, float fX2, float fY2);

bool GetArcMidPoint(float fX1, float fY1, float fX2, float fY2, float fR, int nDir, float& fXMid, float& fYMid);

bool GetArcMidPoint_ex(float fX1, float fY1, float fX2, float fY2, float fR, int nDir, float& fXMid, float& fYMid);

bool GetCircleCenterByStartEndRD(float fXStart, float fYStart, float fXEnd, float fYEnd, float fR, int nDir, float& fXCenter, float& fYCenter, float& fSin, float& fCos);

void GetCircleCenterByThreePoint(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3, float& fXCenter, float& fYCenter);

int GetArcDirByPoint(float fX1, float fY1, float fX2, float fY2, float fCenterX, float fCenterY);

void RotatePoint(float fOrgX, float fOrgY, float fCenterX, float fCenterY, float fClockwiseRotateAng, float& fRotatedX, float& fRotatedY);

int GetRegularAngle(int nAngle);

float GetPointAndLineSegmentDistance(float fPointX, float fPointY, float fLineStartX, float fLineStartY, float fLineEndX, float fLineEndY);

bool ConvertBmpToOneBitBmp(CString strFilePath);

bool RescaleBmp(CString strFilePath, int nTargetWidth, int nTargetHeight);

BOOL CreateMultipleDirectory(const CString& szPath);

BOOL DeleteFilesInDirectory(LPCTSTR szDirName, BOOL bDeleteRootDir = FALSE);

vector<CString> SplitStringByChar(CString& strToSplit, TCHAR cChar);

void ImageRotate180(CString strFileFullPath);

CString GetTimeString();

std::string WStringToUTF8(const wchar_t* lpwcszWString);
std::wstring UTF8ToWString(const char* lpcszString);
std::wstring AnsiToUnicode(const char* szStr);
std::string UnicodeToAnsi(const wchar_t* lpwcszWString);

class XmlHandler
{
public:
	enum EncodingType{ENCODING_UTF8, ENCODING_GB2312};

	XmlHandler();
	~XmlHandler();

	bool LoadFile(LPCTSTR szFileDir);
	bool SaveFile();

	TiXmlDocument* GetDoc();
	EncodingType GenEncodingType();

	bool GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, bool& bRet);
	bool GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, float& fRet);
	bool GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, int& nRet);
	bool GetXmlAttribute(TiXmlElement* elm, LPCSTR szAttributeName, CString& strRet);
	TiXmlElement* GetIndicatedElement(TiXmlElement* elmRoot, char* szElementLabel, char* szAttribute, const char* szIndicatedAttributeValue);
	bool LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, bool& bRet);
	bool LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, float& fRet);
	bool LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, int& nRet);
	bool LocateChildElmAndGetAttr(TiXmlElement* elm, LPCSTR szChildElm, LPCSTR szAttributeName, CString& strRet);

	TiXmlElement* GetChildElm(TiXmlElement* elm, LPCSTR szChildElm);
	vector<TiXmlElement*> GetChildElms(TiXmlElement* elm, LPCSTR szChildElm = NULL);

	bool SetIndicatedElmAttr(TiXmlElement* elmRoot, char* szElementLabel, char* szAttributeToFind, const char* szValueToFind, const char* szAttrToSet, const char* szValueToSet);
	bool SetElmAttr(TiXmlElement* elmRoot, char* szElementLabel, const char* szAttrToSet, const char* szValueToSet);
protected:
	bool GetXmlAttribute_str(TiXmlElement* elm, LPCSTR szAttributeName, CStringA& strRet);

	EncodingType m_eEncodingType;
	TiXmlDocument* m_pDoc;
	char m_szFileDir[MAX_PATH];
};

class CoordinateHandler
{
public:
	enum PositionType{LEFT_RIGHT, FRONT_BACK, TOP_BOTTOM};
	enum PositionLRType{LEFT, RIGHT};
	enum PositionFBType{FRONT, BACK};
	enum PositionTBType{TOP, BOTTOM};
	enum PlusMinusType{PLUS, MINUS};

	CoordinateHandler(PositionLRType eOriginLRPos = LEFT, PositionFBType eOriginFBPos = BACK, PositionTBType eOriginTBPos = TOP, \
		TCHAR chLR = L'X', TCHAR chFB = L'Y', TCHAR chTB = L'Z', \
		PlusMinusType eLRPlusMinusType = PLUS, PlusMinusType eFBPlusMinusType = PLUS, PlusMinusType eTBPlusMinusType = PLUS);

	void GetConvertedCoordinate(float fPanelLRLen, float fPanelFBLen, float fPanelTBLen, float fSourceLRValue, float fSourceFBValue, float fSourceTBValue, \
		float* pfDesXValue, float* pfDesYValue, float* pfDesZValue);
	void GetConvertedValue(float fPanelLRLen, float fPanelFBLen, float fPanelTBLen, float fSourceLRValue, float fSourceFBValue, float fSourceTBValue, \
		float* pfDesLRValue, float* pfDesFBValue, float* pfDesTBValue);
	TCHAR GetPositionChar(PositionType ePositionType);
	int GetOriginPos(PositionType ePositionType);
protected:
	PositionLRType m_eOriginLRPos;
	PositionFBType m_eOriginFBPos;
	PositionTBType m_eOriginTBPos;
	TCHAR m_chLR;
	TCHAR m_chFB;
	TCHAR m_chTB;
	PlusMinusType m_eLRPlusMinusType;
	PlusMinusType m_eFBPlusMinusType;
	PlusMinusType m_eTBPlusMinusType;
};

class Normalizer
{
public:
	Normalizer(int nValueCount, float* paValuePower);
	~Normalizer();

	void InputItem(float* paItemValue);

	vector<float> GetNormalizeResult();

protected:
	int m_nValueCount;
	float* m_afValuePower;
	vector<float*> m_vItemValue;
};
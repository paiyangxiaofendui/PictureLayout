#pragma once

#include <string>
#include <vector>
#include "../../../include/TinyXml/tinyxml.h"

using namespace TinyXml;
using namespace std;

CString GetModulePath();

CString GetFloatString(float fNum, UCHAR uDecimals);

CString GetFloatStringTrimming(float fNum, UCHAR uDecimals);

bool IsFloatEqual(float f1, float f2);

float GetScale(float fPanelWidth, float fPanelHeight, float fDrawAreaWidth, float fDrawAreaHeight);

float GetPointDistance(float fX1, float fY1, float fX2, float fY2);

BOOL SpyBandingString(CString& strBanding, float (&afBanding)[4]);

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
protected:
	bool GetXmlAttribute_str(TiXmlElement* elm, LPCSTR szAttributeName, CStringA& strRet);

	EncodingType m_eEncodingType;
	TiXmlDocument* m_pDoc;
	char m_szFileDir[MAX_PATH];
};
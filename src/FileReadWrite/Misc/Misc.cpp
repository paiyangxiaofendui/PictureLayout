#include "stdafx.h"
#include "Misc.h"
#include <math.h>
#include "HGTools.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "EncryptionInterface.h"


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

bool IsFloatEqual(float f1, float f2)
{
	if(fabs(f1 - f2) <= 0.01)
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

BOOL SpyBandingString(CString& strBanding, float (&afBanding)[4])
{
	USES_CONVERSION;
	int nPos = 0;
	int nPos1 = strBanding.Find(" ") - 1;
	for(int i = 0; i < 4; i++)
	{
		CString strTmp = strBanding.Mid(nPos, nPos1-nPos+1);
		afBanding[i] = atof(strTmp.GetBuffer());
		strTmp.ReleaseBuffer();
		nPos = nPos1 + 1;
		if(i == 2)
			nPos1 = strBanding.GetLength() - 1;
		else
			nPos1 = strBanding.Find(" ", nPos1+2) - 1;
	}
	return TRUE;
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
		//strXMLFile = A2W(HGTools::getXmlPathFromHgxPath(W2A(strFirDir)));

		strXMLFile = strXMLFile.Left(strXMLFile.ReverseFind('\\')+1) + "1849858.xml";



		// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

		decrypt_base64(strFirDir.GetBuffer(), strXMLFile.GetBuffer());

#else

		//HGTools::decryptFile(strFirDir.GetBuffer(), strXMLFile.GetBuffer());

#endif






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
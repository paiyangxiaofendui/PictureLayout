#include "../stdafx.h"
#include "Misc.h"
//#include <Windows.h>
//#include <winnls.h>

using namespace std;


//wstring UTF8ToWString(const char* lpcszString)
//{
//	int len = strlen(lpcszString);
//	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, lpcszString, -1, NULL, 0);
//	wchar_t* pUnicode;
//	pUnicode = new wchar_t[unicodeLen + 1];
//	memset((void*)pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
//	::MultiByteToWideChar(CP_UTF8, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
//	wstring wstrReturn(pUnicode);
//	delete [] pUnicode;
//	return wstrReturn;
//}

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

//std::string UnicodeToAnsi(const wchar_t* lpwcszWString)
//{
//	int nLen = ::WideCharToMultiByte(CP_ACP, NULL, lpwcszWString, /*wcslen(lpwcszWString)*/-1, NULL, 0, NULL, NULL); 
//	if (nLen == 0)
//	{
//		return NULL;
//	}
//	char* szAnsi = new char[nLen+1]; 
//	::WideCharToMultiByte(CP_ACP, NULL, lpwcszWString, /*wcslen(wszString)*/-1, szAnsi, nLen, NULL, NULL); 
//	//szAnsi[ansiLen] = '\0'; 
//	string strReturn(szAnsi);
//	delete[] szAnsi; 
//	szAnsi =NULL; 
//	return strReturn;
//}
#pragma once
#include <string>
using namespace std;

std::string WStringToUTF8(const wchar_t* lpwcszWString);
//std::wstring UTF8ToWString(const char* lpcszString);
std::wstring AnsiToUnicode(const char* szStr);
//std::string UnicodeToAnsi(const wchar_t* lpwcszWString);


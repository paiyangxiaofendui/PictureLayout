#pragma once

#ifdef _USRDLL
#define EXPORT_IMPORT_TYPE dllexport
#else
#define EXPORT_IMPORT_TYPE dllimport
#endif // _DEBUG

extern "C" __declspec(dllexport) int SendStringToPhone(LPCSTR szPhoneNum, LPCSTR szToSend);
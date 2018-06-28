#pragma once
#include <vector>

using namespace std;

CString GetModulePath();

vector<CString> TraversFile(CString strPath, LPCTSTR szSubfix);

bool CopyTheFileToTarget(CString strSourceFilePath, CString strTargetFilePath);

BOOL CreateMultipleDirectory(const CString& szPath);
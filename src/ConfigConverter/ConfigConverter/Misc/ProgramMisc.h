#pragma once
#include <vector>

using namespace std;

bool ConvertTheFile(CString strFileToConvert);

void PreProcessSourceDir(vector<CString>& vAllSourceDir);

bool IsFiltedFile(CString strFilePath);
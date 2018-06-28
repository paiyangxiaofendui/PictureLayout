#include "stdafx.h"
#include "ProgramMisc.h"
#include <io.h>
#include "FileCompressandUnCompress.h"
#include "DES.h"

bool ConvertTheFile(CString strFileToConvert)
{
	bool bRet = true;

	if(access(strFileToConvert, 0) != 0)
		return false;

	CString strTargetXmlFile;
	int nPos = strFileToConvert.ReverseFind(_T('.'));
	strTargetXmlFile = strFileToConvert.Left(nPos) + _T(".xml");
	File_Decrypt(strFileToConvert.GetBuffer(), strTargetXmlFile.GetBuffer());
	strFileToConvert.ReleaseBuffer();
	strTargetXmlFile.ReleaseBuffer();
	if(access(strTargetXmlFile, 0) != 0)
		return false;

	CString strTargetHgeFile;
	nPos = strTargetXmlFile.ReverseFind(_T('.'));
	strTargetHgeFile = strTargetXmlFile.Left(nPos) + _T(".hge");


	BOOL bRes = TRUE;
	if(access(strTargetHgeFile, 0) == 0)
	{
		bRes = DeleteFile(strTargetHgeFile);
	}
	if(!bRes)
		return false;

	int nRes = func_drawing0(strTargetXmlFile.GetBuffer(), strTargetHgeFile.GetBuffer());
	strTargetXmlFile.ReleaseBuffer();
	strTargetHgeFile.ReleaseBuffer();

	DeleteFile(strTargetXmlFile);
	DeleteFile(strFileToConvert);
	if(nRes != 0)
		return false;

	return bRet;
}

void PreProcessSourceDir(vector<CString>& vAllSourceDir)
{
	for(vector<CString>::iterator itr = vAllSourceDir.begin(); itr != vAllSourceDir.end(); )
	{
		if(IsFiltedFile(*itr))
		{
			vAllSourceDir.erase(itr);
		}
		else
			itr++;
	}
}

bool IsFiltedFile(CString strFilePath)
{
	TCHAR* a_szFiltedFile[] = {_T("Config\\HG3DProduction.hgm"), _T("kdata\\DPanelOutlineList.hgm"), _T("kdata\\mapOutList.hgm"), _T("kdata\\OtherShapePic.hgm")};
	for(int i = 0; i < sizeof(a_szFiltedFile)/sizeof(TCHAR*); i++)
	{
		if(strFilePath.Find(a_szFiltedFile[i]) != -1)
			return true;
	}
	return false;
}
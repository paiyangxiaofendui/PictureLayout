#include "stdafx.h"
#include "Misc.h"
#include <io.h>

CString GetModulePath()
{
	TCHAR cStr[MAX_PATH+1];
	GetModuleFileName(/*GetModuleHandle(0)*/AfxGetModuleState()->m_hCurrentInstanceHandle, cStr, MAX_PATH+1);
	CString str(cStr);
	str = str.Left(str.ReverseFind('\\')+1);
	return str;
}

vector<CString> TraversFile(CString strPath, LPCTSTR szSubfix)
{
	vector<CString> vRet;

	CFileFind find;
	CString strFind = strPath + _T("*.*");//遍历这一级全部的目录
	int nResult = find.FindFile(strFind);
	while(nResult)
	{
		nResult = find.FindNextFile();
		if(find.IsDirectory() && !find.IsDots())
		{
			CString strFolder = find.GetFilePath();
			if(strFolder.GetAt(strFolder.GetLength()-1) != _T('\\'))
				strFolder += _T('\\');
			vector<CString> vTmp = TraversFile(strFolder, szSubfix);
			vRet.insert(vRet.end(), vTmp.begin(), vTmp.end());
		}
		else if(!find.IsDirectory())
		{
			CString strFilePath = find.GetFilePath();
			if(strFilePath.Right(3).CompareNoCase(szSubfix) == 0)
				vRet.push_back(strFilePath);
		}

	}
	return vRet;
}

bool CopyTheFileToTarget(CString strSourceFilePath, CString strTargetFilePath)
{
	bool bRet = true;

	CString strTargetDir;
	int nPos = -1;
	nPos = strTargetFilePath.ReverseFind(_T('\\'));
	strTargetDir = strTargetFilePath.Left(nPos);
	if(access(strTargetDir, 0) != 0)
	{
		if(!CreateMultipleDirectory(strTargetDir))
		{
			CString strTmp;
			strTmp.Format(_T("创建目录%s失败！"), strTargetDir);
			AfxMessageBox(strTmp);
			return false;
		}
	}

	BOOL bRes = TRUE;
	if(access(strTargetFilePath, 0) == 0)
	{
		bRes = DeleteFile(strTargetFilePath);
	}
	if(bRes)
		//bRes = MoveFile(strSourceFilePath, strTargetFilePath);
		bRes = CopyFile(strSourceFilePath, strTargetFilePath, TRUE);

	if(!bRes)
	{
		bRet = false;
	}

	return bRet;
}

BOOL CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//存放要创建的目录字符串

	if(strDir.IsEmpty())
		return FALSE;

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
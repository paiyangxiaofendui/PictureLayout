// CIniFile.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CIniFile.h"


//----------------------------------------------CIniFile.cpp-------------------
CIniFile::CIniFile()
{
}

CIniFile::~CIniFile()
{
	if(m_FileContainer.GetSize()>0)
		m_FileContainer.RemoveAll();
}

void CIniFile::SetFileName(LPCTSTR lpFileName)
{
	m_strFileName = lpFileName;
}

void CIniFile::SetFilePath(LPCTSTR lpFilePath)
{
	m_strFilePath = lpFilePath;
}

BOOL CIniFile::OpenIniFile(void)
{
	BOOL bExist = m_IniFile.Open(m_strFilePath+"\\"+m_strFileName,CFile::modeRead);
	if(bExist==FALSE)
	{
		bExist = m_IniFile.Open(m_strFilePath+"\\"+m_strFileName,CFile::modeCreate|CFile::modeRead);
		m_FileContainer.RemoveAll();
	}
	else
	{
		CString strline;
		m_FileContainer.RemoveAll();
		while(m_IniFile.ReadString(strline))
		{
			m_FileContainer.Add(strline);
		}
	}
	m_IniFile.Close();
	return bExist;
}

BOOL CIniFile::OpenIniFileForRead(void)
{
	BOOL bExist = m_IniFile.Open(m_strFilePath+"\\"+m_strFileName,CFile::modeRead);
	if(bExist)
	{
		CString strline;
		m_FileContainer.RemoveAll();
		while(m_IniFile.ReadString(strline))
		{
			m_FileContainer.Add(strline);
		}
		m_IniFile.Close();
	}
	return bExist;
}

BOOL CIniFile::OpenIniFileForWrite(void)
{
	BOOL bExist=m_IniFile.Open(m_strFilePath+"\\"+m_strFileName,CFile::modeCreate | CFile::modeWrite);
	if(bExist==FALSE) return FALSE;
	CString strline;
	for(WORD i=0;i<m_FileContainer.GetSize();i++)
	{
		strline = m_FileContainer[i];
		m_IniFile.WriteString(strline+"\n");
	}
	m_IniFile.Close();
	return bExist;
}

WORD CIniFile::OnFindSection(LPCTSTR lpAppName)
{
	WORD wFileLines = m_FileContainer.GetSize();
	CString strLine;
	WORD wCnt = 0;
	while(wCnt<wFileLines)
	{
		strLine = m_FileContainer.GetAt(wCnt++);
		if(strLine.IsEmpty())
			return 0;
		if(strLine.GetAt(0)=='[')
		{
			if(strLine.Find(lpAppName)>0)
				return wCnt;
		}
	}
	return 0;
}

BOOL CIniFile::OnFindItem(LPCTSTR lpKeyName,WORD& wLine)
{
	WORD wFileLines = m_FileContainer.GetSize();
	CString strLine;
	while(wLine<wFileLines)
	{
		strLine = m_FileContainer.GetAt(wLine++);
		if(strLine.IsEmpty())
		{
			wLine -= 1;
			return FALSE;
		}
		if(strLine.GetAt(0)=='[')
		{
			wLine -= 1;
			return FALSE;
		}
		strLine = strLine.Left(strLine.Find(_T("=")));
		strLine.TrimLeft();
		strLine.TrimRight();
		if(strLine == lpKeyName)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CIniFile::GetSectionAllItem(LPCTSTR lpAppName,CStringArray&strArray)
{
	WORD wCnt = OnFindSection(lpAppName);
	if(wCnt==0) return FALSE;
	WORD wFileLines = m_FileContainer.GetSize();
	CString strLine;
	while(wCnt<wFileLines)
	{
		strLine = m_FileContainer.GetAt(wCnt++);
		if(strLine.IsEmpty()) break;
		if(strLine.GetAt(0)=='[')
		{
			break;
		}
		strLine.TrimLeft();
		strLine.TrimRight();
		strArray.Add(strLine);
	}
	return TRUE;
}

BOOL CIniFile::GetItemString(LPCTSTR lpAppName,LPCTSTR lpKeyName,CString& strValue)
{
	WORD wCnt = OnFindSection(lpAppName);
	if(wCnt==0) return FALSE;
	if(OnFindItem(lpKeyName,wCnt))
	{
		CString strLine = m_FileContainer[wCnt-1];
		WORD wlen = strLine.GetLength();
		strValue = strLine.Right(wlen-strLine.Find(_T("="))-1);
		strValue.TrimLeft();
		strValue.TrimRight();
		return TRUE;
	}
	return FALSE;
}

void CIniFile::WriteSection(LPCTSTR lpAppName)
{
	CString strSection = _T("[");
	strSection += lpAppName;
	strSection += _T("]");
	m_FileContainer.Add(strSection);
}

void CIniFile::WriteItemString(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpValue)
{
	WORD wCnt = OnFindSection(lpAppName);
	CString strLine;
	strLine = lpKeyName;
	strLine += _T("=");
	strLine += lpValue;
	if(wCnt==0)					//no find section
	{
		WriteSection(lpAppName);
		m_FileContainer.Add(strLine);
		return;
	}
	if(OnFindItem(lpKeyName,wCnt))
	{
		m_FileContainer[wCnt-1] = strLine;
	}
	else
	{
		m_FileContainer.InsertAt(wCnt,strLine);
	}
}

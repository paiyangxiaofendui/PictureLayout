#include "StdAfx.h"
#include "XiaoFengLabelProcess.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"

void XiaoFentLabelProcess(vector<XiaoFengDataItem>& vXiaoFengData, CString strOutputDir)
{
	//USES_CONVERSION;

	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	bool bFindBmp = false;
	CFileFind find;
	CString strFind = strOutputDir + _T("*.*");//遍历这一级全部的目录
	int nResult = find.FindFile(strFind);
	while(nResult)
	{
		nResult = find.FindNextFile();
		if(!find.IsDirectory() && !find.IsDots())
		{
			CString strFilePath = find.GetFilePath();
			if(strFilePath.Right(3).CompareNoCase(_T("bmp")) == 0)
			{
				bFindBmp = true;
				break;
			}
		}
	}
	if(!bFindBmp)
	{
		AfxMessageBox("输出目录下没有板件标签！\n须要先导出标签然后导出NC文件！");
		return;
	}



	for(int i = 0; i < vXiaoFengData.size(); i++)
	{
		vector<CString> vFileToMove;

		XiaoFengDataItem& item = vXiaoFengData[i];
		CString strLabelNCName_XiaoFeng;
		CString strNCName_XiaoFeng;
		//CString strLabelNCFullPath_XiaoFeng;
		strLabelNCName_XiaoFeng.Format(_T("1004#%s#%s.pts"), GetFloatString(item.m_pPanel->m_Thickness, 0), GetIntegerString(i+1, 3));
		strNCName_XiaoFeng.Format(_T("1004#%s#%s.nc"), GetFloatString(item.m_pPanel->m_Thickness, 0), GetIntegerString(i+1, 3));

		CString strLabelNCPath, strNCPath;
		strLabelNCPath = item.m_strLabelNCFileName.Left(item.m_strLabelNCFileName.ReverseFind(_T('\\'))+1);
		rename(item.m_strLabelNCFileName, strLabelNCPath + strLabelNCName_XiaoFeng);
		strNCPath = item.m_strNCFileFullPath.Left(item.m_strNCFileFullPath.ReverseFind(_T('\\'))+1);
		rename(item.m_strNCFileFullPath, strNCPath + strNCName_XiaoFeng);

		vFileToMove.push_back(strLabelNCPath + strLabelNCName_XiaoFeng);

		int nComponentCount = GetComponentCountInPanel(*(item.m_pPanel));
		for(int j = 0; j < nComponentCount; j++)
		{
			CString strOldLabelImageFullPath, strLabelImageFullPath_XiaoFeng;
			strOldLabelImageFullPath.Format(_T("%s%s"), strOutputDir, GetLabelImageName(i, j+1));
			strLabelImageFullPath_XiaoFeng.Format(_T("%s1004#%s#%s_%s.jpg"), strOutputDir, GetFloatString(item.m_pPanel->m_Thickness, 0), GetIntegerString(i+1, 3),  GetIntegerString(j+1, 4));
			rename(strOldLabelImageFullPath, strLabelImageFullPath_XiaoFeng);

			ImageRotate180(strLabelImageFullPath_XiaoFeng);

			vFileToMove.push_back(strLabelImageFullPath_XiaoFeng);
		}

		CString strFolderToMove;
		strFolderToMove.Format(_T("%s1004#%s#%s\\"), strOutputDir, GetFloatString(item.m_pPanel->m_Thickness, 0), GetIntegerString(i+1, 3));
		CreateDirectory(strFolderToMove, NULL);
		for(int j = 0; j < vFileToMove.size(); j++)
		{
			CString strTargetFullPath = strFolderToMove + GetFileNameInPath_WithSuffix(vFileToMove[j]);
			MoveFile(vFileToMove[j], strTargetFullPath);
		}
	}
}
#pragma once
#include <vector>

using namespace std;

struct XiaoFengDataItem
{
	//CString m_strNCFilePath;
	Panel* m_pPanel;
	CString m_strLabelNCFileName;
	CString m_strNCFileFullPath;
	//CString m_strPanelImageName;
};

void XiaoFentLabelProcess(vector<XiaoFengDataItem>& vXiaoFengData, CString strOutputDir);
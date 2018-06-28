// TiebiaoEdgeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TiebiaoEdgeDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
#include "SelectPathDlg.h"

// TiebiaoEdgeDlg 对话框

IMPLEMENT_DYNAMIC(TiebiaoEdgeDlg, CDialogEx)

TiebiaoEdgeDlg::TiebiaoEdgeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(TiebiaoEdgeDlg::IDD, pParent)
	, m_fromThin(0)
	, m_toThin(0)
	, m_DisplayEdgeChar(_T(""))
	, m_EdgeImgPath(_T(""))
{

}

TiebiaoEdgeDlg::~TiebiaoEdgeDlg()
{
}

void TiebiaoEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FROM_THIN, m_fromThin);
	DDX_Text(pDX, IDC_TO_THIN, m_toThin);
	DDX_Text(pDX, IDC_DISPLAY_CHAR, m_DisplayEdgeChar);
	DDX_Text(pDX, IDC_EDGE_IMG_PATH, m_EdgeImgPath);
}


BEGIN_MESSAGE_MAP(TiebiaoEdgeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FIND_IMG, &TiebiaoEdgeDlg::OnBnClickedFindImg)
END_MESSAGE_MAP()


/************************************************************************/
/* UI                                                                   */
/************************************************************************/

BOOL TiebiaoEdgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_oneEdgeString != "")
	{
		buildEdgeDataByOneEdgeString();
	}
	UpdateData(FALSE);

	return TRUE;
}


void TiebiaoEdgeDlg::OnOK()
{
	UpdateData(TRUE);
	saveOneEdgeStringByEdgeData();

	CDialogEx::OnOK();
}


void TiebiaoEdgeDlg::OnCancel()
{
	CDialogEx::OnCancel();
}


void TiebiaoEdgeDlg::OnBnClickedFindImg()
{
	UpdateData(TRUE);
	SelectPathDlg pathDlg;
	CString pathName = pathDlg.SelectFileDlgDoModel(
		"打开封边图片",
		"bmp",
		"*",
		HGTools::getContainPath(m_EdgeImgPath),
		DlgLoad
		);
	if (pathName != "")
	{
		CString contain = HGTools::getRealPathFromReleativePath(HGTools::getCurExeContainPath());
		if (pathName.Find(contain) == -1)
		{
			MessageBox("请选择软件目录下图片位置");
		}
		else
		{
			pathName.Replace(contain,".");
			m_EdgeImgPath = pathName;
			UpdateData(FALSE);
		}
	}
}

/************************************************************************/
/* Logic                                                                */
/************************************************************************/

//内部绑定字符串解析到界面上
void TiebiaoEdgeDlg::buildEdgeDataByOneEdgeString()
{
	std::map<std::string,std::string> edgeData = buildEdgeDataByOneEdgeString(m_oneEdgeString.GetBuffer());
	if (edgeData.size() == 4)
	{
		m_fromThin = atof(edgeData["from"].c_str());
		m_toThin = atof(edgeData["to"].c_str());
		m_DisplayEdgeChar = (edgeData["char"].c_str());
		m_EdgeImgPath = (edgeData["path"].c_str());
	}
}
//解析一行封边信息串
std::map<std::string,std::string> TiebiaoEdgeDlg::buildEdgeDataByOneEdgeString( std::string oneEdgeString )
{
	CString tempEdge = CString(oneEdgeString.c_str());
	tempEdge.Replace(";","");

	CStringArray spiltArr;
	HGTools::SplitCString(tempEdge,spiltArr,',');

	std::map<std::string,std::string> edgeData;
	if (spiltArr.GetSize() >=4 )
	{
		CString item;
		int i;

		i = 0;
		item = spiltArr.GetAt(i);
		edgeData["from"]	 =	item.GetBuffer();

		i = 1;
		item = spiltArr.GetAt(i);
		edgeData["to"]		 =	item.GetBuffer();

		i = 2;
		item = spiltArr.GetAt(i);
		edgeData["char"]	 =	item.GetBuffer();

		i = 3;
		item = spiltArr.GetAt(i);
		edgeData["path"]	 =	item.GetBuffer();
	}
	return edgeData;
}

//界面数据转换到内部数据字符串上
void TiebiaoEdgeDlg::saveOneEdgeStringByEdgeData()
{
	CString inputvalue;
	inputvalue.Format("%f,%f,%s,%s;",m_fromThin,m_toThin,m_DisplayEdgeChar,m_EdgeImgPath);
	m_oneEdgeString = inputvalue;
}
//解析一组封边数据为一行字符串
std::string TiebiaoEdgeDlg::saveOneEdgeStringByEdgeData( std::map<std::string,std::string> edgeData )
{
	CString result;
	result.Format("%s,%s,%s,%s;",
		edgeData["from"],
		edgeData["to"]	,
		edgeData["char"],
		edgeData["path"]
		);
	return result.GetBuffer();
}

//将一组封边信息的字符串解析为一组封边信息
std::vector<std::map<std::string,std::string>> TiebiaoEdgeDlg::buildEdgeDataByAllEdgeString( std::string allEdgeString )
{
	CString s_allEdgeString = allEdgeString.c_str();
	CStringArray lines;

	std::vector<std::map<std::string,std::string>> result;

	HGTools::SplitCString(s_allEdgeString,lines,';');
	for (int line_i = 0;line_i<lines.GetSize();line_i++)
	{
		CString csoneEdgeString = lines.GetAt(line_i);
		if (csoneEdgeString != "")
		{
			result.push_back(buildEdgeDataByOneEdgeString(csoneEdgeString.GetBuffer()));
		}
	}
	return result;
}
//将一组封边信息转换为一行
std::string TiebiaoEdgeDlg::saveAllEdgeStringByEdgeData( std::vector<std::map<std::string,std::string>> allEdgeData )
{
	CString result;

	for (int line_i = 0;line_i< allEdgeData.size();line_i++)
	{
		std::string s_oneEdgeString = saveOneEdgeStringByEdgeData(allEdgeData[line_i]);
		CString cs_oneEdgeString = s_oneEdgeString.c_str();
		cs_oneEdgeString.Replace(";","");
		result = result + cs_oneEdgeString + ";";
	}
	return result.GetBuffer();
}

//通过封边数值查找封边符号图片（需要设置AllEdgeString）
std::string TiebiaoEdgeDlg::findImgPathByEdge( float edge )
{
	std::string result = "";
	if (AllEdgeString() != "")
	{
		std::vector<std::map<std::string,std::string>> dataSet = buildEdgeDataByAllEdgeString(AllEdgeString().GetBuffer());
		float diff = 0.0001;
		for (int i = 0; i < dataSet.size() ; i++)
		{
			float from = atof(dataSet[i]["from"].c_str());
			float to = atof(dataSet[i]["to"].c_str());
			if ( ((from - diff) <= (edge)) && ((edge) < (to)))
			{
				result = dataSet[i]["path"];
				break;
			}
		}
	}
	return result;
}
//通过封边数值查找封边符号字符（需要设置AllEdgeString）
std::string TiebiaoEdgeDlg::findCharByEdge( float edge )
{
	std::string result = "";
	if (AllEdgeString() != "")
	{
		std::vector<std::map<std::string,std::string>> dataSet = buildEdgeDataByAllEdgeString(AllEdgeString().GetBuffer());
		float diff = 0.0001;
			for (int i = 0; i < dataSet.size() ; i++)
			{
				float from = atof(dataSet[i]["from"].c_str());
				float to = atof(dataSet[i]["to"].c_str());
				if ( ((from - diff) <= (edge)) && ((edge) < (to)))
				{
					result = dataSet[i]["char"];
					break;
				}
			}
	}
	if (result.compare("") == 0)
	{
		CString mm;
		mm.Format("%.1f",edge);
		result = mm.GetBuffer();
	}
	return result;
}



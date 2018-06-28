// SelectPathDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelectPathDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
//#include "../Misc/XmlHandlePlus.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Misc/Misc.h"
#include "../Work/WorkDef.h"

// SelectPathDlg 对话框

IMPLEMENT_DYNAMIC(SelectPathDlg, CDialogEx)

SelectPathDlg::SelectPathDlg(CString _title,CWnd* pParent /*=NULL*/)
	: CDialogEx(SelectPathDlg::IDD, pParent)
{
	Title(_title);
	CurrentPath("D:\\默认路径");
	loadSettings();
}

SelectPathDlg::~SelectPathDlg()
{
}

void SelectPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH_EDITER, m_currentPath);
}


BEGIN_MESSAGE_MAP(SelectPathDlg, CDialogEx)
	ON_BN_CLICKED(ID_FIND_PATH, &SelectPathDlg::OnBnClickedFindPath)		//浏览
	ON_BN_CLICKED(ID_OPEN_PATH, &SelectPathDlg::OnBnClickedOpenPath)		//创建并打开目录
	ON_BN_CLICKED(ID_CLEAR_PATH, &SelectPathDlg::OnBnClickedClearPath)		//清除这个目录
	ON_BN_CLICKED(IDOK, &SelectPathDlg::OnBnClickedOk)						//确认选择
	ON_BN_CLICKED(IDCANCEL, &SelectPathDlg::OnBnClickedCancel)				//取消
END_MESSAGE_MAP()


/************************************************************************/
/* UI                                                                   */
/************************************************************************/
//浏览
void SelectPathDlg::OnBnClickedFindPath()
{
	UpdateData(TRUE);
	CString newPath = SelectPathDlgDoModel(m_currentPath);
	if (newPath != "")
	{
		m_currentPath = newPath;
		UpdateData(FALSE);
	}
}
//创建并打开目录
void SelectPathDlg::OnBnClickedOpenPath()
{
	UpdateData(TRUE);
	HGTools::createDir(m_currentPath);
	ShellExecute(NULL,"explore", m_currentPath ,NULL,NULL,SW_SHOWNORMAL);
}
//创建并打开目录
void SelectPathDlg::OnBnClickedClearPath()
{
	UpdateData(TRUE);
	if (MessageBox("是否删除该目录下所有文件？","删除提示",MB_OKCANCEL|MB_ICONWARNING  ) == IDOK)
	{
		HGTools::deletePathFiles(m_currentPath);
		MessageBox("清除成功");
	}
}
//确认选择
void SelectPathDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	HGTools::createDir(m_currentPath);
	CDialogEx::OnOK();
}

//取消
void SelectPathDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

//初始化对话框
BOOL SelectPathDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_title != "")
	{
		SetWindowText(m_title);
	}
	UpdateData(FALSE);
	return TRUE;  
}

/************************************************************************/
/* Logic                                                                */
/************************************************************************/

//加载配置
void SelectPathDlg::loadSettings()
{


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//配置文件路径
	CString hgmPath = HGTools::getRelativePath("kdata\\UISetting.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//解密
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif








	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//加载辅助
	//TinyXml::XmlHandlePlus docHandler(&doc);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;


	//默认保存目录
	//m_defaultSavePath = docHandler.getParam("DefaultSavePath","D:\\123\\123").c_str();
	strTmp = _T("D:\\123\\123");
	TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(doc.RootElement(), "Param", "Name", "DefaultSavePath");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_defaultSavePath = strTmp;

	//默认加载目录
	//m_defaultLoadPath = docHandler.getParam("DefaultLoadPath","D:\\123\\123").c_str();	
	strTmp = _T("D:\\123\\123");
	elmTmp = xmlHandler.GetIndicatedElement(doc.RootElement(), "Param", "Name", "DefaultLoadPath");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_defaultLoadPath = strTmp;


	//关闭文件
	HGTools::deleteFile(xmlPath);
}

//保存配置
void SelectPathDlg::saveSettings()
{
	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	//配置文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//解密
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

#else


	//配置文件路径
	CString hgmPath = HGTools::getRelativePath("kdata\\UISetting.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);
	//解密
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif






	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//加载辅助
	//TinyXml::XmlHandlePlus docHandler(&doc);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;

	//默认保存目录
	//docHandler.setParam("DefaultSavePath",m_defaultSavePath.GetBuffer());
	xmlHandler.SetIndicatedElmAttr(doc.RootElement(), "Param", "Name", "DefaultSavePath", "Value", m_defaultSavePath.GetBuffer());
	m_defaultSavePath.ReleaseBuffer();

	//默认加载目录
	//docHandler.setParam("DefaultLoadPath",m_defaultLoadPath.GetBuffer());
	xmlHandler.SetIndicatedElmAttr(doc.RootElement(), "Param", "Name", "DefaultLoadPath", "Value", m_defaultLoadPath.GetBuffer());
	m_defaultLoadPath.ReleaseBuffer();


	//保存
	doc.SaveFile(xmlPath);




	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)



	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());

#else


	//HGTools::encryptFile(xmlPath,hgmPath);
#endif


	//关闭文件
	HGTools::deleteFile(xmlPath);
}

/************************************************************************/
/* 工具函数                                                             */
/************************************************************************/


//用于找到文件路径的回调函数
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  
{
	if(uMsg == BFFM_INITIALIZED)
	{  
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;  
}


//返回用户选择的目录
CString SelectPathDlg::SelectPathDlgDoModel( CString findPath )
{
	CString pathName;

	char  szDir[1000]; 
	BROWSEINFO bi; 
	ITEMIDLIST *pidl;  
	bi.hwndOwner = this->m_hWnd;  
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = szDir;  
	bi.lpszTitle = _T("请选择目录");   
	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS; 
	CFileFind   finder;
	if(finder.FindFile(findPath)==FALSE)
	{
		bi.lParam =0;
		bi.lpfn = NULL;
	}
	else
	{
		bi.lParam = (long)(m_currentPath.GetBuffer(findPath.GetLength()));//初始化路径，形如(_T("c:\\Symbian"));
		bi.lpfn = BrowseCallbackProc;
	}
	finder.Close();
	bi.iImage = 0;  
	pidl = SHBrowseForFolder(&bi); 
	if(pidl == NULL) return "";
	if(!SHGetPathFromIDList(pidl, szDir)) return ""; 

	pathName = szDir;
	return pathName;
}







CString SelectPathDlg::SelectFileDlgDoModel(CString title, CString fileType , CString defaultname, CString findPath  ,DlgType type )
{
	char filterBuffer[100];
	CString filter;
	filter.Format("%s Files (*.%s)|*.%s|All Files (*.*)|*.*||",fileType,fileType,fileType);
	HGTools::Convert2FillChar(filter,filterBuffer);

	CString fileSaveName;
	fileSaveName.Format("%s.%s",defaultname,fileType);

	CFileDialog fileDlg (type, fileType, fileSaveName,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filterBuffer, this);
	fileDlg.m_ofn.lpstrTitle = title; 
	fileDlg.m_ofn.lpstrInitialDir = findPath;


	if (fileDlg.DoModal() == IDOK)
	{
		return fileDlg.GetPathName();
	}
	else
	{
		return "";
	}
}



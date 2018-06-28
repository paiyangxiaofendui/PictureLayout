
// ConfigConverterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigConverter.h"
#include "ConfigConverterDlg.h"
#include "afxdialogex.h"
#include "FileCompressandUnCompress.h"
#include <vector>
#include "Misc/Misc.h"
#include "Misc/ProgramMisc.h"
#include "ConvertingDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConfigConverterDlg 对话框




CConfigConverterDlg::CConfigConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigConverterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConfigConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SOURCE_ROOT_DIR, &CConfigConverterDlg::OnBnClickedButtonSourceRootDir)
	ON_BN_CLICKED(IDC_BUTTON_TARGET_ROOT_DIR, &CConfigConverterDlg::OnBnClickedButtonTargetRootDir)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CConfigConverterDlg::OnBnClickedButtonConvert)
END_MESSAGE_MAP()


// CConfigConverterDlg 消息处理程序

BOOL CConfigConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT_SOURCE_ROOT_DIR)->SetWindowText(GetModulePath());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConfigConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConfigConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConfigConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConfigConverterDlg::OnBnClickedButtonSourceRootDir()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;
	ZeroMemory(szPath, sizeof(szPath));  

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "旧版根目录";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		GetDlgItem(IDC_EDIT_SOURCE_ROOT_DIR)->SetWindowText(szPath);
	}
/*	else   
		AfxMessageBox("目录无效!"); */  

}


void CConfigConverterDlg::OnBnClickedButtonTargetRootDir()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径 

	ZeroMemory(szPath, sizeof(szPath));  

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "新版根目录";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		CString str = szPath;
		if(str.GetAt(str.GetLength()-1) != _T('\\'))
			str += _T("\\");
		GetDlgItem(IDC_EDIT_TARGET_ROOT_DIR)->SetWindowText(str);
	}
}


void CConfigConverterDlg::OnBnClickedButtonConvert()
{
	// TODO: 在此添加控件通知处理程序代码
	//vector<CString> vAllSourceDir;

	CString strSourceDir, strTargetDir;
	GetDlgItem(IDC_EDIT_SOURCE_ROOT_DIR)->GetWindowText(strSourceDir);
	//vAllSourceDir = TraversFile(strSourceDir, _T("hgm"));
	GetDlgItem(IDC_EDIT_TARGET_ROOT_DIR)->GetWindowText(strTargetDir);

	if(strSourceDir.GetAt(strSourceDir.GetLength()-1) != _T('\\'))
		strSourceDir += _T("\\");
	if(strTargetDir.GetAt(strTargetDir.GetLength()-1) != _T('\\'))
		strTargetDir += _T("\\");
	CConvertingDlg dlg(strSourceDir, strTargetDir);
	dlg.DoModal();

	//CString strFindFile = strTargetDir + _T("*.*")
 //   _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName); //匹配格式为*.*,即该目录下的所有文件  
 //   WIN32_FIND_DATAA FindFileData;        
 //   ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  
 //   HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);  
 //   BOOL IsFinded = TRUE;  
 //   while(IsFinded)  
 //   {  
 //       IsFinded = FindNextFileA(hFile, &FindFileData); //递归搜索其他的文件  
 //       if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //如果不是"." ".."目录  
 //       {  
 //           std::string strFileName = "";  
 //           strFileName = strFileName + DirName + "//" + FindFileData.cFileName;  
 //           std::string strTemp;  
 //           strTemp = strFileName;  
 //           if( isDir(strFileName.c_str()) ) //如果是目录，则递归地调用  
 //           {     
 //               printf("目录为:%s/n", strFileName.c_str());  
 //               deletePath(strTemp.c_str());  
 //           }  
 //           else  
 //           {  
 //               DeleteFileA(strTemp.c_str());  
 //           }  
 //       }  
 //   }  
 //   FindClose(hFile); 

}

void CConfigConverterDlg::ConvertAllFile(vector<CString> vAllSourceDir, CString strTargetRootDir)
{

}
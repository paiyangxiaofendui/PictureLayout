
// CppTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CppTest.h"
#include "CppTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "../NewUI/ExportAPI.h"


// 判断是否在测试环境下
#ifdef DEBUG 

#pragma comment(lib, "../../lib/NewUId.lib")

#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/NewUI.lib")

#pragma  message(".......................................当前为release版本")

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


// CCppTestDlg 对话框




CCppTestDlg::CCppTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCppTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCppTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCppTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCppTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCppTestDlg 消息处理程序

BOOL CCppTestDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCppTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCppTestDlg::OnPaint()
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
HCURSOR CCppTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CALLBACK EnumWindowsLikeProc(HWND hwnd,LPARAM lParam)
{
	CString* namelink = (CString*)lParam;
	TCHAR str[500];
	::GetWindowText(hwnd,str,sizeof(str));
	if(CString(str).Find(*namelink) != -1)
	{
		*namelink = CString(str);
		return 0;
	}
	return 1;
}



int FindWindowLike(CString& namelink)
{
	EnumWindows(EnumWindowsLikeProc,(LPARAM)&namelink);
	return 0;
}



CWnd* FindWndByCtrlID(CWnd* pWnd, UINT nCtrlID, LPCTSTR szClassName)
{
	CWnd* pFindingWnd = NULL;
	pFindingWnd = pWnd->GetWindow(GW_CHILD);
	while(pFindingWnd)
	{
		CString strTmp;
		TCHAR szCheckingClassName[MAX_PATH];
		GetClassName(pFindingWnd->GetSafeHwnd(), szCheckingClassName, MAX_PATH);
		strTmp.Format("ID=%d H=%x class=%s\n", pFindingWnd->GetDlgCtrlID(), pFindingWnd->GetSafeHwnd(), szCheckingClassName);
		OutputDebugString(strTmp);

		CString strFindingClassName = szClassName;
		if(pFindingWnd->GetDlgCtrlID() == nCtrlID && (strFindingClassName.IsEmpty() || strFindingClassName.CompareNoCase(szCheckingClassName) == 0))
		{
			return pFindingWnd;
		}

		CWnd* pRes = FindWndByCtrlID(pFindingWnd, nCtrlID, szClassName);
		if(pRes)
			return pRes;

		pFindingWnd = pFindingWnd->GetNextWindow();
	}

	return NULL;
}


void EmptyCtrlContent(HWND hWnd)
{
	for(int j = 0; j < MAX_PATH; j++)
	{
		::PostMessage(hWnd, WM_KEYDOWN, VK_BACK, 0);
		::PostMessage(hWnd, WM_KEYUP, VK_BACK, 0);
	}
	for(int j = 0; j < MAX_PATH; j++)
	{
		::PostMessage(hWnd, WM_KEYDOWN, VK_DELETE, 0);
		::PostMessage(hWnd, WM_KEYUP, VK_DELETE, 0);
	}
}


void CCppTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	

	//Test();
#if 0

	JT_Layouts("F:\\PictureLayout\\真实测试.xml");
#else

	JT_Layouts("E:\\袁梓埠个人文件夹\\代码\\PictureLayout\\真实测试_家里电脑.xml");	
#endif


#if 0

		
		CString title = "蒙泰彩色电子出版系统 V6.0(普及版)";
		int id = 200;

		CString strMsg;
// 		strMsg.Format("默认转发窗口:%s 默认转发控件ID:%d", title, id);
// 		//AfxMessageBox(strMsg);
// 		CWnd* pDefTransWnd = NULL;
// 
// 		CString strWndTitle = title;
// 		FindWindowLike(strWndTitle);
// 		pDefTransWnd = FindWindow(NULL, strWndTitle);
// 
// 
// 		if(pDefTransWnd && pDefTransWnd->GetSafeHwnd())
// 		{
// 			CWnd wndPopup;
// 			wndPopup.Attach(::GetLastActivePopup(pDefTransWnd->GetSafeHwnd()));
// 
// 			CWnd* pDefTransCtrl = FindWndByCtrlID(/*pDefTransWnd*/&wndPopup, id, NULL);
// 			wndPopup.Detach();
// 			if(pDefTransCtrl && pDefTransCtrl->GetSafeHwnd())
// 			{
// 				strMsg.Format("找到了默认窗口及控件！");
// 				//AfxMessageBox(strMsg);
// 
// 				::PostMessage(pDefTransCtrl->GetSafeHwnd(), BM_CLICK, 0, 0);
// 
// 				//CWnd* pPostingWnd = pDefTransCtrl->GetParent();
// 				//while(pPostingWnd)
// 				//{
// 				//	::PostMessage(pDefTransCtrl->GetParent()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(theAction.m_uCtrlID, BN_CLICKED), (LPARAM)(pDefTransCtrl->GetSafeHwnd()));
// 				//	pPostingWnd = pPostingWnd->GetParent();
// 				//}
// 
// 				//::PostMessage(pDefTransCtrl->GetSafeHwnd(), WM_LBUTTONDOWN, 0, 0);
// 				//::PostMessage(pDefTransCtrl->GetSafeHwnd(), WM_LBUTTONUP, 0, 0);
// 			}
// 			else
// 			{
// 				strMsg.Format("找不到默认控件！");
// 				AfxMessageBox(strMsg);
// 			}	
// 		}
// 		else
// 		{
// 			strMsg.Format("找不到默认窗口！");
// 			AfxMessageBox(strMsg);
// 		}



			
				strMsg.Format("默认转发窗口:%s", title);
				//AfxMessageBox(strMsg);
				CWnd* pDefTransWnd = NULL;

				CString strWndTitle = title;
				FindWindowLike(strWndTitle);
				pDefTransWnd = FindWindow(NULL, strWndTitle);


				if(pDefTransWnd && pDefTransWnd->GetSafeHwnd())
				{
					CString strBarcode = "100";
// 					((CEdit*)GetDlgItem(IDC_EDIT_BARCODE))->GetWindowText(strBarcode);
// 					strBarcode += m_config.m_strAddSuffix;
					HWND hPopup = ::GetLastActivePopup(pDefTransWnd->GetSafeHwnd());

					CWnd wndPopup;
					wndPopup.Attach(hPopup);
					CWnd* pTargetCtrl = NULL;
					pTargetCtrl = FindWndByCtrlID(&wndPopup, id, "Edit");
					//CString strPrint = PrintAllCtrl(&wndPopup);
					//AfxMessageBox(strPrint);
					wndPopup.Detach();		

					if(pTargetCtrl && pTargetCtrl->GetSafeHwnd())
					{
						EmptyCtrlContent(pTargetCtrl->GetSafeHwnd());

						CString strTmp;
						TCHAR szClassName[MAX_PATH];
						GetClassName(pTargetCtrl->GetSafeHwnd(), szClassName, MAX_PATH);
						strTmp.Format("ID=%d H=%x class=%s\n", pTargetCtrl->GetDlgCtrlID(), pTargetCtrl->GetSafeHwnd(), szClassName);
						OutputDebugString(strTmp);
						for(int j = 0; j < strBarcode.GetLength(); j++)
						{
							::PostMessage(pTargetCtrl->GetSafeHwnd(), WM_CHAR, strBarcode.GetAt(j), 0);
						}
						//::PostMessage(pTargetCtrl->GetSafeHwnd(), WM_KEYDOWN, VK_RETURN, 0);
					}
					else
					{
						strMsg.Format(_T("在默认窗口下找不到控件：ID=%d CtrlClassName=%s"), id, "Edit");
						AfxMessageBox(strMsg);
					}
				}
				else
				{
					strMsg.Format("找不到默认窗口！");
					AfxMessageBox(strMsg);
				}






#endif







	//CDialogEx::OnOK();
}

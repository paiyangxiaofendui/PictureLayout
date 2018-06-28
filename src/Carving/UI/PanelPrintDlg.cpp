// PanelPrintDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "PanelPrintDlg.h"
//#include "afxdialogex.h"



// CPanelPrintDlg dialog

IMPLEMENT_DYNAMIC(CPanelPrintDlg, CDialogEx)

CPanelPrintDlg::CPanelPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPanelPrintDlg::IDD, pParent)
{

}

CPanelPrintDlg::~CPanelPrintDlg()
{
}

void CPanelPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_PANELPRINT, m_panelprint);
	DDX_Control(pDX, IDC_LIST3, m_plist);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CPanelPrintDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CPanelPrintDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPanelPrintDlg::OnBnClickedOk)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPanelPrintDlg message handlers


void CPanelPrintDlg::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}


void CPanelPrintDlg::OnBnClickedOk()//打印样图->打印预览->打印按钮
{
	
	CDC dc;
	CPrintDialog printDlg(FALSE);
	if(printDlg.DoModal() == IDOK)
	{
		HGLOBAL hDevMode;
		HGLOBAL hDevNames;

		short paperWidth = 2970;
		short paperHeight = 2100;

		printDlg.GetDefaults();
		DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(printDlg.m_pd.hDevMode);
		pDevMode->dmFields = pDevMode->dmFields | DM_PAPERSIZE;
		pDevMode->dmPaperSize = DMPAPER_USER;    

		//pDevMode->dmPaperWidth = paperWidth;
		//pDevMode->dmPaperLength = paperHeight;

		::GlobalUnlock(printDlg.m_pd.hDevMode);

		CPrintInfo Info;

		dc.Attach(printDlg.CreatePrinterDC());
		dc.m_bPrinting = TRUE;
		CString strTitle;
		strTitle.LoadString(AFX_IDS_APP_TITLE);
		DOCINFO di;

		::ZeroMemory (&di, sizeof (DOCINFO));
		di.cbSize = sizeof (DOCINFO);
		di.lpszDocName = "样板打印";

		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_path = ExeFile;
		file_path = file_path.Left(file_path.ReverseFind('\\'));
		file_path = file_path + _T("\\picture");
		int theNum = 1;
		for (int p = 0;p < m_panelNum;p++)
		{
			if (p>theNum*10)//每打印10个睡1s
			{
				theNum++;
				Sleep(1000);
			}
			CString file_name;
			file_name.Format("design-%d.bmp",p);

			BOOL bPrintingOK = dc.StartDoc(&di);

			Info.m_rectDraw.SetRect(0,0,dc.GetDeviceCaps(HORZRES),dc.GetDeviceCaps(VERTRES));

			Info.SetMaxPage (1);
			OnPrint(&dc, &Info,file_path + "\\" + file_name,1188,840);
			if (bPrintingOK)
				dc.EndDoc();
			else
				dc.AbortDoc();

		}
		dc.Detach();
	}
	//CDialogEx::OnOK();
}

void CPanelPrintDlg::PrintView(int bwidth,int bheight)
{
	
}

void CPanelPrintDlg::OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight)
{
	CDC* pDC   = pdc;
	CPrintInfo* pInfo = (CPrintInfo *)lParam;

	CFont DataFont;
	DataFont.CreatePointFont(120,"宋体",pDC);

	HBITMAP hBitmap = (HBITMAP) ::LoadImage(NULL, strFileName, IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);

	HDC dcMem;
	dcMem=::CreateCompatibleDC(pDC->m_hDC);

	HBITMAP hOldBmp=(HBITMAP)::SelectObject(dcMem,hBitmap);

	CRect r = pInfo->m_rectDraw;
	int nVertCenterPos = pDC->GetDeviceCaps (VERTRES) / 2;
	::StretchBlt(pDC->m_hDC, r.left, r.top, r.Width(), r.Height(),
		dcMem,0,0,bwidth,bheight,SRCCOPY);

	::SelectObject(dcMem,hOldBmp);
	::DeleteDC(dcMem);
	::DeleteObject(hBitmap);
}

BOOL CPanelPrintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowWindow(SW_MAXIMIZE);

	//图片加载位置
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	file_path = file_path + _T("\\picture");

	m_plist.DeleteAllItems();
	m_plist.SetFolder(1188,840,file_path,m_panelNum,"design");



	CRect rect;
	this->GetClientRect(&rect);
	m_plist.MoveWindow(0,0,rect.Width(),rect.Height() - 50);
	m_btnCancel.MoveWindow(rect.Width()-80,rect.Height() - 30,70,25);
	m_btnOK.MoveWindow(rect.Width()-150,rect.Height() - 30,70,25);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPanelPrintDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	this->GetClientRect(&rect);
	if(m_plist.GetSafeHwnd())
	{
		m_plist.MoveWindow(0,0,rect.Width(),rect.Height() - 50);
		m_btnCancel.MoveWindow(rect.Width()-80,rect.Height() - 30,70,25);
		m_btnOK.MoveWindow(rect.Width()-150,rect.Height() - 30,70,25);
	}

	CDialogEx::OnSize(nType, cx, cy);	
}

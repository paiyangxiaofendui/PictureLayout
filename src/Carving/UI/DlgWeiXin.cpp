// DlgWeiXin.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWeiXin.h"
#include "afxdialogex.h"
#include "../Misc/Misc.h"


// CDlgWeiXin 对话框

IMPLEMENT_DYNAMIC(CDlgWeiXin, CDialog)

CDlgWeiXin::CDlgWeiXin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWeiXin::IDD, pParent)
	, m_pImg(NULL)
{

}

CDlgWeiXin::~CDlgWeiXin()
{
	if(m_pImg)
		delete m_pImg;
}

void CDlgWeiXin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgWeiXin, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgWeiXin 消息处理程序
BOOL CDlgWeiXin::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// TODO:  在此添加额外的初始化
#ifndef YiWei
	m_pImg = LoadPngImgFromRes(IDB_PNG_QRCODE_LARGE);
#else
	m_pImg = LoadPngImgFromRes(IDB_PNG_QRCODE_LARGE_YW);
#endif
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgWeiXin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);


	Graphics g(dcMem.m_hDC);
	COLORREF colBK = RGB(224, 241, 252);//GetSysColor(CTLCOLOR_DLG);//GetBkColor(dc.m_hDC);

	g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());


	if(m_pImg)
	{
		RectF rfDes((REAL)(rcClient.left+(rcClient.Width()-m_pImg->GetWidth())/2), \
			(REAL)(rcClient.top+(rcClient.Height()-m_pImg->GetHeight())/2), \
			(REAL)m_pImg->GetWidth(), \
			(REAL)m_pImg->GetHeight());
		g.DrawImage(m_pImg, rfDes, 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight(), UnitPixel);
	}



	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}


BOOL CDlgWeiXin::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
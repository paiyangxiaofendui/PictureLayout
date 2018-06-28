// Link.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "Link.h"
#include "MEMDC.H"


// CLink

IMPLEMENT_DYNAMIC(CLink, CStatic)

CLink::CLink(LPCTSTR szLink)
	:m_bTrack(false)
	,m_strLink(szLink)
{

}

CLink::~CLink()
{
	if(m_bmpBk.GetSafeHandle())
		m_bmpBk.DeleteObject();
	if(m_dcBk.GetSafeHdc())
		m_dcBk.DeleteDC();
}


BEGIN_MESSAGE_MAP(CLink, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CLink 消息处理程序
void CLink::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, WS_TABSTOP|SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

BOOL CLink::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}

void CLink::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CRect rcClient;
	GetClientRect(rcClient);

	CMemoryDC memDC(&dc, rcClient);
	DrawBackground(&memDC);


	WCHAR chText[100];
	GetWindowTextW(GetSafeHwnd(), chText, 100);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	bool bHover = false;
	if(rcClient.PtInRect(ptCursor))
		bHover = true;

	Graphics g(memDC.m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Font font(L"Segoe UI", 15, bHover ? FontStyleUnderline : FontStyleRegular, UnitPixel);
	StringFormat sf;
	SolidBrush brushBlue(Color(0, 150, 233));
	sf.SetLineAlignment(StringAlignmentCenter);
	g.DrawString(chText, -1, &font, RectF(0, 0, rcClient.Width(), rcClient.Height()), &sf, &brushBlue);
}

BOOL CLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	CRect rcClient;
	GetClientRect(rcClient);

	if(rcClient.PtInRect(ptCursor))
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_HAND);
		::SetCursor(hCursor); 
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CLink::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnMouseMove(nFlags, point);

	if(!m_bTrack)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize=sizeof(TRACKMOUSEEVENT);
		tme.dwFlags=TME_LEAVE;
		tme.dwHoverTime=HOVER_DEFAULT;
		tme.hwndTrack=m_hWnd;
		_TrackMouseEvent(&tme);
		m_bTrack=TRUE;

	}
	Invalidate();
}

void CLink::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	m_bTrack=FALSE;
	Invalidate();
	CStatic::OnMouseLeave();
}

void CLink::DrawBackground(CDC* pDC)
{
	CRect rcButton;
	GetWindowRect(rcButton);
	GetParent()->ScreenToClient(rcButton);
	CBitmap* pOldBitmap = NULL;
	if(!m_dcBk.GetSafeHdc())
	{
		CClientDC clDC(GetParent());
		m_dcBk.CreateCompatibleDC(&clDC);
		m_bmpBk.CreateCompatibleBitmap(&clDC,rcButton.Width(),rcButton.Height());
		pOldBitmap = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rcButton.Width(), rcButton.Height(), &clDC, rcButton.left, rcButton.top, SRCCOPY);
		m_dcBk.SelectObject(pOldBitmap);
	}
	pOldBitmap = m_dcBk.SelectObject(&m_bmpBk);
	pDC->BitBlt(0, 0, rcButton.Width(), rcButton.Height(), &m_dcBk,0,0,SRCCOPY);
	m_dcBk.SelectObject(pOldBitmap);
}

void CLink::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_strLink.IsEmpty())
	{
		ShellExecute( \
			NULL, \
			_T("open"), \
			m_strLink, \
			NULL, \
			NULL, \
			SW_SHOWDEFAULT);
	}

	CStatic::OnLButtonDown(nFlags, point);
}
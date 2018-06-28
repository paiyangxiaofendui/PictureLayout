// TabBar.cpp : implementation file
//

#include "stdafx.h"
#include "TabBar.h"
#include "MEMDC.H"

#define TAB_WIDTH 135
// CTabBar

IMPLEMENT_DYNAMIC(CTabBar, CStatic)

CTabBar::CTabBar()
	:m_nCurSelTab(-1)
	//,m_pImgLeft(NULL)
	//,m_pImgCenter(NULL)
	//,m_pImgRight(NULL)
	//,m_pImgLeftDown(NULL)
	//,m_pImgCenterDown(NULL)
	//,m_pImgRightDown(NULL)
	,m_textFontNormal(L"Segoe UI", 14, FontStyleRegular, UnitPixel)
	,m_brushWhite(Color(255, 255, 255))
	,m_brushShadow(Color(255, 0, 0, 0))
	,m_bTrack(false)
{

}

CTabBar::~CTabBar()
{
	if(m_bmpBk.GetSafeHandle())
		m_bmpBk.DeleteObject();
	if(m_dcBk.GetSafeHdc())
		m_dcBk.DeleteDC();

	if(m_bmpTabUnselectedN.GetSafeHandle())
		m_bmpTabUnselectedN.DeleteObject();
	if(m_bmpTabUnselectedH.GetSafeHandle())
		m_bmpTabUnselectedH.DeleteObject();
	if(m_bmpTabSelectedN.GetSafeHandle())
		m_bmpTabSelectedN.DeleteObject();
	if(m_bmpTabSelectedH.GetSafeHandle())
		m_bmpTabSelectedH.DeleteObject();

	//if(m_pImgLeft) delete m_pImgLeft;
	//if(m_pImgCenter) delete m_pImgCenter;
	//if(m_pImgRight) delete m_pImgRight;
	//if(m_pImgLeftDown) delete m_pImgLeftDown;
	//if(m_pImgCenterDown) delete m_pImgCenterDown;
	//if(m_pImgRightDown) delete m_pImgRightDown;
}


BEGIN_MESSAGE_MAP(CTabBar, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CTabBar message handlers



void CTabBar::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, WS_TABSTOP|SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

BOOL CTabBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}

//void CTabBar::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CStatic::OnPaint() for painting messages
//	if(m_vTabItem.size() == 0)
//		return;
//	CRect rcClient,rcWindow;
//	GetClientRect(rcClient);
//	GetWindowRect(rcWindow);
//
//	CMemDC memDC(&dc, rcClient);
//	DrawBackground(&memDC);
//
//	Graphics g(memDC.m_hDC);
//	g.SetSmoothingMode(SmoothingModeHighQuality);
//
//	for(int i = 0; i < m_vTabItem.size(); i++)
//	{
//		if (i == 0)
//		{
//			DrawTab(g, TAB_LEFT, i);
//		}
//		else if(i == m_vTabItem.size()-1)
//		{
//			DrawTab(g, TAB_RIGHT, i);
//		}
//		else
//		{
//			DrawTab(g, TAB_CENTER, i);
//		}
//	}
//	//g.DrawImage(m_pImgLeft, 0, 0);
//}

void CTabBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	if(m_vTabItem.size() == 0)
		return;
	CRect rcClient,rcWindow;
	GetClientRect(rcClient);
	GetWindowRect(rcWindow);

	CMemoryDC memDC(&dc, rcClient);
	DrawBackground(&memDC);

	//Graphics g(memDC.m_hDC);
	//g.SetSmoothingMode(SmoothingModeHighQuality);

	for(int i = m_vTabItem.size()-1; i >= 0; i--)
	{
		if(i == m_nCurSelTab)
			continue;

		int nTabLenShort = m_nTabLen - m_nOverlayLen;
		CRect rcTabDrawing;
		rcTabDrawing.SetRect(rcClient.left+nTabLenShort*i, rcClient.top, rcClient.left+nTabLenShort*i+m_nTabLen, rcClient.bottom);
		DrawTab(memDC, m_vTabItem[i], rcTabDrawing);

		//int nTabLenShort = m_nTabLen - m_nOverlayLen;
		//CRect rcTabDrawing;
		//rcTabDrawing.SetRect(rcClient.left+nTabLenShort*i, rcClient.top, rcClient.left+nTabLenShort*i+m_nTabLen, rcClient.bottom);
		//CDC dcBmp;
		//dcBmp.CreateCompatibleDC(&dc);
		//CPoint ptMouse;
		//GetCursorPos(&ptMouse);
		//ScreenToClient(&ptMouse);
		//CBitmap* pTabBmp = NULL;
		//if(m_vTabItem[i].m_pRgnTab->PtInRegion(ptMouse))
		//	pTabBmp = &m_bmpTabH;
		//else
		//	pTabBmp = &m_bmpTabN;
		//CBitmap* pOldBmp = dcBmp.SelectObject(pTabBmp);
		//memDC.TransparentBlt(rcTabDrawing.left, rcTabDrawing.top, rcTabDrawing.Width(), rcTabDrawing.Height(), &dcBmp, 0, 0, rcTabDrawing.Width(), rcTabDrawing.Height(), RGB(0, 0, 0));
		//dcBmp.SelectObject(pOldBmp);
		//dcBmp.DeleteDC();
	}

	//m_nCurSelTab = 1;
	if(m_nCurSelTab != -1)
	{
		int nTabLenShort = m_nTabLen - m_nOverlayLen;
		CRect rcTabDrawing;
		rcTabDrawing.SetRect(rcClient.left+nTabLenShort*m_nCurSelTab, rcClient.top, rcClient.left+nTabLenShort*m_nCurSelTab+m_nTabLen, rcClient.bottom);
		DrawTab(memDC, m_vTabItem[m_nCurSelTab], rcTabDrawing, true);
	}
}

void CTabBar::DrawBackground(CDC* pDC)
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

//void CTabBar::AddTab(LPCTSTR szName, UINT uID)
//{
//
//	TAB_ITEM tabItem(szName, uID);
//	m_vTabItem.push_back(tabItem);
//
//	CWnd* pParent  = GetParent();
//	CRect rcCtrl, rcCtrlNew;
//	GetWindowRect(rcCtrl);
//	pParent->ScreenToClient(rcCtrl);
//	int nWidth = TAB_WIDTH * (int)m_vTabItem.size();
//	rcCtrlNew.left = (rcCtrl.left + rcCtrl.right)/2 - nWidth/2;
//	rcCtrlNew.right = rcCtrlNew.left + nWidth;
//	rcCtrlNew.top = rcCtrl.top;
//	rcCtrlNew.bottom = rcCtrl.bottom;
//	MoveWindow(rcCtrlNew);
//
//	Invalidate();
//}

void CTabBar::AddTab(LPCTSTR szName, UINT uID)
{
	TAB_ITEM tabItem(szName, uID);
	//TAB_ITEM tabItem;
	//tabItem.m_strName = szName;
	//tabItem.m_uID = uID;
	//tabItem.m_pRgnTab = NULL;
	m_vTabItem.push_back(tabItem);

	RefreshTabRgn();

	Invalidate();
}

//void CTabBar::SetImage(LPCTSTR szLeft, LPCTSTR szLeftDown, LPCTSTR szCenter, LPCTSTR szCenterDown, LPCTSTR szRight, LPCTSTR szRightDown)
//{
//	m_pImgLeft = Image::FromFile(szLeft);
//	m_pImgCenter = Image::FromFile(szCenter);
//	m_pImgRight = Image::FromFile(szRight);
//	m_pImgLeftDown = Image::FromFile(szLeftDown);
//	m_pImgCenterDown = Image::FromFile(szCenterDown);
//	m_pImgRightDown = Image::FromFile(szRightDown);
//
//	CRect rcWnd;
//	GetWindowRect(rcWnd);
//	SetWindowPos(NULL, 0, 0, rcWnd.Width(), m_pImgLeft->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
//}

void CTabBar::SetUIInfo(int nOverlayLen, UINT nUnselectedN, UINT nUnselectedH, UINT nSelectedN, UINT nSelectedH)
{
	m_bmpTabUnselectedN.LoadBitmap(nUnselectedN);
	m_bmpTabUnselectedH.LoadBitmap(nUnselectedH);
	m_bmpTabSelectedN.LoadBitmap(nSelectedN);
	m_bmpTabSelectedH.LoadBitmap(nSelectedH);
	m_nOverlayLen = nOverlayLen;

	BITMAP bm;
	m_bmpTabUnselectedN.GetBitmap(&bm);
	m_nTabLen = bm.bmWidth;
	m_nTabHeight = bm.bmHeight;

	CRect rcWnd;
	GetWindowRect(rcWnd);
	SetWindowPos(NULL, 0, 0, rcWnd.Width(), bm.bmHeight, SWP_NOMOVE|SWP_NOZORDER);
}

//void CTabBar::DrawTab(Graphics& g, TAB_POS eTabPos, int nTabNum)
//{
//	Image* pImage = NULL;
//	switch (eTabPos)
//	{
//	case TAB_LEFT:
//		if(nTabNum == m_nCurSelTab)
//			pImage = m_pImgLeftDown;
//		else
//			pImage = m_pImgLeft;
//		break;
//	case TAB_CENTER:
//		if(nTabNum == m_nCurSelTab)
//			pImage = m_pImgCenterDown;
//		else
//			pImage = m_pImgCenter;
//		break;
//	case TAB_RIGHT:
//		if(nTabNum == m_nCurSelTab)
//			pImage = m_pImgRightDown;
//		else
//			pImage = m_pImgRight;
//		break;
//	default:
//		ASSERT(0);
//		break;
//	}
//
//	RectF rfTab(TAB_WIDTH*nTabNum, 0, TAB_WIDTH, pImage->GetHeight());
//	if(eTabPos == TAB_LEFT || eTabPos == TAB_CENTER)
//	{
//		g.DrawImage(pImage, rfTab, 0, 0, rfTab.Width, rfTab.Height, (Unit)UnitPixel);
//	}
//	else
//	{
//		g.DrawImage(pImage, rfTab, pImage->GetWidth()-TAB_WIDTH, 0, TAB_WIDTH, pImage->GetHeight(), (Unit)UnitPixel);
//	}
//
//	Pen penSpanLine(Color(35, 35, 35));
//	if (m_vTabItem.size() > 1 && nTabNum > 0)
//	{
//		g.DrawLine(&penSpanLine, Point(rfTab.X, rfTab.GetTop()), Point(rfTab.X, rfTab.GetBottom()-2));
//	}
//	else if (m_vTabItem.size() == 1 && nTabNum == 0)
//	{
//		g.DrawLine(&penSpanLine, Point(rfTab.GetRight(), rfTab.GetTop()), Point(rfTab.GetRight(), rfTab.GetBottom()-2));
//	}
//
//	StringFormat sf;
//	sf.SetAlignment(StringAlignmentCenter);
//	sf.SetLineAlignment(StringAlignmentCenter);
//	rfTab.Offset(0,-1);
//	g.DrawString(m_vTabItem.at(nTabNum).m_strName, -1, &m_textFontNormal, rfTab, &sf, &m_brushShadow);
//	rfTab.Offset(0,1);
//	g.DrawString(m_vTabItem.at(nTabNum).m_strName, -1, &m_textFontNormal, rfTab, &sf, &m_brushWhite);
//}

void CTabBar::DrawTab(CDC& memDC, TAB_ITEM& item, CRect& rcTabDrawing, bool bSelected)
{
	//int nTabLenShort = m_nTabLen - m_nOverlayLen;
	//CRect rcTabDrawing;
	//rcTabDrawing.SetRect(rcClient.left+nTabLenShort*i, rcClient.top, rcClient.left+nTabLenShort*i+m_nTabLen, rcClient.bottom);
	USES_CONVERSION;
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&memDC);
	CPoint ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);
	CBitmap* pTabBmp = NULL;
	if(!bSelected)
	{
		if(item.m_pRgnTab->PtInRegion(ptMouse))
			pTabBmp = &m_bmpTabUnselectedH;
		else
			pTabBmp = &m_bmpTabUnselectedN;
	}
	else
	{
		if(item.m_pRgnTab->PtInRegion(ptMouse))
			pTabBmp = &m_bmpTabSelectedH;
		else
			pTabBmp = &m_bmpTabSelectedN;
	}
		

	CBitmap* pOldBmp = dcBmp.SelectObject(pTabBmp);
	memDC.TransparentBlt(rcTabDrawing.left, rcTabDrawing.top, rcTabDrawing.Width(), rcTabDrawing.Height(), &dcBmp, 0, 0, rcTabDrawing.Width(), rcTabDrawing.Height(), RGB(0, 0, 0));

	Graphics g(memDC.m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	RectF rfTabText;
	rfTabText.X = rcTabDrawing.left + m_nOverlayLen;
	rfTabText.Y = rcTabDrawing.top;
	rfTabText.Width = rcTabDrawing.Width() - m_nOverlayLen;
	rfTabText.Height = rcTabDrawing.Height();

	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);
	//rfTab.Offset(0,-1);
	g.DrawString(A2W(item.m_strName), -1, &m_textFontNormal, rfTabText, &sf, &m_brushShadow);
	//rfTab.Offset(0,1);
	//g.DrawString(m_vTabItem.at(nTabNum).m_strName, -1, &m_textFontNormal, rfTab, &sf, &m_brushWhite);


	dcBmp.SelectObject(pOldBmp);
	dcBmp.DeleteDC();
}

bool CTabBar::SetTab(int nSelTab)
{
	if (nSelTab > m_vTabItem.size() - 1)
	{
		return false;
	}
	else
	{
		m_nCurSelTab = nSelTab;
		Invalidate();
		return true;
	}
}

bool CTabBar::SwitchTab(int nSelTab)
{
	if (nSelTab > m_vTabItem.size() - 1)
	{
		return false;
	}
	else
	{
		m_nCurSelTab = nSelTab;
		CWnd* pParent = GetParent();
		::PostMessage(pParent->m_hWnd, WM_COMMAND, m_vTabItem.at(nSelTab).m_uID, 0);
		Invalidate();
		return true;
	}
}


void CTabBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//Invalidate();
	int nTabNum = -1;
	for(int i = 0; i < m_vTabItem.size(); i++)
	{
		TAB_ITEM& item = m_vTabItem[i];
		if(item.m_pRgnTab->PtInRegion(point))
		{
			SwitchTab(i);
			break;
		}
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CTabBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//Invalidate();
	CStatic::OnLButtonUp(nFlags, point);
}

void CTabBar::OnMouseMove(UINT nFlags, CPoint point)
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

void CTabBar::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	m_bTrack=FALSE;
	Invalidate();
	CStatic::OnMouseLeave();
}

void CTabBar::RefreshTabRgn()
{
	CRect rcClient;
	GetClientRect(rcClient);

	for(int i = 0; i < m_vTabItem.size(); i++)
	{
		TAB_ITEM& item = m_vTabItem[i];
		if(item.m_pRgnTab)
		{
			if(item.m_pRgnTab->GetSafeHandle())
				item.m_pRgnTab->DeleteObject();
			delete item.m_pRgnTab;
			item.m_pRgnTab = NULL;
		}

		CRect rcTab;
		if(i == 0)
		{
			rcTab.SetRect(0, 0, m_nTabLen, m_nTabHeight);
		}
		else
		{
			int m_nTabShortLen = m_nTabLen - m_nOverlayLen;
			rcTab.SetRect(m_nTabShortLen*i+m_nOverlayLen, 0, m_nTabShortLen*(i+1)+m_nOverlayLen, m_nTabHeight);
		}

		item.m_pRgnTab = new CRgn;
		item.m_pRgnTab->CreateRectRgn(rcTab.left, rcTab.top, rcTab.right, rcTab.bottom);
	}
}
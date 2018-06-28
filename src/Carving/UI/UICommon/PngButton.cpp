// PngButton.cpp : implementation file
//

#include "stdafx.h"
#include "PngButton.h"
#include "MEMDC.h"

Bitmap* LoadPngImgFromRes(UINT uResID);
// CPngButton

IMPLEMENT_DYNAMIC(CPngButton, CButton)

CPngButton::CPngButton():
	m_pNormalBitmap(NULL),
	m_pHoveredBitmap(NULL),
	m_pDownBitmap(NULL),
	m_pDisabledBitmap(NULL),
	m_pFocusedBitmap(NULL),
	m_eCurentStatus(BS_NORMAL),
	m_bTrack(FALSE),
	textFontNormal(L"Segoe UI" ,9*96.0/72 ,FontStyleRegular,UnitPixel),
	//textFontBold(_T("Segoe UI") ,9*96.0/72 ,FontStyleBold,UnitPixel),
	textBrushNormal(Color(0,0,0)),
	textBrushGrey(Color(100,100,100)),
	textBrushHover(Color(255,255,255)),
	m_nShadowOffsetX(1),
	m_nShadowOffsetY(1)
{
	m_textFormat.SetAlignment(StringAlignmentNear);
	m_textFormat.SetLineAlignment(StringAlignmentCenter);
	m_bBold = FALSE;
	m_pCustomFont = NULL;
	m_bShadow = FALSE;
	m_nWordOffsetLeft = 0;
	m_nWordOffsetRight = 0;
	m_nWordOffsetTop = 0;
	m_nWordOffsetBottom = 0;
}

CPngButton::~CPngButton()
{
	if (m_pNormalBitmap != NULL)
		delete m_pNormalBitmap;	
	if (m_pHoveredBitmap != NULL)
		delete m_pHoveredBitmap;
	if (m_pDownBitmap != NULL)
		delete m_pDownBitmap;
	if (m_pDisabledBitmap != NULL)
		delete m_pDisabledBitmap;
	if (m_pFocusedBitmap != NULL)
		delete m_pFocusedBitmap;
	if (m_pCustomFont != NULL)
		delete m_pCustomFont;
}


BEGIN_MESSAGE_MAP(CPngButton, CButton)
//	ON_WM_DRAWITEM()
ON_WM_MOUSEMOVE()
ON_WM_MOUSELEAVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CPngButton message handlers



//void CPngButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}

void CPngButton::DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/)
{

	// TODO:  Add your code to draw the specified item
	ASSERT(lpDIS!=NULL);
	CRect rcClient,rcWindow;
	GetClientRect(rcClient);
	GetWindowRect(rcWindow);
	CDC*pDC=CDC::FromHandle(lpDIS->hDC);

	CMemoryDC memDC(pDC, rcClient);
	DrawBackground(&memDC);


	Graphics g(memDC.m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	CWnd* pParentWin=GetParent();
	if(!pParentWin->GetSafeHwnd())
		return;

	pParentWin->ScreenToClient(rcWindow);
	bool m_bMouseDown=false;
	CPoint ptCursorPos;
	GetCursorPos(&ptCursorPos);
	ScreenToClient(&ptCursorPos);
	if (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON)?VK_RBUTTON:VK_LBUTTON) & 0x8000)
	{
		m_bMouseDown=true;
	}
	Bitmap* pBitmap=NULL;

	SolidBrush* pTextBrush = &textBrushNormal;
	DWORD dw  = GetWindowLong(this->m_hWnd, GWL_STYLE);
	if(dw & WS_DISABLED)
	{
		pBitmap=m_pDisabledBitmap;
	}
	else if(GetFocus() == this && m_bMouseDown  && rcClient.PtInRect(ptCursorPos))
	{
		pBitmap=m_pDownBitmap;
		pTextBrush = &textBrushHover;
	}
	else if(rcClient.PtInRect(ptCursorPos))
	{
		pBitmap=m_pHoveredBitmap;
		pTextBrush = &textBrushHover;
	}
	else if((lpDIS->itemState & ODS_FOCUS) && m_pFocusedBitmap != NULL)
	{
		pBitmap=m_pFocusedBitmap;
	}
	else
	{
		pBitmap=m_pNormalBitmap;
	}
	
	RectF rf(0,0,rcWindow.Width(), rcWindow.Height());
	g.DrawImage(pBitmap,rf);

	CStringW szCaption;
	WCHAR chCaption[100];
	GetWindowTextW(m_hWnd, chCaption, 100);
	szCaption = chCaption;
	if(m_textFormat.GetAlignment() == StringAlignmentNear && m_nWordOffsetLeft == 0)
	{
		rf.X = 8;
		rf.Width = rf.Width - 8;
	}
	else if(m_textFormat.GetAlignment() == StringAlignmentFar && m_nWordOffsetRight == 0)
	{
		rf.Width = rf.Width - 8;
	}
	CRect rcTemp(rf.X, rf.Y, rf.X+rf.Width-1, rf.Y+rf.Height-1);
	rcTemp.left += m_nWordOffsetLeft;
	rcTemp.right += m_nWordOffsetRight;
	rcTemp.top += m_nWordOffsetTop;
	rcTemp.bottom += m_nWordOffsetBottom;
	rf.X = rcTemp.left;
	rf.Y = rcTemp.top;
	rf.Width = rcTemp.Width();
	rf.Height = rcTemp.Height();

	Gdiplus::Font* pFont = (m_pCustomFont != NULL)? m_pCustomFont : &textFontNormal;
	if(m_bShadow)
	{
		RectF rfShadow(rf.X + m_nShadowOffsetX, rf.Y + m_nShadowOffsetY, rf.Width, rf.Height);
		g.DrawString(szCaption ,-1, pFont, rfShadow, &m_textFormat, &textBrushGrey);
	}
	g.DrawString(szCaption ,-1, pFont, rf, &m_textFormat, /*&textBrushNormal*/pTextBrush);
}

void CPngButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CPngButton::LoadImageFromFile( WCHAR* szNormal, WCHAR* szOver, WCHAR* szDown, WCHAR* szDisable, WCHAR* szFocused)
{
	m_pNormalBitmap=Bitmap::FromFile(szNormal);
	m_pHoveredBitmap=Bitmap::FromFile(szOver);
	m_pDownBitmap=Bitmap::FromFile(szDown);
	if(szDisable != NULL)
	{
		m_pDisabledBitmap=Bitmap::FromFile(szDisable);
	}
	else
	{
		m_pDisabledBitmap=Bitmap::FromFile(szNormal);
	}
	if(szFocused != NULL)
	{
		m_pFocusedBitmap = Bitmap::FromFile(szFocused);
	}
	
	//CRect rcButton;
	//GetWindowRect(rcButton);
	SetWindowPos(NULL, 0, 0, m_pNormalBitmap->GetWidth(), m_pNormalBitmap->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
}

void CPngButton::LoadImageFromRes(UINT uNormal, UINT uOver, UINT uDown, UINT uDisable, UINT uFocused)
{
	m_pNormalBitmap=LoadPngImgFromRes(uNormal);
	m_pHoveredBitmap=LoadPngImgFromRes(uOver);
	m_pDownBitmap=LoadPngImgFromRes(uDown);
	if(uDisable != 0)
	{
		m_pDisabledBitmap=LoadPngImgFromRes(uDisable);
	}
	else
	{
		m_pDisabledBitmap=LoadPngImgFromRes(uNormal);
	}
	if(uFocused != 0)
	{
		m_pFocusedBitmap = LoadPngImgFromRes(uFocused);
	}

	//CRect rcButton;
	//GetWindowRect(rcButton);
	SetWindowPos(NULL, 0, 0, m_pNormalBitmap->GetWidth(), m_pNormalBitmap->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
}

void CPngButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bTrack)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize=sizeof(TRACKMOUSEEVENT);
		tme.dwFlags=TME_LEAVE;
		tme.dwHoverTime=HOVER_DEFAULT;
		tme.hwndTrack=m_hWnd;
		_TrackMouseEvent(&tme);
		m_bTrack=TRUE;
		Invalidate();
	}
	CButton::OnMouseMove(nFlags, point);
	
}

void CPngButton::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	m_bTrack=FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}

void CPngButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CButton::OnLButtonDown(nFlags, point);
}

void CPngButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}

void CPngButton::DrawBackground(CDC* pDC)
{
	CRect rcButton;
	GetWindowRect(rcButton);
	GetParent()->ScreenToClient(rcButton);
	if(!m_dcBk.GetSafeHdc())
	{
		CClientDC clDC(GetParent());
		m_dcBk.CreateCompatibleDC(&clDC);
		m_bmpBk.CreateCompatibleBitmap(&clDC,rcButton.Width(),rcButton.Height());
		m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rcButton.Width(), rcButton.Height(), &clDC, rcButton.left, rcButton.top, SRCCOPY);
	}
	pDC->BitBlt(0, 0, rcButton.Width(), rcButton.Height(), &m_dcBk,0,0,SRCCOPY);
}

BOOL CPngButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//DrawBackground(pDC);
	return TRUE;//CButton::OnEraseBkgnd(pDC);
}

void CPngButton::SetStringFormat(StringFormat& textFormat)
{
	m_textFormat.SetAlignment(textFormat.GetAlignment());
	m_textFormat.SetLineAlignment(textFormat.GetLineAlignment());
	m_textFormat.SetTrimming(textFormat.GetTrimming());
	m_textFormat.SetFormatFlags(textFormat.GetFormatFlags());
}

void CPngButton::SetShadow(BOOL bShadow/* = TRUE*/, int nOffsetX /*= 1*/, int nOffsetY /*= 1*/)
{
	m_bShadow = bShadow;
	m_nShadowOffsetX = nOffsetX;
	m_nShadowOffsetY = nOffsetY;
}
void CPngButton::SetCustomFont(WCHAR* szFontName, int nHight, FontStyle fontStyle/* = FontStyleRegular*/)
{
	if(m_pCustomFont != NULL)
		delete m_pCustomFont;
	m_pCustomFont = new Gdiplus::Font(szFontName , nHight/**96.0/72*/, fontStyle, UnitPixel);
}
void CPngButton::SetWordOffset(int nOffset, OFFSET_ORIENT offsetOrient)
{
	switch(offsetOrient)
	{
	case LEFT:
		m_nWordOffsetLeft = nOffset;
		break;
	case RIGHT:
		m_nWordOffsetRight = nOffset;
		break;
	case TOP:
		m_nWordOffsetTop = nOffset;
		break;
	case BOTTOM:
		m_nWordOffsetBottom = nOffset;
		break;
	}
}

void CPngButton::SetTextColorEx(COLORREF dwColor)
{
	BYTE r, g, b;
	r = (dwColor & 0x000000FF);
	g = (dwColor & 0x0000FF00)>>8;
	b = (dwColor & 0x00FF0000)>>16;
	textBrushNormal.SetColor(Color(r, g, b));
}

void CPngButton::SetShadowColor(COLORREF dwColor)
{
	BYTE r, g, b;
	r = (dwColor & 0x000000FF);
	g = (dwColor & 0x0000FF00)>>8;
	b = (dwColor & 0x00FF0000)>>16;
	textBrushGrey.SetColor(Color(r, g, b));
}

Bitmap* LoadPngImgFromRes(UINT uResID)
{
	HINSTANCE hInst = /*AfxGetResourceHandle()*/AfxGetModuleState()->m_hCurrentInstanceHandle;
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(uResID), _T("PNG")); // type
	if (!hRsrc)
		return NULL;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return NULL;

	// Allocate global memory on which to create stream
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pMem = (BYTE*)GlobalLock(hMem);
	memcpy(pMem,lpRsrc,len);
	IStream* pStm;
	CreateStreamOnHGlobal(hMem,FALSE,&pStm);

	Bitmap* pImg=Gdiplus::Bitmap::FromStream(pStm);

	// free/release stuff
	pStm->Release();
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	FreeResource(lpRsrc);

	return pImg;
}
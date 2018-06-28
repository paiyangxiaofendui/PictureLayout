// PngButton.cpp : implementation file
//

#include "stdafx.h"
#include "PngButton_Stretchable.h"
#include "MEMDC.h"

#define EDGE_WIDTH 11
//Bitmap* LoadPngImgFromRes(UINT uResID);
// CPngButton

IMPLEMENT_DYNAMIC(CPngButton_Stretchable, CPngButton)

CPngButton_Stretchable::CPngButton_Stretchable()
{

}

CPngButton_Stretchable::~CPngButton_Stretchable()
{

}

BEGIN_MESSAGE_MAP(CPngButton_Stretchable, CPngButton)
END_MESSAGE_MAP()

void CPngButton_Stretchable::DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/)
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
	//g.DrawImage(pBitmap,rf);
	RectF rfSource, rfDest;
	rfSource.X = 0;
	rfSource.Y = 0;
	rfSource.Width = EDGE_WIDTH;
	rfSource.Height = rf.Height;
	rfDest = rf;
	rfDest.Width = EDGE_WIDTH;
	g.DrawImage(pBitmap, rfDest, rfSource.X, rfSource.Y, rfSource.Width, rfSource.Height, UnitPixel);

	rfSource.X = pBitmap->GetWidth()-EDGE_WIDTH;
	rfSource.Y = 0;
	rfSource.Width = EDGE_WIDTH;
	rfSource.Height = rf.Height;
	rfDest = rf;
	rfDest.Width = EDGE_WIDTH;
	rfDest.X = rf.X + rf.Width - EDGE_WIDTH;
	g.DrawImage(pBitmap, rfDest, rfSource.X, rfSource.Y, rfSource.Width, rfSource.Height, UnitPixel);

	int nCurStartPos = rf.X + EDGE_WIDTH;
	int nMidImgLen = pBitmap->GetWidth() - EDGE_WIDTH*2;
	while(nCurStartPos < rf.GetRight() - EDGE_WIDTH)
	{
		int nPasteLen;
		if((rf.GetRight() - EDGE_WIDTH) - nCurStartPos > nMidImgLen)
			nPasteLen = nMidImgLen;
		else
			nPasteLen = (rf.GetRight() - EDGE_WIDTH) - nCurStartPos;
		rfSource.X = EDGE_WIDTH;
		rfSource.Y = 0;
		rfSource.Width = nPasteLen;
		rfSource.Height = rf.Height;
		rfDest.X = nCurStartPos;
		rfDest.Y = rf.Y;
		rfDest.Width = nPasteLen;
		rfDest.Height = rf.Height;
		g.DrawImage(pBitmap, rfDest, rfSource.X, rfSource.Y, rfSource.Width, rfSource.Height, UnitPixel);
		nCurStartPos += nPasteLen;
	}

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

//void CPngButton::LoadImageFromFile( WCHAR* szNormal, WCHAR* szOver, WCHAR* szDown, WCHAR* szDisable, WCHAR* szFocused)
//{
//	m_pNormalBitmap=Bitmap::FromFile(szNormal);
//	m_pHoveredBitmap=Bitmap::FromFile(szOver);
//	m_pDownBitmap=Bitmap::FromFile(szDown);
//	if(szDisable != NULL)
//	{
//		m_pDisabledBitmap=Bitmap::FromFile(szDisable);
//	}
//	else
//	{
//		m_pDisabledBitmap=Bitmap::FromFile(szNormal);
//	}
//	if(szFocused != NULL)
//	{
//		m_pFocusedBitmap = Bitmap::FromFile(szFocused);
//	}
//	
//	//CRect rcButton;
//	//GetWindowRect(rcButton);
//	SetWindowPos(NULL, 0, 0, m_pNormalBitmap->GetWidth(), m_pNormalBitmap->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
//}
//
//void CPngButton::LoadImageFromRes(UINT uNormal, UINT uOver, UINT uDown, UINT uDisable, UINT uFocused)
//{
//	m_pNormalBitmap=LoadPngImgFromRes(uNormal);
//	m_pHoveredBitmap=LoadPngImgFromRes(uOver);
//	m_pDownBitmap=LoadPngImgFromRes(uDown);
//	if(uDisable != 0)
//	{
//		m_pDisabledBitmap=LoadPngImgFromRes(uDisable);
//	}
//	else
//	{
//		m_pDisabledBitmap=LoadPngImgFromRes(uNormal);
//	}
//	if(uFocused != 0)
//	{
//		m_pFocusedBitmap = LoadPngImgFromRes(uFocused);
//	}
//
//	//CRect rcButton;
//	//GetWindowRect(rcButton);
//	SetWindowPos(NULL, 0, 0, m_pNormalBitmap->GetWidth(), m_pNormalBitmap->GetHeight(), SWP_NOMOVE|SWP_NOZORDER);
//}
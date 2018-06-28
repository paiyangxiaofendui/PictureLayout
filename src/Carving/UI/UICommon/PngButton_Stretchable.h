#pragma once
#include "PngButton.h"

// CPngButton

class CPngButton_Stretchable : public CPngButton
{
	DECLARE_DYNAMIC(CPngButton_Stretchable)

public:
	CPngButton_Stretchable();
	virtual ~CPngButton_Stretchable();
	//enum eButtonState{BS_NORMAL, BS_OVER, BS_PRESSED, BS_NOUSE} m_eCurentStatus;
	//enum OFFSET_ORIENT{LEFT = 0, RIGHT, TOP, BOTTOM};
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	//void LoadImageFromFile( WCHAR* szNormal, WCHAR* szOver, WCHAR* szDown, WCHAR* szDisable, WCHAR* szFocused);
	//void LoadImageFromRes(UINT uNormal, UINT uOver, UINT uDown, UINT uDisable, UINT uFocused);
	//void SetStringFormat(StringFormat& textFormat);
	//void SetShadow(BOOL bShadow = TRUE, int nOffsetX = 1, int nOffsetY = 1);
	//void SetCustomFont(WCHAR* szFontName, int nHight, FontStyle fontStyle = FontStyleRegular);
	//void SetWordOffset(int nOffset, OFFSET_ORIENT offsetOrient);
	//void SetTextColorEx(COLORREF dwColor);
	//void SetShadowColor(COLORREF dwColor);
protected:
	//virtual void PreSubclassWindow();
//	virtual void DrawBackground(CDC*pDC);
//	Bitmap* m_pNormalBitmap;
//	Bitmap* m_pHoveredBitmap;
//	Bitmap* m_pDownBitmap;
//	Bitmap* m_pDisabledBitmap;
//	Bitmap* m_pFocusedBitmap;
//	BOOL m_bTrack;
//	CDC m_dcBk;
//	CBitmap m_bmpBk;
//
//	Gdiplus::Font textFontNormal;
//	SolidBrush textBrushNormal;
//	SolidBrush textBrushGrey;
//	SolidBrush textBrushHover;
//	StringFormat m_textFormat;
//	BOOL m_bBold;
//	Gdiplus::Font* m_pCustomFont;
//	BOOL m_bShadow;
//	int m_nWordOffsetLeft;
//	int m_nWordOffsetRight;
//	int m_nWordOffsetTop;
//	int m_nWordOffsetBottom;
//	int m_nShadowOffsetX;
//	int m_nShadowOffsetY;
//public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnMouseLeave();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



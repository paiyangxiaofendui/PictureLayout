// SkinListCtrlBase.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "SkinListCtrlBase.h"


// CSkinListCtrlBase

IMPLEMENT_DYNAMIC(CSkinListCtrlBase, CListCtrl)

CSkinListCtrlBase::CSkinListCtrlBase()
{

}

CSkinListCtrlBase::~CSkinListCtrlBase()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrlBase, CListCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()



// CSkinListCtrlBase 消息处理程序
void CSkinListCtrlBase::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 32;
}

void CSkinListCtrlBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages
	CRect rect;
	CDC dcMem; 
	CBitmap bmpMem;

	GetClientRect(&rect);   
	dcMem.CreateCompatibleDC(&dc);  
	bmpMem.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	CBitmap* pOldMemDCBmp = dcMem.SelectObject(&bmpMem);

	dcMem.FillSolidRect(&rect,GetBkColor());   
	//Firstly, use default painting to paint to memory DC.  
	DefWindowProc(WM_PAINT,(WPARAM)dcMem.m_hDC,(LPARAM)0); 

	//Secondly, BitBlt memory DC bitmap to device DC.
	//In report style, do not paint header block to device DC, to avoid overwriting header self-draw image.
	DWORD dwStyle = GetWindowLong( this->m_hWnd, GWL_STYLE);
	if((dwStyle & LVS_TYPEMASK) == LVS_REPORT)
	{
		CRect headerRect(0, 0, 0, 0);
		CWnd* pHeaderWnd = GetDlgItem(0);
		if(pHeaderWnd->GetSafeHwnd())
		{
			pHeaderWnd->GetWindowRect(&headerRect);  
		}

		//draw list round conner to memory dc
		//ImageAttributes ia;
		//ia.SetColorKey(Color(230,230,230), Color(230, 230, 230)/*, ColorAdjustTypeBitmap*/);
		//RectF rfCorner(rect.right-(m_pImgListCorner->GetWidth()-1), rect.top, m_pImgListCorner->GetWidth()-1, m_pImgListCorner->GetHeight()-1);
		//Graphics g(dcMem.m_hDC);
		//g.DrawImage(m_pImgListCorner, rfCorner, 0, 1, m_pImgListCorner->GetWidth()-1, m_pImgListCorner->GetHeight()-1, UnitPixel, &ia);

		dc.BitBlt(0,headerRect.Height(),rect.Width(),  rect.Height() - headerRect.Height(),&dcMem,0, headerRect.Height(),SRCCOPY);
		//dc.BitBlt(rfCorner.X, rfCorner.Y, rfCorner.Width,  rfCorner.Height, &dcMem, rfCorner.X, rfCorner.Y,SRCCOPY);
	}
	else
	{
		dc.BitBlt(0,0,rect.Width(),  rect.Height(),&dcMem,0, 0,SRCCOPY); 
	}

	dcMem.SelectObject(pOldMemDCBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
}

BOOL CSkinListCtrlBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CListCtrl::OnEraseBkgnd(pDC);
}

void CSkinListCtrlBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	USES_CONVERSION;
	//::TextOut(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, "a", 1);
	Graphics g(lpDrawItemStruct->hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);
	sf.SetTrimming( StringTrimmingEllipsisCharacter);
	sf.SetFormatFlags(StringFormatFlagsLineLimit);
	Gdiplus::Font font(L"Segoe UI", 14, FontStyleRegular, UnitPixel);
	SolidBrush brushText(Color(0, 0, 0));
	SolidBrush brushTextFocused(Color(255, 255, 255));
	SolidBrush brushFocusedBk(Color(49, 106, 197));
	SolidBrush brushLightBk(Color(240, 240, 255));
	Pen penLine(Color(150, 200, 250), 1);

	SolidBrush* pTextBursh = &brushText;
	if(lpDrawItemStruct->itemState & LVIS_FOCUSED)
	{
		g.FillRectangle(&brushFocusedBk, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top -1);
		pTextBursh = &brushTextFocused;
	}
	else if((lpDrawItemStruct->itemID % 2) == 1)
	{
		g.FillRectangle(&brushLightBk, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top -1);
	}
	int nColumnCount = 0;
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	if(pHeaderCtrl)
		nColumnCount = pHeaderCtrl->GetItemCount();
	for(int i = 0; i < nColumnCount; i++)
	{
		CRect rcSubItem;
		GetSubItemRect(lpDrawItemStruct->itemID, i, /*LVIR_ICON | */LVIR_LABEL, rcSubItem);
		RectF rfText(rcSubItem.left, rcSubItem.top, rcSubItem.Width(), rcSubItem.Height());
		CString strText = GetItemText(lpDrawItemStruct->itemID, i);


		g.DrawString(A2W(strText), -1, &font, rfText, &sf, pTextBursh);

		g.DrawLine(&penLine, rcSubItem.right, rcSubItem.top, rcSubItem.right, rcSubItem.bottom);

		//if(i == nColumnCount - 1)
		//	g.DrawLine(&penLine, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom -1, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom -1);
	}
	//if(lpDrawItemStruct->itemID == GetItemCount() - 1)
	g.DrawLine(&penLine, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom -1, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom -1);
}

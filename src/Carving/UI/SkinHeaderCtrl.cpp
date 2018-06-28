// SkinHeaderCtrl.cpp : 实现文件
//

#include "stdafx.h"
//#include "OfficeUI_Demo.h"
#include "SkinHeaderCtrl.h"
#include "./UICommon/MEMDC.H"


// CSkinHeaderCtrl

IMPLEMENT_DYNAMIC(CSkinHeaderCtrl, CHeaderCtrl)

CSkinHeaderCtrl::CSkinHeaderCtrl()
	: m_eState(State::STATE_NORMAL)
{
	m_eSortOrderType = 0;
	m_nSortintSubItemID = -1;
}

CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()



// CSkinHeaderCtrl 消息处理程序
LRESULT CSkinHeaderCtrl::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 
	int nHeight = (int)(/*pwpos->cy * m_Height*/30); //改变高度,m_Height为倍数
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}

BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;//CHeaderCtrl::OnEraseBkgnd(pDC);
}

void CSkinHeaderCtrl::OnPaint()
{
	USES_CONVERSION;
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CHeaderCtrl::OnPaint() for painting messages
	CRect rcClient;
	GetClientRect(rcClient);
	//rcClient.InflateRect(0, 0, 10, 0);
	CMemoryDC memDC(&dc, rcClient);
	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc); 

	memDC.FillSolidRect(rcClient.left, rcClient.top, rcClient.Width()/*+10*/, rcClient.Height(), RGB(255,255,255));

	CPen penEdge(PS_SOLID, 1, RGB(67, 149, 250));
	CPen penEdge1(PS_SOLID, 1, RGB(238, 245, 255));
	//CPen* pOldPen = memDC->SelectObject(&penEdge);
	//memDC.MoveTo(0, rcClient.bottom-1);
	//memDC->LineTo(rcClient.right, rcClient.bottom-1);
	//memDC->SelectObject(pOldPen);
	Graphics g(memDC.m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	//RectF rfBk(0, 0,  rcClient.Width()+2, rcClient.Height());
	//g.Rec

	//for(vector<CSortArrowButton*>::const_iterator itr = m_vSortButton.begin(); itr != m_vSortButton.end(); itr++)
	//{
	//	if((*itr)->Width() > 0)
	//		(*itr)->Draw(g);
	//}

	int nItems = GetItemCount();
	for(int i = 0; i <nItems; i++)
	{
	//	if (i == 0)
	//		continue;

	//	if(i == 1)
	//	{
	//		m_btnCheckBox.Draw(g);
	//	}
		TCHAR szText[MAX_PATH];
		CRect rcItemTitleArea;
		HD_ITEM hdItem;

		hdItem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hdItem.pszText = szText;
		hdItem.cchTextMax = MAX_PATH;
		GetItem(i, &hdItem );
		rcItemTitleArea = GetTitleDrawingArea(i);
		//GetItemRect(i, &rcItemTitleArea);

		LinearGradientBrush brush(Point(rcItemTitleArea.left, rcItemTitleArea.top), Point(rcItemTitleArea.left, rcItemTitleArea.bottom), Color(255, 255, 255, 255), \
			Color(255, (m_eState == STATE_WARNING) ? 255 : 140, (m_eState == STATE_WARNING) ? 0 : 176, (m_eState == STATE_WARNING) ? 0 : 230));
		g.FillRectangle(&brush, rcItemTitleArea.left, rcItemTitleArea.top, rcItemTitleArea.Width(), rcItemTitleArea.Height());


	//	//CBitmap* pOldBitmap = NULL;

	//	//if(i != 0)
	//	//{
			CPen* pOldPen = memDC.SelectObject(&penEdge);
			memDC.MoveTo(rcItemTitleArea.right, 0);
			memDC.LineTo(rcItemTitleArea.right, rcItemTitleArea.bottom-1);
			memDC.LineTo(rcItemTitleArea.left, rcItemTitleArea.bottom-1);
			/*memDC.SelectObject(&penEdge1);
			memDC.MoveTo(rcItem.right + 1, 1);
			memDC.LineTo(rcItem.right + 1, rect.bottom);*/
			memDC.SelectObject(pOldPen);
	//	//}





	//	CRect rcText(rcItem);
	//	if (i == 1)
	//	{
	//		rcText.left = m_btnCheckBox.right - 3;
	//	}
	//	if (rcText.Width() <= 8 + 2)
	//	{
	//		m_vSortButton.at(i)->SetRect(rcItem.left, 0, rcItem.left, 0);
	//		continue;
	//	}
	//	rcText.DeflateRect(8, 2, 2/*12 + m_pImgArrowUp->GetWidth()*/, 2);
	//	RectF rfText(rcText.left, rcText.top, rcText.Width(), rcText.Height());
	//	

		RectF rfText(rcItemTitleArea.left, rcItemTitleArea.top, rcItemTitleArea.Width(), rcItemTitleArea.Height());
		StringFormat sf;
		sf.SetAlignment(StringAlignmentCenter);
		sf.SetLineAlignment(StringAlignmentCenter);
		sf.SetTrimming( StringTrimmingEllipsisCharacter);
		sf.SetFormatFlags(StringFormatFlagsLineLimit);
		Gdiplus::Font font(L"Segoe UI", 14, FontStyleRegular, UnitPixel);
		SolidBrush brushText(Color(0, 0, 0));
		
		g.DrawString(A2W(szText), -1, &font, rfText, &sf, &brushText);

	//	RectF rfTextTest;
	//	g.MeasureString(szText, -1, &font, PointF(rfText.X, rfText.Y), &rfTextTest);
	//	//g.DrawRectangle(&Pen(Color(Color::Red)), rfTextTest);

	//	CRect rcSortButton(rcItem);
	//	rcSortButton.left = rcText.left;
	//	if (rcSortButton.Width() <= rfTextTest.Width + 6 + m_pImgArrowUp->GetWidth() + 12)
	//	{
	//		m_vSortButton.at(i)->SetRect(rcItem.left, 0, rcItem.left, 0);
	//		continue;
	//	}
	//	else
	//	{
	//		int nSortButtonTop = rcSortButton.top + (rcSortButton.Height() - m_pImgArrowUp->GetHeight()) / 2;
	//		int nSortButtonLeft = rcSortButton.left + rfTextTest.Width + 6;
	//		m_vSortButton.at(i)->SetRect(nSortButtonLeft, nSortButtonTop, nSortButtonLeft + m_pImgArrowUp->GetWidth(), nSortButtonTop + m_pImgArrowUp->GetHeight());
	//		m_vSortButton.at(i)->Draw(g);
	//	}

		if(i == m_nSortintSubItemID)
		{
			Image* pSortImg = (m_eSortOrderType == 0) ? m_pImgSortAscending : m_pImgSortDescending;
			RectF rfSortIcon(rcItemTitleArea.left+(rcItemTitleArea.Width()-pSortImg->GetWidth())/2, \
				rcItemTitleArea.top, \
				pSortImg->GetWidth(), \
				pSortImg->GetHeight());
			
			g.DrawImage(pSortImg, rfSortIcon, 0, 0, pSortImg->GetWidth(), pSortImg->GetHeight(), UnitPixel);
		}
	}
}

void CSkinHeaderCtrl::SetSortingImg(Image* pImgSortAscending, Image* pImgSortDescending)
{
	m_pImgSortAscending = pImgSortAscending;
	m_pImgSortDescending = pImgSortDescending;
}

void CSkinHeaderCtrl::SetSortingSubItem(int nSortintSubItemID, int eSortOrderType)
{
	m_nSortintSubItemID = nSortintSubItemID;
	m_eSortOrderType = eSortOrderType;
	Invalidate();
}

CRect CSkinHeaderCtrl::GetTitleDrawingArea(int nItem)
{
	CRect rcRet;
	GetItemRect(nItem, &rcRet);
	return rcRet;
}

void CSkinHeaderCtrl::SetState(State eState)
{
	m_eState = eState;
	Invalidate();
}
// ListTotalResult.cpp : 实现文件
//

#include "stdafx.h"
//#include "../OfficeUI_Demo.h"
#include "ListTotalResult.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../Resource.h"


// CListTotalResult

IMPLEMENT_DYNAMIC(CListTotalResult, CSkinListCtrlBase)

CListTotalResult::CListTotalResult()
	:m_pImgUncheck(NULL)
	,m_pImgCheck(NULL)
{
	m_pImgUncheck = LoadPngImgFromRes(IDB_PNG_CHECKBOX_UNCHECK);
	m_pImgCheck = LoadPngImgFromRes(IDB_PNG_CHECKBOX_CHECK);
}

CListTotalResult::~CListTotalResult()
{
	if(m_pImgUncheck)
		delete m_pImgUncheck;
	if(m_pImgCheck)
		delete m_pImgCheck;
}

BEGIN_MESSAGE_MAP(CListTotalResult, CSkinListCtrlBase)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	//ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CLICK, &CListTotalResult::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CListTotalResult::OnNMDblclk)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CListTotalResult 消息处理程序
void CListTotalResult::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 93;
}

void CListTotalResult::OnPaint()
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

	DrawItems(dcMem);


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

//BOOL CListTotalResult::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//	return TRUE;
//	return CListCtrl::OnEraseBkgnd(pDC);
//}

void CListTotalResult::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{

	// TODO:  Add your code to draw the specified item
}

void CListTotalResult::DrawItems(CDC& dc)
{
	USES_CONVERSION;
	Graphics g(dc.m_hDC);
	CPen penEdge(PS_SOLID, 1, RGB(67, 149, 250));
	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);
	sf.SetTrimming( StringTrimmingEllipsisCharacter);
	sf.SetFormatFlags(StringFormatFlagsLineLimit);
	Gdiplus::Font font(L"Segoe UI", 15, FontStyleRegular, UnitPixel);
	Gdiplus::Font fontThicknessAndMaterial(L"Segoe UI", 10, FontStyleRegular, UnitPixel);
	SolidBrush brushText(Color(0, 0, 0));
	SolidBrush brushTextSelected(Color(220, 20, 60));

	int nSelectedItemID = GetNextItem(-1, LVIS_SELECTED);
	int nColumnCount = 0;
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	if(pHeaderCtrl)
		nColumnCount = pHeaderCtrl->GetItemCount();

	for(int i = 0; i < GetItemCount(); i++)
	{
		bool bSelected = (i == nSelectedItemID) ? true : false;

		PanelViewingParam* pParam = (PanelViewingParam*)GetItemData(i);

		for(int j = 0; j < nColumnCount; j++)
		{
			CRect rcSubItem;
			GetSubItemRect(i, j, /*LVIR_ICON | */LVIR_LABEL, rcSubItem);
			//dc.Rectangle(rcSubItem);

			if(j == 0)
			{
				rcSubItem.left = 0;
				LinearGradientBrush brush(Point(rcSubItem.left, rcSubItem.top), Point(rcSubItem.left, rcSubItem.bottom), Color(255, 220, 235, 255), bSelected ? Color(255, 249, 240, 231): Color(255, 140, 176, 230));
				g.FillRectangle(&brush, rcSubItem.left, rcSubItem.top, rcSubItem.Width(), rcSubItem.Height());

				CString strText;
				strText.Format("#%d", i+1);
				RectF rfText(rcSubItem.left, rcSubItem.top, rcSubItem.Width(), rcSubItem.Height());

				g.DrawString(A2W(strText), -1, &font, rfText, &sf, bSelected ? &brushTextSelected : &brushText);

				CPen* pOldPen = dc.SelectObject(&penEdge);
				dc.MoveTo(rcSubItem.right, 0);
				dc.LineTo(rcSubItem.right, rcSubItem.bottom-1);
				dc.LineTo(rcSubItem.left, rcSubItem.bottom-1);
				dc.SelectObject(pOldPen);

				Image* pImgCheck = pParam->m_bChecked ? m_pImgCheck : m_pImgUncheck;
				CRect rcCheck = GetCheckRect(i);
				g.DrawImage(pImgCheck, \
					RectF(rcCheck.left, rcCheck.top, rcCheck.Width(), rcCheck.Height()), \
					0, 0, pImgCheck->GetWidth(), pImgCheck->GetHeight(), \
					UnitPixel);
			}
			else
			{
				if(bSelected)
				{
					LinearGradientBrush brush(Point(rcSubItem.left, rcSubItem.top), Point(rcSubItem.left, rcSubItem.top+23), Color(255, 255, 229, 226), Color(255, 246, 244, 244));
					g.FillRectangle(&brush, rcSubItem.left+1, rcSubItem.top, rcSubItem.Width(), 23);
				}
				CPen* pOldPen = dc.SelectObject(&penEdge);
				dc.MoveTo(rcSubItem.right, rcSubItem.top);
				dc.LineTo(rcSubItem.right, rcSubItem.top+23);
				dc.LineTo(rcSubItem.left, rcSubItem.top+23);
				dc.MoveTo(rcSubItem.left, rcSubItem.bottom-1);
				dc.LineTo(rcSubItem.right, rcSubItem.bottom-1);
				dc.SelectObject(pOldPen);

				CString strText;
				RectF rfText(rcSubItem.left, rcSubItem.top, rcSubItem.Width(), 23);
				if(j == 1)
				{		
					//strText.Format(_T("1000*2000"));
					strText= GetItemText(i, j);
				}
				else if(j == 2)
				{
					//strText.Format(_T("95%%"));
					strText = GetItemText(i, j);
					float fValue = atof(strText);
					fValue = fValue * 100;
					strText.Format(_T("%s%%"), GetFloatString(fValue, 1));
				}
				else if(j == 3)
				{
					strText.Format(_T("1"));
				}
				else if(j == 4)
				{
					strText = GetItemText(i, j);
				}
				g.DrawString(A2W(strText), -1, &font, rfText, &sf, bSelected ? &brushTextSelected : &brushText);
			}
		}
		//LV_ITEM item;
		//memset(&item, 0, sizeof(LV_ITEM));
		
		CRect rcSubItem, rcItem, rcPanelDrawArea;
		GetSubItemRect(i, 0, LVIR_LABEL, rcSubItem);
		GetItemRect(i, rcItem, LVIR_BOUNDS);
		rcPanelDrawArea.SetRect(rcSubItem.right, rcItem.top+23, rcItem.right, rcItem.bottom);

		int nPanelLen = /*pParam->m_pPanel->m_RealLength*/pParam->m_pPanel->m_OrgLen;
		int nPanelWidth = /*pParam->m_pPanel->m_RealWidth*/pParam->m_pPanel->m_OrgWidth;

		float fScale = GetScale(nPanelLen, nPanelWidth, rcPanelDrawArea.Width()-10, rcPanelDrawArea.Height()-10);
		CRect rcPanelArea(rcPanelDrawArea.left + 10/2, rcPanelDrawArea.top + 10/2, rcPanelDrawArea.right-10/2, rcPanelDrawArea.bottom-10/2);

		CRect rcPanel(rcPanelDrawArea.left+(rcPanelDrawArea.Width()-nPanelLen*fScale)/2, \
			rcPanelDrawArea.top+(rcPanelDrawArea.Height()-nPanelWidth*fScale)/2, \
			rcPanelDrawArea.left+(rcPanelDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
			rcPanelDrawArea.top+(rcPanelDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

		if(bSelected)
			g.FillRectangle(&SolidBrush(Color(255, 255, 240, 230)), rcPanelDrawArea.left+1, rcPanelDrawArea.top+1, rcPanelDrawArea.Width()-2, rcPanelDrawArea.Height()-2);
		g.DrawRectangle(&Pen(Color(255, 67, 149, 250), 1), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height()-1);

		Panel& thePanel = *(pParam->m_pPanel);
		vector<Component*> vAllComponent;
		FindAllComponentInPanel(thePanel, vAllComponent);

		for(int i = 0; i < vAllComponent.size(); i++)
		{
			Component& theComponent = *(vAllComponent[i]);
			CRect rcComponent(theComponent.m_x*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - theComponent.m_y)*fScale);
			rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

			g.DrawRectangle(&Pen(Color(67, 149, 250), 1), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
			//CString str;
			//str.Format("%d", theComponent.m_id);

			//g->DrawString(AnsiToUnicode(str).c_str(), -1, &font, PointF((rcComponent.left + rcComponent.right)/2, (rcComponent.top + rcComponent.bottom)/2), &sf, &brush);
		}

		CString strThicknessAndMaterial;
		strThicknessAndMaterial.Format(_T("%s-%s"), GetFloatString(thePanel.m_Thickness, 1), thePanel.m_Material);
		RectF rfTextThicknessAndMaterial(rcPanelDrawArea.left, rcPanelDrawArea.top, rcPanelDrawArea.Width(), rcPanelDrawArea.Height());
		StringFormat sfThicknessAndMaterial;
		g.DrawString(A2W(strThicknessAndMaterial), -1, &fontThicknessAndMaterial, rfTextThicknessAndMaterial, &sfThicknessAndMaterial, bSelected ? &brushTextSelected : &brushText);
	}
}

void CListTotalResult::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	OnClick();
	CListCtrl::OnLButtonUp(nFlags, point);
}

CRect CListTotalResult::GetCheckRect(int nItem)
{
	CRect rcCheck;
	if(nItem >= 0 && nItem < GetItemCount())
	{
		CRect rcSubItem;
		GetSubItemRect(nItem, 0, LVIR_LABEL, rcSubItem);
		rcSubItem.left = 0;
		rcCheck.SetRect(rcSubItem.left, rcSubItem.bottom-m_pImgCheck->GetHeight(), rcSubItem.left + m_pImgCheck->GetWidth(), rcSubItem.bottom);
	}
	return rcCheck;
}

void CListTotalResult::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnClick();

	*pResult = 0;
}

void CListTotalResult::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnClick();
	*pResult = 0;
}

void CListTotalResult::OnClick()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	for(int i = 0; i < GetItemCount(); i++)
	{
		CRect rcItemCheck = GetCheckRect(i);
		if(rcItemCheck.PtInRect(ptCursor))
		{
			PanelViewingParam* pParam = (PanelViewingParam*)GetItemData(i);
			pParam->m_bChecked = !pParam->m_bChecked;
			Invalidate();
			break;
		}
	}
}

BOOL CListTotalResult::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	for(int i = 0; i < GetItemCount(); i++)
	{
		CRect rcItemCheck = GetCheckRect(i);
		if(rcItemCheck.PtInRect(ptCursor))
		{
			SetCursor(LoadCursor(NULL,IDC_HAND)); 
			return TRUE;
		}
	}

	return CListCtrl::OnSetCursor(pWnd, nHitTest, message);
}
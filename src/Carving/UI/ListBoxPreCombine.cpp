// ListBoxClipBoard.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "ListBoxPreCombine.h"
//#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../Drawing/Drawing.h"
//#include "../Misc/Misc.h"
//#include "../Resource.h"
//#include "../Misc/ProgramMisc.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../DataProcess/DataProcess.h"
#include "../Misc/ProgramMisc.h"
#include "../resource.h"

// CListBoxClipBoard
#define PRECOMBINE_ITEM_HEIGHT 150
#define GAP 10

int CListBoxPreCombine::m_nCurID = 0;

IMPLEMENT_DYNAMIC(CListBoxPreCombine, CListBox)

CListBoxPreCombine::CListBoxPreCombine()
{
	//m_pImgRotateClockwise90 = LoadPngImgFromRes(IDB_PNG_ROTATE_CLOCKWISE_90);
	//m_pImgRotateAntiClockwise90 = LoadPngImgFromRes(IDB_PNG_ROTATE_ANTICLOCKWISE_90);
	m_pImgNO = LoadPngImgFromRes(IDB_PNG_FLAG_NO);
}

CListBoxPreCombine::~CListBoxPreCombine()
{
	//if(m_pImgRotateClockwise90)
	//	delete m_pImgRotateClockwise90;
	//if(m_pImgRotateAntiClockwise90)
	//	delete m_pImgRotateAntiClockwise90;
	if(m_pImgNO)
		delete m_pImgNO;
}


BEGIN_MESSAGE_MAP(CListBoxPreCombine, CListBox)
	////ON_NOTIFY_REFLECT(NM_CLICK, &CListBoxClipBoard::OnNMClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CListBoxClipBoard 消息处理程序
void CListBoxPreCombine::AddItem(vector<PreCombineComponent>& theItem)
{
	CString strItemText;
	strItemText.Format("%d", m_nCurID);
	int nItem = InsertString(GetCount(), strItemText);
	m_mapIDAndItem.insert(make_pair(m_nCurID, theItem));
	m_nCurID++;

	SetCurSel(GetCount()-1);
	SetItemHeight(GetCount()-1, PRECOMBINE_ITEM_HEIGHT);
}

////例如CNewListBox继承自CListBox，重载虚MeasureItem和DrawItem这两个虚函数，代码如下：
//
void CListBoxPreCombine::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	USES_CONVERSION;

	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	PreCombineItem precobmineItem = GetPreCombineItem(lpDrawItemStruct->itemID);


	//ASSERT(lpszText != NULL);
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);

	Graphics g(dc.GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeHighQuality);

	StringFormat sf;
	sf.SetTrimming( StringTrimmingEllipsisCharacter);
	Gdiplus::Font font(L"Segoe UI", 10, FontStyleRegular, UnitPixel);
	SolidBrush brushText(Color(0, 0, 0));
	COLORREF colorHightLightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	SolidBrush brushTextFocused(Color(GetRValue(colorHightLightText), GetGValue(colorHightLightText), GetBValue(colorHightLightText)));
	SolidBrush* pBursh = NULL;

	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		//dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		//dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		pBursh = &brushTextFocused;
	}
	else
	{
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255,255,255));
		pBursh = &brushText;
	}
	
	//CString strText;
	//strText = pComponent->m_BarCode + _T("\n") + GetFloatString(pComponent->m_Thickness, 1) + _T("-") + pComponent->m_Material + _T("\n") \
	//	+ GetFloatString(pComponent->m_RealLength, 0) + _T("*") + GetFloatString(pComponent->m_RealWidth, 0);
	//RectF rfText(lpDrawItemStruct->rcItem.left + 5, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top);
	//g.DrawString(A2W(strText), -1, &font, rfText, &sf, pBursh);

	//CRect rcDiagram = lpDrawItemStruct->rcItem;
	//rcDiagram.DeflateRect(1, 50, 1, 1);

	//DrawingPanelParam param;
	//param.m_nGap = 10;
	//DrawComponent(&dc, pComponent, rcDiagram, param);

	//RectF rfRotateIcon(lpDrawItemStruct->rcItem.right - m_pImgRotateClockwise90->GetWidth(), \
	//	lpDrawItemStruct->rcItem.top, \
	//	m_pImgRotateClockwise90->GetWidth(), \
	//	m_pImgRotateClockwise90->GetHeight());
	//g.DrawImage(/*pComponent->m_bRotated ? m_pImgRotateAntiClockwise90 : */m_pImgRotateClockwise90, rfRotateIcon, 0, 0, m_pImgRotateClockwise90->GetWidth(), m_pImgRotateClockwise90->GetHeight(), UnitPixel);

	RectF rfRemove(lpDrawItemStruct->rcItem.right - m_pImgNO->GetWidth(), \
		lpDrawItemStruct->rcItem.top, \
		m_pImgNO->GetWidth(), \
		m_pImgNO->GetHeight());
	g.DrawImage(m_pImgNO, rfRemove, 0, 0, m_pImgNO->GetWidth(), m_pImgNO->GetHeight(), UnitPixel);

	//g.DrawRectangle(&Pen(Color(0, 0, 255)), \
	//	lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left-1, lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top-1);

	//rcDrawArea = GetPanelViewRect();

	//dcMem.FillSolidRect(rcDrawArea, RGB(255, 255, 255));

	CRect rcDiagram = lpDrawItemStruct->rcItem;
	rcDiagram.DeflateRect(1, 1, 1, 1);

	//DrawingPanelParam param;
	//param.m_nGap = 10;
	//DrawComponent(&dc, pComponent, rcDiagram, param);

	Panel panel;
	panel.m_OrgLen = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength;
	panel.m_OrgWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth;
	DrawingPanelParam param;
	param.m_nGap = GAP;
	DrawPanel(&dc, &panel, rcDiagram, param);

	for(int i = 0; i < precobmineItem.size(); i++)
	{
		PreCombineComponent& thePreCombineComponent = precobmineItem[i];
		Component* pTheComponent = ConvertOneInputInfoToOneComponent(thePreCombineComponent.m_Item);
		int nRotateTimes = thePreCombineComponent.m_nRotatedAngleWhenCombine / 90;
		for(int i = 0; i < abs(nRotateTimes); i++)
		{
			if(nRotateTimes < 0)
				ComponentRotateClockwise90(pTheComponent);
		}

		pTheComponent->m_x = thePreCombineComponent.m_fX_Left;
		pTheComponent->m_y = thePreCombineComponent.m_fY_Bottom;

		DrawingPanelParam param;
		param.m_nGap = GAP;
		//if(thePreCombineComponent.m_nState == PreCombineComponent::bs_Hovered || thePreCombineComponent.m_nState == PreCombineComponent::bs_Down)
		//	param.m_color = RGB(180, 0, 0);
		//if(i == m_nSelectedItem)
		//	param.m_color = RGB(255, 0, 0);
		DrawComponentInPanel(&dc, pTheComponent, rcDiagram, param, panel.m_OrgLen, panel.m_OrgWidth);

		delete pTheComponent;
	}

	g.DrawRectangle(&Pen(Color(0, 0, 255)), \
		lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left-1, lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top-1);

	dc.Detach();
}
////
////void CNewListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
////{
////	// TODO: Add your code to determine the size of specified item
////	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
////	LPCTSTR lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
////	ASSERT(lpszText != NULL);
////	CSize   sz;
////	CDC*    pDC = GetDC();
////
////	sz = pDC->GetTextExtent(lpszText);
////
////	ReleaseDC(pDC);
////
////	lpMeasureItemStruct->itemHeight = 2*sz.cy;
////}
////
////// 其中m_listBox为CNewListBox类型的对象
////#define IDC_LISTBOX 0x1101
////
////m_listBox.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
////	LBS_OWNERDRAWVARIABLE, CRect(0, 0, 380, 280), this, IDC_LISTBOX);
//
////void CListBoxClipBoard::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
////{
////	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
////	// TODO: 在此添加控件通知处理程序代码
////	//OnClick();
////	AfxMessageBox("a");
////
////	*pResult = 0;
////}
//
void CListBoxPreCombine::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OnClick();

	CListBox::OnLButtonDown(nFlags, point);
}

void CListBoxPreCombine::OnClick()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	for(int i = 0; i < GetCount(); i++)
	{
		CRect rcItemRemove = GetRemoveButtonRect(i);
		if(rcItemRemove.PtInRect(ptCursor))
		{		
			//if(MessageBox("确定删除此项？", NULL, MB_OKCANCEL) == IDOK)
			{
				DeleteString(i);
				::PostMessage(GetParent()->GetSafeHwnd(), WM_SHOW_OR_HIDE_PRECOMBINE, 0, 0);
			}
			break;
		}
	}
}
//
//CRect CListBoxClipBoard::GetRotateButtonRect(int nItem)
//{
//	CRect rcRet;
//
//	if(nItem >= 0 && nItem < GetCount())
//	{
//		CRect rcItem;
//		GetItemRect(nItem, rcItem);
//		rcRet.SetRect(rcItem.right - m_pImgRotateClockwise90->GetWidth(), \
//			rcItem.top, \
//			rcItem.right, \
//			rcItem.top + m_pImgRotateClockwise90->GetHeight());
//	}
//	return rcRet;
//}
//
CRect CListBoxPreCombine::GetRemoveButtonRect(int nItem)
{
	CRect rcRet;

	if(nItem >= 0 && nItem < GetCount())
	{
		CRect rcItem;
		GetItemRect(nItem, rcItem);
		rcRet.SetRect(rcItem.right - m_pImgNO->GetWidth(), \
			rcItem.top, \
			rcItem.right, \
			rcItem.top + m_pImgNO->GetHeight());
	}
	return rcRet;
}
//
//void CListBoxClipBoard::ItemRotate90(/*Component* pComponent*/int nItem)
//{
//	//Component* pComponent = (Component*)GetItemData(nItem);
//	Component* pComponent = GetComponent(nItem);
//	if(pComponent == NULL)
//		return;
//	ComponentRotateClockwise90(pComponent);
//	Invalidate();
//}
//
PreCombineItem CListBoxPreCombine::GetPreCombineItem(int nItem)
{
	PreCombineItem* pRet = NULL;
	if(nItem >= 0 && nItem < GetCount())
	{
		CString strItemText;
		GetText(nItem, strItemText);
		int nItemKeyID = atoi(strItemText.GetBuffer());
		strItemText.ReleaseBuffer();
		map<int, PreCombineItem>::iterator itr = m_mapIDAndItem.find(nItemKeyID);
		if(itr != m_mapIDAndItem.end())
			pRet = &(itr->second);
	}
	return *pRet;
}
//
int CListBoxPreCombine::DeleteString(UINT nIndex)
{
	if(nIndex >= 0 && nIndex < GetCount())
	{
		CString strItemText;
		GetText(nIndex, strItemText);
		int nItemKeyID = atoi(strItemText.GetBuffer());
		strItemText.ReleaseBuffer();
		map<int, PreCombineItem>::iterator itr = m_mapIDAndItem.find(nItemKeyID);
		if(itr != m_mapIDAndItem.end())
			m_mapIDAndItem.erase(itr);
	}
	return CListBox::DeleteString(nIndex);
}

void CListBoxPreCombine::EmptyPreCombine()
{
	while(GetCount() > 0)
	{
		DeleteString(0);
	}
	m_mapIDAndItem.clear();
}

void CListBoxPreCombine::OnPaint()
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

	dcMem.FillSolidRect(&rect, RGB(255, 255, 255));   
	//Firstly, use default painting to paint to memory DC.  
	DefWindowProc(WM_PAINT,(WPARAM)dcMem.m_hDC,(LPARAM)0); 

	dc.BitBlt(0,0,rect.Width(),  rect.Height(),&dcMem,0, 0,SRCCOPY); 

	dcMem.SelectObject(pOldMemDCBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
}
//
BOOL CListBoxPreCombine::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

vector<PreCombineItem> CListBoxPreCombine::GetItems()
{
	vector<PreCombineItem> vItem;
	for(map<int, PreCombineItem>::iterator itr = m_mapIDAndItem.begin(); itr != m_mapIDAndItem.end(); itr++)
	{
		vItem.push_back(itr->second);
	}
	return vItem;
}
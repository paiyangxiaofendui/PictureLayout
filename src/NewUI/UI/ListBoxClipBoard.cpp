// ListBoxClipBoard.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "ListBoxClipBoard.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../Drawing/Drawing.h"
#include "../Misc/Misc.h"
#include "../Resource.h"
#include "../Misc/ProgramMisc.h"

// CListBoxClipBoard

int CListBoxClipBoard::m_nCurID = 0;

IMPLEMENT_DYNAMIC(CListBoxClipBoard, CListBox)

CListBoxClipBoard::CListBoxClipBoard()
{
	m_pImgRotateClockwise90 = LoadPngImgFromRes(IDB_PNG_ROTATE_CLOCKWISE_90);
	m_pImgRotateAntiClockwise90 = LoadPngImgFromRes(IDB_PNG_ROTATE_ANTICLOCKWISE_90);
	m_pImgNO = LoadPngImgFromRes(IDB_PNG_FLAG_NO);
}

CListBoxClipBoard::~CListBoxClipBoard()
{
	if(m_pImgRotateClockwise90)
		delete m_pImgRotateClockwise90;
	if(m_pImgRotateAntiClockwise90)
		delete m_pImgRotateAntiClockwise90;
	if(m_pImgNO)
		delete m_pImgNO;
}


BEGIN_MESSAGE_MAP(CListBoxClipBoard, CListBox)
	//ON_NOTIFY_REFLECT(NM_CLICK, &CListBoxClipBoard::OnNMClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CListBoxClipBoard 消息处理程序
void CListBoxClipBoard::AddItem(Component* pComponent)
{
	CString strItemText;
	strItemText.Format("%d", m_nCurID);
	int nItem = InsertString(GetCount(), strItemText);
	//SetItemData(nItem, (DWORD_PTR)pComponent);
	m_mapIDAndComponent.insert(make_pair(m_nCurID, pComponent));
	m_nCurID++;
}

//例如CNewListBox继承自CListBox，重载虚MeasureItem和DrawItem这两个虚函数，代码如下：

void CListBoxClipBoard::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	USES_CONVERSION;

	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	//LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData;
	//Component* pComponent = (Component*)(lpDrawItemStruct->itemData);
	Component* pComponent = GetComponent(lpDrawItemStruct->itemID);
	if(pComponent == NULL)
		return;


	//ASSERT(lpszText != NULL);
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);

	//// Save these value to restore them when done drawing.
	//COLORREF crOldTextColor = dc.GetTextColor();
	//COLORREF crOldBkColor = dc.GetBkColor();

	//// If this item is selected, set the background color 
	//// and the text color to appropriate values. Also, erase
	//// rect by filling it with the background color.
	//if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
	//	(lpDrawItemStruct->itemState & ODS_SELECTED))
	//{
	//	dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	//	dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
	//	dc.FillSolidRect(&lpDrawItemStruct->rcItem, 
	//		::GetSysColor(COLOR_HIGHLIGHT));
	//}
	//else
	//{
	//	if(lpDrawItemStruct->itemID%2)
	//		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(128,128,128));
	//	else
	//		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255,128,255));
	//}

	//// If this item has the focus, draw a red frame around the
	//// item's rect.
	//if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
	//	(lpDrawItemStruct->itemState & ODS_FOCUS))
	//{
	//	CBrush br(RGB(0, 0, 128));
	//	dc.FrameRect(&lpDrawItemStruct->rcItem, &br);
	//}
	//lpDrawItemStruct->rcItem.left += 5;
	//// Draw the text.
	//dc.DrawText(
	//	lpszText,
	//	strlen(lpszText),
	//	&lpDrawItemStruct->rcItem,
	//	DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	//// Reset the background color and the text color back to their
	//// original values.
	//dc.SetTextColor(crOldTextColor);
	//dc.SetBkColor(crOldBkColor);

	//dc.Detach();

	Graphics g(dc.GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeHighQuality);

	StringFormat sf;
	sf.SetTrimming( StringTrimmingEllipsisCharacter);
	//sf.SetFormatFlags(StringFormatFlagsLineLimit);
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
		//if(lpDrawItemStruct->itemID%2)
		//	dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(128,128,128));
		//else
		//	dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255,128,255));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255,255,255));
		pBursh = &brushText;
	}
	
	CString strText;
	strText = pComponent->m_BarCode + _T("\n") + GetFloatString(pComponent->m_Thickness, 1) + _T("-") + pComponent->m_Material + _T("\n") \
		+ GetFloatString(pComponent->m_RealLength, 0) + _T("*") + GetFloatString(pComponent->m_RealWidth, 0);
	RectF rfText(lpDrawItemStruct->rcItem.left + 5, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top);
	g.DrawString(A2W(strText), -1, &font, rfText, &sf, pBursh);

	CRect rcDiagram = lpDrawItemStruct->rcItem;
	rcDiagram.DeflateRect(1, 50, 1, 1);

	DrawingPanelParam param;
	param.m_nGap = 10;
	DrawComponent(&dc, pComponent, rcDiagram, param);

	RectF rfRotateIcon(lpDrawItemStruct->rcItem.right - m_pImgRotateClockwise90->GetWidth(), \
		lpDrawItemStruct->rcItem.top, \
		m_pImgRotateClockwise90->GetWidth(), \
		m_pImgRotateClockwise90->GetHeight());
	g.DrawImage(/*pComponent->m_bRotated ? m_pImgRotateAntiClockwise90 : */m_pImgRotateClockwise90, rfRotateIcon, 0, 0, m_pImgRotateClockwise90->GetWidth(), m_pImgRotateClockwise90->GetHeight(), UnitPixel);

	RectF rfRemove(lpDrawItemStruct->rcItem.right - m_pImgNO->GetWidth(), \
		lpDrawItemStruct->rcItem.top + 31, \
		m_pImgNO->GetWidth(), \
		m_pImgNO->GetHeight());
	g.DrawImage(m_pImgNO, rfRemove, 0, 0, m_pImgNO->GetWidth(), m_pImgNO->GetHeight(), UnitPixel);

	g.DrawRectangle(&Pen(Color(0, 0, 255)), \
		lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left-1, lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top-1);

	dc.Detach();
}
//
//void CNewListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
//{
//	// TODO: Add your code to determine the size of specified item
//	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
//	LPCTSTR lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
//	ASSERT(lpszText != NULL);
//	CSize   sz;
//	CDC*    pDC = GetDC();
//
//	sz = pDC->GetTextExtent(lpszText);
//
//	ReleaseDC(pDC);
//
//	lpMeasureItemStruct->itemHeight = 2*sz.cy;
//}
//
//// 其中m_listBox为CNewListBox类型的对象
//#define IDC_LISTBOX 0x1101
//
//m_listBox.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
//	LBS_OWNERDRAWVARIABLE, CRect(0, 0, 380, 280), this, IDC_LISTBOX);

//void CListBoxClipBoard::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//OnClick();
//	AfxMessageBox("a");
//
//	*pResult = 0;
//}

void CListBoxClipBoard::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OnClick();

	CListBox::OnLButtonDown(nFlags, point);
}

void CListBoxClipBoard::OnClick()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	for(int i = 0; i < GetCount(); i++)
	{
		CRect rcItemRotate = GetRotateButtonRect(i);
		CRect rcItemRemove = GetRemoveButtonRect(i);
		if(rcItemRotate.PtInRect(ptCursor))
		{		
			ItemRotate90(i);
			break;
		}
		else if(rcItemRemove.PtInRect(ptCursor))
		{		
			if(MessageBox("确定删除此粘贴板件？", NULL, MB_OKCANCEL) == IDOK)
			{
				DeleteString(i);
				::PostMessage(GetParent()->GetSafeHwnd(), WM_SHOW_OR_HIDE_CLIPBOARD, 0, 0);
			}
			break;
		}
	}
}

CRect CListBoxClipBoard::GetRotateButtonRect(int nItem)
{
	CRect rcRet;

	if(nItem >= 0 && nItem < GetCount())
	{
		CRect rcItem;
		GetItemRect(nItem, rcItem);
		rcRet.SetRect(rcItem.right - m_pImgRotateClockwise90->GetWidth(), \
			rcItem.top, \
			rcItem.right, \
			rcItem.top + m_pImgRotateClockwise90->GetHeight());
	}
	return rcRet;
}

CRect CListBoxClipBoard::GetRemoveButtonRect(int nItem)
{
	CRect rcRet;

	if(nItem >= 0 && nItem < GetCount())
	{
		CRect rcItem;
		GetItemRect(nItem, rcItem);
		rcRet.SetRect(rcItem.right - m_pImgRotateClockwise90->GetWidth(), \
			rcItem.top + 31, \
			rcItem.right, \
			rcItem.top + 31 + m_pImgRotateClockwise90->GetHeight());
	}
	return rcRet;
}

void CListBoxClipBoard::ItemRotate90(/*Component* pComponent*/int nItem)
{
	//Component* pComponent = (Component*)GetItemData(nItem);
	Component* pComponent = GetComponent(nItem);
	if(pComponent == NULL)
		return;
	ComponentRotateClockwise90(pComponent);
	Invalidate();
}

Component* CListBoxClipBoard::GetComponent(int nItem)
{
	Component* pRet = NULL;
	if(nItem >= 0 && nItem < GetCount())
	{
		CString strItemText;
		GetText(nItem, strItemText);
		int nItemKeyID = atoi(strItemText.GetBuffer());
		strItemText.ReleaseBuffer();
		map<int, Component*>::iterator itr = m_mapIDAndComponent.find(nItemKeyID);
		if(itr != m_mapIDAndComponent.end())
			pRet = itr->second;
	}
	return pRet;
}

int CListBoxClipBoard::DeleteString(UINT nIndex)
{
	if(nIndex >= 0 && nIndex < GetCount())
	{
		CString strItemText;
		GetText(nIndex, strItemText);
		int nItemKeyID = atoi(strItemText.GetBuffer());
		strItemText.ReleaseBuffer();
		map<int, Component*>::iterator itr = m_mapIDAndComponent.find(nItemKeyID);
		if(itr != m_mapIDAndComponent.end())
			m_mapIDAndComponent.erase(itr);
	}
	return CListBox::DeleteString(nIndex);
}

void CListBoxClipBoard::EmptyClipBoard()
{
	while(GetCount() > 0)
	{
		DeleteString(0);
	}
	m_mapIDAndComponent.clear();
}

void CListBoxClipBoard::OnPaint()
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

BOOL CListBoxClipBoard::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
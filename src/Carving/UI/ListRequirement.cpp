// ListRequirement.cpp : 实现文件
//

#include "stdafx.h"
//#include "../OfficeUI_Demo.h"
#include "ListRequirement.h"
#include "../Work/WorkDef.h"
//#include "../Misc/Misc.h"

namespace NameSpace_ListRequirement
{
	std::wstring AnsiToUnicode(const char* szStr);
}

using namespace NameSpace_ListRequirement;

// CListRequirement

IMPLEMENT_DYNAMIC(CListRequirement, CSkinListCtrlBase)

CListRequirement::CListRequirement()
:m_LastToolTipCol(-1)
,m_LastToolTipRow(-1)
{

}

CListRequirement::~CListRequirement()
{
}


BEGIN_MESSAGE_MAP(CListRequirement, CSkinListCtrlBase)
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
	ON_WM_MOUSEMOVE()
	//ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CListRequirement::OnLvnDeleteitem)
END_MESSAGE_MAP()



// CListRequirement 消息处理程序
void CListRequirement::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 32;
}

void CListRequirement::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	USES_CONVERSION;
	//::TextOut(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, "a", 1);
	bool bWarning = (((RequirementItemData*)lpDrawItemStruct->itemData)->m_eListItemState == ListItemState::LIST_ITEM_STATE_WARNING) ? true : false;

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
	SolidBrush brushWarningBk(Color(255, 0, 0));
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

	if(bWarning)
		g.FillRectangle(&brushWarningBk, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top -1);

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

BOOL CListRequirement::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	CString tooltip = GetToolTipText(nRow, nCol);
	if (tooltip.IsEmpty())
		return FALSE;

	// Non-unicode applications can receive requests for tooltip-text in unicode
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
	{
		//mbstowcs(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
		wcsncpy(pTTTW->szText, AnsiToUnicode(tooltip.GetBuffer()).c_str(), sizeof(pTTTW->szText)/sizeof(WCHAR));
		tooltip.ReleaseBuffer();
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
#endif
	// If wanting to display a tooltip which is longer than 80 characters,
	// then one must allocate the needed text-buffer instead of using szText,
	// and point the TOOLTIPTEXT::lpszText to this text-buffer.
	// When doing this, then one is required to release this text-buffer again
	return TRUE;
}

void CListRequirement::CellHitTest(const CPoint& pt, int& nRow, int& nCol) const
{
	nRow = -1;
	nCol = -1;

	LVHITTESTINFO lvhti = {0};
	lvhti.pt = pt;
	nRow = ListView_SubItemHitTest(m_hWnd, &lvhti);	// SubItemHitTest is non-const
	nCol = lvhti.iSubItem;
	if (!(lvhti.flags & LVHT_ONITEMLABEL))
		nRow = -1;
}

CString CListRequirement::GetToolTipText(int nRow, int nCol)
{
	CString strRet;
	if (nRow!=-1/* && nCol!=-1*/)
	{
		RequirementItemData* pData = (RequirementItemData*)GetItemData(nRow);
		if(pData)
			strRet = pData->m_strToolTip;	// Cell-ToolTip
	}

	return strRet;
}

void CListRequirement::PreSubclassWindow()
{
	CSkinListCtrlBase::PreSubclassWindow();

	// Disable the CToolTipCtrl of CListCtrl so it won't disturb our own tooltip-ctrl
	GetToolTips()->Activate(FALSE);

	// Enable our own tooltip-ctrl and make it show tooltip even if not having focus
	 m_OwnToolTipCtrl.Create(this, TTS_ALWAYSTIP);
	m_OwnToolTipCtrl.Activate(TRUE);
}

void CListRequirement::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	// Find the subitem
	LVHITTESTINFO hitinfo = {0};
	hitinfo.flags = nFlags;
	hitinfo.pt = pt;
	SubItemHitTest(&hitinfo);

	if (m_LastToolTipCol!=hitinfo.iSubItem || m_LastToolTipRow!=hitinfo.iItem)
	{
		// Mouse moved over a new cell
		m_LastToolTipCol = hitinfo.iSubItem;
		m_LastToolTipRow = hitinfo.iItem;

		// Remove the old tooltip (if available)
		if (m_OwnToolTipCtrl.GetToolCount()>0)
		{
			m_OwnToolTipCtrl.DelTool(this);
			m_OwnToolTipCtrl.Activate(FALSE);
		}

		// Add the new tooltip (if available)
		if (m_LastToolTipRow!=-1 && m_LastToolTipRow!=-1)
		{
			// Not using CToolTipCtrl::AddTool() because it redirects the messages to CListCtrl parent
			TOOLINFO ti = {0};
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND;	// Indicate that uId is handle to a control
			ti.uId = (UINT_PTR)m_hWnd;	// Handle to the control
			ti.hwnd = m_hWnd;			// Handle to window to receive the tooltip-messages
			ti.hinst = AfxGetInstanceHandle();
			ti.lpszText = LPSTR_TEXTCALLBACK;
			m_OwnToolTipCtrl.SendMessage(TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
			m_OwnToolTipCtrl.Activate(TRUE);
		}
	}

	CSkinListCtrlBase::OnMouseMove(nFlags, point);
}

BOOL CListRequirement::PreTranslateMessage(MSG* pMsg)
{
	if (m_OwnToolTipCtrl.m_hWnd)
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	return CSkinListCtrlBase::PreTranslateMessage(pMsg);
}

//void CListRequirement::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

namespace NameSpace_ListRequirement
{
	wstring AnsiToUnicode(const char* szStr)
	{
		int nLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
		if (nLen == 0)
		{
			return NULL;
		}
		wchar_t* pResult = new wchar_t[nLen+1];
		::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
		wstring strReturn(pResult);
		delete[] pResult;
		return strReturn;
	}
}
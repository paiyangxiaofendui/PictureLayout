// SkinHeaderCtrl.cpp : 实现文件
//

#include "stdafx.h"
//#include "OfficeUI_Demo.h"
#include "SkinHeaderCtrl_ExcelSetting.h"
#include "../Work/WorkDef.h"
//#include "./UICommon/MEMDC.H"
//#include "BtnExcelSetting.h"
#include "./UICommon/PngButton_Stretchable.h"
#include "Menu_ExcelSetting.h"
#include "../Misc/Misc.h"
#include "../Resource.h"

#define HEADER_HEIGHT 50
#define COMBOBOX_HEIGHT 25
#define IGNORE_ITEM_ID 0

bool IsNecessaryField(UINT uFieldID);

// CSkinHeaderCtrl

IMPLEMENT_DYNAMIC(CSkinHeaderCtrl_ExcelSetting, CSkinHeaderCtrl)

CSkinHeaderCtrl_ExcelSetting::CSkinHeaderCtrl_ExcelSetting()
{
	m_eSortOrderType = 0;
	m_nSortintSubItemID = -1;
	m_pImgCheck = LoadPngImgFromRes(IDB_PNG_FLAG_YES);
}

CSkinHeaderCtrl_ExcelSetting::~CSkinHeaderCtrl_ExcelSetting()
{
	if(m_pImgCheck)
		delete m_pImgCheck;
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl_ExcelSetting, CSkinHeaderCtrl)
	//ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_MESSAGE(HDM_INSERTITEMW, OnItemInsert)
	ON_MESSAGE(HDM_DELETEITEM, OnItemDelete)
	//ON_MESSAGE(WM_WINDOWPOSCHANGING, OnWinPosChanging)
	//ON_MESSAGE(HDM, OnItemDelete)
END_MESSAGE_MAP()



// CSkinHeaderCtrl 消息处理程序
LRESULT CSkinHeaderCtrl_ExcelSetting::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 
	int nHeight = (int)(/*pwpos->cy * m_Height*/HEADER_HEIGHT); //改变高度,m_Height为倍数
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}

//BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return FALSE;//CHeaderCtrl::OnEraseBkgnd(pDC);
//}
//
//void CSkinHeaderCtrl::OnPaint()
//{
//	USES_CONVERSION;
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CHeaderCtrl::OnPaint() for painting messages
//	CRect rcClient;
//	GetClientRect(rcClient);
//	//rcClient.InflateRect(0, 0, 10, 0);
//	CMemoryDC memDC(&dc, rcClient);
//	CDC bitmapDC;
//	bitmapDC.CreateCompatibleDC(&dc); 
//
//	memDC.FillSolidRect(rcClient.left, rcClient.top, rcClient.Width()/*+10*/, rcClient.Height(), RGB(255,255,255));
//
//	CPen penEdge(PS_SOLID, 1, RGB(67, 149, 250));
//	CPen penEdge1(PS_SOLID, 1, RGB(238, 245, 255));
//	//CPen* pOldPen = memDC->SelectObject(&penEdge);
//	//memDC.MoveTo(0, rcClient.bottom-1);
//	//memDC->LineTo(rcClient.right, rcClient.bottom-1);
//	//memDC->SelectObject(pOldPen);
//	Graphics g(memDC.m_hDC);
//	g.SetSmoothingMode(SmoothingModeHighQuality);
//	//RectF rfBk(0, 0,  rcClient.Width()+2, rcClient.Height());
//	//g.Rec
//
//	//for(vector<CSortArrowButton*>::const_iterator itr = m_vSortButton.begin(); itr != m_vSortButton.end(); itr++)
//	//{
//	//	if((*itr)->Width() > 0)
//	//		(*itr)->Draw(g);
//	//}
//
//	int nItems = GetItemCount();
//	for(int i = 0; i <nItems; i++)
//	{
//	//	if (i == 0)
//	//		continue;
//
//	//	if(i == 1)
//	//	{
//	//		m_btnCheckBox.Draw(g);
//	//	}
//		TCHAR szText[MAX_PATH];
//		CRect rcItem;
//		HD_ITEM hdItem;
//
//		hdItem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
//		hdItem.pszText = szText;
//		hdItem.cchTextMax = MAX_PATH;
//		GetItem(i, &hdItem );	
//		GetItemRect(i, &rcItem);
//
//		LinearGradientBrush brush(Point(rcItem.left, rcItem.top), Point(rcItem.left, rcItem.bottom), Color(255, 255, 255, 255), Color(255, 140, 176, 230));
//		g.FillRectangle(&brush, rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
//
//
//	//	//CBitmap* pOldBitmap = NULL;
//
//	//	//if(i != 0)
//	//	//{
//			CPen* pOldPen = memDC.SelectObject(&penEdge);
//			memDC.MoveTo(rcItem.right, 0);
//			memDC.LineTo(rcItem.right, rcItem.bottom-1);
//			memDC.LineTo(rcItem.left, rcItem.bottom-1);
//			/*memDC.SelectObject(&penEdge1);
//			memDC.MoveTo(rcItem.right + 1, 1);
//			memDC.LineTo(rcItem.right + 1, rect.bottom);*/
//			memDC.SelectObject(pOldPen);
//	//	//}
//
//
//
//
//
//	//	CRect rcText(rcItem);
//	//	if (i == 1)
//	//	{
//	//		rcText.left = m_btnCheckBox.right - 3;
//	//	}
//	//	if (rcText.Width() <= 8 + 2)
//	//	{
//	//		m_vSortButton.at(i)->SetRect(rcItem.left, 0, rcItem.left, 0);
//	//		continue;
//	//	}
//	//	rcText.DeflateRect(8, 2, 2/*12 + m_pImgArrowUp->GetWidth()*/, 2);
//	//	RectF rfText(rcText.left, rcText.top, rcText.Width(), rcText.Height());
//	//	
//
//		RectF rfText(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
//		StringFormat sf;
//		sf.SetAlignment(StringAlignmentCenter);
//		sf.SetLineAlignment(StringAlignmentCenter);
//		sf.SetTrimming( StringTrimmingEllipsisCharacter);
//		sf.SetFormatFlags(StringFormatFlagsLineLimit);
//		Gdiplus::Font font(L"Segoe UI", 14, FontStyleRegular, UnitPixel);
//		SolidBrush brushText(Color(0, 0, 0));
//		
//		g.DrawString(A2W(szText), -1, &font, rfText, &sf, &brushText);
//
//	//	RectF rfTextTest;
//	//	g.MeasureString(szText, -1, &font, PointF(rfText.X, rfText.Y), &rfTextTest);
//	//	//g.DrawRectangle(&Pen(Color(Color::Red)), rfTextTest);
//
//	//	CRect rcSortButton(rcItem);
//	//	rcSortButton.left = rcText.left;
//	//	if (rcSortButton.Width() <= rfTextTest.Width + 6 + m_pImgArrowUp->GetWidth() + 12)
//	//	{
//	//		m_vSortButton.at(i)->SetRect(rcItem.left, 0, rcItem.left, 0);
//	//		continue;
//	//	}
//	//	else
//	//	{
//	//		int nSortButtonTop = rcSortButton.top + (rcSortButton.Height() - m_pImgArrowUp->GetHeight()) / 2;
//	//		int nSortButtonLeft = rcSortButton.left + rfTextTest.Width + 6;
//	//		m_vSortButton.at(i)->SetRect(nSortButtonLeft, nSortButtonTop, nSortButtonLeft + m_pImgArrowUp->GetWidth(), nSortButtonTop + m_pImgArrowUp->GetHeight());
//	//		m_vSortButton.at(i)->Draw(g);
//	//	}
//
//		if(i == m_nSortintSubItemID)
//		{
//			Image* pSortImg = (m_eSortOrderType == 0) ? m_pImgSortAscending : m_pImgSortDescending;
//			RectF rfSortIcon(rcItem.left+(rcItem.Width()-pSortImg->GetWidth())/2, \
//				rcItem.top, \
//				pSortImg->GetWidth(), \
//				pSortImg->GetHeight());
//			
//			g.DrawImage(pSortImg, rfSortIcon, 0, 0, pSortImg->GetWidth(), pSortImg->GetHeight(), UnitPixel);
//		}
//	}
//}
//
//void CSkinHeaderCtrl::SetSortingImg(Image* pImgSortAscending, Image* pImgSortDescending)
//{
//	m_pImgSortAscending = pImgSortAscending;
//	m_pImgSortDescending = pImgSortDescending;
//}
//
//void CSkinHeaderCtrl::SetSortingSubItem(int nSortintSubItemID, int eSortOrderType)
//{
//	m_nSortintSubItemID = nSortintSubItemID;
//	m_eSortOrderType = eSortOrderType;
//	Invalidate();
//}

CRect CSkinHeaderCtrl_ExcelSetting::GetTitleDrawingArea(int nItem)
{
	CRect rcRet;
	GetItemRect(nItem, &rcRet);
	rcRet.bottom -= COMBOBOX_HEIGHT;
	return rcRet;
}

//BOOL CSkinHeaderCtrl_ExcelSetting::DeleteItem(int nPos)
//{
//	return CSkinHeaderCtrl::DeleteItem(nPos);
//}
//
//int CSkinHeaderCtrl_ExcelSetting::InsertItem(int nPos, HDITEM* phdi)
//{
//	return CSkinHeaderCtrl::InsertItem(nPos, phdi);
//}

LRESULT CSkinHeaderCtrl_ExcelSetting::OnItemInsert( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CSkinHeaderCtrl::DefWindowProc(HDM_INSERTITEMW, wParam, lParam);

	static UINT uID = 1;
	CRect rcComboBox = GetComboBoxArea(wParam);
	//CComboBox* pComboBox = new CComboBox;
	//pComboBox->Create(CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP, rcComboBox, this, 1);
	//InitComboBoxMenu(pComboBox);
	//pComboBox->ShowWindow(SW_SHOW);
	CPngButton_Stretchable* pButton = new CPngButton_Stretchable;
	pButton->Create("", BS_PUSHBUTTON, rcComboBox, this, uID++);
	pButton->LoadImageFromRes(IDB_PNG_BTN_EXCEL_SETTING_N, IDB_PNG_BTN_EXCEL_SETTING_H, IDB_PNG_BTN_EXCEL_SETTING_H, IDB_PNG_BTN_EXCEL_SETTING_N, 0);
	StringFormat textFormat;
	textFormat.SetLineAlignment(StringAlignmentCenter);
	textFormat.SetAlignment(StringAlignmentCenter);
	textFormat.SetTrimming(StringTrimmingNone);
	pButton->SetStringFormat(textFormat);
	pButton->SetCustomFont(L"Segoe UI", 15);
	pButton->ShowWindow(SW_SHOW);
	HeaderItemData data;
	//data.m_pComboBox = pComboBox;
	data.m_pButton = pButton;
	data.m_nCurSel = -1;
	data.m_uBtnID = ((CButton*)pButton)->GetDlgCtrlID();
	list<HeaderItemData>::iterator itr = m_listHeaderItemData.begin();
	std::advance(itr, wParam);
	m_listHeaderItemData.insert(itr, 1, data);

	//Invalidate();
	
	return lResult; 
}

LRESULT CSkinHeaderCtrl_ExcelSetting::OnItemDelete( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CSkinHeaderCtrl::DefWindowProc(HDM_DELETEITEM, wParam, lParam); 

	list<HeaderItemData>::iterator itr = m_listHeaderItemData.begin();
	std::advance(itr, wParam);
	//CComboBox* pComobBox = itr->m_pComboBox;
	CPngButton_Stretchable* pButton = itr->m_pButton;
	m_listHeaderItemData.erase(itr);
	//pComobBox->DestroyWindow();
	//delete pComobBox;
	pButton->DestroyWindow();
	delete pButton;

	return lResult; 
}

CRect CSkinHeaderCtrl_ExcelSetting::GetComboBoxArea(int nItem)
{
	CRect rcRet;
	GetItemRect(nItem, &rcRet);
	rcRet.top += HEADER_HEIGHT - COMBOBOX_HEIGHT;
	return rcRet;
}

//void CSkinHeaderCtrl_ExcelSetting::InitComboBoxMenu(CComboBox* pComboBox)
//{
//	pComboBox->InsertString(0, "忽略");
//	for(int i = 0; i < sizeof(g_aRequirementListTitle) / sizeof(char*); i++)
//	{
//		pComboBox->InsertString(i+1, g_aRequirementListTitle[i]);
//	}
//}

void CSkinHeaderCtrl_ExcelSetting::SetInitMapping(map<int, int> mapExcelToRequirement)
{
	m_mapExcelToRequirement = mapExcelToRequirement;
	UpdateMapping(false);
}

void CSkinHeaderCtrl_ExcelSetting::UpdateMapping(bool bSave)
{
	if(!bSave)
	{
		for(int i = 0; i < GetItemCount(); i++)
		{
			if(i < m_listHeaderItemData.size())
			{
				int nCurSelID = IGNORE_ITEM_ID;

				list<HeaderItemData>::iterator itrHeaderItemData = m_listHeaderItemData.begin();
				std::advance(itrHeaderItemData, i);

				map<int, int>::iterator itrMapping = m_mapExcelToRequirement.find(i);
				if(itrMapping != m_mapExcelToRequirement.end())
				{
					//itrHeaderItemData->m_pComboBox->SetCurSel(itrMapping->second+1);
					nCurSelID = itrMapping->second+1;
				}

				SetOneItemMapping(*itrHeaderItemData, nCurSelID);
			}
		}
	}
	else
	{
		m_mapExcelToRequirement.clear();
		for(int i = 0; i < GetItemCount(); i++)
		{
			if(i < m_listHeaderItemData.size())
			{
				list<HeaderItemData>::iterator itrHeaderItemData = m_listHeaderItemData.begin();
				std::advance(itrHeaderItemData, i);
				//CComboBox* pTheComboBox = itrHeaderItemData->m_pComboBox;
				//int nSel = pTheComboBox->GetCurSel();
				int nSel = itrHeaderItemData->m_nCurSel;
				if(nSel != -1 && nSel != IGNORE_ITEM_ID)
				{
					m_mapExcelToRequirement[i] = nSel - 1;
				}
			}
		}
	}
}

map<int, int> CSkinHeaderCtrl_ExcelSetting::GetMapping()
{
	UpdateMapping(true);
	return m_mapExcelToRequirement;
}

void CSkinHeaderCtrl_ExcelSetting::SetOneItemMapping(HeaderItemData& theHeaderItemData, int nCurSelID)
{
	if(nCurSelID < 0)
		return;

	theHeaderItemData.m_nCurSel = nCurSelID;
	CString strText = GetMenuTextByID(nCurSelID);
	//if(nCurSelID == 0)
	//	strText = "(忽略)";
	//else if(nCurSelID-1 < sizeof(g_aRequirementListTitle) / sizeof(char*))
	//{
	//	strText = g_aRequirementListTitle[nCurSelID-1];
	//}

	theHeaderItemData.m_pButton->SetWindowText(strText);
}

//BOOL CSkinHeaderCtrl_ExcelSetting::PreTranslateMessage(MSG* pMsg)
//{
//	//PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)0);  
//	if (pMsg->message == WM_COMMAND)  
//	{
//		int i = 0;
//		UINT uLow = LOWORD(pMsg->wParam);
//		UINT uHight = HIWORD(pMsg->wParam);
//		return TRUE;
//	}
//	return CHeaderCtrl::PreTranslateMessage(pMsg);
//}

//void CSkinHeaderCtrl_ExcelSetting::OnBtn1()
//{
//	AfxMessageBox("a");
//}

BOOL CSkinHeaderCtrl_ExcelSetting::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED)  
	{
		if(lParam != 0)
		{
			UINT uBtnID = LOWORD(wParam);
			OnBtnClicked(uBtnID);
		}
		else
		{
			UINT uMenuID = wParam;
			OnMenu(uMenuID);
		}

		return TRUE;
	}
	return CHeaderCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

void CSkinHeaderCtrl_ExcelSetting::OnBtnClicked(UINT uBtnID)
{
	list<HeaderItemData>::iterator itr = m_listHeaderItemData.begin();
	for(; itr != m_listHeaderItemData.end(); itr++)
	{
		if(itr->m_uBtnID == uBtnID)
			break;
	}
	if(itr == m_listHeaderItemData.end())
		return;

	CMenu_ExcelSetting menu;
	map<UINT, CString> mapCommandIDToText;
	vector<int> vSpecColorItemID;
	menu.SetCheckImage(m_pImgCheck);
	menu.CreatePopupMenu();
	int nID = 0;
	menu.AppendMenu(MF_STRING | MF_OWNERDRAW, nID, GetMenuTextByID(nID));
	mapCommandIDToText.insert(make_pair(nID, GetMenuTextByID(nID)));
	nID++;
	menu.AppendMenu(MF_SEPARATOR, 100000, "");
	//int nCommandItemID = 1;
	for(int i = 0; i < sizeof(g_aRequirementListTitle) / sizeof(char*); i++)
	{
		if(i != itr->m_nCurSel-1)
		{
			CString strText = GetMenuTextByID(nID);
			menu.AppendMenu(MF_STRING | MF_OWNERDRAW, nID, /*g_aRequirementListTitle[i]*/strText);
			mapCommandIDToText.insert(make_pair(nID, strText));
			if(IsNecessaryField(i))
				vSpecColorItemID.push_back(/*nCommandItemID*/nID);
			for(list<HeaderItemData>::iterator itrCheck = m_listHeaderItemData.begin(); itrCheck != m_listHeaderItemData.end(); itrCheck++)
			{
				if(itrCheck->m_nCurSel == nID)
				{
					menu.CheckMenuItem(menu.GetMenuItemCount()-1, MF_BYPOSITION | MF_CHECKED);
					break;
				}
			}
			//nCommandItemID++;
		}
		nID++;
	}
	menu.SetItemsText(mapCommandIDToText);
	menu.SetSpecialColorItemsID(vSpecColorItemID);
	if(itr->m_nCurSel == IGNORE_ITEM_ID)
	{
		menu.EnableMenuItem(IGNORE_ITEM_ID, MF_DISABLED);
	}

	CPngButton_Stretchable* pTheBtn = itr->m_pButton;
	CRect rcBtn;
	pTheBtn->GetWindowRect(rcBtn);
	//TPMPARAMS param;
	//param.cbSize = sizeof(TPMPARAMS);
	//param.rcExclude.left = rcBtn.left;
	//param.rcExclude.top = rcBtn.top;
	//param.rcExclude.right = rcBtn.right;
	//param.rcExclude.bottom = rcBtn.bottom;
	menu.TrackPopupMenuEx(TPM_LEFTALIGN|TPM_RIGHTBUTTON, rcBtn.left, rcBtn.bottom, this, /*&param*/NULL); 
	m_uClickedBtnID = uBtnID;
	menu.DestroyMenu();
	//MenuItemData_ExcelSetting* pData = new MenuItemData_ExcelSetting;
	//delete pData;
}

void CSkinHeaderCtrl_ExcelSetting::OnMenu(UINT uMenuID)
{
	//if(uMenuID != IGNORE_ITEM_ID)
	//{
		for(list<HeaderItemData>::iterator itr = m_listHeaderItemData.begin(); itr != m_listHeaderItemData.end(); itr++)
		{
			if(uMenuID != IGNORE_ITEM_ID && itr->m_nCurSel == uMenuID)
			{
				SetOneItemMapping(*itr, IGNORE_ITEM_ID);
			}
			if(itr->m_uBtnID == m_uClickedBtnID)
			{
				SetOneItemMapping(*itr, uMenuID);
			}
		}
	//}

}

CString CSkinHeaderCtrl_ExcelSetting::GetMenuTextByID(int nID)
{
	CString strRet;
	if(nID >= 0 && nID <= sizeof(g_aRequirementListTitle) / sizeof(char*))
	{
		if(nID == 0)
			strRet = "(忽略)";
		else
			strRet = g_aRequirementListTitle[nID-1];
	}
	return strRet;
}

//LRESULT CSkinHeaderCtrl_ExcelSetting::OnWinPosChanging(WPARAM wParam, LPARAM lParam)
//{
//	if(GetItemCount() == m_listHeaderItemData.size() && GetItemCount() != 0)
//	{
//		int i = 0;
//		for(list<HeaderItemData>::iterator itr = m_listHeaderItemData.begin(); itr != m_listHeaderItemData.end(); itr++, i++)
//		{
//			CRect rcItem;
//			GetItemRect(i, &rcItem);
//			CRect rcBtn;
//			rcBtn.CopyRect(rcItem);
//			rcBtn.top = rcBtn.bottom - COMBOBOX_HEIGHT;
//			//rcBtn.left++;
//			//rcBtn.right--;
//			itr->m_pButton->MoveWindow(rcBtn);
//			Invalidate();
//		}
//	}
//
//	return DefWindowProc(WM_WINDOWPOSCHANGING, wParam, lParam);
//}

void CSkinHeaderCtrl_ExcelSetting::ArrangeBtns()
{
	if(GetItemCount() == m_listHeaderItemData.size() && GetItemCount() != 0)
	{
		int i = 0;
		for(list<HeaderItemData>::iterator itr = m_listHeaderItemData.begin(); itr != m_listHeaderItemData.end(); itr++, i++)
		{
			CRect rcItem;
			GetItemRect(i, &rcItem);
			CRect rcBtn;
			rcBtn.CopyRect(rcItem);
			rcBtn.top = rcBtn.bottom - COMBOBOX_HEIGHT;
			//rcBtn.left++;
			//rcBtn.right--;
			itr->m_pButton->MoveWindow(rcBtn);
			itr->m_pButton->Invalidate();
			//Invalidate();
		}
	}
}

bool IsNecessaryField(UINT uFieldID)
{
	bool bRet = false;

	static UINT a_nNecessaryItemID[] = {LIST_COLUMN_BARCODE, LIST_COLUMN_MATERIAL, LIST_COLUMN_CARVING_LEN, LIST_COLUMN_CARVING_WIDTH, LIST_COLUMN_CARVING_THICKNESS, LIST_COLUMN_COUNT, LIST_COLUMN_TEXTURE};
	for(int i = 0; i < sizeof(a_nNecessaryItemID)/sizeof(UINT); i++)
	{
		if(a_nNecessaryItemID[i] == uFieldID)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
};
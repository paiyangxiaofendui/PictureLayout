// Menu_ExcelSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Menu_ExcelSetting.h"


// CMenu_ExcelSetting

IMPLEMENT_DYNAMIC(CMenu_ExcelSetting, CMenu)

CMenu_ExcelSetting::CMenu_ExcelSetting()
:m_pImgCheck(NULL)
{

}

CMenu_ExcelSetting::~CMenu_ExcelSetting()
{
}


//BEGIN_MESSAGE_MAP(CMenu_ExcelSetting, CWnd)
//END_MESSAGE_MAP()



// CMenu_ExcelSetting 消息处理程序
void CMenu_ExcelSetting::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth = 100;
}

void CMenu_ExcelSetting::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 添加您的代码以绘制指定项
	USES_CONVERSION;

	CDC dcMem;
	CBitmap bmpMem;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItemInMenu(lpDrawItemStruct->rcItem);
	CRect rcItem;
	rcItem.SetRect(0, 0, rcItemInMenu.Width(), rcItemInMenu.Height());
	dcMem.CreateCompatibleDC(pDC);
	bmpMem.CreateCompatibleBitmap(pDC, rcItem.Width(), rcItem.Height());
	dcMem.SelectObject(&bmpMem);

	CString strText; 
	//CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC); //获取菜单项的设备句柄 
	//ItemInfo *info = (ItemInfo*)lpDrawItemStruct->itemData;
	
	Graphics g(dcMem.GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeHighQuality);

	//if(info->m_itemState==0)//分隔条
	//{
	//	pDC->FillSolidRect(rcItem,COLOR_BK);
	//	CRect r = rcItem;
	//	r.top =r.Height()/2+r.top ;
	//	r.bottom =r.top +NUM_SEPARATOR_HEIGHT;
	//	r.left += 5;
	//	r.right -= 5;

	//	pDC->Draw3dRect(r,COLOR_SEPARAROR,COLOR_SEPARAROR);//RGB(64,0,128));

	//	return;
	//}
	COLORREF colorrrefBg = GetBkColor(lpDrawItemStruct->hDC);
	Color colorItemBg, colorText;
	colorItemBg = Color(GetRValue(colorrrefBg), GetGValue(colorrrefBg), GetBValue(colorrrefBg));
	if((lpDrawItemStruct->itemState & ODS_SELECTED) && !(lpDrawItemStruct->itemState & ODS_DISABLED))
		colorItemBg = Color(152, 231, 253);

	colorText = Color::Black;
	if(lpDrawItemStruct->itemState & ODS_DISABLED)
		colorText = Color(100, 100, 100);
	else
	{
		for(int i = 0; i < m_vSpecialColorItemID.size(); i++)
		{
			if(m_vSpecialColorItemID[i] == lpDrawItemStruct->itemID)
			{
				colorText = Color::Red;
				break;
			}
		}
	}
	//char szTmp[100];
	//itoa(lpDrawItemStruct->itemState, szTmp, 2);
	//CString strTmp;
	//strTmp.Format(_T("%s\n"), szTmp);
	//OutputDebugString(strTmp);
	g.FillRectangle(&SolidBrush(colorItemBg), -1, -1, rcItem.Width()+1, rcItem.Height()+1);
	g.DrawLine(&Pen(Color::Blue), rcItem.left+20, rcItem.top, rcItem.left+20, rcItem.bottom);
	

	//strText = (LPCTSTR)(lpDrawItemStruct->itemData);
	//strText.Format(_T("%d"), lpDrawItemStruct->itemID);
	//strText = m_vItemText.at(lpDrawItemStruct->itemID);
	if(m_mapItemCommandIDToText.find(lpDrawItemStruct->itemID) != m_mapItemCommandIDToText.end())
		strText = m_mapItemCommandIDToText[lpDrawItemStruct->itemID];
	Gdiplus::Font font(L"Segoe UI", 15, FontStyleRegular, UnitPixel);
	StringFormat sf;
	SolidBrush brushText(colorText);
	sf.SetLineAlignment(StringAlignmentCenter);
	g.DrawString(A2W(strText), -1, &font, RectF(rcItem.left+20, rcItem.top, rcItem.Width()-20, rcItem.Height()), &sf, &brushText);

	if((lpDrawItemStruct->itemState & ODS_CHECKED) && m_pImgCheck)
	{
		RectF rfCheck;
		rfCheck.X = 3;
		rfCheck.Y = (rcItem.Height()-15)/2;
		rfCheck.Width = 15;
		rfCheck.Height = 15;
		g.DrawImage(m_pImgCheck, rfCheck, 0, 0, m_pImgCheck->GetWidth(), m_pImgCheck->GetHeight(), UnitPixel);
	}

	//if (lpDrawItemStruct->itemState & ODS_GRAYED)
	//{
	//	pDC->FillSolidRect(rcItem,COLOR_BK);
	//	pDC->SetTextColor(COLOR_DISABLE);
	//}    
	//else if (lpDrawItemStruct->itemState & ODS_SELECTED )
	//{ 
	//	//在菜单项上自绘矩形框的背景颜色 
	//	pDC->FillSolidRect(rcItem,COLOR_SEL);
	//	//设置菜单文字颜色
	//	pDC->SetTextColor(COLOR_TEXT);
	//} 
	//else
	//{
	//	pDC->FillSolidRect(rcItem,COLOR_BK);
	//	pDC->SetTextColor(COLOR_TEXT);
	//}    
	//pDC->SetBkMode(TRANSPARENT);

	//if(info->m_icon != NULL)
	//{
	//	DrawIconEx(pDC->m_hDC,rcItem.left+7,rcItem.top+16,info->m_icon,16,16,0,NULL,DI_NORMAL);
	//} 
	////文字字体和字号设置
	//LOGFONT fontInfo;
	//pDC->GetCurrentFont()->GetLogFont(&fontInfo);

	//fontInfo.lfHeight = 18;
	//lstrcpy(fontInfo.lfFaceName, _T("华文细黑"));
	//CFont fontCh;
	//fontCh.CreateFontIndirectA(&fontInfo);
	//pDC->SelectObject(&fontCh);

	//if(info->m_itemState == -1)//子菜单
	//{
	//	pDC->TextOutA(rcItem.left + 36, rcItem.top + 13, info->m_strText, info->m_strText.GetLength());
	//	//::ExcludeClipRect(pDC->m_hDC,rect.right-15,rect.top,rect.right,rect.bottom);
	//	//DrawIconEx(pDC->m_hDC,rect.right-40,rect.top+7,AfxGetApp()->LoadIconA(IDI_ICON1),32,32,1,NULL,DI_NORMAL);        
	//}
	//else
	//{
	//	pDC->TextOutA(rcItem.left + 36, rcItem.top + 13, info->m_strText, info->m_strText.GetLength());
	//	fontInfo.lfHeight = 16;
	//	CFont fontEn;
	//	lstrcpy(fontInfo.lfFaceName, _T("Arial"));
	//	fontEn.CreateFontIndirectA(&fontInfo);
	//	pDC->SelectObject(&fontEn);
	//	pDC->TextOutA(rcItem.left + 86, rcItem.top + 16, info->m_strShortcut, info->m_strShortcut.GetLength());
	//}     

	pDC->BitBlt(rcItemInMenu.left, rcItemInMenu.top, rcItemInMenu.Width(), rcItemInMenu.Height(), &dcMem, 0, 0, SRCCOPY);
}

void CMenu_ExcelSetting::SetCheckImage(Image* pImgCheck)
{
	m_pImgCheck = pImgCheck;
}

void CMenu_ExcelSetting::SetItemsText(map<UINT, CString> mapItemCommandIDToText)
{
	m_mapItemCommandIDToText = mapItemCommandIDToText;
}

void CMenu_ExcelSetting::SetSpecialColorItemsID(vector<int> vSpecialColorItemID)
{
	m_vSpecialColorItemID = vSpecialColorItemID;
}
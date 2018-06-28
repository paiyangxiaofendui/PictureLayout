// RibbonCategorySelf.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "RibbonCategorySelf.h"
#include "../../Misc/Misc.h"


// CRibbonCategorySelf
CRibbonCategorySelf::CRibbonCategorySelf(CMFCRibbonBar* pParentRibbonBar, LPCTSTR lpszName, UINT uiSmallImagesResID, UINT uiLargeImagesResID, CSize sizeSmallImage, CSize sizeLargeImage)
	: CMFCRibbonCategory(pParentRibbonBar, lpszName, uiSmallImagesResID, uiLargeImagesResID, sizeSmallImage, sizeLargeImage)
{
	int i;
	i = 0;
}


CRibbonCategorySelf::CRibbonCategorySelf()
{
	int i;
	i = 0;
}

CRibbonCategorySelf::~CRibbonCategorySelf()
{
	for(int i = 0; i < m_vCategoryIcon.size(); i++)
	{
		if(m_vCategoryIcon[i].m_pImgIcon)
			delete m_vCategoryIcon[i].m_pImgIcon;
	}
}


// CRibbonCategorySelf 成员函数
void CRibbonCategorySelf::OnDraw(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}


	CMFCRibbonCategory::OnDraw(pDC);

	//pDC->TextOut(0, 50, _T("helloa"));
	//pDC->Rectangle(GetRect());
	//CMFCRibbonBaseElement* pElm = GetLastVisibleElement();
	//if(pElm)
	//{
	//	pDC->Rectangle(pElm->GetRect());
	//}
	//CMFCRibbonPanel* pPanel = GetPanel(GetPanelCount()-1);
	//if(pPanel)
	//{
	//	pDC->Rectangle(pPanel->GetRect());
	//}

	Graphics g(pDC->m_hDC);
	CRect rcCategory = GetRect();
	CRect rcLastPanel = GetPanel(GetPanelCount()-1)->GetRect();
	for(int i = 0; i < m_vCategoryIcon.size(); i++)
	{
		CategoryIcon& item = m_vCategoryIcon[i];
		POINT ptCenter;
		ptCenter.x = rcCategory.right - m_vCategoryIcon[i].m_ptIconCenter.x;
		ptCenter.y = rcCategory.top + m_vCategoryIcon[i].m_ptIconCenter.y;
		RectF rfIcon(ptCenter.x - item.m_pImgIcon->GetWidth()/2, ptCenter.y - item.m_pImgIcon->GetHeight()/2, item.m_pImgIcon->GetWidth(), item.m_pImgIcon->GetHeight());
		if(rfIcon.X > rcLastPanel.right)
			g.DrawImage(item.m_pImgIcon, rfIcon, 0, 0, item.m_pImgIcon->GetWidth(), item.m_pImgIcon->GetHeight(), UnitPixel);
	}
}

void CRibbonCategorySelf::SetIsActive(bool bActive)
{
	m_bIsActive = bActive;
}

void CRibbonCategorySelf::AddIcon(POINT ptIconCenter, UINT uID)
{
	CategoryIcon item;
	item.m_ptIconCenter = ptIconCenter;
	item.m_pImgIcon = LoadPngImgFromRes(uID);
	m_vCategoryIcon.push_back(item);
}
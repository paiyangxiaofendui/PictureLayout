// RibbonBarSelf.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "RibbonBarSelf.h"
#include "RibbonCategorySelf.h"


// CRibbonBarSelf

IMPLEMENT_DYNAMIC(CRibbonBarSelf, CMFCRibbonBar)

CRibbonBarSelf::CRibbonBarSelf()
	:CMFCRibbonBar()
{
	int i;
	i = 0;
}

CRibbonBarSelf::~CRibbonBarSelf()
{
}


BEGIN_MESSAGE_MAP(CRibbonBarSelf, CMFCRibbonBar)
	//ON_WM_PAINT()
END_MESSAGE_MAP()



// CRibbonBarSelf 消息处理程序
//void CRibbonBarSelf::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//
//	CRect rect;
//	CDC dcMem; 
//	CBitmap bmpMem;
//
//	GetClientRect(&rect);   
//	dcMem.CreateCompatibleDC(&dc);  
//	bmpMem.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
//	CBitmap* pOldMemDCBmp = dcMem.SelectObject(&bmpMem);
//
//	//dcMem.FillSolidRect(&rect,::GetBkColor());   
//	//Firstly, use default painting to paint to memory DC.  
//	DefWindowProc(WM_PAINT,(WPARAM)dcMem.m_hDC,(LPARAM)0); 
//
//	//dcMem.TextOut(0, 0, "hello");
//	dc.BitBlt(0,0,rect.Width(),  rect.Height(),&dcMem,0, 0,SRCCOPY); 
//
//
//	dcMem.SelectObject(pOldMemDCBmp);
//	dcMem.DeleteDC();
//	bmpMem.DeleteObject();
//
//}

CRibbonCategorySelf* CRibbonBarSelf::AddCategorySelf(LPCTSTR lpszName, UINT uiSmallImagesResID, UINT uiLargeImagesResID,
	CSize sizeSmallImage, CSize sizeLargeImage, int nInsertAt)
{
	ASSERT_VALID(this);
	ENSURE(lpszName != NULL);

	CRibbonCategorySelf* pCategory = NULL;


	pCategory = new CRibbonCategorySelf(this, lpszName, uiSmallImagesResID, uiLargeImagesResID, sizeSmallImage, sizeLargeImage);


	if (nInsertAt < 0)
	{
		m_arCategories.Add(pCategory);
	}
	else
	{
		m_arCategories.InsertAt(nInsertAt, pCategory);
	}

	if (m_pActiveCategory == NULL)
	{
		m_pActiveCategory = pCategory;
		((CRibbonCategorySelf*)m_pActiveCategory)->SetIsActive(TRUE);
	}

	m_bRecalcCategoryHeight = TRUE;
	m_bRecalcCategoryWidth = TRUE;

	return pCategory;
}
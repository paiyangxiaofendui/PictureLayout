// ListCtrlMaterial.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "ListCtrlMaterial.h"


// CListCtrlMaterial

IMPLEMENT_DYNAMIC(CListCtrlMaterial, CSkinListCtrlBase)

CListCtrlMaterial::CListCtrlMaterial()
{

}

CListCtrlMaterial::~CListCtrlMaterial()
{
}


BEGIN_MESSAGE_MAP(CListCtrlMaterial, CSkinListCtrlBase)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()



// CListCtrlMaterial 消息处理程序
void CListCtrlMaterial::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 32;
}


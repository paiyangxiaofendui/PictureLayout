#pragma once
#include "./UICommon/SkinListCtrlBase.h"
// CListCtrlMaterial

class CListCtrlMaterial : public CSkinListCtrlBase
{
	DECLARE_DYNAMIC(CListCtrlMaterial)

public:
	CListCtrlMaterial();
	virtual ~CListCtrlMaterial();

protected:
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
};



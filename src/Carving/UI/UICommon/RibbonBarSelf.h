#pragma once

class CRibbonCategorySelf;
// CRibbonBarSelf

class CRibbonBarSelf : public CMFCRibbonBar
{
	friend class CRibbonCategorySelf;
	friend class CMFCRibbonCategory;

	DECLARE_DYNAMIC(CRibbonBarSelf)

public:
	CRibbonBarSelf();
	virtual ~CRibbonBarSelf();

	CRibbonCategorySelf* AddCategorySelf(LPCTSTR lpszName, UINT uiSmallImagesResID, UINT uiLargeImagesResID,
		CSize sizeSmallImage = CSize(16, 16), CSize sizeLargeImage = CSize(32, 32), int nInsertAt = -1);
protected:
	DECLARE_MESSAGE_MAP()

	//afx_msg void OnPaint();

};



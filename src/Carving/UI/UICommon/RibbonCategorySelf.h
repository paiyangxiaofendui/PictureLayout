#pragma once
#include <vector>

using namespace std;

struct CategoryIcon
{
	Image* m_pImgIcon;
	POINT m_ptIconCenter;
};

class CMFCRibbonBar;
// CRibbonCategorySelf ÃüÁîÄ¿±ê

class CRibbonCategorySelf : public CMFCRibbonCategory
{
public:
	CRibbonCategorySelf();
	CRibbonCategorySelf(CMFCRibbonBar* pParentRibbonBar, LPCTSTR lpszName, UINT uiSmallImagesResID,
		UINT uiLargeImagesResID, CSize sizeSmallImage = CSize(16, 16), CSize sizeLargeImage = CSize(32, 32));
	virtual ~CRibbonCategorySelf();

	virtual void OnDraw(CDC* pDC);

	void SetIsActive(bool bActive);

	void AddIcon(POINT ptIconCenter, UINT uID);

protected:
	vector<CategoryIcon> m_vCategoryIcon;
};



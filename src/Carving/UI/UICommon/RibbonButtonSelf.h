#pragma once
#include "afxribbonbutton.h"
// CRibbonButtonSelf ÃüÁîÄ¿±ê

class CRibbonButtonSelf : public CMFCRibbonButton
{
	friend class CMFCRibbonPanel;
	friend class CMFCRibbonPanelMenuBar;
	friend class CMFCToolTipCtrl;
	friend class CMFCRibbonCollector;
	friend class CMFCRibbonConstructor;

	friend class CMFCToolBarMenuButton;
	friend class CMFCToolBarButton;
	//friend class CMFCToolBarMenuButton;
public:
	CRibbonButtonSelf();
	CRibbonButtonSelf(UINT nID, LPCTSTR lpszText, int nSmallImageIndex = -1, int nLargeImageIndex = -1, UINT uImgResID = 0, BOOL bAlwaysShowDescription = FALSE, bool bStretch = true, bool bDrawText = true);
	virtual ~CRibbonButtonSelf();

	virtual void OnDraw(CDC* pDC);

protected:
	Image* m_pImgIcon;
	bool m_bStretch;
	bool m_bDrawText;
};



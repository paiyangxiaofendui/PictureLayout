#pragma once
#include "./UICommon/SkinListCtrlBase.h"
// CListTotalResult

class CListTotalResult : public CSkinListCtrlBase
{
	DECLARE_DYNAMIC(CListTotalResult)

public:
	CListTotalResult();
	virtual ~CListTotalResult();

protected:
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnPaint();
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

	void DrawItems(CDC& dc);
	CRect GetCheckRect(int nItem);
	void OnClick();

	Image* m_pImgUncheck;
	Image* m_pImgCheck;
};



#pragma once


// CSkinListCtrlBase

class CSkinListCtrlBase : public CListCtrl
{
	DECLARE_DYNAMIC(CSkinListCtrlBase)

public:
	CSkinListCtrlBase();
	virtual ~CSkinListCtrlBase();

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
};



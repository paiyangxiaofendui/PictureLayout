#pragma once
#include "./UICommon/SkinListCtrlBase.h"

// CListRequirement
enum ListItemState{LIST_ITEM_STATE_NORMAL, LIST_ITEM_STATE_WARNING};

struct RequirementItemData
{
	RequirementItemData()
		:m_eListItemState(LIST_ITEM_STATE_NORMAL)
	{

	}

	UINT m_uSerialID;
	ListItemState m_eListItemState;
	CString m_strToolTip;
	CString m_strSortingText;
};

class CListRequirement : public CSkinListCtrlBase
{
	DECLARE_DYNAMIC(CListRequirement)

public:
	CListRequirement();
	virtual ~CListRequirement();

protected:
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()



	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	bool ShowToolTip(const CPoint& pt) const;
	CString GetToolTipText(int nRow, int nCol);

	CToolTipCtrl m_OwnToolTipCtrl;
	int m_LastToolTipCol;
	int m_LastToolTipRow;
};



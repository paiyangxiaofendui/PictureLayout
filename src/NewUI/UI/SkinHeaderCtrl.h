#pragma once


// CSkinHeaderCtrl

class CSkinHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CSkinHeaderCtrl)

public:
	enum State{STATE_NORMAL, STATE_WARNING};

	CSkinHeaderCtrl();
	virtual ~CSkinHeaderCtrl();

	void SetSortingImg(Image* pImgSortAscending, Image* pImgSortDescending);
	void SetSortingSubItem(int nSortintSubItemID, int eSortOrderType);
	void SetState(State eState);
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual CRect GetTitleDrawingArea(int nItem);

	Image* m_pImgSortAscending;
	Image* m_pImgSortDescending;
	int m_eSortOrderType;
	int m_nSortintSubItemID;
	State m_eState;
};



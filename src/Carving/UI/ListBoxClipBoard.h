#pragma once
#include <map>

using namespace std;

#define WM_SHOW_OR_HIDE_CLIPBOARD	WM_USER + 10089

class Component;
// CListBoxClipBoard

class CListBoxClipBoard : public CListBox
{
	DECLARE_DYNAMIC(CListBoxClipBoard)

public:
	CListBoxClipBoard();
	virtual ~CListBoxClipBoard();

	void AddItem(Component* pComponent);
	Component* GetComponent(int nItem);
	int DeleteString(UINT nIndex);
	void EmptyClipBoard();
	void ItemRotate90(/*Component* pComponent*/int nItem);
protected:
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void OnClick();
	CRect GetRotateButtonRect(int nItem);
	CRect GetRemoveButtonRect(int nItem);
	
	//vector<Component*> m_vClipboardItem;
	Image* m_pImgRotateClockwise90;
	Image* m_pImgRotateAntiClockwise90;
	Image* m_pImgNO;

	static int m_nCurID;
	map<int, Component*> m_mapIDAndComponent;
};



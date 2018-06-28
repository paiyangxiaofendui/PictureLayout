#pragma once
#include <map>
#include "../UIData/UIData.h"

#define WM_SHOW_OR_HIDE_PRECOMBINE	WM_USER + 10090

class Component;
// CListBoxClipBoard

class CListBoxPreCombine : public CListBox
{
	DECLARE_DYNAMIC(CListBoxPreCombine)

public:
	CListBoxPreCombine();
	virtual ~CListBoxPreCombine();

	void AddItem(vector<PreCombineComponent>& theItem);
	vector<PreCombineItem> GetItems();
	PreCombineItem GetPreCombineItem(int nItem);
	int DeleteString(UINT nIndex);
	void EmptyPreCombine();
	//void ItemRotate90(/*Component* pComponent*/int nItem);
protected:
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void OnClick();
	//CRect GetRotateButtonRect(int nItem);
	CRect GetRemoveButtonRect(int nItem);
	//
	////vector<Component*> m_vClipboardItem;
	//Image* m_pImgRotateClockwise90;
	//Image* m_pImgRotateAntiClockwise90;
	//Image* m_pImgNO;

	static int m_nCurID;
	map<int, PreCombineItem> m_mapIDAndItem;

	Image* m_pImgNO;
};



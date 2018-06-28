#pragma once
#include <vector>
using namespace std;

struct TAB_ITEM
{
	TAB_ITEM(LPCTSTR szName, UINT uID):m_strName(szName), m_uID(uID), m_pRgnTab(NULL){}
	CString m_strName;
	UINT    m_uID;
	CRgn*    m_pRgnTab;
};

// CTabBar
class CTabBar : public CStatic
{
	DECLARE_DYNAMIC(CTabBar)

public:
	enum TAB_POS{TAB_LEFT = 0, TAB_CENTER, TAB_RIGHT};

	CTabBar();
	virtual ~CTabBar();

	void AddTab(LPCTSTR szName, UINT uID);
	//void SetImage(LPCTSTR szLeft, LPCTSTR szLeftDown, LPCTSTR szCenter, LPCTSTR szCenterDown, LPCTSTR szRight, LPCTSTR szRightDown);
	void SetUIInfo(/*int nTabLen, */int nOverlayLen, UINT nUnselectedN, UINT nUnselectedH, UINT nSelectedN, UINT nSelectedH);
	bool SetTab(int nSelTab);
	bool SwitchTab(int nSelTab);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
protected:
	void DrawBackground(CDC*pDC);
	//void DrawTab(Graphics& g, TAB_POS eTabPos, int nTabNum);
	void DrawTab(CDC& memDC, TAB_ITEM& item, CRect& rcTabDrawing, bool bSelected = false);
	void RefreshTabRgn();
protected:
	CDC m_dcBk;
	CBitmap m_bmpBk;
	Gdiplus::Font m_textFontNormal;
	SolidBrush  m_brushWhite;
	SolidBrush  m_brushShadow;

	int m_nCurSelTab;
	vector<TAB_ITEM> m_vTabItem;

	//Image* m_pImgLeft;
	//Image* m_pImgCenter;
	//Image* m_pImgRight;
	//Image* m_pImgLeftDown;
	//Image* m_pImgCenterDown;
	//Image* m_pImgRightDown;

	CBitmap m_bmpTabUnselectedN;
	CBitmap m_bmpTabUnselectedH;
	CBitmap m_bmpTabSelectedN;
	CBitmap m_bmpTabSelectedH;
	int m_nTabLen;
	int m_nTabHeight;
	int m_nOverlayLen;

	bool m_bTrack;
};



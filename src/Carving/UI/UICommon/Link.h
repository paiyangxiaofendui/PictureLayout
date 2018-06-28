#pragma once


// CLink

class CLink : public CStatic
{
	DECLARE_DYNAMIC(CLink)

public:
	CLink(LPCTSTR szLink = NULL);
	virtual ~CLink();

	void SetLink(LPCTSTR szLink){m_strLink = szLink;}
protected:
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void DrawBackground(CDC*pDC);

	bool m_bTrack;
	CDC m_dcBk;
	CBitmap m_bmpBk;

	CString m_strLink;
};



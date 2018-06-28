#pragma once

#include <map>
#include <vector>
#include "../Resource.h"
using namespace std;
// CInOutDlg dialog

class CInOutDlg : public CDialogEx//设置->导入设置->excel对应
{
	DECLARE_DYNAMIC(CInOutDlg)

public:
	CInOutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInOutDlg();

// Dialog Data
	enum { IDD = IDD_INOUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
private:
	map<int,CString> m_srclist;
	map<int,int> m_inoutmap;
	vector<CString> strValue;
	int m_scrollindex;
	int m_clickindex;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
//	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	bool LoadBmpPic(const CString strBmpPath, CBitmap&bmp);
	CComboBox m_comselect;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelchangeCombo1();
};

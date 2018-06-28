#pragma once

#include <vector>
#include "../Resource.h"
// CPrintUserSetDlg dialog

struct PrintNode
{
	std::string _fontname;
	std::string _fontsize;
	std::string _leftstr;
	std::string _name;
	std::string _value;
	std::string _rightstr;
	int _x;
	int _y;
};

class CPrintUserSetDlg : public CDialogEx//打印模版设置对话框
{
	DECLARE_DYNAMIC(CPrintUserSetDlg)

public:
	CPrintUserSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrintUserSetDlg();

// Dialog Data
	enum { IDD = IDD_SETPRINTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	CStatic m_test;
	bool m_move;
	bool m_shift;
	CPoint m_oldpoint;
	std::vector<PrintNode> m_clickNode;
	std::vector<PrintNode> m_allnode;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	int m_srcx;
	int m_srcy;
	int m_endx;
	int m_endy;
	void Prinode();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CString m_fontsize;
	CString m_fonttype;
	CString m_leftstr;
	CString m_rightstr;
	CString m_value;
	CString m_x;
	CString m_y;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton9();
	CComboBox m_comname;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton8();
};

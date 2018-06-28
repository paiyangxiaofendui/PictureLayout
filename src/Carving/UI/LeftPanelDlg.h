#pragma once
#include "../Resource.h"

// CLeftPanelDlg dialog

class CLeftPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftPanelDlg)

public:
	CLeftPanelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLeftPanelDlg();

// Dialog Data
	enum { IDD = IDD_LEFTPANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CButton m_checkshow;
//	CEdit m_edith;
//	CEdit m_edits;
	int m_edith;
	int m_edits;
	int m_editw;
	virtual BOOL OnInitDialog();
};

#pragma once
#include "../Resource.h"

// CDirSetDlg dialog

class CDirSetDlg : public CDialogEx//纹理设置对话框
{
	DECLARE_DYNAMIC(CDirSetDlg)

public:
	CDirSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEXTURE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_combo;
	CString m_value;
	virtual BOOL OnInitDialog();
};

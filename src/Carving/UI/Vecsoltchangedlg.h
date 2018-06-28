#pragma once


// CVecsoltchangedlg dialog

class CVecsoltchangedlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVecsoltchangedlg)

public:
	CVecsoltchangedlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVecsoltchangedlg();

// Dialog Data
	enum { IDD = IDD_VECSOLTCHANGEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_result;
	CString m_size;
//	CEdit m_x;
	CString m_x;
	CString m_y;
};

#pragma once


// CVecOffset dialog

class CVecOffset : public CDialogEx
{
	DECLARE_DYNAMIC(CVecOffset)

public:
	CVecOffset(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVecOffset();

// Dialog Data
	enum { IDD = IDD_DIALOG_HOLE_OFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_result;
	float m_d;
	float m_f;
};

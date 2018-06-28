#pragma once
#include "../Resource.h"

// COPSetDlg dialog

class COPSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COPSetDlg)

public:
	COPSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COPSetDlg();

// Dialog Data
	enum { IDD = IDD_OPPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void loadSettings();
	void saveSettings();
	void AutoSetOpSettings(int CpnNum);


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//CComboBox m_comknife;
	//CComboBox m_comtype;
	//CString m_editrate;


	
	//long m_op_timeout;	// ”≈ªØ≥¨ ±

	int		m_nEditStep1Count;
	int		m_nComboStep1Alg;
	int		m_nEditStep2Count;
	int		m_nEditStep3Count;
	float	m_fEditStep3AcceptableUti;
	BOOL m_valDownerInfoFirst;
};

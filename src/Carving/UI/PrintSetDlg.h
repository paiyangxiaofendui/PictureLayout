#pragma once
#include "../Misc/Barcode39.h"
#include "../Resource.h"

// CPrintSetDlg dialog

class CPrintSetDlg : public CDialogEx//标签设置响应对话框
{
	DECLARE_DYNAMIC(CPrintSetDlg)

public:
	CPrintSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrintSetDlg();

// Dialog Data
	enum { IDD = IDD_PRINTSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	CButton m_moduleshow;
	CStatic m_moduleshow;
	virtual BOOL OnInitDialog();
	CComboBox m_modulesel;
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	void OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight);
	int m_srcx;
	int m_srcy;
	int m_paperWidth;
	int	m_paperHeight;
	afx_msg void OnBnClickedCheck1();
	CButton m_showsole;
//	CComboBox m_comtype;
	afx_msg void OnCbnSelchangeCombo2();
//	afx_msg void OnCbnSelendcancelCombo2();
//	CString m_typecom;
	CString m_typecom;
	int MyBarcode(CDC* cdc,int x1,int y1,int y2,int codetype,int ipen,const char * cCode);
	afx_msg void OnBnClickedButton2();
};

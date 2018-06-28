#pragma once
#include "ImageListCtrl.h"
#include "../Resource.h"

// CPanelPrintDlg dialog

class CPanelPrintDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPanelPrintDlg)

public:
	CPanelPrintDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPanelPrintDlg();

// Dialog Data
	enum { IDD = IDD_DAYINGTUYANG_DLG };

	void PrintView(int bwidth,int bheight);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	void OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:

//	CStatic m_panelprint;

	//map<int,Panel_Knife> m_printcoor;

	CImageList image_list;

	CImageListCtrl m_plist;
	int m_panelNum;

	CButton m_btnCancel;
	CButton m_btnOK;
};

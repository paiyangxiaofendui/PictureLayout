#pragma once
#include "../resource.h"
#include "DlgResult.h"

// CDlgWeiXin 对话框

  
class  CDlgNewUI : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewUI)

public:
	CDlgNewUI(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewUI();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEW_UI };



public:

	CDlgResult* m_pDlgResult;

	CRect GetViewRect();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

};

#pragma once
#include "../Resource.h"

// CDlgWeiXin 对话框

class CDlgWeiXin : public CDialog
{
	DECLARE_DYNAMIC(CDlgWeiXin)

public:
	CDlgWeiXin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWeiXin();

// 对话框数据
	enum { IDD = IDD_DLG_WEIXIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	Image* m_pImg;
};

#pragma once
#include "afxwin.h"
#include "../Resource.h"
#include "ParamSettingDlg.h"
// UISettingDlg 对话框

class UISettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UISettingDlg)

public:
	UISettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UISettingDlg();

// 对话框数据
	enum { IDD = IDD_UI_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	/************************************************************************/
	/* ui 事件                                                              */
	/************************************************************************/

	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedSaveDefault();
	virtual void OnOK();
	virtual void OnCancel();

private:
	CScrollBar m_settingScroll;				// 子窗口滚动条
	CRect m_settingDlgContainerRect;		// 子窗口包围区域
	CRect m_settingDlgRect;					// 子窗口对话框区域

public:
	/************************************************************************/
	/* 逻辑事件                                                             */
	/************************************************************************/
	void loadSettings();
	void saveSettings();
public:
	ParamSettingDlg param;	
	
};

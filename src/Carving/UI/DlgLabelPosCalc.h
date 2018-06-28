#pragma once
#include "../Resource.h"

// CDlgLabelPosCalc 对话框

class CDlgLabelPosCalc : public CDialog
{
	DECLARE_DYNAMIC(CDlgLabelPosCalc)

public:
	CDlgLabelPosCalc(LPCTSTR szMachineName, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLabelPosCalc();

// 对话框数据
	enum { IDD = IDD_DLG_LABEL_POS_CALC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();					//窗口初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam);	// 响应消息
	DECLARE_MESSAGE_MAP()

	void StartTieBiaoCalcThread();
	static DWORD WINAPI TieBiaoPosCalcThread( PVOID lpThreadParameter );				// 优化线程函数
	
	CProgressCtrl m_ProgressBar;
	HANDLE m_hdthreadTieBiaoCalc;					// 贴标线程句柄
	CString m_strMachineName;
};

#pragma once
#include "../Resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "./UICommon/SkinListCtrlBase.h"
#include "SkinHeaderCtrl.h"
#include "../UIData/UIData.h"

// CDirSetDlg dialog


class OptimizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OptimizeDlg)

public:
	OptimizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptimizeDlg();

	// Dialog Data
	enum { IDD = IDD_OPTIMIZE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	// 优化相关函数
	static int m_LastPanelNum;
	static vector<ComponentInputItem> m_vComponentInputItem;		// 要优化的板件
	static vector<PreCombineItem> m_vPreCombineItem;
	static vector<RemainderInputItem> m_vRemainderInputItem;		// 余料




	static BOOL m_ThreadIsRunning;		// 优化线程运行标志
	HANDLE m_hdthread;					// 优化线程句柄

	void StartOptimizeThread();
	static DWORD WINAPI OptimizeThread( PVOID lpThreadParameter );				// 优化线程函数
	afx_msg LRESULT OnWM_CALCULATE(WPARAM wParam, LPARAM lParam);				// 响应消息
	afx_msg LRESULT OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam);	// 响应消息
	afx_msg LRESULT OnWM_WM_UPDATE_REMAIN_TIME(WPARAM wParam, LPARAM lParam);	// 响应消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedUseNow();
	virtual BOOL OnInitDialog();					//窗口初始化




	CStatic m_TxtOpMessage;
	CProgressCtrl m_ProgressBar;







	CStatic m_TxtOpMessage2;
	CSkinListCtrlBase m_ListCtrlSolutionInfo;
	CSkinHeaderCtrl m_hcSolutionInfo;
	CButton m_BtnUseNow;
	CStatic m_TxtRemainTime;
};

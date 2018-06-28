// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "Resource.h"
#include "UI/UICommon/RibbonButtonSelf.h"
#include "UI/UICommon/RibbonBarSelf.h"

class BaseInfo;
class CParamDlg;

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 特性
public:

	CParamDlg* m_pDlgBaseParam;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//void GetAlgBaseInfo(BaseInfo& info);

protected:  // 控件条嵌入成员
	/*CMFCRibbonBar*/CRibbonBarSelf     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	//CCalendarBar      m_wndCalendar;
	//CMFCCaptionBar    m_wndCaptionBar;
	CRibbonButtonSelf/*CMFCRibbonButton*/* m_pBtnAboutUs;
	CRibbonButtonSelf* m_pBtnOurCustomers;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	DECLARE_MESSAGE_MAP()

	//BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	//BOOL CreateCaptionBar();

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

protected:
	void InitializeRibbon();

	CMFCRibbonEdit* m_pEditKerf;
	CMFCRibbonEdit* m_pEditPanelLength;
	CMFCRibbonEdit* m_pEditPanelWidth;
	CMFCRibbonEdit* m_pEditStep1Count;
	CMFCRibbonComboBox* m_pComboStep1Alg;
	CMFCRibbonComboBox* m_pComboLayoutOrg;
	CMFCRibbonEdit* m_pEditStep2Count;
	CMFCRibbonEdit* m_pEditStep3Count;
	CMFCRibbonEdit* m_pEditStep3AcceptableUti;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



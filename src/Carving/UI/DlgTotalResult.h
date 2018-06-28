#pragma once
#include "afxcmn.h"


// CDlgTotalResult 对话框
#include "ListTotalResult.h"
#include "ListCtrlMaterial.h"
#include "SkinHeaderCtrl.h"
#include <vector>
#include "./UICommon/DialogChildBase.h"

using namespace std;

class PanelViewingParam;
class Panel;

class CDlgTotalResult : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgTotalResult)

public:
	CDlgTotalResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTotalResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOTAL_RESULT };

	void RefreshOptimizeResult();
	PanelViewingParam* GetSelectedItemViewingParam();
	vector<Panel*> GetPanels(bool bChecked);
	bool IsPanelChecked(Panel* pPanel);
	void UpdateList();
	int GetCurSelPanelItemID();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnLvnItemchangedListTotalSolutionPanel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListTotalSolutionPanel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIdDeletePanel();
	afx_msg void OnIdAddPanel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	void RefreshTotalSolutionPanel();

	CListTotalResult/*CListCtrl*/ m_lcTotalSolutionPanel;
	CSkinHeaderCtrl m_hcPanelList;
	CListCtrlMaterial m_lcTotalMaterial;
	CSkinHeaderCtrl m_hcTotalMaterial;
};

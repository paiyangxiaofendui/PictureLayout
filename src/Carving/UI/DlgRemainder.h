#pragma once
#include "../resource.h"
#include "./UICommon/DialogChildBase.h"
#include "./UICommon/SkinListCtrlBase.h"
#include "SkinHeaderCtrl.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

// CDlgRemainder 对话框

class CDlgRemainder : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgRemainder)

public:
	CDlgRemainder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRemainder();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT_REMAINDER };

	vector<RemainderInputItem> GetRemainderInputItem();
	void SetRemainderInputItem(const vector<RemainderInputItem>& vRemainderInputItem);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnRemove();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

	void ArrangeUI();
	void ResetMaterialCombobox();

	CSkinListCtrlBase m_lcRemainder;
	CSkinHeaderCtrl m_hcRemainder;
};

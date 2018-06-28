#pragma once
#include "../Resource.h"

// CDlgResetMachiningInfo 对话框

class CDlgResetMachiningInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgResetMachiningInfo)

public:
	CDlgResetMachiningInfo(int nResetType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResetMachiningInfo();

// 对话框数据
	enum { IDD = IDD_DLG_RESET_MACHINING_INFO };
	enum {RESET_OTHERSHAPE, RESET_SLOT, RESET_HOLE};

	int m_nResetOtherShape;
	int m_nResetSlot_UpperFace;
	int m_nResetSlot_DownerFace;
	int m_nResetHole_UpperFace;
	int m_nResetHole_DownerFace;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
};

#pragma once
#include "../Resource.h"

// LingLiaoDlg 对话框

class LingLiaoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LingLiaoDlg)

public:
	LingLiaoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LingLiaoDlg();

// 对话框数据
	enum { IDD = IDD_LINGLIAO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void loadSettings();
	void saveSettings();

private:
	// 公司名称
	CString m_CompanyName;
	// 订单编号
	CString m_OrderID;
	// 排产人员
	CString m_ShiftEmployee;
	// 排产日期
	CString m_ShiftDate;
	// 领料人员
	CString m_UseEmployee;
	// 领料日期
	CString m_UseDate;
	// 仓管人员
	CString m_ManageEmployee;
	// 接单店面
	CString m_OrderStore;
	// 审核人员
	CString m_AuditEmployee;
	// 开料类型
	CString m_UseMachineName;
	// 订单注释
	CString m_OrderMoreInfo;

public:
	// 公司名称
	CString CompanyName() const { return m_CompanyName; }
	void CompanyName(CString val) { m_CompanyName = val; }

	// 订单编号
	CString OrderID() const { return m_OrderID; }
	void OrderID(CString val) { m_OrderID = val; }

	// 排产人员
	CString ShiftEmployee() const { return m_ShiftEmployee; }
	void ShiftEmployee(CString val) { m_ShiftEmployee = val; }

	// 排产日期
	CString ShiftDate() const { return m_ShiftDate; }
	void ShiftDate(CString val) { m_ShiftDate = val; }

	// 领料人员
	CString UseEmployee() const { return m_UseEmployee; }
	void UseEmployee(CString val) { m_UseEmployee = val; }

	// 领料日期
	CString UseDate() const { return m_UseDate; }
	void UseDate(CString val) { m_UseDate = val; }

	// 仓管人员
	CString ManageEmployee() const { return m_ManageEmployee; }
	void ManageEmployee(CString val) { m_ManageEmployee = val; }

	// 接单店面
	CString OrderStore() const { return m_OrderStore; }
	void OrderStore(CString val) { m_OrderStore = val; }

	// 审核人员
	CString AuditEmployee() const { return m_AuditEmployee; }
	void AuditEmployee(CString val) { m_AuditEmployee = val; }

	// 开料类型
	CString UseMachineName() const { return m_UseMachineName; }
	void UseMachineName(CString val) { m_UseMachineName = val; }

	// 订单注释
	CString OrderMoreInfo() const { return m_OrderMoreInfo; }
	void OrderMoreInfo(CString val) { m_OrderMoreInfo = val; }

};

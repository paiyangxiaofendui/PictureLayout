#pragma once
#include "../resource.h"
#include <vector>

using namespace std;

// CDlgSetReminder 对话框

class CDlgSetReminder : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetReminder)

public:
	CDlgSetReminder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetReminder();

// 对话框数据
	enum { IDD = IDD_DLG_SET_REMINDER };

	void AppendReminder(LPCTSTR szReminder);
	CString GetReminder();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

	vector<CString> m_vReminder;
	CString m_strRet;
};

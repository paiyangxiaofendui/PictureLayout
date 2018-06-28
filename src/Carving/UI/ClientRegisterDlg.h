
#pragma once

#include "../resource.h"
#include "./UICommon/Link.h"
#include "./UICommon/PngButton.h"
#include <vector>

using namespace std;

class ClientRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ClientRegisterDlg)

public:
	ClientRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ClientRegisterDlg();


	DECLARE_MESSAGE_MAP()

	enum{IDD = IDD_DIALOG_REGISTER};

public:
	enum
	{
		Dlg_Register = 0,
		Dlg_Update
	};

	int m_RegisterOrUpdate;		// 注册


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual void OnOK();
	virtual void OnCancel();


	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnSendCodeToPhone();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CString GetRandomCode();
	bool IsIgnoreVerifyPhoneNumMode();


	CString m_valClientQQNumber;
	CString m_valClientPhoneNumber;
//	CEdit m_valClientName;
	CString m_valClientAddress;
	CString m_valClientName;

	Image* m_pImgBackground;
	CLink m_Link;
	CPngButton m_btnRegister;
	CPngButton m_btnSendVerifyCodeToPhone;
	CFont m_fontEdit;

	vector<pair<CString, CString>> m_vVerifyCodePhoneNumPair;
	int m_nReEnableSendingButtonRemainTime;
	int m_nVerifyCodeSendedCount;
};
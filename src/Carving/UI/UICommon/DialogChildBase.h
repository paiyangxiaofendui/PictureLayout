#pragma once

// CDialogChildBase 对话框

class CDialogChildBase : public CDialog
{
	DECLARE_DYNAMIC(CDialogChildBase)

public:
	//CDialogChildBase(CWnd* pParent = NULL);   // 标准构造函数
	CDialogChildBase(UINT uID, CWnd* pParent = NULL);
	virtual ~CDialogChildBase();

// 对话框数据
	//enum { IDD = IDD_DIALOGCHILDBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();

};

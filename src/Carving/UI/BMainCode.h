#pragma once
#include "../resource.h"

// BMainCode 对话框

class BMainCode : public CDialogEx
{
	DECLARE_DYNAMIC(BMainCode)

public:
	BMainCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BMainCode();
	CString BMainFirst1;//反面工位1文件头
	CString BMainFirst2;//反面工位2文件头
	CString BMainLast1;//反面工位1文件尾
	CString BMainLast2;//反面工位2文件尾
// 对话框数据
	enum { IDD = IDD_BMAINCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

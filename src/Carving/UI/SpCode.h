#pragma once
#include "../resource.h"

// SpCode 对话框

class SpCode : public CDialogEx
{
	DECLARE_DYNAMIC(SpCode)

public:
	SpCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SpCode();

// 对话框数据
	enum { IDD = IDD_SPCODE };

	CString SpFirst1;	//小板工位1文件头
	CString SpFirst2;	//小板工位2文件头
	CString SpLast1;	//小板工位1文件尾
	CString SpLast2;	//小板工位2文件尾

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

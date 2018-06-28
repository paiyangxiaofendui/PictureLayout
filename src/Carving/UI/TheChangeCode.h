#pragma once
#include "afxwin.h"
#include "../Resource.h"

// TheChangeCode 对话框

class TheChangeCode : public CDialogEx
{
	DECLARE_DYNAMIC(TheChangeCode)

public:
	TheChangeCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TheChangeCode();

// 对话框数据
	enum { IDD = IDD_ChangeCode };
	virtual BOOL OnInitDialog();
	CString ChangeFirst1;//总文件工位1文件头
	CString ChangeFirst2;//总文件工位2文件头
	CString ChangeLast1;//总文件工位1文件尾
	CString ChangeLast2;//总文件工位2文件尾
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
	DECLARE_MESSAGE_MAP()
public:
};

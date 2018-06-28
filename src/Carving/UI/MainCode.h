#pragma once
#include "../resource.h"

// MainCode 对话框

class MainCode : public CDialogEx
{
	DECLARE_DYNAMIC(MainCode)

public:
	MainCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MainCode();

// 对话框数据
	enum { IDD = IDD_MAINCODE };
	CString MainFirst1;//主轴工位1文件头
	CString MainFirst2;//主轴工位2文件头
	CString MainLast1;//主轴工位1文件尾
	CString MainLast2;//主轴工位2文件尾
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
};

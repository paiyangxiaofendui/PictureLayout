#pragma once
#include "../Resource.h"

// PzCode 对话框

class PzCode : public CDialogEx
{
	DECLARE_DYNAMIC(PzCode)

public:
	PzCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PzCode();

// 对话框数据
	enum { IDD = IDD_PZCODE };
	CString PzFirst1;//排钻工位1文件头
	CString PzFirst2;//排钻工位2文件头
	CString PzLast1;//排钻工位1文件尾
	CString PzLast2;//排钻工位2文件尾
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


// ConfigConverterDlg.h : 头文件
//

#pragma once
#include <vector>

using namespace std;

// CConfigConverterDlg 对话框
class CConfigConverterDlg : public CDialogEx
{
// 构造
public:
	CConfigConverterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONFIGCONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void ConvertAllFile(vector<CString> vAllSourceDir, CString strTargetRootDir);
public:
	afx_msg void OnBnClickedButtonSourceRootDir();
	afx_msg void OnBnClickedButtonTargetRootDir();
	afx_msg void OnBnClickedButtonConvert();
};

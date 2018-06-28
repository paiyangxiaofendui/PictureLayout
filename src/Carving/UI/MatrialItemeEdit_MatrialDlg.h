#pragma once
#include "afxwin.h"
#include <vector>
#include <string>
#include "../resource.h"
// MatrialItemeEdit_MatrialDlg 对话框

class MatrialItemeEdit_MatrialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MatrialItemeEdit_MatrialDlg)

public:
	MatrialItemeEdit_MatrialDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MatrialItemeEdit_MatrialDlg();

// 对话框数据
	enum { IDD = IDD_MATRIAL_ITEM_EDIT_MATRIAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_MatrialNum;						//候选材质数量
	CString m_MatrialName;						//设置材质名称
	CComboBox m_MatrialNameBox;					//候选材质名称
	std::vector<std::string> m_MatrialNameList;	//候选材质名称列表
public:
	//设置材质名称
	CString GetMatrialName() const { return m_MatrialName; }

	//候选材质名称列表
	void SetMatrialNameList(std::vector<std::string> val) { m_MatrialNameList = val; }

	//候选材质名称列表
	void AppendMatrialName(std::string name);


public:
	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
};

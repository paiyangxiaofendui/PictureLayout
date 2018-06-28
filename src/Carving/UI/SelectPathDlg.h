#pragma once
#include "../resource.h"

// SelectPathDlg 对话框

typedef enum {
	DlgSave = false,
	DlgLoad = true,
}DlgType;

class SelectPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SelectPathDlg)

public:
	SelectPathDlg(CString _title = "请选择目录" ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SelectPathDlg();

// 对话框数据
	enum { IDD = IDD_FILE_PATH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/************************************************************************/
	/* UI                                                                   */
	/************************************************************************/
public:
	afx_msg void OnBnClickedFindPath();		//浏览
	afx_msg void OnBnClickedOpenPath();		//创建并打开目录
	afx_msg void OnBnClickedClearPath();	//清除这个目录
	afx_msg void OnBnClickedOk();			//确认选择
	afx_msg void OnBnClickedCancel();		//取消
	virtual BOOL OnInitDialog();			//初始化对话框

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	void loadSettings();			//加载配置
	void saveSettings();			//保存配置

	/************************************************************************/
	/* 工具函数                                                             */
	/************************************************************************/
protected:
	CString SelectPathDlgDoModel(CString findPath);	//返回用户选择的目录
public:

	CString SelectFileDlgDoModel(CString title, CString fileType , CString defaultname, CString findPath  ,DlgType type );	//返回用户选择的目录

private:
	CString m_currentPath;		//当前目录
	CString m_title;			//标题栏显示
	CString m_defaultSavePath;	//默认保存目录
	CString m_defaultLoadPath;	//默认加载目录
public:
	//当前目录
	CString CurrentPath() const { return m_currentPath; }
	//当前目录
	void CurrentPath(CString val) { m_currentPath = val; }

	//标题栏显示
	CString Title() const { return m_title; }
	//标题栏显示
	void Title(CString val) { m_title = val; }

	//默认保存目录
	CString DefaultSavePath() const { return m_defaultSavePath; }
	//默认保存目录
	void DefaultSavePath(CString val) { m_defaultSavePath = val; }

	//默认加载目录
	CString DefaultLoadPath() const { return m_defaultLoadPath; }
	//默认加载目录
	void DefaultLoadPath(CString val) { m_defaultLoadPath = val; }
};

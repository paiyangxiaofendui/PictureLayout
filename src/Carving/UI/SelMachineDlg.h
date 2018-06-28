#pragma once
//#include "stdafx.h"
#include <string>
#include <vector>
#include <map>


// CSelMachineDlg dialog

class CSelMachineDlg : public CDialogEx//导出G码对话框
{
	DECLARE_DYNAMIC(CSelMachineDlg)

public:
	CSelMachineDlg(CString title = "",CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelMachineDlg();

// Dialog Data
	enum { IDD = IDD_SEL_MACHINE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	/************************************************************************/
	/* 界面事件                                                             */
	/************************************************************************/
public:
	virtual BOOL OnInitDialog();		// 初始化
	afx_msg void OnBnClickedOk();		// 确认
	afx_msg void OnBnClickedCancel3();	// 取消
	afx_msg void OnNMDblclkType(NMHDR *pNMHDR, LRESULT *pResult);	// 双击表单
	void loadSettings();				//加载数据
	std::vector<std::map<std::string,std::string>> getMachineKnifes();	//获得当前机型的所有刀具

	
public:
	CString m_windowTitle;

	CComboBox m_combo;
	std::string m_nodename;
	std::string m_printname;
	std::string m_pname;
	std::string m_plast;
	std::string m_cname;
	std::string m_clast;
	std::string m_dvcut;			// 0,单面打孔，1双面打孔1（W）,2双面打孔2（H）
	std::string m_pxhx;				// p的下划线
	std::string m_cxhx;				// c的下划线

	int m_usedLineX1;				// 用户设置有效边，用于修边偏移（工位1X）
	int m_usedLineY1;				// 用户设置有效边，用于修边偏移（工位1Y）
	int m_usedLineX2;				// 用户设置有效边，用于修边偏移（工位2X）
	int m_usedLineY2;				// 用户设置有效边，用于修边偏移（工位2Y）
	CString m_EachNCLineExtra;		// NC文件每行追加
	CString m_SmallPanelNC;			//小板NC是否导出
	CString m_fileClassifyMove;		// 生成NC、贴标文件归整
	CString m_NC_CommandChange;		// NC命令字变更
	CString m_NC_SpaceRemove;		// NC坐标空格删除
	CString m_NC_ModifyArcMillingFormat;// NC修改铣弧格式

	int m_iscombine;				// 是否合并正反头尾
	int m_iscomline;				// 是否连接NC文件
	int m_iscomball;				// 是否合并所有NC文件
	int m_lessKnifeChange;			// 最少换刀设置

	bool _cutdouble;
	bool _coorsysuse;
	int _coortype;
	int _coortype2;
	std::string _grouptype;
	std::string _groupstr;
	CListCtrl m_type;

	std::string m_header;		//总文件头GFirst1
	std::string m_ender;		//总文件尾GLast1
	std::string m_GFirst2;		//总文件头GFirst2
	std::string m_GLast2;		//总文件尾GLast2

	std::string m_GBFirst1;		//总B文件头GFirst1
	std::string m_GBFirst2;		//总文件尾GLast1
	std::string m_GBLast1;		//总B文件WEI
	std::string m_GBLast2;		//总文件尾GLast2
	 

	std::string m_MFirst1;		//主轴文件头GFirst1
	std::string m_MLast1;		//主轴文件尾GLast1
	std::string m_MFirst2;		//主轴文件头GFirst2
	std::string m_MLast2;		//主轴文件尾GLast2

	std::string m_PZFirst1;		//排钻文件头GFirst1
	std::string m_PZLast1;		//排钻文件尾GLast1
	std::string m_PZFirst2;		//排钻文件头GFirst2
	std::string m_PZLast2;		//排钻文件尾GLast2


	std::string m_SPFirst1;		//小板文件头GFirst1
	std::string m_SPLast1;		//小板文件尾GLast1
	std::string m_SPFirst2;		//小板文件头GFirst2
	std::string m_SPLast2;		//小板文件尾GLast2

	float m_trim1;				//反面修边值
	float m_trim2;
	float m_trim3;
	float m_trim4;

	float m_ftrim1;//正面修边值
	float m_ftrim2;
	float m_ftrim3;
	float m_ftrim4;

	bool m_bLabelInReverseSide;
};

/*--------------------------------------------------------------------------------------------------------------------*/
//	SelectPrintDlg.h      --	 选择打印窗口声明文件
//	
//	作者：	yuanzb
//	时间：	2017.2.20
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <map>
#include <vector>
#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

class Panel;
class Component;


class CSelectPrintDlg : public CDialogEx//打印对话框
{
	DECLARE_DYNAMIC(CSelectPrintDlg)

public:
	CSelectPrintDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPrintDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_PRINT };


	std::vector<print_info> _printinfo;			//数据源
	map<int,Panel_Knife> m_pkcoor;				//板件信息


	//大板列表
	CListCtrl m_panelList;		
	//全选大板
	CButton m_checkSelectAllPanel;

	//默认是否全选大板设置
	BOOL m_checkSelectAllPanelSetting;


	//小板列表
	CListCtrl m_smallpanelList;

	CButton m_checkSelectAllSmallpanel;

	//默认是否全选小板设置
	BOOL m_checkSelectAllSmallpanelSetting;

	//图样选择
	CComboBox m_picStyle;


	//大板条码输出使能
	BOOL m_PanelCodeEnable;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void InitPanelList();
	void InitComponentList();
	void GetAllPanelInPanelList(vector<Panel*>& vAllPanel);
	void GetAllPrintInfoOfComponent(vector<print_info>& vAllPrintInfo);
	int GetIndicatePanelCheckedComponentCount(int nPanelID);
public:


	virtual BOOL OnInitDialog();				//初始化





	//设置打开选中大板
	void setPanelListCheck(int bigPanelNum);
	//设置打开选中大板
	void setPanelCheck(int panelNum);
	//获取生成NC代码的文件名称信息
	std::map<Panel*,std::map<CString,CString>> OnOutputgcodeFileName(vector<Panel*> vPanel, bool enable);
	//生成二维码
	void CreateQCodeInImage( CImage &image, CString codeWord, int code_x, int code_y , float code_size );
	//加载默认选项设置
	void loadSettings_DefaultSelect();
	//保存默认选项设置
	void saveSettings_DefaultSelect();


	afx_msg void OnSelectAllPanel();
	
	//选中大板影响小板选中
	afx_msg void OnCheckPanelItem(NMHDR *pNMHDR, LRESULT *pResult);





	//全选小板
	afx_msg void OnSelectAllSmallpanel();






	//打印图样
	afx_msg void OnBtnPrintPanel();





	//打印条码
	afx_msg void OnBtnPrintSmallPanel();




	//关闭
	afx_msg void OnBtnClose();




};

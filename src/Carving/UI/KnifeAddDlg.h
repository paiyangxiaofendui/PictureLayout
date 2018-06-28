#pragma once

//#include "hg3d/Utils.h"
//#include "hg3d/hgtypes.h"
//#include <map>
//#include "afxcmn.h"
//
#include "TheChangeCode.h"
#include "MainCode.h"
#include "PzCode.h"
#include "SpCode.h"
#include "BMainCode.h"
//#include "afxwin.h"
#include "../../../include/TinyXml/tinyxml.h"

#include "../Resource.h"
#include <vector>
#include <map>

using namespace std;

// CKnifeAddDlg dialog

class CKnifeAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKnifeAddDlg)

public:
	CKnifeAddDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKnifeAddDlg();

// Dialog Data
	enum { IDD = IDD_KNIFE_TYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_knifecode;
	float m_knifel;
	float m_knifer;
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddKnife();
	CString m_nearspeed;
	CString m_feedrate;
	CString m_mgcode;
	CString m_spindlespeed;
	CString m_tooldiameter;
	CString m_toollength;
	CString m_KnifeLength;
	CString m_typename;
	CListCtrl m_knifelist;
	afx_msg void OnBtnDelKnife();
	CComboBox m_comtype;
	CString m_mgcodee;
	std::map<int,int> map_comtype;
	CString m_endspeech;
	CString m_overspeech;
	afx_msg void OnNMDblclkKnifelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComtype();
	CString m_coorx;
	CString m_coory;
	CString m_knum;
	CString m_ksysa;
	CString m_ksysb;
	CString m_mgcode2;
	CString m_mgcodee2;
	CListCtrl m_knifetype;
	
	int t_Item;
	int l_Item;

	POINT oSize;
	bool ischeck;//修改m_Knifelist某行之前，将此值保存是否选中状态，再重新添加的时候重新写入到行数据中
	afx_msg void OnNMClickKnifetype(NMHDR *pNMHDR, LRESULT *pResult);

	void InputKnifeTypeParamToDialog( CString &str_name );

	afx_msg void OnBtnChangeKnife();
	afx_msg void OnNMDblclkKnifetype(NMHDR *pNMHDR, LRESULT *pResult);

	CTabCtrl m_tabChange;
	TheChangeCode page1;
	MainCode      page2;
	PzCode        page3;
	BMainCode     page4;
	SpCode		  page5;
	afx_msg void OnTcnSelchangingTabchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickKnifelist(NMHDR *pNMHDR, LRESULT *pResult);

	void InputMessageToDialog(CString theName);	//将刀具信息输出到对话框
	
	CComboBox m_double;				//单双工位设置
	CComboBox m_coorsysuse;			//偏移方式设置
	CComboBox m_dvcut;				//单双面打孔设置
	CImageList m_imglist;			//在头文件中声明列表控制变量CImageList m_imglist;
	CComboBoxEx m_coortype;			//坐标系设置
	CString m_btrim1;				//反面修边1
	CString m_btrim2;
	CString m_btrim3;
	CString m_btrim4;
	CString m_ftrim1;				//正面修边1
	CString m_ftrim2;
	CString m_ftrim3;
	CString m_ftrim4;
	CString m_thepname;				//贴边文件名
	CString m_theplast;				//贴标文件后缀
	CString m_thecname;				//开料文件名
	CString m_theclast;				//开料文件后缀
	CString m_groupstr;				//开料文件后缀
	//CString m_autop;				//自动贴标值
	CComboBox m_comboLabelMachine;  //自动贴标值

	afx_msg void OnBtnMachineSettingSave();

	int getSelect2Setting(CComboBox& combox);
	void setSelectText(CComboBox& combox, std::string theName );

	void EditKnifeList(int theIndex);	//修改
	afx_msg void OnCbnSelchangeCombo1();
	CButton m_iscombine;
	CButton m_Iscomline;
	CButton m_iscomball;						//是否将所有正面nc合并
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_combineSet;
	CComboBoxEx m_coortype2;

	std::map<std::string,std::string> m_usedLineMap;
	CComboBox m_usedLineX1;// 工位1有效X使用哪条边
	CComboBox m_usedLineY1;// 工位1有效Y使用哪条边
	CComboBox m_usedLineX2;// 工位2有效X使用哪条边
	CComboBox m_usedLineY2;// 工位2有效Y使用哪条边
	
	CString m_EachNCLineExtra;	// 用于每行NC码后面增加指定符合
	CButton m_lessKnifeChange;
	BOOL m_smallPanelNC;		// 小板是否独立NC
	BOOL m_fileClassifyMove;	// 生成NC、贴标文件归整
	BOOL m_NC_CommandChange;	// NC命令字变更
	BOOL m_NC_SpaceRemove;		// NC坐标空格删除
	BOOL m_bModifyArcMillingFormat; //修改铣弧格式

public:
	//加载配置信息
	void loadSettings();
	//保存配置信息
	void saveSettings();
	//加载机器名
	std::vector<CString> getMachineNames();
	//加载机器使用状态
	std::vector<CString> getMachineUseds();

private:
	TinyXml::TiXmlDocument m_doc;	//文档数据基础节点
public:
	
};

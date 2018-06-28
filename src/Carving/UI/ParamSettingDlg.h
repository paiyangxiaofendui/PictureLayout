#pragma once

#include "../resource.h"
#include "afxwin.h"
#include "../Misc/XmlHandlePlus.h"
#include "../Work/WorkDef.h"
// ParamSettingDlg 对话框

class ParamSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParamSettingDlg)

public:
	ParamSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ParamSettingDlg();

// 对话框数据
	enum { IDD = IDD_PARAM_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	/************************************************************************/
	/* UI 设置                                                              */
	/************************************************************************/
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

private:

public:
	/************************************************************************/
	/* 逻辑                                                                 */
	/************************************************************************/
private:
	//加载文件封装
	void ReloadFile();
	//保存文件封装
	void SaveFile();

public:
	//加载设置
	void loadSettings();
	//保存设置
	void saveSettings();

private:
	CString m_UISettingHgmFilePath;
	static long long m_UISettingLastTime;
	static TinyXml::TiXmlDocument m_UISetting_hgm;

public:
	/************************************************************************/
	/* 界面操作                                                             */
	/************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	//贴标
#if 1
private:
	//标签板件封边符号设置
	CString m_tiebiao_panel_edge;
	//板件封边符号列表
	CListBox m_tiebiao_edge_list;

	//贴标偏移X
	float m_tiebiao_offset_x;
	//贴标偏移Y
	float m_tiebiao_offset_y;

	//自动避孔
	BOOL m_tiebiao_auto_vec;
	//自动避槽
	BOOL m_tiebiao_auto_slot;
	//自动避内异形
	BOOL m_tiebiao_auto_inner;
	//自动避异形轮廓
	BOOL m_tiebiao_auto_outline;

	// 每打印多少张开始休息
	int m_print_per_num;
	// 每打印多少张休息多久
	int m_print_sleep;

	//贴标位置
	int m_nLablePosType;

	//领料单排序方式
	LingLiaoSortType m_eLingLiaoSortType;

	//助记号修改方式
	ReminderModifyType m_eReminderModifyType;

	//把标签文件名修改为以NC文件名开头的格式（格式：NC文件名-x，x为序号），某厂要求这样
	bool m_bModifyLabelImgNameLikeNCName;
public:
	//标签板件封边符号设置
	CString Tiebiao_panel_edge() const { return m_tiebiao_panel_edge; }
	//标签板件封边符号设置
	void Tiebiao_panel_edge(CString val) { m_tiebiao_panel_edge = val; }

	//贴标偏移X
	float Tiebiao_offset_x() const { return m_tiebiao_offset_x; }
	//贴标偏移X
	void Tiebiao_offset_x(float val) { m_tiebiao_offset_x = val; }

	//贴标偏移Y
	float Tiebiao_offset_y() const { return m_tiebiao_offset_y; }
	//贴标偏移Y
	void Tiebiao_offset_y(float val) { m_tiebiao_offset_y = val; }


	//自动避孔
	BOOL Tiebiao_auto_vec() const { return m_tiebiao_auto_vec; }
	//自动避孔
	void Tiebiao_auto_vec(BOOL val) { m_tiebiao_auto_vec = val; }
	//自动避槽
	BOOL Tiebiao_auto_slot() const { return m_tiebiao_auto_slot; }
	//自动避槽
	void Tiebiao_auto_slot(BOOL val) { m_tiebiao_auto_slot = val; }
	//自动避内异形
	BOOL Tiebiao_auto_inner() const { return m_tiebiao_auto_inner; }
	//自动避内异形
	void Tiebiao_auto_inner(BOOL val) { m_tiebiao_auto_inner = val; }
	//自动避异形轮廓
	BOOL Tiebiao_auto_outline() const { return m_tiebiao_auto_outline; }
	//自动避异形轮廓
	void Tiebiao_auto_outline(BOOL val) { m_tiebiao_auto_outline = val; }
	//自动避全部
	BOOL Tiebiao_auto_all_not() const { return (!Tiebiao_auto_vec() && !Tiebiao_auto_slot() && !Tiebiao_auto_inner() && !Tiebiao_auto_outline()); }

	//双击添加封边
	afx_msg void OnLbnDblclkTiebiaoEdge();
	//添加封边
	afx_msg void OnBnClickedTiebiaoEdgeAdd();
	//编辑封边
	afx_msg void OnBnClickedTiebiaoEdgeEdit();
	//删除封边
	afx_msg void OnBnClickedTiebiaoEdgeDel();
	//调整封边优先级
	afx_msg void OnBnClickedTiebiaoEdgeUp();
	//调整封边优先级
	afx_msg void OnBnClickedTiebiaoEdgeDown();
	//从界面中获得封边设置
	CString GetEdgeFromUI();
	//获得封边设置到界面中
	void SetEdgeToUI(CString edge);

	
	// 每打印多少张开始休息
	int Print_per_num() const { return m_print_per_num; }
	// 每打印多少张开始休息
	void Print_per_num(int val) { m_print_per_num = val; }
	
	// 每打印多少张休息多久
	int Print_sleep() const { return m_print_sleep; }
	// 每打印多少张休息多久
	void Print_sleep(int val) { m_print_sleep = val; }

	//贴标位置
	int LabelPosType() const { return m_nLablePosType;}
	void LabelPosType(int nType){ m_nLablePosType = nType; }

	//领料单排序方式
	LingLiaoSortType LingLiaoSort() const { return m_eLingLiaoSortType;}
	void LingLiaoSort(LingLiaoSortType eType){ m_eLingLiaoSortType = eType; }

	//助记号修改方式
	ReminderModifyType ReminderModify() const { return m_eReminderModifyType;}
	void ReminderModify(ReminderModifyType eType){ m_eReminderModifyType = eType; }

	//把标签文件名修改为以NC文件名开头的格式（格式：NC文件名-x，x为序号），某厂要求这样
	bool ModifyLabelImgNameLikeNCName() const {return m_bModifyLabelImgNameLikeNCName;}
	void ModifyLabelImgNameLikeNCName(bool bModifyLabelImgNameLikeNCName){m_bModifyLabelImgNameLikeNCName = bModifyLabelImgNameLikeNCName;}
#endif

public:

	CComboBox m_cbxDecimaPlace;		// 小数位数
	int m_valDecimalPlace;

	int getDecimalPlace()  { return m_valDecimalPlace; }
	void setDecimalPlace(int val) { m_valDecimalPlace = val; }

	BOOL m_chkboxPanelComments;
};

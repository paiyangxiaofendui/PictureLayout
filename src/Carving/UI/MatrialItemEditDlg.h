#pragma once
#include "../Resource.h"
#include "afxcmn.h"
#include <string>
#include <vector>
#include <map>
#include "afxpropertygridctrl.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"


class MatrialItemEditDlg : public CDialogEx//需切成品对话框
{
	DECLARE_DYNAMIC(MatrialItemEditDlg)

public:
	MatrialItemEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MatrialItemEditDlg();
	// 对话框数据
	enum { IDD = IDD_MATERIAL_ITEM_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
private:

	//////////////////////////////////////////////////////////////////////////
	//界面编辑
private:
	int m_currentIndex;										//当前操作项目
	int m_totalLine;										//总项目数
	CString m_checkInfo;									//勾选信息
	CMFCPropertyGridCtrl m_editPropertyTable;				//属性窗口

public:
	afx_msg void OnBnClickedNewItem();						//新对象
	afx_msg void OnBnClickedPrevItem();						//上一个
	afx_msg void OnBnClickedNextItem();						//下一个
	afx_msg void OnBnClickedFindItem();						//查找
	afx_msg void OnBnClickedAddItem();						//添加
	afx_msg void OnBnClickedSaveItem();						//保存
	afx_msg void OnBnClickedDelItem();						//删除

	virtual BOOL OnInitDialog();							//初始化
	virtual void OnOK();									//ok
	virtual void OnCancel();								//关闭
 

	//////////////////////////////////////////////////////////////////////////
	//数据存储
private:
	int m_currentShowIndex;												
	std::vector<int> m_selectList;										//选中的项目
	//std::vector<std::map<std::string,std::string>> m_matrialList;		//所有项目
	vector<ComponentInputItem> m_vComponentInputItem;                 //所有项目
public:
	void setSourceMatrial(/*const std::vector<std::map<std::string,std::string>> matrialList*/vector<ComponentInputItem>& vComponentInputItem);	//数据存储：设置数据
	//std::vector<std::map<std::string,std::string>> getResultMatrial();							//数据存储：获取数据
	vector<ComponentInputItem> getResultMatrial();							//数据存储：获取数据
	void setSourceSelectIndex(const std::vector<int> selectList);								//设置选中项目


	//////////////////////////////////////////////////////////////////////////
	// 数据逻辑

private:
	//生成新的条码
	CString newBarCode(int seed);
	//获得唯一条码
	CString uniqueBarCode(CString originBarCode,std::vector<CString>& barCodeCheck);
	//唯一码校准
	std::vector<CString> m_barCodeCheck;


#if 1
	//订单号
	CString m_edit_order_num ;
	BOOL m_check_order_num ;

	//柜号
	CString m_edit_cabinet_num ;
	BOOL m_check_cabinet_num ;

	//材质名称
	CString m_edit_matrail_name ;
	BOOL m_check_matrail_name ;

	//条码编码
	CString m_edit_barcode ;
	BOOL m_check_barcode ;

	//成品名称
	CString m_edit_porduct_name ;
	BOOL m_check_porduct_name ;

	//开料长度
	CString m_edit_cut_len ;
	BOOL m_check_cut_len ;

	//开料宽度
	CString m_edit_cut_width ;
	BOOL m_check_cut_width ;

	//开料厚度
	CString m_edit_cut_thin ;
	BOOL m_check_cut_thin ;

	//需切数量
	CString m_edit_cut_num ;
	BOOL m_check_cut_num ;

	//纹路方向
	CString m_edit_cut_dir ;
	BOOL m_check_cut_dir ;

	//封长1
	CString m_edit_edge_len_1 ;
	BOOL m_check_edge_len_1 ;

	//封长2
	CString m_edit_edge_len_2 ;
	BOOL m_check_edge_len_2 ;

	//封宽1
	CString m_edit_edge_width_1 ;
	BOOL m_check_edge_width_1 ;

	//封宽2
	CString m_edit_edge_width_2 ;
	BOOL m_check_edge_width_2 ;

	//客户信息
	CString m_edit_custome_info ;
	BOOL m_check_custome_info ;

	//加盟店
	CString m_edit_store ;
	BOOL m_check_store ;

	//是否开槽
	CString m_edit_slot ;
	BOOL m_check_slot ;

	//异形
	CString m_edit_other ;
	BOOL m_check_other ;

	//客户地址
	CString m_edit_custome_addr ;
	BOOL m_check_custome_addr ;
#endif

public:
	void clearModel();
	//void fromMap(std::map<std::string,std::string> item );
	void fromInputItem(ComponentInputItem item);
	//std::map<std::string,std::string> toMap(std::map<std::string,std::string> inputBase = std::map<std::string,std::string>());
	ComponentInputItem toInputItem(ComponentInputItem inputBase = ComponentInputItem());

	bool isIndexAvalible();
	bool isCurrDataOK();
	void IndexPrev();
	void IndexNext();
	void InsertNewItem();
	void LoadCurrentItem();
	void SaveCurrentItem();
	void DelCurrentItem();

	afx_msg void OnBnClickedReloadItem();
};

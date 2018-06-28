#pragma once

#include <vector>
#include <map>
#include "ImageListCtrl.h"
//#include "KnifeClass.h"

//#include "YHInfoTools.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Resource.h"

// COtherShape dialog
struct point_vector
{
	std::string _x;
	std::string _y;
};


struct param_list
{
	std::string _name;
	std::string _remark;
	std::string _abbreviation;
};

struct Shape_NamePath 
{
	std::string _name;
	std::string _path;
	std::string _index;
};

//异形设置对话框
class COtherShape : public CDialogEx
{
	DECLARE_DYNAMIC(COtherShape)

public:
	COtherShape(CWnd* pParent = NULL);   // standard constructor
	virtual ~COtherShape();

// Dialog Data
	enum { IDD = IDD_OTHERSHAPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	//加载分类文件 √
	//分类刷新到分裂列表中 √
	//分析当前otherShapeID √
	//加载OtherShapeID所在分类图形图片，及选中 √
	//加载OtherShapeID所在参数列表 √
	//加载参数到参数列表中 √
	//通过参数列表生成outline √
	//标记outline生成类型 √
	//根据outline画图	√
	//根据其他东西旋转outline √
	//确定返回outline，异形string √


	//旋转板件
	//参数变化重绘 √
	//绘制过程中数据有误停止绘制 √

private:
	static bool isInitPanelOutlineList;
	static vector<Shape_NamePath> m_PanelOutlineList;									//异形和分类
	static map<int,vector<param_list>> m_otherShapeParamList;							//异形参数列表
	static std::map<int,std::vector<std::map<std::string,std::string>>> m_outlineParam;	//带参异形点信息



	std::vector<std::map<std::string,std::string>> m_outline;	//异形参数点阵
	std::map<std::string,std::string> m_item;					//操作数据
	std::vector<PointInfo> m_pointInfos;						//异形点阵
	int m_changetype;											//旋转角度


	//////////////////////////////////////////////////////////////////////////
	// 事件
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo3();

	//更改图片选择
	afx_msg void OnPictureListSelectChange(NMHDR *pNMHDR, LRESULT *pResult);

	//修改异形板数据操作
	afx_msg void OnEnChangeListedit();

	//更改异形分类选择
	afx_msg void OnChangeSelectClassifyList();

	//刷新整个界面
	afx_msg void OnBnClickedBtnRefresh();
public:
	std::map<std::string,std::string> Item() const { return m_item; }
	void Item(std::map<std::string,std::string> val) { m_item = val; }

	vector<PointInfo> GetPointInfos(){ return m_pointInfos; }
	std::string GetPointInfosString();
	std::string GetOtherShapeString();

	int Changetype() const { return m_changetype; }
	std::string ChangetypeStr() const;
	void Changetype(int val) { m_changetype = val; }
	void Changetype(std::string val) { m_changetype = atoi(val.c_str()); }

	//////////////////////////////////////////////////////////////////////////
	// 读写数据
public:
	//从文件中加载所有异形配置数据
	void load_OutlineList_ParamList();

	//把异形配置数据更新到数据分类中
	void write_UIClassifyList_ByOutlineList();

	//将异形ID字符串反映到界面图片选择以及数据分类选择
	void write_UIPictureList_ByOtherShapeID(std::string otherShapeId = "");

	//更新异形ID对应的参数列表
	void write_UIParamList_ByOtherShapeID(std::string otherShapeId, std::vector<param_point> pa_p = std::vector<param_point>());

	//将内部m_pointInfos画到界面上
	void paint_UIPicture_ByOutline();

	//从界面上加载配置项
	std::vector<param_point> read_UIParamList_FromUI();

	//////////////////////////////////////////////////////////////////////////
	// 判断
public:
	bool isPaintable();
	bool checkParamPointLegal(const std::vector<param_point>& pa_p);
	bool checkDrawLegal(const std::vector<PointInfo>& pointInfos);

	bool checkOtherShapeIDRectangle();
	bool checkAll();

private:
	bool m_checkParamPointLegal;
	bool m_checkDrawLegal;
	bool m_paint_UIPicture_ByOutline_ok;

	//////////////////////////////////////////////////////////////////////////
	// 功能
public:
	void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);

	std::vector<param_point> convert_UIParamList_FromUI(std::vector<param_point> ui_pa_p);










public:
	int m_width;
	int m_height;
	std::string m_resultvalue;
	std::string m_stypeid;
	std::string m_isrot;

	

	CString m_othershapeString;	//异形string

	CString m_panelwidth;
	CString m_panelheight;

	vector<std::string> m_param;
	vector<point_vector> m_allpoint;

	CString m_panelID;
	afx_msg void OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusListedit();
private:
	int m_row;				//用于定位编辑参数列表的哪行
	int m_column;			//用于定位编辑参数列表的哪列

public:
	afx_msg void OnEnErrspaceListedit();


	CButton m_button1;
	CButton m_button2;
	CButton m_button3;
	CButton m_button4;
	CButton m_button5;
	CButton m_button6;
	CButton m_button7;
	CButton m_button8;
	CButton m_button9;
	CButton m_button10;
	CButton m_button11;
	CButton m_button12;
	CButton m_button13;
	CButton m_button14;
	CButton m_button15;
	CButton m_button16;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	CTreeCtrl m_pictreelist;
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	

private:
	CListBox m_ClassifyList;		//分类列表
	CImageListCtrl m_PictureList;	//图片列表
	CListCtrl m_ParamList;			//参数列表
	CEdit m_ParamListEdit;			//编辑参数列表用到的输入框
	CComboBox m_comchange;			//旋转选项

public:
	
};

#pragma once
#include "afxwin.h"

#include "../Misc/CIniFile.h"
#include <map>
#include <vector>
#include "../resource.h"

// CParamDlg dialog


//基础设置
class CParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParamDlg)

public:
	CParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParamDlg();

// Dialog Data
	enum { IDD = IDD_BASE_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	//UI
	virtual BOOL OnInitDialog();
	void update2Dialog();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	//logic
	void loadSettings();
	void saveSettings();

	// 更新单例类中的基础信息
	void UpdateBaseInfo(void);


public:
	/************************************************************************/
	/* 切割参数                                                             */
	/************************************************************************/
	//切割参数 部分窗口初始化
	void CParamDlg::OnInitDialog_QieGeCanShu();

	//更新数据变化到界面
	void CParamDlg::updateToDialog_QieGeCanShu();

	//切割参数 数据加载
	void CParamDlg::loadSettings_QieGeCanShu();

	//切割参数 数据加载
	void CParamDlg::saveSettings_QieGeCanShu();

	//从界面加载数据
	void updateFromDialog_QieGeCanShu();

	//响应下拉板厚修改
	void CParamDlg::OnCbnSelchangeComboYuandian();

	//安全高度
	float m_upheight;
	float Upheight() const { return m_upheight; }
	void Upheight(float val) { m_upheight = val; }

	//下刀深度
	float m_downheight;
	float Downheight() const { return m_downheight; }
	void Downheight(float val) { m_downheight = val; }

	//对刀原点
	CComboBox m_DuiDaoYuanDian;

	//下刀深度
	float m_knife_org_offset;
	float KnifeOrgOffset() const { return m_knife_org_offset; }
	void KnifeOrgOffset(float val) { m_knife_org_offset = val; }


	//下刀偏移1
	float m_distance1;
	float Distance1() const { return m_distance1; }
	void Distance1(float val) { m_distance1 = val; }

	//下刀偏移2
	float m_distance2;
	float Distance2() const { return m_distance2; }
	void Distance2(float val) { m_distance2 = val; }

	//原料长度
	float m_width;
	float Width() const { return m_width; }
	void Width(float val) { m_width = val; }

	//原料宽度
	float m_height;
	float Height() const { return m_height; }
	void Height(float val) { m_height = val; }

	//修边距离
	float m_Deburringwidth;
	float Deburringwidth() const { return m_Deburringwidth; }
	void Deburringwidth(float val) { m_Deburringwidth = val; }

	//排样缝隙
	float m_Kerfwidth;
	float Kerfwidth() const { return m_Kerfwidth; }
	void Kerfwidth(float val) { m_Kerfwidth = val; }

	//收刀偏移
	CComboBox control_knife_shifting;
	int m_knife_shifting;
	int Knife_shifting() const { return m_knife_shifting; }
	void Knife_shifting(int val) { m_knife_shifting = val; }

	//切割刀缝
	//float m_OtherKerfWidth;
	//float OtherKerfWidth() const { return m_OtherKerfWidth; }
	//void OtherKerfWidth(float val) { m_OtherKerfWidth = val; }

	CComboBox control_arranging_origin;
	int m_arranging_origin;
	int Arranging_origin() const { return m_arranging_origin; }
	void Arranging_origin(int val) { m_arranging_origin = val; }

	/************************************************************************/
	/* 小板切割                                                             */
	/************************************************************************/
	//小板信息 部分窗口初始化
	void CParamDlg::OnInitDialog_SmallPanelInfo();
	//更新数据变化到界面
	void CParamDlg::updateToDialog_SmallPanelInfo();
	//小板信息 数据加载
	void CParamDlg::loadSettings_SmallPanelInfo();
	//小板信息 数据加载
	void CParamDlg::saveSettings_SmallPanelInfo();
	void CParamDlg::loadSettings_MinimentPanelParam();
	void CParamDlg::saveSettings_MinimentPanelParam();
	//小板分两刀切相关功能使能
	void CParamDlg::OnBnClickedLdoublecheck();


	//小板分两刀切	
	CButton m_ldoublecut;
	BOOL m_ldoublecutEnable;
	BOOL LdoublecutEnable() const { return m_ldoublecutEnable; }
	void LdoublecutEnable(BOOL val) { m_ldoublecutEnable = val; }

	//小板最小尺寸
	float m_minarea;//最小吸附面积
	float Minarea() const { if (!m_ldoublecutEnable){ return 0;}	else {return m_minarea;} }
	void Minarea(float val) { m_minarea = val; }

	//预雕保留厚度
	float m_keepthickness;//预雕保留厚度
	float Keepthickness() const { if (!m_ldoublecutEnable){ return 0;}	else {return m_keepthickness;} }
	void Keepthickness(float val) { m_keepthickness = val; }

	//第二刀加工速度
	float m_secondspeedrate;//第二刀加工速率
	float Secondspeedrate() const { if (!m_ldoublecutEnable){ return 0;}	else {return m_secondspeedrate;} }
	void Secondspeedrate(float val) { m_secondspeedrate = val; }


	//分两刀切 - 厚度界限
	float m_twocut_threshold;
	float Twocut_threshold() const { return m_twocut_threshold; }
	void Twocut_threshold(float val) { m_twocut_threshold = val; }

	//分两刀切 - 预留厚度
	float m_twocut_reserve;
	float Twocut_reserve() const { return m_twocut_reserve; }
	void Twocut_reserve(float val) { m_twocut_reserve = val; }

	//分两刀切 - 速度
	float m_twocut_workSpeed;
	float Twocut_workSpeed() const { return m_twocut_workSpeed; }
	void Twocut_workSpeed(float val) { m_twocut_workSpeed = val; }

	//////////////////////////////////////////////////////////////////////////
	//界面没有的数据

	//开料速度
	float m_cutspeed;
	float Cutspeed() const { return m_cutspeed; }
	void Cutspeed(float val) { m_cutspeed = val; }

	//下刀速度
	float m_prospeed;
	float Prospeed() const { return m_prospeed; }
	void Prospeed(float val) { m_prospeed = val; }

	//收尾速度
	float m_endspeech;
	float Endspeech() const { return m_endspeech; }
	void Endspeech(float val) { m_endspeech = val; }

	//空跑速度
	float m_overspeed;
	float Overspeed() const { return m_overspeed; }
	void Overspeed(float val) { m_overspeed = val; }
	
	//////////////////////////////////////////////////////////////////////////

	//最小大板参数 - 最小长度
	CString m_minw;
	CString Minw() const { return m_minw; }
	void Minw(CString val) { m_minw = val; }

	//最小大板参数 - 最小宽度
	CString m_minh;
	CString Minh() const { return m_minh; }
	void Minh(CString val) { m_minh = val; }

	//最小大板参数 - 最小面积
	CString m_minsize;
	CString Minsize() const { return m_minsize; }
	void Minsize(CString val) { m_minsize = val; }


	//关于替换导出EXCEL
	BOOL m_IsOutPutExcel;
	BOOL IsOutPutExcel() const { return m_IsOutPutExcel; }
	void IsOutPutExcel(BOOL val) { m_IsOutPutExcel = val; }

	/************************************************************************/
	/* 加工信息                                                             */
	/************************************************************************/
	//加工信息 部分窗口初始化
	void CParamDlg::OnInitDialog_JiaGongXinXi();
	//更新数据变化到界面
	void CParamDlg::updateToDialog_JiaGongXinXi();
	//加工信息 数据加载
	void CParamDlg::loadSettings_JiaGongXinXi();
	//加工信息 数据加载
	void CParamDlg::saveSettings_JiaGongXinXi();
	//孔位过滤（找到指定孔直径的孔，并移除）
	void CParamDlg::OnDblclkVecFilter();
	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	void CParamDlg::OnDblclkVecAdder();


	//正面孔
	BOOL m_isVecInput;
	BOOL VecInput() const { return m_isVecInput; }
	void VecInput(BOOL val) { m_isVecInput = val; }
	//正面槽
	BOOL m_isSoltInput;
	BOOL SoltInput() const { return m_isSoltInput; }
	void SoltInput(BOOL val) { m_isSoltInput = val; }
	//反面孔
	BOOL m_isDVecInput;
	BOOL DVecInput() const { return m_isDVecInput; }
	void DVecInput(BOOL val) { m_isDVecInput = val; }
	//反面槽
	BOOL m_isDSoltInput;
	BOOL DSoltInput() const { return m_isDSoltInput; }
	void DSoltInput(BOOL val) { m_isDSoltInput = val; }
	// 是否导入异形
	BOOL m_isOthershapeInput;
	BOOL OthershapeInput() const { return m_isOthershapeInput; }
	void OthershapeInput(BOOL val) { m_isOthershapeInput = val; }
	// 是否加工通底孔
	BOOL m_isReserveDeepHole;
	BOOL ReverseDeepHole() const { return m_isReserveDeepHole; }
	void ReverseDeepHole(BOOL val) { m_isReserveDeepHole = val; }
	// 是否加工通底槽
	BOOL m_isReserveDeepSlot;
	BOOL ReverseDeepSlot() const { return m_isReserveDeepSlot; }
	void ReverseDeepSlot(BOOL val) { m_isReserveDeepSlot = val; }
	//切割倍数
	int m_editnum;
	int Editnum() const { return m_editnum; }
	void Editnum(int val) { m_editnum = val; }
	// 孔槽移位X
	float m_moveX;
	float MoveX() const { return m_moveX; }
	void MoveX(float val) { m_moveX = val; }
	// 孔槽移位Y
	float m_moveY;
	float MoveY() const { return m_moveY; }
	void MoveY(float val) { m_moveY = val; }
	//孔位过滤（找到指定孔直径的孔，并移除）
	std::vector<float> GetVecFilter();
	CString m_vecFilter;
	CString VecFilter() const { return m_vecFilter; }
	void VecFilter(CString val) { m_vecFilter = val; }
	CListBox m_listVecFilter;
	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	std::map<float,float> GetVecAdder();
	CString m_vecAdder;
	CString VecAdder() const { return m_vecAdder; }
	void VecAdder(CString val) { m_vecAdder = val; }
	CListBox m_listVecAdder;


	//翻转信息
	CComboBox control_comchange;
	int m_comchange;
	int Comchange() const { return m_comchange; }
	void Comchange(int val) { m_comchange = val; }


	//需切成品界面的轮廓点那一列是否显示
	int m_outlineColomnShowable;
	int OutlineColomnShowable() const { return m_outlineColomnShowable; }
	void OutlineColomnShowable(int val) { m_outlineColomnShowable = val; }


	/************************************************************************/
	/* 工具类                                                               */
	/************************************************************************/

	CString ReadIniSetting(CIniFile& inifile, CString classify, CString colomn);
	void WriteIniSetting(CIniFile& inifile, CString classify, CString colomn, CString value);
	
	// 最后一块小板靠边时预修边
	BOOL m_CheckPreDeburring;




	// 小板导入时设置为无纹理
	BOOL m_CheckSmallCpnNoTexture;
//	CString m_valMinLen1;
	float m_valHMinLen1;
	float m_valVMinLen2;
	float m_valHMinWidth1;
	float m_valVMinWidth2;

	afx_msg void OnBnClickedCheckSmallCpnNoTexture();

	BOOL m_CheckRemainderLibrary;
};

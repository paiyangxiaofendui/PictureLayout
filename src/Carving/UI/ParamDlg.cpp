// ParamDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "ParamDlg.h"
#include "afxdialogex.h"
#include <iostream>

#include "VecOffset.h"
#include "Vecsoltchangedlg.h"

#include "../Misc/HGTools.h"
#include "../Misc/XmlHandlePlus.h"
#include "../Misc/Misc.h"
#include "../Carving.h"

#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// CParamDlg dialog

IMPLEMENT_DYNAMIC(CParamDlg, CDialogEx)

CParamDlg::CParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParamDlg::IDD, pParent)
{
	//安全高度
	m_upheight = 0;
	//下刀深度
	m_downheight = 0;

	// 原点偏移
	m_knife_org_offset = 0;
	//下刀偏移1
	m_distance1 = 0;
	//下刀偏移2
	m_distance2 = 0;
	//原料长度
	m_width = 0;
	//原料宽度
	m_height = 0;
	//修边距离
	m_Deburringwidth = 0;
	//排样缝隙
	m_Kerfwidth = 0;
	//收刀偏移
	int m_knife_shifting = 0;
	//切割刀缝
	//m_OtherKerfWidth = 0;

	m_arranging_origin = 1;

	//小板分两刀切	
	m_ldoublecutEnable = 0;
	//小板最小尺寸
	m_minarea = 0;//最小吸附面积
	//预雕保留厚度
	m_keepthickness = 0;//预雕保留厚度
	//第二刀加工速度
	m_secondspeedrate = 0;//第二刀加工速率


	//分两刀切 - 厚度界限
	m_twocut_threshold = 0;
	//分两刀切 - 预留厚度
	m_twocut_reserve = 0;
	//分两刀切 - 速度
	m_twocut_workSpeed = 0;
	//开料速度
	m_cutspeed = 0;

	//下刀速度
	m_prospeed = 0;
	//收尾速度
	m_endspeech = 0;
	//空跑速度
	m_overspeed = 0;

	//神奇导出需切成品excel功能，关于按钮失效
	m_IsOutPutExcel = FALSE;

	//////////////////////////////////////////////////////////////////////////

	//最小大板参数 - 最小长度
	m_minw = "";
	//最小大板参数 - 最小宽度
	m_minh = "";
	//最小大板参数 - 最小面积
	m_minsize = "";


	//正面孔
	m_isVecInput = 0;
	//正面槽
	m_isSoltInput = 0;
	//反面孔
	m_isDVecInput = 0;
	//反面槽
	m_isDSoltInput = 0;
	// 是否导入异形
	m_isOthershapeInput = 0;
	// 是否加工通底孔
	m_isReserveDeepHole = 1;
	// 是否加工通底槽
	m_isReserveDeepSlot = 1;

	//切割倍数
	int m_editnum = 0;
	// 孔槽移位X
	m_moveX = 0;
	// 孔槽移位Y
	m_moveY = 0;
	//孔位过滤（找到指定孔直径的孔，并移除）
	m_vecFilter = "";
	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	m_vecAdder = "";


	//翻转信息
	int m_comchange = 0;
	//  m_valMinLen1 = _T("");
	m_valHMinLen1 = 0.0f;
	m_valVMinLen2 = 0.0f;
	m_valHMinWidth1 = 0.0f;
	m_valVMinWidth2 = 0.0f;
}

CParamDlg::~CParamDlg()
{
}

void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//切割参数
	DDX_Text(pDX, IDC_UPHEIGHT, m_upheight);
	DDX_Text(pDX, IDC_DOWNHEIGHT, m_downheight);
	DDX_Text(pDX, IDC_ORG_OFFSET, m_knife_org_offset);
	DDX_Control(pDX, IDC_COMBO_YUANDIAN, m_DuiDaoYuanDian);
	DDX_Text(pDX, IDC_DISTANCE1, m_distance1);
	DDX_Text(pDX, IDC_DISTANCE2, m_distance2);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Text(pDX, IDC_DWIDTH, m_Deburringwidth);
	DDX_Text(pDX, IDC_KWIDTH, m_Kerfwidth);
	DDX_Control(pDX, IDC_KNIFE_SHIFTING, control_knife_shifting);
	DDX_CBIndex(pDX, IDC_KNIFE_SHIFTING, m_knife_shifting);
	//DDX_Text(pDX, IDC_OTHER_KERF_WIDTH, m_OtherKerfWidth);
	DDX_Control(pDX, IDC_COMBO_LAYOUT_ORIGIN, control_arranging_origin);
	DDX_CBIndex(pDX, IDC_COMBO_LAYOUT_ORIGIN, m_arranging_origin);


	//小板切割
	DDX_Control(pDX, IDC_LDOUBLECHECK, m_ldoublecut);
	DDX_Check(pDX, IDC_LDOUBLECHECK, m_ldoublecutEnable);
	DDX_Text(pDX, IDC_MINSBAREA, m_minarea);
	DDX_Text(pDX, IDC_KEEPTHICKNESS, m_keepthickness);
	DDX_Text(pDX, IDC_SECENDSPEEDRATE, m_secondspeedrate);


	//DDX_Text(pDX, IDC_TWO_CUT_THRESHOLD, m_twocut_threshold);
	//DDX_Text(pDX, IDC_TWO_CUT_RESERVE, m_twocut_reserve);
	//DDX_Text(pDX, IDC_TWO_CUT_WORK_SPEED, m_twocut_workSpeed);


	DDX_Text(pDX, IDC_MINW, m_minw);
	DDX_Text(pDX, IDC_MINH, m_minh);
	DDX_Text(pDX, IDC_MINSIZE, m_minsize);



	//加工信息
	DDX_Check(pDX, IDC_DOWNSOLT, m_isDSoltInput);
	DDX_Check(pDX, IDC_DOWNVEC, m_isDVecInput);
	DDX_Check(pDX, IDC_UPSOLT, m_isSoltInput);
	DDX_Check(pDX, IDC_UPVEC, m_isVecInput);
	DDX_Check(pDX, IDC_OTHERSHAPE_INPUT, m_isOthershapeInput);
	DDX_Check(pDX, IDC_CHECK_DEEP_HOLE, m_isReserveDeepHole);
	DDX_Check(pDX, IDC_CHECK_DEEP_SLOT, m_isReserveDeepSlot);
	DDX_Text(pDX, IDC_EDITNUM, m_editnum);
	DDX_Text(pDX, IDC_EDITX, m_moveX);
	DDX_Text(pDX, IDC_EDITY, m_moveY);
	DDX_Control(pDX, IDC_COMBO1, control_comchange);
	DDX_CBIndex(pDX, IDC_COMBO1, m_comchange);



	DDX_Control(pDX, IDC_LIST5, m_listVecFilter);
	DDX_Control(pDX, IDC_LIST1, m_listVecAdder);
	DDX_Check(pDX, IDC_CHECK_PRE_DEBURRING, m_CheckPreDeburring);
	DDX_Check(pDX, IDC_CHECK_SMALL_COMPONENT_NO_TEXTURE, m_CheckSmallCpnNoTexture);
	//  DDX_Text(pDX, IDC_EDIT_MIN_LEN1, m_valMinLen1);
	DDX_Text(pDX, IDC_EDIT_MIN_LEN1, m_valHMinLen1);
	DDX_Text(pDX, IDC_EDIT_MIN_LEN2, m_valVMinLen2);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH1, m_valHMinWidth1);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH2, m_valVMinWidth2);
	DDX_Check(pDX, IDC_CHECKBOX_REMAINDER_LIBRARY, m_CheckRemainderLibrary);
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParamDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST5, &CParamDlg::OnDblclkVecFilter)
	ON_LBN_DBLCLK(IDC_LIST1, &CParamDlg::OnDblclkVecAdder)
	ON_CBN_SELCHANGE(IDC_COMBO_YUANDIAN, &CParamDlg::OnCbnSelchangeComboYuandian)
	ON_BN_CLICKED(IDC_LDOUBLECHECK, &CParamDlg::OnBnClickedLdoublecheck)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_SMALL_COMPONENT_NO_TEXTURE, &CParamDlg::OnBnClickedCheckSmallCpnNoTexture)

END_MESSAGE_MAP()


// 初始化
BOOL CParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnInitDialog_QieGeCanShu();
	OnInitDialog_SmallPanelInfo();
	OnInitDialog_JiaGongXinXi();

	loadSettings();

	UpdateData(FALSE);

	updateToDialog_QieGeCanShu();
	updateToDialog_SmallPanelInfo();
	updateToDialog_JiaGongXinXi();
	

	// 
	OnBnClickedCheckSmallCpnNoTexture();





	return TRUE;  
}

void CParamDlg::update2Dialog()
{

}

/*---------------------------------------*/
//	函数说明：
//		点击小板无纹理
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void CParamDlg::OnBnClickedCheckSmallCpnNoTexture()
{
	UpdateData(TRUE);

	if (m_CheckSmallCpnNoTexture == TRUE)
	{
		
		GetDlgItem(IDC_STATIC_H_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_H_TEXTURE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_WIDTH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_H_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_H_TEXTURE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_H_TEXTURE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_V_TEXTURE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_V_TEXTURE_WIDTH)->EnableWindow(FALSE);

	}
}






//保存
void CParamDlg::OnBnClickedOk()
{
	updateFromDialog_QieGeCanShu();
	UpdateData(TRUE);

	if ((m_distance2 + m_distance1) > 50)
	{
		MessageBox(_T("两个位移距离相加不可超过50mm"));
		return;
	}

	saveSettings();

	// 同步设置基础信息
	UpdateBaseInfo();



	CDialogEx::OnOK();
}




HBRUSH CParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(pWnd->GetDlgCtrlID())	//修改界面静态文本的颜色
	{
		case IDC_STATIC_CUT:	//切割参数
		case IDC_STATIC_SMALL:	//小板切割
		case IDC_STATIC_ADD:	//加工信息
		{
			pDC->SetBkMode(TRANSPARENT);   //设置背景透明
			pDC->SetTextColor(RGB(0,0,255));
			return   HBRUSH(GetStockObject(NULL_BRUSH));
		}
		break;
		default:
			break;
	}
	return hbr;
}


// 加载设置
void CParamDlg::loadSettings()
{
	loadSettings_QieGeCanShu();
	loadSettings_SmallPanelInfo();
	loadSettings_MinimentPanelParam();
	loadSettings_JiaGongXinXi();

	// 更新基础信息
	UpdateBaseInfo();
}

// 保存设置
void CParamDlg::saveSettings()
{
	saveSettings_QieGeCanShu();
 	saveSettings_SmallPanelInfo();
 	saveSettings_MinimentPanelParam();
 	saveSettings_JiaGongXinXi();
}



// 更新单例类中的基础信息
void CParamDlg::UpdateBaseInfo(void)
{
	// 初始化基础信息
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->m_BaseInfo.m_PanelLength		= m_width;				// 长度
	pSingleton->m_BaseInfo.m_PanelWidth			= m_height;				// 宽度
	pSingleton->m_BaseInfo.m_SawKerfWidth		= m_Kerfwidth;			// 锯缝
	pSingleton->m_BaseInfo.m_DeburringWidth		= m_Deburringwidth;		// 修边
	pSingleton->m_BaseInfo.m_LayoutOrg			= m_arranging_origin;	// 排样原点

	pSingleton->m_BaseInfo.m_UpHeight			= m_upheight;			// 安全高度
	pSingleton->m_BaseInfo.m_DownHeight			= m_downheight;			// 下刀深度
	pSingleton->m_BaseInfo.m_distance1			= m_distance1;			// 下刀偏移1
	pSingleton->m_BaseInfo.m_distance2			= m_distance2;			// 下刀偏移2
	pSingleton->m_BaseInfo.m_MinLength			= _ttof(m_minw);		// 最小大板参数 - 最小长度
	pSingleton->m_BaseInfo.m_MinWidth			= _ttof(m_minh);		// 最小大板参数 - 最小宽度
	pSingleton->m_BaseInfo.m_MinArea			= _ttof(m_minsize);		// 最小大板参数 - 最小面积
}





/************************************************************************/
/* 工具类                                                               */
/************************************************************************/

CString CParamDlg::ReadIniSetting(CIniFile& inifile, CString classify, CString colomn)
{
	CString result;
	inifile.GetItemString(classify,colomn,result);
	return result;
}
void CParamDlg::WriteIniSetting(CIniFile& inifile, CString classify, CString colomn, CString value)
{
	inifile.WriteItemString(classify,colomn,value);
}

/************************************************************************/
/* 切割参数                                                             */
/************************************************************************/
#if 1
//切割参数 部分窗口初始化
void CParamDlg::OnInitDialog_QieGeCanShu()
{
	m_DuiDaoYuanDian.Clear();
	m_DuiDaoYuanDian.InsertString(0,"垫板上");
	m_DuiDaoYuanDian.InsertString(1,"板厚");

	control_knife_shifting.Clear();
	control_knife_shifting.InsertString(0,"下刀偏移2");
	control_knife_shifting.InsertString(1,"板件顶点");

	control_arranging_origin.Clear();
	control_arranging_origin.InsertString(0,_T("左下角"));
	control_arranging_origin.InsertString(1,_T("左上角"));
	control_arranging_origin.InsertString(2,_T("右下角"));
	control_arranging_origin.InsertString(3,_T("右上角"));
}

//更新数据变化到界面
void CParamDlg::updateToDialog_QieGeCanShu()
{
	if (m_downheight == 10000.0)
	{
		m_DuiDaoYuanDian.SetCurSel(1);		
		GetDlgItem(IDC_DOWNHEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("10000");
	}
	else
	{
		m_DuiDaoYuanDian.SetCurSel(0);
	}
}

//切割参数 数据加载
void CParamDlg::loadSettings_QieGeCanShu()
{
	// 加载配置文件
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFileForRead();

	//安全高度
	m_upheight = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("UpHeight")));

	//下刀深度
	m_downheight = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("DownHeight")));

	//对刀原点偏移
	m_knife_org_offset = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("KnifeOrgOffset")));

	//下刀偏移1
	m_distance1 = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("Distance1")));

	//下刀偏移2
	m_distance2 = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("Distance2")));

	//原料长度
	m_width = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Width")));

	//原料宽度
	m_height = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Height")));

	//排样缝隙
	m_Kerfwidth = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Kerfwidth")));

	//修边距离
	m_Deburringwidth = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("Deburringwidth")));
	m_Deburringwidth = m_Deburringwidth + m_Kerfwidth;

	//收刀偏移
	m_knife_shifting = _ttoi(ReadIniSetting(inifile,_T("KnifeParam"),_T("KnifeShifting")));

	//切割刀缝
	//m_OtherKerfWidth = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("OtherKerfWidth")));

	// 排样原点
	m_arranging_origin = _ttoi(ReadIniSetting(inifile,_T("KnifeParam"),_T("ArrangingOrigin")));

	// 最后一块大板预修边
	CString str_pre_deburring = ReadIniSetting(inifile,_T("KnifeParam"),_T("PreDeburring"));

	if (str_pre_deburring.IsEmpty() == TRUE)
	{
		m_CheckPreDeburring = FALSE;
	}
	else
	{
		m_CheckPreDeburring = _ttoi(str_pre_deburring);
	}

	// 余料库功能
	CString str_rmd_lib = ReadIniSetting(inifile,_T("KnifeParam"),_T("RemainderLibrary"));

	if (str_rmd_lib.IsEmpty() == TRUE)
	{
		m_CheckRemainderLibrary	 = FALSE;
	}
	else
	{
		m_CheckRemainderLibrary = _ttoi(str_rmd_lib);
	}





	// 导入小板无纹理
	CString str_small_cpn_no_texture = ReadIniSetting(inifile,_T("KnifeParam"),_T("SmallCpnNoTexture"));

	if (str_small_cpn_no_texture.IsEmpty() == TRUE)
	{
		m_CheckSmallCpnNoTexture = FALSE;
	}
	else
	{
		m_CheckSmallCpnNoTexture = _ttoi(str_small_cpn_no_texture);
	}


	// 导入小板设为无纹理--最小长度1
	m_valHMinLen1 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinLen1")));

	// 导入小板设为无纹理--最小宽度1
	m_valHMinWidth1 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinWidth1")));

	// 导入小板设为无纹理--最小长度2
	m_valVMinLen2 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinLen2")));

	// 导入小板设为无纹理--最小宽度2
	m_valVMinWidth2 = _ttof(ReadIniSetting(inifile,_T("KnifeParam"),_T("MinWidth2")));


}

//切割参数 数据加载
void CParamDlg::saveSettings_QieGeCanShu()
{
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFile();




	CString inputvalue;

	//安全高度
	inputvalue.Format("%f",m_upheight);
	WriteIniSetting(inifile,_T("Machine"),_T("UpHeight"),inputvalue);

	//下刀深度
	inputvalue.Format("%f",m_downheight);
	WriteIniSetting(inifile,_T("Machine"),_T("DownHeight"),inputvalue);



	//对刀原点偏移
	inputvalue.Format("%f",m_knife_org_offset);
	WriteIniSetting(inifile,_T("Machine"),_T("KnifeOrgOffset"),inputvalue);

	
	//下刀偏移1
	inputvalue.Format("%f",m_distance1);
	WriteIniSetting(inifile,_T("Machine"),_T("Distance1"),inputvalue);

	//下刀偏移2
	inputvalue.Format("%f",m_distance2);
	WriteIniSetting(inifile,_T("Machine"),_T("Distance2"),inputvalue);

	//原料长度
	inputvalue.Format("%f",m_width);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Width"),inputvalue);

	//原料宽度
	inputvalue.Format("%f",m_height);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Height"),inputvalue);

	//排样缝隙
	inputvalue.Format("%f",m_Kerfwidth);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Kerfwidth"),inputvalue);

	//修边距离
	m_Deburringwidth = m_Deburringwidth - m_Kerfwidth;
	inputvalue.Format("%f",m_Deburringwidth);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("Deburringwidth"),inputvalue);

	//收刀偏移
	inputvalue.Format("%d",m_knife_shifting);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("KnifeShifting"),inputvalue);

	//切割刀缝
	//inputvalue.Format("%f",m_OtherKerfWidth);
	//WriteIniSetting(inifile,_T("KnifeParam"),_T("OtherKerfWidth"),inputvalue);

	// 排样原点
	inputvalue.Format("%d",m_arranging_origin);
	WriteIniSetting(inifile,_T("KnifeParam"),_T("ArrangingOrigin"),inputvalue);
	
	// 预修边
	inputvalue.Format("%d",m_CheckPreDeburring);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("PreDeburring"), inputvalue);

	// 余料库
	inputvalue.Format("%d",m_CheckRemainderLibrary);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("RemainderLibrary"), inputvalue);
	


	// 导入小板设为无纹理
	inputvalue.Format("%d",m_CheckSmallCpnNoTexture);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("SmallCpnNoTexture"), inputvalue);


	// 导入小板设为无纹理--最小长度1
	inputvalue.Format("%0.1f",m_valHMinLen1);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinLen1"), inputvalue);

	// 导入小板设为无纹理--最小宽度1
	inputvalue.Format("%0.1f",m_valHMinWidth1);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinWidth1"), inputvalue);

	// 导入小板设为无纹理--最小长度2
	inputvalue.Format("%0.1f",m_valVMinLen2);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinLen2"), inputvalue);

	// 导入小板设为无纹理--最小宽度2
	inputvalue.Format("%0.1f",m_valVMinWidth2);
	WriteIniSetting(inifile,_T("KnifeParam"), _T("MinWidth2"), inputvalue);

	inifile.OpenIniFileForWrite();
}

//从界面加载数据
void CParamDlg::updateFromDialog_QieGeCanShu()
{
	if (m_DuiDaoYuanDian.GetCurSel() == 1)
	{
		GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("10000.0");
	}
}

//响应下拉板厚修改
void CParamDlg::OnCbnSelchangeComboYuandian()
{
	int select = m_DuiDaoYuanDian.GetCurSel();
	if (select == 0)//板上
	{
		GetDlgItem(IDC_DOWNHEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("0");
		m_downheight = 0.0;
	}
	else//板厚
	{
		GetDlgItem(IDC_DOWNHEIGHT)->EnableWindow(FALSE);
		//GetDlgItem(IDC_DOWNHEIGHT)->SetWindowText("10000");
		m_downheight = 10000.0;
		UpdateData(FALSE);
	}
}

#endif
/************************************************************************/
/* 小板信息                                                             */
/************************************************************************/
#if 1
//小板信息 部分窗口初始化
void CParamDlg::OnInitDialog_SmallPanelInfo()
{

}

//更新数据变化到界面
void CParamDlg::updateToDialog_SmallPanelInfo()
{
	int changeDisableArr[] = {IDC_STATIC5,IDC_STATIC4,IDC_STATIC6,IDC_MINSBAREA,IDC_KEEPTHICKNESS,IDC_SECENDSPEEDRATE};

	for (int i = 0;i<sizeof(changeDisableArr)/sizeof(int);i++)
	{
		if (m_ldoublecutEnable)
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(FALSE);
		}
	}	
}

//小板信息 数据加载
void CParamDlg::loadSettings_SmallPanelInfo()
{
	// 加载配置文件
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFileForRead();




	//小板分两刀切	
	m_ldoublecutEnable = _ttoi(ReadIniSetting(inifile,_T("SmallCut"),_T("LDoubleCut")));

	//小板最小尺寸
	m_minarea = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("MinArea")));

	//预雕保留厚度
	m_keepthickness = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("KeepThickness")));

	//第二刀加工速度
	m_secondspeedrate = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("SecondSpeedRate")));


	//分两刀切 - 厚度界限
	m_twocut_threshold = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("twoCut_threshold")));

	//分两刀切 - 预留厚度
	m_twocut_reserve = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("twoCut_reserve")));

	//分两刀切 - 速度
	m_twocut_workSpeed = _ttof(ReadIniSetting(inifile,_T("SmallCut"),_T("twoCut_speed")));

	//////////////////////////////////////////////////////////////////////////
	//界面没有的数据

	//开料速度
	m_cutspeed = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("CutSpeech")));

	//下刀速度
	m_prospeed = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("ProcessSpeech")));

	//收尾速度
	m_endspeech = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("EndSpeech")));

	//空跑速度
	m_overspeed = _ttof(ReadIniSetting(inifile,_T("Machine"),_T("OverSpeech")));


	//关于替换导出EXCEL
	m_IsOutPutExcel = _ttoi(ReadIniSetting(inifile,_T("Machine"),_T("IsOutPutExcel")));
}

//小板信息 数据加载
void CParamDlg::saveSettings_SmallPanelInfo()
{
	CIniFile inifile;

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_name = ExeFile;
	file_name = file_name.Left(file_name.ReverseFind('\\'));

	inifile.SetFilePath(file_name);
	inifile.SetFileName(_T("Knife.ini"));
	inifile.OpenIniFile();



	CString inputvalue;
	
	//小板分两刀切	
	inputvalue.Format("%d",m_ldoublecutEnable);
	WriteIniSetting(inifile,_T("SmallCut"),_T("LDoubleCut"),inputvalue);

	//小板最小尺寸
	inputvalue.Format("%f",m_minarea);
	WriteIniSetting(inifile,_T("SmallCut"),_T("MinArea"),inputvalue);

	//预雕保留厚度
	inputvalue.Format("%f",m_keepthickness);
	WriteIniSetting(inifile,_T("SmallCut"),_T("KeepThickness"),inputvalue);

	//第二刀加工速度
	inputvalue.Format("%f",m_secondspeedrate);
	WriteIniSetting(inifile,_T("SmallCut"),_T("SecondSpeedRate"),inputvalue);


	//分两刀切 - 厚度界限
	inputvalue.Format("%f",m_twocut_threshold);
	WriteIniSetting(inifile,_T("SmallCut"),_T("twoCut_threshold"),inputvalue);

	//分两刀切 - 预留厚度
	inputvalue.Format("%f",m_twocut_reserve);
	WriteIniSetting(inifile,_T("SmallCut"),_T("twoCut_reserve"),inputvalue);

	//分两刀切 - 速度
	inputvalue.Format("%f",m_twocut_workSpeed);
	WriteIniSetting(inifile,_T("SmallCut"),_T("twoCut_speed"),inputvalue);

	//////////////////////////////////////////////////////////////////////////
	//界面没有的数据

	//开料速度
	inputvalue.Format("%f",m_cutspeed);
	WriteIniSetting(inifile,_T("Machine"),_T("CutSpeech"),inputvalue);

	//下刀速度
	inputvalue.Format("%f",m_prospeed);
	WriteIniSetting(inifile,_T("Machine"),_T("ProcessSpeech"),inputvalue);

	//收尾速度
	inputvalue.Format("%f",m_endspeech);
	WriteIniSetting(inifile,_T("Machine"),_T("EndSpeech"),inputvalue);

	//空跑速度
	inputvalue.Format("%f",m_overspeed);
	WriteIniSetting(inifile,_T("Machine"),_T("OverSpeech"),inputvalue);

	//关于替换导出EXCEL
	inputvalue.Format("%d",m_IsOutPutExcel);
	WriteIniSetting(inifile,_T("Machine"),_T("IsOutPutExcel"),inputvalue);

	inifile.OpenIniFileForWrite();
}

void CParamDlg::loadSettings_MinimentPanelParam()
{

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\Op%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//配置文件路径
	CString hgmPath = HGTools::getRelativePath("kdata\\Op.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//解密
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif








	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//加载辅助
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");




	//读取数据
	m_minw.Format("%s",docHandler.getParam("MinW").c_str());
	m_minh.Format("%s",docHandler.getParam("MinD").c_str());
	m_minsize.Format("%s",docHandler.getParam("MinSize").c_str());



	//关闭文件
	HGTools::deleteFile(xmlPath);
}

void CParamDlg::saveSettings_MinimentPanelParam()
{

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\Op%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//配置文件路径
	CString hgmPath = HGTools::getRelativePath("kdata\\Op.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//解密
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif







	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//加载辅助
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");




	//保存数据
	docHandler.setParam("MinW",m_minw.GetBuffer());
	docHandler.setParam("MinD",m_minh.GetBuffer());
	docHandler.setParam("MinSize",m_minsize.GetBuffer());





	//保存
	doc.SaveFile(xmlPath);


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	encrypt_base64( xmlPath.GetBuffer(), hgePath.GetBuffer());

#else

	HGTools::encryptFile(xmlPath,hgmPath);

#endif




	//关闭文件
	HGTools::deleteFile(xmlPath);
}



//小板分两刀切相关功能使能
//下属的元素设置是否变灰
void CParamDlg::OnBnClickedLdoublecheck()
{
	int changeDisableArr[] = {IDC_STATIC5,IDC_STATIC4,IDC_STATIC6,IDC_MINSBAREA,IDC_KEEPTHICKNESS,IDC_SECENDSPEEDRATE};

	for (int i = 0;i<sizeof(changeDisableArr)/sizeof(int);i++)
	{
		if (m_ldoublecut.GetCheck())
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(changeDisableArr[i])->EnableWindow(FALSE);
		}
	}	
}

#endif
/************************************************************************/
/* 加工信息                                                             */
/************************************************************************/
#if 1

//加工信息 部分窗口初始化
void CParamDlg::OnInitDialog_JiaGongXinXi()
{
	//孔位过滤（找到指定孔直径的孔，并移除）
	m_listVecFilter.ResetContent();
	m_listVecFilter.AddString("双击添加");


	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	m_listVecAdder.ResetContent();
	m_listVecAdder.AddString("双击添加");

	//翻转信息
	control_comchange.InsertString(0,"不翻转");
	control_comchange.InsertString(1,"正面无信息翻转");
	control_comchange.InsertString(2,"打孔优先翻转");
	control_comchange.InsertString(3,"开槽优先翻转");
	control_comchange.InsertString(4,"背面孔多翻转");
}

//更新数据变化到界面
void CParamDlg::updateToDialog_JiaGongXinXi()
{
	//孔位过滤（找到指定孔直径的孔，并移除）
	m_listVecFilter.ResetContent();
	CStringArray arrVecFilter;
	HGTools::SplitCString(m_vecFilter,arrVecFilter,';');
	for (int i = 0; i<arrVecFilter.GetSize();i++)
	{
		CString vecfilter = arrVecFilter.GetAt(i);
		if (vecfilter != "")
		{
			m_listVecFilter.AddString(vecfilter);
		}
	}
	m_listVecFilter.AddString("双击添加");



	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	m_listVecAdder.ResetContent();
	CStringArray arrVecAdder;
	HGTools::SplitCString(m_vecAdder,arrVecAdder,';');
	for (int i = 0;i<arrVecAdder.GetSize();i++)
	{
		CString vecadd = arrVecAdder.GetAt(i);
		if (vecadd != "")
		{
			m_listVecAdder.AddString(vecadd);
		}
	}
	m_listVecAdder.AddString("双击添加");


}


//加工信息 数据加载
void CParamDlg::loadSettings_JiaGongXinXi()
{

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\VecSolt%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else

	//配置文件路径
	CString hgmPath = HGTools::getRelativePath("kdata\\VecSolt.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//解密
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif






	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//加载辅助
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");


	CString inputvalue;

	//读取数据
	//正面孔
	m_isVecInput = atoi(docHandler.getParam("VecUp","1").c_str());

	//正面槽
	m_isSoltInput = atoi(docHandler.getParam("VecDown","1").c_str());

	//反面孔
	m_isDVecInput = atoi(docHandler.getParam("SoltUp","1").c_str());

	//反面槽
	m_isDSoltInput = atoi(docHandler.getParam("SoltDown","1").c_str());

	// 是否导入异形
	m_isOthershapeInput = atoi(docHandler.getParam("OthershapeInput","1").c_str());

	// 是否加工通底孔
	m_isReserveDeepHole = atoi(docHandler.getParam("ReverseDeepHole","1").c_str());

	// 是否加工通底槽
	m_isReserveDeepSlot = atoi(docHandler.getParam("ReverseDeepSlot","1").c_str());

	//切割倍数
	m_editnum = atoi(docHandler.getParam("ComNum","1").c_str());

	// 孔槽移位X
	m_moveX = atof(docHandler.getParam("ChangeX").c_str());

	// 孔槽移位Y
	m_moveX = atof(docHandler.getParam("ChangeY").c_str());

	//孔位过滤（找到指定孔直径的孔，并移除）
	m_vecFilter = (docHandler.getParam("VecFilter").c_str());

	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	m_vecAdder = (docHandler.getParam("VecAdder").c_str());

	//翻转信息
	m_comchange = atoi(docHandler.getParam("ComChange").c_str());


	//需切成品界面的轮廓点那一列是否显示
	m_outlineColomnShowable = atoi(docHandler.getParam("OutlineColomnShowable","0").c_str());


	//关闭文件
	HGTools::deleteFile(xmlPath);
}

//加工信息 数据加载
void CParamDlg::saveSettings_JiaGongXinXi()
{


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hge文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\VecSolt%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


#else
	//配置文件路径
	CString hgmPath = HGTools::getRelativePath("kdata\\VecSolt.hgm");
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgmPath);

	//解密
	//HGTools::decryptFile(hgmPath,xmlPath);

#endif

	

	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//加载辅助
	TinyXml::XmlHandlePlus docHandler(&doc);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");

	CString inputvalue;




	//保存数据

	//正面孔
	inputvalue.Format("%d",m_isVecInput);
	docHandler.setParam("VecUp",inputvalue.GetBuffer());

	//正面槽
	inputvalue.Format("%d",m_isSoltInput);
	docHandler.setParam("VecDown",inputvalue.GetBuffer());

	//反面孔
	inputvalue.Format("%d",m_isDVecInput);
	docHandler.setParam("SoltUp",inputvalue.GetBuffer());

	//反面槽
	inputvalue.Format("%d",m_isDSoltInput);
	docHandler.setParam("SoltDown",inputvalue.GetBuffer());

	// 是否导入异形
	inputvalue.Format("%d",m_isOthershapeInput);
	docHandler.setParam("OthershapeInput",inputvalue.GetBuffer());

	// 是否加工通底孔
	inputvalue.Format("%d",m_isReserveDeepHole);
	docHandler.setParam("ReverseDeepHole",inputvalue.GetBuffer());

	// 是否加工通底槽
	inputvalue.Format("%d",m_isReserveDeepSlot);
	docHandler.setParam("ReverseDeepSlot",inputvalue.GetBuffer());

	//切割倍数
	inputvalue.Format("%d",m_editnum);
	docHandler.setParam("ComNum",inputvalue.GetBuffer());

	// 孔槽移位X
	inputvalue.Format("%f",m_moveX);
	docHandler.setParam("ChangeX",inputvalue.GetBuffer());

	// 孔槽移位Y
	inputvalue.Format("%f",m_moveX);
	docHandler.setParam("ChangeY",inputvalue.GetBuffer());

	//孔位过滤（找到指定孔直径的孔，并移除）
	inputvalue.Format("%s",m_vecFilter);
	docHandler.setParam("VecFilter",inputvalue.GetBuffer());

	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	inputvalue.Format("%s",m_vecAdder);
	docHandler.setParam("VecAdder",inputvalue.GetBuffer());

	//翻转信息
	inputvalue.Format("%d",m_comchange);
	docHandler.setParam("ComChange",inputvalue.GetBuffer());

	//需切成品界面的轮廓点那一列是否显示
	inputvalue.Format("%d",m_outlineColomnShowable);
	docHandler.setParam("OutlineColomnShowable",inputvalue.GetBuffer());


	//保存
	doc.SaveFile(xmlPath);


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
#else

	//HGTools::encryptFile(xmlPath, hgmPath);

#endif




	//关闭文件
	HGTools::deleteFile(xmlPath);
}

//孔位过滤（找到指定孔直径的孔，并移除）
void CParamDlg::OnDblclkVecFilter()
{

	CString str;
	m_listVecFilter.GetText(m_listVecFilter.GetCurSel(),str);
	if (str == "双击添加")
	{
		CVecsoltchangedlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_listVecFilter.AddString(dlg.m_result);
		}
	}
	else
	{
		if (MessageBox("确认删除？","提示",MB_OKCANCEL) == IDOK)
		{
			m_listVecFilter.DeleteString(m_listVecFilter.GetCurSel());
		}
	}

	if (m_listVecFilter.GetCount()>0)
	{
		m_vecFilter = "";
		for (int i = 0; i<m_listVecFilter.GetCount()-1 ; i++)
		{
			CString str;
			m_listVecFilter.GetText(i,str);
			m_vecFilter = m_vecFilter + str + ";";
		}
	}
}

//孔位偏差（找到指定孔直径的孔，其直径加一个值）
void CParamDlg::OnDblclkVecAdder()
{
	CString str;
	m_listVecAdder.GetText(m_listVecAdder.GetCurSel(),str);
	if (str == "双击添加")
	{
		CVecOffset dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_listVecAdder.AddString(dlg.m_result);
		}
	}
	else
	{
		if (MessageBox("确认删除？","提示",MB_OKCANCEL) == IDOK)
		{
			m_listVecAdder.DeleteString(m_listVecAdder.GetCurSel());
		}
	}

	if (m_listVecAdder.GetCount()>0)
	{
		m_vecAdder = "";
		for (int i = 0; i<m_listVecAdder.GetCount()-1 ; i++)
		{
			CString str;
			m_listVecAdder.GetText(i,str);
			m_vecAdder = m_vecAdder + str + ";";
		}
	}
}

std::vector<float> CParamDlg::GetVecFilter()
{
	std::vector<float> _cxy;	//孔位过滤（找到指定孔直径的孔，并移除）
	CString sVecFilter = VecFilter();
	CStringArray sVecFilter_Lines;
	HGTools::SplitCString(sVecFilter,sVecFilter_Lines,';');
	for (int line_i = 0;line_i<sVecFilter_Lines.GetSize();line_i++)
	{
		CString vecadd = sVecFilter_Lines.GetAt(line_i);
		if (vecadd != "")
		{
			_cxy.push_back(_ttof(vecadd));
		}
	}
	return _cxy;
}

std::map<float,float> CParamDlg::GetVecAdder()
{
	std::map<float,float> vecAdder;	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	CString sVecAdder = VecAdder();
	CStringArray sVecAdder_Lines;
	HGTools::SplitCString(sVecAdder,sVecAdder_Lines,';');
	for (int line_i = 0;line_i<sVecAdder_Lines.GetSize();line_i++)
	{
		CString vecadd = sVecAdder_Lines.GetAt(line_i);
		if (vecadd != "")
		{
			CStringArray sVecAdder_Item;
			HGTools::SplitCString(vecadd,sVecAdder_Item,',');

			if (sVecAdder_Item.GetSize() == 2)
			{
				CString vecitem = sVecAdder_Item.GetAt(0);
				CString vecvalue = sVecAdder_Item.GetAt(1);
				vecAdder[_ttof(vecitem)] = _ttof(vecvalue);
			}
		}
	}
	return vecAdder;
}


#endif
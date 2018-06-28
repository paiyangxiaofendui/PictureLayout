// ParamSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParamSettingDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
#include "TiebiaoEdgeDlg.h"
#include "../Misc/Misc.h"


#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// ParamSettingDlg 对话框

IMPLEMENT_DYNAMIC(ParamSettingDlg, CDialogEx)

ParamSettingDlg::ParamSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ParamSettingDlg::IDD, pParent)
	, m_tiebiao_offset_x(0)
	, m_tiebiao_offset_y(0)
	, m_tiebiao_auto_vec(FALSE)
	, m_tiebiao_auto_slot(FALSE)
	, m_tiebiao_auto_inner(FALSE)
	, m_tiebiao_auto_outline(FALSE)
	, m_print_per_num(0)
	, m_print_sleep(0)
{


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)
	CString strTmp;
	strTmp.Format(_T("kdata\\UISetting%s"), g_szEncyptSubfix);
	m_UISettingHgmFilePath =  HGTools::getRealPathFromReleativePath(HGTools::getRelativePath(strTmp));

#else

	m_UISettingHgmFilePath =  HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\UISetting.hgm"));

#endif


	m_valDecimalPlace = 3;
}

ParamSettingDlg::~ParamSettingDlg()
{
}

void ParamSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIEBIAO_EDGE, m_tiebiao_edge_list);
	DDX_Text(pDX, IDC_TIEBIAO_OFFSET_X, m_tiebiao_offset_x);
	DDX_Text(pDX, IDC_TIEBIAO_OFFSET_Y, m_tiebiao_offset_y);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_VEC, m_tiebiao_auto_vec);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_SLOT, m_tiebiao_auto_slot);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_INNER, m_tiebiao_auto_inner);
	DDX_Check(pDX, IDC_TIEBIAO_AUTO_OUTLINE, m_tiebiao_auto_outline);
	DDX_Text(pDX, IDC_PRINT_PERNUM, m_print_per_num);
	DDV_MinMaxInt(pDX, m_print_per_num, 1, 9999);
	DDX_Text(pDX, IDC_PRINT_SLEEP, m_print_sleep);
	DDX_Control(pDX, IDC_COMBO_DECIMAL_PLACE, m_cbxDecimaPlace);
	DDX_CBIndex(pDX, IDC_COMBO_DECIMAL_PLACE, m_valDecimalPlace);
	DDV_MinMaxInt(pDX, m_valDecimalPlace, 0, 4);
	DDX_Check(pDX, IDC_CHECK_PANEL_COMMENTS, m_chkboxPanelComments);
}


BEGIN_MESSAGE_MAP(ParamSettingDlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_TIEBIAO_EDGE, &ParamSettingDlg::OnLbnDblclkTiebiaoEdge)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_ADD, &ParamSettingDlg::OnBnClickedTiebiaoEdgeAdd)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_EDIT, &ParamSettingDlg::OnBnClickedTiebiaoEdgeEdit)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_DEL, &ParamSettingDlg::OnBnClickedTiebiaoEdgeDel)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_UP, &ParamSettingDlg::OnBnClickedTiebiaoEdgeUp)
	ON_BN_CLICKED(IDC_TIEBIAO_EDGE_DOWN, &ParamSettingDlg::OnBnClickedTiebiaoEdgeDown)
END_MESSAGE_MAP()


/************************************************************************/
/* UI 设置                                                              */
/************************************************************************/

BOOL ParamSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	loadSettings();

	SetEdgeToUI(Tiebiao_panel_edge());

	UpdateData(FALSE);

	int nItemID = 0;
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("中间"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("左上角"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("右上角"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("左下角"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->InsertString(nItemID++, _T("右下角"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->SetCurSel(LabelPosType());

	((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->InsertString(0, _T("材质优先排序"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->InsertString(1, _T("厚度优先排序"));
	((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->SetCurSel(LingLiaoSort());

	((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->InsertString(0, _T("不修改"));
	((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->InsertString(1, _T("根据日期和订单号格式修改"));
	((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->SetCurSel(ReminderModify());

	((CButton*)GetDlgItem(IDC_CHECK_MODIFY_LABEL_IMG_NAME_LINE_NC_NAME))->SetCheck(ModifyLabelImgNameLikeNCName());

	// 导出NC文件小数点位数
	m_cbxDecimaPlace.InsertString(0, _T("1"));
	m_cbxDecimaPlace.InsertString(1, _T("2"));
	m_cbxDecimaPlace.InsertString(2, _T("3"));
	m_cbxDecimaPlace.InsertString(3, _T("4"));
	m_cbxDecimaPlace.SetCurSel(m_valDecimalPlace);



	return TRUE;  
}

void ParamSettingDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{

		Tiebiao_panel_edge(GetEdgeFromUI());

		LabelPosType(((CComboBox*)GetDlgItem(IDC_COMBO_LABEL_POS))->GetCurSel());

		LingLiaoSort((LingLiaoSortType)(((CComboBox*)GetDlgItem(IDC_COMBO_LingLiao_SORT_TYPE))->GetCurSel()));

		ReminderModify((ReminderModifyType)(((CComboBox*)GetDlgItem(IDC_COMBO_REMINDER_MODIFY))->GetCurSel()));

		ModifyLabelImgNameLikeNCName(((CButton*)GetDlgItem(IDC_CHECK_MODIFY_LABEL_IMG_NAME_LINE_NC_NAME))->GetCheck());

		saveSettings();
	}


	//CDialogEx::OnOK();
}


void ParamSettingDlg::OnCancel()
{
	//CDialogEx::OnCancel();
}


/************************************************************************/
/* 逻辑                                                                 */
/************************************************************************/

long long ParamSettingDlg::m_UISettingLastTime;
TinyXml::TiXmlDocument ParamSettingDlg::m_UISetting_hgm;

//加载文件封装
void ParamSettingDlg::ReloadFile()
{
	CString xmlPath = HGTools::getXmlPathFromHgxPath(m_UISettingHgmFilePath);
	long long lastTime = HGTools::GetFileLastTime(m_UISettingHgmFilePath);
	if (m_UISettingLastTime != lastTime)
	{



		// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

		//解密
		decrypt_base64(m_UISettingHgmFilePath.GetBuffer(), xmlPath.GetBuffer());
#else

		//解密
		//HGTools::decryptFile(m_UISettingHgmFilePath,xmlPath);

#endif



		//加载文件
		m_UISetting_hgm.LoadFile(xmlPath);
		//关闭文件
		HGTools::deleteFile(xmlPath);
	}
}

//保存文件封装
void ParamSettingDlg::SaveFile()
{
	CString xmlPath = HGTools::getXmlPathFromHgxPath(m_UISettingHgmFilePath);

	//保存
	m_UISetting_hgm.SaveFile(xmlPath);

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	//解密
	encrypt_base64(xmlPath.GetBuffer(), m_UISettingHgmFilePath.GetBuffer());
#else

	//解密
	//HGTools::encryptFile(xmlPath, m_UISettingHgmFilePath);

#endif




	//关闭文件
	HGTools::deleteFile(xmlPath);
}

//加载设置
void ParamSettingDlg::loadSettings()
{
	ReloadFile();

	//加载辅助
	//TinyXml::XmlHandlePlus docHandler(&m_UISetting_hgm);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_UISetting_hgm.RootElement();


	TiXmlElement* elmTheElm = NULL;
	CString strTmp;
	//标签板件封边符号设置
	//Tiebiao_panel_edge(docHandler.getParam("TiebiaoPanelEdge","").c_str());
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoPanelEdge");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_panel_edge(strTmp);

	//贴标偏移X
	//Tiebiao_offset_x(_ttof(docHandler.getParam("TiebiaoOffsetX","0.0").c_str()));
	strTmp = _T("0.0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoOffsetX");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_offset_x(_ttof(strTmp));

	//贴标偏移Y
	//Tiebiao_offset_y(_ttof(docHandler.getParam("TiebiaoOffsetY","0.0").c_str()));
	strTmp = _T("0.0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoOffsetY");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_offset_y(_ttof(strTmp));

	int nLabelPosType = 0;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoPosType");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", nLabelPosType);
	LabelPosType(nLabelPosType);

	int nLingLiaoSortType = LingLiaoSortType::LingLiao_SORT_MATERIAL_FIRST;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "LingLiaoSortType");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", nLingLiaoSortType);
	LingLiaoSort((LingLiaoSortType)nLingLiaoSortType);

	int nReminderModifyType = ReminderModifyType::REMINDER_MODIFY_NO;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ReminderModifyType");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", nReminderModifyType);
	ReminderModify((ReminderModifyType)nReminderModifyType);

	bool bModifyLabelImgNameLikeNCName = false;
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ModifyLabelImgNameLikeNCName");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", bModifyLabelImgNameLikeNCName);
	ModifyLabelImgNameLikeNCName(bModifyLabelImgNameLikeNCName);

	//自动避孔
	//Tiebiao_auto_vec(_ttoi(docHandler.getParam("TiebiaoAutoVec","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoVec");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_vec(_ttoi(strTmp));

	//自动避槽
	//Tiebiao_auto_slot(_ttoi(docHandler.getParam("TiebiaoAutoSlot","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoSlot");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_slot(_ttoi(strTmp));

	//自动避内异形
	//Tiebiao_auto_inner(_ttoi(docHandler.getParam("TiebiaoAutoInner","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoInner");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_inner(_ttoi(strTmp));

	//自动避异形轮廓
	//Tiebiao_auto_outline(_ttoi(docHandler.getParam("TiebiaoAutoOutline","0").c_str()));
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "TiebiaoAutoOutline");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Tiebiao_auto_outline(_ttoi(strTmp));

	// 每打印多少张开始休息
	//Print_per_num(_ttoi(docHandler.getParam("PrintPerNum","1").c_str()));
	strTmp = _T("1");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "PrintPerNum");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Print_per_num(_ttoi(strTmp));

	// 每打印多少张休息多久
	//Print_sleep(_ttoi(docHandler.getParam("PrintSleep","200").c_str()));
	strTmp = _T("200");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "PrintSleep");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	Print_sleep(_ttoi(strTmp));

	// 小数位数
	strTmp = _T("3");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "DecimalPlace");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	setDecimalPlace(_ttoi(strTmp));

	// 板件注释
	strTmp = _T("0");
	elmTheElm = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "PanelComments");
	xmlHandler.GetXmlAttribute(elmTheElm, "Value", strTmp);
	m_chkboxPanelComments = _ttoi(strTmp);

}

//保存设置
void ParamSettingDlg::saveSettings()
{
	ReloadFile();

	//加载辅助
	TinyXml::XmlHandlePlus docHandler(&m_UISetting_hgm);
	docHandler.changeDirectoryTo("/Root/");
	docHandler.setParamElementName("Param");

	CString inputvalue;

	//标签板件封边符号设置
	docHandler.setParam("TiebiaoPanelEdge",Tiebiao_panel_edge().GetBuffer());


	//贴标偏移X
	inputvalue.Format("%f",Tiebiao_offset_x());
	docHandler.setParam("TiebiaoOffsetX",inputvalue.GetBuffer());
	//贴标偏移Y
	inputvalue.Format("%f",Tiebiao_offset_y());
	docHandler.setParam("TiebiaoOffsetY",inputvalue.GetBuffer());

	inputvalue.Format("%d",LabelPosType());
	docHandler.setParam("TiebiaoPosType",inputvalue.GetBuffer());

	inputvalue.Format("%d",LingLiaoSort());
	docHandler.setParam("LingLiaoSortType",inputvalue.GetBuffer());

	inputvalue.Format("%d",ReminderModify());
	docHandler.setParam("ReminderModifyType",inputvalue.GetBuffer());

	inputvalue.Format("%d",ModifyLabelImgNameLikeNCName());
	docHandler.setParam("ModifyLabelImgNameLikeNCName",inputvalue.GetBuffer());

	//自动避孔
	inputvalue.Format("%d",Tiebiao_auto_vec());
	docHandler.setParam("TiebiaoAutoVec",inputvalue.GetBuffer());
	//自动避槽
	inputvalue.Format("%d",Tiebiao_auto_slot());
	docHandler.setParam("TiebiaoAutoSlot",inputvalue.GetBuffer());
	//自动避内异形
	inputvalue.Format("%d",Tiebiao_auto_inner());
	docHandler.setParam("TiebiaoAutoInner",inputvalue.GetBuffer());
	//自动避异形轮廓
	inputvalue.Format("%d",Tiebiao_auto_outline());
	docHandler.setParam("TiebiaoAutoOutline",inputvalue.GetBuffer());

	// 每打印多少张开始休息
	inputvalue.Format("%d",Print_per_num());
	docHandler.setParam("PrintPerNum",inputvalue.GetBuffer());
	// 每打印多少张休息多久
	inputvalue.Format("%d",Print_sleep());
	docHandler.setParam("PrintSleep",inputvalue.GetBuffer());

	// 小数位数
	inputvalue.Format("%d",getDecimalPlace());
	docHandler.setParam("DecimalPlace",inputvalue.GetBuffer());


	// 板件注释
	inputvalue.Format("%d",m_chkboxPanelComments);
	docHandler.setParam("PanelComments",inputvalue.GetBuffer());



	SaveFile();
}



/************************************************************************/
/* 界面操作                                                             */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
//贴标
#if 1
//双击添加封边
void ParamSettingDlg::OnLbnDblclkTiebiaoEdge()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (index < m_tiebiao_edge_list.GetCount())
	{
		m_tiebiao_edge_list.GetText(index,str);

		TiebiaoEdgeDlg dlg;
		dlg.OneEdgeString(str);
		if (dlg.DoModal() == IDOK)
		{
			m_tiebiao_edge_list.DeleteString(index);
			m_tiebiao_edge_list.InsertString(index,dlg.OneEdgeString());
		}
	}
}

//添加封边
void ParamSettingDlg::OnBnClickedTiebiaoEdgeAdd()
{
	TiebiaoEdgeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_tiebiao_edge_list.AddString(dlg.OneEdgeString());
	}
}

//编辑封边
void ParamSettingDlg::OnBnClickedTiebiaoEdgeEdit()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (index < m_tiebiao_edge_list.GetCount())
	{
		m_tiebiao_edge_list.GetText(index,str);

		TiebiaoEdgeDlg dlg;
		dlg.OneEdgeString(str);
		if (dlg.DoModal() == IDOK)
		{
			m_tiebiao_edge_list.DeleteString(index);
			m_tiebiao_edge_list.InsertString(index,dlg.OneEdgeString());
		}
	}
}

//删除封边
void ParamSettingDlg::OnBnClickedTiebiaoEdgeDel()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (0<= index && index < m_tiebiao_edge_list.GetCount())
	{
		m_tiebiao_edge_list.GetText(index,str);
		m_tiebiao_edge_list.DeleteString(index);
	}
}

//调整封边优先级
void ParamSettingDlg::OnBnClickedTiebiaoEdgeUp()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (0 < index && index <= m_tiebiao_edge_list.GetCount() - 1)
	{
		m_tiebiao_edge_list.GetText(index,str);
		m_tiebiao_edge_list.DeleteString(index);
		m_tiebiao_edge_list.InsertString(index - 1,str);
		m_tiebiao_edge_list.SetCurSel(index - 1);
	}
}

//调整封边优先级
void ParamSettingDlg::OnBnClickedTiebiaoEdgeDown()
{
	CString str;
	int index = m_tiebiao_edge_list.GetCurSel();
	if (0 <= index && index < m_tiebiao_edge_list.GetCount() - 1)
	{
		m_tiebiao_edge_list.GetText(index,str);
		m_tiebiao_edge_list.InsertString(index + 2,str);
		m_tiebiao_edge_list.SetCurSel(index + 2);
		m_tiebiao_edge_list.DeleteString(index);
	}
}

//从界面中获得封边设置
CString ParamSettingDlg::GetEdgeFromUI()
{
	CString result;
	for (int i = 0; i <  m_tiebiao_edge_list.GetCount() ; i++)
	{
		CString str;
		m_tiebiao_edge_list.GetText(i,str);
		str.Replace(";","");

		result = result + str + ";";
	}
	return result;
}

//获得封边设置到界面中
void ParamSettingDlg::SetEdgeToUI( CString edge )
{
	CStringArray lines;
	HGTools::SplitCString(edge,lines,';');
	for (int line_i = 0;line_i<lines.GetSize();line_i++)
	{
		CString csoneEdgeString = lines.GetAt(line_i);
		if (csoneEdgeString != "")
		{
			m_tiebiao_edge_list.InsertString(line_i,csoneEdgeString);
		}
	}
}

#endif


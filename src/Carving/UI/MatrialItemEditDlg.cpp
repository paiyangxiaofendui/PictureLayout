// PleaseWaitDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrialItemEditDlg.h"
#include "afxdialogex.h"
#include "../Misc/HGTools.h"
//#include "OtherShapeHelper.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Misc/Misc.h"
#include "../Work/WorkDef.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
// MatrialItemEditDlg 对话框

IMPLEMENT_DYNAMIC(MatrialItemEditDlg, CDialogEx)
	
	MatrialItemEditDlg::MatrialItemEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MatrialItemEditDlg::IDD, pParent)
, m_currentIndex(0)
, m_totalLine(0)
, m_check_barcode(TRUE)
, m_checkInfo(_T(""))
{
}

MatrialItemEditDlg::~MatrialItemEditDlg()
{
}

void MatrialItemEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INDEX, m_currentIndex);
	DDX_Text(pDX, IDC_TOTALNUM, m_totalLine);




#if 1

	//订单号
	DDX_Text(pDX,        IDC_EDIT_ORDER_NUM	,m_edit_order_num );
	DDX_Check(pDX, IDC_CHECK_ORDER_NUM,m_check_order_num );

	//柜号
	DDX_Text(pDX,        IDC_EDIT_CABINET_NUM	,m_edit_cabinet_num );
	DDX_Check(pDX, IDC_CHECK_CABINET_NUM,m_check_cabinet_num );

	//材质名称
	DDX_Text(pDX,        IDC_EDIT_MATRAIL_NAME	,m_edit_matrail_name );
	DDX_Check(pDX, IDC_CHECK_MATRAIL_NAME,m_check_matrail_name );

	//条码编码
	DDX_Text(pDX,        IDC_EDIT_BARCODE	,m_edit_barcode );
	DDX_Check(pDX, IDC_CHECK_BARCODE,m_check_barcode );

	//成品名称
	DDX_Text(pDX,        IDC_EDIT_PORDUCT_NAME	,m_edit_porduct_name );
	DDX_Check(pDX, IDC_CHECK_PORDUCT_NAME,m_check_porduct_name );

	//开料长度
	DDX_Text(pDX,        IDC_EDIT_CUT_LEN	,m_edit_cut_len );
	DDX_Check(pDX, IDC_CHECK_CUT_LEN,m_check_cut_len );

	//开料宽度
	DDX_Text(pDX,        IDC_EDIT_CUT_WIDTH	,m_edit_cut_width );
	DDX_Check(pDX, IDC_CHECK_CUT_WIDTH,m_check_cut_width );

	//开料厚度
	DDX_Text(pDX,        IDC_EDIT_CUT_THIN	,m_edit_cut_thin );
	DDX_Check(pDX, IDC_CHECK_CUT_THIN,m_check_cut_thin );

	//需切数量
	DDX_Text(pDX,        IDC_EDIT_CUT_NUM	,m_edit_cut_num );
	DDX_Check(pDX, IDC_CHECK_CUT_NUM,m_check_cut_num );

	//纹路方向
	DDX_Text(pDX,        IDC_EDIT_CUT_DIR	,m_edit_cut_dir );
	DDX_Check(pDX, IDC_CHECK_CUT_DIR,m_check_cut_dir );

	//封长1
	DDX_Text(pDX,        IDC_EDIT_EDGE_LEN_1	,m_edit_edge_len_1 );
	DDX_Check(pDX, IDC_CHECK_EDGE_LEN_1,m_check_edge_len_1 );

	//封长2
	DDX_Text(pDX,        IDC_EDIT_EDGE_LEN_2	,m_edit_edge_len_2 );
	DDX_Check(pDX, IDC_CHECK_EDGE_LEN_2,m_check_edge_len_2 );

	//封宽1
	DDX_Text(pDX,        IDC_EDIT_EDGE_WIDTH_1	,m_edit_edge_width_1 );
	DDX_Check(pDX, IDC_CHECK_EDGE_WIDTH_1,m_check_edge_width_1 );

	//封宽2
	DDX_Text(pDX,        IDC_EDIT_EDGE_WIDTH_2	,m_edit_edge_width_2 );
	DDX_Check(pDX, IDC_CHECK_EDGE_WIDTH_2,m_check_edge_width_2 );

	//客户信息
	DDX_Text(pDX,        IDC_EDIT_CUSTOME_INFO	,m_edit_custome_info );
	DDX_Check(pDX, IDC_CHECK_CUSTOME_INFO,m_check_custome_info );

	//加盟店
	DDX_Text(pDX,        IDC_EDIT_STORE	,m_edit_store );
	DDX_Check(pDX, IDC_CHECK_STORE,m_check_store );

	//是否开槽
	DDX_Text(pDX,        IDC_EDIT_SLOT	,m_edit_slot );
	DDX_Check(pDX, IDC_CHECK_SLOT,m_check_slot );

	//异形
	DDX_Text(pDX,        IDC_EDIT_OTHER	,m_edit_other );
	DDX_Check(pDX, IDC_CHECK_OTHER,m_check_other );

	//客户地址
	DDX_Text(pDX,        IDC_EDIT_CUSTOME_ADDR	,m_edit_custome_addr );
	DDX_Check(pDX, IDC_CHECK_CUSTOME_ADDR,m_check_custome_addr );
#endif
}


BEGIN_MESSAGE_MAP(MatrialItemEditDlg, CDialog)
	ON_BN_CLICKED(ID_NEW_ITEM, &MatrialItemEditDlg::OnBnClickedNewItem)
	ON_BN_CLICKED(ID_PREV_ITEM, &MatrialItemEditDlg::OnBnClickedPrevItem)
	ON_BN_CLICKED(ID_NEXT_ITEM, &MatrialItemEditDlg::OnBnClickedNextItem)
	ON_BN_CLICKED(ID_FIND_ITEM, &MatrialItemEditDlg::OnBnClickedFindItem)
	ON_BN_CLICKED(ID_ADD_ITEM, &MatrialItemEditDlg::OnBnClickedAddItem)
	ON_BN_CLICKED(ID_SAVE_ITEM, &MatrialItemEditDlg::OnBnClickedSaveItem)
	ON_BN_CLICKED(ID_DEL_ITEM, &MatrialItemEditDlg::OnBnClickedDelItem)
	ON_BN_CLICKED(ID_RELOAD_ITEM, &MatrialItemEditDlg::OnBnClickedReloadItem)
END_MESSAGE_MAP()


// MatrialItemEditDlg 消息处理程序
//////////////////////////////////////////////////////////////////////////
// 界面

 

//新对象
void MatrialItemEditDlg::OnBnClickedNewItem()
{
	UpdateData(TRUE);
	if (isIndexAvalible())
	{
		clearModel();
	}
	UpdateData(FALSE);
}

//上一个
void MatrialItemEditDlg::OnBnClickedPrevItem()
{
	OnBnClickedSaveItem();

	if (isIndexAvalible())
	{
		IndexPrev();
		LoadCurrentItem();
		UpdateData(FALSE);
	}
}

//下一个
void MatrialItemEditDlg::OnBnClickedNextItem()
{
	OnBnClickedSaveItem();

	if (isIndexAvalible())
	{
		IndexNext();
		LoadCurrentItem();
		UpdateData(FALSE);
	}
}

//查找
void MatrialItemEditDlg::OnBnClickedFindItem()
{

}

//添加
void MatrialItemEditDlg::OnBnClickedAddItem()
{
	UpdateData(TRUE);
	if (isCurrDataOK())
	{
		InsertNewItem();
	}
	UpdateData(FALSE);
}

//保存
void MatrialItemEditDlg::OnBnClickedSaveItem()
{
	UpdateData(TRUE);
	if (isCurrDataOK() && isIndexAvalible())
	{
		SaveCurrentItem();
	}
	UpdateData(FALSE);
}

//删除
void MatrialItemEditDlg::OnBnClickedDelItem()
{
	UpdateData(TRUE);
	if (isIndexAvalible())
	{
		DelCurrentItem();
		LoadCurrentItem();
	}
	else
	{

	}
	UpdateData(FALSE);
}

//撤销
void MatrialItemEditDlg::OnBnClickedReloadItem()
{
	if (isIndexAvalible())
	{
		LoadCurrentItem();
		UpdateData(FALSE);
	}
}


//初始化
BOOL MatrialItemEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < m_vComponentInputItem.size() ; i++)
	{
		m_barCodeCheck.push_back(m_vComponentInputItem[i].m_strBarcode);
	}

	if (m_vComponentInputItem.size()>0)
	{
		m_currentIndex = m_selectList.at(0);
		m_totalLine = m_vComponentInputItem.size() - 1;
		LoadCurrentItem();
	}
	UpdateData(FALSE);
	return TRUE;
}

//OK
void MatrialItemEditDlg::OnOK()
{
	OnBnClickedSaveItem();
	CDialog::OnOK();
}

//取消
void MatrialItemEditDlg::OnCancel()
{
	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 数据存储

//数据存储：设置数据
void MatrialItemEditDlg::setSourceMatrial(/*const std::vector<std::map<std::string,std::string>> matrialList*/vector<ComponentInputItem>& vComponentInputItem)
{
	//m_matrialList = matrialList;
	m_vComponentInputItem = vComponentInputItem;
}

//数据存储：获取数据
void MatrialItemEditDlg::setSourceSelectIndex(const std::vector<int> selectList)
{
	m_selectList = selectList;
}

//设置选中项目
//std::vector<std::map<std::string,std::string>> MatrialItemEditDlg::getResultMatrial()
//{
//	return m_matrialList;
//}
vector<ComponentInputItem> MatrialItemEditDlg::getResultMatrial()
{
	return m_vComponentInputItem;
}


//////////////////////////////////////////////////////////////////////////
// 逻辑操作


void MatrialItemEditDlg::clearModel()
{
#if 1
	//订单号
	m_edit_order_num = "";

	//柜号
	m_edit_cabinet_num = "";

	//材质名称
	m_edit_matrail_name = "";

	//条码编码
	m_edit_barcode = "";

	//成品名称
	m_edit_porduct_name = "";

	//开料长度
	m_edit_cut_len = "";

	//开料宽度
	m_edit_cut_width = "";

	//开料厚度
	m_edit_cut_thin =  "";

	//需切数量
	m_edit_cut_num = "";

	//纹路方向
	m_edit_cut_dir = "";

	//封长1
	m_edit_edge_len_1 = "";

	//封长2
	m_edit_edge_len_2 = "";

	//封宽1
	m_edit_edge_width_1 = "";

	//封宽2
	m_edit_edge_width_2 = "";

	//客户信息
	m_edit_custome_info = "";

	//加盟店
	m_edit_store  = "";

	//是否开槽
	m_edit_slot = "";

	//异形
	m_edit_other = "";

	//客户地址
	m_edit_custome_addr = "";
#endif
}

//void MatrialItemEditDlg::fromMap(std::map<std::string,std::string> item)
//{
//#if 1
//	//订单号
//	m_edit_order_num = item["order"].c_str();
//
//	//柜号
//	m_edit_cabinet_num = item["cabinet"].c_str();
//
//	//材质名称
//	m_edit_matrail_name = item["matrial"].c_str();
//
//	//条码编码
//	m_edit_barcode = item["info"].c_str();
//
//	//成品名称
//	m_edit_porduct_name = item["porduct"].c_str();
//
//	//开料长度
//	m_edit_cut_len = item["width"].c_str();
//
//	//开料宽度
//	m_edit_cut_width = item["height"].c_str();
//
//	//开料厚度
//	m_edit_cut_thin = item["thin"].c_str();
//
//	//需切数量
//	m_edit_cut_num = item["num"].c_str();
//
//	//纹路方向
//	m_edit_cut_dir = item["dir"].c_str();
//
//	//封长1
//	m_edit_edge_len_1 = item["edgeLen1"].c_str();
//
//	//封宽1
//	m_edit_edge_width_1 = item["edgeWidth1"].c_str();
//
//	//封长2
//	m_edit_edge_len_2 = item["edgeLen2"].c_str();
//
//	//封宽2
//	m_edit_edge_width_2 = item["edgeWidth2"].c_str();
//
//	//客户信息
//	m_edit_custome_info = item["customeInfo"].c_str();
//
//	//加盟店
//	m_edit_store  = item["store"].c_str();
//
//	//是否开槽
//	m_edit_slot = item["DrillingLogo"].c_str();
//
//	//异形
//	m_edit_other = item["otherID"].c_str();
//
//	//客户地址
//	m_edit_custome_addr = item["customeAddr"].c_str();
//#endif
//
//}

void MatrialItemEditDlg::fromInputItem(ComponentInputItem item)
{
#if 1
	//订单号
	//m_edit_order_num = item["order"].c_str();
	m_edit_order_num = item.m_strOrderID;

	//柜号
	//m_edit_cabinet_num = item["cabinet"].c_str();
	m_edit_cabinet_num = item.m_strCabinetID;

	//材质名称
	//m_edit_matrail_name = item["matrial"].c_str();
	m_edit_matrail_name = item.m_strMaterial;

	//条码编码
	//m_edit_barcode = item["info"].c_str();
	m_edit_barcode = item.m_strBarcode;

	//成品名称
	//m_edit_porduct_name = item["porduct"].c_str();
	m_edit_porduct_name = item.m_strPanelName;

	//开料长度
	//m_edit_cut_len = item["width"].c_str();
	//m_edit_cut_len.Format(_T("%f"), item.m_fLength);
	m_edit_cut_len = GetFloatStringTrimming(item.m_fLength, 1);

	//开料宽度
	//m_edit_cut_width = item["height"].c_str();
	//m_edit_cut_width.Format(_T("%f"), item.m_fWidth);
	m_edit_cut_width = GetFloatStringTrimming(item.m_fWidth, 1);

	//开料厚度
	//m_edit_cut_thin = item["thin"].c_str();
	//m_edit_cut_thin.Format(_T("%f"), item.m_fThickness);
	m_edit_cut_thin = GetFloatStringTrimming(item.m_fThickness, 1);

	//需切数量
	//m_edit_cut_num = item["num"].c_str();
	m_edit_cut_num.Format(_T("%d"), item.m_nCount);

	//纹路方向
	//m_edit_cut_dir = item["dir"].c_str();
	m_edit_cut_dir = item.m_strTexture;

	//封长1
	//m_edit_edge_len_1 = item["edgeLen1"].c_str();
	//m_edit_edge_len_1.Format(_T("%f"), item.m_afBanding[0]);
	m_edit_edge_len_1 = GetFloatString(item.m_afBanding[0], 1);

	//封宽1
	//m_edit_edge_width_1 = item["edgeWidth1"].c_str();
	//m_edit_edge_width_1.Format(_T("%f"), item.m_afBanding[1]);
	m_edit_edge_width_1 = GetFloatString(item.m_afBanding[1], 1);

	//封长2
	//m_edit_edge_len_2 = item["edgeLen2"].c_str();
	//m_edit_edge_len_2.Format(_T("%f"), item.m_afBanding[2]);
	m_edit_edge_len_2 = GetFloatString(item.m_afBanding[2], 1);

	//封宽2
	//m_edit_edge_width_2 = item["edgeWidth2"].c_str();
	//m_edit_edge_width_2.Format(_T("%f"), item.m_afBanding[3]);
	m_edit_edge_width_2 = GetFloatString(item.m_afBanding[3], 1);

	//客户信息
	//m_edit_custome_info = item["customeInfo"].c_str();
	m_edit_custome_info = item.m_strCustomerInfo;

	//加盟店
	//m_edit_store  = item["store"].c_str();
	m_edit_store  = item.m_strJoinedStore;

	//是否开槽
	//m_edit_slot = item["DrillingLogo"].c_str();

	//异形
	//m_edit_other = item["otherID"].c_str();
	m_edit_other = item.m_strOtherShapeID;

	//客户地址
	//m_edit_custome_addr = item["customeAddr"].c_str();
	m_edit_custome_addr = item.m_strCustomerAddress;
#endif

}

//std::map<std::string,std::string> MatrialItemEditDlg::toMap(std::map<std::string,std::string> inputBase)
//{
//#if 1
//	//订单号
//	inputBase["order"] = HGTools::Convert2StdString(m_edit_order_num);
//
//	//柜号
//	inputBase["cabinet"] = HGTools::Convert2StdString(m_edit_cabinet_num);
//
//	//材质名称
//	inputBase["matrial"] = HGTools::Convert2StdString(m_edit_matrail_name);
//
//	//条码编码
//	inputBase["info"] = HGTools::Convert2StdString(m_edit_barcode);
//
//	//成品名称
//	inputBase["porduct"] = HGTools::Convert2StdString(m_edit_porduct_name);
//
//	//开料长度
//	inputBase["width"] = HGTools::Convert2StdString(m_edit_cut_len);
//
//	//开料宽度
//	inputBase["height"] = HGTools::Convert2StdString(m_edit_cut_width);
//
//	//开料厚度
//	inputBase["thin"] = HGTools::Convert2StdString(m_edit_cut_thin);
//
//	//需切数量
//	inputBase["num"] = HGTools::Convert2StdString(m_edit_cut_num);
//
//	//纹路方向
//	inputBase["dir"] = HGTools::Convert2StdString(m_edit_cut_dir);
//
//	//封长1
//	inputBase["edgeLen1"] = HGTools::Convert2StdString(m_edit_edge_len_1);
//
//	//封宽1
//	inputBase["edgeWidth1"] = HGTools::Convert2StdString(m_edit_edge_width_1);
//
//	//封长2
//	inputBase["edgeLen2"] = HGTools::Convert2StdString(m_edit_edge_len_2);
//
//	//封宽2
//	inputBase["edgeWidth2"] = HGTools::Convert2StdString(m_edit_edge_width_2);
//
//	//客户信息
//	inputBase["customeInfo"] = HGTools::Convert2StdString(m_edit_custome_info);
//
//	//加盟店
//	inputBase["store"] = HGTools::Convert2StdString(m_edit_store);
//
//	//是否开槽
//	inputBase["DrillingLogo"] = HGTools::Convert2StdString(m_edit_slot);
//
//	//异形
//	inputBase["otherID"] = HGTools::Convert2StdString(m_edit_other);
//
//	//客户地址
//	inputBase["customeAddr"] = HGTools::Convert2StdString(m_edit_custome_addr);
//
//	//////////////////////////////////////////////////////////////////////////
//	// 更新成品长宽 = 原料长宽 + 封边
//	//成品长度
//	CString inputValue;
//
//	//成品长度
//	inputValue.Format("%.1f",atof(inputBase["width"].c_str()) + atof(inputBase["edgeWidth1"].c_str()) + atof(inputBase["edgeWidth2"].c_str()));
//	inputBase["ProductWidth"] = HGTools::Convert2StdString(inputValue);
//
//	//成品宽度
//	inputValue.Format("%.1f",atof(inputBase["height"].c_str()) + atof(inputBase["edgeLen1"].c_str()) + atof(inputBase["edgeLen2"].c_str()));
//	inputBase["ProductHeight"] = HGTools::Convert2StdString(inputValue);
//
//	//成品厚度
//	inputBase["ProductThin"] = inputBase["thin"];
//
//#endif
//
//	return inputBase;
//}

ComponentInputItem MatrialItemEditDlg::toInputItem(ComponentInputItem inputBase)
{
#if 1
	bool bNeedToResetOtherShapePoint = false;
	//订单号
	//inputBase["order"] = HGTools::Convert2StdString(m_edit_order_num);
	inputBase.m_strOrderID = m_edit_order_num;

	//柜号
	//inputBase["cabinet"] = HGTools::Convert2StdString(m_edit_cabinet_num);
	inputBase.m_strCabinetID = m_edit_cabinet_num;

	//材质名称
	//inputBase["matrial"] = HGTools::Convert2StdString(m_edit_matrail_name);
	inputBase.m_strMaterial = m_edit_matrail_name;

	//条码编码
	//inputBase["info"] = HGTools::Convert2StdString(m_edit_barcode);
	inputBase.m_strBarcode = m_edit_barcode;

	//成品名称
	//inputBase["porduct"] = HGTools::Convert2StdString(m_edit_porduct_name);
	inputBase.m_strPanelName = m_edit_porduct_name;

	//开料长度
	//inputBase["width"] = HGTools::Convert2StdString(m_edit_cut_len);
	if(!IsFloatEqual(inputBase.m_fLength, atof(m_edit_cut_len.GetBuffer())))
		bNeedToResetOtherShapePoint = true;
	inputBase.m_fLength = atof(m_edit_cut_len.GetBuffer());
	m_edit_cut_len.ReleaseBuffer();

	//开料宽度
	//inputBase["height"] = HGTools::Convert2StdString(m_edit_cut_width);
	if(!IsFloatEqual(inputBase.m_fWidth, atof(m_edit_cut_width.GetBuffer())))
		bNeedToResetOtherShapePoint = true;
	inputBase.m_fWidth = atof(m_edit_cut_width.GetBuffer());
	m_edit_cut_width.ReleaseBuffer();

	//开料厚度
	//inputBase["thin"] = HGTools::Convert2StdString(m_edit_cut_thin);
	inputBase.m_fThickness = atof(m_edit_cut_thin.GetBuffer());
	m_edit_cut_thin.ReleaseBuffer();

	//需切数量
	//inputBase["num"] = HGTools::Convert2StdString(m_edit_cut_num);
	inputBase.m_nCount = atoi(m_edit_cut_num.GetBuffer());
	m_edit_cut_num.ReleaseBuffer();

	//纹路方向
	//inputBase["dir"] = HGTools::Convert2StdString(m_edit_cut_dir);
	inputBase.m_strTexture = m_edit_cut_dir;

	//封长1
	//inputBase["edgeLen1"] = HGTools::Convert2StdString(m_edit_edge_len_1);
	inputBase.m_afBanding[0] = _ttof(m_edit_edge_len_1.GetBuffer());
	m_edit_edge_len_1.ReleaseBuffer();

	//封宽1
	//inputBase["edgeWidth1"] = HGTools::Convert2StdString(m_edit_edge_width_1);
	inputBase.m_afBanding[1] = _ttof(m_edit_edge_width_1.GetBuffer());
	m_edit_edge_width_1.ReleaseBuffer();

	//封长2
	//inputBase["edgeLen2"] = HGTools::Convert2StdString(m_edit_edge_len_2);
	inputBase.m_afBanding[2] = _ttof(m_edit_edge_len_2.GetBuffer());
	m_edit_edge_len_2.ReleaseBuffer();

	//封宽2
	//inputBase["edgeWidth2"] = HGTools::Convert2StdString(m_edit_edge_width_2);
	inputBase.m_afBanding[3] = _ttof(m_edit_edge_width_2.GetBuffer());
	m_edit_edge_width_2.ReleaseBuffer();

	//客户信息
	//inputBase["customeInfo"] = HGTools::Convert2StdString(m_edit_custome_info);
	inputBase.m_strCustomerInfo = m_edit_custome_info;

	//加盟店
	//inputBase["store"] = HGTools::Convert2StdString(m_edit_store);
	inputBase.m_strJoinedStore = m_edit_store;


	//inputBase["DrillingLogo"] = HGTools::Convert2StdString(m_edit_slot);
	inputBase.m_strHoleSlotFlag = m_edit_slot;

	//异形
	//inputBase["otherID"] = HGTools::Convert2StdString(m_edit_other);
	if(inputBase.m_strOtherShapeID.Compare(m_edit_other) != 0)
		bNeedToResetOtherShapePoint = true;
	inputBase.m_strOtherShapeID = m_edit_other;

	//客户地址
	//inputBase["customeAddr"] = HGTools::Convert2StdString(m_edit_custome_addr);
	inputBase.m_strCustomerAddress = m_edit_custome_addr;

	//////////////////////////////////////////////////////////////////////////
	// 更新成品长宽 = 原料长宽 + 封边
	////成品长度
	//CString inputValue;

	////成品长度
	//inputValue.Format("%.1f",atof(inputBase["width"].c_str()) + atof(inputBase["edgeWidth1"].c_str()) + atof(inputBase["edgeWidth2"].c_str()));
	//inputBase["ProductWidth"] = HGTools::Convert2StdString(inputValue);
	inputBase.m_fProductLength = inputBase.m_fLength + inputBase.m_afBanding[BANDING_ID_RIGHT] + inputBase.m_afBanding[BANDING_ID_LEFT];

	////成品宽度
	//inputValue.Format("%.1f",atof(inputBase["height"].c_str()) + atof(inputBase["edgeLen1"].c_str()) + atof(inputBase["edgeLen2"].c_str()));
	//inputBase["ProductHeight"] = HGTools::Convert2StdString(inputValue);
	inputBase.m_fProductWidth = inputBase.m_fWidth + inputBase.m_afBanding[BANDING_ID_FRONT] + inputBase.m_afBanding[BANDING_ID_BACK];

	////成品厚度
	//inputBase["ProductThin"] = inputBase["thin"];
	inputBase.m_fProductThickness = inputBase.m_fThickness;

	if(bNeedToResetOtherShapePoint || inputBase.m_strAbnormalInfo.IsEmpty())
	{
		inputBase.m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(
		OtherShapeHelper::getPointInfoRectangle(
		string("0"), \
		string(GetFloatString(inputBase.m_fLength, 1)), \
		string(GetFloatString(inputBase.m_fWidth, 1))) \
		).c_str();
	}
#endif

	return inputBase;
}

bool MatrialItemEditDlg::isIndexAvalible()
{
	if (m_currentIndex >= 0 
		&& m_currentIndex < m_vComponentInputItem.size() 
		&& m_vComponentInputItem.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool MatrialItemEditDlg::isCurrDataOK()
{
	return true;
}
void MatrialItemEditDlg::IndexPrev()
{
	m_currentIndex--;
	if (!isIndexAvalible())
	{
		m_currentIndex++;
	}
}
void MatrialItemEditDlg::IndexNext()
{
	m_currentIndex++;
	if (!isIndexAvalible())
	{
		m_currentIndex--;
	}
}
void MatrialItemEditDlg::InsertNewItem()
{
	m_currentIndex = m_vComponentInputItem.size();
	
	//校验条码重复
	if (m_check_barcode)
	{
		//std::map<std::string,std::string> mapItem = toMap();
		ComponentInputItem inputItem = toInputItem();
		inputItem.m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;
		//mapItem["info"] = HGTools::Convert2StdString(uniqueBarCode(HGTools::Convert2CString(mapItem["info"]),m_barCodeCheck));
		m_edit_barcode = inputItem.m_strBarcode = uniqueBarCode(inputItem.m_strBarcode, m_barCodeCheck);
		UpdateData(FALSE);

		
		//mapItem["OtherPoints"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(
		//	OtherShapeHelper::getPointInfoRectangle(
		//	mapItem["rotate"],
		//	mapItem["width"],
		//	mapItem["height"])
		//	);
		std::string strLength = GetFloatString(inputItem.m_fLength, 1);
		std::string strWidth = GetFloatString(inputItem.m_fWidth, 1);
		inputItem.m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(
		OtherShapeHelper::getPointInfoRectangle(
		/*mapItem["rotate"]*/"0",
		strLength,
		strWidth)
		).c_str();

		//m_matrialList.push_back(mapItem);
		m_vComponentInputItem.push_back(inputItem);
	}
	else
	{
		//std::map<std::string,std::string> mapItem = toMap();
		ComponentInputItem inputItem = toInputItem();
		inputItem.m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

		//mapItem["OtherPoints"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(
		//	OtherShapeHelper::getPointInfoRectangle(
		//	mapItem["rotate"],
		//	mapItem["width"],
		//	mapItem["height"])
		//	);
		std::string strLength = GetFloatString(inputItem.m_fLength, 1);
		std::string strWidth = GetFloatString(inputItem.m_fWidth, 1);
		inputItem.m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(
			OtherShapeHelper::getPointInfoRectangle(
			/*mapItem["rotate"]*/"0",
			strLength,
			strWidth)
			).c_str();

		//m_matrialList.push_back(mapItem);
		m_vComponentInputItem.push_back(inputItem);

		//m_barCodeCheck.push_back(HGTools::Convert2CString(mapItem["info"]));
		m_barCodeCheck.push_back(inputItem.m_strBarcode);
	}
	
	m_totalLine = m_vComponentInputItem.size()-1;
}
void MatrialItemEditDlg::LoadCurrentItem()
{
	//fromMap(m_matrialList[m_currentIndex]);
	fromInputItem(m_vComponentInputItem[m_currentIndex]);
}
void MatrialItemEditDlg::SaveCurrentItem()
{
	//m_matrialList[m_currentIndex] = toMap(m_matrialList[m_currentIndex]);
	m_vComponentInputItem[m_currentIndex] = toInputItem(m_vComponentInputItem[m_currentIndex]);
}
void MatrialItemEditDlg::DelCurrentItem()
{
	/*auto pIndex = m_matrialList.begin();*/
	int i = 0 ;
	//for (auto pIndex = m_matrialList.begin();  pIndex != m_matrialList.end() ; pIndex++ , i++)
	for (auto pIndex = m_vComponentInputItem.begin();  pIndex != m_vComponentInputItem.end() ; pIndex++ , i++)
	{
		if (i == m_currentIndex)
		{
			//m_matrialList.erase(pIndex);
			m_vComponentInputItem.erase(pIndex);
			//if (m_currentIndex >= m_matrialList.size())
			//{
			//	m_currentIndex = m_matrialList.size() - 1;
			//}
			if (m_currentIndex >= m_vComponentInputItem.size())
			{
				m_currentIndex = m_vComponentInputItem.size() - 1;
			}
			break;
		}
	}
}


//生成新的条码
CString MatrialItemEditDlg::newBarCode(int seed)
{
	CString newCode;
	CTime timersetting = CTime::GetCurrentTime();
	newCode.Format("%04d%02d%02d%02d%02d%02d%03d"
		,timersetting.GetYear()
		,timersetting.GetMonth()
		,timersetting.GetDay()
		,timersetting.GetHour()
		,timersetting.GetMinute()
		,timersetting.GetSecond()
		,seed);
	return newCode;
}
//获得唯一条码
CString MatrialItemEditDlg::uniqueBarCode(CString originBarCode,std::vector<CString>& barCodeCheck)
{
	int size = barCodeCheck.size();
	if (StrCmpC(originBarCode,"") == 0)
	{
		CString newCode = newBarCode(size);
		barCodeCheck.push_back(newCode);
		return newCode;
	}
	for (int i = 0 ; i < size ; i++)
	{
		CString cmp = barCodeCheck.at(i);
		if (StrCmpC(originBarCode,cmp) == 0)
		{
			CString newCode = newBarCode(size);
			barCodeCheck.push_back(newCode);
			return newCode;
		}
	}
	barCodeCheck.push_back(originBarCode);
	return originBarCode;
}



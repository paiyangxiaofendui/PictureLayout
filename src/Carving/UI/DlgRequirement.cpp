// DlgRequirement.cpp : 实现文件
//

#include "stdafx.h"
#include "../Carving.h"
#include "DlgRequirement.h"
#include "afxdialogex.h"
#include "../Misc/Misc.h"
#include "../Misc/HGTools.h"
#include "OtherShape.h"
#include "SlottingDlg.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "DirSetDlg.h"
#include "MatrialItemeEdit_MatrialDlg.h"
#include "MatrialItemEditDlg.h"
#include "../../../include/AbnormalShapeManager/AbTools.h"
#include "VecSoltDlg.h"
#include "../DataProcess/DataProcess.h"
#include "../Work/WorkDef.h"
#include "DlgResetMachiningInfo.h"
#include "DlgRequirement_PreCombine.h"
#include "DlgSetReminder.h"
#include "../Misc/ProgramMisc.h"
#include <map>

//#define PRE_COMBINE_DLG_HEIGHT 200
#define PRE_COMBINE_LISTBOX_WIDTH 200

struct SortParam
{
	int m_nSortOrderType;
	int m_nSortColumnID;
};

// CDlgRequirement 对话框

IMPLEMENT_DYNAMIC(CDlgRequirement, CDialogChildBase)

CDlgRequirement::CDlgRequirement(CWnd* pParent /*=NULL*/)
	: CDialogChildBase(CDlgRequirement::IDD, pParent)
	, m_pvComponentInputItem(NULL)
	, m_eSortOrderType(0)
	, m_nSortintSubItemID(0)
	, m_pImgSortAscending(NULL)
	, m_pImgSortDescending(NULL)
{

}

CDlgRequirement::~CDlgRequirement()
{
	if(m_pImgSortAscending)
		delete m_pImgSortAscending;
	if(m_pImgSortDescending)
		delete m_pImgSortDescending;
}

void CDlgRequirement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PANEL, m_lcRequirement);
	DDX_Control(pDX, IDC_LIST_PRE_COMBINE, m_lbPreCombine);
}


BEGIN_MESSAGE_MAP(CDlgRequirement, CDialogChildBase)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PANEL, &CDlgRequirement::OnNMRClickRequirmentList)			//鼠标右键点击

	//鼠标右键菜单
	ON_COMMAND(IDM_DIFFERENTPANEL, &CDlgRequirement::OnOtherShapeSetting)	//异形设置
	ON_COMMAND(IDM_DELTYPE, &CDlgRequirement::OnDelOtherShape)				//清除异型设置
	ON_COMMAND(IDM_SLOT, &CDlgRequirement::OnSlot)							//开槽设置
	ON_COMMAND(IDM_DELETESLOT, &CDlgRequirement::OnDeleteslot)				//清除开槽设置
	ON_COMMAND(IDM_SETDIR, &CDlgRequirement::OnSetdir)						//设定纹理
	ON_COMMAND(IDM_SETMATRIAL, &CDlgRequirement::OnSetMatrial)				//设定材质
	ON_COMMAND(IDM_SET_REMINDER, &CDlgRequirement::OnSetReminder)
	ON_COMMAND(IDM_CHANG_MATRAIL_ITEM, &CDlgRequirement::OnEditMatrialList)	//设置需切成品
	ON_COMMAND(IDM_DELETEITEM, &CDlgRequirement::OnDelMatrialList)			//删除板件
	ON_COMMAND(ID_SELF_OTHERSHAP, &CDlgRequirement::OnSelfOthershap)		//自定义异形
	ON_COMMAND(ID_MENU_RESET_HOLE_INFO, &CDlgRequirement::OnResetHoleInfo)
	ON_COMMAND(ID_MENU_PRE_COMBINE, &CDlgRequirement::OnPreCombine)
	ON_COMMAND(ID_MENU_EXPORT_COMPONENT_NC, &CDlgRequirement::OnExportComponentNC)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PANEL, &CDlgRequirement::OnNMDblclkListRequirement)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PANEL, &CDlgRequirement::OnLvnColumnclickListRequirement)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_PANEL, &CDlgRequirement::OnLvnDeleteitemListRequirement)
	ON_MESSAGE(WM_SHOW_OR_HIDE_PRECOMBINE, &CDlgRequirement::OnShowOrHidePreCombine)
END_MESSAGE_MAP()


// CDlgRequirement 消息处理程序


void CDlgRequirement::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	ArrangeUI();
}


BOOL CDlgRequirement::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lcRequirement.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/);
	//m_lcRequirement.InsertColumn(0, _T("条码"), 0, 160);
	//m_lcRequirement.InsertColumn(1, _T("柜体名称"), 0, 160);
	//m_lcRequirement.InsertColumn(2, _T("板件名称"), 0, 160);
	//m_lcRequirement.InsertColumn(3, _T("成品长"), 0, 160);
	//m_lcRequirement.InsertColumn(4, _T("成品宽"), 0, 160);
	//m_lcRequirement.InsertColumn(5, _T("板件厚"), 0, 160);
	//m_lcRequirement.InsertColumn(6, _T("材质"), 0, 160);
	//m_lcRequirement.InsertColumn(7, _T("纹理"), 0, 160);
	//m_lcRequirement.InsertColumn(8, _T("需切数量"), 0, 160);

	m_lcRequirement.InsertColumn(0,"订单号",0,120);
	m_lcRequirement.InsertColumn(1,"柜号",0,40);
	m_lcRequirement.InsertColumn(2,"条码编号",0,140);
	m_lcRequirement.InsertColumn(3,"产品名称",0,120);
	m_lcRequirement.InsertColumn(4,"成品名称",0,120);
	m_lcRequirement.InsertColumn(5,"材质名称",0,120);
	m_lcRequirement.InsertColumn(6,"开料长度",0,80);
	m_lcRequirement.InsertColumn(7,"开料宽度",0,80);
	m_lcRequirement.InsertColumn(8,"开料厚度",0,80);
	m_lcRequirement.InsertColumn(9,"需切数量",0,80);
	m_lcRequirement.InsertColumn(10,"纹路方向",0,80);
	m_lcRequirement.InsertColumn(11,"封长1",0,80);
	m_lcRequirement.InsertColumn(12,"封宽1",0,80);
	m_lcRequirement.InsertColumn(13,"封长2",0,80);
	m_lcRequirement.InsertColumn(14,"封宽2",0,80);
	m_lcRequirement.InsertColumn(15,"异型",0,80);
	m_lcRequirement.InsertColumn(16,"旋转",0,80);
	m_lcRequirement.InsertColumn(17,"正面槽",0,80);
	m_lcRequirement.InsertColumn(18,"槽翻转",0,80);
	m_lcRequirement.InsertColumn(19,"反面槽",0,80);
	m_lcRequirement.InsertColumn(20,"正面孔",0,80);
	m_lcRequirement.InsertColumn(21,"孔翻转",0,80);
	m_lcRequirement.InsertColumn(22,"反面孔",0,80);
	m_lcRequirement.InsertColumn(23,"客户信息",0,80);
	m_lcRequirement.InsertColumn(24,"加盟店",0,80);
	m_lcRequirement.InsertColumn(25,"拉槽标识",0,80);
	m_lcRequirement.InsertColumn(26,"助记号",0,80);
	m_lcRequirement.InsertColumn(27,"钻孔",0,80);
	m_lcRequirement.InsertColumn(28,"订单类型",0,80);
	m_lcRequirement.InsertColumn(29,"反面条码",0,80);
	m_lcRequirement.InsertColumn(30,"成品长度",0,80);
	m_lcRequirement.InsertColumn(31,"成品宽度",0,80);
	m_lcRequirement.InsertColumn(32,"成品厚度",0,80);
	m_lcRequirement.InsertColumn(33,"异型ID",0,80);
	m_lcRequirement.InsertColumn(34,"客户地址",0,120);
	m_lcRequirement.InsertColumn(35,"钻槽标识",0,120);
	//m_lcRequirement.InsertColumn(36,"异形点信息",0, paramDlg.OutlineColomnShowable());

	HWND hwnd = ::GetDlgItem(m_lcRequirement.m_hWnd, 0); 
	if(hwnd) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcRequirement.SubclassWindow(hwnd); 
			} 
		} 
		else
		{
			m_hcRequirement.SubclassWindow(hwnd);
		}
	}

	LONG dwStyle = GetWindowLong(m_lcRequirement.m_hWnd, GWL_STYLE);
	SetWindowLong(m_lcRequirement.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_lcRequirement.SetExtendedStyle((m_lcRequirement.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	m_pImgSortAscending = LoadPngImgFromRes(IDB_PNG_SORT_ASCENDING);
	m_pImgSortDescending = LoadPngImgFromRes(IDB_PNG_SORT_DESCENDING);
	//m_lcRequirement.SetSortingImg(m_pImgSortAscending, m_pImgSortDescending);
	((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetSortingImg(m_pImgSortAscending, m_pImgSortDescending);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgRequirement::SetPanelInfo(vector<ComponentInputItem>& vComponentInputItem)
{
	m_pvComponentInputItem = &vComponentInputItem;

	RefreshListCtrl();
}

//鼠标右键弹出菜单
void CDlgRequirement::OnNMRClickRequirmentList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = m_lcRequirement.GetNextItem(-1,LVIS_SELECTED);

	if(nItem >= 0)
	{
		CMenu menu, *pPopup;  
		menu.LoadMenu(IDR_MENU_REQUIREMENT_LIST);  
		pPopup = menu.GetSubMenu(0); 

		bool bEnablePreCombine = true;
		if(m_lcRequirement.GetSelectedCount() <= 1)
			bEnablePreCombine = false;
		else
		{
			bool bAllTheSameThicknessAndMaterial = true;
			vector<ComponentInputItem*> vSelectedItem;
			POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
			while (pos)
			{
				int nItem = m_lcRequirement.GetNextSelectedItem(pos);

				ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
				if(pComponentInputItem)
				{
					vSelectedItem.push_back(pComponentInputItem);
				}
				else
				{
					bAllTheSameThicknessAndMaterial = false;
					break;
				}
			}
			for(int i = 1; i < vSelectedItem.size(); i++)
			{
				if( !IsFloatEqual(vSelectedItem[i]->m_fThickness, vSelectedItem[0]->m_fThickness) || !(vSelectedItem[i]->m_strMaterial.Compare(vSelectedItem[0]->m_strMaterial) == 0))
				{
					bAllTheSameThicknessAndMaterial = false;
					break;
				}
			}
			if(!bAllTheSameThicknessAndMaterial)
				bEnablePreCombine = false;
		}
		if(!bEnablePreCombine)
			pPopup->EnableMenuItem(ID_MENU_PRE_COMBINE, MF_BYCOMMAND | MF_DISABLED);

		CPoint myPoint;  
		ClientToScreen(&myPoint);  
		GetCursorPos(&myPoint); 
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this); 
	}

	*pResult = 1;
}

//异形设置
void CDlgRequirement::OnOtherShapeSetting()
{
	int nItem = m_lcRequirement.GetNextItem(-1,LVIS_SELECTED);

	std::map<std::string,std::string> noProcessSmallPanelOne = GetMatrialItemsOne(nItem);

	COtherShape dlg;
	dlg.Item(noProcessSmallPanelOne);

	if (dlg.DoModal() == IDOK)
	{
		noProcessSmallPanelOne["pointInfo"] = dlg.GetOtherShapeString();	//设置异形字符串
		noProcessSmallPanelOne["OtherPoints"] = dlg.GetPointInfosString();	//设置异形outline
		noProcessSmallPanelOne["rotate"] = dlg.ChangetypeStr();				//旋转
		SetMatrialItemsOne(nItem,noProcessSmallPanelOne);
	}
}

//从界面材料获得1个
std::map<std::string,std::string> CDlgRequirement::GetMatrialItemsOne( int index )
{
	std::map<std::string,std::string> noProcessSmallPanelOne;

	noProcessSmallPanelOne["order"]			 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,0));	//订单号
	noProcessSmallPanelOne["cabinet"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,1));	//柜号
	noProcessSmallPanelOne["info"]			 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,2));	//条码
	noProcessSmallPanelOne["PorductName"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,3));	//产品名称
	noProcessSmallPanelOne["porduct"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,4));	//成品名称
	noProcessSmallPanelOne["matrial"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,5));	//材质
	noProcessSmallPanelOne["width"]			 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,6));	//长
	noProcessSmallPanelOne["height"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,7));	//宽
	noProcessSmallPanelOne["thin"]			 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,8));	//厚度
	//noProcessSmallPanelOne["num"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,9));	//切割数量
	noProcessSmallPanelOne["dir"]			 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,10));	//纹理方向
	noProcessSmallPanelOne["edgeLen1"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,11));	//封长1
	noProcessSmallPanelOne["edgeWidth1"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,12));	//封宽1
	noProcessSmallPanelOne["edgeLen2"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,13));	//封长2
	noProcessSmallPanelOne["edgeWidth2"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,14));	//封宽2
	noProcessSmallPanelOne["pointInfo"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,15));	//异形信息
	noProcessSmallPanelOne["rotate"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,16));	//是否旋转
	noProcessSmallPanelOne["slotInfo"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,17));	//槽
	noProcessSmallPanelOne["slotRotate"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,18));	//槽翻转
	noProcessSmallPanelOne["dslotInfo"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,19));	//反面槽
	noProcessSmallPanelOne["vecInfo"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,20));	//孔
	noProcessSmallPanelOne["vecRotate"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,21));	//孔翻转
	noProcessSmallPanelOne["dvecInfo"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,22));	//反面孔
	noProcessSmallPanelOne["customeInfo"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,23));	//客户信息
	noProcessSmallPanelOne["store"]			 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,24));	//加盟店
	noProcessSmallPanelOne["SawMark"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,25));	//拉槽标识
	noProcessSmallPanelOne["HelpSign"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,26));	//助记号
	noProcessSmallPanelOne["Vertical"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,27));	//钻孔
	noProcessSmallPanelOne["OrderType"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,28));	//订单类型
	noProcessSmallPanelOne["BackCode"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,29));	//反面条码
	noProcessSmallPanelOne["ProductWidth"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,30));	//成品长度
	noProcessSmallPanelOne["ProductHeight"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,31));	//成品宽度
	noProcessSmallPanelOne["ProductThin"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,32));	//成品厚度
	noProcessSmallPanelOne["otherID"]		 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,33));	//异型ID
	noProcessSmallPanelOne["customeAddr"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,34));	//客户地址
	noProcessSmallPanelOne["DrillingLogo"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,35));	//钻槽标识
	noProcessSmallPanelOne["OtherPoints"]	 = HGTools::Convert2StdString(m_lcRequirement.GetItemText(index,36));	//异形点信息

	return (noProcessSmallPanelOne);
}

//设置界面材料1个
void CDlgRequirement::SetMatrialItemsOne( int i,std::map<std::string,std::string> noProcessSmallPanelOne )
{

	//m_matriallist.SetItemText(i,0	,noProcessSmallPanelOne["order"]			.c_str());//订单号
	//m_matriallist.SetItemText(i,1	,noProcessSmallPanelOne["cabinet"]			.c_str());//柜号
	//m_matriallist.SetItemText(i,2	,noProcessSmallPanelOne["info"]				.c_str());//条码
	//m_matriallist.SetItemText(i,3	,noProcessSmallPanelOne["PorductName"]		.c_str());//产品名称
	//m_matriallist.SetItemText(i,4	,noProcessSmallPanelOne["porduct"]			.c_str());//成品名称
	//m_matriallist.SetItemText(i,5	,noProcessSmallPanelOne["matrial"]			.c_str());//材质
	//m_matriallist.SetItemText(i,6	,noProcessSmallPanelOne["width"]			.c_str());//长
	//m_matriallist.SetItemText(i,7	,noProcessSmallPanelOne["height"]			.c_str());//宽
	//m_matriallist.SetItemText(i,8	,noProcessSmallPanelOne["thin"]				.c_str());//厚度
	//m_matriallist.SetItemText(i,9	,noProcessSmallPanelOne["num"]				.c_str());//切割数量
	//m_matriallist.SetItemText(i,10	,noProcessSmallPanelOne["dir"]				.c_str());//纹理方向
	//m_matriallist.SetItemText(i,11	,noProcessSmallPanelOne["edgeLen1"]			.c_str());//封长1
	//m_matriallist.SetItemText(i,12	,noProcessSmallPanelOne["edgeWidth1"]		.c_str());//封宽1
	//m_matriallist.SetItemText(i,13	,noProcessSmallPanelOne["edgeLen2"]			.c_str());//封长2
	//m_matriallist.SetItemText(i,14	,noProcessSmallPanelOne["edgeWidth2"]		.c_str());//封宽2
	//m_matriallist.SetItemText(i,15	,noProcessSmallPanelOne["pointInfo"]		.c_str());//异形信息
	//m_matriallist.SetItemText(i,16	,noProcessSmallPanelOne["rotate"]			.c_str());//是否旋转
	//m_matriallist.SetItemText(i,17	,noProcessSmallPanelOne["slotInfo"]			.c_str());//槽
	//m_matriallist.SetItemText(i,18	,noProcessSmallPanelOne["slotRotate"]		.c_str());//槽翻转
	//m_matriallist.SetItemText(i,19	,noProcessSmallPanelOne["dslotInfo"]		.c_str());//反面槽
	//m_matriallist.SetItemText(i,20	,noProcessSmallPanelOne["vecInfo"]			.c_str());//孔
	//m_matriallist.SetItemText(i,21	,noProcessSmallPanelOne["vecRotate"]		.c_str());//反面孔
	//m_matriallist.SetItemText(i,22	,noProcessSmallPanelOne["dvecInfo"]			.c_str());//反面孔
	//m_matriallist.SetItemText(i,23	,noProcessSmallPanelOne["customeInfo"]		.c_str());//客户信息
	//m_matriallist.SetItemText(i,24	,noProcessSmallPanelOne["store"]			.c_str());//加盟店
	//m_matriallist.SetItemText(i,25	,noProcessSmallPanelOne["SawMark"]			.c_str());//拉槽标识
	//m_matriallist.SetItemText(i,26	,noProcessSmallPanelOne["HelpSign"]			.c_str());//助记号
	//m_matriallist.SetItemText(i,27	,noProcessSmallPanelOne["Vertical"]			.c_str());//钻孔
	//m_matriallist.SetItemText(i,28	,noProcessSmallPanelOne["OrderType"]		.c_str());//订单类型
	//m_matriallist.SetItemText(i,29	,noProcessSmallPanelOne["BackCode"]			.c_str());//反面条码
	//m_matriallist.SetItemText(i,30	,noProcessSmallPanelOne["ProductWidth"]		.c_str());//成品长度
	//m_matriallist.SetItemText(i,31	,noProcessSmallPanelOne["ProductHeight"]	.c_str());//成品宽度
	//m_matriallist.SetItemText(i,32	,noProcessSmallPanelOne["ProductThin"]		.c_str());//成品厚度
	//m_matriallist.SetItemText(i,33	,noProcessSmallPanelOne["otherID"]			.c_str());//异型ID
	//m_matriallist.SetItemText(i,34	,noProcessSmallPanelOne["customeAddr"]		.c_str());//客户地址
	//m_matriallist.SetItemText(i,35	,noProcessSmallPanelOne["DrillingLogo"]		.c_str());//钻槽标识
	//m_matriallist.SetItemText(i,36	,noProcessSmallPanelOne["OtherPoints"]		.c_str());//异形点信息

	ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(i);
	if(pComponentInputItem)
	{
		ComponentInputItem& theComponentInputItem = *pComponentInputItem;
		theComponentInputItem.m_strOtherShapeIDAndParam = noProcessSmallPanelOne["pointInfo"].c_str(); //异形参数
		theComponentInputItem.m_strAbnormalInfo = noProcessSmallPanelOne["OtherPoints"].c_str();	   // 异形点信息
		theComponentInputItem.m_eRotateType = atoi(noProcessSmallPanelOne["rotate"].c_str());          //旋转
		CString strOtherShapeIDValue = OtherShapeHelper::GetOtherShapeId(noProcessSmallPanelOne["pointInfo"]).c_str();
		if(strOtherShapeIDValue.IsEmpty())
			strOtherShapeIDValue = _T("0");
		theComponentInputItem.m_strOtherShapeID.Format(_T("异%s"), strOtherShapeIDValue);  // 异形ID  

		SetOneItemTextAndData(i, theComponentInputItem);
	}
}

bool CDlgRequirement::SetOneItemTextAndData(int nIndex, ComponentInputItem& theComponentInputItem)
{
	if(nIndex < 0 || nIndex >= m_lcRequirement.GetItemCount())
		return false;

	m_lcRequirement.SetItemText(nIndex, 0, theComponentInputItem.m_strOrderID);
	m_lcRequirement.SetItemText(nIndex, 1, theComponentInputItem.m_strCabinetID);
	m_lcRequirement.SetItemText(nIndex, 2, theComponentInputItem.m_strBarcode);
	m_lcRequirement.SetItemText(nIndex, 3, theComponentInputItem.m_strCabinetName);
	m_lcRequirement.SetItemText(nIndex, 4, theComponentInputItem.m_strPanelName);
	m_lcRequirement.SetItemText(nIndex, 5, theComponentInputItem.m_strMaterial);
	m_lcRequirement.SetItemText(nIndex, 6, GetFloatStringTrimming(theComponentInputItem.m_fLength, 1));
	m_lcRequirement.SetItemText(nIndex, 7, GetFloatStringTrimming(theComponentInputItem.m_fWidth, 1));
	m_lcRequirement.SetItemText(nIndex, 8, GetFloatStringTrimming(theComponentInputItem.m_fThickness, 1));
	m_lcRequirement.SetItemText(nIndex, 9, GetFloatString(theComponentInputItem.m_nCount, 0));
	m_lcRequirement.SetItemText(nIndex, 10, theComponentInputItem.m_strTexture);
	m_lcRequirement.SetItemText(nIndex, 11, GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_FRONT], 1));
	m_lcRequirement.SetItemText(nIndex, 12, GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_RIGHT], 1));
	m_lcRequirement.SetItemText(nIndex, 13, GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_BACK], 1));
	m_lcRequirement.SetItemText(nIndex, 14, GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_LEFT], 1));
	m_lcRequirement.SetItemText(nIndex, 15, theComponentInputItem.m_strOtherShapeIDAndParam);
	CString strTmp;
	strTmp.Format(_T("%d"), theComponentInputItem.m_eRotateType);
	m_lcRequirement.SetItemText(nIndex, 16, strTmp);
	m_lcRequirement.SetItemText(nIndex, 17, theComponentInputItem.m_strUpperFaceSlotInfo);
	m_lcRequirement.SetItemText(nIndex, 18, theComponentInputItem.m_bSlotFlipped ? _T("是") :  _T(""));
	m_lcRequirement.SetItemText(nIndex, 19, theComponentInputItem.m_strDownerFaceSlotInfo);
	m_lcRequirement.SetItemText(nIndex, 20, theComponentInputItem.m_strUpperFaceHoleInfo);
	m_lcRequirement.SetItemText(nIndex, 21, theComponentInputItem.m_bVHoleFlipped ? _T("是") :  _T(""));
	m_lcRequirement.SetItemText(nIndex, 22, theComponentInputItem.m_strDownerFaceHoleInfo);
	m_lcRequirement.SetItemText(nIndex, 23, theComponentInputItem.m_strCustomerInfo);
	m_lcRequirement.SetItemText(nIndex, 24, theComponentInputItem.m_strJoinedStore);
	m_lcRequirement.SetItemText(nIndex, 25, theComponentInputItem.m_strSlottingFlag);
	m_lcRequirement.SetItemText(nIndex, 26, theComponentInputItem.m_strReminder);
	m_lcRequirement.SetItemText(nIndex, 27, theComponentInputItem.m_strDrilling);
	m_lcRequirement.SetItemText(nIndex, 28, theComponentInputItem.m_strOrderType);
	m_lcRequirement.SetItemText(nIndex, 29, theComponentInputItem.m_strReverseSideBarcode);
	m_lcRequirement.SetItemText(nIndex, 30, GetFloatStringTrimming(theComponentInputItem.m_fProductLength, 1));
	m_lcRequirement.SetItemText(nIndex, 31, GetFloatStringTrimming(theComponentInputItem.m_fProductWidth, 1));
	m_lcRequirement.SetItemText(nIndex, 32, GetFloatStringTrimming(theComponentInputItem.m_fProductThickness, 1));
	m_lcRequirement.SetItemText(nIndex, 33, theComponentInputItem.m_strOtherShapeID);
	m_lcRequirement.SetItemText(nIndex, 34, theComponentInputItem.m_strCustomerAddress);
	m_lcRequirement.SetItemText(nIndex, 35, theComponentInputItem.m_strHoleSlotFlag);
	//m_lcRequirement.SetItemText(nIndex, 36, theComponentInputItem.m_strAbnormalInfo);

	RequirementItemData* pData = (RequirementItemData*)m_lcRequirement.GetItemData(nIndex);
	pData->m_uSerialID = theComponentInputItem.m_uSerialID;

	RefreshAllItemState();

	//bool bIsVHoleDeeperThanThickness = IsVHoleDeeperThanThickness(theComponentInputItem);
	//bool bIsSlotDeeperThanThickness = IsSlotDeeperThanThickness(theComponentInputItem);
	//bool bIsComponentOverSize = IsComponentOverSize(theComponentInputItem);
	//bool bItemStateWarning = (bIsVHoleDeeperThanThickness || bIsSlotDeeperThanThickness || bIsComponentOverSize);

	//pData->m_eListItemState = bItemStateWarning ? ListItemState::LIST_ITEM_STATE_WARNING : ListItemState::LIST_ITEM_STATE_NORMAL;
	//CString strToolTip;
	//if(bIsVHoleDeeperThanThickness)
	//	strToolTip.Format(_T("孔过深"));
	//if(bIsSlotDeeperThanThickness)
	//{
	//	if(!strToolTip.IsEmpty())
	//		strToolTip += _T("，");
	//	strToolTip += _T("槽过深");
	//}
	//if(bIsComponentOverSize)
	//{
	//	if(!strToolTip.IsEmpty())
	//		strToolTip += _T("，");
	//	strToolTip += _T("板件尺寸过大");
	//}
	//pData->m_strToolTip = strToolTip;

	//bool bNoWarning = true;
	//for(int i = 0; i < m_pvComponentInputItem->size(); i++)
	//{
	//	if(IsVHoleDeeperThanThickness((*m_pvComponentInputItem)[i]) || IsSlotDeeperThanThickness((*m_pvComponentInputItem)[i]) || IsComponentOverSize((*m_pvComponentInputItem)[i]))
	//	{
	//		bNoWarning = false;
	//	}
	//}
	//if(bNoWarning)
	//	((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetState(CSkinHeaderCtrl::State::STATE_NORMAL);
	//else
	//	((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetState(CSkinHeaderCtrl::State::STATE_WARNING);

	return true;
}

void CDlgRequirement::RefreshAllItemState()
{
	vector<vector<int>> vDuplicatedBarcodeItem;
	CheckDuplicatedBarcode(vDuplicatedBarcodeItem);
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	{
		ComponentInputItem& theComponentInputItem = *(GetComponentInputItemByID(i));
		RequirementItemData* pData = (RequirementItemData*)m_lcRequirement.GetItemData(i);

		bool bIsVHoleDeeperThanThickness = IsVHoleDeeperThanThickness(theComponentInputItem);
		bool bIsSlotDeeperThanThickness = IsSlotDeeperThanThickness(theComponentInputItem);
		bool bIsComponentOverSize = IsComponentOverSize(theComponentInputItem);
		bool bIsDuplicatedBarcode = false;
		bool bIsVHoleOutOfPanel = IsVHoleOutOfPanel(theComponentInputItem);
		for(int j = 0; j < vDuplicatedBarcodeItem.size(); j++)
		{
			for(int k = 0; k < vDuplicatedBarcodeItem[j].size(); k++)
			{
				if(vDuplicatedBarcodeItem[j][k] == i)
				{
					bIsDuplicatedBarcode = true;
					break;
				}
			}
			if(bIsDuplicatedBarcode)
				break;
		}

		bool bItemStateWarning = (bIsVHoleDeeperThanThickness || bIsSlotDeeperThanThickness || bIsComponentOverSize || bIsDuplicatedBarcode || bIsVHoleOutOfPanel);

		pData->m_eListItemState = bItemStateWarning ? ListItemState::LIST_ITEM_STATE_WARNING : ListItemState::LIST_ITEM_STATE_NORMAL;
		CString strToolTip;
		if(bIsVHoleDeeperThanThickness)
			strToolTip.Format(_T("孔过深"));
		if(bIsSlotDeeperThanThickness)
		{
			if(!strToolTip.IsEmpty())
				strToolTip += _T("，");
			strToolTip += _T("槽过深");
		}
		if(bIsComponentOverSize)
		{
			if(!strToolTip.IsEmpty())
				strToolTip += _T("，");
			strToolTip += _T("板件尺寸过大");
		}
		if(bIsDuplicatedBarcode)
		{
			if(!strToolTip.IsEmpty())
				strToolTip += _T("，");
			strToolTip += _T("条码编号有重复");
		}
		if(bIsVHoleOutOfPanel)
		{
			if(!strToolTip.IsEmpty())
				strToolTip += _T("，");
			strToolTip += _T("孔位超出板件");
		}
		pData->m_strToolTip = strToolTip;
	}


	bool bNoWarning = true;
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	{
		RequirementItemData* pData = (RequirementItemData*)m_lcRequirement.GetItemData(i);
		if(pData->m_eListItemState == ListItemState::LIST_ITEM_STATE_WARNING)
		{
			bNoWarning = false;
			break;
		}
	}
	if(bNoWarning)
		((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetState(CSkinHeaderCtrl::State::STATE_NORMAL);
	else
		((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetState(CSkinHeaderCtrl::State::STATE_WARNING);
}

void CDlgRequirement::CheckDuplicatedBarcode(vector<vector<int>>& vDuplicatedBarcodeItem)
{
	vDuplicatedBarcodeItem.clear();

	map<std::string, vector<int>> mapBarcodeToItems;
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	{
		std::string stringBarcode = m_lcRequirement.GetItemText(i, LIST_COLUMN_BARCODE).GetBuffer();
		mapBarcodeToItems[stringBarcode].push_back(i);
	}

	for(map<std::string, vector<int>>::iterator itr = mapBarcodeToItems.begin(); itr != mapBarcodeToItems.end(); itr++)
	{
		if(itr->second.size() > 1)
		{
			vDuplicatedBarcodeItem.push_back(itr->second);
		}
	}
}

//清除异形
void CDlgRequirement::OnDelOtherShape()
{
	OnResetMachiningInfo(CDlgResetMachiningInfo::RESET_OTHERSHAPE);

	//if (MessageBox("是否确认删除异型信息","提示",MB_YESNO) == IDYES)
	//{
	//	POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
	//	if (pos == NULL)
	//	{
	//		return;
	//	}
	//	else
	//	{
	//		while (pos)
	//		{
	//			int nItem = m_lcRequirement.GetNextSelectedItem(pos);
	//			std::map<std::string,std::string> noProcessSmallPanelOne = GetMatrialItemsOne(nItem);
	//			noProcessSmallPanelOne["pointInfo"] = "";
	//			noProcessSmallPanelOne["rotate"] = "0";  /////////////////////////////////////////////////////////////////////////////
	//			std::vector<PointInfo> pointInfo = OtherShapeHelper::getPointInfoRectangle(
	//				noProcessSmallPanelOne["rotate"],    
	//				noProcessSmallPanelOne["width"],
	//				noProcessSmallPanelOne["height"]);
	//			noProcessSmallPanelOne["OtherPoints"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(pointInfo);
	//			
	//			SetMatrialItemsOne(nItem,noProcessSmallPanelOne);
	//		}
	//	}
	//}
}

//开槽设置
void CDlgRequirement::OnSlot()
{
	// TODO: Add your command handler code here
	int nItem = m_lcRequirement.GetNextItem(-1,LVIS_SELECTED);

	if(nItem < 0 || nItem >= m_lcRequirement.GetItemCount())
		return;

	ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
	if(pComponentInputItem == NULL)
		return;

	CSlottingDlg dlg;
	//if(m_matriallist.GetItemText(nItem,15) == "")
	//	dlg.m_strpoint = "ID:0;PW:" + m_matriallist.GetItemText(nItem,6) + ";" + "PD:" + m_matriallist.GetItemText(nItem,7) + ";";
	//else
	//	dlg.m_strpoint =  m_matriallist.GetItemText(nItem,15);
	dlg.m_strOutlinePointString = pComponentInputItem->m_strAbnormalInfo;

	std::string Slotx,Sloty,SlotWidth,SlotDepth;
	ReadSlotTemplate(Slotx,Sloty,SlotWidth,SlotDepth);
	dlg.m_editx = Slotx.c_str();
	dlg.m_edity = Sloty.c_str();
	dlg.m_edith = SlotWidth.c_str();
	dlg.m_editd = SlotDepth.c_str();
	dlg.m_fComponentXLen = pComponentInputItem->m_fLength;
	dlg.m_fComponentYLen = pComponentInputItem->m_fWidth;
	//if (m_matriallist.GetItemText(nItem,10)=="横纹")
	if(pComponentInputItem->m_strTexture.CompareNoCase(_T("竖纹")) == 0)
	{
		dlg.m_editdir = "1";
		dlg.m_editSlotLength = /*m_matriallist.GetItemText(nItem,7)*/GetFloatString(pComponentInputItem->m_fWidth, 1);
	}else
	{
		dlg.m_editdir = "0";
		dlg.m_editSlotLength = /*m_matriallist.GetItemText(nItem,6)*/GetFloatString(pComponentInputItem->m_fLength, 1);
	}
	dlg.m_strPreExistSlots = pComponentInputItem->m_strUpperFaceSlotInfo;
	if (dlg.DoModal() == IDOK)
	{
		//m_matriallist.SetItemText(nItem,17,dlg.m_result);
		pComponentInputItem->m_strUpperFaceSlotInfo = dlg.m_result;
		SetOneItemTextAndData(nItem, *pComponentInputItem);
	}

}

//获取开槽功能函数
void CDlgRequirement::ReadSlotTemplate(std::string &Slotx,std::string &Sloty,std::string &SlotWidth,std::string &SlotDepth)
{
	//hg3d::TString strConfigFile1 = _T("kdata");
	//strConfigFile1 = hg3d::getFullFileName(strConfigFile1);
	//strConfigFile1 += _T("\\SlotTemplate.xml");

	//hg3d::CXmlFile *doc = new hg3d::CXmlFile();
	CString strTmp;
	strTmp.Format(_T("kdata\\SlotTemplate%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
	TinyXml::TiXmlDocument doc(xmlPath);
	bool bLoadSuccess = doc.LoadFile();
	HGTools::deleteFile(xmlPath);

	if (!bLoadSuccess)
	{
		AfxMessageBox("读取SlotTemplate.xml失败",MB_OK);
		Slotx = "";
		Sloty = "";
		SlotWidth = "";
		SlotDepth = "";
		return;
	}

	XmlHandler xmlHandler;
	vector<TiXmlElement*> vSlotMessage = xmlHandler.GetChildElms(doc.RootElement());
	//for (int i = 0;i<doc->GetChildNodes().size();i++)
	for(int i = 0;i<vSlotMessage.size();i++)
	{
		//hg3d::CXmlNode* chil_node = doc->GetChildNodes().at(i);
		//std::string theName = chil_node->GetAttributeValue(_T("name"));
		//std::string theValue = chil_node->GetAttributeValue(_T("value"));
		TiXmlElement* elmSlotMessage = vSlotMessage[i];
		CString strTheName, strTheValue;
		xmlHandler.GetXmlAttribute(elmSlotMessage, "name", strTheName);
		xmlHandler.GetXmlAttribute(elmSlotMessage, "value", strTheValue);

		if (strTheName.CompareNoCase(_T("Slotx")) == 0)
		{
			Slotx = strTheValue;
		}
		if (strTheName.CompareNoCase(_T("Sloty")) == 0)
		{
			Sloty = strTheValue;
		}
		if (strTheName.CompareNoCase(_T("SlotWidth")) == 0)
		{
			SlotWidth = strTheValue;
		}
		if (strTheName.CompareNoCase(_T("SlotDepth")) == 0)
		{
			SlotDepth = strTheValue;
		}
	}
}

void CDlgRequirement::OnDeleteslot()
{
	// TODO: Add your command handler code here
	OnResetMachiningInfo(CDlgResetMachiningInfo::RESET_SLOT);
}

//设定纹理
void CDlgRequirement::OnSetdir()
{
	// TODO: Add your command handler code here
	CDirSetDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString _result = dlg.m_value;
		POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			return;
		}
		else
		{
			while (pos)
			{
				int nItem = m_lcRequirement.GetNextSelectedItem(pos);
				
				ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
				if(pComponentInputItem)
				{
					pComponentInputItem->m_strTexture = _result;
					SetOneItemTextAndData(nItem, *pComponentInputItem);
				}
			}
		}
	}


}


//设定材质
void CDlgRequirement::OnSetMatrial()
{
	MatrialItemeEdit_MatrialDlg editDlg;

	//循环加载所有材质
	POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_lcRequirement.GetNextSelectedItem(pos);
			ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
			if(pComponentInputItem)
			{
				editDlg.AppendMatrialName(pComponentInputItem->m_strMaterial.GetBuffer());
				pComponentInputItem->m_strMaterial.ReleaseBuffer();
			}
		}
	}

	//打开窗口
	if (editDlg.DoModal() != IDOK)
	{
		return;
	}

	//循环赋值所有材质
	POSITION pos2 = m_lcRequirement.GetFirstSelectedItemPosition();
	if (pos2 == NULL)
	{
		return;
	}
	else
	{
		while (pos2)
		{


			int nItem = m_lcRequirement.GetNextSelectedItem(pos2);
			ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
			if(pComponentInputItem)
			{
				pComponentInputItem->m_strMaterial = editDlg.GetMatrialName();
				SetOneItemTextAndData(nItem, *pComponentInputItem);
			}
		}
	}


}

void CDlgRequirement::OnSetReminder()
{
	CDlgSetReminder dlg;

	//循环加载所有材质
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	{
		ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(i);
		if(pComponentInputItem && !pComponentInputItem->m_strReminder.IsEmpty())
		{
			dlg.AppendReminder(pComponentInputItem->m_strReminder.GetBuffer());
			pComponentInputItem->m_strReminder.ReleaseBuffer();
		}
	}

	//打开窗口
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	//循环赋值所有材质
	POSITION pos2 = m_lcRequirement.GetFirstSelectedItemPosition();
	if (pos2 == NULL)
	{
		return;
	}
	else
	{
		while (pos2)
		{
			int nItem = m_lcRequirement.GetNextSelectedItem(pos2);
			ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
			if(pComponentInputItem)
			{
				pComponentInputItem->m_strReminder = dlg.GetReminder();
				SetOneItemTextAndData(nItem, *pComponentInputItem);
			}
		}
	}


}

//编辑需切成品
void CDlgRequirement::OnEditMatrialList()
{
	//设置数据进对话框
	MatrialItemEditDlg dlg;

	//获取全部变量进去
	vector<ComponentInputItem> vComponentInputItem;
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	{
		ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(i);
		if(pComponentInputItem)
			vComponentInputItem.push_back(*pComponentInputItem);
	}

	//获得全部选中的选项序号
	std::vector<int>& selectList = getSelectIndexFromMatrialList();


	//对话框操作：新建插入，更改某项，删除某项
	dlg.setSourceMatrial(/*(*m_pvComponentInputItem)*/vComponentInputItem);
	dlg.setSourceSelectIndex(selectList);

	//确认修改，关闭窗口
	if (dlg.DoModal() == IDOK)
	{
		//SetMatrialItems(dlg.getResultMatrial());
		(*m_pvComponentInputItem) = dlg.getResultMatrial();
		RefreshListCtrl();
	}
}

//获得选择序号
std::vector<int> CDlgRequirement::getSelectIndexFromMatrialList()
{
	std::vector<int> indexList;
	POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_lcRequirement.GetNextSelectedItem(pos);
			indexList.push_back(nItem);
		}
	}
	return indexList;
}

void CDlgRequirement::RefreshListCtrl()
{
	m_lcRequirement.DeleteAllItems();

	int nItem = 0;
	for(int i = 0; i < (*m_pvComponentInputItem).size(); i++)
	{
		m_lcRequirement.InsertItem(nItem, _T(""));
		RequirementItemData* pData = new RequirementItemData;
		m_lcRequirement.SetItemData(nItem, (DWORD_PTR)pData);
		SetOneItemTextAndData(nItem, (*m_pvComponentInputItem)[i]);
		nItem++;
	}

	m_eSortOrderType = 0;
	m_nSortintSubItemID = -1;
	//m_lcRequirement.SetSortingSubItem(m_nSortintSubItemID, m_eSortOrderType);
	((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetSortingSubItem(m_nSortintSubItemID, m_eSortOrderType);
}

void CDlgRequirement::OnDelMatrialList()
{
	if (MessageBox("是否确认删除板件","提示",MB_YESNO) == IDYES)
	{
		POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			return;
		}
		else
		{
			//std::vector<std::map<std::string,std::string>> items = GetMatrialItems();
			//std::vector<std::map<std::string,std::string>> result;
			//int nItem = m_matriallist.GetNextSelectedItem(pos);
			//for (int i = 0; i < items.size() ; i++)
			//{

			//	if (i == nItem)
			//	{
			//		nItem = m_matriallist.GetNextSelectedItem(pos);
			//	}
			//	else
			//	{
			//		result.push_back(items[i]);
			//	}
			//}
			//SetMatrialItems(result);
			vector<ComponentInputItem> vInputItemTmp;
			int nItem = m_lcRequirement.GetNextSelectedItem(pos);
			for (int i = 0; i < m_lcRequirement.GetItemCount() ; i++)
			{

				if (i == nItem)
				{
					nItem = m_lcRequirement.GetNextSelectedItem(pos);
				}
				else
				{
					ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(i);
					if(pComponentInputItem)
						vInputItemTmp.push_back(*pComponentInputItem);
				}
			}
			(*m_pvComponentInputItem) = vInputItemTmp;
			RefreshListCtrl();
		}
	}
}

/*---------------------------------------*/
//	函数说明：
//		计算轮廓点的长宽
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
void CalPointInfoRect(vector<PointInfo>& PointInfos, float& len, float& width)
{
	std::vector<PointInfo> input_pointInfos = PointInfos;

	// 计算异形点矩形
	int nCount = input_pointInfos.size();
	float OutlineMax_x, OutlineMax_y, OutlineMin_x, OutlineMin_y, max_r;

	// 开始计算轮廓点

	PointInfo pnt = input_pointInfos[0];
	OutlineMax_x = OutlineMin_x = pnt.x;
	OutlineMax_y = OutlineMin_y = pnt.y;
	max_r = pnt.r;

	// 求得矩形
	for (int j = 1; j < nCount; j++)
	{
		pnt = input_pointInfos[j];
		float x = pnt.x;
		float y = pnt.y;
		float r = pnt.r;

		if (x > OutlineMax_x)
			OutlineMax_x = x;

		if (x < OutlineMin_x)
			OutlineMin_x = x;

		if (y > OutlineMax_y)
			OutlineMax_y = y;

		if (y < OutlineMin_y)
			OutlineMin_y = y;

		if (r > max_r)
			max_r = r;
	}

	len		= abs(OutlineMax_x - OutlineMin_x);
	width	= abs(OutlineMax_y - OutlineMin_y);
}


//自定义异形
void CDlgRequirement::OnSelfOthershap()
{
	int nItem = m_lcRequirement.GetNextItem(-1,LVIS_SELECTED);
	ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
	if(pComponentInputItem == NULL)
		return;

	AbTools ab;

	if (ab.ShowDialog() == IDOK)
	{
		CString selfDefineOtherShape = ab.GetPanelOutlineStr();
		CString otherShapeSetting = "";
		//m_matriallist.SetItemText(nItem,15,otherShapeSetting);		//设置原异形

		// 2017-11-6 yuanzb 发现自定义异形为“D”形状时，计算矩形出错，因为三个点在同一条直线上，应该将圆弧在最中间处断开

		std::vector<PointInfo> pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(selfDefineOtherShape.GetBuffer());
		pointInfos = OtherShapeHelper::ClearUnnessaryPointInfo(pointInfos);

		// 判断异形数据是否超出了板件尺寸
		float input_len, input_width;

		CalPointInfoRect(pointInfos, input_len, input_width);

		if (input_len != pComponentInputItem->m_fLength || input_width != pComponentInputItem->m_fWidth)
		{
			CString str_msg;
			str_msg.Format("板件尺寸：%0.1fx%0.1f --> DXF尺寸：%0.1fx%0.1f", pComponentInputItem->m_fLength, pComponentInputItem->m_fWidth, input_len, input_width);

			AfxMessageBox(str_msg, MB_OK);
			
			pComponentInputItem->m_fLength = input_len;
			pComponentInputItem->m_fWidth = input_width;
			
		}



		pointInfos = OtherShapeHelper::MoveOutlinePoints(pointInfos, 0, pComponentInputItem->m_fWidth);


		pComponentInputItem->m_strOtherShapeID = _T("");
		pComponentInputItem->m_strOtherShapeIDAndParam = _T("");
		pComponentInputItem->m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(pointInfos).c_str();//设置自定义异形

		// 判断异形与板件大小是否匹配，不匹配则调整板件大小

		

		// 设置板件长宽
		SetOneItemTextAndData(nItem, *pComponentInputItem);
	}
}

void CDlgRequirement::OnNMDblclkListRequirement(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nItem = pNMItemActivate->iItem;
	if(nItem > -1 && nItem < m_pvComponentInputItem->size())
	{
		ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
		if(pComponentInputItem)
		{
			Component* pTheComponent = ConvertOneInputInfoToOneComponent(*pComponentInputItem);
			CVecSoltDlg dlg;
			dlg.SetComponent(pTheComponent);
			dlg.DoModal();
			delete pTheComponent;
		}
	}

	*pResult = 0;
}

static int CALLBACK SortRequirementListFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString &lp1 = ((RequirementItemData *)lParam1)->m_strSortingText;
	CString &lp2 = ((RequirementItemData *)lParam2)->m_strSortingText;
	//int &sort = *(int *)lParamSort;
	SortParam& param = *((SortParam*)lParamSort);
	if (param.m_nSortOrderType == 0)
	{
		if(param.m_nSortColumnID == LIST_COLUMN_CARVING_LEN \
			|| param.m_nSortColumnID == LIST_COLUMN_CARVING_WIDTH \
			|| param.m_nSortColumnID == LIST_COLUMN_CARVING_THICKNESS \
			|| param.m_nSortColumnID == LIST_COLUMN_COUNT \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_FRONT \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_RIGHT \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_BACK \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_LEFT \
			|| param.m_nSortColumnID == LIST_COLUMN_PRODUCT_LEN \
			|| param.m_nSortColumnID == LIST_COLUMN_PRODUCT_WIDTH \
			|| param.m_nSortColumnID == LIST_COLUMN_PRODUCT_THICKNESS)
		{
			float fVal1 = atof(lp1.GetBuffer());
			lp1.ReleaseBuffer();
			float fVal2 = atof(lp2.GetBuffer());
			lp2.ReleaseBuffer();
			return fVal1 > fVal2;
		}
		else
			return lp1.CompareNoCase(lp2);
	}
	else
	{
		if(param.m_nSortColumnID == LIST_COLUMN_CARVING_LEN \
			|| param.m_nSortColumnID == LIST_COLUMN_CARVING_WIDTH \
			|| param.m_nSortColumnID == LIST_COLUMN_CARVING_THICKNESS \
			|| param.m_nSortColumnID == LIST_COLUMN_COUNT \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_FRONT \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_RIGHT \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_BACK \
			|| param.m_nSortColumnID == LIST_COLUMN_BANDING_LEFT \
			|| param.m_nSortColumnID == LIST_COLUMN_PRODUCT_LEN \
			|| param.m_nSortColumnID == LIST_COLUMN_PRODUCT_WIDTH \
			|| param.m_nSortColumnID == LIST_COLUMN_PRODUCT_THICKNESS)
		{
			float fVal1 = atof(lp1.GetBuffer());
			lp1.ReleaseBuffer();
			float fVal2 = atof(lp2.GetBuffer());
			lp2.ReleaseBuffer();
			return fVal1 < fVal2;
		}
		else
			return lp2.CompareNoCase(lp1);
	}
}


void CDlgRequirement::OnLvnColumnclickListRequirement(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	{
		//CString* pstrItemData = new CString;
		//*pstrItemData = m_lcRequirement.GetItemText(i, pNMLV->iSubItem);
		//m_lcRequirement.SetItemData(i, (DWORD_PTR)pstrItemData);

		RequirementItemData* pData = (RequirementItemData*)m_lcRequirement.GetItemData(i);
		pData->m_strSortingText = m_lcRequirement.GetItemText(i, pNMLV->iSubItem);

	}

	if (pNMLV->iSubItem != m_nSortintSubItemID)
	{
		m_eSortOrderType = 0;	
	}
	else
	{
		m_eSortOrderType = (m_eSortOrderType == 0) ? 1 : 0;
	}
	m_nSortintSubItemID = pNMLV->iSubItem;
	SortParam param;
	param.m_nSortOrderType = m_eSortOrderType;
	param.m_nSortColumnID = pNMLV->iSubItem;
	m_lcRequirement.SortItems(SortRequirementListFunc, (DWORD_PTR)&param);
	//for(int i = 0; i < m_lcRequirement.GetItemCount(); i++)
	//{
	//	CString* pstrItemData = (CString*)(m_lcRequirement.GetItemData(i));
	//	delete pstrItemData;
	//	m_lcRequirement.SetItemData(i, (DWORD_PTR)NULL);
	//}

	((CSkinHeaderCtrl*)m_lcRequirement.GetDlgItem(0))->SetSortingSubItem(m_nSortintSubItemID, m_eSortOrderType);

	*pResult = 0;
}

ComponentInputItem* CDlgRequirement::GetComponentInputItemByID(int nItem)
{
	ComponentInputItem* pRet = NULL;
	if(nItem <  0 || nItem >= m_lcRequirement.GetItemCount())
		return pRet;

	//CString strBarcode = m_lcRequirement.GetItemText(nItem, RequirementListColumn::LIST_COLUMN_BARCODE);
	UINT uSerialID = ((RequirementItemData*)m_lcRequirement.GetItemData(nItem))->m_uSerialID;

	for(int i = 0; i < m_pvComponentInputItem->size(); i++)
	{
		//if(strBarcode.Compare((*m_pvComponentInputItem)[i].m_strBarcode) == 0)
		if((*m_pvComponentInputItem)[i].m_uSerialID == uSerialID)
		{
			pRet = &(*m_pvComponentInputItem)[i];
			break;
		}
	}
	return pRet;
}

void CDlgRequirement::OnResetMachiningInfo(int nType)
{
	CDlgResetMachiningInfo dlg(nType);
	dlg.DoModal();

	POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_lcRequirement.GetNextSelectedItem(pos);
			ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
			if(pComponentInputItem)
			{
				if(dlg.m_nResetOtherShape)
				{
					std::string stringRotateFlag = "0";
					std::string stringComponentLen = GetFloatString(pComponentInputItem->m_fLength, 1).GetBuffer();
					std::string stringComponentWidth = GetFloatString(pComponentInputItem->m_fWidth, 1).GetBuffer();
					std::vector<PointInfo> pointInfo = OtherShapeHelper::getPointInfoRectangle(
						stringRotateFlag,    
						stringComponentLen,
						stringComponentWidth);

					pComponentInputItem->m_strOtherShapeIDAndParam = ""; //异形参数
					pComponentInputItem->m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(pointInfo).c_str();	   // 异形点信息
					pComponentInputItem->m_eRotateType = 0;          //旋转
					pComponentInputItem->m_strOtherShapeID.Format(_T(""));  // 异形ID  
				}

				if(dlg.m_nResetSlot_UpperFace)
					pComponentInputItem->m_strUpperFaceSlotInfo = _T("");

				if(dlg.m_nResetSlot_DownerFace)
					pComponentInputItem->m_strDownerFaceSlotInfo = _T("");

				if(dlg.m_nResetHole_UpperFace)
					pComponentInputItem->m_strUpperFaceHoleInfo = _T("");

				if(dlg.m_nResetHole_DownerFace)
					pComponentInputItem->m_strDownerFaceHoleInfo = _T("");

				SetOneItemTextAndData(nItem, *pComponentInputItem);
			}		
		}
	}
}

void CDlgRequirement::OnResetHoleInfo()
{
	OnResetMachiningInfo(CDlgResetMachiningInfo::RESET_HOLE);
}

void CDlgRequirement::OnPreCombine()
{
	vector<CString> vPreCombineBarcode;

	POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_lcRequirement.GetNextSelectedItem(pos);

		ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
		if(pComponentInputItem)
		{
			vPreCombineBarcode.push_back(pComponentInputItem->m_strBarcode);
		}
	}

	if(vPreCombineBarcode.size() > 1)
	{
		AddPreCombine(vPreCombineBarcode);
	}
}

void CDlgRequirement::OnExportComponentNC()
{
	vector<CString> vComponentBarcode;

	POSITION pos = m_lcRequirement.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_lcRequirement.GetNextSelectedItem(pos);

		ComponentInputItem* pComponentInputItem = GetComponentInputItemByID(nItem);
		if(pComponentInputItem)
		{
			vComponentBarcode.push_back(pComponentInputItem->m_strBarcode);
		}
	}

	if(vComponentBarcode.size() > 0)
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_USER_EXPORT_COMPONENT_NC, (WPARAM)(&vComponentBarcode), 0);
	}

}

void CDlgRequirement::AddPreCombine(vector<CString> vPreCombineBarcode)
{
	vector<ComponentInputItem> vPreCombineComponentItem;
	for(int i = 0; i < m_pvComponentInputItem->size(); i++)
	{
		for(int j = 0; j < vPreCombineBarcode.size(); j++)
		{
			CString strBarcode_InList = (*m_pvComponentInputItem)[i].m_strBarcode;
			CString strBarcode_Selected = vPreCombineBarcode[j];
			if(strBarcode_InList.CompareNoCase(strBarcode_Selected) == 0)
			{
				vPreCombineComponentItem.push_back((*m_pvComponentInputItem)[i]);
			}
		}
	}

	//m_pDlgPreCombine->AddPreCombine(vPreCombineComponentItem);
	CDlgRequirement_PreCombine dlg;
	dlg.SetPreCombine(vPreCombineComponentItem);
	if(dlg.DoModal() == IDOK)
	{
		vector<PreCombineComponent> thePreCombineItem = dlg.GetPreCombine();
		if(thePreCombineItem.size() > 1)
		{
			m_lbPreCombine.AddItem(thePreCombineItem);
			ShowOrHidePreCombineListBox();
		}
	}
}

void CDlgRequirement::ShowOrHidePreCombineListBox()
{
	ArrangeUI();
	//if(m_pDlgPreCombine->GetItemCount() > 0 && !m_pDlgPreCombine->IsWindowVisible())
	//{
	//	m_pDlgPreCombine->ShowWindow(SW_SHOW);
	//}
	//else if(m_pDlgPreCombine->GetItemCount() == 0 && m_pDlgPreCombine->IsWindowVisible())
	//{
	//	m_pDlgPreCombine->ShowWindow(SW_HIDE);
	//}
}

void CDlgRequirement::ArrangeUI()
{
	CRect rcClient;
	GetClientRect(rcClient);

	if(m_lcRequirement.GetSafeHwnd())
	{
		//m_lcRequirement.MoveWindow(0, 0, rcClient.Width(), rcClient.Height()-PRE_COMBINE_DLG_HEIGHT);
		//if(m_pDlgPreCombine->GetItemCount() > 0)
		//{
		//	m_pDlgPreCombine->MoveWindow(0, rcClient.Height()-PRE_COMBINE_DLG_HEIGHT, rcClient.Width(), rcClient.Height());
		//}
		//else
		//	m_lcRequirement.MoveWindow(0, 0, rcClient.Width(), rcClient.Height());

		if(m_lbPreCombine.GetCount() > 0)
		{
			CRect rcCtrl;
			rcCtrl.SetRect(rcClient.right-PRE_COMBINE_LISTBOX_WIDTH, rcClient.top, rcClient.right, rcClient.bottom);
			m_lbPreCombine.MoveWindow(rcCtrl);
			m_lbPreCombine.ShowWindow(SW_SHOW);
			rcCtrl.SetRect(rcClient.left, rcClient.top, rcClient.right-PRE_COMBINE_LISTBOX_WIDTH, rcClient.bottom);
			m_lcRequirement.MoveWindow(rcCtrl);
		}
		else
		{
			m_lbPreCombine.ShowWindow(SW_HIDE);
			m_lcRequirement.MoveWindow(rcClient);
		}
	}
}

vector<PreCombineItem> CDlgRequirement::GetPreCombineItems()
{
	return m_lbPreCombine.GetItems();
}

LRESULT CDlgRequirement::OnShowOrHidePreCombine(WPARAM wparam, LPARAM lparam)
{
	ArrangeUI();
	return 0;
}

void CDlgRequirement::SetRequirement(vector<ComponentInputItem>& vComponentInputItem, bool bResetPreCombine)
{
	SetPanelInfo(vComponentInputItem);
	if(bResetPreCombine)
	{
		m_lbPreCombine.EmptyPreCombine();
		ArrangeUI();
	}
}

BOOL CDlgRequirement::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(IsKeyPressed(VK_CONTROL) && IsKeyPressed('A'))
		{
			SelectAll();
			return TRUE;
		}
	}

	return CDialogChildBase::PreTranslateMessage(pMsg);
}

void CDlgRequirement::SelectAll()
{
	for(int i = 0; i < m_lcRequirement.GetItemCount(); i++) 
		m_lcRequirement.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void CDlgRequirement::OnLvnDeleteitemListRequirement(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	RequirementItemData* pData = (RequirementItemData*)m_lcRequirement.GetItemData(pNMLV->iItem);
	if(pData)
		delete pData;
	*pResult = 0;
}
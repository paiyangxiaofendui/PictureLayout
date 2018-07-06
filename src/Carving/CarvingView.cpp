// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CarvingView.cpp : CCarvingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Carving.h"
#endif

#include "CarvingDoc.h"
#include "CarvingView.h"
#include "MainFrm.h"

// 外部DataManager.dll的头文件
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../include/DataManager/BaseDataType/RemainderManager/RemainderManager.h"
#include "../../include/DataManager/BaseDataType/RemainderGroup/RemainderGroup.h"
#include "../../include/DataManager/BaseDataType/RemainderItem/RemainderItem.h"


// 外部FileReadWrite.dll的头文件
#include "../../include/FileReadWrite/ExcelReadWrite/ExcelReadWrite.h"
#include "../../include/FileReadWrite/HgmReadWrite/HgmReadWrite.h"
#include "../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"
#include "../../include/FileReadWrite/NCReadWrite/NCReadWrite.h"
#include "../../include/FileReadWrite/ClientInfoReadWrite/ClientInfoReadWrite.h"
#include "../../include/FileReadWrite/DxfReadWrite/DxfReadWrite.h"
#include "../../include/FileReadWrite/RemainderLibraryReadWrite/RemainderLibraryReadWrite.h"



// 外部InternetManager.dll的头文件
#include "../../include/InternetManager/InternetChecking/InternetChecking.h"


#include "UI/ParamDlg.h"
#include "UI/SelMachineDlg.h"
#include "UI/SelectPathDlg.h"
#include "UI/DlgRemainder.h"
#include "UI/DlgRequirement.h"
#include "UI/DlgResult.h"
#include "UI/SelectPrintDlg.h"
#include "UI/ClientRegisterDlg.h"
#include "UI/KnifeAddDlg.h"
#include "UI/OPSetDlg.h"
#include "UI/LeftPanelDlg.h"
#include "UI/UISettingDlg.h"
#include "UI/PrintSetDlg.h"
#include "UI/InOutDlg.h"
#include "UI/LingLiaoDlg.h"
#include "UI/TiebiaoEdgeDlg.h"
#include "UI/OptimizeDlg.h"
#include "UI/DlgLabelPosCalc.h"
#include "UI/DlgWeiXin.h"
#include "UI/DlgExcelInputSetting.h"
#include "UI/RemainderDlg.h"

#include "DataProcess/DataProcess.h"
#include "NCProcess/SyntecProcess.h"
#include "NCProcess/XiaoFengLabelProcess.h"

#include "Misc/HGTools.h"
#include "Misc/ProgramMisc.h"
#include "Misc/Misc.h"

#include "Work/MachineDef.h"

#include "afx.h"

#include <fstream>
#include <io.h>

#include <algorithm>

#include "NCProcess/CNCDataConverter.h"

#include "Work/ProgramDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//class CSelMachineDlg;
//class KnifeClass;
//class CParamDlg;
struct PrepareExportGCodeData
{
	PrepareExportGCodeData()
		:m_pSelMachineDlg(NULL)
		,node_double(false)
		,key(1)
		,pParamDlg(NULL)
		,_cutDouble(false)
		,nodecoor_type(0)
		,nodecoor_type1(0)
		,nodecoor_type2(0)
		,node_print(0)
		,node_sysuse(false)
		,_lessKnifeChange(0)
		,_isLabelInReverseSide(false)
		,other_str(0)
		,_isComline(1)
		,_isComball(0)
	{}

	CSelMachineDlg* m_pSelMachineDlg;
	std::string node_name;
	bool node_double;
	int node_print;
	bool node_sysuse;
	std::string node_groupstr;

	int nodecoor_type;
	int nodecoor_type1;//单工位坐标系
	int nodecoor_type2;//双工位坐标系

	bool _cutDouble;//单双工位判断

	std::string node_cxhx;
	std::string node_pxhx;

	std::string node_pname;
	std::string node_plast;
	std::string node_cname;
	std::string node_clast;
	std::string node_dvcut;

	std::string temp_header1;//正面工位1，2种临时头尾
	std::string temp_ender1;
	std::string temp_header2;
	std::string temp_ender2;

	std::string temp_GFirst2_1;//正面工位2，2种临时头尾
	std::string temp_GLast2_1;
	std::string temp_GFirst2_2;
	std::string temp_GLast2_2;

	std::string temp_GBFirst1_1;//反面工位1，2种临时头尾
	std::string temp_GBLast1_1;
	std::string temp_GBFirst1_2;
	std::string temp_GBLast1_2;

	std::string temp_GBFirst2_1;//反面工位2，2种临时头尾
	std::string temp_GBLast2_1;
	std::string temp_GBFirst2_2;
	std::string temp_GBLast2_2;

	float fCutTwice_SmallSize;
	float fCutTwice_ReserveHeight;
	float fCutTwice_SecondSpeed;
	bool bCutTwice_Enable;
	float fTwoCutThreshold;
	float fTwoCutReserve;
	float fTwoCutSpeed;

	int key;

	CParamDlg* pParamDlg;
	CString pathName;
	CString pathPrint;

	int _lessKnifeChange;
	int _knife_shifting;
	bool _isLabelInReverseSide;
	int other_str;
	int _isComline;
	int _isComball;

	KnifeClass* pKnifeClass;
};
struct PrepareKnifeInfoData
{
	PrepareKnifeInfoData()
		:pThePanel(NULL)
	{}

	Panel* pThePanel;
	std::vector<KnifeType> kt;
};

struct TransCoordinateData
{
	std::string nc_font_temp_path;
	std::string nc_font_path;
	std::string nc_back_w_path;
	std::string nc_back_w_temp_path;
	std::string nc_back_h_path;
	std::string nc_back_h_temp_path;
	bool HaveDvsh;
	bool HaveDvsw;
	bool HaveDvs;

	int _usedLineX;
	int _usedLineY;

	int nDecimal;
};

struct CombineFileData
{
	std::string str_nextthin;
	std::string tempnextcname2;
	std::string str_thin;

	std::string nc_font_temp_path;
	std::string nc_font_path;
	std::string nc_back_h_path;
	std::string nc_back_w_path;
	std::string allpath2;
};

void ExcelSettingForTheFistTime(CString strFilePath);
//#ifndef REQUIREMENT_LIST_TITLE
//#define REQUIREMENT_LIST_TITLE

//#endif

// CCarvingView


IMPLEMENT_DYNCREATE(CCarvingView, CFormView)

BEGIN_MESSAGE_MAP(CCarvingView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCarvingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN_SOURCE_HG, &CCarvingView::OnOpenSourceHG)
	ON_COMMAND(ID_MENU_OPEN_HGE_APPEND, &CCarvingView::OnOpenSourceHGAppend)
	ON_COMMAND(ID_MENU_OPEN_EXCEL_APPEND, &CCarvingView::OnOpenSourceExcelAppend)
	ON_COMMAND(ID_OPTIONS_PEN, &CCarvingView::OnOptionsPen)
	ON_COMMAND(ID_VIEW_REMAINDER, &CCarvingView::OnViewRemainder)
	ON_COMMAND(ID_VIEW_COMPONENT, &CCarvingView::OnViewComponent)
	ON_COMMAND(ID_VIEW_SOLUTION, &CCarvingView::OnViewSolution)
	ON_WM_SIZE()
	ON_COMMAND(ID_OPTIMIZE, &CCarvingView::OnOptimize)
	ON_COMMAND(ID_SAVE, &CCarvingView::OnSave)
	ON_COMMAND(ID_EDIT_PRAGMA_SAW_KERF, &CCarvingView::OnEditPragmaSawKerf)
	ON_COMMAND(ID_EDIT_PANEL_LENGTH, &CCarvingView::OnEditPanelLength)
	ON_COMMAND(ID_EDIT_PANEL_WIDTH, &CCarvingView::OnEditPanelWidth)
	ON_COMMAND(ID_EDIT_STEP1_COUNT, &CCarvingView::OnEditStep1Count)
	ON_COMMAND(ID_EDIT_STEP2_COUNT, &CCarvingView::OnEditStep2Count)
	ON_COMMAND(ID_EDIT_STEP3_COUNT, &CCarvingView::OnEditStep3Count)
	ON_COMMAND(ID_EDIT_STEP3_ACCEPTABLE_UTI, &CCarvingView::OnEditStep3AcceptableUti)
	ON_COMMAND(ID_COMBO_STEP1_ALG, &CCarvingView::OnComboStep1Alg)
	ON_COMMAND(ID_EXPORT_GCODE, &CCarvingView::OnExportGcode)
	ON_COMMAND(ID_COMBO_LAYOUT_ORIGIN, &CCarvingView::OnLayoutOrg)
	ON_COMMAND(ID_BUTTON_PRINT_LABEL, &CCarvingView::OnButtonPrintLabel)
	ON_COMMAND(ID_MENU_MACHINE_PARAM_SETTING, &CCarvingView::OnMenuMachineParamSetting)
	ON_COMMAND(ID_MENU_BASE_SETTING, &CCarvingView::OnMenuBaseSetting)
	ON_COMMAND(ID_MENU_OPTIMIZE_SETTING, &CCarvingView::OnMenuOptimizeSetting)
	ON_COMMAND(ID_MENU_REMAINDER_SETTING, &CCarvingView::OnRemainderSetting)
	ON_COMMAND(ID_MENU_DEFAULT_LOADING_DIR_SETTING, &CCarvingView::OnMenuDefaultLoadingDirSetting)
	ON_COMMAND(ID_MENU_DEFAULT_SAVING_DIR_SETTING, &CCarvingView::OnMenuDefaultSavingDirSetting)
	ON_COMMAND(ID_MENU_OTHER_SETTING, &CCarvingView::OnMenuOtherSetting)
	ON_COMMAND(ID_MENU_LABEL_SETTING, &CCarvingView::OnMenuLabelSetting)
	ON_COMMAND(ID_MENU_EXCEL_TEMPLATE_SETTING, &CCarvingView::OnMenuExcelTemplateSetting)
	ON_COMMAND(ID_MENU_LINGLIAO_TABLE_SETTING, &CCarvingView::OnMenuLingliaoTableSetting)
	ON_COMMAND(ID_EXPORT_METERIAL_TABLE1, &CCarvingView::OnExportMeterialTable1)
	ON_COMMAND(ID_EXPORT_METERIAL_TABLE2, &CCarvingView::OnExportMeterialTable2)
	ON_COMMAND(ID_OPEN_SOURCE_EXCEL, &CCarvingView::OnOpenSourceExcel)
	ON_COMMAND(ID_OPEN_SOLUTION, &CCarvingView::OnOpenSolution)
	ON_COMMAND(ID_EXPORT_PANEL_METERIAL_TABLE, &CCarvingView::OnExportPanelMeterialTable)
	ON_MESSAGE(WM_USER_EXPORT_MATERIAL_LIST, &CCarvingView::OnExportMaterialList)
	ON_MESSAGE(WM_USER_EXPORT_COMPONENT_NC, &CCarvingView::OnExportComponentNC)
	ON_COMMAND(ID_BUTTON_ABOUT_US, &CCarvingView::OnButtonAboutUs)
	ON_COMMAND(ID_BUTTON_OUR_CUSTOMERS, &CCarvingView::OnButtonOurCustomers)
	ON_COMMAND(ID_MENU_CONTACK_WITH_US_QQ, &CCarvingView::OnButtonContackWithUsQQ)
	ON_COMMAND(ID_MENU_CONTACK_WITH_US_WEIXIN, &CCarvingView::OnButtonContackWithUsWeiXin)
	ON_COMMAND(ID_MENU_HELP_TUTORIAL, &CCarvingView::OnButtonTutorial)
	ON_COMMAND(ID_MENU_VIDEO, &CCarvingView::OnButtonVideo)
	ON_COMMAND(ID_MENU_OPEN_TEMPLATE, &CCarvingView::OnButtonOpenTemplate)
	ON_COMMAND(ID_MENU_SETTING_SAVE, &CCarvingView::OnMenuSaveAllSettings)
	ON_COMMAND(ID_MENU_SETTING_REVERT, &CCarvingView::OnMenuRevertAllSettings)
END_MESSAGE_MAP()

// CCarvingView 构造/析构

CCarvingView::CCarvingView()
	: CFormView(IDD)
	, m_eCurViewType(VIEW_PANEL_LIST)
	, m_pDlgRemainder(NULL)
	, m_pDlgRequirement(NULL)
	, m_pDlgResult(NULL)
{
	// TODO: 在此处添加构造代码

}

CCarvingView::~CCarvingView()
{
}

void CCarvingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TAB, m_Tab);
}

BOOL CCarvingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

// CCarvingView 绘制

void CCarvingView::OnDraw(CDC* /*pDC*/)
{
	CCarvingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCarvingView 打印


void CCarvingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCarvingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCarvingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCarvingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CCarvingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCarvingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCarvingView 诊断

#ifdef _DEBUG
void CCarvingView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCarvingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCarvingDoc* CCarvingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarvingDoc)));
	return (CCarvingDoc*)m_pDocument;
}
#endif //_DEBUG




// CCarvingView 消息处理程序
void CCarvingView::OnOpenSourceHG()
{

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CSingleon* pSingleton = CSingleon::GetSingleton();

#ifndef YiWei
	CString filter = "hge 文件(*.hge)|*.hge|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hge"), _T("*.hge"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("导入开料单(hge)");
#else
	CString filter = "ywe 文件(*.ywe)|*.ywe|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("ywe"), _T("*.ywe"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("导入开料单(ywe)");
#endif
	


	if ( fileDlg.DoModal() == IDOK)
	{

		// 先清除上一次的数据
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		CString strOpenedFile = fileDlg.GetPathName();
		m_vOpenedFile.clear();
		m_vOpenedFile.push_back(strOpenedFile);

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

#ifndef YiWei
		if (strExtName == "hge")
#else
		if (strExtName == "ywe")
#endif
		{
			//GetParent()->SetWindowText("def");
			GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//正面孔
			param.b_downvecImportEnable		= param_dlg->DVecInput();//反面孔
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//正面槽
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//反面槽
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//异形过滤
			param.i_comnum	= param_dlg->Editnum();//切割数量
			param.i_comchange = param_dlg->Comchange();//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
			param.f_changex = param_dlg->MoveX();//孔槽偏移
			param.f_changey = param_dlg->MoveY();//孔槽偏移	
			param.vecAdder = param_dlg->GetVecAdder();//孔位偏差（找到指定孔直径的孔，其直径加一个值）		
			param._cxy = param_dlg->GetVecFilter();//孔位过滤（找到指定孔直径的孔，并移除）
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(HgmReadWrite::ReadPanelInfo(strOpenedFile, m_vComponentInputItem, param) == true)
			{
				//// 检查条码重复
				//CheckRepeatBarcode(m_vComponentInputItem);


				// 检查板件是否有超出尺寸范围的，删掉
				//CheckComponentList(m_vComponentInputItem);

				// 小板设为无纹理
				if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
				{
					SetSmallCpnNoTexture(m_vComponentInputItem);
				}

				for(int i = 0; i < m_vComponentInputItem.size(); i++)
					m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				ParamSettingDlg dlgOtherSetting;
				dlgOtherSetting.loadSettings();
				ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
				if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
				{
					ModifyReminderByDateAndOrderFormat(m_vComponentInputItem);
				}
			}

			// 计算小板
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "块小板";
			AfxMessageBox(str);

			// 自动计算优化参数
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

		}


		// 切换视图
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
		
		RecordMaterial();
	}

	// 设置界面输入板件组
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem);


	// 备份输入板件组
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);





}

/*---------------------------------------*/
//	函数说明：
//		检测重复条码
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
BOOL CCarvingView::IsRepeatBarcode(vector<ComponentInputItem>& src_list, vector<ComponentInputItem>& add_list)
{
	int nSrcCount = src_list.size();
	int nAddCount = add_list.size();

	if (nSrcCount == 0)
	{
		return FALSE;
	}

	for (int i_add = 0; i_add < nAddCount; i_add++)
	{
		ComponentInputItem& add_cpn = add_list.at(i_add);
		CString strAddBarcode = add_cpn.m_strBarcode;

		for (int i_src = 0; i_src < nSrcCount; i_src++)
		{
			ComponentInputItem& src_cpn = src_list.at(i_src);
			CString strSrcBarcode = src_cpn.m_strBarcode;

			// 有相同的条码
			if (strSrcBarcode == strAddBarcode)
			{

				//AfxMessageBox("检测到有重复条码:" + strSrcBarcode +  "请检查后再追加！");
				return TRUE;
			}
		}
	}

	return FALSE;
}



/*---------------------------------------*/
//	函数说明：
//		检测重复条码
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
BOOL CCarvingView::IsRepeatBarcode(vector<ComponentInputItem>& src_list)
{
	int nSrcCount = src_list.size();

	if (nSrcCount == 0)
	{
		return FALSE;
	}

	CString msg;
	int i,j;
	BOOL flag = FALSE;
	for ( i = 0; i < nSrcCount-1; i++)
	{
		for ( j = i+1; j < nSrcCount; j++)
		{
			ComponentInputItem& cpn1 = src_list.at(i);
			CString strBarcode1 = cpn1.m_strBarcode;


			ComponentInputItem& cpn2 = src_list.at(j);
			CString strBarcode2 = cpn2.m_strBarcode;

			// 有相同的条码
			if (strBarcode1 == strBarcode2)
			{
				msg += "检测到有重复条码:" + strBarcode1 +  "\n";
				flag = TRUE;
			}
		}
	}

	if (flag == TRUE)
	{
		//AfxMessageBox(msg);
		return TRUE;
	}
	else
		return FALSE;
}





// 追加
void CCarvingView::OnOpenSourceHGAppend()
{

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	CSingleon* pSingleton = CSingleon::GetSingleton();

#ifndef YiWei
	CString filter = "hge 文件(*.hge)|*.hge|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hge"), _T("*.hge"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("导入开料单(hge)");
#else
	CString filter = "ywe 文件(*.ywe)|*.ywe|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("ywe"), _T("*.ywe"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("导入开料单(ywe)");
#endif

	if ( fileDlg.DoModal() == IDOK)
	{

		// 先清除上一次的数据
		//pSingleton->ClearAllData();
		//m_vComponentInputItem.clear();
		vector<ComponentInputItem> vAppendComponentInputItem;


		CString strOpenedFile = fileDlg.GetPathName();
		m_vOpenedFile.push_back(strOpenedFile);

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

#ifndef YiWei
		if (strExtName == "hge")
#else
		if (strExtName == "ywe")
#endif
		{
			//GetParent()->SetWindowText("def");
			GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//正面孔
			param.b_downvecImportEnable		= param_dlg->DVecInput();//反面孔
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//正面槽
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//反面槽
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//异形过滤
			param.i_comnum	= param_dlg->Editnum();//切割数量
			param.i_comchange = param_dlg->Comchange();//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
			param.f_changex = param_dlg->MoveX();//孔槽偏移
			param.f_changey = param_dlg->MoveY();//孔槽偏移	
			param.vecAdder = param_dlg->GetVecAdder();//孔位偏差（找到指定孔直径的孔，其直径加一个值）		
			param._cxy = param_dlg->GetVecFilter();//孔位过滤（找到指定孔直径的孔，并移除）
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(HgmReadWrite::ReadPanelInfo(strOpenedFile, vAppendComponentInputItem, param) == true)
			{
				// 检查板件是否有超出尺寸范围的，删掉
				//CheckComponentList(vAppendComponentInputItem);

				// 小板设为无纹理
				if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
				{
					SetSmallCpnNoTexture(vAppendComponentInputItem);
				}

				// 追加之前先判断是否有条码重复
				//if (CheckRepeatBarcode(m_vComponentInputItem, vAppendComponentInputItem) == FALSE)
				{
					// 追加
					for(int i = 0; i < vAppendComponentInputItem.size(); i++)
						vAppendComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

					ParamSettingDlg dlgOtherSetting;
					dlgOtherSetting.loadSettings();
					ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
					if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
					{
						ModifyReminderByDateAndOrderFormat(vAppendComponentInputItem);
					}

					m_vComponentInputItem.insert(m_vComponentInputItem.end(), vAppendComponentInputItem.begin(), vAppendComponentInputItem.end());
				}


				//// 再次判断重复条码
				//CheckRepeatBarcode(m_vComponentInputItem);
			}

			// 计算小板
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "块小板";
			AfxMessageBox(str);

			// 自动计算优化参数
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

		}


		// 切换视图
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
	}

	// 设置界面输入板件组
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem, false);

	// 备份输入板件组
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

}

/*---------------------------------------*/
//	函数说明：
//		追加excel
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
void CCarvingView::OnOpenSourceExcelAppend()
{
	// TODO: 在此添加命令处理程序代码

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CSingleon* pSingleton = CSingleon::GetSingleton();

	CString filter = "xls 文件(*.xls)|*.xls|所有文件 (*.*)|*.*||";

	CFileDialog fileDlg(true, _T("xls"), _T("*.xls"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("导入开料单(excel)");
	if ( fileDlg.DoModal() == IDOK)
	{

		// 先清除上一次的数据
		//pSingleton->ClearAllData();
		//m_vComponentInputItem.clear();
		vector<ComponentInputItem> vAppendComponentInputItem;


		CString strOpenedFile = fileDlg.GetPathName();
		m_vOpenedFile.push_back(strOpenedFile);

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xls" || strExtName == "xlsx" )
		{
			GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
			SourceFilePreProccesParam param;
			param.b_upvecImportEnable    = param_dlg->VecInput();//正面孔
			param.b_downvecImportEnable		= param_dlg->DVecInput();//反面孔
			param.b_upsoltImportEnable		= param_dlg->SoltInput();//正面槽
			param.b_downsoltImportEnable = param_dlg->DSoltInput();//反面槽
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//异形过滤
			param.i_comnum	= param_dlg->Editnum();//切割数量
			param.i_comchange = param_dlg->Comchange();//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
			param.f_changex = param_dlg->MoveX();//孔槽偏移
			param.f_changey = param_dlg->MoveY();//孔槽偏移	
			param.vecAdder = param_dlg->GetVecAdder();//孔位偏差（找到指定孔直径的孔，其直径加一个值）		
			param._cxy = param_dlg->GetVecFilter();//孔位过滤（找到指定孔直径的孔，并移除）
			param.f_width = param_dlg->Width();
			param.f_height = param_dlg->Height();
			param.f_Deburringwidth = param_dlg->Deburringwidth();
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(ExcelReadWrite::ReadPanelInfo(strOpenedFile, vAppendComponentInputItem, param) == true)
			{
				
			}
			else
			{
				if (strExtName == "xlsx")
				{
					AfxMessageBox("由于office兼容性问题，系统无法使用Excel 2003 自动读取 Excel 2007保存的 xlsx文件! \n请双击" + strOpenedFile + ",另存为xls文件后，再导入！");
				}

			}

			// 检查板件是否有超出尺寸范围的，删掉
			//CheckComponentList(vAppendComponentInputItem);
			// 小板设为无纹理
			if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
			{
				SetSmallCpnNoTexture(vAppendComponentInputItem);
			}

			//// 追加之前先判断是否有条码重复
			//if (CheckRepeatBarcode(m_vComponentInputItem, vAppendComponentInputItem) == FALSE)
			{
				// 追加
				for(int i = 0; i < vAppendComponentInputItem.size(); i++)
					vAppendComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				ParamSettingDlg dlgOtherSetting;
				dlgOtherSetting.loadSettings();
				ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
				if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
				{
					ModifyReminderByDateAndOrderFormat(vAppendComponentInputItem);
				}

				m_vComponentInputItem.insert(m_vComponentInputItem.end(), vAppendComponentInputItem.begin(), vAppendComponentInputItem.end());
			}

			//// 再次判断重复条码
			//CheckRepeatBarcode(m_vComponentInputItem);


			// 计算小板
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "块小板";
			AfxMessageBox(str);


			// 自动计算优化参数
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);


		}

		// 切换视图
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
	}

	// 设置界面输入板件组
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem, false);

	// 备份输入板件组
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

}







void CCarvingView::OnOptionsPen()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//m_Tab.SetUIInfo(10, IDB_TAB1_N, IDB_TAB1_H);
	//m_Tab.SetUIInfo(0, IDB_TAB_UNSELECTED_NORMAL, IDB_TAB_UNSELECTED_HOVER, IDB_TAB_SELECTED_NORMAL, IDB_TAB_SELECTED_NORMAL);
	m_Tab.SetUIInfo(10, IDB_TAB1_N, IDB_TAB1_H, IDB_TAB1_N, IDB_TAB1_H);
	m_Tab.AddTab(_T("排板余料"), ID_VIEW_REMAINDER);
	m_Tab.AddTab(_T("需切成品"), ID_VIEW_COMPONENT);
	m_Tab.AddTab(_T("优化方案"), ID_VIEW_SOLUTION);
	//m_Tab.AddTab(L"c", 3);

	m_pDlgRemainder = new CDlgRemainder(this);
	m_pDlgRemainder->Create(CDlgRemainder::IDD, this);

	//vector<RemainderInputItem> vRemainderInputItem;
	//RemainderInputItem remainder = {100, 50, 18, _T("木"), 1};
	//vRemainderInputItem.push_back(remainder);
	//m_pDlgRemainder->SetRemainderInputItem(vRemainderInputItem);

	SwitchView(VIEW_PANEL_LIST);

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);


	if (param_dlg->m_CheckRemainderLibrary == TRUE)
	{

		// 读取余料库

		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_name = ExeFile;
		file_name = file_name.Left(file_name.ReverseFind('\\')+1) ;


		CString filename = file_name + "RemainderLibrary.hge";

		RemainderLibraryReadWrite::Read(filename.GetBuffer());
		vector<RemainderInputItem> vRemainderInputItem;

		vector<RemainderItem*> all_item_list;
		CSingleon::GetSingleton()->m_RemainderManager.GetAllItems(all_item_list);

		// 余料赋值
		for(int i_rmd = 0; i_rmd < all_item_list.size(); i_rmd++)
		{
			RemainderItem* pRmd = all_item_list.at(i_rmd);
			RemainderInputItem rmd_item;

			rmd_item.m_nXLen		=	pRmd->m_Length		;
			rmd_item.m_nYLen		=	pRmd->m_Width		;
			rmd_item.m_strMaterial	=	pRmd->m_Material	;
			rmd_item.m_fThickness	=	pRmd->m_Thickness	;
			rmd_item.m_nCount		=	pRmd->m_nCount		;

			vRemainderInputItem.push_back(rmd_item);
		}

		m_pDlgRemainder->SetRemainderInputItem(vRemainderInputItem);


	}
	
	// 判断是否需要注册 ，如果有些电脑无法联网又想使用，把这里注释掉




#if 0

	if (ClientInfoReadWrite::IsClientInfoFileExist() == FALSE)
	{
		// 提示注册原因
		AfxMessageBox("未注册！");


		ClientRegisterDlg RegDlg;

		RegDlg.m_RegisterOrUpdate = ClientRegisterDlg::Dlg_Register;

		RegDlg.DoModal();
	}


	// 检测是否注册
	if (ClientInfoReadWrite::IsClientRegistered() == FALSE)
	{
		ClientRegisterDlg RegDlg;


		RegDlg.m_RegisterOrUpdate = ClientRegisterDlg::Dlg_Update;

		RegDlg.DoModal();
	}

	// 日常登录统计 采用线程方式，不阻塞主程序
	HANDLE hdthread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoginStatisticsDailyThread, NULL, NULL, NULL);
	if (hdthread == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("登录统计线程启动失败！");
	}
	

#endif
}

/*---------------------------------------*/
//	函数说明：
//		日常登陆统计线程函数
//
//
//	参数：
//		PVOID lpThreadParameter		-- 参数
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
DWORD CCarvingView::LoginStatisticsDailyThread( PVOID lpThreadParameter )
{
	try
	{
		InternetChecking::LoginStatisticsDaily();
	}
	catch(CException* e)
	{
		TCHAR   szError[1024];   
		e->GetErrorMessage(szError,1024);   //  e.GetErrorMessage(szError,1024); 
		::AfxMessageBox(szError); 
	}


	return 0;
}

void CCarvingView::OnViewRemainder()
{
	// TODO: 在此添加命令处理程序代码
	SwitchView(VIEW_REMAINDER);
}

void CCarvingView::OnViewComponent()
{
	// TODO: 在此添加命令处理程序代码
	SwitchView(VIEW_PANEL_LIST);
}


void CCarvingView::OnViewSolution()
{
	// TODO: 在此添加命令处理程序代码
	SwitchView(VIEW_RESULT);
}

void CCarvingView::SwitchView(ViewType eViewType)
{
	//if(eViewType == m_eCurViewType)
	//	return;

	switch(eViewType)
	{
	case VIEW_REMAINDER:
		if(m_pDlgRemainder == NULL || m_pDlgRemainder->GetSafeHwnd() == NULL)
		{
			delete m_pDlgRemainder;
			m_pDlgRemainder = new CDlgRemainder(this);
			m_pDlgRemainder->Create(CDlgRemainder::IDD, this);
		}
		if(m_pDlgRequirement != NULL && m_pDlgRequirement->GetSafeHwnd())
			m_pDlgRequirement->ShowWindow(SW_HIDE);
		if(m_pDlgResult != NULL && m_pDlgResult->GetSafeHwnd())
			m_pDlgResult->ShowWindow(SW_HIDE);
		m_pDlgRemainder->MoveWindow(GetViewRect());
		m_pDlgRemainder->ShowWindow(SW_SHOW);
		m_eCurViewType = VIEW_REMAINDER;
		m_Tab.SetTab(VIEW_REMAINDER);
		break;

	case VIEW_PANEL_LIST:
		if(m_pDlgRequirement == NULL || m_pDlgRequirement->GetSafeHwnd() == NULL)
		{
			delete m_pDlgRequirement;
			m_pDlgRequirement = new CDlgRequirement(this);
			m_pDlgRequirement->Create(CDlgRequirement::IDD, this);

		}
		if(m_pDlgRemainder != NULL && m_pDlgRemainder->GetSafeHwnd())
			m_pDlgRemainder->ShowWindow(SW_HIDE);
		if(m_pDlgResult != NULL && m_pDlgResult->GetSafeHwnd())
			m_pDlgResult->ShowWindow(SW_HIDE);
		m_pDlgRequirement->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
		m_pDlgRequirement->ShowWindow(SW_SHOW);
		m_eCurViewType = VIEW_PANEL_LIST;
		m_Tab.SetTab(VIEW_PANEL_LIST);
		break;

	case VIEW_RESULT:
		if(m_pDlgResult == NULL || m_pDlgResult->GetSafeHwnd() == NULL)
		{
			delete m_pDlgResult;
			m_pDlgResult = new CDlgResult(this);
			m_pDlgResult->Create(CDlgResult::IDD, this);

		}
		if(m_pDlgRemainder != NULL && m_pDlgRemainder->GetSafeHwnd())
			m_pDlgRemainder->ShowWindow(SW_HIDE);
		if(m_pDlgRequirement != NULL && m_pDlgRequirement->GetSafeHwnd())
			m_pDlgRequirement->ShowWindow(SW_HIDE);
		m_pDlgResult->ShowWindow(SW_SHOW);
		m_pDlgResult->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
		m_eCurViewType = VIEW_RESULT;
		m_Tab.SetTab(VIEW_RESULT);
		break;
	}
}

CRect CCarvingView::GetViewRect()
{
	CRect rcRet;
	CRect rcClient, rcTab;
	GetClientRect(rcClient);
	m_Tab.GetWindowRect(rcTab);
	ScreenToClient(rcTab);
	rcRet.CopyRect(rcClient);
	rcRet.InflateRect(0, -rcTab.Height(), 0, 0);
	return rcRet;
}

void CCarvingView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pDlgRemainder != NULL && m_pDlgRemainder->GetSafeHwnd())
		m_pDlgRemainder->MoveWindow(GetViewRect());
	if(m_pDlgRequirement != NULL && m_pDlgRequirement->GetSafeHwnd())
		m_pDlgRequirement->MoveWindow(GetViewRect());
	if(m_pDlgResult != NULL && m_pDlgResult->GetSafeHwnd())
		m_pDlgResult->MoveWindow(GetViewRect());
}


void CCarvingView::OnOptimize()
{
	// TODO: 在此添加命令处理程序代码



	// 获取界面数据
	UpdateData(TRUE);

	// 拷贝需要优化的板件，不直接处理需要优化的板件，避免原始数据遭到破坏
	vector<ComponentInputItem> vOptimizeComponent = m_vComponentInputItem;

	// 检测板件超出
	CheckAndDeleteOverSizeComponentList(vOptimizeComponent);
	if(!CheckDeepHoleSlot(vOptimizeComponent))
		return;

	// 判断是否有优化数据
	if (vOptimizeComponent.size() == 0)
	{
		AfxMessageBox("没有可优化的板件！");
		return;
	}

	if(IsRepeatBarcode(vOptimizeComponent))
	{
		AfxMessageBox("请重新混排订单或者手动修改条码编号。");
		return;
	}
	
	// 获取余料信息
	vector<RemainderInputItem> vRemainderInputItem = m_pDlgRemainder->GetRemainderInputItem();

	SwitchView(VIEW_RESULT);

	CSingleon* pSingleton = CSingleon::GetSingleton();

	// 设置优化相关信息

	//pSingleton->ClearAllData();
	//((CMainFrame*)theApp.GetMainWnd())->GetAlgBaseInfo(pSingleton->m_BaseInfo);


	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	COPSetDlg dlgOptimizeSetting;
	dlgOptimizeSetting.loadSettings();
	
	
	pSingleton->m_BaseInfo.m_PanelLength = param_dlg->m_width;				// 长度
	pSingleton->m_BaseInfo.m_PanelWidth = param_dlg->m_height;				// 宽度
	pSingleton->m_BaseInfo.m_SawKerfWidth = param_dlg->m_Kerfwidth;			// 锯缝
	pSingleton->m_BaseInfo.m_DeburringWidth = param_dlg->m_Deburringwidth;	// 修边
	pSingleton->m_BaseInfo.m_LayoutOrg = param_dlg->m_arranging_origin;		// 排样原点

	// 第一阶段参数
	pSingleton->m_BaseInfo.m_FirstSectionOPTimes = dlgOptimizeSetting.m_nEditStep1Count;
	pSingleton->m_BaseInfo.m_FirstSectionOPMethod = dlgOptimizeSetting.m_nComboStep1Alg;
	// 第二阶段参数
	pSingleton->m_BaseInfo.m_SecondSectionOPTimes = dlgOptimizeSetting.m_nEditStep2Count;
	// 第三阶段参数
	pSingleton->m_BaseInfo.m_ThirdSectionOPTimes = dlgOptimizeSetting.m_nEditStep3Count;
	pSingleton->m_BaseInfo.m_ThirdSectionOAccptableUtilization = dlgOptimizeSetting.m_fEditStep3AcceptableUti;

	// 反面信息优先排样
	pSingleton->m_BaseInfo.m_bDownerFaceFirst = dlgOptimizeSetting.m_valDownerInfoFirst;


	// 保存原始板件数据
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

	// 清空所有数据,准备优化
	ClearAllData();

	// 阻塞式弹出优化窗口，窗口中使用线程优化
	OptimizeDlg OpDlg;

	OpDlg.m_vComponentInputItem = vOptimizeComponent;
	OpDlg.m_vPreCombineItem = m_pDlgRequirement->GetPreCombineItems();
	OpDlg.m_vRemainderInputItem = vRemainderInputItem;

	OpDlg.DoModal();

	// 此时优化已完成，解决方案，需要根据优化率从高到低排序
	for (int i_sln = 0; i_sln < pSingleton->m_BackupSolutionList.size(); i_sln++)
	{
		CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

		pSln->SortPanelList();
	}
	


	// 更新解决方案ID和板件ID
	pSingleton->UpdateSlnNum();

	//更新板件轮廓点、孔槽等加工信息（排板过程中可能有的板件被旋转了，所以排板之后被旋转过的板件的轮廓点、孔槽也要随之旋转一下）
	pSingleton->UpdateComponentMachiningInfo();
	pSingleton->UpdatePreCombinedComponent();

	// 设置下刀点
	CParamDlg paramdlg;
	paramdlg.loadSettings();

	//小板切两刀
	bool bCutTwice_Enable = paramdlg.LdoublecutEnable();
	float fCutTwice_SmallSize = paramdlg.Minarea();
	float fCutTwice_ReserveHeight = paramdlg.Keepthickness();
	float fCutTwice_SecondSpeed = paramdlg.Secondspeedrate();

	//分两刀切
	float fTwoCutThreshold = paramdlg.Twocut_threshold();
	float fTwoCutReserve = paramdlg.Twocut_reserve();
	float fTwoCutSpeed = paramdlg.Twocut_workSpeed();


	KnifeClass* pKnife = pSingleton->m_pKnifeClass;
	for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
	{
		CSolution* pTheSolution = pSingleton->m_BackupSolutionList.at(i_sln);


		// 基本信息
		pKnife->setKerfwidth(pTheSolution->m_BaseInfo.m_SawKerfWidth);		//锯缝
		pKnife->setPanelWidth(pTheSolution->m_BaseInfo.m_PanelLength);		//大板宽度
		pKnife->setPanelHeight(pTheSolution->m_BaseInfo.m_PanelWidth);		//大板长度
		pKnife->setThickness(pTheSolution->m_fThickness);					//大板厚度					
		pKnife->setDeburringwidth(paramdlg.Deburringwidth());				//修边距离				
		pKnife->SetInputOrderNum("a1");										//订单号

		string strThicknessAndMetrial;
		CString cstrThicknessAndMetrial = GetFloatString(pTheSolution->m_fThickness, 1) + "-" + pTheSolution->m_strMaterial;
		strThicknessAndMetrial = cstrThicknessAndMetrial.GetBuffer();
		cstrThicknessAndMetrial.ReleaseBuffer();

		pKnife->setThinMatrial(strThicknessAndMetrial);						//大板厚度 + 名称
		pKnife->setPanelMatrial(pTheSolution->m_strMaterial.GetBuffer());	//大板名称
		pTheSolution->m_strMaterial.ReleaseBuffer();


		//设置G码参数
		//kc->setGcodeParam(gcodeparam);		



		// 两刀切参数 //小板最小尺寸 //预雕保留厚度 //第二刀加工速率
		pKnife->SetLeftParam(fCutTwice_SmallSize,fCutTwice_ReserveHeight,fCutTwice_SecondSpeed);

		// 设置下刀点
		//for(int i_panel = 0; i_panel < pTheSolution->GetPanelNum(); i_panel++)
		//{
		//	Panel* pThePanel = pTheSolution->GetPanel(i_panel);

			//pKnife->SetKnifeDownPos(pThePanel);
		//}
	}
	CalcKnifeDowPos();


	m_pDlgResult->ResetResultDlg();


}


void CCarvingView::OnSave()
{
#if 1

	// TODO: 在此添加命令处理程序代码
	CString strFilePath;

#ifndef YiWei
	CString filter = "hgo 文件(*.hgo)|*.hgo|所有文件 (*.*)|*.*||";
	CString strDefFileName = GetFileNameInPath(m_vOpenedFile[m_vOpenedFile.size()-1]) + _T(".hgo");
	CFileDialog fileDlg (FALSE, _T("hgo"), /*_T("HG001.hgo")*/strDefFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
#else
	CString filter = "ywo 文件(*.ywo)|*.ywo|所有文件 (*.*)|*.*||";
	CString strDefFileName = GetFileNameInPath(m_vOpenedFile[m_vOpenedFile.size()-1]) + _T(".ywo");
	CFileDialog fileDlg (FALSE, _T("ywo"), /*_T("HG001.hgo")*/strDefFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
#endif

	CString strDefDir = SelectPathDlg().DefaultSavePath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	
	if ( fileDlg.DoModal() == IDOK)
	{
		strFilePath = fileDlg.GetPathName();
		
		if (HgyReadWrite::WriteHgy(strFilePath) == true)
		{
			AfxMessageBox("文件保存成功！");
		}
		else
		{
			AfxMessageBox("文件保存失败！");
		}
	}

#else

	CString strFilePath;

	CString filter = "dxf 文件(*.dxf)|*.dxf|所有文件 (*.*)|*.*||";
	CString strDefFileName = GetFileNameInPath(m_vOpenedFile[m_vOpenedFile.size()-1]) + _T(".dxf");
	CFileDialog fileDlg (FALSE, _T("dxf"), /*_T("HG001.hgo")*/strDefFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultSavePath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;

	if ( fileDlg.DoModal() == IDOK)
	{
		strFilePath = fileDlg.GetPathName();

		if (DxfReadWrite::OutputDownerFaceDxf(strFilePath) == true)
		{
			AfxMessageBox("文件保存成功！");
		}
		else
		{
			AfxMessageBox("文件保存失败！");
		}
	}


#endif




}


void CCarvingView::OnEditPragmaSawKerf()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnEditPanelLength()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnEditPanelWidth()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnEditStep1Count()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnEditStep2Count()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnEditStep3Count()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnEditStep3AcceptableUti()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnComboStep1Alg()
{
	// TODO: 在此添加命令处理程序代码
}



// 过滤一个字符串中的中文
void DelChineseInString(string& str)
{
	int i, nCount;

	nCount = str.size();
	for(int i = 0; i < nCount; i++)
	{
		char c = str[i];

		//不是全角字符？
		if( c >= 0 && c <= 127 ) 
		{
			// 英文
		}
		else 
		{
			//是全角字符 中文
			str.erase(str.begin()+i);
			str.erase(str.begin()+i);

			return DelChineseInString(str);
		}
	}

}

bool CCarvingView::PrepareExportGCode(PrepareExportGCodeData* pPrepareExportGCodeData)
{
	
	pPrepareExportGCodeData->_knife_shifting = pPrepareExportGCodeData->pParamDlg->Knife_shifting();

	//小板切两刀
	pPrepareExportGCodeData->bCutTwice_Enable = pPrepareExportGCodeData->pParamDlg->LdoublecutEnable();
	pPrepareExportGCodeData->fCutTwice_SmallSize = pPrepareExportGCodeData->pParamDlg->Minarea();
	pPrepareExportGCodeData->fCutTwice_ReserveHeight = pPrepareExportGCodeData->pParamDlg->Keepthickness();
	pPrepareExportGCodeData->fCutTwice_SecondSpeed = pPrepareExportGCodeData->pParamDlg->Secondspeedrate();

	//分两刀切
	pPrepareExportGCodeData->fTwoCutThreshold = pPrepareExportGCodeData->pParamDlg->Twocut_threshold();
	pPrepareExportGCodeData->fTwoCutReserve = pPrepareExportGCodeData->pParamDlg->Twocut_reserve();
	pPrepareExportGCodeData->fTwoCutSpeed = pPrepareExportGCodeData->pParamDlg->Twocut_workSpeed();

	//CSelMachineDlg selectMachine;
	if (pPrepareExportGCodeData->m_pSelMachineDlg->DoModal() == IDOK)
	{
		pPrepareExportGCodeData->node_name = pPrepareExportGCodeData->m_pSelMachineDlg->m_nodename;
		pPrepareExportGCodeData->node_print = atoi(pPrepareExportGCodeData->m_pSelMachineDlg->m_printname.c_str());
		pPrepareExportGCodeData->node_sysuse = pPrepareExportGCodeData->m_pSelMachineDlg->_coorsysuse;
		pPrepareExportGCodeData->node_double = pPrepareExportGCodeData->m_pSelMachineDlg->_cutdouble;
		pPrepareExportGCodeData->nodecoor_type = pPrepareExportGCodeData->m_pSelMachineDlg->_coortype;//单工位坐标系(默认)
		pPrepareExportGCodeData->nodecoor_type1 = pPrepareExportGCodeData->m_pSelMachineDlg->_coortype;
		pPrepareExportGCodeData->nodecoor_type2 = pPrepareExportGCodeData->m_pSelMachineDlg->_coortype2;//双工位坐标系
		pPrepareExportGCodeData->node_groupstr = pPrepareExportGCodeData->m_pSelMachineDlg->_groupstr;

		pPrepareExportGCodeData->node_pname = pPrepareExportGCodeData->m_pSelMachineDlg->m_pname;
		pPrepareExportGCodeData->node_plast = pPrepareExportGCodeData->m_pSelMachineDlg->m_plast;
		pPrepareExportGCodeData->node_cname = pPrepareExportGCodeData->m_pSelMachineDlg->m_cname;
		pPrepareExportGCodeData->node_clast = pPrepareExportGCodeData->m_pSelMachineDlg->m_clast;
		pPrepareExportGCodeData->node_dvcut = pPrepareExportGCodeData->m_pSelMachineDlg->m_dvcut;//0,单面打孔，1双面打孔1（W）,2双面打孔2（H）
		pPrepareExportGCodeData->node_pxhx = pPrepareExportGCodeData->m_pSelMachineDlg->m_pxhx;
		pPrepareExportGCodeData->node_cxhx = pPrepareExportGCodeData->m_pSelMachineDlg->m_cxhx;

		_trim1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim1;
		_trim2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim2;
		_trim3 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim3;
		_trim4 = pPrepareExportGCodeData->m_pSelMachineDlg->m_trim4;
		_ftrim1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim1;
		_ftrim2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim2;
		_ftrim3 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim3;
		_ftrim4 = pPrepareExportGCodeData->m_pSelMachineDlg->m_ftrim4;

		_iscombine = pPrepareExportGCodeData->m_pSelMachineDlg->m_iscombine;							//	是否合并正反头尾
		pPrepareExportGCodeData->_isComline = pPrepareExportGCodeData->m_pSelMachineDlg->m_iscomline;							//	是否连接NC文件
		pPrepareExportGCodeData->_isComball = pPrepareExportGCodeData->m_pSelMachineDlg->m_iscomball;							//	是否合并所有NC文件
		pPrepareExportGCodeData->_isLabelInReverseSide = pPrepareExportGCodeData->m_pSelMachineDlg->m_bLabelInReverseSide;	//	是否标签贴在反面

		pPrepareExportGCodeData->_lessKnifeChange = pPrepareExportGCodeData->m_pSelMachineDlg->m_lessKnifeChange;

		CString theUpHeigt;//获取安全高度
		theUpHeigt.Format(_T("%0.1f"),pPrepareExportGCodeData->pParamDlg->Upheight());
		std::string theUPH = theUpHeigt.GetBuffer();

		CString theOverSpeed;//获取空跑速度
		theOverSpeed.Format(_T("%0.1f"),pPrepareExportGCodeData->pParamDlg->Overspeed());
		std::string theFs = theOverSpeed.GetBuffer();

		_header = pPrepareExportGCodeData->m_pSelMachineDlg->m_header;
		//ChangeHeader(_header,theUPH,theFs);
		_ender = pPrepareExportGCodeData->m_pSelMachineDlg->m_ender;
		// ChangeEnder(_ender,theUPH,theFs);
		_GFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GFirst2;//工位2文件头
		_GLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GLast2;//工位2文件尾
		//_cutdouble = true双工位，_cutdouble = false单工位
		pPrepareExportGCodeData->_cutDouble = pPrepareExportGCodeData->m_pSelMachineDlg->_cutdouble;
		_MFirst1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MFirst1;
		_MLast1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MLast1;
		_MFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MFirst2;
		_MLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_MLast2;

		_PZFirst1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZFirst1;
		_PZLast1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZLast1;
		_PZFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZFirst2;
		_PZLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_PZLast2;

		_GBFirst1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBFirst1;//工位1正面文件头
		_GBFirst2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBFirst2;//工位2正面文件头
		_GBLast1 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBLast1;//工位1反面文件尾
		_GBLast2 = pPrepareExportGCodeData->m_pSelMachineDlg->m_GBLast2;//工位2反面文件尾
	}
	else
	{
		return false;
	}

	/*std::string temp_header1,temp_ender1,temp_header2,temp_ender2,temp_GFirst2_1,temp_GLast2_1,temp_GFirst2_2,temp_GLast2_2,temp_GBFirst1_1,temp_GBLast1_1,
	temp_GBFirst1_2,temp_GBLast1_2,temp_GBFirst2_1,temp_GBLast2_1,temp_GBFirst2_2,temp_GBLast2_2;*/
	{
		pPrepareExportGCodeData->temp_header1 = _header;//保存临时文件头1
		pPrepareExportGCodeData->temp_ender1 = _ender;//保存临时文件尾1
		DeleteSpecialCode(pPrepareExportGCodeData->temp_header1);//删除整条
		DeleteSpecialCode(pPrepareExportGCodeData->temp_ender1);
	
		
		pPrepareExportGCodeData->temp_header2 = _header;//保存临时文件头2
		pPrepareExportGCodeData->temp_ender2 = _ender;//保存临时文件尾2
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_header2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_ender2);
	    

		pPrepareExportGCodeData->temp_GFirst2_1 = _GFirst2;//正面工位2，2种临时头尾
		pPrepareExportGCodeData->temp_GLast2_1 = _GLast2;
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GFirst2_1);
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GLast2_1);

		pPrepareExportGCodeData->temp_GFirst2_2 = _GFirst2;
		pPrepareExportGCodeData->temp_GLast2_2 = _GLast2;
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GFirst2_2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GLast2_2);

		pPrepareExportGCodeData->temp_GBFirst1_1 = _GBFirst1;//反面工位1，2种临时头尾
		pPrepareExportGCodeData->temp_GBLast1_1 = _GBLast1;
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBFirst1_1);
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBLast1_1);

		pPrepareExportGCodeData->temp_GBFirst1_2 = _GBFirst1;
		pPrepareExportGCodeData->temp_GBLast1_2 = _GBLast1;
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBFirst1_2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBLast1_2);

		pPrepareExportGCodeData->temp_GBFirst2_1 = _GBFirst2;//反面工位2，2种临时头尾
		pPrepareExportGCodeData->temp_GBLast2_1 = _GBLast2;
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBFirst2_1);
		DeleteSpecialCode(pPrepareExportGCodeData->temp_GBLast2_1);

		pPrepareExportGCodeData->temp_GBFirst2_2 = _GBFirst2;
		pPrepareExportGCodeData->temp_GBLast2_2 = _GBLast2;
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBFirst2_2);
		DeleteSpecialCode2(pPrepareExportGCodeData->temp_GBLast2_2);
	}

	if (pPrepareExportGCodeData->nodecoor_type < 0)
	{
		pPrepareExportGCodeData->other_str = 1;
		pPrepareExportGCodeData->nodecoor_type = abs(pPrepareExportGCodeData->nodecoor_type);
	}

	if (pPrepareExportGCodeData->node_name == "")
	{
		return false;
	}

	//CString pathName;

	SelectPathDlg pathDlg;
	pathDlg.CurrentPath(pathDlg.DefaultSavePath());
	if (pathDlg.DoModal() == IDCANCEL)
		return false;

	pPrepareExportGCodeData->pathName = pathDlg.CurrentPath();
	//pathName = pathName+"\\nc";
	//HGTools::deletePath(pathName);
	//HGTools::createDir(pathName);

	NCSavePath = pPrepareExportGCodeData->pathName;
	CString pathCut = pPrepareExportGCodeData->pathName;
	pPrepareExportGCodeData->pathPrint = pPrepareExportGCodeData->pathName;

	//创建对应的目录
	//对应的 NC 文件移动到当前目录的 cut目录 下
	//对应的 TB贴标 文件移动到当前目录的 print目录 下
	if (pPrepareExportGCodeData->m_pSelMachineDlg->m_fileClassifyMove == "1")
	{
		//生成的时候就把全部文件放到cut目录，
		//独立把贴标文件拷贝到print目录

		pathCut = NCSavePath + "\\cut";
		pPrepareExportGCodeData->pathName = pathCut;
		pPrepareExportGCodeData->pathPrint = NCSavePath + "\\print";

		if (!HGTools::isDir(pathCut))
		{
			CreateDirectory(pathCut,NULL);
		}

		if (!HGTools::isDir(pPrepareExportGCodeData->pathPrint))
		{
			CreateDirectory(pPrepareExportGCodeData->pathPrint,NULL);
		}
	}

	return true;
}

void CCarvingView::PrepareKnifeInfoAndHoleSetting(PrepareExportGCodeData* pPrepareExportGCodeData, PrepareKnifeInfoData* pPrepareKnifeInfoData)
{
	if (1)
	{
		std::vector<std::map<std::string,std::string>> knifes = pPrepareExportGCodeData->m_pSelMachineDlg->getMachineKnifes();

		KnifeType tempkt;//0,5,4中刀长最合适的刀
		tempkt.kt = -1;
		int ktnum_kl = 0;

		//获取机械刀具信息
		for (int knife_i = 0;knife_i < knifes.size();knife_i++)
		{
			std::map<std::string,std::string> cur_knife = knifes[knife_i];


			//读取数据
			std::string str_Index			= cur_knife["Index"];
			std::string str_TypeName		= cur_knife["TypeName"];
			std::string str_Diameter		= cur_knife["Diameter"];
			std::string str_Length			= cur_knife["Length"];
			std::string str_ChangeCode		= cur_knife["ChangeCode"];
			std::string str_FeedRate		= cur_knife["FeedRate"];
			std::string str_SpindleSpeed	= cur_knife["SpindleSpeed"];
			std::string str_EndSpeed		= cur_knife["EndSpeed"];
			std::string str_OverSpeed		= cur_knife["OverSpeed"];
			std::string str_kcoor			= cur_knife["Kcoor"];
			std::string str_knum			= cur_knife["KNum"];
			std::string str_CoorSys			= cur_knife["CoorSys"];
			std::string str_KnifeLength		= cur_knife["KnifeLength"];
			std::string str_NearSpeed		= cur_knife["NearSpeed"];
			std::string str_ischeck			= cur_knife["Used"];//判断刀具是否使用

			if (pPrepareExportGCodeData->node_double == true && pPrepareExportGCodeData->key%2 == 0)
			{
				str_ChangeCode	= cur_knife["ChangeCode1"];
				str_CoorSys		= cur_knife["CoorSys1"];
			}

			//组织工位开始结束代码
			std::string str_ChangeCodeB = "";
			std::string str_ChangeCodeE = "";
			bool IsBegin = true;
			for (int c_i = 0;c_i < str_ChangeCode.size();c_i++)
			{
				if (str_ChangeCode.at(c_i) == '#')
				{
					IsBegin = false;
					continue;
				}
				if (IsBegin == true)
				{
					str_ChangeCodeB = str_ChangeCodeB + str_ChangeCode.at(c_i);
				}
				else
				{
					str_ChangeCodeE = str_ChangeCodeE + str_ChangeCode.at(c_i);
				}
			}

			std::string str_kx = "";
			std::string str_ky = "";
			IsBegin = true;
			for (int c_i = 0;c_i < str_kcoor.size();c_i++)
			{
				if (str_kcoor.at(c_i) == ',')
				{
					IsBegin = false;
					continue;
				}
				if (IsBegin == true)
				{
					str_kx = str_kx + str_kcoor.at(c_i);
				}
				else
				{
					str_ky = str_ky + str_kcoor.at(c_i);
				}
			}
			if (str_kx == "")
			{
				str_kx = "0";
			}
			if (str_ky == "")
			{
				str_ky = "0";
			}

			KnifeType onekt;

			onekt._diameter		= atof(str_Diameter.c_str());
			onekt._feedrate		= atof(str_FeedRate.c_str());
			onekt._spindlespeed = atof(str_SpindleSpeed.c_str());
			onekt._tgcodeb		= str_ChangeCodeB;;
			onekt._tgcodee		= str_ChangeCodeE;
			onekt.kt			= atoi(str_TypeName.c_str());
			onekt._length		= atof(str_Length.c_str());
			onekt._endspeed		= atof(str_EndSpeed.c_str());
			onekt._overspend	= atof(str_OverSpeed.c_str());
			onekt._ky			= atof(str_kx.c_str());
			onekt._kx			= atof(str_ky.c_str());
			onekt._kn			= atoi(str_knum.c_str());
			onekt._coorsys		= str_CoorSys;
			onekt._kinfelength	= atof(str_KnifeLength.c_str());
			onekt._facespeed	= atof(str_NearSpeed.c_str());



			if ((	onekt.kt == KnifeType_Spindle_Hole_Slot_Material
				||	onekt.kt == KnifeType_Spindle_Material
				||	onekt.kt == KnifeType_Spindle_Slot_Material)
				&&(str_ischeck!="0"))//需要判断刀具长度
			{
				float thickness = /*kc->getThickness()*/pPrepareKnifeInfoData->pThePanel->m_Thickness;
				if (thickness>onekt._kinfelength)
				{
					if (ktnum_kl==0&&onekt._kinfelength==0)
					{
						tempkt = onekt;
						ktnum_kl++;
					}
				}
				else//刀具长度大于板件厚度
				{
					if (ktnum_kl>0)
					{
						//KnifeType tempKt = tempkt;
						if (tempkt._kinfelength>onekt._kinfelength||tempkt._kinfelength==0)
						{
							tempkt = onekt;
						}
						//kt.push_back(onekt);
					}
					else
					{
						tempkt = onekt;
						ktnum_kl++;
					}
				}
			}
			else
			{
				if (str_ischeck!="0")
				{
					pPrepareKnifeInfoData->kt.push_back(onekt);
				}

			}
		}

		//yuanzb 2017-6-23  开料刀一定是排最后的而且只能是一把 
		if (tempkt.kt != -1)
		{
			pPrepareKnifeInfoData->kt.push_back(tempkt);//将合适的开料刀传入
		}

		pPrepareExportGCodeData->pKnifeClass->setKnifeType(pPrepareKnifeInfoData->kt);
	}

	vector<float> _cxy = pPrepareExportGCodeData->pParamDlg->GetVecFilter();
	pPrepareExportGCodeData->pKnifeClass->setvecchangexy(_cxy);


	float _changex = pPrepareExportGCodeData->pParamDlg->MoveX();
	float _changey = pPrepareExportGCodeData->pParamDlg->MoveY();

	pPrepareExportGCodeData->pKnifeClass->setvecchangexy(_changex,_changey);
}

void CCarvingView::AfterExportGCode_TransCoordinate(PrepareExportGCodeData* pPrepareExportGCodeData, TransCoordinateData* pTransCoordinateData)
{

	//再次打开NC代码 path 正面
#if 1
	try
	{
		// 第一次 

		//L4Cpp::Log()->debug("6.coor change");
		/*******************************************正面孔槽生成**************************************************/
		if (pPrepareExportGCodeData->nodecoor_type == 0)//基准原点的情况
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);

			//L4Cpp::Log()->debug("6.coor ofstream patho");
			//L4Cpp::Log()->debug(patho.c_str());

			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);

			//L4Cpp::Log()->debug("6.coor ifstream path");
			//L4Cpp::Log()->debug(path.c_str());

			try
			{
				while(!rfile.eof())
				{	
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//other_y对应现在X
						{
							ischangex = false;
							float other_y;
							if (pTransCoordinateData->HaveDvs)//有修边，坐标系0时候的X
							{
								other_y = atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;//X轴偏移
							}
							else
								other_y =atof(tmp_stry.c_str());


							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							if (pTransCoordinateData->HaveDvs)//有修边，坐标系0时候的Y
							{
								other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;//Y轴偏移
							}
							else
								other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
			}
			catch (...)
			{
				//L4Cpp::Log()->debug("6.coor catch while(!rfile.eof())");
			}

			//wfile<<endl;

			//L4Cpp::Log()->debug("6.coor wfile<<endl; close");

			wfile.close();
			rfile.close();

			//L4Cpp::Log()->debug("6.coor DeleteFile");

			DeleteFile(pTransCoordinateData->nc_font_path.c_str());

			//L4Cpp::Log()->debug("6.coor rename");

			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());


			//L4Cpp::Log()->debug("6.coor eachLineAddChar");
			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(0,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 1)	//坐标原点在左下角，机器坐标原点以左上角为准
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_header;//新加
			//}					
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
					{
						ischangex = false;
						float other_y;



						if (pTransCoordinateData->HaveDvs)//有修边
						{
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
						}
						else
						{
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
						}

						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边//y
						{
							other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}				
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(1,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 2)//坐标原点在右上角（往上为正Y，往右为正X）
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_header;//新加
			//}
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//有反面修边//X
						{
							other_y = -1.0 *(atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX);
						}
						else
							other_y = -1.0 * atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边
						{
							other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}		
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(2,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 3)//坐标原点在左下角
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_header;//新加
			//}	
			while(!rfile.eof())
			{	
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//有修边,坐标系3时候的X
						{
							other_y = atof(tmp_stry.c_str())+pTransCoordinateData->_usedLineX;
							other_y = other_y - pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						}
						else
							other_y =atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边
						{
							other_x = atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}	
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(3,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 4)
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			while(!rfile.eof())
			{	
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
					{
						ischangex = false;
						float other_y;//交换的时候相当于X，不交换的话相当于Y
						if (pTransCoordinateData->HaveDvs)//temp_stry为X1,temp_strx为Y1；
						{
							other_y = atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
						}
						else
							other_y =atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边，坐标系0时候的Y
						{
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str())+pTransCoordinateData->_usedLineY;//Y轴偏移
						}
						else
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (1)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}	
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(0,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 5)	
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_header;//新加
			//}					
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//有修边X
						{
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight()-atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;

						}
						else
							other_y =  pPrepareExportGCodeData->pKnifeClass->getPanelHeight()-atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//y
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边
						{
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}				
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			//MovePositonInDifCordinate(1,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 6)
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_header;//新加
			//}
			while(!rfile.eof())
			{
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//有反面修边
						{
							other_y = -atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
						}
						else
							other_y = -atof(tmp_stry.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边
						{
							other_x = -1.0 *(pPrepareExportGCodeData->pKnifeClass->getPanelWidth()-atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY);

						}
						else
							other_x = -1.0 *(pPrepareExportGCodeData->pKnifeClass->getPanelWidth()-atof(tmp_strx.c_str()));
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}		
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);

			//MovePositonInDifCordinate(2,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 7)//坐标原点在左下角
		{
			std::ofstream wfile(pTransCoordinateData->nc_font_temp_path.c_str(),std::ios::out);
			std::ifstream rfile(pTransCoordinateData->nc_font_path.c_str(),std::ios::in);
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_header;//新加
			//}	
			while(!rfile.eof())
			{	
				char str[1023];
				rfile.getline(str,1023);
				std::string get_str = str;
				std::string new_str = "";
				std::string tmp_strx = "";
				std::string tmp_stry = "";
				bool ischangex = false;
				bool ischangey = false;
				bool isCanChange = false;
				if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
					get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
				{
					isCanChange = true;
				}
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == true && get_str.at(g) != ' ')
					{
						tmp_stry.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
					{
						ischangex = false;
						float other_y;
						if (pTransCoordinateData->HaveDvs)//有修边,坐标系3时候的X
						{
							other_y = atof(tmp_stry.c_str())-pTransCoordinateData->_usedLineX;
							other_y = other_y - pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						}
						else
							other_y =atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_y);
						tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
						tmp_stry = tmp_s.GetBuffer();
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						tmp_strx.push_back(get_str.at(g));
					}
					if (ischangey == true && get_str.at(g) == ' ')//X = y0;
					{
						ischangey = false;
						float other_x;
						if (pTransCoordinateData->HaveDvs)//有修边
						{
							other_x = -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str())-pTransCoordinateData->_usedLineY;
						}
						else
							other_x =  -pPrepareExportGCodeData->pKnifeClass->getPanelWidth()+atof(tmp_strx.c_str());
						CString tmp_s;
						//tmp_s.Format("%0.4f",other_x);
						tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
						tmp_strx = tmp_s.GetBuffer();
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						g++;
					}

				}
				ischangex = false;
				ischangey = false;
				for (int g = 0;g < get_str.size();g++)
				{
					if (ischangex == false && ischangey == false)
					{
						new_str.push_back(get_str.at(g));
					}
					if (ischangex == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangex == true && get_str.at(g) == ' ')
					{
						ischangex = false;
						new_str = new_str + tmp_stry;
						new_str.push_back(' ');
					}
					if (ischangey == true && get_str.at(g) != ' ')
					{
						continue;
					}
					if (ischangey == true && get_str.at(g) == ' ')
					{
						ischangey = false;
						new_str = new_str + tmp_strx;
						new_str.push_back(' ');
					}
					if (get_str.at(g) == 'Y'&&isCanChange == true)
					{
						ischangey = true;
						new_str.push_back(' ');
						g++;
					}
					if (get_str.at(g) == 'X'&&isCanChange == true)
					{
						ischangex = true;
						new_str.push_back(' ');
						g++;
					}

				}
				if (new_str.size() > 0)
				{
					if (pPrepareExportGCodeData->other_str == 0)
					{
						wfile<<new_str<<std::endl;
					}
					else
					{
						wfile<<new_str<<";"<<std::endl;
					}
				}
			}
			//if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
			//{
			//	wfile<<_ender;//新加
			//}	
			//wfile<<endl;
			wfile.close();
			rfile.close();
			DeleteFile(pTransCoordinateData->nc_font_path.c_str());
			rename(pTransCoordinateData->nc_font_temp_path.c_str(),pTransCoordinateData->nc_font_path.c_str());

			//处理每行添加内容
			eachLineAddChar(pTransCoordinateData->nc_font_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);

			//MovePositonInDifCordinate(3,0,patho,path,HaveDvs,v_trim,other_str,kc);
		}


		/*******************************************正面孔槽生成**************************************************/

		//L4Cpp::Log()->debug("6.coor change @W");
		//再次打开NC代码 pathd @W
		/*******************************************反面孔槽生成（W翻转方式左右翻转）******************************/
		if (pPrepareExportGCodeData->nodecoor_type == 0)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);		
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加//写入反面文件头
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						char tmp_char = get_str.at(g);

						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 1)//反面孔，坐标原点在左下角，w翻转
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}					
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}			
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 2)//反面孔，坐标原点在右下角
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 3)//坐标原点在左下角
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;//转换后的Y
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}	
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(3,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 4)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);		
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加//写入反面文件头
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 5)//反面孔，坐标原点在左下角，w翻转
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}					
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}			
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);

		}
		if (pPrepareExportGCodeData->nodecoor_type == 6)//反面孔，坐标原点在右下角
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 7)//坐标原点在左下角
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_w_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_w_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())- pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;//转换后的Y
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}	
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_w_path.c_str());
				rename(pTransCoordinateData->nc_back_w_temp_path.c_str(),pTransCoordinateData->nc_back_w_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_w_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(3,1,pathdo,pathd,HaveDvs,v_trimw,other_str,kc);
		}


		/*******************************************反面孔槽生成（W翻转方式左右翻转）******************************/

		//L4Cpp::Log()->debug("6.coor change @H");
		//再次打开NC代码 pathd1 @H
		/*******************************************反面孔槽生成（H翻转方式上下翻转）******************************/
		if (pPrepareExportGCodeData->nodecoor_type == 0)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 1)//h翻转
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}		
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 2)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}	
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 3)//坐标原点在左下角
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}

			//MovePositonInDifCordinate(3,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 4)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位1
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位2
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(0,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 5)//h翻转
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}		
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = pPrepareExportGCodeData->pKnifeClass->getPanelHeight() - atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(1,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 6)
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							float other_y;
							other_y = -1.0 * atof(tmp_stry.c_str());
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}	
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}
			//MovePositonInDifCordinate(2,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}
		if (pPrepareExportGCodeData->nodecoor_type == 7)//坐标原点在左下角
		{
			std::ifstream rfile(pTransCoordinateData->nc_back_h_path.c_str(),std::ios::in);
			if (rfile)
			{
				std::ofstream wfile(pTransCoordinateData->nc_back_h_temp_path.c_str(),std::ios::out);
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件头写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBFirst1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBFirst1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBFirst2<<std::endl;//新加
						}
					}
				}	
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					std::string get_str = str;
					std::string new_str = "";
					std::string tmp_strx = "";
					std::string tmp_stry = "";
					bool ischangex = false;
					bool ischangey = false;
					bool isCanChange = false;
					if (get_str.find("G00")!=std::string::npos||get_str.find("G01")!=std::string::npos||
						get_str.find("G02")!=std::string::npos||get_str.find("G03")!=std::string::npos)
					{
						isCanChange = true;
					}
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == true && get_str.at(g) != ' ')
						{
							tmp_stry.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) == ' ')//转换后的Y = height - x0(基准下的x坐标);
						{
							ischangex = false;
							float other_y;
							other_y = atof(tmp_stry.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelHeight();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_y);
							tmp_s = GetFloatString(other_y, pTransCoordinateData->nDecimal);
							tmp_stry = tmp_s.GetBuffer();
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							tmp_strx.push_back(get_str.at(g));
						}
						if (ischangey == true && get_str.at(g) == ' ')//X = y0;
						{
							ischangey = false;
							float other_x;
							other_x = atof(tmp_strx.c_str())-pPrepareExportGCodeData->pKnifeClass->getPanelWidth();
							CString tmp_s;
							//tmp_s.Format("%0.4f",other_x);
							tmp_s = GetFloatString(other_x, pTransCoordinateData->nDecimal);
							tmp_strx = tmp_s.GetBuffer();
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							g++;
						}

					}
					ischangex = false;
					ischangey = false;
					for (int g = 0;g < get_str.size();g++)
					{
						if (ischangex == false && ischangey == false)
						{
							new_str.push_back(get_str.at(g));
						}
						if (ischangex == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangex == true && get_str.at(g) == ' ')
						{
							ischangex = false;
							new_str = new_str + tmp_stry;
							new_str.push_back(' ');
						}
						if (ischangey == true && get_str.at(g) != ' ')
						{
							continue;
						}
						if (ischangey == true && get_str.at(g) == ' ')
						{
							ischangey = false;
							new_str = new_str + tmp_strx;
							new_str.push_back(' ');
						}
						if (get_str.at(g) == 'Y'&&isCanChange == true)
						{
							ischangey = true;
							new_str.push_back(' ');
							g++;
						}
						if (get_str.at(g) == 'X'&&isCanChange == true)
						{
							ischangex = true;
							new_str.push_back(' ');
							g++;
						}

					}
					if (new_str.size() > 0)
					{
						if (pPrepareExportGCodeData->other_str == 0)
						{
							wfile<<new_str<<std::endl;
						}
						else
						{
							wfile<<new_str<<";"<<std::endl;
						}
					}
				}
				if (!_iscombine||_iscombine==2)//如果不合并时，将文件尾写入
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_GBLast1<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GBLast1<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_GBLast2<<std::endl;//新加
						}
					}
				}
				//wfile<<endl;
				wfile.close();
				rfile.close();
				DeleteFile(pTransCoordinateData->nc_back_h_path.c_str());
				rename(pTransCoordinateData->nc_back_h_temp_path.c_str(),pTransCoordinateData->nc_back_h_path.c_str());

				//处理每行添加内容
				eachLineAddChar(pTransCoordinateData->nc_back_h_path,pPrepareExportGCodeData->m_pSelMachineDlg->m_EachNCLineExtra);
			}

			//MovePositonInDifCordinate(3,2,pathd1o,pathd1,HaveDvs,v_trimh,other_str,kc);
		}

		/*******************************************反面孔槽生成（H翻转方式上下翻转）******************************/
	}
	catch (...)
	{
		MessageBox("NC坐标系偏移错误");
		return;
	}

#endif
}

void CCarvingView::AfterExportGCode_CombineFile(PrepareExportGCodeData* pPrepareExportGCodeData, CombineFileData* pCombineFileData)
{
	try
	{

		//单、双工位正面NC文件合并
		std::string tempend = "";
		std::string tempend2 = "";
		if (pCombineFileData->str_nextthin == "")
		{
			pCombineFileData->tempnextcname2 = "";
		}
		if (pPrepareExportGCodeData->_isComline== 1)//需要连接的时候
		{
			tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
			tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//工位2文件尾带*<>处理
		}
		else//不连接的时候
		{
			if (pCombineFileData->str_nextthin==pCombineFileData->str_thin)//不连接且不是最后一个
			{
				tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
				tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//工位2文件尾带*<>处理
			}
			else
			{
				if(pCombineFileData->tempnextcname2=="")
				{
					tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
					tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//工位2文件尾带*<>处理
				}
				else
				{
					tempend = _ender;
					tempend2 = _GLast2;
				}		
			}
		}

		//   tempend = ReplaceWithNextPath(_ender,pCombineFileData->tempnextcname2);
		//tempend2 = ReplaceWithNextPath(_GLast2,pCombineFileData->tempnextcname2);//工位2文件尾带*<>处理
		{//@W时
			if (pPrepareExportGCodeData->node_dvcut == "0")//单面打孔将文件头尾加入文件
			{
				std::ofstream wfile(pCombineFileData->nc_font_temp_path.c_str(),std::ios::out | std::ios::app );
				std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//正面文件
				if (rfile)
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_header<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GFirst2<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_header<<std::endl;//新加
						}
					}
					while(!rfile.eof())
					{
						char str[1023];
						rfile.getline(str,1023);
						wfile<<str<<std::endl;
					}
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<tempend<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<tempend2<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<tempend<<std::endl;//新加
						}
					}
					//wfile<<endl;
					rfile.close();
					wfile.close();
					DeleteFile(pCombineFileData->nc_font_path.c_str());	
					rename(pCombineFileData->nc_font_temp_path.c_str(),pCombineFileData->nc_font_path.c_str());	
				}	
			}
			if (pPrepareExportGCodeData->node_dvcut=="1")//双面打孔1（w）
			{
				DeleteFile(pCombineFileData->nc_back_h_path.c_str());//删除@H
				std::ofstream wfile(pCombineFileData->nc_font_temp_path.c_str(),std::ios::out | std::ios::app );
				std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//正面文件
				std::ifstream rfile2(pCombineFileData->nc_back_w_path.c_str(),std::ios::in);//反面文件
				if (rfile)
				{

					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_header<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GFirst2<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_header<<std::endl;//新加
						}
					}
					if (rfile2&&_iscombine==1)
					{
						while(!rfile2.eof())
						{
							char str[1023];
							rfile2.getline(str,1023);
							wfile<<str<<std::endl;
						}
					}
					while(!rfile.eof())
					{
						char str[1023];
						rfile.getline(str,1023);
						wfile<<str<<std::endl;
					}

					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<tempend<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<tempend2<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<tempend<<std::endl;//新加
						}
					}

					//wfile<<endl;
					rfile.close();
					rfile2.close();
					wfile.close();
					DeleteFile(pCombineFileData->nc_font_path.c_str());
					if (_iscombine==1)//删除反面文件
					{
						DeleteFile(pCombineFileData->nc_back_w_path.c_str());
					}	
					rename(pCombineFileData->nc_font_temp_path.c_str(),pCombineFileData->nc_font_path.c_str());	
				}						
			}
		}
		{//@H时
			if (pPrepareExportGCodeData->node_dvcut=="2")//双面打孔2（w）
			{
				DeleteFile(pCombineFileData->nc_back_w_path.c_str());//删除@W
				std::ofstream wfile(pCombineFileData->nc_font_temp_path.c_str(),std::ios::out | std::ios::app );
				std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//正面文件
				std::ifstream rfile2(pCombineFileData->nc_back_h_path.c_str(),std::ios::in);//反面文件
				if (rfile)
				{
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<_header<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<_GFirst2<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<_header<<std::endl;//新加
						}
					}
					if (rfile2&&_iscombine==1)
					{
						while(!rfile2.eof())
						{
							char str[1023];
							rfile2.getline(str,1023);
							wfile<<str<<std::endl;
						}
					}
					while(!rfile.eof())
					{
						char str[1023];
						rfile.getline(str,1023);

						// 空行不写
						std::string tmp_str = str;

						if (!tmp_str.empty())
						{
							wfile<<str<<std::endl;
						}
					}
					if (pPrepareExportGCodeData->_cutDouble == false)//单工位
					{
						wfile<<tempend<<std::endl;//新加
					}
					else
					{
						if (pPrepareExportGCodeData->key%2!=0)//工位2
						{
							wfile<<tempend2<<std::endl;//新加
						}
						else//工位1
						{
							wfile<<tempend<<std::endl;//新加
						}
					}
					//wfile<<endl;
					rfile.close();
					rfile2.close();
					wfile.close();
					DeleteFile(pCombineFileData->nc_font_path.c_str());
					if (_iscombine==1)//删除反面文件
					{
						DeleteFile(pCombineFileData->nc_back_h_path.c_str());
					}	
					rename(pCombineFileData->nc_font_temp_path.c_str(),pCombineFileData->nc_font_path.c_str());	
				}	
			}
		}

		//合并所有NC文件到一个
		if (pPrepareExportGCodeData->_isComball==1)
		{


			std::ofstream wfile(pCombineFileData->allpath2.c_str(),std::ios::out | std::ios::app );
			std::ifstream rfile(pCombineFileData->nc_font_path.c_str(),std::ios::in);//正面文件
			if (rfile)
			{
				while(!rfile.eof())
				{
					char str[1023];
					rfile.getline(str,1023);
					wfile<<str<<std::endl;
				}
				//wfile<<endl;
				rfile.close();
				wfile.close();
			}
		}	

	}
	catch (...)
	{
		MessageBox("文件合并 错误");
		return;
	}
}

void CCarvingView::OnExportGcode()
{
	// TODO: 在此添加命令处理程序代码

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CSingleon* pSingleton = CSingleon::GetSingleton();


	if (pMainFrame->m_pDlgBaseParam->m_CheckRemainderLibrary == TRUE)
	{

		RemainderDlg rmd_dlg;

		if (rmd_dlg.DoModal() == IDOK)
		{
			vector<RemainderInputItem> vRemainderInputItem;

			vector<RemainderItem*> all_item_list;
			pSingleton->m_RemainderManager.GetAllItems(all_item_list);

			// 余料赋值
			for(int i_rmd = 0; i_rmd < all_item_list.size(); i_rmd++)
			{
				RemainderItem* pRmd = all_item_list.at(i_rmd);
				RemainderInputItem rmd_item;

				rmd_item.m_nXLen		=	pRmd->m_Length		;
				rmd_item.m_nYLen		=	pRmd->m_Width		;
				rmd_item.m_strMaterial	=	pRmd->m_Material	;
				rmd_item.m_fThickness	=	pRmd->m_Thickness	;
				rmd_item.m_nCount		=	pRmd->m_nCount		;

				vRemainderInputItem.push_back(rmd_item);
			}

			m_pDlgRemainder->SetRemainderInputItem(vRemainderInputItem);


			char ExeFile[2000];
			GetModuleFileName(NULL,ExeFile,500);
			CString file_name = ExeFile;
			file_name = file_name.Left(file_name.ReverseFind('\\')+1) ;


			CString filename = file_name + "RemainderLibrary.hge";
			RemainderLibraryReadWrite::Write(filename.GetBuffer());

		}
	}












#pragma region SaveSceneToLog
	CString strLogDirThisTime = GetLogDir() + GetTimeString();
	MakeSureFolderExist(strLogDirThisTime);

#ifndef YiWei
	CString strSolutionFilePath = strLogDirThisTime + _T("\\Solution.hgo");
#else
	CString strSolutionFilePath = strLogDirThisTime + _T("\\Solution.ywo");
#endif
	HgyReadWrite::WriteHgy(strSolutionFilePath);

	for(int i = 0; i < m_vOpenedFile.size(); i++)
	{
		CString strCopyFullPath;
		strCopyFullPath.Format(_T("%s\\%s"), strLogDirThisTime, GetFileNameInPath_WithSuffix(m_vOpenedFile[i]));
		BOOL bCopySuccess = CopyFile(m_vOpenedFile[i], strCopyFullPath, TRUE);
	}
	RemoveRedundandCoppiedDataFile();
#pragma endregion SaveSceneToLog

	// 保存客户信息
	ClientInfoReadWrite::m_Info.m_OutputNCTimes++ ;
	ClientInfoReadWrite::WriteClientInfo();

	vector<SyntecDataItem> vSyntecData;
	vector<XiaoFengDataItem> vXiaoFengData;
	std::string stringActualLabelNCFilePath;
	map<int, CString> mapPanelSerIDToNCFileFullPath;

	ParamSettingDlg dlgOtherSetting;
	dlgOtherSetting.loadSettings();
	bool bModifyLabelImgNameLikeNCName = dlgOtherSetting.ModifyLabelImgNameLikeNCName();

	


	KnifeClass& knifeClass = *(CSingleon::GetSingleton()->m_pKnifeClass);


	// 提示信息初始化，只提示一块板
	knifeClass.EnaleMsgReport();

	//恩德项目结束板件NC输出信号
	knifeClass.ResetOutputTBNum();	

	CSelMachineDlg selectMachine;
	CParamDlg paramdlg;
	paramdlg.loadSettings();

	PrepareExportGCodeData prepareExportGCodeData;
	prepareExportGCodeData.m_pSelMachineDlg = &selectMachine;
	prepareExportGCodeData.pParamDlg = &paramdlg;
	prepareExportGCodeData.pKnifeClass = &knifeClass;

	bool bPrepareResult = PrepareExportGCode(&prepareExportGCodeData);
	if(!bPrepareResult)
		return;

	// 获取小数位数、板件注释
	ParamSettingDlg settings;
	settings.loadSettings();
	int DecimalPlace = settings.m_valDecimalPlace;
	BOOL PanelCommnets = settings.m_chkboxPanelComments;



	vector<Panel*> total_check_panel = m_pDlgResult->GetPanels(true);
	int total_check_panel_num = total_check_panel.size();
	vector<PanelAndTheSolution> vPanelAndTheSolution;
	GetSolutionPanel(vPanelAndTheSolution);
	if (vPanelAndTheSolution.size() == 0)
	{
		AfxMessageBox(_T("还未做优化计算"));
		return;
	}

	CDlgLabelPosCalc dlgLabelPosCalc(selectMachine.m_nodename.c_str());
	dlgLabelPosCalc.DoModal();

	/////////////////////////////////////////////////////////////////////////////////
	////获得每项的信息，索引是条码
	vector<P_Info> _printinfo;
	for(int i = 0; i < m_vComponentInputItem.size(); i++)
	{
		P_Info temp;
		temp._info = m_vComponentInputItem[i].m_strBarcode;
		temp._content = getPrintInfoAsString(m_vComponentInputItem[i]);
		_printinfo.push_back(temp);
	}

	//if(selectMachine.m_nodename.compare("豪迈") == 0)
	//{
	//	ExportGCode_Homag(vPanelAndTheSolution);
	//	ExportLabelCode(vPanelAndTheSolution);
	//	return;
	//}
	vector<HomagDataItem> vHomagItem;


			// 设置knifeClass信息
			for(int i = 0; i < vPanelAndTheSolution.size(); i++)
			{
				Panel* pThePanel =vPanelAndTheSolution[i].m_pPanel;
				CSolution* pTheSolution = vPanelAndTheSolution[i].m_pSolution;

				// 基本信息设置
				knifeClass.setKerfwidth(pTheSolution->m_BaseInfo.m_SawKerfWidth);
				knifeClass.setPanelWidth(pTheSolution->m_BaseInfo.m_PanelLength);
				knifeClass.setPanelHeight(pTheSolution->m_BaseInfo.m_PanelWidth);
				knifeClass.setThickness(pTheSolution->m_fThickness);
				knifeClass.setDeburringwidth(paramdlg.Deburringwidth());

				string strThicknessAndMetrial;
				CString cstrThicknessAndMetrial = GetFloatString(pTheSolution->m_fThickness, 1) + "-" + pTheSolution->m_strMaterial;
				strThicknessAndMetrial = cstrThicknessAndMetrial.GetBuffer();
				cstrThicknessAndMetrial.ReleaseBuffer();

				knifeClass.setThinMatrial(strThicknessAndMetrial);
				knifeClass.setPanelMatrial(pTheSolution->m_strMaterial.GetBuffer());
				pTheSolution->m_strMaterial.ReleaseBuffer();
				knifeClass.SetInputOrderNum("a1");
				knifeClass.SetLeftParam(prepareExportGCodeData.fCutTwice_SmallSize,//小板最小尺寸
					prepareExportGCodeData.fCutTwice_ReserveHeight,//预雕保留厚度
					prepareExportGCodeData.fCutTwice_SecondSpeed);//第二刀加工速率

				// 设置gcodeparam的信息
				knifeClass.m_gcodeparam.upheight = paramdlg.Upheight();
				knifeClass.m_gcodeparam.downheight = paramdlg.Downheight();
				knifeClass.m_gcodeparam.distance1 = paramdlg.Distance1();
				knifeClass.m_gcodeparam.distance2 = paramdlg.Distance2();
				knifeClass.m_gcodeparam.org_offset = paramdlg.KnifeOrgOffset();

// 
// 				GcodeParam gcodeparam;
// 				gcodeparam.cutspeech = paramdlg.Cutspeed();
// 				gcodeparam.prospeech = paramdlg.Prospeed();
// 				gcodeparam.distance1 = paramdlg.Distance1();
// 				gcodeparam.distance2 = paramdlg.Distance2();
// 				gcodeparam.ender = "";
// 				gcodeparam.header = "";
// 				gcodeparam.upheight = paramdlg.Upheight();
// 				gcodeparam.endspeech = paramdlg.Endspeech();
// 				gcodeparam.strRinget = _strRinget;
// 				gcodeparam.downheight = paramdlg.Downheight();








				//这个板件是否选中，如果不选中，不导出NC
				//if (/*m_picturedlg->m_piclist.m_panelChecked[panel_index] == false*/0)//////////////////////////////////////////////////
				if(!m_pDlgResult->IsPanelChecked(pThePanel))
				{
					continue;
				}

				if (prepareExportGCodeData._cutDouble==false)//单工位时
				{
					prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;//单工位坐标系
				}
				else//双工位时候
				{
					if (prepareExportGCodeData.key%2!=0)//工位1
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;
					}
					else//工位2
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type2;
					}
				}

				/************************************************************************/
				/* 根据index位置，获取板件信息                                          */
				/************************************************************************/
				//获取当前板件
				//Panel_Knife pk = m_picturedlg->_picturecoor[panel_index];
				//PanelNode * node = pk._panel;
				//KnifeClass * kc = pk._kc;

				////获取下一块板件
				//int next_panel_index = panel_index;	//初始值
				//do 
				//{
				//	next_panel_index++;
				//}while((next_panel_index < m_picturedlg->_picturecoor.size())
				//	&&(m_picturedlg->m_piclist.m_panelChecked[next_panel_index] == false));

				////下块板件是否可用
				//bool isHaveNextPanelUsable = (next_panel_index < m_picturedlg->_picturecoor.size());
				bool isHaveNextPanelUsable = (i < (vPanelAndTheSolution.size() - 1)) ? true : false;

				//////////////////////////////////////////////////////////////////////////
				//文件头尾取值
				//如果有下个文件，处理方式
				if (isHaveNextPanelUsable)
				{
					_header = prepareExportGCodeData.temp_header1;
					_ender = prepareExportGCodeData.temp_ender1;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_1;//工位2文件头
					_GLast2 = prepareExportGCodeData.temp_GLast2_1;//工位2文件尾

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_1;//工位1正面文件头
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_1;//工位2正面文件头
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_1;//工位1反面文件尾
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_1;//工位2反面文件尾
				}
				else
				{
					_header = prepareExportGCodeData.temp_header2;
					_ender = prepareExportGCodeData.temp_ender2;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_2;//工位2文件头
					_GLast2 = prepareExportGCodeData.temp_GLast2_2;//工位2文件尾

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_2;//工位1正面文件头
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_2;//工位2正面文件头
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_2;//工位1反面文件尾
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_2;//工位2反面文件尾
				}


				//2018-5-3 yuanzb 去除\r\n中的\r避免多余的换行,有些机器无法读取空行
#if 1
				
				if (_header.find("\r\n") != std::string::npos)
				{
					_header.erase(std::remove(_header.begin(), _header.end(), '\r'), _header.end());
				}

				if (_ender.find("\r\n") != std::string::npos)
				{
					_ender.erase(std::remove(_ender.begin(), _ender.end(), '\r'), _ender.end());
				}

				if (_GFirst2.find("\r\n") != std::string::npos)
				{
					_GFirst2.erase(std::remove(_GFirst2.begin(), _GFirst2.end(), '\r'), _GFirst2.end());
				}

				if (_GLast2.find("\r\n") != std::string::npos)
				{
					_GLast2.erase(std::remove(_GLast2.begin(), _GLast2.end(), '\r'), _GLast2.end());
				}
				
				if (_GBFirst1.find("\r\n") != std::string::npos)
				{
					_GBFirst1.erase(std::remove(_GBFirst1.begin(), _GBFirst1.end(), '\r'), _GBFirst1.end());
				}

				if (_GBFirst2.find("\r\n") != std::string::npos)
				{
					_GBFirst2.erase(std::remove(_GBFirst2.begin(), _GBFirst2.end(), '\r'), _GBFirst2.end());
				}
					
				if (_GBLast1.find("\r\n") != std::string::npos)
				{
					_GBLast1.erase(std::remove(_GBLast1.begin(), _GBLast1.end(), '\r'), _GBLast1.end());
				}
					
				if (_GBLast2.find("\r\n") != std::string::npos)
				{
					_GBLast2.erase(std::remove(_GBLast2.begin(), _GBLast2.end(), '\r'), _GBLast2.end());
				}

					
					
					 
					
#endif

				 



				//生成文件名
#if 1

				std::string tempnode_pname = "";
				std::string tempnode_plast = "";
				std::string tempnode_cname = "";
				std::string tempnode_clast = "";

				//获得下块板件信息
				std::string str_thinmatrial = /*kc->getPanelMatrial()*/pThePanel->m_Material.GetBuffer();
				pThePanel->m_Material.ReleaseBuffer();
				CString s_thinmatrial,s_thin,s_nextthin;
				std::string str_nextthin = "";//下一块板件的板厚
				std::string str_nextthinmatrial = "";//下一块板的材质
				s_thinmatrial.Format("%s",str_thinmatrial.c_str());
				s_thin.Format("%0.0f",/*kc->getThickness()*/pThePanel->m_Thickness);
				s_thinmatrial.Replace("\/","");
				s_thinmatrial.Replace("\\","");
				str_thinmatrial = s_thinmatrial.GetBuffer();
				std::string str_thin = s_thin.GetBuffer();

				//这里选择下一块板件，并获取相关信息
				if (isHaveNextPanelUsable)
				{
					//KnifeClass * nextkc = m_picturedlg->_picturecoor[next_panel_index]._kc;
					Panel* pNextPanel = vPanelAndTheSolution[i+1].m_pPanel;
					s_nextthin.Format("%0.1f",/*nextkc->getThickness()*/pNextPanel->m_Thickness);
					str_nextthin = s_nextthin.GetBuffer();
					str_nextthinmatrial = /*nextkc->getPanelMatrial()*/pNextPanel->m_Material.GetBuffer();
					pNextPanel->m_Material.ReleaseBuffer();
				}

				CString s_nextmaterial;
				s_nextmaterial.Format("%s",str_nextthinmatrial.c_str());
				s_nextthin.Format("%s",str_nextthin.c_str());
				s_nextmaterial.Replace("\/","");
				s_nextmaterial.Replace("\\","");
				str_nextthinmatrial = s_nextmaterial.GetBuffer();
				std::string filename,filenamed,filenamed1,allfile,pfilename,filenameo,pfilenameo,filenamedo,filenamed1o;
				char ch_filename[1000],ch_filenamed[1000],ch_filenamed1[1000], ch_allfile[1000],ch_pfilename[1000],ch_filenameo[1000],ch_pfilenameo[1000],ch_filenamedo[1000],ch_filenamed1o[1000];
				char ch_allFileName[1000];
				char theKey[1000],theKey2[1000];//theKey2表示下一块板件的流水号

				//流水号
				if (/*panel_index*/i<10)
				{
					sprintf_s(theKey,"0%d",/*panel_index*/i + 1);
				}
				else
				{
					sprintf_s(theKey,"%d",/*panel_index*/i + 1);
				}

				if (/*next_panel_index*/i+1 <10)
				{
					sprintf_s(theKey2,"0%d",/*next_panel_index*/(i+1) + 1);
				}
				else
				{
					sprintf_s(theKey2,"%d",/*next_panel_index*/(i+1) + 1);
				}



				std::string tempcname2;//文件名+流水号+板件名称+板件厚度cname
				std::string temppname2;//文件名+流水号+板件名称+板件厚度pname
				std::string tempnextcname2;//文件名+流水号+板件名称+板件厚度cname
				std::string tempnextpname2;//文件名+流水号+板件名称+板件厚度pname
				std::string theAllCname;
				theAllCname = prepareExportGCodeData.node_cname;

				//导出nc代码的文件名
				{
					/*sprintf_s(ch_allfile,"\\All-%s-%s.nc",node_fname.c_str(),str_thin.c_str());
					sprintf_s(ch_filename,"\\C_%s%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilename,"\\P_%s%d.nc",node_fname.c_str(),key);
					sprintf_s(ch_filenameo,"\\C_%s-%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilenameo,"\\P_%s-%d.nc",node_fname.c_str(),key);*/

					if (prepareExportGCodeData.node_cname.find("@")!=std::string::npos)//nc文件名中需要有材质或者板厚
					{
						int thePos1 = prepareExportGCodeData.node_cname.find_first_of('@');
						std::string theCname = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc文件名
						std::string theCname2 = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc文件名
						theAllCname = theCname2;
						std::string theCMaterial = str_thinmatrial;//材质
						std::string theCThickness = str_thin;//板厚
						std::string thenextCMaterial = str_nextthinmatrial;//下一块板件材质
						std::string thenextCThickness = str_nextthin;//下一块板件板厚
						//如果精度是“.0”去掉“.0”，如果精度“0.n”，保留“.n”
						CString cstr_thenextCThickness = HGTools::Convert2CString(thenextCThickness);
						cstr_thenextCThickness.Replace(".0","");
						thenextCThickness = HGTools::Convert2StdString(cstr_thenextCThickness);

						if (prepareExportGCodeData.node_cname.find("@Material@")!=std::string::npos)//存在材质
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCMaterial;
							tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey + prepareExportGCodeData.node_cxhx+theCMaterial; 
							tempnextcname2 = theCname2+ prepareExportGCodeData.node_cxhx + theKey2+ prepareExportGCodeData.node_cxhx + thenextCMaterial;
						}
						if (prepareExportGCodeData.node_cname.find("@Thickness@")!=std::string::npos)//存在板厚
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCThickness + prepareExportGCodeData.node_cxhx;
							if (tempcname2=="")
							{
								tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey+ prepareExportGCodeData.node_cxhx + theCThickness;
							}
							else
								tempcname2 = tempcname2+ prepareExportGCodeData.node_cxhx + theCThickness;
							if (tempnextcname2=="")
							{
								tempnextcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey2 + prepareExportGCodeData.node_cxhx+ thenextCThickness;
							}
							else
								tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_cxhx + thenextCThickness;
						}
						tempnode_cname = theCname+ prepareExportGCodeData.node_cxhx + theKey;
					}
					else
					{
						tempnode_cname = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+theKey;
						tempcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey; 
						tempnextcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey2;
					}

					if (prepareExportGCodeData.node_pname.find("@")!=std::string::npos)//p文件名中需要有材质或者板厚
					{
						int thePos1 = prepareExportGCodeData.node_pname.find_first_of('@');
						std::string thePname = prepareExportGCodeData.node_pname.substr(0,thePos1);//p文件名
						std::string thePname2 = prepareExportGCodeData.node_pname.substr(0,thePos1);//p文件名
						std::string thePMaterial = str_thinmatrial;//材质
						std::string thePThickness = str_thin;//板厚
						std::string thenextPMaterial = str_nextthinmatrial;//下一块板件材质
						std::string thenextPThickness = str_nextthin;//下一块板件板厚
						//如果精度是“.0”去掉“.0”，如果精度“0.n”，保留“.n”
						CString cstr_thenextPThickness = HGTools::Convert2CString(thenextPThickness);
						cstr_thenextPThickness.Replace(".0","");
						thenextPThickness = HGTools::Convert2StdString(cstr_thenextPThickness);


						if (prepareExportGCodeData.node_pname.find("@Material@")!=std::string::npos)//存在材质
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePMaterial;
							temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx+thePMaterial; 
							tempnextpname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPMaterial;
						}
						if (prepareExportGCodeData.node_pname.find("@Thickness@")!=std::string::npos)//存在板厚
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePThickness + prepareExportGCodeData.node_pxhx;
							if (temppname2=="")
							{
								temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx + thePThickness;
							}else
								temppname2 = temppname2 + prepareExportGCodeData.node_pxhx + thePThickness;
							if (tempnextpname2=="")
							{
								tempnextpname2 = thePname2+ prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPThickness;
							}else
								tempnextpname2 = tempnextpname2 + prepareExportGCodeData.node_pxhx+ thenextPThickness;
						}
						tempnode_pname = thePname+ prepareExportGCodeData.node_pxhx + theKey;
					}
					else
					{
						tempnode_pname = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx +theKey;
						temppname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey; 
						tempnextpname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey2;
					}
					//sprintf_s(ch_allfile,"\\All-%s.nc",tempcname2.c_str());
					sprintf_s(ch_filename,"\\%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilename,"\\%s%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());

					sprintf_s(ch_filenameo,"\\%s_%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilenameo,"\\%s_%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				}

				theAllCname = theAllCname + prepareExportGCodeData.node_cxhx + str_thinmatrial + prepareExportGCodeData.node_cxhx + str_thin;//合并同一类型后的文件名

				sprintf_s(ch_allfile,"\\All%s_.nc",theAllCname.c_str());
				sprintf_s(ch_allFileName,"\\All%s.nc",theAllCname.c_str());
				/*tempnextpname2 = tempnextpname2 + node_plast;
				tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_clast;*/
				tempnextpname2 = tempnextpname2;
				tempnextcname2 = tempnextcname2;

				std::string thCnameW = tempcname2+"@W";
				std::string thCnameH = tempcname2+"@H";
				std::string thCname_W = tempcname2+"@-W";
				std::string thCname_H = tempcname2+"@-H";
				/*sprintf_s(ch_filenamed,"\\HG_%d-%s@W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1,"\\HG_%d-%s@H.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamedo,"\\HG_%d-%s@-W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1o,"\\HG_%d-%s@-H.nc",key,str_thin.c_str());*/
				sprintf_s(ch_filenamed,"\\%s%s",thCnameW.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1,"\\%s%s",thCnameH.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamedo,"\\%s%s",thCname_W.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1o,"\\%s%s",thCname_H.c_str(),prepareExportGCodeData.node_clast.c_str());

				filename = ch_filename;//nc
				pfilename = ch_pfilename;//p
				filenamed = ch_filenamed;//@W
				filenamed1 = ch_filenamed1;//@H
				allfile = ch_allfile;
				filenameo = ch_filenameo;//-nc
				pfilenameo = ch_pfilenameo;//-p
				filenamedo = ch_filenamedo;//@-W
				filenamed1o = ch_filenamed1o;//@-H

				std::string print_str1,next_tb_font_path,next_nc_font_path;
				char ch_print_str1[1000],ch_print_str2[1000],ch_print_str3[1000];
				sprintf_s(ch_print_str1,"%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				

				//生成下一块板
				sprintf_s(ch_print_str3,"%s%s",tempnextcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_print_str2,"%s%s",tempnextpname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				/*sprintf_s(ch_print_str1,"C_HG%d-%s.nc",key,str_thin.c_str());
				sprintf_s(ch_print_str3,"C_HG%d-%s.nc",key+1,str_nextthin.c_str());
				sprintf_s(ch_print_str2,"P_HG%d.nc",key+1);*/
				print_str1 = ch_print_str1;


				if (!isHaveNextPanelUsable)
				{
					next_tb_font_path = "";
					next_nc_font_path = "";
				}
				else
				{
					next_tb_font_path = ch_print_str2;
					next_nc_font_path = ch_print_str3;
				}



				std::string prev_tempFilename		=	ch_allFileName;
#if 1

				// 2016-11-11	yzb		需求HM16110101，星辉的机器无法识别中文，但材料中的英文需保留
				// 2017-5-56	yuanzb	新代不支持中文
				if (prepareExportGCodeData.node_name.find("星辉") != string::npos ||
					prepareExportGCodeData.node_print == 14)
				{
					DelChineseInString(filename	);
					DelChineseInString(pfilename);
					DelChineseInString(pfilename);
					DelChineseInString(filenameo);

					DelChineseInString(pfilenameo);
					DelChineseInString(filenamed	);
					DelChineseInString(filenamed1);
					DelChineseInString(allfile	);

					DelChineseInString(filenamedo	 );
					DelChineseInString(filenamed1o	 );
					DelChineseInString(prev_tempFilename);

					DelChineseInString(next_nc_font_path);
					DelChineseInString(next_tb_font_path);

				}

#endif



				//准备好所有输出的文件名
				std::string nc_font_path		=prepareExportGCodeData.pathName.GetBuffer(0)		+ filename		;//nc			//nc文件
				std::string tb_font_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilename		;//p			//自动贴标nc文件
				std::string tb_font_new_path	= prepareExportGCodeData.pathPrint.GetBuffer(0)	+ pfilename		;//p			//自动贴标nc文件
				std::string nc_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenameo		;//-nc			//nc临时文件名

				std::string tb_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilenameo	;//-p			//自动贴标临时文件
				std::string nc_back_w_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed		;//@W			//W翻转nc文件
				std::string nc_back_h_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1	;//@H			//H翻转nc文件
				std::string allpath				= prepareExportGCodeData.pathName.GetBuffer(0)		+ allfile		;

				std::string nc_back_w_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamedo	;//@-W			//W翻转nc临时文件
				std::string nc_back_h_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1o	;//@-H			//H翻转nc临时文件
				std::string tempFilename		=							  prev_tempFilename							;
				std::string allpath2			= prepareExportGCodeData.pathName.GetBuffer(0)		+ tempFilename	;				//合并NC文件

				next_nc_font_path;																			//下一块板件nc文件名
				next_tb_font_path;																			//下一块板件tb文件名


#endif


				//读取配置信息
#if 1
				PrepareKnifeInfoData prepareKnifeInfoData;
				prepareKnifeInfoData.pThePanel = pThePanel;
				//prepareKnifeInfoData.pKnifeClass = &knifeClass;
				//读取刀库信息
				//孔位过滤（找到指定孔直径的孔，并移除）
				PrepareKnifeInfoAndHoleSetting(&prepareExportGCodeData, &prepareKnifeInfoData);
#endif

				//工位偏移计算
#if 1
				bool HaveDvsh = false,HaveDvsw = false,HaveDvs = false;
				std::vector<float> v_trim;
				std::vector<float> v_trimh;
				std::vector<float> v_trimw;

				int _usedLineX = 0;
				int _usedLineY = 0;

				if (prepareExportGCodeData._cutDouble==false)//单工位时
				{
					v_trim.push_back(_trim1);
					v_trim.push_back(_trim2);
					v_trim.push_back(_trim3);
					v_trim.push_back(_trim4);

					v_trimh.push_back(_trim1);
					v_trimh.push_back(_trim4);
					v_trimh.push_back(_trim3);
					v_trimh.push_back(_trim2);

					v_trimw.push_back(_trim3);
					v_trimw.push_back(_trim2);
					v_trimw.push_back(_trim1);
					v_trimw.push_back(_trim4);


					_usedLineX = v_trim[selectMachine.m_usedLineX1];
					_usedLineY = v_trim[selectMachine.m_usedLineY1];

				}
				else//双工位时候
				{
					if (prepareExportGCodeData.key%2!=0)//工位1
					{
						v_trim.push_back(_trim1);
						v_trim.push_back(_trim2);
						v_trim.push_back(_trim3);
						v_trim.push_back(_trim4);

						v_trimh.push_back(_trim1);
						v_trimh.push_back(_trim4);
						v_trimh.push_back(_trim3);
						v_trimh.push_back(_trim2);

						v_trimw.push_back(_trim3);
						v_trimw.push_back(_trim2);
						v_trimw.push_back(_trim1);
						v_trimw.push_back(_trim4);



						_usedLineX = v_trim[selectMachine.m_usedLineX1];
						_usedLineY = v_trim[selectMachine.m_usedLineY1];
					}
					else//工位2
					{
						v_trim.push_back(_ftrim1);
						v_trim.push_back(_ftrim2);
						v_trim.push_back(_ftrim3);
						v_trim.push_back(_ftrim4);

						v_trimh.push_back(_ftrim1);
						v_trimh.push_back(_ftrim4);
						v_trimh.push_back(_ftrim3);
						v_trimh.push_back(_ftrim2);

						v_trimw.push_back(_ftrim3);
						v_trimw.push_back(_ftrim2);
						v_trimw.push_back(_ftrim1);
						v_trimw.push_back(_ftrim4);


						_usedLineX = v_trim[selectMachine.m_usedLineX2];
						_usedLineY = v_trim[selectMachine.m_usedLineY2];
					}
				}	
#endif
				knifeClass.SetSmallPanelCutTwicePara(prepareExportGCodeData.bCutTwice_Enable,prepareExportGCodeData.fCutTwice_SmallSize,prepareExportGCodeData.fCutTwice_ReserveHeight,prepareExportGCodeData.fCutTwice_SecondSpeed);		// 设置小板下两刀参数
				knifeClass.SetThicknessThresholdPara(prepareExportGCodeData.fTwoCutThreshold,prepareExportGCodeData.fTwoCutReserve,prepareExportGCodeData.fTwoCutSpeed);											// 设置小板两刀切

				// 自动计算贴标位置功能
				float tiebiao_width = 0;
				float tiebiao_height = 0;
				GetTiebiaoSize(tiebiao_width,tiebiao_height);


				knifeClass.LabelHeight(tiebiao_height);				//设置贴标标签高度
				knifeClass.LabelWidth(tiebiao_width);				//设置贴标标签宽度

				if(selectMachine.m_nodename.compare("豪迈") != 0)
					PreProcessOfExportGCode();


				// 最后一块小板预修边功能
				if (paramdlg.m_CheckPreDeburring == TRUE)
				{
					// 当前Panel如果最后一张小板靠边，修边，防止跑板
					CSingleon* pSingleton = CSingleon::GetSingleton();
					float knife_radius = pSingleton->m_BaseInfo.m_SawKerfWidth/2;

					int nCpnNum = pThePanel->GetComponentNum();
					Component* pLastCpn = pThePanel->GetComponent(nCpnNum);

					// 单面处理，或者双面无反面信息时，预修边
					if (prepareExportGCodeData.node_dvcut == "0" || pThePanel->HaveDownerFaceInfo() == false)
					{

						if (pLastCpn != NULL)
						{
							float cpn_max_x, cpn_max_y, cpn_min_x, cpn_min_y;

							cpn_min_x = pLastCpn->m_x;
							cpn_min_y = pLastCpn->m_y;
							cpn_max_x = pLastCpn->m_x + pLastCpn->m_RealLength;
							cpn_max_y = pLastCpn->m_y + pLastCpn->m_RealWidth;


							// 大板四个点坐标。
							float panel_max_x, panel_max_y, panel_min_x, panel_min_y;

							panel_min_x = pThePanel->m_x;
							panel_min_y = pThePanel->m_y;
							panel_max_x = pThePanel->m_x + pThePanel->m_RealLength;
							panel_max_y = pThePanel->m_y + pThePanel->m_RealWidth;

							// 判断四条边

#if 1
							// 2017-10-10 yuanzb 邵卫要求单面处理，或者双面无反面信息时，预修边

							if (cpn_min_x == panel_min_x)	// 左边划一刀
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_min_x - knife_radius;
								one_cut._y1 = cpn_max_y;
								one_cut._x2 = cpn_min_x - knife_radius;
								one_cut._y2 = cpn_min_y;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

							if (cpn_min_y == panel_min_y)	// 下边划一刀
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_min_x;
								one_cut._y1 = cpn_min_y - knife_radius;
								one_cut._x2 = cpn_max_x;
								one_cut._y2 = cpn_min_y - knife_radius;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

							if (cpn_max_x == panel_max_x)	// 右边划一刀
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_max_x + knife_radius;
								one_cut._y1 = cpn_min_y;
								one_cut._x2 = cpn_max_x + knife_radius;
								one_cut._y2 = cpn_max_y;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

							if (cpn_max_y == panel_max_y)	// 上边划一刀
							{
								CutPath one_cut ;

								one_cut._x1 = cpn_max_x;
								one_cut._y1 = cpn_max_y + knife_radius;
								one_cut._x2 = cpn_min_x;
								one_cut._y2 = cpn_max_y + knife_radius;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}



#else

							// 2017-8-16 yuanzb 俊宝要求只修长边的一半
							if (cpn_max_y == panel_max_y)	// 上边划一刀
							{
								CutPath one_cut ;

								one_cut._x1 = /*cpn_max_x*/pThePanel->m_RealLength/2;
								one_cut._y1 = cpn_max_y + knife_radius;
								one_cut._x2 = /*cpn_min_x*//*pThePanel->m_OriginX*/0;
								one_cut._y2 = cpn_max_y + knife_radius;

								pThePanel->m_vPreCutPath.push_back(one_cut);
							}

#endif

						}
					}


				}

				try
				{
					int panel_index = i;

					//导出反面孔G代码
					if(selectMachine.m_nodename.compare("豪迈") != 0)
					{
						if (prepareExportGCodeData.node_dvcut=="0")//单面打孔
						{
							HaveDvsw = FALSE;
							HaveDvs = FALSE;
						}
						else//双面打孔
						{
							if (prepareExportGCodeData._cutDouble == false)//单工位
							{
								HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
									prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
									next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
									next_nc_font_path,	1,					v_trimw,		_GBFirst1,
									_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
									_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);

								HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
									prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
									next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
									next_nc_font_path,	2,					v_trimh,		_GBFirst1,
									_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
									_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
							}
							else//双工位
							{
								if (prepareExportGCodeData.key%2!=1)//工位2
								{
									HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
										prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1 ,
										next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
										next_nc_font_path,	1,					v_trimw,		_GBFirst2,	
										_GBLast2,			_iscombine,			_MFirst2,		_MLast2,	
										_PZFirst2,			_PZLast2, total_check_panel_num,	PanelCommnets);

									HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
										prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
										next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
										next_nc_font_path,	2,					v_trimh,		_GBFirst2,
										_GBLast2,			_iscombine,			_MFirst2,		_MLast2,
										_PZFirst2,			_PZLast2, total_check_panel_num,	PanelCommnets);
								}
								else//工位1
								{
									HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
										prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
										next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
										next_nc_font_path,	1,					v_trimw,		_GBFirst1,
										_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
										_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);

									HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
										prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
										next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
										next_nc_font_path,	2,					v_trimh,		_GBFirst1,
										_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
										_PZFirst1,		_PZLast1, total_check_panel_num,	PanelCommnets);
								}
							}
						}

						//L4Cpp::Log()->debug("4.output font face");

						//生成NC代码
						//path HG打头，普通NC文件，pathp TB打头，自动贴标
						//print_str2, 普通NC文件的下一个nc文件名，print_str3 贴标文件的下一个nc文件名

						// 双工位时 单数的板为工位1 双数的板为工位2

						if (prepareExportGCodeData.node_double == true)
						{
							// 工位2
							if ((panel_index+1) % 2 == 0)
							{
								knifeClass.OutputGcode(	pThePanel,			nc_font_path,		tb_font_path,		prepareExportGCodeData.node_print,			
									prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,					panel_index+1,		
									next_tb_font_path,	next_nc_font_path,	_printinfo,			prepareExportGCodeData.node_groupstr,	
									next_nc_font_path,	_MFirst2,			_MLast2,			_PZFirst2,		
									_PZLast2,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
									prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace,	PanelCommnets);
							}
							else		// 工位1
							{
								knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
									prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
									_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
									_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
									prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace,	PanelCommnets);
							}
						}
						else
						{

							knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
								prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
								_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
								_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
								prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace,	PanelCommnets);
						}

						PostProcessOfExportGCode();
					}
					else
					{
						HomagDataItem homagItem;
						homagItem.m_PanelAndTheSolution.m_pPanel = pThePanel;
						homagItem.m_PanelAndTheSolution.m_pSolution = pTheSolution;
						homagItem.m_strPanelUpperFaceFileName = nc_font_path.c_str();
						if (prepareExportGCodeData.node_dvcut =="1")//非单面打孔,水平翻转
						{
							homagItem.m_eFlipType = DirVHType::DIR_H;
							homagItem.m_strPanelDownerFaceFileName_Flip = nc_back_w_path.c_str();
						}
						else if (prepareExportGCodeData.node_dvcut =="2")//非单面打孔，垂直翻转
						{
							homagItem.m_eFlipType = DirVHType::DIR_V;
							homagItem.m_strPanelDownerFaceFileName_Flip = nc_back_h_path.c_str();
						}
						vHomagItem.push_back(homagItem);

						//if(atoi(selectMachine.m_printname.c_str()) != LabelMachine::Excitech)
						//{
						//	DeleteFile(tb_font_path.c_str());
						//	ofstream wfile;
						//	knifeClass.OutputLabel(pThePanel, nc_font_path, tb_font_path, prepareExportGCodeData.node_print, \
						//		panel_index+1, next_tb_font_path, next_nc_font_path, _printinfo, wfile, _newcoor, node_dvcut, LabelInReverseSide);
						//}
						//else
						//	AfxMessageBox(_T("星辉贴标机与豪迈雕刻机不兼容！"));
						
					}

				}
				catch(...)
				{
					MessageBox("NC 生成出错！");
					if(selectMachine.m_nodename.compare("豪迈") != 0)
						PostProcessOfExportGCode();
					return;
				}

				if(selectMachine.m_nodename.compare("豪迈") != 0)
				{
					TransCoordinateData transCoordinateData;
					transCoordinateData.nc_font_temp_path = nc_font_temp_path;
					transCoordinateData.nc_font_path = nc_font_path;
					transCoordinateData.nc_back_w_path = nc_back_w_path;
					transCoordinateData.nc_back_w_temp_path = nc_back_w_temp_path;
					transCoordinateData.nc_back_h_path = nc_back_h_path;
					transCoordinateData.nc_back_h_temp_path = nc_back_h_temp_path;
					transCoordinateData.HaveDvsh = HaveDvsh;
					transCoordinateData.HaveDvsw = HaveDvsw;
					transCoordinateData.HaveDvs = HaveDvs;
					transCoordinateData._usedLineX = _usedLineX;
					transCoordinateData._usedLineY = _usedLineY;
					transCoordinateData.nDecimal = DecimalPlace + 1;
					AfterExportGCode_TransCoordinate(&prepareExportGCodeData, &transCoordinateData);

					prepareExportGCodeData.key++;
					//文件合并

#if 1
					CombineFileData combineFileData;
					combineFileData.str_nextthin = str_nextthin;
					combineFileData.tempnextcname2 = tempnextcname2;
					combineFileData.str_thin = str_thin;
					combineFileData.nc_font_temp_path = nc_font_temp_path;
					combineFileData.nc_font_path = nc_font_path;
					combineFileData.nc_back_h_path = nc_back_h_path;
					combineFileData.nc_back_w_path = nc_back_w_path;
					combineFileData.allpath2 = allpath2;
					AfterExportGCode_CombineFile(&prepareExportGCodeData, &combineFileData);

#endif
					stringActualLabelNCFilePath = tb_font_path;
					//L4Cpp::Log()->debug("7.file clear");
					//对应的 NC 文件移动到当前目录的 cut目录 下
					//对应的 TB贴标 文件移动到当前目录的 print目录 下
					if (selectMachine.m_fileClassifyMove == "1")
					{
						rename(tb_font_path.c_str(),tb_font_new_path.c_str());
						stringActualLabelNCFilePath = tb_font_new_path;
					}
#if 1			
					//NC文件命令字变更
					if (selectMachine.m_NC_CommandChange == "1")
					{
						try
						{
							Change_G0X_To_GX(nc_font_path);
							Change_G0X_To_GX(nc_back_w_path);
							Change_G0X_To_GX(nc_back_h_path);
							Change_G0X_To_GX(allpath2);
						}
						catch (...)
						{
							MessageBox("NC文件扫描处理 更改CMD命令字 错误");
							return;
						}
					}

					//NC坐标空格删除
					if (selectMachine.m_NC_SpaceRemove == "1")
					{
						try
						{
							Change_PosSpace_To_Empty(nc_font_path);
							Change_PosSpace_To_Empty(nc_back_w_path);
							Change_PosSpace_To_Empty(nc_back_h_path);
							Change_PosSpace_To_Empty(allpath2);
						}
						catch (...)
						{
							MessageBox("NC文件扫描处理 NC坐标空格删除 错误");
							return;
						}
					}

					//NC修改铣弧格式,将“G90 G03 X-134.0000 Y-568.0000 R14.0000”格式，改为“G03 X-134.0000 Y-568.0000 U14.0000”
					if (selectMachine.m_NC_ModifyArcMillingFormat == "1")
					{
						try
						{
							Change_Arc_Milling_Format(nc_font_path);
							Change_Arc_Milling_Format(nc_back_w_path);
							Change_Arc_Milling_Format(nc_back_h_path);
							Change_Arc_Milling_Format(allpath2);
						}
						catch (...)
						{
							MessageBox("NC文件扫描处理 修改铣弧格式 错误");
							return;
						}
					}

					std::string aOutputFile[] = {nc_font_path, nc_back_w_path, nc_back_h_path, allpath2};
					for(int n = 0; n < sizeof(aOutputFile)/sizeof(std::string); n++)
					{
						CString strCopiedFilePath;
						strCopiedFilePath.Format(_T("%s\\%s"), strLogDirThisTime, GetFileNameInPath_WithSuffix(aOutputFile[n].c_str()));
						CopyFile(aOutputFile[n].c_str(), strCopiedFilePath, TRUE);
					}
					
#endif
				}


				//L4Cpp::Log()->debug("7.one coor end");
				SyntecDataItem syntecDataItem = {nc_font_path.c_str(), pThePanel};
				vSyntecData.push_back(syntecDataItem);

				XiaoFengDataItem xiaoFengDataItem = {pThePanel, stringActualLabelNCFilePath.c_str(), nc_font_path.c_str()};
				vXiaoFengData.push_back(xiaoFengDataItem);

				mapPanelSerIDToNCFileFullPath[i+1] = CString(nc_font_path.c_str());

				//2018-6-21 yaunzb 处理多余的换行
				try
				{
					dealRepeatCharReturnNewline(nc_font_path);
					dealRepeatCharReturnNewline(nc_back_w_path);
					dealRepeatCharReturnNewline(nc_back_h_path);
					dealRepeatCharReturnNewline(allpath2);
				}
				catch (...)
				{
					MessageBox("NC文件扫描处理 处理重复换行 错误");
					return;
				}




			}

			if (selectMachine.m_SmallPanelNC == "1")
			{
				try
				{
					//OnOutputSmallPanelGcode(&selectMachine,NCSavePath);
				}
				catch (...)
				{
					MessageBox("导出小板NC产生异常");
					return;
				}
			}

			if(atoi(selectMachine.m_printname.c_str()) == LabelMachine::SyntecCycle)
			{
				LabelPastingSide eSide = LabelPastingSide::LABEL_UP_SIDE;
				if(prepareExportGCodeData._isLabelInReverseSide)
				{
					if (prepareExportGCodeData.node_dvcut.compare("1") == 0)
						eSide = LabelPastingSide::LABEL_BACK_SIDE_H_FLIP;
					else if (prepareExportGCodeData.node_dvcut.compare("2") == 0)		
						eSide = LabelPastingSide::LABEL_BACK_SIDE_V_FLIP;
				}

				SyntecSummaryOutput(vSyntecData, /*prepareExportGCodeData.nodecoor_type*/0, prepareExportGCodeData.pathName, eSide);
			}
			else if(atoi(selectMachine.m_printname.c_str()) == LabelMachine::XiaoFeng)
				XiaoFentLabelProcess(vXiaoFengData, prepareExportGCodeData.pathName);

			if(selectMachine.m_nodename.compare("豪迈") == 0)
			{
				ExportGCode_Homag(vHomagItem, prepareExportGCodeData.pathName);
			}

			if(bModifyLabelImgNameLikeNCName)
			{
				ModifyLabelImgNameLikeNCName(mapPanelSerIDToNCFileFullPath, prepareExportGCodeData.pathName);
			}

			AfxMessageBox(_T("导出完毕"));

	//L4Cpp::Log()->info("nc ok");
}

LRESULT CCarvingView::OnExportComponentNC(WPARAM wparam, LPARAM lparam)
{
	vector<CString>& vComponentBarcode = *((vector<CString>*)wparam);
	vector<Panel*> vComponentPanel;
	for(int i = 0; i < vComponentBarcode.size(); i++)
	{
		for(int j = 0; j < m_vComponentInputItem.size(); j++)
		{
			if(m_vComponentInputItem[j].m_strBarcode.Compare(vComponentBarcode[i]) == 0)
			{
				Component* pComponent = ConvertOneInputInfoToOneComponent(m_vComponentInputItem[j]);
				pComponent->m_x = pComponent->m_y = 0;
				Panel* pPanel = new Panel;
				pPanel->m_OrgLen = pPanel->m_RealLength = pComponent->m_RealLength;
				pPanel->m_OrgWidth = pPanel->m_RealWidth = pComponent->m_RealWidth;
				pPanel->m_OriginX = pPanel->m_OriginY = 0;
				pPanel->m_Material = pComponent->m_Material;
				pPanel->m_Texture = pComponent->m_Texture;	
				pPanel->m_Thickness = pComponent->m_Thickness;
				pPanel->m_type = NodeType_CombinePanel;
				pPanel->m_pParent = NULL;
				pPanel->m_ChildrenList.push_back(pComponent);

				vComponentPanel.push_back(pPanel);

				break;
			}
		}
	}
	
	ExportComponentNC(vComponentPanel);

	for(int i = 0; i < vComponentPanel.size(); i++)
	{
		delete (Component*)(vComponentPanel[i]->m_ChildrenList[0]);
		vComponentPanel[i]->m_ChildrenList.clear();
		delete vComponentPanel[i];
	}

	return 0;
}

int CCarvingView::ExportComponentNC(vector<Panel*>& vComponentPanel)
{
	KnifeClass& knifeClass = *(CSingleon::GetSingleton()->m_pKnifeClass);

	// 提示信息初始化，只提示一块板
	knifeClass.EnaleMsgReport();


	// 获取小数位数、板件注释
	ParamSettingDlg settings;
	settings.loadSettings();
	int DecimalPlace = settings.m_valDecimalPlace;
	BOOL PanelCommnets = settings.m_chkboxPanelComments;





	CSelMachineDlg selectMachine;
	CParamDlg paramdlg;
	paramdlg.loadSettings();
	PrepareExportGCodeData prepareExportGCodeData;
	prepareExportGCodeData.m_pSelMachineDlg = &selectMachine;
	prepareExportGCodeData.pParamDlg = &paramdlg;
	prepareExportGCodeData.pKnifeClass = &knifeClass;
	bool bPrepareResult = PrepareExportGCode(&prepareExportGCodeData);
	if(!bPrepareResult)
		return 1;
	prepareExportGCodeData.node_print = 0;

	int total_check_panel_num = 1;

		//CDlgLabelPosCalc dlgLabelPosCalc(selectMachine.m_nodename.c_str());
		//dlgLabelPosCalc.DoModal();

		/////////////////////////////////////////////////////////////////////////////////
		////获得每项的信息，索引是条码
		vector<P_Info> _printinfo;
		//for(int i = 0; i < m_vComponentInputItem.size(); i++)
		//{
		//	P_Info temp;
		//	temp._info = m_vComponentInputItem[i].m_strBarcode;
		//	temp._content = getPrintInfoAsString(m_vComponentInputItem[i]);
		//	_printinfo.push_back(temp);
		//}





			// 设置knifeClass信息
			//for(int i = 0; i < prepareExportGCodeData.vPanelAndTheSolution.size(); i++)
			//{
				//Panel* pThePanel = prepareExportGCodeData.vPanelAndTheSolution[i].m_pPanel;
				//CSolution* pTheSolution = prepareExportGCodeData.vPanelAndTheSolution[i].m_pSolution;
			for(int j = 0; j < vComponentPanel.size(); j++)
			{
				Panel* pThePanel = vComponentPanel[j];
				Component& theComponent = *((Component*)(pThePanel->m_ChildrenList[0]));

				int i = 0;

				// 基本信息设置
				knifeClass.setKerfwidth(0);
				knifeClass.setPanelWidth(pThePanel->m_RealLength);
				knifeClass.setPanelHeight(pThePanel->m_RealWidth);
				knifeClass.setThickness(pThePanel->m_Thickness);
				knifeClass.setDeburringwidth(/*paramdlg.Deburringwidth()*/0);

				string strThicknessAndMetrial;
				CString cstrThicknessAndMetrial = GetFloatString(pThePanel->m_Thickness, 1) + "-" + pThePanel->m_Material;
				strThicknessAndMetrial = cstrThicknessAndMetrial.GetBuffer();
				cstrThicknessAndMetrial.ReleaseBuffer();

				knifeClass.setThinMatrial(strThicknessAndMetrial);
				knifeClass.setPanelMatrial(pThePanel->m_Material.GetBuffer());
				pThePanel->m_Material.ReleaseBuffer();
				knifeClass.SetInputOrderNum("a1");
				knifeClass.SetLeftParam(prepareExportGCodeData.fCutTwice_SmallSize,//小板最小尺寸
					prepareExportGCodeData.fCutTwice_ReserveHeight,//预雕保留厚度
					prepareExportGCodeData.fCutTwice_SecondSpeed);//第二刀加工速率

				// 设置gcodeparam的信息
				knifeClass.m_gcodeparam.upheight = paramdlg.Upheight();
				knifeClass.m_gcodeparam.downheight = paramdlg.Downheight();
				knifeClass.m_gcodeparam.distance1 = paramdlg.Distance1();
				knifeClass.m_gcodeparam.distance2 = paramdlg.Distance2();


				if (prepareExportGCodeData._cutDouble==false)//单工位时
				{
					prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;//单工位坐标系
				}
				else//双工位时候
				{
					if (prepareExportGCodeData.key%2!=0)//工位1
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type1;
					}
					else//工位2
					{
						prepareExportGCodeData.nodecoor_type = prepareExportGCodeData.nodecoor_type2;
					}
				}

				/************************************************************************/
				/* 根据index位置，获取板件信息                                          */
				/************************************************************************/
				//获取当前板件
				//Panel_Knife pk = m_picturedlg->_picturecoor[panel_index];
				//PanelNode * node = pk._panel;
				//KnifeClass * kc = pk._kc;

				////获取下一块板件
				//int next_panel_index = panel_index;	//初始值
				//do 
				//{
				//	next_panel_index++;
				//}while((next_panel_index < m_picturedlg->_picturecoor.size())
				//	&&(m_picturedlg->m_piclist.m_panelChecked[next_panel_index] == false));

				////下块板件是否可用
				bool isHaveNextPanelUsable = false;

				//////////////////////////////////////////////////////////////////////////
				//文件头尾取值
				//如果有下个文件，处理方式
				if (isHaveNextPanelUsable)
				{
					_header = prepareExportGCodeData.temp_header1;
					_ender = prepareExportGCodeData.temp_ender1;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_1;//工位2文件头
					_GLast2 = prepareExportGCodeData.temp_GLast2_1;//工位2文件尾

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_1;//工位1正面文件头
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_1;//工位2正面文件头
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_1;//工位1反面文件尾
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_1;//工位2反面文件尾
				}
				else
				{
					_header = prepareExportGCodeData.temp_header2;
					_ender = prepareExportGCodeData.temp_ender2;
					_GFirst2 = prepareExportGCodeData.temp_GFirst2_2;//工位2文件头
					_GLast2 = prepareExportGCodeData.temp_GLast2_2;//工位2文件尾

					_GBFirst1 = prepareExportGCodeData.temp_GBFirst1_2;//工位1正面文件头
					_GBFirst2 = prepareExportGCodeData.temp_GBFirst2_2;//工位2正面文件头
					_GBLast1 = prepareExportGCodeData.temp_GBLast1_2;//工位1反面文件尾
					_GBLast2 = prepareExportGCodeData.temp_GBLast2_2;//工位2反面文件尾
				}


				//2018-5-3 yuanzb 去除\r\n中的\r避免多余的换行,有些机器无法读取空行
#if 1

				if (_header.find("\r\n") != std::string::npos)
				{
					_header.erase(std::remove(_header.begin(), _header.end(), '\r'), _header.end());
				}

				if (_ender.find("\r\n") != std::string::npos)
				{
					_ender.erase(std::remove(_ender.begin(), _ender.end(), '\r'), _ender.end());
				}

				if (_GFirst2.find("\r\n") != std::string::npos)
				{
					_GFirst2.erase(std::remove(_GFirst2.begin(), _GFirst2.end(), '\r'), _GFirst2.end());
				}

				if (_GLast2.find("\r\n") != std::string::npos)
				{
					_GLast2.erase(std::remove(_GLast2.begin(), _GLast2.end(), '\r'), _GLast2.end());
				}

				if (_GBFirst1.find("\r\n") != std::string::npos)
				{
					_GBFirst1.erase(std::remove(_GBFirst1.begin(), _GBFirst1.end(), '\r'), _GBFirst1.end());
				}

				if (_GBFirst2.find("\r\n") != std::string::npos)
				{
					_GBFirst2.erase(std::remove(_GBFirst2.begin(), _GBFirst2.end(), '\r'), _GBFirst2.end());
				}

				if (_GBLast1.find("\r\n") != std::string::npos)
				{
					_GBLast1.erase(std::remove(_GBLast1.begin(), _GBLast1.end(), '\r'), _GBLast1.end());
				}

				if (_GBLast2.find("\r\n") != std::string::npos)
				{
					_GBLast2.erase(std::remove(_GBLast2.begin(), _GBLast2.end(), '\r'), _GBLast2.end());
				}





#endif




				//生成文件名
#if 1

				std::string tempnode_pname = "";
				std::string tempnode_plast = "";
				std::string tempnode_cname = "";
				std::string tempnode_clast = "";

				//获得下块板件信息
				std::string str_thinmatrial = /*kc->getPanelMatrial()*/pThePanel->m_Material.GetBuffer();
				pThePanel->m_Material.ReleaseBuffer();
				CString s_thinmatrial,s_thin,s_nextthin;
				std::string str_nextthin = "";//下一块板件的板厚
				std::string str_nextthinmatrial = "";//下一块板的材质
				s_thinmatrial.Format("%s",str_thinmatrial.c_str());
				s_thin.Format("%0.0f",/*kc->getThickness()*/pThePanel->m_Thickness);
				s_thinmatrial.Replace("\/","");
				s_thinmatrial.Replace("\\","");
				str_thinmatrial = s_thinmatrial.GetBuffer();
				std::string str_thin = s_thin.GetBuffer();

				CString s_nextmaterial;
				s_nextmaterial.Format("%s",str_nextthinmatrial.c_str());
				s_nextthin.Format("%s",str_nextthin.c_str());
				s_nextmaterial.Replace("\/","");
				s_nextmaterial.Replace("\\","");
				str_nextthinmatrial = s_nextmaterial.GetBuffer();
				std::string filename,filenamed,filenamed1,allfile,pfilename,filenameo,pfilenameo,filenamedo,filenamed1o;
				char ch_filename[1000],ch_filenamed[1000],ch_filenamed1[1000], ch_allfile[1000],ch_pfilename[1000],ch_filenameo[1000],ch_pfilenameo[1000],ch_filenamedo[1000],ch_filenamed1o[1000];
				char ch_allFileName[1000];
				char theKey[1000],theKey2[1000];//theKey2表示下一块板件的流水号

				//流水号
				if (/*panel_index*/i<10)
				{
					sprintf_s(theKey,"0%d",/*panel_index*/i + 1);
				}
				else
				{
					sprintf_s(theKey,"%d",/*panel_index*/i + 1);
				}

				if (/*next_panel_index*/i+1 <10)
				{
					sprintf_s(theKey2,"0%d",/*next_panel_index*/(i+1) + 1);
				}
				else
				{
					sprintf_s(theKey2,"%d",/*next_panel_index*/(i+1) + 1);
				}



				std::string tempcname2;//文件名+流水号+板件名称+板件厚度cname
				std::string temppname2;//文件名+流水号+板件名称+板件厚度pname
				std::string tempnextcname2;//文件名+流水号+板件名称+板件厚度cname
				std::string tempnextpname2;//文件名+流水号+板件名称+板件厚度pname
				std::string theAllCname;
				theAllCname = prepareExportGCodeData.node_cname;

				//导出nc代码的文件名
				{
					/*sprintf_s(ch_allfile,"\\All-%s-%s.nc",node_fname.c_str(),str_thin.c_str());
					sprintf_s(ch_filename,"\\C_%s%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilename,"\\P_%s%d.nc",node_fname.c_str(),key);
					sprintf_s(ch_filenameo,"\\C_%s-%d-%s.nc",node_fname.c_str(),key,str_thin.c_str());
					sprintf_s(ch_pfilenameo,"\\P_%s-%d.nc",node_fname.c_str(),key);*/

					if (prepareExportGCodeData.node_cname.find("@")!=std::string::npos)//nc文件名中需要有材质或者板厚
					{
						int thePos1 = prepareExportGCodeData.node_cname.find_first_of('@');
						std::string theCname = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc文件名
						std::string theCname2 = prepareExportGCodeData.node_cname.substr(0,thePos1);//nc文件名
						theAllCname = theCname2;
						std::string theCMaterial = str_thinmatrial;//材质
						std::string theCThickness = str_thin;//板厚
						std::string thenextCMaterial = str_nextthinmatrial;//下一块板件材质
						std::string thenextCThickness = str_nextthin;//下一块板件板厚
						//如果精度是“.0”去掉“.0”，如果精度“0.n”，保留“.n”
						CString cstr_thenextCThickness = HGTools::Convert2CString(thenextCThickness);
						cstr_thenextCThickness.Replace(".0","");
						thenextCThickness = HGTools::Convert2StdString(cstr_thenextCThickness);

						if (prepareExportGCodeData.node_cname.find("@Material@")!=std::string::npos)//存在材质
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCMaterial;
							tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey + prepareExportGCodeData.node_cxhx+theCMaterial; 
							tempnextcname2 = theCname2+ prepareExportGCodeData.node_cxhx + theKey2+ prepareExportGCodeData.node_cxhx + thenextCMaterial;
						}
						if (prepareExportGCodeData.node_cname.find("@Thickness@")!=std::string::npos)//存在板厚
						{
							theCname = theCname + prepareExportGCodeData.node_cxhx +theCThickness + prepareExportGCodeData.node_cxhx;
							if (tempcname2=="")
							{
								tempcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey+ prepareExportGCodeData.node_cxhx + theCThickness;
							}
							else
								tempcname2 = tempcname2+ prepareExportGCodeData.node_cxhx + theCThickness;
							if (tempnextcname2=="")
							{
								tempnextcname2 = theCname2 + prepareExportGCodeData.node_cxhx + theKey2 + prepareExportGCodeData.node_cxhx+ thenextCThickness;
							}
							else
								tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_cxhx + thenextCThickness;
						}
						tempnode_cname = theCname+ prepareExportGCodeData.node_cxhx + theKey;
					}
					else
					{
						tempnode_cname = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+theKey;
						tempcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey; 
						tempnextcname2 = prepareExportGCodeData.node_cname + prepareExportGCodeData.node_cxhx+ theKey2;
					}

					if (prepareExportGCodeData.node_pname.find("@")!=std::string::npos)//p文件名中需要有材质或者板厚
					{
						int thePos1 = prepareExportGCodeData.node_pname.find_first_of('@');
						std::string thePname = prepareExportGCodeData.node_pname.substr(0,thePos1);//p文件名
						std::string thePname2 = prepareExportGCodeData.node_pname.substr(0,thePos1);//p文件名
						std::string thePMaterial = str_thinmatrial;//材质
						std::string thePThickness = str_thin;//板厚
						std::string thenextPMaterial = str_nextthinmatrial;//下一块板件材质
						std::string thenextPThickness = str_nextthin;//下一块板件板厚
						//如果精度是“.0”去掉“.0”，如果精度“0.n”，保留“.n”
						CString cstr_thenextPThickness = HGTools::Convert2CString(thenextPThickness);
						cstr_thenextPThickness.Replace(".0","");
						thenextPThickness = HGTools::Convert2StdString(cstr_thenextPThickness);


						if (prepareExportGCodeData.node_pname.find("@Material@")!=std::string::npos)//存在材质
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePMaterial;
							temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx+thePMaterial; 
							tempnextpname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPMaterial;
						}
						if (prepareExportGCodeData.node_pname.find("@Thickness@")!=std::string::npos)//存在板厚
						{
							thePname = thePname + prepareExportGCodeData.node_pxhx +thePThickness + prepareExportGCodeData.node_pxhx;
							if (temppname2=="")
							{
								temppname2 = thePname2 + prepareExportGCodeData.node_pxhx + theKey + prepareExportGCodeData.node_pxhx + thePThickness;
							}else
								temppname2 = temppname2 + prepareExportGCodeData.node_pxhx + thePThickness;
							if (tempnextpname2=="")
							{
								tempnextpname2 = thePname2+ prepareExportGCodeData.node_pxhx + theKey2 + prepareExportGCodeData.node_pxhx + thenextPThickness;
							}else
								tempnextpname2 = tempnextpname2 + prepareExportGCodeData.node_pxhx+ thenextPThickness;
						}
						tempnode_pname = thePname+ prepareExportGCodeData.node_pxhx + theKey;
					}
					else
					{
						tempnode_pname = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx +theKey;
						temppname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey; 
						tempnextpname2 = prepareExportGCodeData.node_pname + prepareExportGCodeData.node_pxhx + theKey2;
					}
					//sprintf_s(ch_allfile,"\\All-%s.nc",tempcname2.c_str());
					sprintf_s(ch_filename,"\\%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilename,"\\%s%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());

					sprintf_s(ch_filenameo,"\\%s_%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
					sprintf_s(ch_pfilenameo,"\\%s_%s",temppname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				}

				theAllCname = theAllCname + prepareExportGCodeData.node_cxhx + str_thinmatrial + prepareExportGCodeData.node_cxhx + str_thin;//合并同一类型后的文件名

				sprintf_s(ch_allfile,"\\All%s_.nc",theAllCname.c_str());
				sprintf_s(ch_allFileName,"\\All%s.nc",theAllCname.c_str());
				/*tempnextpname2 = tempnextpname2 + node_plast;
				tempnextcname2 = tempnextcname2 + prepareExportGCodeData.node_clast;*/
				tempnextpname2 = tempnextpname2;
				tempnextcname2 = tempnextcname2;

				std::string thCnameW = tempcname2+"@W";
				std::string thCnameH = tempcname2+"@H";
				std::string thCname_W = tempcname2+"@-W";
				std::string thCname_H = tempcname2+"@-H";
				/*sprintf_s(ch_filenamed,"\\HG_%d-%s@W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1,"\\HG_%d-%s@H.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamedo,"\\HG_%d-%s@-W.nc",key,str_thin.c_str());
				sprintf_s(ch_filenamed1o,"\\HG_%d-%s@-H.nc",key,str_thin.c_str());*/
				sprintf_s(ch_filenamed,"\\%s%s",thCnameW.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1,"\\%s%s",thCnameH.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamedo,"\\%s%s",thCname_W.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_filenamed1o,"\\%s%s",thCname_H.c_str(),prepareExportGCodeData.node_clast.c_str());

				filename = ch_filename;//nc
				pfilename = ch_pfilename;//p
				filenamed = ch_filenamed;//@W
				filenamed1 = ch_filenamed1;//@H
				allfile = ch_allfile;
				filenameo = ch_filenameo;//-nc
				pfilenameo = ch_pfilenameo;//-p
				filenamedo = ch_filenamedo;//@-W
				filenamed1o = ch_filenamed1o;//@-H

				std::string print_str1,next_tb_font_path,next_nc_font_path;
				char ch_print_str1[1000],ch_print_str2[1000],ch_print_str3[1000];
				sprintf_s(ch_print_str1,"%s%s",tempcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				

				//生成下一块板
				sprintf_s(ch_print_str3,"%s%s",tempnextcname2.c_str(),prepareExportGCodeData.node_clast.c_str());
				sprintf_s(ch_print_str2,"%s%s",tempnextpname2.c_str(),prepareExportGCodeData.node_plast.c_str());
				/*sprintf_s(ch_print_str1,"C_HG%d-%s.nc",key,str_thin.c_str());
				sprintf_s(ch_print_str3,"C_HG%d-%s.nc",key+1,str_nextthin.c_str());
				sprintf_s(ch_print_str2,"P_HG%d.nc",key+1);*/
				print_str1 = ch_print_str1;


				if (!isHaveNextPanelUsable)
				{
					next_tb_font_path = "";
					next_nc_font_path = "";
				}
				else
				{
					next_tb_font_path = ch_print_str2;
					next_nc_font_path = ch_print_str3;
				}



				std::string prev_tempFilename		=	ch_allFileName;
#if 1

				// 2016-11-11	yzb		需求HM16110101，星辉的机器无法识别中文，但材料中的英文需保留
				// 2017-5-56	yuanzb	新代不支持中文
				if (prepareExportGCodeData.node_name.find("星辉") != string::npos ||
					prepareExportGCodeData.node_print == 14)
				{
					DelChineseInString(filename	);
					DelChineseInString(pfilename);
					DelChineseInString(pfilename);
					DelChineseInString(filenameo);

					DelChineseInString(pfilenameo);
					DelChineseInString(filenamed	);
					DelChineseInString(filenamed1);
					DelChineseInString(allfile	);

					DelChineseInString(filenamedo	 );
					DelChineseInString(filenamed1o	 );
					DelChineseInString(prev_tempFilename);

					DelChineseInString(next_nc_font_path);
					DelChineseInString(next_tb_font_path);

				}

#endif



				//准备好所有输出的文件名
				//std::string nc_font_path		=prepareExportGCodeData.pathName.GetBuffer(0)		+ filename		;//nc			//nc文件
				//std::string tb_font_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilename		;//p			//自动贴标nc文件
				//std::string tb_font_new_path	= prepareExportGCodeData.pathPrint.GetBuffer(0)	+ pfilename		;//p			//自动贴标nc文件
				//std::string nc_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenameo		;//-nc			//nc临时文件名

				//std::string tb_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilenameo	;//-p			//自动贴标临时文件
				//std::string nc_back_w_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed		;//@W			//W翻转nc文件
				//std::string nc_back_h_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1	;//@H			//H翻转nc文件
				//std::string allpath				= prepareExportGCodeData.pathName.GetBuffer(0)		+ allfile		;

				//std::string nc_back_w_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamedo	;//@-W			//W翻转nc临时文件
				//std::string nc_back_h_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ filenamed1o	;//@-H			//H翻转nc临时文件
				//std::string tempFilename		=							  prev_tempFilename							;
				//std::string allpath2			= prepareExportGCodeData.pathName.GetBuffer(0)		+ tempFilename	;				//合并NC文件

				//next_nc_font_path;																			//下一块板件nc文件名
				//next_tb_font_path;																			//下一块板件tb文件名

				CString strTmp;
				CString strFrontName, strBackName;
				if(theComponent.m_strReverseSideBarcode.IsEmpty())
				{
					strFrontName = theComponent.m_BarCode;
					strBackName.Format(_T("%sDELETE"), theComponent.m_BarCode);
				}
				else if(theComponent.m_strReverseSideBarcode.Compare(theComponent.m_BarCode) == 0)
				{
					if(_iscombine!=1)
					{
						strFrontName.Format(_T("%sDELETE"), theComponent.m_BarCode);
						strBackName = theComponent.m_BarCode;
					}
					else
					{
						strFrontName = theComponent.m_BarCode;
						strBackName.Format(_T("%sDELETE"), theComponent.m_BarCode);
					}
				}
				else
				{
					strFrontName = theComponent.m_BarCode;
					strBackName = theComponent.m_strReverseSideBarcode;
				}

				strTmp.Format(_T("%s%s%s"), _T("\\"), strFrontName, prepareExportGCodeData.node_clast.c_str());
				std::string nc_font_path		=prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp		;//nc			//nc文件
				std::string tb_font_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilename		;//p			//自动贴标nc文件
				std::string tb_font_new_path	= prepareExportGCodeData.pathPrint.GetBuffer(0)	+ pfilename		;//p			//自动贴标nc文件
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), strFrontName, _T("_"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp		;//-nc			//nc临时文件名

				std::string tb_font_temp_path	= prepareExportGCodeData.pathName.GetBuffer(0)		+ pfilenameo	;//-p			//自动贴标临时文件
				strTmp.Format(_T("%s%s%s%s"), _T("\\"),strBackName, _T("@W"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_w_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp		;//@W			//W翻转nc文件
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), strBackName, _T("@H"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_h_path		= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;//@H			//H翻转nc文件
				strTmp.Format(_T("%s%s%s%s%s"), _T("\\"), _T("All"), theComponent.m_BarCode, _T("_"), prepareExportGCodeData.node_clast.c_str());
				std::string allpath				= prepareExportGCodeData.pathName.GetBuffer(0)		+ 		strTmp;

				strTmp.Format(_T("%s%s%s%s"), _T("\\"), strBackName, _T("@-W"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_w_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;//@-W			//W翻转nc临时文件
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), strBackName, _T("@-H"), prepareExportGCodeData.node_clast.c_str());
				std::string nc_back_h_temp_path = prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;//@-H			//H翻转nc临时文件
				std::string tempFilename		=							  prev_tempFilename							;
				strTmp.Format(_T("%s%s%s%s"), _T("\\"), _T("All"), theComponent.m_BarCode, prepareExportGCodeData.node_clast.c_str());
				std::string allpath2			= prepareExportGCodeData.pathName.GetBuffer(0)		+ strTmp	;				//合并NC文件

				next_nc_font_path;																			//下一块板件nc文件名
				next_tb_font_path;																			//下一块板件tb文件名


#endif


				//读取配置信息
#if 1
				PrepareKnifeInfoData prepareKnifeInfoData;
				prepareKnifeInfoData.pThePanel = pThePanel;
				//prepareKnifeInfoData.pKnifeClass = &knifeClass;
				//读取刀库信息
				//孔位过滤（找到指定孔直径的孔，并移除）
				PrepareKnifeInfoAndHoleSetting(&prepareExportGCodeData, &prepareKnifeInfoData);
#endif

				//工位偏移计算
#if 1
				bool HaveDvsh = false,HaveDvsw = false,HaveDvs = false;
				std::vector<float> v_trim;
				std::vector<float> v_trimh;
				std::vector<float> v_trimw;

				int _usedLineX = 0;
				int _usedLineY = 0;

				//if (prepareExportGCodeData._cutDouble==false)//单工位时
				//{
				//	v_trim.push_back(_trim1);
				//	v_trim.push_back(_trim2);
				//	v_trim.push_back(_trim3);
				//	v_trim.push_back(_trim4);

				//	v_trimh.push_back(_trim1);
				//	v_trimh.push_back(_trim4);
				//	v_trimh.push_back(_trim3);
				//	v_trimh.push_back(_trim2);

				//	v_trimw.push_back(_trim3);
				//	v_trimw.push_back(_trim2);
				//	v_trimw.push_back(_trim1);
				//	v_trimw.push_back(_trim4);


				//	_usedLineX = v_trim[selectMachine.m_usedLineX1];
				//	_usedLineY = v_trim[selectMachine.m_usedLineY1];

				//}
				//else//双工位时候
				//{
				//	if (prepareExportGCodeData.key%2!=0)//工位1
				//	{
				//		v_trim.push_back(_trim1);
				//		v_trim.push_back(_trim2);
				//		v_trim.push_back(_trim3);
				//		v_trim.push_back(_trim4);

				//		v_trimh.push_back(_trim1);
				//		v_trimh.push_back(_trim4);
				//		v_trimh.push_back(_trim3);
				//		v_trimh.push_back(_trim2);

				//		v_trimw.push_back(_trim3);
				//		v_trimw.push_back(_trim2);
				//		v_trimw.push_back(_trim1);
				//		v_trimw.push_back(_trim4);



				//		_usedLineX = v_trim[selectMachine.m_usedLineX1];
				//		_usedLineY = v_trim[selectMachine.m_usedLineY1];
				//	}
				//	else//工位2
				//	{
				//		v_trim.push_back(_ftrim1);
				//		v_trim.push_back(_ftrim2);
				//		v_trim.push_back(_ftrim3);
				//		v_trim.push_back(_ftrim4);

				//		v_trimh.push_back(_ftrim1);
				//		v_trimh.push_back(_ftrim4);
				//		v_trimh.push_back(_ftrim3);
				//		v_trimh.push_back(_ftrim2);

				//		v_trimw.push_back(_ftrim3);
				//		v_trimw.push_back(_ftrim2);
				//		v_trimw.push_back(_ftrim1);
				//		v_trimw.push_back(_ftrim4);


				//		_usedLineX = v_trim[selectMachine.m_usedLineX2];
				//		_usedLineY = v_trim[selectMachine.m_usedLineY2];
				//	}
				//}	
#endif
				knifeClass.SetSmallPanelCutTwicePara(prepareExportGCodeData.bCutTwice_Enable,prepareExportGCodeData.fCutTwice_SmallSize,prepareExportGCodeData.fCutTwice_ReserveHeight,prepareExportGCodeData.fCutTwice_SecondSpeed);		// 设置小板下两刀参数
				knifeClass.SetThicknessThresholdPara(prepareExportGCodeData.fTwoCutThreshold,prepareExportGCodeData.fTwoCutReserve,prepareExportGCodeData.fTwoCutSpeed);											// 设置小板两刀切

				// 自动计算贴标位置功能
				float tiebiao_width = 0;
				float tiebiao_height = 0;
				GetTiebiaoSize(tiebiao_width,tiebiao_height);


				knifeClass.LabelHeight(tiebiao_height);				//设置贴标标签高度
				knifeClass.LabelWidth(tiebiao_width);				//设置贴标标签宽度

				//PreProcessOfExportGCode();

				for(int i = 0; i < theComponent.m_vDownerFaceHole.size(); i++)
				{
					theComponent.m_vDownerFaceHole[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceHole[i]._x;
				}
				for(int i = 0; i < theComponent.m_vDownerFaceSlot.size(); i++)
				{
					theComponent.m_vDownerFaceSlot[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceSlot[i]._x;
				}

				try
				{
					int panel_index = i;

					//导出反面孔G代码
					if (/*prepareExportGCodeData.node_dvcut=="0"*/false)//单面打孔
					{
						HaveDvsw = FALSE;
						HaveDvs = FALSE;
					}
					else//双面打孔
					{
						if (/*prepareExportGCodeData._cutDouble == false*/true)//单工位
						{
							HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
																	prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
																	next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																	next_nc_font_path,	1,					v_trimw,		_GBFirst1,
																	_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																	_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
							
							HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
																	prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index,
																	next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
																	next_nc_font_path,	2,					v_trimh,		_GBFirst1,
																	_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
																	_PZFirst1,			_PZLast1, total_check_panel_num,	PanelCommnets);
						}
						else//双工位
						{
							//if (prepareExportGCodeData.key%2!=1)//工位2
							//{
							//	HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1 ,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	1,					v_trimw,		_GBFirst2,	
							//											_GBLast2,			_iscombine,			_MFirst2,		_MLast2,	
							//											_PZFirst2,			_PZLast2, prepareExportGCodeData.total_check_panel_num);
							//	
							//	HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	2,					v_trimh,		_GBFirst2,
							//											_GBLast2,			_iscombine,			_MFirst2,		_MLast2,
							//											_PZFirst2,			_PZLast2, prepareExportGCodeData.total_check_panel_num);
							//}
							//else//工位1
							//{
							//	HaveDvsw = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_w_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	1,					v_trimw,		_GBFirst1,
							//											_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
							//											_PZFirst1,			_PZLast1, prepareExportGCodeData.total_check_panel_num);

							//	HaveDvs = knifeClass.OutputdvsGcodem(	pThePanel,			nc_back_h_path,		tb_font_path,	prepareExportGCodeData.node_print,
							//											prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,				panel_index+1,
							//											next_tb_font_path,	next_nc_font_path,	_printinfo,		prepareExportGCodeData.node_groupstr,
							//											next_nc_font_path,	2,					v_trimh,		_GBFirst1,
							//											_GBLast1,			_iscombine,			_MFirst1,		_MLast1,
							//											_PZFirst1,		_PZLast1, prepareExportGCodeData.total_check_panel_num);
							//}
						}
					}

					//L4Cpp::Log()->debug("4.output font face");

					//生成NC代码
					//path HG打头，普通NC文件，pathp TB打头，自动贴标
					//print_str2, 普通NC文件的下一个nc文件名，print_str3 贴标文件的下一个nc文件名

					// 双工位时 单数的板为工位1 双数的板为工位2

					if (/*prepareExportGCodeData.node_double == true*/false)
					{
						// 工位2
						if ((panel_index+1) % 2 == 0)
						{
							knifeClass.OutputGcode(	pThePanel,			nc_font_path,		tb_font_path,		prepareExportGCodeData.node_print,			
													prepareExportGCodeData.node_double,		prepareExportGCodeData.node_sysuse,		0,					panel_index+1,		
													next_tb_font_path,	next_nc_font_path,	_printinfo,			prepareExportGCodeData.node_groupstr,	
													next_nc_font_path,	_MFirst2,			_MLast2,			_PZFirst2,		
													_PZLast2,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
													prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace, PanelCommnets);
						}
						else		// 工位1
						{
							knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
													prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
													_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
													_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
													prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace, PanelCommnets);
						}
					}
					else
					{

						knifeClass.OutputGcode(	pThePanel,		nc_font_path,	tb_font_path,		prepareExportGCodeData.node_print,			prepareExportGCodeData.node_double,
												prepareExportGCodeData.node_sysuse,	0,				panel_index+1,		next_tb_font_path,	next_nc_font_path,
												_printinfo,		prepareExportGCodeData.node_groupstr,	next_nc_font_path,	_MFirst1,			_MLast1,	
												_PZFirst1,		_PZLast1,		prepareExportGCodeData._lessKnifeChange,	prepareExportGCodeData._knife_shifting,	prepareExportGCodeData.node_dvcut,
												prepareExportGCodeData._isLabelInReverseSide, total_check_panel_num, DecimalPlace, PanelCommnets);
					}

					//PostProcessOfExportGCode();
				}
				catch(...)
				{
					MessageBox("NC 生成出错！");
					//PostProcessOfExportGCode();
					return 1;
				}

				TransCoordinateData transCoordinateData;
				transCoordinateData.nc_font_temp_path = nc_font_temp_path;
				transCoordinateData.nc_font_path = nc_font_path;
				transCoordinateData.nc_back_w_path = nc_back_w_path;
				transCoordinateData.nc_back_w_temp_path = nc_back_w_temp_path;
				transCoordinateData.nc_back_h_path = nc_back_h_path;
				transCoordinateData.nc_back_h_temp_path = nc_back_h_temp_path;
				transCoordinateData.HaveDvsh = HaveDvsh;
				transCoordinateData.HaveDvsw = HaveDvsw;
				transCoordinateData.HaveDvs = HaveDvs;
				transCoordinateData._usedLineX = _usedLineX;
				transCoordinateData._usedLineY = _usedLineY;
				transCoordinateData.nDecimal = DecimalPlace + 1;
				AfterExportGCode_TransCoordinate(&prepareExportGCodeData, &transCoordinateData);

				prepareExportGCodeData.key++;
				//文件合并

#if 1
				CombineFileData combineFileData;
				combineFileData.str_nextthin = str_nextthin;
				combineFileData.tempnextcname2 = tempnextcname2;
				combineFileData.str_thin = str_thin;
				combineFileData.nc_font_temp_path = nc_font_temp_path;
				combineFileData.nc_font_path = nc_font_path;
				combineFileData.nc_back_h_path = nc_back_h_path;
				combineFileData.nc_back_w_path = nc_back_w_path;
				combineFileData.allpath2 = allpath2;
				AfterExportGCode_CombineFile(&prepareExportGCodeData, &combineFileData);

#endif

				//L4Cpp::Log()->debug("7.file clear");
				//对应的 NC 文件移动到当前目录的 cut目录 下
				//对应的 TB贴标 文件移动到当前目录的 print目录 下
				if (selectMachine.m_fileClassifyMove == "1")
				{
					rename(tb_font_path.c_str(),tb_font_new_path.c_str());
				}
#if 1			
				//NC文件命令字变更
				if (selectMachine.m_NC_CommandChange == "1")
				{
					try
					{
						Change_G0X_To_GX(nc_font_path);
						Change_G0X_To_GX(nc_back_w_path);
						Change_G0X_To_GX(nc_back_h_path);
					}
					catch (...)
					{
						MessageBox("NC文件扫描处理 更改CMD命令字 错误");
						return 1;
					}
				}

				//NC坐标空格删除
				if (selectMachine.m_NC_SpaceRemove == "1")
				{
					try
					{
						Change_PosSpace_To_Empty(nc_font_path);
						Change_PosSpace_To_Empty(nc_back_w_path);
						Change_PosSpace_To_Empty(nc_back_h_path);
					}
					catch (...)
					{
						MessageBox("NC文件扫描处理 NC坐标空格删除 错误");
						return 1;
					}
				}

				//NC修改铣弧格式,将“G90 G03 X-134.0000 Y-568.0000 R14.0000”格式，改为“G03 X-134.0000 Y-568.0000 U14.0000”
				if (selectMachine.m_NC_ModifyArcMillingFormat == "1")
				{
					try
					{
						Change_Arc_Milling_Format(nc_font_path);
						Change_Arc_Milling_Format(nc_back_w_path);
						Change_Arc_Milling_Format(nc_back_h_path);
					}
					catch (...)
					{
						MessageBox("NC文件扫描处理 修改铣弧格式 错误");
						return 1;
					}
				}

				std::string a_string_OutputFile[] = {nc_font_path, nc_back_w_path, nc_back_h_path};
				for(int j = 0; j < 3; j++)
				{
					std::string stringFile = a_string_OutputFile[j];
					CString strFileName = GetFileNameInPath_WithSuffix(stringFile.c_str());
					if(strFileName.Find(_T("DELETE")) != -1)
						DeleteFile(stringFile.c_str());
					else if(strFileName.Find(_T("@H")) != -1)
					{
						CString strFileRename = stringFile.c_str();
						strFileRename.Replace(_T("@H"), _T(""));
						rename(stringFile.c_str(), strFileRename);
					}
					else if(strFileName.Find(_T("@W")) != -1)
					{
						CString strFileRename = stringFile.c_str();
						strFileRename.Replace(_T("@W"), _T(""));
						rename(stringFile.c_str(), strFileRename);
					}
				}
#endif


/*				SyntecDataItem syntecDataItem = {nc_font_path.c_str(), pThePanel};
				vSyntecData.push_back(syntecDataItem)*/;
			}

			if (selectMachine.m_SmallPanelNC == "1")
			{
				try
				{
					//OnOutputSmallPanelGcode(&selectMachine,NCSavePath);
				}
				catch (...)
				{
					MessageBox("导出小板NC产生异常");
					return 1;
				}
			}

			//if(atoi(selectMachine.m_printname.c_str()) == LabelMachine::SyntecCycle)
			//	SyntecSummaryOutput(vSyntecData, /*prepareExportGCodeData.nodecoor_type*/0, prepareExportGCodeData.pathName);

			AfxMessageBox(_T("导出完毕"));
			return 0;
}

void CCarvingView::DeleteSpecialCode(std::string &_code)//删除^123^之间内容
{
	int posbegin = _code.find("^");
	if (posbegin<0)
	{
		return ;
	}
	int posend = 0;
	posend = _code.rfind('^');
	if(posend<0)
	{
		return;
	}
	int num = posend - posbegin;
	if (num<=0)
	{
		return;
	}
	else
	{
		_code.erase(posbegin,posend+1);
	}
}

void CCarvingView::DeleteSpecialCode2(std::string &_code)//删除‘^’符号
{
	int posbegin = _code.find("^");
	std::string::size_type startpos = 0;
	while(startpos!=std::string::npos)
	{
		startpos = _code.find("^");
		if (startpos!=std::string::npos)
		{
			_code.replace(startpos,1,""); //将“\\”替换为空 
		} 
	}
}

//void CCarvingView::TMP_ConvertInputItemToOldInputItem(vector<map<string,string>>& vItem)
//{
//	for (int i = 0;i < m_vComponentInputItem.size();i++)
//	{
//		map<string,string> mapItem;
//		mapItem["order"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,0));	//订单号
//		mapItem["cabinet"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,1));	//柜号
//		mapItem["info"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,2));	//条码
//		mapItem["PorductName"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,3));	//产品名称
//		mapItem["porduct"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,4));	//成品名称
//		mapItem["matrial"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,5));	//材质
//		mapItem["width"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,6));	//长
//		mapItem["height"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,7));	//宽
//		mapItem["thin"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,8));	//厚度
//		mapItem["num"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,9));	//切割数量
//		mapItem["dir"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,10));	//纹理方向
//		mapItem["edgeLen1"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,11));	//封长1
//		mapItem["edgeWidth1"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,12));	//封宽1
//		mapItem["edgeLen2"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,13));	//封长2
//		mapItem["edgeWidth2"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,14));	//封宽2
//		mapItem["pointInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,15));	//异形信息
//		mapItem["rotate"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,16));	//是否旋转
//		mapItem["slotInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,17));	//槽
//		mapItem["slotRotate"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,18));	//槽翻转
//		mapItem["dslotInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,19));	//反面槽
//		mapItem["vecInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,20));	//孔
//		mapItem["vecRotate"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,21));	//孔翻转
//		mapItem["dvecInfo"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,22));	//反面孔
//		mapItem["customeInfo"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,23));	//客户信息
//		mapItem["store"]			 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,24));	//加盟店
//		mapItem["SawMark"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,25));	//拉槽标识
//		mapItem["HelpSign"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,26));	//助记号
//		mapItem["Vertical"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,27));	//钻孔
//		mapItem["OrderType"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,28));	//订单类型
//		mapItem["BackCode"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,29));	//反面条码
//		mapItem["ProductWidth"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,30));	//成品长度
//		mapItem["ProductHeight"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,31));	//成品宽度
//		mapItem["ProductThin"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,32));	//成品厚度
//		mapItem["otherID"]		 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,33));	//异型ID
//		mapItem["customeAddr"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,34));	//客户地址
//		mapItem["DrillingLogo"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,35));	//钻槽标识
//		mapItem["OtherPoints"]	 = HGTools::Convert2StdString(m_matriallist.GetItemText(index,36));	//异形点信息
//	}

//处理每行添加内容
bool CCarvingView::eachLineAddChar(std::string patho,CString word)
{
	if (HGTools::isFileExists(patho.c_str()))
	{
		CString path;
		path.Format("%s",patho.c_str());

		CStdioFile m_filew;
		CStdioFile m_filer;
		if(m_filew.Open(path+"t",CFile::modeWrite|CFile::modeCreate)){
			if(m_filer.Open(path,CFile::modeRead)){
				try
				{
					CString contents;  
					while (m_filer.ReadString(contents))  
					{  
						if (contents != "")
						{
							contents+=word;
							contents+="\n";
							m_filew.WriteString(contents);
						}				 
					}  	

					m_filer.Close();
					m_filew.Close();
					DeleteFile(path);
					MoveFile(path+"t",path);
					return true;
				}
				catch (...)
				{
					return false;
				}
			}
		}
		return false;
	}
	return false;
}

std::string CCarvingView::ReplaceWithNextPath(std::string _code,std::string theNextPath)//替换*<abc>
{
	int posbegin = _code.find_first_of("*<");
	if (posbegin<0)
	{
		return _code;
	}
	int posend = 0;
	char temp[1000] = { };
	int j = 0;
	for (int i = posbegin+2;i<_code.size();i++)
	{
		if (_code.at(i)!='>')
		{
			temp[j++] = _code.at(i);
		}
		else
		{
			temp[j] ='\0';
			posend = i;
			break;
		}		
	}
	if (theNextPath!="")
	{
		_code.replace(posbegin,abs(posend-posbegin+1),theNextPath.c_str());
	}
	else
	{
		_code.replace(posbegin,abs(posend-posbegin+1),temp);
	}
	return _code;
}

//刷新NC文件：命令字 G0X 变为 GX
void CCarvingView::Change_G0X_To_GX(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			for (int g = 0;g < get_str.size();g++)
			{
				new_str.push_back(get_str.at(g));
				if (g > 0)
				{
					bool g_command = (get_str.at(g-1) == 'G' && get_str.at(g) == '0');

					if (g_command) //G后面跟0跳过一次不保存这个数据
					{
						new_str.pop_back();
					}
				}
			}

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	//wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}

//刷新NC文件：坐标 X *** Y *** Z *** F *** R *** -> X*** Y*** Z*** F*** R***
void CCarvingView::Change_PosSpace_To_Empty(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			for (int g = 0;g < get_str.size();g++)
			{
				new_str.push_back(get_str.at(g));
				if (g > 0)
				{
					bool xchange = (get_str.at(g-1) == 'X' && get_str.at(g) == ' ');
					bool ychange = (get_str.at(g-1) == 'Y' && get_str.at(g) == ' ');
					bool zchange = (get_str.at(g-1) == 'Z' && get_str.at(g) == ' ');
					bool fchange = (get_str.at(g-1) == 'F' && get_str.at(g) == ' ');
					bool rchange = (get_str.at(g-1) == 'R' && get_str.at(g) == ' ');

					int compare_size = 5;
					bool compare[] = {xchange,ychange,zchange,fchange,rchange};

					for (int c_index = 0; c_index < compare_size ; c_index++)
					{
						if (compare[c_index]) //跳过空格
						{
							new_str.pop_back();
						}
					}

				}
			}

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	//wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}


// 处理多余的回车\r和换行\n
void  CCarvingView::dealRepeatCharReturnNewline(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			// 去除\r
			if (get_str.find("\r") != std::string::npos)
			{
				get_str.erase(std::remove(get_str.begin(), get_str.end(), '\r'), get_str.end());
			}

			// 去除重复的\n
			CString strOrg = get_str.c_str();
			if(strOrg.Find("\n\n") != -1)
			{
				strOrg.Replace("\n\n", "\n");
				
				new_str = strOrg.GetBuffer();
				strOrg.ReleaseBuffer();
			}
			else
				new_str = get_str;

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	//wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}





//刷新NC文件：修改铣弧格式,将“G90 G03 X*** Y*** R***”格式，改为“G03 X*** Y*** U***”
void CCarvingView::Change_Arc_Milling_Format(std::string fileName)
{
	if ( !HGTools::isFileExists(HGTools::Convert2CString(fileName)) )
	{
		return;
	}

	std::string tempfileName = fileName + "_";
	std::ofstream wfile(tempfileName.c_str(),std::ios::out);
	std::ifstream rfile(fileName.c_str(),std::ios::in);

	try
	{
		while(!rfile.eof())
		{	
			char str[1023];
			rfile.getline(str,1023);
			std::string get_str = str;
			std::string new_str = "";

			CStringA strOrg = get_str.c_str();
			if(strOrg.Find("G90") != -1 && (strOrg.Find("G02") != -1 || strOrg.Find("G2") != -1) || strOrg.Find("G03") != -1 || strOrg.Find("G3") != -1)
			{
				strOrg.Replace("G90 ", "");
				strOrg.Replace("G90", "");
				strOrg.Replace("R", "U");
				new_str = strOrg.GetBuffer();
				strOrg.ReleaseBuffer();
			}
			else
				new_str = get_str;

			if (new_str.size() > 0)
			{
				wfile<<new_str<<std::endl;
			}
		}
	}
	catch (...)
	{

	}

	//wfile<<endl;
	wfile.close();
	rfile.close();



	DeleteFile(fileName.c_str());
	rename(tempfileName.c_str(),fileName.c_str());
}

void CCarvingView::OnLayoutOrg()
{
	// TODO: 在此添加命令处理程序代码
}


void CCarvingView::OnButtonPrintLabel()
{
	// TODO: 在此添加命令处理程序代码
	CSelectPrintDlg dlg;
	dlg.DoModal();
}


BOOL CCarvingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CCarvingView::OnMenuMachineParamSetting()
{
	// TODO: 在此添加命令处理程序代码
	CKnifeAddDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnMenuBaseSetting()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_pDlgBaseParam->DoModal();
}


void CCarvingView::OnMenuOptimizeSetting()
{
	// TODO: 在此添加命令处理程序代码
	COPSetDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnRemainderSetting()
{
	// TODO: 在此添加命令处理程序代码
	CLeftPanelDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnMenuDefaultLoadingDirSetting()
{
	// TODO: 在此添加命令处理程序代码
	SelectPathDlg dlg("设置文件默认加载路径");
	dlg.CurrentPath(dlg.DefaultLoadPath());
	if (dlg.DoModal() == IDOK)
	{
		dlg.DefaultLoadPath(dlg.CurrentPath());
		dlg.saveSettings();
	}
}


void CCarvingView::OnMenuDefaultSavingDirSetting()
{
	// TODO: 在此添加命令处理程序代码
	SelectPathDlg dlg("设置文件默认保存路径");
	dlg.CurrentPath(dlg.DefaultSavePath());
	if (dlg.DoModal() == IDOK)
	{
		dlg.DefaultSavePath(dlg.CurrentPath());
		dlg.saveSettings();
	}
}


void CCarvingView::OnMenuOtherSetting()
{
	// TODO: 在此添加命令处理程序代码
	UISettingDlg dlg;
	dlg.DoModal();
}


void CCarvingView::OnMenuLabelSetting()
{
	// TODO: 在此添加命令处理程序代码
	CPrintSetDlg dlg;
	dlg.DoModal();
}



void CCarvingView::OnMenuExcelTemplateSetting()
{
	// TODO: 在此添加命令处理程序代码
	//CInOutDlg dlg;
	//dlg.DoModal();

	SelectPathDlg pathDlg;
	CString strFilePath = pathDlg.SelectFileDlgDoModel(
		"导入开料单(excel)",
		"xls",
		"*",
		pathDlg.DefaultLoadPath(),
		DlgLoad
		);

	CDlgExcelInputSetting dlg(strFilePath);
	dlg.DoModal();
}

void CCarvingView::OnMenuLingliaoTableSetting()
{
	// TODO: 在此添加命令处理程序代码
	LingLiaoDlg dlg;
	dlg.DoModal();
}


void CCarvingView::ExportMeterialTable1(vector<Panel*>& vPanelToExport)
{
	CString tempReadName = GetProjectName();
	tempReadName.Replace("雕刻机对接-","");

	SelectPathDlg pathDlg;
	CString pathName = pathDlg.SelectFileDlgDoModel(
		"保存",
		"csv",
		"移出板件-" + tempReadName,
		pathDlg.DefaultSavePath(),
		DlgSave
		);

	if( pathName != "" )
	{
		ParamSettingDlg settings;
		settings.loadSettings();
		TiebiaoEdgeDlg tiebiao;
		tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());


		int nComponentInputItemCount = m_vComponentInputItem.size();
		int* aExportCount = new int[nComponentInputItemCount];
		for(int i = 0; i < nComponentInputItemCount; i++)
			aExportCount[i] = 0;

		for(int i = 0; i < vPanelToExport.size(); i++)
		{
			vector<Component*> vAllComponent;
			FindAllComponentInPanel((Component&)(*(vPanelToExport[i])), vAllComponent);
			for(int j = 0; j < vAllComponent.size(); j++)
			{
				int nIndexOfTheComponent = -1;
				for(int k = 0; k < nComponentInputItemCount; k++)
				{
					if(m_vComponentInputItem[k].m_strBarcode.CompareNoCase(vAllComponent[j]->m_BarCode) == 0)
					{
						nIndexOfTheComponent = k;
						break;
					}
				}
				if(nIndexOfTheComponent != -1)
					aExportCount[nIndexOfTheComponent]++;
			}

		}

		//所有小板数据
		std::vector<std::map<std::string,std::string>> resultArr;
		for(int i = 0; i < nComponentInputItemCount; i++)
		{
			if(aExportCount[i] > 0)
			{
				map<std::string,std::string> mapOneExportItem = GetMapByComponentInputItem(m_vComponentInputItem[i]);//m_pDlgRequirement->GetMatrialItemsOne(i);
				CString strNum;
				strNum.Format("%d", aExportCount[i]);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				mapOneExportItem["num"] = strNum;
				mapOneExportItem["pointInfo"] = ""	;
				mapOneExportItem["slotInfo"]	 = ""	;
				mapOneExportItem["dslotInfo"] = ""	;
				mapOneExportItem["vecInfo"] = ""		;
				mapOneExportItem["dvecInfo"] = ""	;
				mapOneExportItem["OtherPoints"] = ""	;
				mapOneExportItem["edgeLen1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen1"].c_str())).c_str();//"封长1");
				mapOneExportItem["edgeWidth1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth1"].c_str())).c_str();//"封宽1");
				mapOneExportItem["edgeLen2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen2"].c_str())).c_str();//"封长2");
				mapOneExportItem["edgeWidth2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth2"].c_str())).c_str();//"封宽2");
				resultArr.push_back(mapOneExportItem);
			}

		}

		delete[] aExportCount;

		std::vector<std::string> title;
		for(int i = 0; i < sizeof(g_aRequirementListTitle) / sizeof(char*); i++)
		{
			title.push_back(g_aRequirementListTitle[i]);
		}
		std::vector<std::string> dataCol;
		for(int i = 0; i < sizeof(g_aRequirementListTitle_ENU) / sizeof(char*); i++)
		{
			dataCol.push_back(g_aRequirementListTitle_ENU[i]);
		}
		//按照EXCEL的定义生成excel
		OnWriteDataCSV(pathName, title, dataCol, resultArr);

		MessageBox("保存完成");
	}
}

void CCarvingView::ExportMeterialTable2(vector<Panel*>& vPanelToExport)
{
	CString tempReadName = GetProjectName();
	tempReadName.Replace("雕刻机对接-","");

	SelectPathDlg pathDlg;
	CString pathName = pathDlg.SelectFileDlgDoModel(
		"保存",
		"csv",
		"移出板件-" + tempReadName,
		pathDlg.DefaultSavePath(),
		DlgSave
		);

	if( pathName != "" )
	{
		ParamSettingDlg settings;
		settings.loadSettings();
		TiebiaoEdgeDlg tiebiao;
		tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());

		
		int nComponentInputItemCount = m_vComponentInputItem.size();
		int* aExportCount = new int[nComponentInputItemCount];
		for(int i = 0; i < nComponentInputItemCount; i++)
			aExportCount[i] = 0;

		for(int i = 0; i < vPanelToExport.size(); i++)
		{
			vector<Component*> vAllComponent;
			FindAllComponentInPanel((Component&)(*(vPanelToExport[i])), vAllComponent);
			for(int j = 0; j < vAllComponent.size(); j++)
			{
				int nIndexOfTheComponent = -1;
				for(int k = 0; k < nComponentInputItemCount; k++)
				{
					if(m_vComponentInputItem[k].m_strBarcode.CompareNoCase(vAllComponent[j]->m_BarCode) == 0)
					{
						nIndexOfTheComponent = k;
						break;
					}
				}
				if(nIndexOfTheComponent != -1)
					aExportCount[nIndexOfTheComponent]++;
			}

		}

		//所有小板数据
		std::vector<std::map<std::string,std::string>> resultArr;
		for(int i = 0; i < nComponentInputItemCount; i++)
		{
			if(aExportCount[i] > 0)
			{
				map<std::string,std::string> mapOneExportItem = GetMapByComponentInputItem(m_vComponentInputItem[i]);//m_pDlgRequirement->GetMatrialItemsOne(i);
				CString strNum;
				strNum.Format("%d", aExportCount[i]);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				mapOneExportItem["num"] = strNum;
				mapOneExportItem["pointInfo"] = ""	;
				mapOneExportItem["slotInfo"]	 = ""	;
				mapOneExportItem["dslotInfo"] = ""	;
				mapOneExportItem["vecInfo"] = ""		;
				mapOneExportItem["dvecInfo"] = ""	;
				mapOneExportItem["OtherPoints"] = ""	;
				mapOneExportItem["edgeLen1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen1"].c_str())).c_str();//"封长1");
				mapOneExportItem["edgeWidth1"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth1"].c_str())).c_str();//"封宽1");
				mapOneExportItem["edgeLen2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeLen2"].c_str())).c_str();//"封长2");
				mapOneExportItem["edgeWidth2"] = tiebiao.findCharByEdge(atof(mapOneExportItem["edgeWidth2"].c_str())).c_str();//"封宽2");

				CString strHoleSlotFlag = mapOneExportItem["DrillingLogo"].c_str();
				if(strHoleSlotFlag.Find(_T("#")) == -1)
				{
					mapOneExportItem["DrillingLogoA"] = "";
					mapOneExportItem["DrillingLogoB"] = "";
				}
				else
				{
					int nPos = strHoleSlotFlag.Find(_T("#"));
					CString strA, strB;
					strA = strHoleSlotFlag.Left(nPos);
					strB = strHoleSlotFlag.Right(strHoleSlotFlag.GetLength() - nPos - 1);

					mapOneExportItem["DrillingLogoA"] = strA.GetBuffer();
					mapOneExportItem["DrillingLogoB"] = strB.GetBuffer();
					strA.ReleaseBuffer();
					strB.ReleaseBuffer();
				}

				resultArr.push_back(mapOneExportItem);
			}

		}

		delete[] aExportCount;

		std::vector<std::string> title;
		for(int i = 0; i < sizeof(g_aMaterialListItem) / sizeof(int); i++)
		{
			title.push_back(g_aRequirementListTitle[g_aMaterialListItem[i]]);
		}
		title.push_back("A面钻槽标识");
		title.push_back("B面钻槽标识");
		std::vector<std::string> dataCol;
		for(int i = 0; i < sizeof(g_aMaterialListItem) / sizeof(int); i++)
		{
			dataCol.push_back(g_aRequirementListTitle_ENU[g_aMaterialListItem[i]]);
		}
		dataCol.push_back("DrillingLogoA");
		dataCol.push_back("DrillingLogoB");
		//按照EXCEL的定义生成excel
		OnWriteDataCSV(pathName, title, dataCol, resultArr);

		MessageBox("保存完成");
	}
}

void CCarvingView::OnExportMeterialTable1()
{
	// TODO: 在此添加命令处理程序代码

	//所有选中的板件(把雕刻机不擅长加工的板件变成料单)
	if(m_pDlgResult == NULL)
		return;
	vector<Panel*> vUncheckedPanel = m_pDlgResult->GetPanels(false);
	ExportMeterialTable1(vUncheckedPanel);
}

CString CCarvingView::GetProjectName()
{
	CString strRet;
	if(m_vOpenedFile.size() > 0 && !m_vOpenedFile[m_vOpenedFile.size()-1].IsEmpty())
		strRet = HGTools::getFileName(m_vOpenedFile[m_vOpenedFile.size()-1]);
	return strRet;
}

//写CSV
void CCarvingView::OnWriteDataCSV(CString savePath, std::vector<std::string> title,std::vector<std::string> dataMap, std::vector<std::map<std::string,std::string>> dataSrc)
{
	if (HGTools::isFileExists(savePath))
	{
		HGTools::deleteFile(savePath);
	}


	std::ofstream csv_file(savePath.GetBuffer(), std::ios::out);

	//标题
	if (1)
	{
		for (int i = 0; i < title.size() ; i++)
		{
			csv_file << title[i] << ",";
		}
		csv_file << std::endl;
	}

	//正文
	if (1)
	{
		for (int i = 0; i < dataSrc.size() ; i++)
		{
			for (int col = 0; col < dataMap.size() ; col++)
			{
				csv_file << dataSrc[i][dataMap[col]] << ",";
			}
			csv_file << std::endl ;
		}
	}
	csv_file.close();
}


void CCarvingView::OnExportMeterialTable2()
{
	// TODO: 在此添加命令处理程序代码
	//所有选中的板件(把雕刻机不擅长加工的板件变成料单)
	if(m_pDlgResult == NULL)
		return;
	vector<Panel*> vUncheckedPanel = m_pDlgResult->GetPanels(false);
	ExportMeterialTable2(vUncheckedPanel);
}


void CCarvingView::OnOpenSourceExcel()
{
	// TODO: 在此添加命令处理程序代码
	CSingleon* pSingleton = CSingleon::GetSingleton();
	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	CString filter = "xls 文件(*.xls)|*.xls|所有文件 (*.*)|*.*||";

	CFileDialog fileDlg(true, _T("xls"), _T("*.xls"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	fileDlg.m_ofn.lpstrTitle = _T("导入开料单(excel)");
	if ( fileDlg.DoModal() == IDOK)
	{
		CString strOpenedFile = fileDlg.GetPathName();
		m_vOpenedFile.clear();
		m_vOpenedFile.push_back(strOpenedFile);

		ExcelSettingForTheFistTime(strOpenedFile);

		// 先清除上一次的数据
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xls" || strExtName == "xlsx" )
		{
			GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

			CParamDlg* param_dlg;
			param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

			SourceFilePreProccesParam param;
			param.b_upvecImportEnable		= param_dlg->VecInput();			//正面孔
			param.b_downvecImportEnable		= param_dlg->DVecInput();			//反面孔
			param.b_upsoltImportEnable		= param_dlg->SoltInput();			//正面槽
			param.b_downsoltImportEnable	= param_dlg->DSoltInput();			//反面槽
			param.b_othershapeImportEnable	= param_dlg->OthershapeInput();		//异形过滤
			param.i_comnum					= param_dlg->Editnum();				//切割数量
			param.i_comchange				= param_dlg->Comchange();			//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
			param.f_changex					= param_dlg->MoveX();				//孔槽偏移
			param.f_changey					= param_dlg->MoveY();				//孔槽偏移	
			param.vecAdder					= param_dlg->GetVecAdder();			//孔位偏差（找到指定孔直径的孔，其直径加一个值）		
			param._cxy						= param_dlg->GetVecFilter();		//孔位过滤（找到指定孔直径的孔，并移除）
			param.f_width					= param_dlg->Width();
			param.f_height					= param_dlg->Height();
			param.f_Deburringwidth			= param_dlg->Deburringwidth();
			param.bReserveDeepHole = param_dlg->ReverseDeepHole();
			param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();

			if(ExcelReadWrite::ReadPanelInfo(strOpenedFile, m_vComponentInputItem, param) == true)
			{

				//// 检查条码重复
				//CheckRepeatBarcode(m_vComponentInputItem);
			}
			else
			{
				if (strExtName == "xlsx")
				{
					AfxMessageBox("由于office兼容性问题，系统无法使用Excel 2003 自动读取 Excel 2007保存的 xlsx文件! \n请双击" + strOpenedFile + ",另存为xls文件后，再导入！");
				}

			}


			//CheckComponentList(m_vComponentInputItem);
			// 小板设为无纹理
			if (param_dlg->m_CheckSmallCpnNoTexture == TRUE)
			{
				SetSmallCpnNoTexture(m_vComponentInputItem);
			}

			for(int i = 0; i < m_vComponentInputItem.size(); i++)
				m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

			ParamSettingDlg dlgOtherSetting;
			dlgOtherSetting.loadSettings();
			ReminderModifyType eReminderModifyType = dlgOtherSetting.ReminderModify();
			if(eReminderModifyType == ReminderModifyType::REMINDER_MODIFY_DATE_AND_ORDER_FORMAT)
			{
				ModifyReminderByDateAndOrderFormat(m_vComponentInputItem);
			}


			// 计算小板
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "块小板";
			AfxMessageBox(str);

			
			// 自动计算优化参数
			COPSetDlg dlgOptimizeSetting;

			dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);

			RecordMaterial();
		}

		// 切换视图
		SwitchView(VIEW_PANEL_LIST);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();
	}

	// 设置界面需切小板数据结构
	m_pDlgRequirement->SetRequirement(m_vComponentInputItem);

	// 备份输入板件组
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);


}


void CCarvingView::OnOpenSolution()
{
	// TODO: 在此添加命令处理程序代码


	CSingleon* pSingleton = CSingleon::GetSingleton();

	CString strTmp;
#ifndef YiWei
	strTmp = _T("hgo");
	CString filter = "hgo 文件(*.hgo)|*.hgo|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hgo"), _T("*.hgo"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
#else
	strTmp = _T("ywo");
	CString filter = "ywo 文件(*.ywo)|*.ywo|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("ywo"), _T("*.ywo"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
#endif
	
	CString strDefDir = SelectPathDlg().DefaultLoadPath();
	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	if ( fileDlg.DoModal() == IDOK)
	{

		// 先清除上一次的数据
		//pSingleton->ClearAllData();
		ClearAllData();
		m_vComponentInputItem.clear();

		CString strOpenedFile = fileDlg.GetPathName();
		m_vOpenedFile.clear();
		m_vOpenedFile.push_back(strOpenedFile);

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName.CompareNoCase(strTmp) == 0)
		{
			if(HgyReadWrite::ReadHgy(strOpenedFile) != true)
			{
				AfxMessageBox("优化文件出错!");
			}
			else
			{
				GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

				m_vComponentInputItem = pSingleton->m_vBackupComponentInputItem;

				for(int i = 0; i < m_vComponentInputItem.size(); i++)
					m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				// 检查板件是否有超出尺寸范围的，删掉
				//CheckComponentList(m_vComponentInputItem);
			}

		}

		// 计算小板
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}


		// 自动计算优化参数
		COPSetDlg dlgOptimizeSetting;

		dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);


		// 切换视图
		SwitchView(VIEW_RESULT);

		if (m_pDlgResult != NULL)
			m_pDlgResult->ResetResultDlg();

		if (m_pDlgRequirement != NULL)
			m_pDlgRequirement->SetRequirement(m_vComponentInputItem);


		// 备份输入板件组
		pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);
	}
}


void CCarvingView::OnExportPanelMeterialTable()
{
	// TODO: 在此添加命令处理程序代码
	CString tempReadName = GetProjectName();
	tempReadName.Replace("雕刻机对接-","");

	SelectPathDlg pathDlg;
	CString savePath = pathDlg.SelectFileDlgDoModel(
		"保存",
		"xlsx",
		"领料单-" + tempReadName ,
		pathDlg.DefaultSavePath(),
		DlgSave
		);

	if( savePath != "" )
	{
		//保存目录
		CString path = HGTools::getContainPath(savePath);


		//////////////////////////////////////////////////////////////////////////
		// 数据头
#if 1
		std::vector<std::vector<std::string>> infos;

		//用户信息
		std::vector<std::string> cusInfo;
		cusInfo.push_back("CompanyLogoUrl");
		cusInfo.push_back("CompanyName");
		cusInfo.push_back("OrderCodeUrl");
		cusInfo.push_back("OrderID");
		cusInfo.push_back("ShiftEmployee");
		cusInfo.push_back("ShiftDate");
		cusInfo.push_back("UseEmployee");
		cusInfo.push_back("UseDate");
		cusInfo.push_back("ManageEmployee");
		cusInfo.push_back("OrderStore");
		cusInfo.push_back("AuditEmployee");
		cusInfo.push_back("UseMachineName");
		cusInfo.push_back("OrderMoreInfo");
		infos.push_back(cusInfo);

		//板件使用信息
		std::vector<std::string> dataCol;
		dataCol.push_back("Index");
		dataCol.push_back("Thickness");
		dataCol.push_back("WidthHeight");
		dataCol.push_back("Code");
		dataCol.push_back("PanelMatrial");
		dataCol.push_back("PanelNum");
		dataCol.push_back("Units");
		dataCol.push_back("Brand");
		dataCol.push_back("Remark");
		infos.push_back(dataCol);

		//占用面积及小板
		std::vector<std::string> smallCol;
		smallCol.push_back("PanelMatrial");
		smallCol.push_back("Thickness");
		smallCol.push_back("SmallPanelNum");
		smallCol.push_back("TotalArea");
		infos.push_back(smallCol);


		CString excelFormat = path + "\\excel.xsd" ;
		SaveExcelFormat(excelFormat,infos);

#endif

		LingLiaoDlg lingliao;
		lingliao.loadSettings();

		//////////////////////////////////////////////////////////////////////////
		// 数据内容
#if 1
		std::vector<std::vector<std::map<std::string,std::string>>> dataSrcs;

		//用户信息
		std::vector<std::map<std::string,std::string>> cusData;
		std::map<std::string,std::string> cusDataMap;
		cusDataMap["CompanyLogoUrl"] =	"";
		cusDataMap["CompanyName"] =		lingliao.CompanyName().GetBuffer();
		cusDataMap["OrderCodeUrl"] =	"";
		cusDataMap["OrderID"] =			lingliao.OrderID().GetBuffer();
		cusDataMap["ShiftEmployee"] =	lingliao.ShiftEmployee().GetBuffer();
		cusDataMap["ShiftDate"] =		lingliao.ShiftDate().GetBuffer();
		cusDataMap["UseEmployee"] =		lingliao.UseEmployee().GetBuffer();
		cusDataMap["UseDate"] =			lingliao.UseDate().GetBuffer();
		cusDataMap["ManageEmployee"] =	lingliao.ManageEmployee().GetBuffer();
		cusDataMap["OrderStore"] =		lingliao.OrderStore().GetBuffer();
		cusDataMap["AuditEmployee"] =	lingliao.AuditEmployee().GetBuffer();
		cusDataMap["UseMachineName"] =	lingliao.UseMachineName().GetBuffer();
		cusDataMap["OrderMoreInfo"] =	lingliao.OrderMoreInfo().GetBuffer();
		cusData.push_back(cusDataMap);
		dataSrcs.push_back(cusData);

		//板件使用信息
		std::vector<std::map<std::string,std::string>> panelInfos;
		std::map<std::string,std::string> panelInfo;


		CString inputvalue;
		//遍历所有大板
		int panelMatrialIndex = 1;
		//for (int i = 0;i<m_picturedlg->_picturecoor.size();i++)
		//{
		//	//所有选中的板件(把雕刻机需要加工的材料料单导出)
		//	if (m_picturedlg->m_piclist.m_panelChecked[i] == false)
		//	{
		//		continue;
		//	}

		//	Panel_Knife panelKnife = m_picturedlg->_picturecoor[i];	//找到当前板件
		//	vector<Coordinate> _coors = panelKnife._coor;
		//	KnifeClass* _knifeClass = panelKnife._kc;
		//	PanelNode* rootNode = panelKnife._panel;
		//	vector<gcodeCoor> handcoor = panelKnife._kc->getNewcoor(rootNode);

		//	//查找是否已有材质
		//	bool isFindSameMatrial = false;
		//	int find_index = 0;
		//	for (; find_index < panelInfos.size() ; find_index++)
		//	{
		//		if (panelInfos[find_index]["ThinMatrial"].compare( _knifeClass->getThinMatrial() ) == 0)
		//		{
		//			isFindSameMatrial = true;
		//			break;
		//		}
		//	}

		//	//区分插入和更改列表中的耗材
		//	if (isFindSameMatrial)
		//	{
		//		int panelNum = atoi(panelInfos[find_index]["PanelNum"].c_str());
		//		inputvalue.Format("%d",panelNum + panelKnife._panel->_num);
		//		panelInfos[find_index]["PanelNum"] = inputvalue.GetBuffer();

		//		int smallPanelNum = atoi(panelInfos[find_index]["SmallPanelNum"].c_str());
		//		inputvalue.Format("%d",_knifeClass->getcomnum(rootNode) + smallPanelNum);
		//		panelInfos[find_index]["SmallPanelNum"] = inputvalue.GetBuffer();


		//		float panelArea = atof(panelInfos[find_index]["TotalArea"].c_str());
		//		//找到所有小板
		//		for (int c = 0;c < handcoor.size();c++)
		//		{
		//			panelArea += (handcoor[c]._height / 1000) * (handcoor[c]._width / 1000);
		//		}
		//		inputvalue.Format("%0.2f",panelArea);
		//		panelInfos[find_index]["TotalArea"] = inputvalue.GetBuffer();
		//	}
		//	else
		//	{
		//		inputvalue.Format("%d",panelMatrialIndex++);
		//		panelInfo["Index"] = inputvalue.GetBuffer();

		//		panelInfo["Code"] = "";
		//		panelInfo["Brand"] = "";
		//		panelInfo["Remark"] = "";
		//		panelInfo["Units"] = "块";

		//		panelInfo["ThinMatrial"] = _knifeClass->getThinMatrial();
		//		panelInfo["PanelMatrial"] = _knifeClass->getPanelMatrial();

		//		inputvalue.Format("%0.1f",_knifeClass->getThickness());
		//		panelInfo["Thickness"] = inputvalue.GetBuffer();

		//		inputvalue.Format("%0.1f x %0.1f",_knifeClass->getPanelWidth(),_knifeClass->getPanelHeight());
		//		panelInfo["WidthHeight"] = inputvalue.GetBuffer();

		//		inputvalue.Format("%d",panelKnife._panel->_num);
		//		panelInfo["PanelNum"] = inputvalue.GetBuffer();

		//		inputvalue.Format("%d",_knifeClass->getcomnum(rootNode));
		//		panelInfo["SmallPanelNum"] = inputvalue.GetBuffer();


		//		float panelArea = 0.0;
		//		//找到所有小板
		//		for (int c = 0;c < handcoor.size();c++)
		//		{
		//			panelArea += (handcoor[c]._height / 1000) * (handcoor[c]._width / 1000);
		//		}
		//		inputvalue.Format("%0.2f",panelArea);
		//		panelInfo["TotalArea"] = inputvalue.GetBuffer();

		//		panelInfos.push_back(panelInfo);
		//		panelInfo.clear();
		//	}
		//}

		vector<Panel*> vCheckedPanel = m_pDlgResult->GetPanels(true);
		for(int i = 0; i < vCheckedPanel.size(); i++)
		{
			Panel& thePanel = *(vCheckedPanel[i]);
			CString strThicknessAndMeterial;
			strThicknessAndMeterial.Format(_T("%s-%s"), GetFloatString(thePanel.m_Thickness, 1), thePanel.m_Material);
			//查找是否已有材质
			bool isFindSameMatrial = false;
			int find_index = 0;
			for (; find_index < panelInfos.size() ; find_index++)
			{
				
				string stringThicknessAndMeterial = strThicknessAndMeterial;
				if (panelInfos[find_index]["ThinMatrial"].compare(stringThicknessAndMeterial) == 0)
				{
					isFindSameMatrial = true;
					break;
				}
			}

			//区分插入和更改列表中的耗材
			if (isFindSameMatrial)
			{
				int panelNum = atoi(panelInfos[find_index]["PanelNum"].c_str());
				inputvalue.Format("%d",panelNum + 1);
				panelInfos[find_index]["PanelNum"] = inputvalue.GetBuffer();

				int smallPanelNum = atoi(panelInfos[find_index]["SmallPanelNum"].c_str());
				inputvalue.Format("%d", GetComponentCountInPanel(thePanel) + smallPanelNum);
				panelInfos[find_index]["SmallPanelNum"] = inputvalue.GetBuffer();


				float panelArea = atof(panelInfos[find_index]["TotalArea"].c_str());
				//找到所有小板
				vector<Component*> vAllComponent;
				FindAllComponentInPanel(thePanel, vAllComponent);
				for(int j = 0; j < vAllComponent.size(); j++)
				{
					Component& theComponent = *(vAllComponent[j]);
					panelArea += (theComponent.m_RealLength / 1000) * (theComponent.m_RealWidth / 1000);
				}
				inputvalue.Format("%0.2f",panelArea);
				panelInfos[find_index]["TotalArea"] = inputvalue.GetBuffer();
			}
			else
			{
				panelInfo["Code"] = "";
				panelInfo["Brand"] = "";
				panelInfo["Remark"] = "";
				panelInfo["Units"] = "块";

				panelInfo["ThinMatrial"] = strThicknessAndMeterial;
				panelInfo["PanelMatrial"] = thePanel.m_Material;

				inputvalue.Format("%0.1f", thePanel.m_Thickness);
				panelInfo["Thickness"] = inputvalue.GetBuffer();

				inputvalue.Format("%0.1f x %0.1f",thePanel.m_OrgLen, thePanel.m_OrgWidth);
				panelInfo["WidthHeight"] = inputvalue.GetBuffer();

				inputvalue.Format("%d", 1);
				panelInfo["PanelNum"] = inputvalue.GetBuffer();

				inputvalue.Format("%d", GetComponentCountInPanel(thePanel));
				panelInfo["SmallPanelNum"] = inputvalue.GetBuffer();


				float panelArea = 0.0;
				//找到所有小板
				vector<Component*> vAllComponent;
				FindAllComponentInPanel(thePanel, vAllComponent);
				for(int j = 0; j < vAllComponent.size(); j++)
				{
					Component& theComponent = *(vAllComponent[j]);
					panelArea += (theComponent.m_RealLength / 1000) * (theComponent.m_RealWidth / 1000);
				}
				inputvalue.Format("%0.2f",panelArea);
				panelInfo["TotalArea"] = inputvalue.GetBuffer();

				panelInfos.push_back(panelInfo);
				panelInfo.clear();
			}
		}

		ParamSettingDlg dlgOtherSetting;
		dlgOtherSetting.loadSettings();
		LingLiaoSortType eLingLiaoSortType = dlgOtherSetting.LingLiaoSort();
		if(eLingLiaoSortType == LingLiao_SORT_THICKNESS_FIRST)
			std::sort(panelInfos.begin(), panelInfos.end(), SortMaterialListData);

		for(int i = 0; i < panelInfos.size(); i++)
			panelInfos[i]["Index"] = GetFloatString(i+1, 0).GetBuffer();

		dataSrcs.push_back(panelInfos);
		dataSrcs.push_back(panelInfos);

		CString excelData = path + "\\excel.xml" ;
		SaveExcelData(excelData, infos, dataSrcs);
#endif

		//数据排序
#if 1

#endif

		//数据输出
#if 1
		// 调用外部程序打开指定的 report 加这些数据
		CString ExcelExportCmdPath = HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("report\\ExcelExportCmd.exe"));
		CString ReportPath = HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("report\\lingliaobiao.frx"));

		CString Param;
		//Param.Format("preview \"%s\" \"%s\"",ReportPath,excelData);
		Param.Format("excel \"%s\" \"%s\" \"%s\"",ReportPath,excelData,savePath);
		if (HGTools::isFileExists(ExcelExportCmdPath)
			&& HGTools::isFileExists(ReportPath)
			&& HGTools::isFileExists(excelData))
		{
			ShellExecute(NULL,"open",ExcelExportCmdPath,Param,NULL,SW_HIDE);
			MessageBox("保存完成");
		}
		else
		{
			MessageBox("生成工具丢失");
		}
#endif
		HGTools::deleteFile(excelFormat);
		HGTools::deleteFile(excelData);
	}
}

void CCarvingView::SaveExcelFormat(CString savePath, std::vector<std::vector<std::string>> infos)
{
	//////////////////////////////////////////////////////////////////////////
	/* //保存表头schema格式
	<?xml version="1.0" encoding="GB2312" ?>
	<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
		targetNamespace="http://www.techtimesun.com"
		xmlns="http://www.techtimesun.com"
		elementFormDefault="unqualified">
		<xs:element name="data">
			<xs:complexType>
				<xs:sequence>
					<xs:element name="item" maxOccurs="unbounded">
						<xs:complexType>
							<xs:all>
								<xs:element name="order" type="xs:string" minOccurs="0"/>
								<xs:element name="cabinet" type="xs:string" minOccurs="0"/>
								<xs:element name="info" type="xs:string" minOccurs="0"/>
								<xs:element name="PorductName" type="xs:string" minOccurs="0"/>
							</xs:all>
						</xs:complexType>
					</xs:element>
				</xs:sequence>
			</xs:complexType>
		</xs:element>
	</xs:schema>
	*/
	TinyXml::TiXmlDocument docSchema;
	docSchema.Parse(
		"<?xml version=\"1.0\" encoding=\"GB2312\" ?>"
		"<xs:schema xmlns:xs=\"http://www.w3.org\/2001\/XMLSchema\" "
		"targetNamespace=\"http://www.techtimesun.com\" "
		"xmlns=\"\http://www.techtimesun.com\" "
		"elementFormDefault=\"unqualified\">"
		"<xs:element name=\"data\">"
		"<xs:complexType>"
		"<xs:sequence>"
		"</xs:sequence>"
		"</xs:complexType>"
		"</xs:element>"
		"</xs:schema>"
		);
	//TinyXml::XmlHandlePlus handleSchema(&docSchema);
	//handleSchema.changeDirectoryTo("/xs:schema/xs:element/xs:complexType/xs:sequence");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = docSchema.RootElement();
	TiXmlElement* elmTmp = xmlHandler.GetChildElm(elmRoot, "xs:element");
	elmTmp = xmlHandler.GetChildElm(elmTmp, "xs:complexType");
	elmTmp = xmlHandler.GetChildElm(elmTmp, "xs:sequence");
	{
		for (int info_index = 0; info_index < infos.size() ; info_index++)
		{
			std::vector<std::string> info = infos[info_index];
			CString infoName;
			infoName.Format("info_%d",info_index);
			TinyXml::TiXmlElement element("xs:element");
			element.SetAttribute("name",infoName.GetBuffer());
			element.SetAttribute("maxOccurs","unbounded");
			{
				TinyXml::TiXmlElement complexType("xs:complexType");
				{
					TinyXml::TiXmlElement all("xs:all");
					{
						for (int i = 0; i < info.size() ; i++)
						{
							TinyXml::TiXmlElement element("xs:element");
							element.SetAttribute("name",info[i].c_str());
							element.SetAttribute("type","xs:string");
							element.SetAttribute("minOccurs","0");
							all.InsertEndChild(element);
						}
					}
					complexType.InsertEndChild(all);
				}
				element.InsertEndChild(complexType);
			}
			//handleSchema.Node()->InsertEndChild(element);
			elmTmp->InsertEndChild(element);
		}
	}
	docSchema.SaveFile(savePath.GetBuffer());
}

void CCarvingView::SaveExcelData(CString savePath, std::vector<std::vector<std::string>> infos,std::vector<std::vector<std::map<std::string,std::string>>> dataSrcs)
{
	//////////////////////////////////////////////////////////////////////////
	/* //保存数据为xml格式
	<?xml version="1.0" encoding="GB2312" ?>
	<data xmlns="http://www.techtimesun.com"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.techtimesun.com excel.xsd">
		<item>
			<order>1</order>
			<cabinet>11</cabinet>
			<info>111</info>
			<PorductName>1111</PorductName>
		</item>
		<item>
			<order>2</order>
			<cabinet>22</cabinet>
			<info>222</info>
			<PorductName>2222</PorductName>
		</item>
		<item>
			<order>3</order>
			<cabinet>33</cabinet>
			<info>333</info>
			<PorductName>3333</PorductName>
		</item>
	</data>
	*/
	TinyXml::TiXmlDocument docXmlData;
	docXmlData.Parse(
		"<?xml version=\"1.0\" encoding=\"GB2312\" ?>"
		"<data xmlns=\"http://www.techtimesun.com\" "
		"	xmlns:xsi=\"http://www.w3.org\/2001\/XMLSchema-instance\" "
		"	xsi:schemaLocation=\"http://www.techtimesun.com excel.xsd\"> "
		"</data>"
		);
	//TinyXml::XmlHandlePlus handleData(&docXmlData);
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = docXmlData.RootElement();
	//handleData.changeDirectoryTo("/data");
	{
		for (int dataIndex = 0; dataIndex < dataSrcs.size() ; dataIndex++)
		{
			std::vector<std::string> info = infos[dataIndex];
			std::vector<std::map<std::string,std::string>> dataSrc = dataSrcs[dataIndex];

			for (int i = 0; i < dataSrc.size() ; i++)
			{
				//这里开始有多个
				CString infoName;
				infoName.Format("info_%d",dataIndex);
				TinyXml::TiXmlElement item(infoName.GetBuffer());
				{
					for (int col = 0; col < info.size() ; col++)
					{
						TinyXml::TiXmlElement colitem(info[col].c_str());
						{
							TinyXml::TiXmlText text(dataSrc[i][ info[col] ].c_str());
							colitem.InsertEndChild(text);
						}
						item.InsertEndChild(colitem);
					}
				}
				//handleData.Node()->InsertEndChild(item);
				elmRoot->InsertEndChild(item);
			}
		}
	}
	docXmlData.SaveFile(savePath.GetBuffer());
}



/*---------------------------------------*/
//	函数说明：
//		设置小板无纹理
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
void CCarvingView::SetSmallCpnNoTexture(vector<ComponentInputItem>& vComponentInputItem)
{
	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	//float small_len		= _ttof(param_dlg->m_minw);			// 最小大板参数 - 最小长度
	//float small_width	= _ttof(param_dlg->m_minh);			// 最小大板参数 - 最小宽度
	//float small_Area	= _ttof(param_dlg->m_minsize);		// 最小大板参数 - 最小面积



	float small_h_len1		= param_dlg->m_valHMinLen1;				// 最小长度1
	float small_h_width1	= param_dlg->m_valHMinWidth1;			// 最小宽度1
	float small_w_len2		= param_dlg->m_valVMinLen2;				// 最小长度2
	float small_w_width2	= param_dlg->m_valVMinWidth2;			// 最小宽度2

	for(vector<ComponentInputItem>::iterator it = vComponentInputItem.begin(); it != vComponentInputItem.end(); it++)
	{
		ComponentInputItem& cpn = *it;

		float cpn_len		= 	cpn.m_fLength;
		float cpn_width		=	cpn.m_fWidth;
		
		// 横纹小板 设为无纹理
		if (cpn.m_strTexture == "横纹" && cpn_len	<= small_h_len1 && cpn_width <= small_h_width1)
		{
			cpn.m_strTexture = "无纹理";
		}

		// 竖纹小板 设为无纹理
		if (cpn.m_strTexture == "竖纹" && cpn_len	<= small_w_len2 && cpn_width <= small_w_width2)
		{
			cpn.m_strTexture = "无纹理";
		}

	}
}


/*---------------------------------------*/
//	函数说明：
//		检查板件大小是否超出并警告超长板件
//
//
//	参数：
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
//void CCarvingView::CheckAndWarnOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
//{
//	CSingleon* pSingleton = CSingleon::GetSingleton();
//	BaseInfo base_info = pSingleton->m_BaseInfo;
//	vector<ComponentInputItem>::iterator it, it_begin, it_end;
//	CString strMsg;
//
//	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();it++)
//	{
//		ComponentInputItem& pCpn = *it;
//
//		if (pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth || pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth)
//		{
//			if (pCpn.m_strTexture == "无纹理")
//			{
//				// 旋转后，再次判断
//				if (pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth || pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth)
//				{
//					// 报错
//					strMsg += "超出范围板件，板件号：" + pCpn.m_strBarcode + "\n";
//
//				}
//			}
//			else
//			{
//
//				// 报错
//				strMsg += "超出范围板件，板件号：" + pCpn.m_strBarcode + "\n";
//
//
//			}
//		}
//		else if (pCpn.m_strTexture == "竖纹" &&
//			(pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth || pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth))
//		{
//			// 报错
//			strMsg += "超出范围板件，板件号：" + pCpn.m_strBarcode + "\n";
//		}
//
//	}
//
//	// 有信息
//	if (strMsg.IsEmpty() != true)
//	{
//		AfxMessageBox(strMsg);
//	}
//
//}

/*---------------------------------------*/
//	函数说明：
//		检查板件大小是否超出并删除超长板件
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
void CCarvingView::CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;
	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "无纹理")
		{
			if(pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 旋转后，再次判断
				if (pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth 
					|| pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth
					|| pCpn.m_fLength <= 0
					|| pCpn.m_fWidth <= 0)
				{
					// 还是超出，删除
					bOverSize = true;
				}
			}
		}
		else if(pCpn.m_strTexture == "横纹")
		{
			if (pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// 报错
			strMsg += "删除超出范围板件，板件号：" + pCpn.m_strBarcode + "\n";

			// 删除
			it = vComponentInputItem.erase(it);
		}
		else
		{
			it++;
		}
	}

	// 有信息
	if (strMsg.IsEmpty() != true)
	{
		AfxMessageBox(strMsg);
	}


}

bool CCarvingView::CheckDeepHoleSlot(vector<ComponentInputItem>& vComponent)
{
	bool bCancelOptimize = false;

	CParamDlg* param_dlg;
	param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);
	bool bNoDeepHole = true;
	for(int i = 0; i < vComponent.size(); i++)
	{
		if(IsVHoleDeeperThanThickness(vComponent[i]))
		{
			bNoDeepHole = false;
			break;
		}
	}
	bool bNoDeepSlot = true;
	for(int i = 0; i < vComponent.size(); i++)
	{
		if(IsSlotDeeperThanThickness(vComponent[i]))
		{
			bNoDeepSlot = false;
			break;
		}
	}

	if(!bNoDeepHole || !bNoDeepSlot)
	{
		if(MessageBox(_T("存在深于板厚的垂直孔/槽，是否继续优化？"), NULL, MB_YESNO) != IDYES)
			bCancelOptimize = true;
	}

	return !bCancelOptimize;
}

// 获取G代码参数
GcodeParam GetGcodeParam()
{
	

	float _CutSpeech;			//开料速度
	float _ProSpeech;			//下刀速度
	float _EndSpeech;			//收尾速度
	float _OverSpeech;			//空跑速度
	float _distance1;
	float _distance2;
	float _upHeight;
	float _downHeight;
	std::string _strRinget;

	GcodeParam gcodeparam;

	gcodeparam.cutspeech = _CutSpeech;
	gcodeparam.prospeech = _ProSpeech;
	gcodeparam.overspeech = _OverSpeech;
	gcodeparam.distance1 = _distance1;
	gcodeparam.distance2 = _distance2;
	CString s_ender1;
	CString s_header1;

	{
		CIniFile inifile;
		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_name = ExeFile;
		file_name = file_name.Left(file_name.ReverseFind('\\'));
		inifile.SetFilePath(file_name);
		inifile.SetFileName(_T("Knife.ini"));
		inifile.OpenIniFileForRead(); 

		inifile.GetItemString(_T("Machine"),_T("header"),s_header1);
		inifile.GetItemString(_T("Machine"),_T("ender"),s_ender1);
	}

	gcodeparam.ender = s_ender1.GetBuffer();
	gcodeparam.header = s_header1.GetBuffer();

	gcodeparam.upheight = _upHeight;
	gcodeparam.downheight = _downHeight;
	gcodeparam.endspeech = _EndSpeech;
	gcodeparam.strRinget = _strRinget;

	return gcodeparam;
}







LRESULT CCarvingView::OnExportMaterialList(WPARAM wparam, LPARAM lparam)
{
	if(wparam == 0)
	{
		if(lparam == 0)
			OnExportMeterialTable1();
		else if (lparam == 1)
		{
			PanelViewingParam* pParam = m_pDlgResult->GetSelectedItemViewingParam();
			if(pParam && pParam->m_pPanel)
			{
				vector<Panel*> vExportPanel;
				vExportPanel.push_back(pParam->m_pPanel);
				ExportMeterialTable1(vExportPanel);
			}
		}
	}
	else if(wparam == 1)
	{
		if(lparam == 0)
			OnExportMeterialTable2();
		else if (lparam == 1)
		{
			PanelViewingParam* pParam = m_pDlgResult->GetSelectedItemViewingParam();
			if(pParam && pParam->m_pPanel)
			{
				vector<Panel*> vExportPanel;
				vExportPanel.push_back(pParam->m_pPanel);
				ExportMeterialTable2(vExportPanel);
			}
		}
	}

	return 0;
}

void CCarvingView::OnButtonAboutUs()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute( \
		NULL, \
		_T("open"), \
		g_szCompanyWebSide, \
		NULL, \
		NULL, \
		SW_SHOWDEFAULT);
}


void CCarvingView::OnButtonOurCustomers()
{
	// TODO: 在此添加命令处理程序代码
}

void CCarvingView::OnButtonContackWithUsQQ()
{
	// TODO: 在此添加命令处理程序代码
	CString strTmp = _T("tencent://AddContact/?fromId=50&fromSubId=1&subcmd=all&uin=%s");
	CString strCommand;
	strCommand.Format(strTmp, g_szCustomerServiceQQ);
	
	ShellExecute( \
		NULL, \
		_T("open"), \
		strCommand, \
		NULL, \
		NULL, \
		SW_SHOWDEFAULT);
}

void CCarvingView::OnButtonContackWithUsWeiXin()
{
	// TODO: 在此添加命令处理程序代码
	CDlgWeiXin dlg;
	dlg.DoModal();
}

void CCarvingView::OnButtonTutorial()
{
	// TODO: 在此添加命令处理程序代码
	CString sHelp = HGTools::getCurExeContainPath() + "\\Help.chm";
	if (HGTools::isFileExists(sHelp))
	{
		ShellExecute(NULL, "open", sHelp, NULL, NULL, SW_SHOWMAXIMIZED);
	}
}

void CCarvingView::OnButtonVideo()
{
	// TODO: 在此添加命令处理程序代码
	CString strLink = _T("http://www.techtimesun.com/list-166-1.html");
	ShellExecute( \
		NULL, \
		_T("open"), \
		strLink, \
		NULL, \
		NULL, \
		SW_SHOWDEFAULT);
}

void CCarvingView::OnButtonOpenTemplate()
{
	// TODO: 在此添加命令处理程序代码
	CString strTemplate = HGTools::getCurExeContainPath() + "\\开料大师模板.xls";
	if (HGTools::isFileExists(strTemplate))
	{
		ShellExecute(this->m_hWnd,"Open",strTemplate,"","",SW_SHOW);
	}
}

void CCarvingView::ClearAllData()
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();

	if (m_pDlgResult != NULL)
		m_pDlgResult->ResetResultDlg();
}

void CCarvingView::OnMenuSaveAllSettings()
{
	// TODO: 在此添加命令处理程序代码

#ifndef YiWei
	CString backup_path = HGTools::getRelativePath("kdata\\备份配置数据文件.bat");
#else
	CString backup_path = HGTools::getRelativePath("kdata\\YW_备份配置数据文件.bat");
#endif
	

	//AfxMessageBox(backup_path);

	system("\"" + backup_path + "\"");
	
	AfxMessageBox("备份配置文件完成！");

}

void CCarvingView::OnMenuRevertAllSettings()
{
	// TODO: 在此添加命令处理程序代码
#ifndef YiWei
	CString recover_path = HGTools::getRelativePath("kdata\\恢复配置数据文件.bat");
#else
	CString recover_path = HGTools::getRelativePath("kdata\\YW_恢复配置数据文件.bat");
#endif

	system("\"" + recover_path + "\"");
	AfxMessageBox("恢复配置文件完成！");
}

void CCarvingView::RecordMaterial()
{
	vector<CString> vRecordedMaterial;

	CString strTemporaryRecordFile = GetModulePath() + TEMPORARY_RECORD_FILE_DIR;
	TiXmlElement * elmHistoryMaterial = NULL;
	TiXmlDocument * xmlDoc = new TiXmlDocument();
	if(access(strTemporaryRecordFile, 0) != 0)
	{
		TiXmlElement* elmRoot = new TiXmlElement("Root");
		xmlDoc->LinkEndChild(elmRoot);
		elmHistoryMaterial = new TiXmlElement("HistoryMaterial");
		elmRoot->LinkEndChild(elmHistoryMaterial);

	}
	else
	{
		bool bLoadRes = xmlDoc->LoadFile(strTemporaryRecordFile);
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			elmHistoryMaterial = xmlHandler.GetChildElm(xmlDoc->RootElement(), "HistoryMaterial");
			if(!elmHistoryMaterial)
			{
				elmHistoryMaterial = new TiXmlElement("HistoryMaterial");
				 xmlDoc->RootElement()->LinkEndChild(elmHistoryMaterial);
			}
			else
			{
				vector<TiXmlElement*> vPreExistItem = xmlHandler.GetChildElms(elmHistoryMaterial, "Item");
				for(int i = 0; i < vPreExistItem.size(); i++)
				{
					CString strMaterial;
					xmlHandler.GetXmlAttribute(vPreExistItem[i], "Name", strMaterial);
					vRecordedMaterial.push_back(strMaterial);
				}
			}
		}
		else
		{
			delete xmlDoc;
			return;
		}
	}

	for(int i = 0; i < m_vComponentInputItem.size(); i++)
	{
		CString strMaterial = m_vComponentInputItem[i].m_strMaterial;
		bool bNeedToInsert = true;
		for(int j = 0; j < vRecordedMaterial.size(); j++)
		{
			if(strMaterial.Compare(vRecordedMaterial[j]) == 0)
			{
				bNeedToInsert = false;
				break;
			}
		}
		if(bNeedToInsert)
		{
			TiXmlElement* elmItem = new TiXmlElement("Item");
			elmHistoryMaterial->LinkEndChild(elmItem);
			elmItem->SetAttribute("Name", strMaterial.GetBuffer());
			strMaterial.ReleaseBuffer();
			vRecordedMaterial.push_back(strMaterial);
		}
	}

	xmlDoc->SaveFile(strTemporaryRecordFile);
	delete xmlDoc;
}

void ExcelSettingForTheFistTime(CString strFilePath)
{
	bool bImportedExcel = false;

	CString strTemporaryRecordFile = GetModulePath() + TEMPORARY_RECORD_FILE_DIR;
	if(access(strTemporaryRecordFile, 0) == 0)
	{
		TinyXml::TiXmlDocument doc(strTemporaryRecordFile);
		bool bLoadRes = doc.LoadFile();
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			TiXmlElement* elmUsingState = xmlHandler.GetChildElm(doc.RootElement(), "UsingState");
			xmlHandler.GetXmlAttribute(elmUsingState, "ImportedExcel", bImportedExcel);
		}
	}
	if(!bImportedExcel)
	{
		//_T("这是您第一次从Excel表格导入开料数据吧？\n是否先去设置一下Excel表格里面的各项内容与程序所需开料数据之间的对应关系？");
		CString strPrompt = _T("是否需要设置Excel导入格式？\n您还可以通过工具栏“设置”->“模板设置”进行设置。");
		if(AfxMessageBox(strPrompt, MB_YESNO) == IDYES)
		{
			CDlgExcelInputSetting dlg(strFilePath);
			dlg.DoModal();
		}
	}

	if(access(strTemporaryRecordFile, 0) != 0)
	{
		TiXmlDocument * xmlDoc = new TiXmlDocument();

		TiXmlElement * elmRoot = new TiXmlElement("Root");
		xmlDoc->LinkEndChild(elmRoot);
		TiXmlElement * elmUsingState = new TiXmlElement("UsingState");
		elmRoot->LinkEndChild(elmUsingState);
		elmUsingState->SetAttribute("ImportedExcel", "1");

		xmlDoc->SaveFile(strTemporaryRecordFile);
		delete xmlDoc;
	}
	else
	{
		TinyXml::TiXmlDocument doc(strTemporaryRecordFile);
		bool bLoadRes = doc.LoadFile();
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			xmlHandler.SetElmAttr(doc.RootElement(), "UsingState", "ImportedExcel", "1");
		}
	}
}
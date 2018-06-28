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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Carving.h"

#include "MainFrm.h"

#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/BaseInfo/BaseInfo.h"
#include "UI/ParamDlg.h"
#include "UI/UICommon/RibbonButtonSelf.h"
#include "UI/UICommon/RibbonCategorySelf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	//ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
	: m_pDlgBaseParam(NULL)
	, m_pBtnAboutUs(NULL)
	, m_pBtnOurCustomers(NULL)
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
	if(m_pDlgBaseParam)
	{
		if (m_pDlgBaseParam->GetSafeHwnd())
		{
			m_pDlgBaseParam->DestroyWindow();
		}
		delete m_pDlgBaseParam;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	//m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	CString strTmp;
	strTmp.Format(_T("%s 荣誉出品 %s"), g_szCompanyFullName, g_szVersion);
	strTitlePane2 = strTmp;
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, /*strTitlePane1*/_T(""), TRUE), /*strTitlePane1*/_T(""));
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 导航窗格将创建在左侧，因此将暂时禁用左侧的停靠:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	//// 创建并设置“Outlook”导航栏:
	//if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	//{
	//	TRACE0("未能创建导航窗格\n");
	//	return -1;      // 未能创建
	//}

	// 创建标题栏:
	//if (!CreateCaptionBar())
	//{
	//	TRACE0("未能创建标题栏\n");
	//	return -1;      // 未能创建
	//}

	// 已创建 Outlook 栏，应允许在左侧停靠。
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	m_pDlgBaseParam = new CParamDlg;
	m_pDlgBaseParam->loadSettings();



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

//BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
//{
//	bar.SetMode2003();
//
//	BOOL bNameValid;
//	CString strTemp;
//	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
//	ASSERT(bNameValid);
//	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
//	{
//		return FALSE; // 未能创建
//	}
//
//	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();
//
//	if (pOutlookBar == NULL)
//	{
//		ASSERT(FALSE);
//		return FALSE;
//	}
//
//	pOutlookBar->EnableInPlaceEdit(TRUE);
//
//	static UINT uiPageID = 1;
//
//	// 可浮动，可自动隐藏，可调整大小，但不能关闭
//	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;
//
//	CRect rectDummy(0, 0, 0, 0);
//	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
//
//	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
//	bNameValid = strTemp.LoadString(IDS_FOLDERS);
//	ASSERT(bNameValid);
//	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);
//
//	calendar.Create(rectDummy, &bar, 1201);
//	bNameValid = strTemp.LoadString(IDS_CALENDAR);
//	ASSERT(bNameValid);
//	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);
//
//	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
//
//	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
//	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
//	pOutlookBar->RecalcLayout();
//
//	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
//	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);
//
//	bar.SetButtonsFont(&afxGlobalData.fontBold);
//
//	return TRUE;
//}

//BOOL CMainFrame::CreateCaptionBar()
//{
//	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
//	{
//		TRACE0("未能创建标题栏\n");
//		return FALSE;
//	}
//
//	BOOL bNameValid;
//
//	CString strTemp, strTemp2;
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButtonToolTip(strTemp);
//
//	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);
//
//	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
//	ASSERT(bNameValid);
//	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);
//
//	return TRUE;
//}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//void CMainFrame::OnViewCaptionBar()
//{
//	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
//	RecalcLayout(FALSE);
//}

//void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
//{
//	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
//}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

void CMainFrame::InitializeRibbon()//初始化Ribbon工具栏
{
	CString strTemp;
	//左上角的菜单按钮初始化
	strTemp = L"文件";
	m_PanelImages.SetImageSize(CSize(16, 16));//设置左上角菜单按钮图标大小
	m_PanelImages.Load(IDB_BUTTONS);// 加载菜单图像
	//初始化主菜单按钮
#ifndef YiWei
	m_MainButton.SetImage(IDB_MAIN);
#else
	m_MainButton.SetImage(IDB_MAIN_YW);
#endif
	
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));

	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	//strTemp = L"new";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
	//strTemp = L"open";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
	//strTemp = L"save";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
	//strTemp = L"save as";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

	//strTemp = L"print";
	//CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, strTemp, 6, 6);
	//pBtnPrint->SetKeys(_T("p"), _T("w"));
	//strTemp = L"print label";
	//pBtnPrint->AddSubItem(new CMFCRibbonLabel(strTemp));
	//strTemp = L"print quick";
	//pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, strTemp, 7, 7, TRUE));
	//strTemp = L"print preview";
	//pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, strTemp, 8, 8, TRUE));
	//strTemp = L"print setup";
	//pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, strTemp, 11, 11, TRUE));
	//pMainPanel->Add(pBtnPrint);

	//strTemp = L"close";
	//pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	//strTemp = L"recent doc";
	//pMainPanel->AddRecentFilesList(strTemp);

	//strTemp = L"exit";
	//pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));


#pragma region CreateRibbonBar
	strTemp = /*L"导入"*/_T("开始");
#ifndef YiWei
	//CMFCRibbonCategory* pCategoryImport = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL/*IDB_PNG1*/, IDB_WRITELARGE1/*IDB_PNG2*/);
	//CRibbonCategorySelf* pCategoryImport = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1, RUNTIME_CLASS(CRibbonCategorySelf));
	CRibbonCategorySelf* pCategoryStart = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1);
	//AddCategory(LPCTSTR lpszName, UINT uiSmallImagesResID, UINT uiLargeImagesResID,
	//	CSize sizeSmallImage = CSize(16, 16), CSize sizeLargeImage = CSize(32, 32), int nInsertAt = -1, CRuntimeClass* pRTI = NULL)
#else
	CRibbonCategorySelf* pCategoryStart = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1_YW, CSize(16, 16), CSize(32, 32), -1);
#endif


	//strTemp = L"优化";
	//CMFCRibbonCategory* pCategoryOptimize = m_wndRibbonBar.AddCategory(strTemp,IDB_WRITESMALL,IDB_WRITELARGE1);

	//strTemp = L"导出";
	//CMFCRibbonCategory* pCategoryExport = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1);

	strTemp = _T("设置");
#ifndef YiWei
	//CMFCRibbonCategory* pCategorySetting = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1);
	CRibbonCategorySelf* pCategorySetting = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1);
#else
	CRibbonCategorySelf* pCategorySetting = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1_YW, CSize(16, 16), CSize(32, 32), -1);
#endif


	strTemp = L"帮助";
#ifndef YiWei
	//CMFCRibbonCategory* pCategoryHelp = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1);
	CRibbonCategorySelf* pCategoryHelp = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1, CSize(16, 16), CSize(32, 32), -1);
#else
	CRibbonCategorySelf* pCategoryHelp = (CRibbonCategorySelf*)m_wndRibbonBar.AddCategorySelf(strTemp, IDB_WRITESMALL, IDB_WRITELARGE1_YW, CSize(16, 16), CSize(32, 32), -1);
#endif

	strTemp = L"导入";
	CMFCRibbonPanel* pPanelImportMethod = pCategoryStart->AddPanel(strTemp/*, m_PanelImages.ExtractIcon (1)*/);

#ifndef YiWei
	strTemp = L"HG文件";
	CRibbonButtonSelf* pBtnImportHG = new CRibbonButtonSelf(ID_OPEN_SOURCE_HG,strTemp,0,0, IDB_PNG_HG_FILE, FALSE, true, true);
#else
	strTemp = L"YW文件";
	CRibbonButtonSelf* pBtnImportHG = new CRibbonButtonSelf(ID_OPEN_SOURCE_HG,strTemp,0,0, IDB_PNG_HG_FILE_YW, FALSE, true, true);
#endif
	
	
	pBtnImportHG->SetMenu(IDR_MENU_OPEN_HGE, TRUE);
	pPanelImportMethod->Add(pBtnImportHG);

// 	strTemp = L"HG文件";
// 	//pPanelImportMethod->Add(new CMFCRibbonButton(ID_OPEN_SOURCE_HG,strTemp,0,0));
// 	pPanelImportMethod->Add(new CRibbonButtonSelf(ID_OPEN_SOURCE_HG,strTemp,0,0, IDB_PNG_HG_FILE, FALSE, true, true));

	strTemp = L"Excel";
	CRibbonButtonSelf* pBtnImportExcel = new CRibbonButtonSelf(ID_OPEN_SOURCE_EXCEL,strTemp,0,0, IDB_PNG_EXCEL, FALSE, true, true);
	pBtnImportExcel->SetMenu(IDR_MENU_OPEN_EXCEL, TRUE);
	pPanelImportMethod->Add(pBtnImportExcel);





	strTemp = L"优化方案";
	//pPanelImportMethod->Add(new CMFCRibbonButton(ID_OPEN_SOLUTION,strTemp,2,2));
	pPanelImportMethod->Add(new CRibbonButtonSelf(ID_OPEN_SOLUTION,strTemp,0,0, IDB_PNG_TIME_FOLDER, FALSE, true, true));





	strTemp = L"优化";
	CMFCRibbonPanel* pPanelOptimize = /*pCategoryOptimize*/pCategoryStart->AddPanel(strTemp);

	strTemp = L"优化";
	//pPanelOptimize->Add(new CMFCRibbonButton(ID_OPTIMIZE,strTemp,3,3));
	pPanelOptimize->Add(new CRibbonButtonSelf(ID_OPTIMIZE,strTemp,0,0, IDB_PNG_TIME, FALSE, true, true));

	strTemp = L"保存";
	CMFCRibbonPanel* pPanelSave = pCategoryStart->AddPanel(strTemp);

	strTemp = L"保存";
	pPanelSave->Add(new CRibbonButtonSelf(ID_SAVE,strTemp,0,0, IDB_PNG_SAVE, FALSE, true, true));

	strTemp = L"导出";
	CMFCRibbonPanel* pPanelExport = /*pCategoryExport*/pCategoryStart->AddPanel(strTemp);

	strTemp = L"打印";
	//pPanelExport->Add(new CMFCRibbonButton(ID_BUTTON_PRINT_LABEL,strTemp,5,5));
	pPanelExport->Add(new CRibbonButtonSelf(ID_BUTTON_PRINT_LABEL,strTemp,0,0, IDB_PNG_PRINT, FALSE, true, true));

	strTemp = L"NC码";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_GCODE,strTemp,6,6));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_GCODE,strTemp,0,0, IDB_PNG_NC_CODE, FALSE, true, true));

	strTemp = L"料单1";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE1,strTemp,19,19));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_METERIAL_TABLE1,strTemp,19,19, IDB_PNG_EXCEL16, FALSE, true, true));

	strTemp = L"料单2";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE2,strTemp,19,19));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_METERIAL_TABLE2,strTemp,19,19, IDB_PNG_EXCEL16, FALSE, true, true));

	strTemp = L"打印领料单";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_PANEL_METERIAL_TABLE,strTemp,19,19));
	pPanelExport->Add(new CRibbonButtonSelf(ID_EXPORT_PANEL_METERIAL_TABLE,strTemp,19,19, IDB_PNG_EXCEL16, FALSE, true, true));


	strTemp = L"联系我们";
	CMFCRibbonPanel* pPanelContactWithUs = pCategoryStart->AddPanel(strTemp);

	strTemp = L"QQ";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE1,strTemp,19,19));
	pPanelContactWithUs->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_QQ,strTemp,0,0, IDB_PNG_LOGO_QQ, FALSE, true, true));

	strTemp = L"微信";
	//pPanelExport->Add(new CMFCRibbonButton(ID_EXPORT_METERIAL_TABLE2,strTemp,19,19));
	pPanelContactWithUs->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_WEIXIN,strTemp,0,0, IDB_PNG_LOGO_WEIXIN, FALSE, true, true));

	//strTemp = L"参数";
	//CMFCRibbonPanel* pPanelPragma = /*pCategorySetting*/pCategoryImport->AddPanel(strTemp);

	//m_pEditKerf = new CMFCRibbonEdit(ID_EDIT_PRAGMA_SAW_KERF, 50, _T("锯缝宽度:"));
	//pPanelPragma->Add(m_pEditKerf);
	//m_pEditKerf->SetEditText(_T("6"));
	//m_pEditPanelLength = new CMFCRibbonEdit(ID_EDIT_PANEL_LENGTH, 50, _T("大板长度:"));
	//pPanelPragma->Add(m_pEditPanelLength);
	//m_pEditPanelLength->SetEditText(_T("2432"));
	//m_pEditPanelWidth = new CMFCRibbonEdit(ID_EDIT_PANEL_WIDTH, 50, _T("大板宽度:"));
	//pPanelPragma->Add(m_pEditPanelWidth);
	//m_pEditPanelWidth->SetEditText(_T("1212"));

	//m_pEditStep1Count = new CMFCRibbonEdit(ID_EDIT_STEP1_COUNT, 50, _T("第一阶段优化次数:"));
	//pPanelPragma->Add(m_pEditStep1Count);
	//m_pEditStep1Count->SetEditText(_T("200"));

	//m_pComboStep1Alg = new CMFCRibbonComboBox(ID_COMBO_STEP1_ALG, 0, 100, _T("第一阶段优化算法:"));
	//pPanelPragma->Add(m_pComboStep1Alg);
	//m_pComboStep1Alg->AddItem(_T("改进最低轮廓线算法"));
	//m_pComboStep1Alg->AddItem(_T("改进贪心算法"));
	//m_pComboStep1Alg->AddItem(_T("组合算法"));
	//m_pComboStep1Alg->SelectItem(1);

	//m_pEditStep2Count = new CMFCRibbonEdit(ID_EDIT_STEP2_COUNT, 50, _T("第二阶段优化次数:"));
	//pPanelPragma->Add(m_pEditStep2Count);
	//m_pEditStep2Count->SetEditText(_T("0"));

	//m_pEditStep3Count = new CMFCRibbonEdit(ID_EDIT_STEP3_COUNT, 50, _T("第三阶段优化次数:"));
	//pPanelPragma->Add(m_pEditStep3Count);
	//m_pEditStep3Count->SetEditText(_T("0"));
	//m_pEditStep3AcceptableUti = new CMFCRibbonEdit(ID_EDIT_STEP3_ACCEPTABLE_UTI, 50, _T("第三阶段可接受利用率:"));
	//pPanelPragma->Add(m_pEditStep3AcceptableUti);
	//m_pEditStep3AcceptableUti->SetEditText(_T("0.9"));


	//m_pComboLayoutOrg = new CMFCRibbonComboBox(ID_COMBO_LAYOUT_ORIGIN, 0, 100, _T("排样原点:"));
	//pPanelPragma->Add(m_pComboLayoutOrg);
	//m_pComboLayoutOrg->AddItem(_T("左下角"));
	//m_pComboLayoutOrg->AddItem(_T("左上角"));
	//m_pComboLayoutOrg->AddItem(_T("右下角"));
	//m_pComboLayoutOrg->AddItem(_T("右上角"));
	//m_pComboLayoutOrg->SelectItem(1);

#ifndef YiWei
	strTemp = L"a                                                                                              a";
	m_pBtnAboutUs = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE, false, false);
	m_pBtnAboutUs->SetToolTipText(_T("打开官网"));

	strTemp = L"                                                             ";
	CMFCRibbonPanel* pPanelAboutUs = pCategoryStart->AddPanel(strTemp);
	pPanelAboutUs->Add(m_pBtnAboutUs);
	//strTemp = L"                             成功用户                                      ";
	//CMFCRibbonPanel* pPanelOurCustomers = pCategoryStart->AddPanel(strTemp);
	//pPanelOurCustomers->Add(m_pBtnOurCustomers);
#else
	strTemp = L"a                                                                                                                                                   a";
	m_pBtnAboutUs = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US_YW, FALSE, false, false);
	m_pBtnAboutUs->SetToolTipText(_T("打开官网"));

	strTemp = L"                                                                       ";
	CMFCRibbonPanel* pPanelAboutUs = pCategoryStart->AddPanel(strTemp);
	pPanelAboutUs->Add(m_pBtnAboutUs);
#endif

	CMFCRibbonLabel* pLabel = new CMFCRibbonLabel(_T("hello"));

	POINT ptIconCenter;
	ptIconCenter.x = 70;
	ptIconCenter.y = 45;

#ifndef YiWei
	pCategoryStart->AddIcon(ptIconCenter, IDB_PNG_QRCODE);
	pCategorySetting->AddIcon(ptIconCenter, IDB_PNG_QRCODE);
	pCategoryHelp->AddIcon(ptIconCenter, IDB_PNG_QRCODE);
#else
	pCategoryStart->AddIcon(ptIconCenter, IDB_PNG_QRCODE_YW);
	pCategorySetting->AddIcon(ptIconCenter, IDB_PNG_QRCODE_YW);
	pCategoryHelp->AddIcon(ptIconCenter, IDB_PNG_QRCODE_YW);
#endif

	//strTemp = L"a                                                                                              a";
	///*CRibbonButtonSelf* */m_pBtnAboutUs = new CRibbonButtonSelf/*CMFCRibbonButton*/(ID_BUTTON_PRINT_LABEL,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE);
	
	strTemp = L"设置";
	CMFCRibbonPanel* pPanelSetting = /*pCategorySetting*/pCategorySetting->AddPanel(strTemp);

	strTemp = L"基础设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_BASE_SETTING,strTemp,8,8));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_BASE_SETTING,strTemp,0,0, IDB_PNG_OPERATION, FALSE, true, true));

	strTemp = L"刀库";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_MACHINE_PARAM_SETTING,strTemp,7,7));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_MACHINE_PARAM_SETTING,strTemp,0,0, IDB_PNG_MACHINE, FALSE, true, true));

	strTemp = L"模板设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_EXCEL_TEMPLATE_SETTING,strTemp,15,15));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_EXCEL_TEMPLATE_SETTING,strTemp,0,0, IDB_PNG_EXCEL_FILE, FALSE, true, true));

	strTemp = L"标签设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_LABEL_SETTING,strTemp,14,14));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_LABEL_SETTING,strTemp,0,0, IDB_PNG_LABEL_SETTING, FALSE, true, true));

	strTemp = L"优化设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_OPTIMIZE_SETTING,strTemp,9,9));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_OPTIMIZE_SETTING,strTemp,19,19, IDB_PNG_TIME, FALSE, true, true));

	strTemp = L"余料设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_REMAINDER_SETTING,strTemp,10,10));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_REMAINDER_SETTING,strTemp,19,19, IDB_PNG_REMAINDER, FALSE, true, true));

	strTemp = L"领料单设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_LINGLIAO_TABLE_SETTING,strTemp,16,16));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_LINGLIAO_TABLE_SETTING,strTemp,19,19, IDB_PNG_FILE, FALSE, true, true));

	strTemp = L"默认加载设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_DEFAULT_LOADING_DIR_SETTING,strTemp,11,11));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_DEFAULT_LOADING_DIR_SETTING,strTemp,19,19, IDB_PNG_LOAD_SETTING, FALSE, true, true));

	strTemp = L"默认保存设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_DEFAULT_SAVING_DIR_SETTING,strTemp,12,12));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_DEFAULT_SAVING_DIR_SETTING,strTemp,19,19, IDB_PNG_SAVE_SETTING, FALSE, true, true));

	strTemp = L"其他设置";
	//pPanelSetting->Add(new CMFCRibbonButton(ID_MENU_OTHER_SETTING,strTemp,13,13));
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_OTHER_SETTING,strTemp,19,19, IDB_PNG_SETTINGS, FALSE, true, true));

	strTemp = L"打开模板";
	pPanelSetting->Add(new CRibbonButtonSelf(ID_MENU_OPEN_TEMPLATE,strTemp,19,19, IDB_PNG_EXCEL_FILE, FALSE, true, true));

	strTemp = L"备份所有设置";
	CMFCRibbonPanel* pPanelSettingSave = pCategorySetting->AddPanel(strTemp);

	strTemp = L"备份";
	pPanelSettingSave->Add(new CRibbonButtonSelf(ID_MENU_SETTING_SAVE,strTemp,0,0, IDB_PNG_SETTINGS_SAVE, FALSE, true, true));

	strTemp = L"还原";
	pPanelSettingSave->Add(new CRibbonButtonSelf(ID_MENU_SETTING_REVERT,strTemp,0,0, IDB_PNG_SETTINGS_REVERT, FALSE, true, true));

	strTemp = L"联系我们";
	CMFCRibbonPanel* pPanelContactWithUs_InSetting = pCategorySetting->AddPanel(strTemp);
	strTemp = L"QQ";
	pPanelContactWithUs_InSetting->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_QQ,strTemp,0,0, IDB_PNG_LOGO_QQ, FALSE, true, true));
	strTemp = L"微信";
	pPanelContactWithUs_InSetting->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_WEIXIN,strTemp,0,0, IDB_PNG_LOGO_WEIXIN, FALSE, true, true));

#ifndef YiWei
	strTemp = L"a                                                                                              a";
	CRibbonButtonSelf* pBtnAboutUs_InSetting = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE, false, false);
	pBtnAboutUs_InSetting->SetToolTipText(_T("打开官网"));

	strTemp = L"                                                             ";
	CMFCRibbonPanel* pPanelAboutUs_InSetting = pCategorySetting->AddPanel(strTemp);
	pPanelAboutUs_InSetting->Add(pBtnAboutUs_InSetting);
#else
	strTemp = L"a                                                                                                                                                   a";
	CRibbonButtonSelf* pBtnAboutUs_InSetting = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US_YW, FALSE, false, false);
	pBtnAboutUs_InSetting->SetToolTipText(_T("打开官网"));

	strTemp = L"                                                                       ";
	CMFCRibbonPanel* pPanelAboutUs_InSetting = pCategorySetting->AddPanel(strTemp);
	pPanelAboutUs_InSetting->Add(pBtnAboutUs_InSetting);
#endif


	strTemp = L"帮助";
	CMFCRibbonPanel* pPanelHelp = pCategoryHelp->AddPanel(strTemp);

	strTemp = L"教程";
	pPanelHelp->Add(new CRibbonButtonSelf(ID_MENU_HELP_TUTORIAL,strTemp,0,0, IDB_PNG_TUTORIAL, FALSE, true, true));

#ifndef YiWei
	strTemp = L"教学视频";
	pPanelHelp->Add(new CRibbonButtonSelf(ID_MENU_VIDEO,strTemp,0,0, IDB_PNG_VIDEO, FALSE, true, true));
#endif

	strTemp = L"联系我们";
	CMFCRibbonPanel* pPanelContactWithUs_InHelp = pCategoryHelp->AddPanel(strTemp);
	strTemp = L"QQ";
	pPanelContactWithUs_InHelp->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_QQ,strTemp,0,0, IDB_PNG_LOGO_QQ, FALSE, true, true));
	strTemp = L"微信";
	pPanelContactWithUs_InHelp->Add(new CRibbonButtonSelf(ID_MENU_CONTACK_WITH_US_WEIXIN,strTemp,0,0, IDB_PNG_LOGO_WEIXIN, FALSE, true, true));

#ifndef YiWei
	strTemp = L"a                                                                                              a";
	CRibbonButtonSelf* pBtnAboutUs_InHelp = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US, FALSE, false, false);
	pBtnAboutUs_InHelp->SetToolTipText(_T("打开官网"));

	strTemp = L"                                                             ";
	CMFCRibbonPanel* pPanelAboutUs_InHelp = pCategoryHelp->AddPanel(strTemp);
	pPanelAboutUs_InHelp->Add(pBtnAboutUs_InHelp);
#else
	strTemp = L"a                                                                                                                                                   a";
	CRibbonButtonSelf* pBtnAboutUs_InHelp = new CRibbonButtonSelf(ID_BUTTON_ABOUT_US,strTemp,13,4, IDB_PNG_ABOUT_US_YW, FALSE, false, false);
	pBtnAboutUs_InHelp->SetToolTipText(_T("打开官网"));

	strTemp = L"                                                                       ";
	CMFCRibbonPanel* pPanelAboutUs_InHelp = pCategoryHelp->AddPanel(strTemp);
	pPanelAboutUs_InHelp->Add(pBtnAboutUs_InHelp);
#endif

#pragma endregion


	//// 添加快速访问工具栏命令:
	//CList<UINT, UINT> lstQATCmds;

	//lstQATCmds.AddTail(ID_FILE_NEW);
	//lstQATCmds.AddTail(ID_FILE_OPEN);
	//lstQATCmds.AddTail(ID_FILE_SAVE);
	//lstQATCmds.AddTail(ID_EDIT_UNDO);

	//m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds,true);
	CMFCRibbonQuickAccessToolBarDefaultState* qaToolBarState = new CMFCRibbonQuickAccessToolBarDefaultState();
	qaToolBarState->AddCommand(ID_FILE_NEW, true);
	qaToolBarState->AddCommand(ID_FILE_OPEN, true);
	qaToolBarState->AddCommand(ID_FILE_SAVE, true);
	m_wndRibbonBar.SetQuickAccessDefaultState(*qaToolBarState);


	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

//void CMainFrame::GetAlgBaseInfo(BaseInfo& info)
//{
//	CString strTmp;
//
//	// 取消使用，改用基础设置中的数据
//	// 长、宽、锯缝 
//// 	strTmp = m_pEditKerf->GetEditText();
//// 	info.m_SawKerfWidth = atof(strTmp);
//// 	strTmp = m_pEditPanelLength->GetEditText();
//// 	info.m_PanelLength = atof(strTmp);
//// 	strTmp = m_pEditPanelWidth->GetEditText();
//// 	info.m_PanelWidth = atof(strTmp);
//
//	// 第一阶段参数
//	strTmp = m_pEditStep1Count->GetEditText();
//	info.m_FirstSectionOPTimes = atoi(strTmp);
//	info.m_FirstSectionOPMethod = m_pComboStep1Alg->GetCurSel();
//
//	// 第二阶段参数
//	strTmp = m_pEditStep2Count->GetEditText();
//	info.m_SecondSectionOPTimes = atoi(strTmp);
//
//	// 第三阶段参数
//	strTmp = m_pEditStep3Count->GetEditText();
//	info.m_ThirdSectionOPTimes = atoi(strTmp);
//	strTmp = m_pEditStep3AcceptableUti->GetEditText();
//	info.m_ThirdSectionOAccptableUtilization = atof(strTmp);
//
//	//// 排样原点
//	//info.m_LayoutOrg = m_pComboLayoutOrg->GetCurSel();
//
//}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pBtnAboutUs)
	{
		//
		//CRect rc(0, 0, 100, 30);
		//m_pBtnSelf->SetRect(rc);
	}
}

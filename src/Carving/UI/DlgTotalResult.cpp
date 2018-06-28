// DlgTotalResult.cpp : 实现文件
//

#include "stdafx.h"
#include "../Carving.h"
#include "DlgTotalResult.h"
#include "afxdialogex.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"
#include <algorithm>

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"

// CDlgTotalResult 对话框

IMPLEMENT_DYNAMIC(CDlgTotalResult, CDialogChildBase)

CDlgTotalResult::CDlgTotalResult(CWnd* pParent /*=NULL*/)
	//: CDialog(CDlgTotalResult::IDD, pParent)
	: CDialogChildBase(CDlgTotalResult::IDD, pParent)
{

}

CDlgTotalResult::~CDlgTotalResult()
{
}

void CDlgTotalResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOTAL_SOLUTION_PANEL, m_lcTotalSolutionPanel);
	DDX_Control(pDX, IDC_LIST_MATERIAL, m_lcTotalMaterial);
}


BEGIN_MESSAGE_MAP(CDlgTotalResult, CDialogChildBase)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOTAL_SOLUTION_PANEL, &CDlgTotalResult::OnLvnItemchangedListTotalSolutionPanel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TOTAL_SOLUTION_PANEL, &CDlgTotalResult::OnRclickListTotalSolutionPanel)


	ON_COMMAND(ID_MENU_DELETE_PANEL, &CDlgTotalResult::OnIdDeletePanel)
	ON_COMMAND(ID_MENU_ADD_PANEL, &CDlgTotalResult::OnIdAddPanel)

END_MESSAGE_MAP()


// CDlgTotalResult 消息处理程序


BOOL CDlgTotalResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_lcTotalSolutionPanel.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lcTotalSolutionPanel.InsertColumn(0, _T("图号"), 0, 35);
	m_lcTotalSolutionPanel.InsertColumn(1, _T("原片尺寸"), 0, 165);
	m_lcTotalSolutionPanel.InsertColumn(2, _T("%"), 0, 55);
	m_lcTotalSolutionPanel.InsertColumn(3, _T("用量"), 0, 46);
	m_lcTotalSolutionPanel.InsertColumn(4, _T("板数"), 0, 124);


	HWND hwnd = ::GetDlgItem(m_lcTotalSolutionPanel.m_hWnd, 0); 
	if(hwnd) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcPanelList.SubclassWindow(hwnd); 
			} 
		} 
		else
		{
			m_hcPanelList.SubclassWindow(hwnd);
		}
	}

	LONG dwStyle = GetWindowLong(m_lcTotalSolutionPanel.m_hWnd, GWL_STYLE);
	SetWindowLong(m_lcTotalSolutionPanel.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_lcTotalSolutionPanel.SetExtendedStyle((m_lcTotalSolutionPanel.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	m_lcTotalSolutionPanel.DeleteAllItems();


	m_lcTotalMaterial.InsertColumn(0, _T("用料材质"), 0, 135);
	m_lcTotalMaterial.InsertColumn(1, _T("厚度(mm)"), 0, 70);
	m_lcTotalMaterial.InsertColumn(2, _T("数量"), 0, 70);
	m_lcTotalMaterial.InsertColumn(3, _T("面积(平方米)"), 0, 90);
	m_lcTotalMaterial.InsertColumn(4, _T("利用率"), 0, 60);


	HWND hwnd1 = ::GetDlgItem(m_lcTotalMaterial.m_hWnd, 0); 
	if(hwnd1) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd1); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcTotalMaterial.SubclassWindow(hwnd1); 
			} 
		} 
		else
		{
			m_hcTotalMaterial.SubclassWindow(hwnd1);
		}
	}

	dwStyle = GetWindowLong(m_lcTotalMaterial.m_hWnd, GWL_STYLE);
	SetWindowLong(m_lcTotalMaterial.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_lcTotalMaterial.SetExtendedStyle((m_lcTotalMaterial.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	m_lcTotalMaterial.DeleteAllItems();


	//int nItem = 0;

	//for(int j = 0; j < 10; j++)
	//{
	//	m_lcTotalSolutionPanel.InsertItem(nItem, "暖白");			
	//	m_lcTotalSolutionPanel.SetItemText(nItem, 1, GetFloatString(18, 0));	
	//	m_lcTotalSolutionPanel.SetItemText(nItem, 2, "500*100");	
	//	nItem++;
	//}


	//m_lcTotalSolutionPanel.EnsureVisible(0, FALSE);
	//m_lcTotalSolutionPanel.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTotalResult::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_lcTotalSolutionPanel.GetSafeHwnd())
	{
		CRect rcListCtrl(10, 0, cx, cy - 200);
		m_lcTotalSolutionPanel.MoveWindow(rcListCtrl);

		rcListCtrl.SetRect(10, cy - 200 + 10, cx, cy);
		m_lcTotalMaterial.MoveWindow(rcListCtrl);

		//Invalidate();
		////InvalidateRect(GetPanelViewRect());
	}
}

void CDlgTotalResult::OnLvnItemchangedListTotalSolutionPanel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMLV->uNewState & LVIS_SELECTED)
	{
		PanelViewingParam* pParam = (PanelViewingParam*)m_lcTotalSolutionPanel.GetItemData(pNMLV->iItem);
		::PostMessage(GetParent()->GetSafeHwnd(), WM_REFRESH_PANEL_VIEW, (WPARAM)pParam/*0*/, 0);
	}
	else
	{
		int nItem = m_lcTotalSolutionPanel.GetNextItem(-1,LVIS_SELECTED);

		if(nItem == -1)
		{
			::PostMessage(GetParent()->GetSafeHwnd(), WM_REFRESH_PANEL_VIEW, 0, 0);
		}

	}
	m_lcTotalSolutionPanel.Invalidate();
	//m_lcPanelFace.GetFirstSelectedItemPosition()
	*pResult = 0;
}



/*---------------------------------------*/
//	函数说明：
//			弹出右键菜单
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
void CDlgTotalResult::OnRclickListTotalSolutionPanel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 判断是否选中
	PanelViewingParam* pParam = GetSelectedItemViewingParam();
	if(pParam == NULL)
		return;


	CMenu menu, *pPopup;  
	menu.LoadMenu(IDR_MENU_TOTAL_RESULT);  
	pPopup = menu.GetSubMenu(0);  
	CPoint myPoint;  
	//ClientToScreen(&myPoint);  
	GetCursorPos(&myPoint); 
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this); 

	//m_ptRClicked = myPoint;
	//ScreenToClient(&m_ptRClicked);

}

/*---------------------------------------*/
//	函数说明：
//		删除板件处理函数
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
void  CDlgTotalResult::OnIdDeletePanel()
{
	// 判断是否选中
	PanelViewingParam* pParam = GetSelectedItemViewingParam();
	if(pParam == NULL)
		return;

	CSolution* pCurSln = pParam->m_pSolution;
	Panel* pCurPanel = pParam->m_pPanel;

	// 寻找大板，并删除当前选中板件
	vector<Panel*>::iterator itr_cur_panel = std::find(pCurSln->m_PanelList.begin(), pCurSln->m_PanelList.end(), pCurPanel);

	// 找到
	if (itr_cur_panel != pCurSln->m_PanelList.end())
	{
		delete pCurPanel;
		pCurSln->m_PanelList.erase(itr_cur_panel);

		int nPanelCount = pCurSln->GetPanelNum();

		// 没有板件了 删除解决方案
		if (nPanelCount == 0 )
		{
			CSingleon* pSingleton = CSingleon::GetSingleton();
			vector<CSolution*>& SlnList =  pSingleton->m_BackupSolutionList;

			vector<CSolution*>::iterator itr_cur_sln = std::find(SlnList.begin(), SlnList.end(), pCurSln);
			if (itr_cur_sln != SlnList.end())
			{
				delete pCurSln;
				SlnList.erase(itr_cur_sln);
			}
		}

		// 更新显示
		RefreshOptimizeResult();	
	}

}

/*---------------------------------------*/
//	函数说明：
//
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
void CDlgTotalResult::OnIdAddPanel()
{
	// 判断是否选中
	PanelViewingParam* pParam = GetSelectedItemViewingParam();
	if(pParam == NULL)
		return;

	CSolution* pCurSln = pParam->m_pSolution;
	Panel* pCurPanel = pParam->m_pPanel;
	
	// 复制一份大板信息
	Panel* pNewPanel = new Panel(*pCurPanel);

	// 插入大板列表中
	pCurSln->m_PanelList.push_back(pNewPanel);


	// 更新显示
	RefreshOptimizeResult();

}



void CDlgTotalResult::RefreshOptimizeResult()
{
	RefreshTotalSolutionPanel();
	if(m_lcTotalSolutionPanel.GetItemCount() > 0)
	{
		m_lcTotalSolutionPanel.EnsureVisible(0, FALSE);
		m_lcTotalSolutionPanel.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CDlgTotalResult::RefreshTotalSolutionPanel()
{
	m_lcTotalSolutionPanel.DeleteAllItems();

	int nItem = 0;

#if 0
	for(int i = 0; i < CSingleon::GetSingleton()->m_CurrentSolutionList.size(); i++)
	{
		CSolution* pSolution = CSingleon::GetSingleton()->m_CurrentSolutionList[i];
		for(int j = 0; j < CSingleon::GetSingleton()->m_CurrentSolutionList[i]->m_PanelList.size(); j++)
		{
			Panel& thePanel = *(CSingleon::GetSingleton()->m_CurrentSolutionList[i]->m_PanelList[j]);

			CString strSize;
			strSize.Format("%s*%s", GetFloatString(thePanel.m_RealLength, 0), GetFloatString(thePanel.m_RealWidth, 0));
			PanelViewingParam* pParam = new PanelViewingParam;
			pParam->m_pSolution = CSingleon::GetSingleton()->m_CurrentSolutionList[i];
			pParam->m_pPanel = CSingleon::GetSingleton()->m_CurrentSolutionList[i]->m_PanelList[j];

			m_lcTotalSolutionPanel.InsertItem(nItem, pSolution->m_strMaterial);			
			m_lcTotalSolutionPanel.SetItemText(nItem, 1, GetFloatString(thePanel.m_Thickness, 0));	
			m_lcTotalSolutionPanel.SetItemText(nItem, 2, strSize);	
			m_lcTotalSolutionPanel.SetItemText(nItem, 3, "111");	
			m_lcTotalSolutionPanel.SetItemData(nItem, (DWORD_PTR)pParam);
			nItem++;
		}
	}

#else

	for(int i = 0; i < CSingleon::GetSingleton()->m_BackupSolutionList.size(); i++)
	{
		CSolution* pSolution = CSingleon::GetSingleton()->m_BackupSolutionList[i];
		for(int j = 0; j < CSingleon::GetSingleton()->m_BackupSolutionList[i]->m_PanelList.size(); j++)
		{
			Panel& thePanel = *(CSingleon::GetSingleton()->m_BackupSolutionList[i]->m_PanelList[j]);

			CString strSize;
			strSize.Format("%s*%s", GetFloatString(thePanel.m_OrgLen, 0), GetFloatString(thePanel.m_OrgWidth, 0));
			PanelViewingParam* pParam = new PanelViewingParam;
			pParam->m_pSolution = CSingleon::GetSingleton()->m_BackupSolutionList[i];
			pParam->m_pPanel = CSingleon::GetSingleton()->m_BackupSolutionList[i]->m_PanelList[j];
			pParam->m_bChecked = true;

			CString strUtilization;
			strUtilization.Format("%f", thePanel.GetUtilization());
			CString strComponentCount;
			strComponentCount.Format(_T("%d"), GetComponentCountInPanel(thePanel));


			//m_lcTotalSolutionPanel.InsertItem(nItem, pSolution->m_strMaterial);	
			//m_lcTotalSolutionPanel.SetItemText(nItem, 1, strUtilization);			
			//m_lcTotalSolutionPanel.SetItemText(nItem, 2, GetFloatString(thePanel.m_Thickness, 0));	
			//m_lcTotalSolutionPanel.SetItemText(nItem, 3, strSize);	
			//m_lcTotalSolutionPanel.SetItemData(nItem, (DWORD_PTR)pParam);

			m_lcTotalSolutionPanel.InsertItem(nItem, pSolution->m_strMaterial);			
			m_lcTotalSolutionPanel.SetItemText(nItem, 1, strSize);	
			m_lcTotalSolutionPanel.SetItemText(nItem, 2, strUtilization);
			m_lcTotalSolutionPanel.SetItemText(nItem, 4, strComponentCount);
			m_lcTotalSolutionPanel.SetItemData(nItem, (DWORD_PTR)pParam);

			nItem++;
		}
	}

#endif

	m_lcTotalSolutionPanel.EnsureVisible(0, FALSE);
	m_lcTotalSolutionPanel.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	m_lcTotalMaterial.DeleteAllItems();
	nItem = 0;
	for(int i = 0; i < CSingleon::GetSingleton()->m_BackupSolutionList.size(); i++)
	{
		CSolution* pSolution = CSingleon::GetSingleton()->m_BackupSolutionList[i];

		m_lcTotalMaterial.InsertItem(nItem, pSolution->m_strMaterial);
		m_lcTotalMaterial.SetItemText(nItem, 1, GetFloatString(pSolution->m_fThickness, 1));
		m_lcTotalMaterial.SetItemText(nItem, 2, GetFloatString(pSolution->m_PanelList.size(), 0));

		// 面积 = 大板面积x数量x平均利用率
		float fAverageAvailability = 100 - GetSolutionRemainderToatlArea(*pSolution) * 100 / GetSolutionPanelTotalArea(*pSolution);
		float fUsedArea = pSolution->m_BaseInfo.m_PanelLength/1000.0 * pSolution->m_BaseInfo.m_PanelWidth/1000.0 * pSolution->GetPanelNum() * fAverageAvailability/100.0;	// 平方毫米转平方米 利用率/100

		m_lcTotalMaterial.SetItemText(nItem, 3, GetFloatString(/*GetSolutionComponentToatlArea(*pSolution)*/fUsedArea, 2));
		m_lcTotalMaterial.SetItemText(nItem, 4, GetFloatString(fAverageAvailability, 1) + _T("%"));

		nItem++;
	}
}

PanelViewingParam* CDlgTotalResult::GetSelectedItemViewingParam()
{
	//Panel* pRet = NULL;
	int nSelectedItem = m_lcTotalSolutionPanel.GetNextItem(-1, LVIS_SELECTED);
	if(nSelectedItem != -1)
	{
		PanelViewingParam* pParam = (PanelViewingParam*)m_lcTotalSolutionPanel.GetItemData(nSelectedItem);
		//if(pParam != NULL)
		//{
		//	pRet = pParam->m_pPanel;
		//}
		return pParam;
	}
	//return pRet;
	return NULL;
}

vector<Panel*> CDlgTotalResult::GetPanels(bool bChecked)
{
	vector<Panel*> vPanelRes;
	for(int i = 0; i < m_lcTotalSolutionPanel.GetItemCount(); i++)
	{
		PanelViewingParam* pParam = (PanelViewingParam*)m_lcTotalSolutionPanel.GetItemData(i);
		if(bChecked && pParam->m_bChecked)
			vPanelRes.push_back(pParam->m_pPanel);
		else if(!bChecked && !pParam->m_bChecked)
			vPanelRes.push_back(pParam->m_pPanel);
	}
	return vPanelRes;
}

bool CDlgTotalResult::IsPanelChecked(Panel* pPanel)
{
	bool bRes = false;
	for(int i = 0; i < m_lcTotalSolutionPanel.GetItemCount(); i++)
	{
		PanelViewingParam* pParam = (PanelViewingParam*)m_lcTotalSolutionPanel.GetItemData(i);
		if(pParam->m_pPanel == pPanel)
		{
			if(pParam->m_bChecked)
				bRes = true;
			break;;
		}
	}
	return bRes;
}

void CDlgTotalResult::UpdateList()
{
	for(int nItem = 0; nItem < m_lcTotalSolutionPanel.GetItemCount(); nItem++)
	{
		PanelViewingParam* pParam = (PanelViewingParam*)m_lcTotalSolutionPanel.GetItemData(nItem);
		Panel& thePanel = *(pParam->m_pPanel);

		// yuanzb 2017-7-6 显示使用原始大板长宽
		CString strSize;
		strSize.Format("%s*%s", GetFloatString(thePanel.m_OrgLen, 0), GetFloatString(thePanel.m_OrgWidth, 0));

		CString strUtilization;
		strUtilization.Format("%f", thePanel.GetUtilization());
		CString strComponentCount;
		strComponentCount.Format(_T("%d"), GetComponentCountInPanel(thePanel));		
		m_lcTotalSolutionPanel.SetItemText(nItem, 1, strSize);	
		m_lcTotalSolutionPanel.SetItemText(nItem, 2, strUtilization);
		m_lcTotalSolutionPanel.SetItemText(nItem, 4, strComponentCount);
	}
	m_lcTotalSolutionPanel.Invalidate();
}

int CDlgTotalResult::GetCurSelPanelItemID()
{
	int nRet = -1;
	nRet = m_lcTotalSolutionPanel.GetNextItem(-1,LVIS_SELECTED);

	return nRet;
}

BOOL CDlgTotalResult::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)  
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, ID_MENU_ROTATE_PASTING_COMPONENT, 0);
		return TRUE;
	}
	return CDialogChildBase::PreTranslateMessage(pMsg);
}
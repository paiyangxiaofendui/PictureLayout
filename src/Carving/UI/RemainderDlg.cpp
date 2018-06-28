

#include "stdafx.h"
#include "RemainderDlg.h"


#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"


#include "../../../include/DataManager/BaseDataType/RemainderManager/RemainderManager.h"
#include "../../../include/DataManager/BaseDataType/RemainderGroup/RemainderGroup.h"
#include "../../../include/DataManager/BaseDataType/RemainderItem/RemainderItem.h"


// CDirSetDlg dialog

IMPLEMENT_DYNAMIC(RemainderDlg, CDialogEx)

RemainderDlg::RemainderDlg(CWnd* pParent /*=NULL*/)
			: CDialogEx(RemainderDlg::IDD, pParent)
{






}

RemainderDlg::~RemainderDlg()
{
}

void RemainderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_REMAINDER, m_lcRemainder);
}


BEGIN_MESSAGE_MAP(RemainderDlg, CDialogEx)




END_MESSAGE_MAP()



BOOL RemainderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_lcRemainder.InsertColumn(0, "长度",LVCFMT_CENTER, 50, 0);
	m_lcRemainder.InsertColumn(1, "宽度",LVCFMT_CENTER, 50, 1);
	m_lcRemainder.InsertColumn(2, "厚度",LVCFMT_CENTER, 50, 2);
	m_lcRemainder.InsertColumn(3, "材质",LVCFMT_CENTER, 150, 3);

	// 构建余料表
	int LineNum = 0;
	CSingleon* pSgt = CSingleon::GetSingleton();

	int nSlnNum = pSgt->GetBackupSolutionNum();

	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
	{
		CSolution* pCurSln = pSgt->m_BackupSolutionList.at(i_sln);

		int nPanelNum = pCurSln->GetPanelNum();

		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pCurPanel = pCurSln->GetPanel(i_panel);

			// 获取复合条件的余料
			vector<Component*> tmp_rmd_list;
			pCurPanel->GetAllRemainder(tmp_rmd_list, pSgt->m_BaseInfo.m_MinArea,  pSgt->m_BaseInfo.m_MinLength);

			AllRmdList.insert(AllRmdList.end(), tmp_rmd_list.begin(), tmp_rmd_list.end());
		}
	}

	for (vector<Component*>::iterator it = AllRmdList.begin(); it != AllRmdList.end(); it++)
	{
		Component* pCurCpn = *it;
		CString str_len, str_width, str_thickness, str_material;

		str_len.Format("%0.1f",			pCurCpn->m_RealLength);
		str_width.Format("%0.1f",		pCurCpn->m_RealWidth);
		str_thickness.Format("%0.1f",	pCurCpn->m_Thickness);
		str_material = pCurCpn->m_Material;

		int cur_line_num = LineNum;
		m_lcRemainder.InsertItem(cur_line_num, "");

		m_lcRemainder.SetItemText(cur_line_num, 0, str_len.GetBuffer());
		m_lcRemainder.SetItemText(cur_line_num, 1, str_width.GetBuffer());
		m_lcRemainder.SetItemText(cur_line_num, 2, str_thickness.GetBuffer());
		m_lcRemainder.SetItemText(cur_line_num, 3, str_material.GetBuffer());


		LineNum++;
	}

	return TRUE;
}


void RemainderDlg::OnOK()
{
	// 添加余料到余料管理器

	CSingleon* pSingleton = CSingleon::GetSingleton();
	RemainderManager& RmdMgr = pSingleton->m_RemainderManager;

	for (vector<Component*>::iterator it = AllRmdList.begin(); it != AllRmdList.end(); it++)
	{
		Component* pCurCpn = *it;

		RemainderItem* pRmd = new RemainderItem();

		pRmd->m_Length		= pCurCpn->m_RealLength;
		pRmd->m_Width		= pCurCpn->m_RealWidth;
		pRmd->m_Material	= pCurCpn->m_Material;
		pRmd->m_Thickness	= pCurCpn->m_Thickness;
		pRmd->m_nCount		= 1;

		RmdMgr.AddRemainderItem(pRmd);
	}


	CDialogEx::OnOK();
}
void RemainderDlg::OnCancel()
{
	
	//
	CDialogEx::OnCancel();

}
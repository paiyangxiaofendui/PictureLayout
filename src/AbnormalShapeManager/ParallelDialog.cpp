#include "stdafx.h"
#include "ParallelDialog.h"


#include "DataManager/CSingleton.h"
#include "DataManager/Figure/Figure.h"
#include "DataManager/Layer/Layer.h"
#include "GeneralInterface/GeneralInterface.h"

IMPLEMENT_DYNAMIC(ParallelDialog, CDialog)

ParallelDialog::ParallelDialog(CWnd* pParentWnd)
				:CDialog(ParallelDialog::IDD, pParentWnd)
{
	m_parallel_dist = 2.0f;
	m_parallel_dir = 1;
}
ParallelDialog::~ParallelDialog()
{

}
BEGIN_MESSAGE_MAP(ParallelDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ParallelDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, &ParallelDialog::OnClickedRadioRight)
	ON_BN_CLICKED(IDC_RADIO_LEFT, &ParallelDialog::OnClickedRadioLeft)
END_MESSAGE_MAP()


void ParallelDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	int i, nCount;
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = pSingleton->GetLayer(0);
	vector<Figure*>* pSelectList = pSingleton->GetSelectFigureList();
	vector<Figure*> ParallelFigureList;
	
// 	nCount = pSelectList->size();
// 	for (i = 0; i < nCount; i++)
// 	{
// 		Figure* pFig = pSelectList->at(i);
// 		Figure* pParallelFig = pFig->CalParallelFigure(m_parallel_dir, m_parallel_dist);
// 
// 		if (pParallelFig != NULL)
// 		{
// 			pParallelFig->CalOutlineList();
// 			pLayer->AddFigure(pParallelFig);
// 		}
// 	}

	BOOL rt = GeneralInterface::ParallelFigureList(pSelectList, &ParallelFigureList, m_parallel_dir, m_parallel_dist);
	nCount = ParallelFigureList.size();
	for (i = 0; i < nCount; i++)
	{
		Figure* pParallelFig = ParallelFigureList.at(i);

		pParallelFig->CalOutlineList();
		pLayer->AddFigure(pParallelFig);
	}


	
	CDialog::OnOK();
}


void ParallelDialog::OnClickedRadioRight()
{
	// TODO: 在此添加控件通知处理程序代码
	m_parallel_dir = 1;
}



void ParallelDialog::OnClickedRadioLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	m_parallel_dir = 0;
}



void ParallelDialog::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIST, m_parallel_dist);
	DDV_MinMaxFloat(pDX, m_parallel_dist, 1, 100);
	DDX_Radio(pDX, IDC_RADIO_LEFT, m_parallel_dir);
	DDV_MinMaxInt(pDX, m_parallel_dir, 0, 1);
}

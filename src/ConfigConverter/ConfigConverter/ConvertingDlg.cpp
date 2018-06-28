// ConvertingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigConverter.h"
#include "ConvertingDlg.h"
#include "afxdialogex.h"
#include "Misc/Misc.h"
#include "Misc/ProgramMisc.h"


// CConvertingDlg 对话框

IMPLEMENT_DYNAMIC(CConvertingDlg, CDialog)

CConvertingDlg::CConvertingDlg(LPCTSTR szSourceDir, LPCTSTR szTargetDir, CWnd* pParent /*=NULL*/)
	: CDialog(CConvertingDlg::IDD, pParent)
	, m_strSourceDir(szSourceDir)
	, m_strTargetDir(szTargetDir)
	, m_hConvertThread(NULL)
{

}

CConvertingDlg::~CConvertingDlg()
{
}

void CConvertingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CConvertingDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_PROGRESS_BAR, &CConvertingDlg::OnUpdateProgressBar)
END_MESSAGE_MAP()


// CConvertingDlg 消息处理程序
BOOL CConvertingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CConvertingDlg::StartConvertThread()
{
	m_ThreadData.m_pDlg = this;
	m_ThreadData.m_strSourceDir = m_strSourceDir;
	m_ThreadData.m_strTargetDir = m_strTargetDir;
	m_hConvertThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FuncConvert, &m_ThreadData, NULL, NULL);
}

BOOL CConvertingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	StartConvertThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

DWORD CConvertingDlg::FuncConvert(PVOID lpThreadParameter)
{
	ConvertThreadData* pData = (ConvertThreadData*)(lpThreadParameter);

	vector<CString> vAllSourceDir;
	vAllSourceDir = TraversFile(pData->m_strSourceDir, _T("hgm"));

	PreProcessSourceDir(vAllSourceDir);

	CString strErrorMsg;
	for(int i = 0; i < vAllSourceDir.size(); i++)
	{
		CString strTargetFileFullPath;
		CString strMovingFileSubPath;
		strMovingFileSubPath = vAllSourceDir[i].Right(vAllSourceDir[i].GetLength() - pData->m_strSourceDir.GetLength());
		strTargetFileFullPath = pData->m_strTargetDir + strMovingFileSubPath;
		bool bRes = CopyTheFileToTarget(vAllSourceDir[i], strTargetFileFullPath);
		if(bRes)
			bRes = ConvertTheFile(strTargetFileFullPath);
		if(!bRes)
			strErrorMsg += vAllSourceDir[i] + _T("\n");

		::PostMessage(pData->m_pDlg->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, vAllSourceDir.size(), i);
	}

	if(!CopyTheFileToTarget(pData->m_strSourceDir + _T("Knife.ini"), pData->m_strTargetDir + _T("Knife.ini")))
	{
		strErrorMsg += pData->m_strSourceDir + _T("Knife.ini");
	}

	if(!strErrorMsg.IsEmpty())
		AfxMessageBox(strErrorMsg);
	
	::PostMessage(pData->m_pDlg->GetSafeHwnd(), WM_CLOSE, 0, 0);
	//vector<Component*> vAllComponent;
	//FindAllComponentInSingleton(vAllComponent);


	//float tiebiao_width = 0;
	//float tiebiao_height = 0;
	//int nLabelPosType = LABEL_POS_CENTER;
	//GetTiebiaoSize(tiebiao_width,tiebiao_height);
	//ParamSettingDlg settings;
	//settings.loadSettings();
	//nLabelPosType = settings.LabelPosType();

	////标签区域膨胀一定尺寸放置刮到边了
	//float expandSize = 10.0;
	//tiebiao_width = tiebiao_width + expandSize + expandSize;
	//tiebiao_height = tiebiao_height + expandSize + expandSize;

	//if (!settings.Tiebiao_auto_all_not())
	//{
	//	for(int i = 0; i < vAllComponent.size() ; i++)
	//	{
	//		//获得的是标签的中心点
	//		PointFloat tiePoint = createTiebiaoPosition(*(vAllComponent[i]),tiebiao_width,tiebiao_height, nLabelPosType);

	//		PointFloat fpoint;
	//		fpoint.x = tiePoint.x + settings.Tiebiao_offset_x();
	//		fpoint.y = (vAllComponent[i]->m_RealWidth-tiePoint.y) + settings.Tiebiao_offset_y();

	//		vAllComponent[i]->m_fXLabelCenter = fpoint.x;
	//		vAllComponent[i]->m_fYLabelCenter = fpoint.y;

	//		if(i % 10 == 0)
	//			::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, vAllComponent.size(), i);
	//	}
	//}
	//else
	//{
	//	for(int i = 0; i < vAllComponent.size() ; i++)
	//	{
	//		PointFloat ptLabelCenter_NotConverted = GetDefaultLabelPos(*(vAllComponent[i]), tiebiao_width, tiebiao_height, nLabelPosType, true);
	//		PointFloat ptLabelCenter_Converted;
	//		ptLabelCenter_Converted.x = ptLabelCenter_NotConverted.x + settings.Tiebiao_offset_x();
	//		ptLabelCenter_Converted.y = (vAllComponent[i]->m_RealWidth-ptLabelCenter_NotConverted.y) + settings.Tiebiao_offset_y();

	//		vAllComponent[i]->m_fXLabelCenter = ptLabelCenter_Converted.x;
	//		vAllComponent[i]->m_fYLabelCenter = ptLabelCenter_Converted.y;

	//		if(i % 10 == 0)
	//			::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, vAllComponent.size(), i);
	//	}
	//}


	//::PostMessage(self->GetSafeHwnd(), WM_CLOSE, 0, 0);
	return 0;
}

LRESULT CConvertingDlg::OnUpdateProgressBar(WPARAM wParam, LPARAM lParam)
{
	int first_message = (int)wParam;
	int second_massage = (int)lParam;

	// 更新进度条
	m_ProgressBar.SetRange32(0, first_message);
	second_massage = second_massage % first_message+1;
	m_ProgressBar.SetPos(second_massage);

	// 更新文本进度
	float cur_time = lParam;
	float total_time = wParam;
	float progress = cur_time*100/total_time;
	CString strProgress;

	strProgress.Format("当前进度：%0.0f%%......", progress);
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strProgress);

	return 0;
}
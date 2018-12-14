// DirSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptimizeDlg.h"

#include "../DataProcess/DataProcess.h"

#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"


// 每优化一轮发一次结果
#define WM_CALCULATE			WM_USER + 10086
#define WM_UPDATE_PROGRESS_BAR	WM_USER + 10087
#define WM_UPDATE_REMAIN_TIME	WM_USER + 10088

int OptimizeDlg::m_LastPanelNum = 0;
BOOL OptimizeDlg::m_ThreadIsRunning = FALSE;
vector<ComponentInputItem> OptimizeDlg::m_vComponentInputItem;
vector<PreCombineItem> OptimizeDlg::m_vPreCombineItem;
vector<RemainderInputItem>  OptimizeDlg::m_vRemainderInputItem;

// CDirSetDlg dialog

IMPLEMENT_DYNAMIC(OptimizeDlg, CDialogEx)

OptimizeDlg::OptimizeDlg(CWnd* pParent /*=NULL*/)
			: CDialogEx(OptimizeDlg::IDD, pParent)
{
	// 数据初始化
	m_LastPanelNum = 0;
	m_ThreadIsRunning = FALSE;
	m_vComponentInputItem.clear();






}

OptimizeDlg::~OptimizeDlg()
{
}

void OptimizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OP_MESSAGE, m_TxtOpMessage);
	DDX_Control(pDX, IDC_BTN_PROGRESS_BAR, m_ProgressBar);
	DDX_Control(pDX, IDC_STATIC_OP_MESSAGE2, m_TxtOpMessage2);
	DDX_Control(pDX, IDC_LIST_SOLUTION_INFO, m_ListCtrlSolutionInfo);
	DDX_Control(pDX, IDC_BTN_USE_NOW, m_BtnUseNow);
	DDX_Control(pDX, IDC_STATIC_OP_REMAIN_TIME, m_TxtRemainTime);
}


BEGIN_MESSAGE_MAP(OptimizeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &OptimizeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &OptimizeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_USE_NOW, &OptimizeDlg::OnBnClickedUseNow)

	ON_MESSAGE(WM_CALCULATE, &OptimizeDlg::OnWM_CALCULATE)								//响应
	ON_MESSAGE(WM_UPDATE_PROGRESS_BAR, &OptimizeDlg::OnWM_WM_UPDATE_PROGRESS_BAR)		//响应
	ON_MESSAGE(WM_UPDATE_REMAIN_TIME, &OptimizeDlg::OnWM_WM_UPDATE_REMAIN_TIME)			//响应
	



END_MESSAGE_MAP()


void OptimizeDlg::OnBnClickedOk()
{
	

	CDialogEx::OnOK();
}


void OptimizeDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}






/*---------------------------------------*/
//	函数说明：
//		点击使用，停止线程
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
void OptimizeDlg::OnBnClickedUseNow() 
{
	if (m_ThreadIsRunning == TRUE)
	{
		m_ThreadIsRunning = FALSE;
		DWORD dw = WaitForSingleObject(m_hdthread, INFINITE);	// 等待线程结束
		switch(dw)
		{
		case WAIT_OBJECT_0:	
			// The process terminated.
			// 线程退出
			CloseHandle(m_hdthread);
			CDialogEx::OnOK();
			break;

		case WAIT_TIMEOUT:
			// The process did not terminate within 5000 milliseconds.
			CloseHandle(m_hdthread);
			AfxMessageBox("优化线程超时结束！");
			CDialogEx::OnOK();
			break;

		case WAIT_FAILED:
			// Bad call to function (invalid handle?)
			CloseHandle(m_hdthread);
			AfxMessageBox("优化线程退出无效！");
			CDialogEx::OnOK();
			break;
		}
	}
	else
	{
		CDialogEx::OnOK();
	}

}

//窗口初始化
BOOL OptimizeDlg::OnInitDialog()					
{
	CDialogEx::OnInitDialog();

	// 控件初始化
	//CFont font;
	//font.CreatePointFont(100, "Arial");

	//m_ListCtrlSolutionInfo.SetFont(&font);
	HWND hwnd = ::GetDlgItem(m_ListCtrlSolutionInfo.m_hWnd, 0); 
	if(hwnd) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcSolutionInfo.SubclassWindow(hwnd); 
			} 
		} 
		else
		{
			m_hcSolutionInfo.SubclassWindow(hwnd);
		}
	}

	LONG dwStyle = GetWindowLong(m_ListCtrlSolutionInfo.m_hWnd, GWL_STYLE);
	SetWindowLong(m_ListCtrlSolutionInfo.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_ListCtrlSolutionInfo.SetExtendedStyle((m_ListCtrlSolutionInfo.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	CRect rcListCtrl;
	m_ListCtrlSolutionInfo.GetWindowRect(rcListCtrl);
	ScreenToClient(rcListCtrl);
	rcListCtrl.right+=5;
	m_ListCtrlSolutionInfo.MoveWindow(rcListCtrl);

	m_ListCtrlSolutionInfo.InsertColumn(0,"用料材质",0,180);
	m_ListCtrlSolutionInfo.InsertColumn(1,"厚度",0,60);
	m_ListCtrlSolutionInfo.InsertColumn(2,"面积",0,120);
	m_ListCtrlSolutionInfo.InsertColumn(3,"数量",0,60);

	// 禁用应用按键
	m_BtnUseNow.EnableWindow(FALSE);



	// 开始优化线程
	StartOptimizeThread();

	return  TRUE;

}

/*---------------------------------------*/
//	函数说明：
//		创建优化线程
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
void OptimizeDlg::StartOptimizeThread()
{
	
	m_hdthread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OptimizeThread, this, NULL, NULL);
	if (m_hdthread != INVALID_HANDLE_VALUE)
	{
		m_ThreadIsRunning = TRUE;	// 启动
	}



}



/*---------------------------------------*/
//	函数说明：
//		线程函数，负责优化
//
//
//	参数：
//		PVOID lpThreadParameter		-- 优化窗口指针
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
DWORD OptimizeDlg::OptimizeThread( PVOID lpThreadParameter )
{
	OptimizeDlg* self = (OptimizeDlg*)( lpThreadParameter);

	// 时间参数
	CTime last_time = CTime::GetCurrentTime();
	CTime cur_time;
	CTimeSpan  timeSpan;



	// 排样原点、优化次数
	CSingleon* pSingleton = CSingleon::GetSingleton();
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;
	//vector<ComponentList> vComponentList;
	ComponentList componentList;

	int nTotalCount = pSingleton->m_BaseInfo.m_FirstSectionOPTimes;

	// 优化循环开始
	for(int i_progress = 0; i_progress < nTotalCount; i_progress++)
	{
		


// 第一段优化
#if 1


		int i_first_op_times = i_progress;
		//for(int i_first_op_times = 0; i_first_op_times < 1/*pSingleton->m_BaseInfo.m_FirstSectionOPTimes*/; i_first_op_times++)
		{
			// 释放解决方案 
			pSingleton->ClearCurrentSolution();
			pSingleton->ClearRemainderManager();

			// 释放小板分组
			pSingleton->m_vComponentGroup.clear();

			// 输入小板分组
			//ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
			ConvertInputInfoToComponentList(m_vComponentInputItem, self->m_vPreCombineItem, componentList/*, mapComponentToPreCombine*/);

			// 由于存在无纹理比有纹理利用率更差的情况，无纹理优化时，先横竖纹各排一次
			int text_index = i_progress%5;
			float rotate_limit = pSingleton->m_BaseInfo.m_PanelLength >  pSingleton->m_BaseInfo.m_PanelWidth ?  pSingleton->m_BaseInfo.m_PanelWidth :  pSingleton->m_BaseInfo.m_PanelLength ;

			rotate_limit -= 2* pSingleton->m_BaseInfo.m_DeburringWidth;


			if (text_index == 1)
			{
				for(int i_cpn = 0; i_cpn < componentList.size(); i_cpn++)
				{
					Component* pCpn = componentList.at(i_cpn);

					// 全部用横纹排一次, 不能旋转的除外
					if (pCpn->m_Texture == TextureType_NO_TEXTURE &&
						(pCpn->m_RealLength < rotate_limit && pCpn->m_RealWidth < rotate_limit))
					{
						pCpn->m_Texture = TextureType_H_TEXTURE;
					}
					else
					{
						int a = 0;
					}
				}

			}
			else if (text_index == 2)
			{
				for(int i_cpn = 0; i_cpn < componentList.size(); i_cpn++)
				{
					Component* pCpn = componentList.at(i_cpn);

					// 全部用横纹排一次
					if (pCpn->m_Texture == TextureType_NO_TEXTURE &&
						(pCpn->m_RealLength < rotate_limit && pCpn->m_RealWidth < rotate_limit))
					{
						pCpn->m_Texture = TextureType_V_TEXTURE;
					}
					else
					{
						int a = 0;
					}
				}
			}

			// 赋值给单例类的优化原料
			SplitComponentList(componentList, pSingleton->m_vComponentGroup);

			// 余料赋值
			for(int i_rmd = 0; i_rmd < m_vRemainderInputItem.size(); i_rmd++)
			{
				RemainderInputItem& rmd_item =  m_vRemainderInputItem.at(i_rmd);

				RemainderItem* pRmd = new RemainderItem();

				pRmd->m_Length = rmd_item.m_nXLen;
				pRmd->m_Width = rmd_item.m_nYLen;
				pRmd->m_Material = rmd_item.m_strMaterial;
				pRmd->m_Thickness = rmd_item.m_fThickness;
				pRmd->m_nCount = rmd_item.m_nCount;

				pSingleton->m_RemainderManager.AddRemainderItem(pRmd);
			}






			// 优化
			if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// 最低轮廓线
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// 贪心
			{
				if (i_first_op_times == 2)
				{
					pSingleton->Layout(1, CutDir_Horizon, Org);
				}
				else if (i_first_op_times == 3)
				{
					pSingleton->Layout(1, CutDir_Vertical, Org);
				}
				else if (i_first_op_times == 4)
				{
					pSingleton->Layout(1, CutDir_Default, Org);
				}
				else
				{
					pSingleton->Layout(1, CutDir_Random, Org);
				}
			}
			else
			{
				// 组合 贪心+最低轮廓线
				int flag = pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

				if (i_first_op_times > flag) // 随机
				{
					pSingleton->Layout(0, CutDir_Random, Org);
				}
				else
				{
					pSingleton->Layout(1, CutDir_Random, Org);
				}
			}

			pSingleton->BackupBestSolution();

		}


#endif

// 暂时禁用第二、三阶段优化
#if 0

		// 第二段优化
		for (int i_second_op_times = 0; i_second_op_times < 0/*pSingleton->m_BaseInfo.m_SecondSectionOPTimes*/; i_second_op_times++)
		{
			for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
			{
				CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

				pSln->ReOptimizeEveryPanel(Org);
			}
		}




		// 针对优化率不高的板件重新排样
		for (int i_third_op_times = 0; i_third_op_times < 0/*pSingleton->m_BaseInfo.m_ThirdSectionOPTimes*/; i_third_op_times++)
		{
			for(int i_sln = 0; i_sln < pSingleton->GetBackupSolutionNum(); i_sln++)
			{
				CSolution* pSln = pSingleton->m_BackupSolutionList.at(i_sln);

				pSingleton->ReOptimizeSln(pSln, Org);
			}
		}
 
#endif


		// 每计算完一轮发一次计算完成消息
		int nPanelCount = pSingleton->GetBackupSolutionPanelNum(); 


		cur_time = CTime::GetCurrentTime();
		timeSpan = cur_time - last_time;

		int seconds = timeSpan.GetTotalSeconds();


		// 计算完一次向窗口发送刷新文字和进度条消息
		if (i_progress%10 == 0)
		{
			::PostMessage(self->GetSafeHwnd(), WM_CALCULATE, 0, nPanelCount);
			::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, nTotalCount, i_progress);
			::PostMessage(self->GetSafeHwnd(), WM_UPDATE_REMAIN_TIME, (nTotalCount<<16) |  i_progress,  seconds);
		}

		// 判断线程是否继续运行
		if (m_ThreadIsRunning == FALSE)
		{
			return 0;
		}
	}

	// 优化结束
	::PostMessage(self->GetSafeHwnd(), WM_CLOSE, 0, 0);

	return 0;
}


//完成关闭
LRESULT OptimizeDlg::OnWM_CALCULATE(WPARAM wParam, LPARAM lParam)
{
	int messageinfo = (int)wParam;
	int showmessage = (int)lParam;
	CString strMsg =  "目前最优:";

	
	switch(messageinfo)
	{
	case 0:

		if (m_LastPanelNum == 0)
		{
			m_LastPanelNum = showmessage;
		}
		else
		{
			if (showmessage < m_LastPanelNum)
			{
				m_LastPanelNum = showmessage;
				AfxMessageBox("有更优的方案出现！");
			}
		}
		strMsg.Format("已有一套解决方案: %d 块大板。点击“应用”按钮采用！", showmessage);

		m_TxtOpMessage.SetWindowText(strMsg);
		break;
	default:
		break;
	}

	

	return 0;
}

/*---------------------------------------*/
//	函数说明：
//	更新进度条
//
//
//	参数：
//		WPARAM wParam	--	总次数
//		LPARAM lParam	--	当前次数
//
//
//	返回值:
//
//
/*---------------------------------------*/
LRESULT OptimizeDlg::OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam)
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

	strProgress.Format("当前优化进度：%0.2f%%......", progress);
	m_TxtOpMessage2.SetWindowText(strProgress);




	// 更新优化信息

	CSingleon* pSingleton = CSingleon::GetSingleton();
	int nSlnCount = pSingleton->GetBackupSolutionNum();

	m_ListCtrlSolutionInfo.DeleteAllItems();
	for (int i = 0 ; i< nSlnCount ; i++)
	{
		m_ListCtrlSolutionInfo.InsertItem(i,"");
	}

	for (int iSln = 0; iSln < nSlnCount; iSln++)
	{
		CSolution* pSln = pSingleton->m_BackupSolutionList.at(iSln);

		CString str_thickness;
		CString str_panel_size;
		CString str_panel_num;

		str_thickness.Format("%0.0f", pSln->m_fThickness);
		str_panel_size.Format("%0.0f x %0.0f", pSln->m_BaseInfo.m_PanelLength, pSln->m_BaseInfo.m_PanelWidth);
		str_panel_num.Format("%d", pSln->GetPanelNum());

		m_ListCtrlSolutionInfo.SetItemText(iSln, 0, pSln->m_strMaterial);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 1, str_thickness);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 2, str_panel_size);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 3, str_panel_num);
	}

	// 使能应用按键
	m_BtnUseNow.EnableWindow(TRUE);




	return 0;
}


LRESULT OptimizeDlg::OnWM_WM_UPDATE_REMAIN_TIME(WPARAM wParam, LPARAM lParam)
{
	float total_count = wParam>>16;
	float cur_count = wParam & 0xffff;
	float seconds_cur_count = lParam;
	
	// 
	if (cur_count < 1)
	{
		cur_count = 1;
	}

	float remain_time = seconds_cur_count/cur_count*total_count - seconds_cur_count;

	CString strProgress;
	strProgress.Format("剩余时间：%0.0f秒......", remain_time);



	m_TxtRemainTime.SetWindowText(strProgress);

	return 0;
}

BOOL OptimizeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			if(m_BtnUseNow.IsWindowEnabled())
			{
				PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_USE_NOW, BN_CLICKED), (LPARAM)0/*(GetDlgItem(IDC_BTN_USE_NOW)->GetSafeHwnd())*/);  
			}
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
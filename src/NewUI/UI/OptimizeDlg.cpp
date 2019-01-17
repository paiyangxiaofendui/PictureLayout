// DirSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptimizeDlg.h"

#include "../DataProcess/DataProcess.h"

#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"


// 每优化一轮发一次结果
#define WM_CALCULATE			WM_USER + 10086
#define WM_UPDATE_PROGRESS_BAR	WM_USER + 10087
#define WM_UPDATE_REMAIN_TIME	WM_USER + 10088

int							OptimizeDlg::m_LastPanelNum = 0;
BOOL						OptimizeDlg::m_ThreadIsRunning = FALSE;
vector<ComponentInputItem>	OptimizeDlg::m_vComponentInputItem;
vector<PreCombineItem>		OptimizeDlg::m_vPreCombineItem;
vector<RemainderInputItem>  OptimizeDlg::m_vRemainderInputItem;







 BaseInfo					OptimizeDlg::m_BaseInfo;
 vector<RawMaterialInfo>	OptimizeDlg::m_vRawMaterialList;								 

 float	OptimizeDlg::m_len;				
 float	OptimizeDlg::m_width;	
 float	OptimizeDlg::m_x_space;			
 float	OptimizeDlg::m_y_space;			
 float	OptimizeDlg::m_left_offset;		
 float	OptimizeDlg::m_right_offset;		
 float	OptimizeDlg::m_top_offset;		
 float	OptimizeDlg::m_bottom_offset;		
 int	OptimizeDlg::m_arranging_origin;	

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

	m_ListCtrlSolutionInfo.InsertColumn(0,"长度",	0,	110);
	m_ListCtrlSolutionInfo.InsertColumn(1,"宽度",	0,	110);
	m_ListCtrlSolutionInfo.InsertColumn(2,"面积",	0,	110);
	m_ListCtrlSolutionInfo.InsertColumn(3,"利用率",	0,	110);

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


bool OptimizeDlg::CheckRawMaterialUsable(vector<ComponentInputItem>& vComponentInputItem, RawMaterialInfo rm_info, BaseInfo b_info)
{

	float panel_len = rm_info.m_PanelLength;
	float panel_width = rm_info.m_PanelWidth;
	float panel_offset = b_info.m_left_offset + b_info.m_right_offset;

	if (panel_width == 0.0)
	{
		panel_width = DEFAULT_WIDTH;

	}


	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "无纹理")
		{
			if(pCpn.m_fLength > panel_len - panel_offset 
				|| pCpn.m_fWidth > panel_width - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 旋转后，再次判断
				if (pCpn.m_fLength >  panel_width - panel_offset
					|| pCpn.m_fWidth > panel_len - panel_offset
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
			if (pCpn.m_fLength > panel_len - panel_offset 
				|| pCpn.m_fWidth > panel_width - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  panel_width - panel_offset 
				|| pCpn.m_fWidth > panel_len - panel_offset
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
			return false;
		}
		else
		{
			it++;
		}
	}

	// 有信息
	// 	if (strMsg.IsEmpty() != true)
	// 	{
	// 		AfxMessageBox(strMsg);
	// 	}

	return true;
}

/** 检查板件大小是否超出并删除超长板件
 *	@param[in]		
 *	@param[out]		
 *	@return			void
 *  @warning		
 *	@note			
 *	@see            
 */
void OptimizeDlg::CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
{
	CSingleton* pSingleton = CSingleton::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;
	float panel_offset = base_info.m_left_offset + base_info.m_right_offset;
	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "无纹理")
		{
			if(pCpn.m_fLength > base_info.m_PanelLength - panel_offset 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 旋转后，再次判断
				if (pCpn.m_fLength >  base_info.m_PanelWidth - panel_offset 
					|| pCpn.m_fWidth > base_info.m_PanelLength - panel_offset
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
			if (pCpn.m_fLength > base_info.m_PanelLength - panel_offset
				|| pCpn.m_fWidth > base_info.m_PanelWidth - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  base_info.m_PanelWidth - panel_offset 
				|| pCpn.m_fWidth > base_info.m_PanelLength - panel_offset
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

	CSingleton* pSingleton = CSingleton::GetSingleton();



	BaseInfo& singleton_info = pSingleton->m_BaseInfo;




	singleton_info		=	m_BaseInfo	;


	float offset = m_BaseInfo.m_left_offset + m_BaseInfo.m_right_offset;



	// 拷贝需要优化的板件，不直接处理需要优化的板件，避免原始数据遭到破坏
	vector<ComponentInputItem> vOptimizeComponent = m_vComponentInputItem;


	// 保存原始板件数据
	pSingleton->SetBackupComponentInputItem(vOptimizeComponent);

	// 检测板件超出
	CheckAndDeleteOverSizeComponentList(vOptimizeComponent);



	// 排样原点、优化次数
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;
	ComponentList componentList;

	int nTotalCount = singleton_info.m_FirstSectionOPTimes;



	// 优化循环开始
	for(int i_progress = 0; i_progress < nTotalCount; i_progress++)
	{
		int sln_index = 0;
		for (unsigned int raw_index = 0; raw_index < m_vRawMaterialList.size(); raw_index++)
		{
			RawMaterialInfo rm_info = m_vRawMaterialList.at(raw_index);

			if (CheckRawMaterialUsable(m_vComponentInputItem, rm_info, m_BaseInfo) == false)
			{
				continue;;
			}


			m_BaseInfo.m_PanelLength		= rm_info.m_PanelLength;
			m_BaseInfo.m_PanelWidth			= rm_info.m_PanelWidth;


			singleton_info.m_PanelLength		=		m_BaseInfo.m_PanelLength ;	

			if (m_BaseInfo.m_PanelWidth == 0.0)
			{
				singleton_info.m_WidthUnlimited	=		true;
				singleton_info.m_PanelWidth		=		DEFAULT_WIDTH;
			}
			else
			{
				singleton_info.m_PanelWidth		=		m_width;
			}



			



				// 第一段优化
#if 1


			int i_first_op_times = i_progress;

			// 释放解决方案 
			pSingleton->ClearCurrentSolution();
			pSingleton->ClearRemainderManager();

			// 释放小板分组
			pSingleton->m_vComponentGroup.clear();

			// 输入小板分组
			ConvertInputInfoToComponentList(vOptimizeComponent, m_vPreCombineItem, componentList);

			// 由于存在无纹理比有纹理利用率更差的情况，无纹理优化时，先横竖纹各排一次
			int text_index = i_progress%5;
			float rotate_limit = pSingleton->m_BaseInfo.m_PanelLength >  pSingleton->m_BaseInfo.m_PanelWidth ?  pSingleton->m_BaseInfo.m_PanelWidth :  pSingleton->m_BaseInfo.m_PanelLength ;

			rotate_limit -= offset ;


			// 赋值给单例类的优化原料
			SplitComponentList(componentList, pSingleton->m_vComponentGroup);


			// 优化
			if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// 最低轮廓线
			{
				pSingleton->New_Layout(0, CutDir_Horizon, Org);
			}
			else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// 贪心
			{
				if (i_first_op_times == 2)
				{
					pSingleton->New_Layout(1, CutDir_Horizon, Org);
				}
				else if (i_first_op_times == 3)
				{
					pSingleton->New_Layout(1, CutDir_Vertical, Org);
				}
				else if (i_first_op_times == 4)
				{
					pSingleton->New_Layout(1, CutDir_Default, Org);
				}
				else
				{
					pSingleton->New_Layout(1, CutDir_Random, Org);
				}
			}
			else
			{
				// 组合 贪心+最低轮廓线
				int flag = pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

				if (i_first_op_times > flag) // 随机
				{
					pSingleton->New_Layout(0, CutDir_Horizon, Org);
				}
				else
				{
					pSingleton->New_Layout(1, CutDir_Horizon, Org);
				}
			}


#endif


			// 备份较好的方案
			bool better_flag = pSingleton->BackupBetterSolution(sln_index);

// 			if (better_flag)
// 			{
// 				::PostMessage(self->GetSafeHwnd(), WM_CALCULATE, 1, pSingleton->GetBackupSolutionNum());
// 			}
			


			{
				// 每计算完一轮发一次计算完成消息
				

				cur_time = CTime::GetCurrentTime();
				timeSpan = cur_time - last_time;

				int seconds = timeSpan.GetTotalSeconds();


				// 计算完一次向窗口发送刷新文字和进度条消息
				if (i_progress%10 == 0)
				{
					::PostMessage(self->GetSafeHwnd(), WM_CALCULATE, 0, pSingleton->GetBackupSolutionNum());
					::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, nTotalCount, i_progress);
					::PostMessage(self->GetSafeHwnd(), WM_UPDATE_REMAIN_TIME, (nTotalCount<<16) |  i_progress,  seconds);
				}

				// 判断线程是否继续运行
				if (m_ThreadIsRunning == FALSE)
				{
					return 0;
				}
			}

			// 用下一个尺寸进行排样
			sln_index++;

		}
	}
	





























	

	// 优化结束
	::PostMessage(self->GetSafeHwnd(), WM_CLOSE, 0, 0);

	return 0;
}


//完成关闭
LRESULT OptimizeDlg::OnWM_CALCULATE(WPARAM wParam, LPARAM lParam)
{
	int better_info = (int)wParam;
	int show_message = (int)lParam;
	CString strMsg =  "目前最优:";

	if (better_info )
	{
		AfxMessageBox("有更优的方案出现！");
	}


	strMsg.Format("正在计算中.......已算出 %d 套用料方案，点击右上方“应用”按钮可停止计算！", show_message);

	m_TxtOpMessage.SetWindowText(strMsg);

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

	CSingleton* pSingleton = CSingleton::GetSingleton();
	int nSlnCount = pSingleton->GetBackupSolutionNum();

	m_ListCtrlSolutionInfo.DeleteAllItems();
	for (int i = 0 ; i< nSlnCount ; i++)
	{
		m_ListCtrlSolutionInfo.InsertItem(i,"");
	}

	for (int iSln = 0; iSln < nSlnCount; iSln++)
	{
		CSolution* pSln = pSingleton->m_BackupSolutionList.at(iSln);

		if (pSln)
		{
			Panel* pPanel = pSln->GetPanel(0);
			if (pPanel)
			{

				CString str_panel_len;
				CString str_panel_width;
				CString str_panel_area;
				CString str_panel_utilization;

				str_panel_len.Format("%0.3f米",			pPanel->m_OrgLen/1000);
				str_panel_width.Format("%0.3f米",			pPanel->m_OrgWidth/1000);
				str_panel_area.Format("%0.3f平米",			pPanel->m_OrgLen * pPanel->m_OrgWidth/1000000);
				str_panel_utilization.Format("%0.1f%%",	pPanel->GetUtilization()*100);

				m_ListCtrlSolutionInfo.SetItemText(iSln, 0, str_panel_len);
				m_ListCtrlSolutionInfo.SetItemText(iSln, 1, str_panel_width);
				m_ListCtrlSolutionInfo.SetItemText(iSln, 2, str_panel_area);
				m_ListCtrlSolutionInfo.SetItemText(iSln, 3, str_panel_utilization);
			}

		}


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
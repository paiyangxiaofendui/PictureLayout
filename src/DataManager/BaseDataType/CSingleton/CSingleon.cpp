/*--------------------------------------------------------------------------------------------------------------------*
///	CSingleon.cpp    --  单例类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.11.8
//	备注：
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CSingleon.h"

#include <algorithm>

#include "../CommonData/CommonData.h"
#include "../CSolution/CSolution.h"
#include "../Panel/Panel.h"


#include "../../../../include/KnifeDll/KnifeClass.h"
#include "../../Misc/ProgramMisc.h"


CSingleon* CSingleon::m_pSingleton = NULL;


// 构造
CSingleon::CSingleon()
{
	// 优化数据
	m_CurrentSolutionList.clear();
	m_BackupSolutionList.clear();	
	m_vComponentGroup.clear();

	m_pKnifeClass = new KnifeClass;

	m_uComponentInputItemSerialID = 0;
}

// 析构
CSingleon::~CSingleon()
{

}


CSingleon* CSingleon::GetSingleton()
{
	if (m_pSingleton == NULL)
	{
		m_pSingleton = new CSingleon;
	}

	return m_pSingleton;
}



/*--------------------------------------------------------------------------------------*/
//	purpose:
//		对每一组排样优化,
//	
//	param:
//		
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
int CSingleon::Layout(int Method, int CutStyle, int Org)
{
	int nGroupSize = m_vComponentGroup.size();

	for(int i = 0; i < nGroupSize; i++)
	{
		ComponentList& theComponentList = m_vComponentGroup[i];
		if(theComponentList.size() == 0)
			continue;

		// 创建解决方案并初始化数据
		CSolution* pSolution = new CSolution;

		pSolution->m_BaseInfo = m_BaseInfo;							// 基本信息
 		//pSolution->m_fPanelLength = m_BaseInfo.m_PanelLength;		// 长
 		//pSolution->m_fPanelWidth = m_BaseInfo.m_PanelWidth;			// 宽

		pSolution->m_fThickness = theComponentList[0]->m_Thickness;
		pSolution->m_strMaterial = theComponentList[0]->m_Material;

		// 先调整长度优先
		for (int j = 0; j < theComponentList.size();j++)
		{
			Component* pCpn = theComponentList[j];

			if (pCpn->m_Texture == TextureType_V_TEXTURE)
			{
				pCpn->ClockwiseRotate90();
			}
		}
		
		pSolution->m_ComponentList = theComponentList;				// 小板

		if (Method == 0)
		{
			pSolution->LayoutOptimization_RandomSortCut(CutStyle, Org);
		}
		else
		{
			pSolution->LayoutOptimization_MinWaste(CutStyle, Org);					// 优化
		}

		m_CurrentSolutionList.push_back(pSolution);
	}



	return 0;
}

// 对利用率低的板重新打散排样
void CSingleon::ReOptimizeSln(CSolution* pSln, int Org)
{
	if (pSln == NULL)
		return ;
			
	// 对现有方案优化率不足的，重新打散优化
	int ReOpPanelNum = 0, NewOpPanelNum = 0;
	int ReOpLastPanelCpnNum = 0, NewOpLastPanelCpnNum = 0;
	int i, nCount = pSln->GetPanelNum();
	vector<Panel*> ReserverList, ReOpPanelList;
	vector<Component*> ReOpCpnList;

	// 分离优化率不足的大板
	for(i = 0; i < nCount; i++)
	{
		Panel* pPanel = pSln->GetPanel(i);
		// 达到90%利用率，可以采纳
		if (pPanel->GetUtilization() > m_BaseInfo.m_ThirdSectionOAccptableUtilization)
		{
			ReserverList.push_back(pPanel);
		}
		else
		{
			vector<Node*> CpnList;

			pPanel->GetAllLeafNodes(CpnList, NodeType_NeededComponent);
			for (vector<Node*>::iterator it = CpnList.begin(); it != CpnList.end(); it++)
			{
				Node* pNode = *it;

				ReOpCpnList.push_back(static_cast<Component*>(pNode->CopyNode()));
			}

			ReOpPanelList.push_back(pPanel);
		}
	}

	if ( ReOpCpnList.size() > 0)
	{

		// 调整方向
		for (i = 0; i < ReOpCpnList.size(); i++)
		{
			Component* pCpn = ReOpCpnList[i];

			if (pCpn->m_RealWidth > pCpn->m_RealLength && pCpn->IsRotatable() == true)
			{
				pCpn->ClockwiseRotate90();
			}
		}


		// 开始重新优化
		Panel* pLastPanel;
		Component* pLargestRemainderOfReOpLastPanel, *pLargestRemainderOfNewOpLastPanel;

		ReOpPanelNum = ReOpPanelList.size();
		pLastPanel = ReOpPanelList.back();
		ReOpLastPanelCpnNum = pLastPanel->GetComponentNum();
		pLargestRemainderOfReOpLastPanel = pLastPanel->GetLargestRemainder();


		CSolution sln;

		sln.m_BaseInfo = pSln->m_BaseInfo;
		//sln.m_fPanelLength = pSln->m_fPanelLength;
		//sln.m_fPanelWidth = pSln->m_fPanelWidth;


		sln.m_fThickness = pSln->m_fThickness;
		sln.m_strMaterial = pSln->m_strMaterial;

		// 先调整长度优先
		vector<Component*> TmpCpnList;
		for (vector<Component*>::iterator it = ReOpCpnList.begin(); it != ReOpCpnList.end(); it++)
		{
			Component* pNode = static_cast<Component*>((*it)->CopyNode());

			pNode->m_pParent = NULL;

			TmpCpnList.push_back(pNode);
		}


		sln.m_ComponentList = TmpCpnList;		// 小板
		sln.m_BaseInfo = pSln->m_BaseInfo;		// 基本信息

		int cut_layout_method = rand()%2;

		if (cut_layout_method == 0)
		{
			sln.LayoutOptimization_RandomSortCut(CutDir_Random, Org);				// 优化
		}
		else
		{
			sln.LayoutOptimization_MinWaste(CutDir_Random, Org);
		}

		NewOpPanelNum = sln.GetPanelNum();
		if (NewOpPanelNum  < ReOpPanelNum)	
		{
			// 板少了
			ReOpPanelNum = NewOpPanelNum;

			for (int j = 0; j < ReOpPanelList.size(); j++)
			{
				Node* pPanel =  ReOpPanelList.at(j);
				pPanel->DeleteTree(&pPanel);
			}

			// 替换
			ReOpPanelList.clear();
			ReOpPanelList = sln.m_PanelList;
			sln.m_PanelList.clear();
		}
		else if(NewOpPanelNum  == ReOpPanelNum)	
		{
			pLastPanel = sln.m_PanelList.back();
			NewOpLastPanelCpnNum = pLastPanel->GetComponentNum();
			pLargestRemainderOfNewOpLastPanel = pLastPanel->GetLargestRemainder();

			if (NewOpLastPanelCpnNum < ReOpLastPanelCpnNum)
			{
				ReOpLastPanelCpnNum = NewOpLastPanelCpnNum;

				for (int j = 0; j < ReOpPanelList.size(); j++)
				{
					Node* pPanel =  ReOpPanelList.at(j);
					pPanel->DeleteTree(&pPanel);
				}

				// 替换
				ReOpPanelList.clear();
				ReOpPanelList = sln.m_PanelList;
				sln.m_PanelList.clear();
			}
			else if (ReOpLastPanelCpnNum == NewOpLastPanelCpnNum)
			{
				if (pLargestRemainderOfNewOpLastPanel->IsAreaLargeThan(*pLargestRemainderOfReOpLastPanel) == true)
				{
					for (int j = 0; j < ReOpPanelList.size(); j++)
					{
						Node* pPanel =  ReOpPanelList.at(j);
						pPanel->DeleteTree(&pPanel);
					}

					// 替换
					ReOpPanelList.clear();
					ReOpPanelList = sln.m_PanelList;
					sln.m_PanelList.clear();
				}

			}
		}

		// 重新整合优化方案
		pSln->m_PanelList.clear();
		pSln->m_PanelList = ReserverList;
		pSln->m_PanelList.insert(pSln->m_PanelList.end(), ReOpPanelList.begin(), ReOpPanelList.end());
	}
}



// 获取当前优化方案大板数量
int CSingleon::GetCurSolutionPanelNum(void)	
{
	int i, panel_num = 0, nCount = m_CurrentSolutionList.size();

	for (i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_CurrentSolutionList.at(i);

		panel_num += pSln->GetPanelNum();
	}

	return panel_num;
}

// 获取当前优化方案大板数量
int CSingleon::GetBackupSolutionPanelNum(void)	
{
	int i, panel_num = 0, nCount = m_BackupSolutionList.size();

	for (i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		panel_num += pSln->GetPanelNum();
	}

	return panel_num;
}

// 获取当前优化方案数量
int CSingleon::GetCurSolutionNum(void)
{
	return m_CurrentSolutionList.size();
}

// 获取当前优化方案数量
int CSingleon::GetBackupSolutionNum(void)
{
	return m_BackupSolutionList.size();
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		保存最优解决方案,如果新优化方案，比旧优化方案，大板数量少，则采取大板数少的
//		2017-4-14  yuanzb  测试时，发现存在总板数相同，但每个方案板数不同的情况
//		应该一个一个方案来比较，而不应该整体板数来比较	
//
//	param:
//		void
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void CSingleon::BackupBestSolution(void)
{
	// 还没有最优方案，直接备份
	if(m_BackupSolutionList.size() == 0)
	{

		SetBackupSolution(m_CurrentSolutionList);
	}
	else
	{

		// 测试时，发现存在总板数相同，但每个方案板数不同的情况，应该一个一个方案来比较，而不应该整体板数来比较
		// 因为板件是排过序的，解决方案的顺序也相同，所以顺序比较当前和备份的解决方案

		int nSlnCount = m_BackupSolutionList.size();
		int nCurSlnCount = m_CurrentSolutionList.size();

		// 判断
		if (nSlnCount != nCurSlnCount)
		{
			AfxMessageBox("解决方案数量不匹配！");
			return;
		}
		
		// 循坏比较每个解决方案，优者采纳
		for (int  i_sln = 0; i_sln < nSlnCount; i_sln++)
		{
			CSolution* pCurSln = m_CurrentSolutionList.at(i_sln);
			CSolution* pBackupSln = m_BackupSolutionList.at(i_sln);

			// 对比大板数量
			int CurSlnPanelNum = pCurSln->GetPanelNum();
			int BackupSlnPanelNum = pBackupSln->GetPanelNum();

			// 较优则取代 成员替换
			if (CurSlnPanelNum < BackupSlnPanelNum)									// 大板少
			{
				m_BackupSolutionList[i_sln] = pCurSln;
				m_CurrentSolutionList[i_sln] = pBackupSln;
			}
			else if (CurSlnPanelNum == BackupSlnPanelNum )							// 大板数相同，小板数少
			{
				CSolution* pLastSln = NULL;
				Panel* pLastPanel = NULL;
				int CurSlnCpnNumOfLastPanel;
				int BackupSlnCpnNumOfLastPanel;
				Component* pCurSlnLargestRemainderOfLastPanel, *pBackupSlnLargestRemainderOfLastPanel;

				pLastSln = pCurSln;
				pLastPanel = pLastSln->m_PanelList.back();
				pCurSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
				CurSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

				pLastSln = pBackupSln;
				pLastPanel = pLastSln->m_PanelList.back();
				pBackupSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
				BackupSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

				// 用板少的较优，替换
				if (CurSlnCpnNumOfLastPanel < BackupSlnCpnNumOfLastPanel)
				{
					m_BackupSolutionList[i_sln] = pCurSln;
					m_CurrentSolutionList[i_sln] = pBackupSln;
				}
				else if (CurSlnCpnNumOfLastPanel == BackupSlnCpnNumOfLastPanel)		// 大板数，小板数都相同，余料最大的优先
				{
					// 指针保护
					if (pCurSlnLargestRemainderOfLastPanel != NULL && pBackupSlnLargestRemainderOfLastPanel != NULL)
					{
						if (pCurSlnLargestRemainderOfLastPanel->IsAreaLargeThan(*pBackupSlnLargestRemainderOfLastPanel) == true)
						{
							m_BackupSolutionList[i_sln] = pCurSln;
							m_CurrentSolutionList[i_sln] = pBackupSln;
						}
					}
				}
			}
		}

		// 清除当前方案
		ClearCurrentSolution();

#if 0

		// 需要比较
		int CurSlnPanelNum = GetCurSolutionPanelNum();
		int BackupSlnPanelNum = GetBackupSolutionPanelNum();

		// 较优
		if (CurSlnPanelNum < BackupSlnPanelNum)									// 大板少
		{
			ClearBackupSolution();
			SetBackupSolution(m_CurrentSolutionList);
			ClearCurrentSolution();
		}
		else if (CurSlnPanelNum == BackupSlnPanelNum )							// 大板数相同，小板数少
		{
			CSolution* pLastSln = NULL;
			Panel* pLastPanel = NULL;
			int CurSlnCpnNumOfLastPanel;
			int BackupSlnCpnNumOfLastPanel;
			Component* pCurSlnLargestRemainderOfLastPanel, *pBackupSlnLargestRemainderOfLastPanel;

			pLastSln = m_CurrentSolutionList.back();
			pLastPanel = pLastSln->m_PanelList.back();
			pCurSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
			CurSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

			pLastSln = m_BackupSolutionList.back();
			pLastPanel = pLastSln->m_PanelList.back();
			pBackupSlnLargestRemainderOfLastPanel = pLastPanel->GetLargestRemainder();
			BackupSlnCpnNumOfLastPanel = pLastPanel->GetComponentNum();

			if (CurSlnCpnNumOfLastPanel < BackupSlnCpnNumOfLastPanel)
			{
				ClearBackupSolution();
				SetBackupSolution(m_CurrentSolutionList);
				ClearCurrentSolution();
			}
			else if (CurSlnCpnNumOfLastPanel == BackupSlnCpnNumOfLastPanel)		// 大板数，小板数都相同，余料最大的优先
			{
				if (pCurSlnLargestRemainderOfLastPanel->IsAreaLargeThan(*pBackupSlnLargestRemainderOfLastPanel) == true)
				{
					ClearBackupSolution();
					SetBackupSolution(m_CurrentSolutionList);
					ClearCurrentSolution();
				}
			}
		}
		else
		{
			ClearCurrentSolution();
		}

#endif

	}
}

// 清除所有数据
void CSingleon::ClearCurrentSolution(void)
{
	for(int i = 0; i < m_CurrentSolutionList.size(); i++)
	{
		CSolution* pSln = m_CurrentSolutionList[i];

		if (pSln != NULL)
			delete pSln;
	}

	m_CurrentSolutionList.clear();
}

/*---------------------------------------*/
//	函数说明：
//		清除所有数据
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
void CSingleon::ClearBackupSolution(void)
{
	for(int i = 0; i < m_BackupSolutionList.size(); i++)
	{
		CSolution* pSln = m_BackupSolutionList[i];

		if (pSln != NULL)
			delete pSln;
	}

	m_BackupSolutionList.clear();
}

/*---------------------------------------*/
//	函数说明：
//		清除余料管理器
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
void CSingleon::ClearRemainderManager(void)
{

	for(int i = 0; i < m_RemainderManager.m_RemainderGroupList.size(); i++)
	{
		RemainderGroup* pGroup = m_RemainderManager.m_RemainderGroupList[i];

		if (pGroup != NULL)
			delete pGroup;
	}

	m_RemainderManager.m_RemainderGroupList.clear();
}



// 设置备份优化方案
void CSingleon::SetBackupSolution(vector<CSolution*> &sln_list)
{
	ClearBackupSolution();
	
	m_BackupSolutionList = sln_list;
	sln_list.clear();
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		清除所有数据，暂时不考虑内存泄漏问题，稍后再处理
//	
//	param:
//		void
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
// 
void CSingleon::ClearAllData(void)
{
	// 释放解决方案
	ClearCurrentSolution();

	// 释放解决方案
	ClearBackupSolution();

	// 释放余料管理器
	ClearRemainderManager();

	// 释放小板分组
	m_vComponentGroup.clear();
}


// 更新解决方案号
void CSingleon::UpdateSlnNum(void)
{
	int i, nCount;

	nCount = GetBackupSolutionNum();
	for(i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		pSln->m_SlnID = i;

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			pPanel->m_PanelID = i_panel;
		}
	}

}

void CSingleon::UpdateComponentMachiningInfo(void)
{
	int i, nCount;
	nCount = GetBackupSolutionNum();

	for(i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			vector<Component*> vAllComponent;
			FindAllComponentInPanel(*pPanel, vAllComponent);

			for(int j = 0; j < vAllComponent.size(); j++)
			{
				Component& theComponent = *(vAllComponent[j]);
				theComponent.m_nRotatedAngle = theComponent.m_nRotatedAngle % 180;//锁定为“旋转了”、“没有旋转”两种
				if(theComponent.m_nRotatedAngle != 0)//当不为0时，表明排板时把板件旋转了
				{
					theComponent.m_nRotatedAngle = -90;//如排板时旋转了板件，统一定为顺时针旋转了90度
					OutlinePointsClockwiseRotate90(theComponent.m_vOutlinePoint, theComponent.m_RealWidth, theComponent.m_RealLength);
					HoleClockwiseRotate90(theComponent.m_vUpperFaceHole, theComponent.m_RealWidth, theComponent.m_RealLength);
					SlotClockwiseRotate90(theComponent.m_vUpperFaceSlot, theComponent.m_RealWidth, theComponent.m_RealLength);
					HoleClockwiseRotate90(theComponent.m_vDownerFaceHole, theComponent.m_RealWidth, theComponent.m_RealLength);
					SlotClockwiseRotate90(theComponent.m_vDownerFaceSlot, theComponent.m_RealWidth, theComponent.m_RealLength);
					BandingClockwiseRotate90(theComponent.m_afBanding);
					LabelPosClockwiseRotate90(theComponent.m_fXLabelCenter, theComponent.m_fYLabelCenter, theComponent.m_RealWidth, theComponent.m_RealLength);

					float fOrgProductXLen = theComponent.m_fProductLength;
					float fOrgProductYLen = theComponent.m_fProductWidth;
					theComponent.m_fProductLength = fOrgProductYLen;
					theComponent.m_fProductWidth = fOrgProductXLen;
					if (theComponent.m_Texture == 1)
						theComponent.m_Texture = 2;
					else if(theComponent.m_Texture == 2)
						theComponent.m_Texture = 1;
				}
			}
		}
	}
}

// 更新预组合板件：根据排样结果填充预组合板件的一些字段
void CSingleon::UpdatePreCombinedComponent(void)
{
	int i, nCount;
	nCount = GetBackupSolutionNum();

	for(i = 0; i < nCount; i++)
	{
		CSolution* pSln = m_BackupSolutionList.at(i);

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			vector<Component*> vAllComponent;
			FindAllComponentInPanel(*pPanel, vAllComponent);

			for(int j = 0; j < vAllComponent.size(); j++)
			{
				Component& theComponent = *(vAllComponent[j]);
				if(theComponent.m_ChildrenList.size() > 0)
				{
					theComponent.m_nRotatedAngle = theComponent.m_nRotatedAngle % 180;//锁定为“旋转了”、“没有旋转”两种
					for(int k = 0; k < theComponent.m_ChildrenList.size(); k++)
					{
						Component* pThePreCombinedComponent = (Component*)(theComponent.m_ChildrenList[k]);
						float fComponentLen_BeforeOpt = (pThePreCombinedComponent->m_nRotatedAngle / 90) % 2 == 0 ? pThePreCombinedComponent->m_RealLength : pThePreCombinedComponent->m_RealWidth;
						float fComponentWidth_BeforeOpt = (pThePreCombinedComponent->m_nRotatedAngle / 90) % 2 == 0 ? pThePreCombinedComponent->m_RealWidth : pThePreCombinedComponent->m_RealLength;
						float fCombinedLen_BeforeOpt = (theComponent.m_nRotatedAngle != 0) ? theComponent.m_RealWidth : theComponent.m_RealLength;
						float fCombinedWidth_BeforeOpt = (theComponent.m_nRotatedAngle != 0) ? theComponent.m_RealLength : theComponent.m_RealWidth;
						float fComponentX_Org = pThePreCombinedComponent->m_x;
						float fComponentY_Org = pThePreCombinedComponent->m_y;

						//更新被预组合板件的座标
						if(theComponent.m_nRotatedAngle != 0)//当不为0时，表明排板时把板件旋转了
						{
							pThePreCombinedComponent->m_x = theComponent.m_x + fComponentY_Org;
							pThePreCombinedComponent->m_y = theComponent.m_y + (fCombinedLen_BeforeOpt - (fComponentX_Org + fComponentLen_BeforeOpt));
						}
						else
						{
							pThePreCombinedComponent->m_x = theComponent.m_x + fComponentX_Org;
							pThePreCombinedComponent->m_y = theComponent.m_y + fComponentY_Org;
						}

						//更新被预组合板件的角度
						int nClockwiseRotateTimes = abs(GetRegularAngle(pThePreCombinedComponent->m_nRotatedAngle + theComponent.m_nRotatedAngle))/90;
						pThePreCombinedComponent->m_nRotatedAngle = 0;
						for(int l = 0; l < nClockwiseRotateTimes; l++)
						{
							ComponentRotateClockwise90(pThePreCombinedComponent);
						}
					}

					if(theComponent.m_nRotatedAngle != 0)//当不为0时，表明排板时把板件旋转了
					{
						theComponent.m_nRotatedAngle = -90;//如排板时旋转了板件，统一定为顺时针旋转了90度
					}

					theComponent.m_type = NodeType_CombinePanel;
				}
			}

			vAllComponent.clear();
			FindAllComponentInPanel(*pPanel, vAllComponent);
			//更新板内ID,把被预组合板件也算进来
			std::sort(vAllComponent.begin(),vAllComponent.end(), Sort_ComponentByMachiningOrder);
			for(int i = 0; i < vAllComponent.size(); i++)
			{
				vAllComponent[i]->m_NumberInPanel = i + 1;
			}
		}
	}
}

// 备份输入小板接口
void CSingleon::SetBackupComponentInputItem(vector<ComponentInputItem>& InputItem)
{
	m_vBackupComponentInputItem.clear();
	m_vBackupComponentInputItem = InputItem;
}
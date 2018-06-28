/*--------------------------------------------------------------------------------------------------------------------*
///	CSolution.cpp   --  解决方案类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.11.8
//	备注：
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CSolution.h"

#include <time.h>   
#include <algorithm>
#include "xfunctional"

#include "../Panel/Panel.h"
#include "../BaseInfo/BaseInfo.h"
#include "../Component/Component.h"
#include "../RemainderManager/RemainderManager.h"
#include "../RemainderGroup/RemainderGroup.h"
#include "../RemainderItem/RemainderItem.h"
#include "../../Algorithm/Algorithm.h"





// 构造
CSolution::CSolution()
{
	m_SlnID		= 0;					
	//m_fPanelLength	= 0.0f;							
	//m_fPanelWidth	= 0.0f;							
	m_fThickness	= 0.0f;								
	m_strMaterial	= "";				
}

// 析构
CSolution::~CSolution()
{
	// 清空大板链表，释放大板内存
	for (int j = 0; j < m_PanelList.size(); j++)
	{
		Node* pPanel =  m_PanelList.at(j);
		pPanel->DeleteTree(&pPanel);
	}

	m_PanelList.clear();

}


// 快排
void QuickSortComponent(vector<Component*>& List, int l, int r)  
{  
	if (l< r)  
	{        
		int i = l, j = r;
		Component* x = List[l];  
		while (i < j)  
		{  
			while(i < j && List[j]>= x) // 从右向左找第一个小于x的数  
				j--;   
			if(i < j)  
				List[i++] = List[j];  
			while(i < j && List[i]< x) // 从左向右找第一个大于等于x的数  
				i++;   
			if(i < j)  
				List[j--] = List[i];  
		}  
		List[i] = x;  
		QuickSortComponent(List, l, i - 1); // 递归调用  
		QuickSortComponent(List, i + 1, r);  
	}  
}  

// 从小到大排序
bool ComponentCompareLess(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) > (*p2);	
}

// 从小到大排序
bool ComponentCompareGreater(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) < (*p2);	
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		从小到大板件排序，长度优先，面积次之
//	
//	param:
//		void
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
// int CSolution::SortRemainderList_LengthFirst(void)
// {
// 	//  板件从大到小排序
// 	sort(m_RemainderList.begin(), m_RemainderList.end(), ComponentCompareGreater);
// 
// 	return 0;
// }


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		板件排序，长度优先，面积次之
//	
//	param:
//		void
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
int CSolution::SortComponentList_LengthFirst(void)
{
	//  板件从大到小排序
	sort(m_ComponentList.begin(), m_ComponentList.end(), ComponentCompareLess);

	return 0;
}






// 板件排序，长度优先，面积次之
int CSolution::SortComponentList_Random(void)
{
	// 随机排序;
	vector<Component*> new_list;

	srand((unsigned)time(0)); 

	// 从原始板件中随机选取小板，插入到新链表中，
	while(m_ComponentList.size() > 0)
	{
		int nCount = m_ComponentList.size();
		int sel_id = rand() % nCount;

		Component* pCpn = m_ComponentList.at(sel_id);
		new_list.push_back(pCpn);
		m_ComponentList.erase(m_ComponentList.begin() + sel_id);
	}

	// 重新赋值
	m_ComponentList = new_list;
	return 0;
}




// 检查板件大小是否超出
void CSolution::CheckComponentList(void)
{
	vector<Component*>::iterator it;
	CString strMsg;

	for (it = m_ComponentList.begin(); it != m_ComponentList.end();)
	{
		Component* pCpn = *it;

		bool bOverSize = false;
		float fOrgXLen, fOrgYLen;
		if(pCpn->m_nRotatedAngle == 0)
		{
			fOrgXLen = pCpn->m_RealLength;
			fOrgYLen = pCpn->m_RealWidth;
		}
		else
		{
			fOrgXLen = pCpn->m_RealWidth;
			fOrgYLen = pCpn->m_RealLength;
		}

		if (pCpn->IsRotatable())
		{
			if(fOrgXLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth 
				|| fOrgYLen > m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth
				|| fOrgXLen <= 0
				|| fOrgYLen <= 0)
			{
				// 旋转后，再次判断
				if (fOrgXLen >  m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth 
					|| fOrgYLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth
					|| fOrgXLen <= 0
					|| fOrgYLen <= 0)
				{
					// 还是超出，删除
					bOverSize = true;
				}
			}
		}
		else if(pCpn->m_Texture == 1)
		{
			if(fOrgXLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth 
				|| fOrgYLen > m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth
				|| fOrgXLen <= 0
				|| fOrgYLen <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}
		else
		{
			if(fOrgXLen >  m_BaseInfo.m_PanelWidth - 2*m_BaseInfo.m_DeburringWidth 
				|| fOrgYLen > m_BaseInfo.m_PanelLength - 2*m_BaseInfo.m_DeburringWidth
				|| fOrgXLen <= 0
				|| fOrgYLen <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// 报错
			strMsg += "删除超出范围板件，板件号：" + pCpn->m_BarCode + "\n";

			// 删除
			it = m_ComponentList.erase(it);
			delete pCpn;
			pCpn = NULL;
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


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		匹配合适的小板和轮廓线。
//		贪心算法的思想：每次的轮廓线利用率的一定是最大的
//		小板从大到小排序，最低轮廓线从窄到宽排序，
//		顺序匹配，每当匹配成功，必定是最大的板与最窄的轮廓线，使得每次新的轮廓线最窄
//	
//		第一轮匹配，查看能否包含
//		第二轮匹配：旋转再看是否能包含
//		第三轮匹配：最低轮廓线已经无效，清空后更新最低轮廓线链表
//		
//	
//	param:
//		int &nCpnID			--	小板ID
//		int &nOutlineID		--	轮廓线ID
//
//	return:
//		true -- 能匹配到
//		false-- 无法匹配
/*--------------------------------------------------------------------------------------*/
bool CSolution::MatchSuitableComponentNOutline(int &nCpnID, int &nOutlineID)
{

match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = m_ComponentList.size();
	int i_outline, nOutlineSize = m_LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = m_ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = m_LowestOutlineList.at(i_outline);

			if (line.Containable(pCpn) == true)
			{
				bMatchFlag = true;
				nCpnID = i_cpn;
				nOutlineID = i_outline;
				break;
			}
		}

		// 已找到
		if (bMatchFlag == true)
		{
			break;
		}

	}

	// 遍历了一次还未找到匹配的小板和轮廓线，将可旋转的旋转一下，再遍历一遍
	if(bMatchFlag == false)
	{
		for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
		{
			Component* pCpn = m_ComponentList.at(i_cpn);

			// 旋转
			if(pCpn->IsRotatable() == true)
			{
				pCpn->ClockwiseRotate90();
			}

			// 再次匹配
			for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
			{
				Outline& line = m_LowestOutlineList.at(i_outline);

				if (line.Containable(pCpn) == true)
				{
					bMatchFlag = true;
					nCpnID = i_cpn;
					nOutlineID = i_outline;
					break;
				}
			}

			// 已找到
			if (bMatchFlag == true)
			{


				break;
			}
			else
			{
				// 这里需要将旋转后的板件复原，暂未处理
				if(pCpn->IsRotatable() == true)
				{
					pCpn->CouterClockwiseRotate90();
				}
			}
		}
	}

	// 此时还未找到匹配的最低轮廓线，说明最低轮廓线已经无法使用，把最低轮廓线链表清空，把下一级调上来
	if (bMatchFlag == false)
	{
		// 清空最低轮廓线链表
		m_LowestOutlineList.clear();

		// 后面还有轮廓线，继续查找，没有则直接返回false
		if (m_OutlineList.size() > 0)
		{
			// 选出最低轮廓线
			int i, erase_id = 0;
			Outline first_line = m_OutlineList.at(0);
			int nCount = m_OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = m_OutlineList.at(i);

				if (ln.m_start_y == first_line.m_start_y)
				{
					erase_id = i;
				}
				else
				{
					break;
				}
			}

			// 将最低轮廓线元素插入最低轮廓线链表，并从轮廓线链表中移除
			for (i = 0; i <= erase_id; i++)
			{
				Outline ln = m_OutlineList.at(0);
				m_LowestOutlineList.push_back(ln);
				m_OutlineList.erase(m_OutlineList.begin());
			}

			// 重新查找
			goto match_loop;
		}
	}



	return bMatchFlag;
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		匹配合适的小板和轮廓线2，比1少了一轮
//	
//		第一轮匹配，查看能否包含，不能包含，旋转再看是否能包含
//		第二轮匹配：：最低轮廓线已经无效，清空后更新最低轮廓线链表
//		
//	
//	param:
//		int &nCpnID			--	小板ID
//		int &nOutlineID		--	轮廓线ID
//
//	return:
//		true -- 能匹配到
//		false-- 无法匹配
/*--------------------------------------------------------------------------------------*/
bool CSolution::MatchSuitableComponentNOutline2(int &nCpnID, int &nOutlineID)
{
match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = m_ComponentList.size();
	int i_outline, nOutlineSize = m_LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = m_ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = m_LowestOutlineList.at(i_outline);

			if (line.Containable(pCpn) == true)
			{
				bMatchFlag = true;
				nCpnID = i_cpn;
				nOutlineID = i_outline;
				break;
			}
			else
			{
				if (pCpn->IsRotatable() == true)
				{
					pCpn->ClockwiseRotate90();
					if (line.Containable(pCpn) == true)
					{
						bMatchFlag = true;
						nCpnID = i_cpn;
						nOutlineID = i_outline;
						break;
					}
					else
					{
						pCpn->CouterClockwiseRotate90();
					}
				}
			}
		}

		// 已找到
		if (bMatchFlag == true)
		{
			break;
		}

	}

	
	// 此时还未找到匹配的最低轮廓线，说明最低轮廓线已经无法使用，把最低轮廓线链表清空，把下一级调上来
	if (bMatchFlag == false)
	{
		// 清空最低轮廓线链表
		m_LowestOutlineList.clear();

		// 后面还有轮廓线，继续查找，没有则直接返回false
		if (m_OutlineList.size() > 0)
		{
			// 选出最低轮廓线
			int i, erase_id = 0;
			Outline first_line = m_OutlineList.at(0);
			int nCount = m_OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = m_OutlineList.at(i);

				if (ln.m_start_y == first_line.m_start_y)
				{
					erase_id = i;
				}
				else
				{
					break;
				}
			}

			// 将最低轮廓线元素插入最低轮廓线链表，并从轮廓线链表中移除
			for (i = 0; i <= erase_id; i++)
			{
				Outline ln = m_OutlineList.at(0);
				m_LowestOutlineList.push_back(ln);
				m_OutlineList.erase(m_OutlineList.begin());
			}

			// 重新查找
			goto match_loop;
		}
	}



	return bMatchFlag;
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		匹配合适的小板和轮廓线。
//		贪心算法的思想：每次的余料长度或者宽度必须利用到最大，浪费最小，选中一个完全匹配的
//						长度或者宽度就退出，长度匹配就沿长度方向切割，宽度匹配就沿宽度方向
//						切割
//		
//		
//	
//	param:
//		int &nCpnID			--	小板ID
//		int &nOutlineID		--	轮廓线ID
//
//	return:
//		true -- 能匹配到
//		false-- 无法匹配
/*--------------------------------------------------------------------------------------*/

#if 0
bool CSolution::MatchSuitableComponentNRemaider(Panel* pPanel, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder)
{
	int i_remainder, nRemainderNum = m_RemainderList.size();
	int i_cpn, nLeftCpnSize = m_ComponentList.size();

	bool bMatchFlag = false;
	bool bRotateFlag = false;
	float min_remain_length, min_remain_width, min_dist;

	for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
	{
		Component* pRmd = m_RemainderList.at(i_remainder);

		for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
		{
			Component* pCpn = m_ComponentList.at(i_cpn);

			if (pRmd->Containable(pCpn) == true)
			{
				if(bMatchFlag == false)
				{
					//第一次选中
					min_remain_length = pRmd->m_RealLength - pCpn->m_RealLength;
					min_remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

					min_dist = min(min_remain_length, min_remain_width);

					pPerfectMatchComponent = pCpn;
					pPerfectMatchRemainder = pRmd;
					bRotateFlag = false;
					rotate_flag = bRotateFlag;
					bMatchFlag = true;


					// 完全匹配，跳出
					if (min_remain_length == 0.0)
					{
						cut_dir = CutDir_Horizon;

						return true;
					}
					else if (min_remain_width == 0.0)
					{
						cut_dir = CutDir_Vertical;

						return true;
					}
					else
					{
						if (min_remain_width < min_remain_length)
						{
							cut_dir = CutDir_Vertical;
						}
						else
						{
							cut_dir = CutDir_Horizon;
						}
					}

				}
				else
				{
					float remaim_length = pRmd->m_RealLength - pCpn->m_RealLength;
					float remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

					// 遇到更匹配的
					if (remaim_length < min_dist || remain_width < min_dist)
					{
						min_remain_length = remaim_length;
						min_remain_width = remain_width;

						min_dist = min(min_remain_length, min_remain_width);

						pPerfectMatchComponent = pCpn;
						pPerfectMatchRemainder = pRmd;
						bRotateFlag = false;
						rotate_flag = bRotateFlag;

						// 完全匹配，跳出
						if (min_remain_length == 0.0)
						{
							cut_dir = CutDir_Horizon;

							return true;
						}
						else if (min_remain_width == 0.0)
						{
							cut_dir = CutDir_Vertical;

							return true;
						}
						else
						{
							if (min_remain_width < min_remain_length)
							{
								cut_dir = CutDir_Vertical;
							}
							else
							{
								cut_dir = CutDir_Horizon;
							}
						}
					}
				}
			}

			if (pCpn->IsRotatable() == true)	// 匹配时，为减少计算量和数据失真，不真正旋转，只比较数值，排样时才真正旋转
			{
				Component tmp_cpn;
				tmp_cpn.m_RealLength = pCpn->m_RealWidth;
				tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

				if (pRmd->Containable(&tmp_cpn) == true)
				{
					if(bMatchFlag == false)
					{
						//第一次选中
						min_remain_length	= pRmd->m_RealLength - pCpn->m_RealWidth;
						min_remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;


						min_dist = min(min_remain_length, min_remain_width);

						pPerfectMatchComponent = pCpn;
						pPerfectMatchRemainder = pRmd;
						bRotateFlag = true;
						bMatchFlag = true;
						rotate_flag = bRotateFlag;


						// 完全匹配，跳出
						if (min_remain_length == 0.0)
						{
							cut_dir = CutDir_Horizon;

							return true;
						}
						else if (min_remain_width == 0.0)
						{
							cut_dir = CutDir_Vertical;

							return true;
						}
						else
						{
							if (min_remain_width < min_remain_length)
							{
								cut_dir = CutDir_Vertical;
							}
							else
							{
								cut_dir = CutDir_Horizon;
							}
						}
					}
					else
					{
						float remaim_length = pRmd->m_RealLength - pCpn->m_RealWidth;
						float remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;

						// 遇到更匹配的
						if (remaim_length < min_dist || remain_width < min_dist)
						{
							min_remain_length = remaim_length;
							min_remain_width = remain_width;

							min_dist = min(min_remain_length, min_remain_width);

							pPerfectMatchComponent = pCpn;
							pPerfectMatchRemainder = pRmd;
							bRotateFlag = true;
							rotate_flag = bRotateFlag;

							// 完全匹配，跳出
							if (min_remain_length == 0.0)
							{
								cut_dir = CutDir_Horizon;

								return true;
							}
							else if (min_remain_width == 0.0)
							{
								cut_dir = CutDir_Vertical;

								return true;
							}
							else
							{
								if (min_remain_width < min_remain_length)
								{
									cut_dir = CutDir_Vertical;
								}
								else
								{
									cut_dir = CutDir_Horizon;
								}
							}
						}
					}
				}
			}
		}
	}


	return bMatchFlag;

}

#endif


// 获取大板数量
int CSolution::GetPanelNum(void)
{
	return m_PanelList.size();
}

// 获取大板

Panel* CSolution::GetPanel(int panel_num)
{
	int nCount = GetPanelNum();

	if (panel_num > nCount-1)
		return NULL;
	
	return m_PanelList[panel_num];
}







// 从小到大排序
bool CompareOutlineGreater(const Outline& first, const Outline& second) 
{              
	return first < second;	
}

// 从小到大排序
bool CompareOutlineLess(const Outline& first, const Outline& second) 
{              
	return first > second;	
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		排样优化	余料剩余最小，长或宽最大利用优先
//	
//	param:
//		
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
int CSolution::LayoutOptimization_MinWaste(int CutStyle, int Org)
{
	bool bUseRmd = false;
	Panel* pPanel;

	srand((unsigned)time(0)); 

	// 检查小板是否超出范围
	CheckComponentList();

	// 排序 长度优先 面积次之
	SortComponentList_LengthFirst();

	if (m_ComponentList.size() > 0)
	{
		// 新建大板
		pPanel = new Panel;

		// 判断有无余料可利用，有的话直接调用，没有则新建大板
		CSingleon* pSingelton = CSingleon::GetSingleton();
		RemainderManager& RmdManager = pSingelton->m_RemainderManager;
		RemainderItem* pItem = RmdManager.FindRemainder(m_strMaterial, m_fThickness);

		if (pItem != NULL)	// 有余料可用
		{
			// Panel的信息
			pPanel->m_PanelType		= PanelType_Remainder;			// 余料大板
			pPanel->m_OrgLen		= pItem->m_Length;				// 设置大板原始长
			pPanel->m_OrgWidth		= pItem->m_Width;				// 设置大板原始宽

			bUseRmd = true;

			RmdManager.SubRemainderItem(pItem);
		}
		else
		{
			// Panel的信息
			pPanel->m_PanelType		= PanelType_RawMaterial;		// 原料大板
			pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;		// 设置大板原始长
			pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;		// 设置大板原始宽

			bUseRmd = false;
		}

		// Panel的信息
		pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;								// 设置原点x 相对原始大板左下角的坐标
		pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;								// 设置原点y 相对原始大板左下角的坐标

		// Component的信息
		pPanel->m_x				= m_BaseInfo.m_DeburringWidth;								// 设置原点x 相对原始大板左下角的坐标
		pPanel->m_y				= m_BaseInfo.m_DeburringWidth;								// 设置原点y 相对原始大板左下角的坐标
		pPanel->m_RealLength	= pPanel->m_OrgLen		- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实长 需减去左右修边
		pPanel->m_RealWidth		= pPanel->m_OrgWidth	- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实宽 需减去上下修边
		pPanel->m_Material		= m_strMaterial;											// 设置大板材质
		pPanel->m_Thickness		= m_fThickness;												// 设置大板厚度
		pPanel->m_type			= NodeType_Remainder;										// 设置大板节点类型

		m_PanelList.push_back(pPanel);
	}

	// 开始排样 有小板需要排，
	while (m_ComponentList.size() > 0)
	{
		// 排版优化
		ALGORITHM_API::LayoutOnePanel_Greedy(pPanel, m_BaseInfo, m_ComponentList, CutStyle, Org);

#if 1
		// 对板件编号
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent_Sorted(CpnList);
		int nCpnCount = CpnList.size();

		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			pCpn->m_NumberInPanel = nCpnCount - i_cpn;

		}

		// 用了余料却放不进板，删除这块大板
		if (bUseRmd == true && nCpnCount == 0)
		{
			Node* pnode = pPanel;
			pPanel->DeleteTree(&pnode);
			pPanel = NULL;

			m_PanelList.pop_back();
		}

#else

		// 对板件编号
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent(CpnList);
		int nCpnCount = CpnList.size();

		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			pCpn->m_NumberInPanel = nCpnCount - i_cpn;

		}

#endif

		


	


		if (m_ComponentList.size() > 0)
		{
			// 新建大板
			pPanel = new Panel;

			// 判断有无余料可利用，有的话直接调用，没有则新建大板
			CSingleon* pSingelton = CSingleon::GetSingleton();
			RemainderManager& RmdManager = pSingelton->m_RemainderManager;
			RemainderItem* pItem = RmdManager.FindRemainder(m_strMaterial, m_fThickness);

			if (pItem != NULL)	// 有余料可用
			{
				// Panel的信息
				pPanel->m_PanelType		= PanelType_Remainder;			// 余料大板
				pPanel->m_OrgLen		= pItem->m_Length;				// 设置大板原始长
				pPanel->m_OrgWidth		= pItem->m_Width;				// 设置大板原始宽

				bUseRmd = true;
				RmdManager.SubRemainderItem(pItem);

			}
			else
			{
				// Panel的信息
				pPanel->m_PanelType		= PanelType_RawMaterial;		// 原料大板
				pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;		// 设置大板原始长
				pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;		// 设置大板原始宽

				bUseRmd = false;
			}

			// Panel的信息
			pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;								// 设置原点x 相对原始大板左下角的坐标
			pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;								// 设置原点y 相对原始大板左下角的坐标

			// Component的信息
			pPanel->m_x				= m_BaseInfo.m_DeburringWidth;								// 设置原点x 相对原始大板左下角的坐标
			pPanel->m_y				= m_BaseInfo.m_DeburringWidth;								// 设置原点y 相对原始大板左下角的坐标
			pPanel->m_RealLength	= pPanel->m_OrgLen		- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实长 需减去左右修边
			pPanel->m_RealWidth		= pPanel->m_OrgWidth	- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实宽 需减去上下修边
			pPanel->m_Material		= m_strMaterial;											// 设置大板材质
			pPanel->m_Thickness		= m_fThickness;												// 设置大板厚度
			pPanel->m_type			= NodeType_Remainder;										// 设置大板节点类型

			m_PanelList.push_back(pPanel);
		}
	}

	return 0;
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
// int CSolution::LayoutOptimization(void)
// {
// 	Panel* pPanel;
// 
// 	srand((unsigned)time(0)); 
// 
// 
// 	// 检查小板是否超出范围
// 	CheckComponentList();
// 
// 	// 排序 长度优先 面积次之
// 	SortComponentList_LengthFirst();
// 
// 	if (m_ComponentList.size() > 0)
// 	{
// 		// 新建大板
// 		pPanel = new Panel;
// 		pPanel->m_RealLength = m_fPanelLength;		// 设置大板长
// 		pPanel->m_RealWidth = m_fPanelWidth;		// 设置大板宽
// 		pPanel->m_Material = m_strMaterial;			// 设置大板材质
// 		pPanel->m_Thickness = m_fThickness;			// 设置大板厚度
// 		pPanel->m_type = NodeType_Remainder;		// 设置大板节点类型
// 
// 		m_PanelList.push_back(pPanel);
// 
// 		// 建立大板最低轮廓线链表
// 		BuildOutlineList(pPanel);
// 	}
// 
// 	// 开始排样 有小板需要排，
// 	while (m_ComponentList.size() > 0)
// 	{
// 		// 选择最合适的小板排放
// 		bool bMatchable = false;
// 		int nCpnID, nOutlineID;
// 
// 		bMatchable = MatchSuitableComponentNOutline2(nCpnID, nOutlineID);
// 		if (bMatchable == true)
// 		{
// 			Component* pPlaceCpn = m_ComponentList.at(nCpnID);				// 
// 			Outline lowest_line = m_LowestOutlineList.at(nOutlineID);
// 			Component* pParentNode = lowest_line.m_pParent;
// 			
// 			int cut_dir = rand()%2;
// 
// 			if (cut_dir == CutDir_Vertical)
// 			{
// 				// 父节点竖切一刀，父节点一分为二
// 
// 				// 左节点
// 				Component* pLeft	= new Component;
// 				pLeft->m_x			= pParentNode->m_x;
// 				pLeft->m_y			= pParentNode->m_y;
// 				pLeft->m_RealLength	= pPlaceCpn->m_RealLength;
// 				pLeft->m_RealWidth	= pParentNode->m_RealWidth;
// 				pLeft->m_Thickness	= pParentNode->m_Thickness;
// 				pLeft->m_Texture	= pParentNode->m_Texture;
// 				pLeft->m_type		= NodeType_Remainder;
// 
// 				// 右节点
// 				Component* pRight	= new Component;
// 				pRight->m_x			= pParentNode->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// 父节点左下角 + 小板长度 + 锯缝
// 				pRight->m_y			= pParentNode->m_y;
// 				pRight->m_RealLength= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
// 				pRight->m_RealWidth = pParentNode->m_RealWidth;
// 				pRight->m_Thickness = pParentNode->m_Thickness;
// 				pRight->m_Texture	= pParentNode->m_Texture;
// 				pRight->m_type		= NodeType_Remainder;
// 
// 				// 父节点一分为二, 父节点类型从余料变为组合板
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// 横切一刀，左节点再一分为二，变为需要的小板和余料
// 
// 				// 设置小板,在小板链表中删除该小板
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// 删除已排样的板件和最低轮廓线
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// 另一块余料
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x;			
// 				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;			// 父节点左下角 + 小板宽度 + 锯缝
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// 左节点添加子节点，类型变为组合板
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// 更新轮廓线，两个右节点，增加两条轮廓线
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line1.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line1);
// 
// 				if (line2.IsLegal() == true)
// 					m_OutlineList.push_back(line2);
// 			}
// 			else
// 			{
// 				// 父节点横切一刀，父节点一分为二
// 
// 				// 左节点
// 				Component* pRight		= new Component;
// 				pRight->m_x				= pParentNode->m_x;																// 父节点左下角 
// 				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;				// 父节点左下角 + 小板宽度
// 				pRight->m_RealLength	= pParentNode->m_RealLength;													// 父节点长度
// 				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
// 				pRight->m_Thickness		= pParentNode->m_Thickness;
// 				pRight->m_Texture		= pParentNode->m_Texture;
// 				pRight->m_type			= NodeType_Remainder;
// 
// 				// 右节点
// 				Component* pLeft		= new Component;																
// 				pLeft->m_x				= pParentNode->m_x;				// 父节点左下角  + 小板长度 + 锯缝
// 				pLeft->m_y				= pParentNode->m_y;				// 父节点左下角 
// 				pLeft->m_RealLength		= pParentNode->m_RealLength;	// 父节点长度 
// 				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// 小板宽度
// 				pLeft->m_Thickness		= pParentNode->m_Thickness;
// 				pLeft->m_Texture		= pParentNode->m_Texture;
// 				pLeft->m_type			= NodeType_Remainder;
// 
// 				// 父节点一分为二, 父节点类型从余料变为组合板
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// 竖切一刀，右节点再一分为二，变为需要的小板和余料
// 
// 				// 设置小板,在小板链表中删除该小板
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// 删除已排样的板件和最低轮廓线
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// 另一块余料
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// 父节点左下角 + 小板宽度 + 锯缝
// 				pSecondRight->m_y			= pLeft->m_y;															// 父节点左下角 
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// 
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;	// 父节点宽度 - 小板宽度 - 锯缝
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// 左节点添加子节点，类型变为组合板
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// 更新轮廓线，两个右节点，增加两条轮廓线
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line2.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line2);
// 
// 				if (line1.IsLegal() == true)
// 					m_OutlineList.push_back(line1);
// 			}
// 
// 
// 			// 排序
// 			sort(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), CompareOutlineGreater);
// 			sort(m_OutlineList.begin(), m_OutlineList.end(), CompareOutlineGreater);
// 
// 			int a = 0;
// 			//BuildOutlineList(pPanel);
// 		}
// 		else
// 		{
// 			// 新建大板
// 			pPanel = new Panel;
// 			pPanel->m_RealLength = m_fPanelLength;		// 设置大板长
// 			pPanel->m_RealWidth = m_fPanelWidth;		// 设置大板宽
// 			pPanel->m_Material = m_strMaterial;			// 设置大板材质
// 			pPanel->m_Thickness = m_fThickness;			// 设置大板厚度
// 			pPanel->m_type = NodeType_Remainder;		// 设置大板节点类型
// 
// 
// 			m_PanelList.push_back(pPanel);
// 
// 			// 建立大板最低轮廓线链表
// 			BuildOutlineList(pPanel);
// 		}
// 
// 	}
// 
// 
// 	// 更新最低轮廓线链表
// 
// 	// 排样完成
// 
// 	return 0;
// }


	// 排样优化
int CSolution::LayoutOptimization_RandomSortCut(int CutStyle, int Org)
{
	Panel* pPanel;

	srand((unsigned)time(0)); 


	// 检查小板是否超出范围
	CheckComponentList();

	// 排序 长度优先 面积次之
	SortComponentList_Random();

	if (m_ComponentList.size() > 0)
	{
		// 新建大板
		pPanel = new Panel;

		// Panel的信息
		pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;										// 设置大板原始长
		pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;										// 设置大板原始宽
		pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;							// 设置原点x 相对原始大板左下角的坐标
		pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;							// 设置原点y 相对原始大板左下角的坐标

		// Component的信息
		pPanel->m_x				= m_BaseInfo.m_DeburringWidth;							// 设置原点x 相对原始大板左下角的坐标
		pPanel->m_y				= m_BaseInfo.m_DeburringWidth;							// 设置原点y 相对原始大板左下角的坐标
		pPanel->m_RealLength	= m_BaseInfo.m_PanelLength	- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实长 需减去左右修边
		pPanel->m_RealWidth		=  m_BaseInfo.m_PanelWidth		- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实宽 需减去上下修边
		pPanel->m_Material		= m_strMaterial;										// 设置大板材质
		pPanel->m_Thickness		= m_fThickness;											// 设置大板厚度
		pPanel->m_type			= NodeType_Remainder;									// 设置大板节点类型

		m_PanelList.push_back(pPanel);

		// 建立大板最低轮廓线链表
		//BuildOutlineList(pPanel);
	}


	// 开始排样 有小板需要排，
	while (m_ComponentList.size() > 0)
	{
		ALGORITHM_API::LayoutOnePanel_LowestOutline(pPanel, m_BaseInfo, m_ComponentList, CutStyle, Org);

		// 对板件编号
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent(CpnList);
		int nCpnCount = CpnList.size();

		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			pCpn->m_NumberInPanel = nCpnCount - i_cpn;

		}


		if (m_ComponentList.size() > 0)
		{
			// 新建大板
			pPanel = new Panel;

			// Panel的信息
			pPanel->m_OrgLen		= m_BaseInfo.m_PanelLength;										// 设置大板原始长
			pPanel->m_OrgWidth		=  m_BaseInfo.m_PanelWidth;										// 设置大板原始宽
			pPanel->m_OriginX		= m_BaseInfo.m_DeburringWidth;							// 设置原点x 相对原始大板左下角的坐标
			pPanel->m_OriginY		= m_BaseInfo.m_DeburringWidth;							// 设置原点y 相对原始大板左下角的坐标

			// Component的信息
			pPanel->m_x				= m_BaseInfo.m_DeburringWidth;							// 设置原点x 相对原始大板左下角的坐标
			pPanel->m_y				= m_BaseInfo.m_DeburringWidth;							// 设置原点y 相对原始大板左下角的坐标
			pPanel->m_RealLength	= m_BaseInfo.m_PanelLength	- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实长 需减去左右修边
			pPanel->m_RealWidth		=  m_BaseInfo.m_PanelWidth		- 2*m_BaseInfo.m_DeburringWidth;	// 设置大板真实宽 需减去上下修边
			pPanel->m_Material		= m_strMaterial;										// 设置大板材质
			pPanel->m_Thickness		= m_fThickness;											// 设置大板厚度
			pPanel->m_type			= NodeType_Remainder;									// 设置大板节点类型

			m_PanelList.push_back(pPanel);
		}
	}







	// 开始排样 有小板需要排，
// 	while (m_ComponentList.size() > 0)
// 	{
// 		// 选择最合适的小板排放
// 		bool bMatchable = false;
// 		int nCpnID, nOutlineID;
// 
// 		bMatchable = MatchSuitableComponentNOutline2(nCpnID, nOutlineID);
// 		if (bMatchable == true)
// 		{
// 			Component* pPlaceCpn = m_ComponentList.at(nCpnID);				// 
// 			Outline lowest_line = m_LowestOutlineList.at(nOutlineID);
// 			Component* pParentNode = lowest_line.m_pParent;
// 
// 			int cut_dir = rand()%2;
// 
// 			if (cut_dir == 1)
// 			{
// 				// 父节点竖切一刀，父节点一分为二
// 
// 				// 左节点
// 				Component* pLeft	= new Component;
// 				pLeft->m_x			= pParentNode->m_x;
// 				pLeft->m_y			= pParentNode->m_y;
// 				pLeft->m_RealLength	= pPlaceCpn->m_RealLength;
// 				pLeft->m_RealWidth	= pParentNode->m_RealWidth;
// 				pLeft->m_Thickness	= pParentNode->m_Thickness;
// 				pLeft->m_Texture	= pParentNode->m_Texture;
// 				pLeft->m_type		= NodeType_Remainder;
// 
// 				// 右节点
// 				Component* pRight	= new Component;
// 				pRight->m_x			= pParentNode->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// 父节点左下角 + 小板长度 + 锯缝
// 				pRight->m_y			= pParentNode->m_y;
// 				pRight->m_RealLength= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
// 				pRight->m_RealWidth = pParentNode->m_RealWidth;
// 				pRight->m_Thickness = pParentNode->m_Thickness;
// 				pRight->m_Texture	= pParentNode->m_Texture;
// 				pRight->m_type		= NodeType_Remainder;
// 
// 				// 父节点一分为二, 父节点类型从余料变为组合板
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// 横切一刀，左节点再一分为二，变为需要的小板和余料
// 
// 				// 设置小板,在小板链表中删除该小板
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// 删除已排样的板件和最低轮廓线
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// 另一块余料
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x;			
// 				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;			// 父节点左下角 + 小板宽度 + 锯缝
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// 左节点添加子节点，类型变为组合板
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// 更新轮廓线，两个右节点，增加两条轮廓线
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line1.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line1);
// 
// 				if (line2.IsLegal() == true)
// 					m_OutlineList.push_back(line2);
// 			}
// 			else
// 			{
// 				// 父节点横切一刀，父节点一分为二
// 
// 				// 左节点
// 				Component* pRight		= new Component;
// 				pRight->m_x				= pParentNode->m_x;																// 父节点左下角 
// 				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + m_BaseInfo.m_SawKerfWidth;				// 父节点左下角 + 小板宽度
// 				pRight->m_RealLength	= pParentNode->m_RealLength;													// 父节点长度
// 				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - m_BaseInfo.m_SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
// 				pRight->m_Thickness		= pParentNode->m_Thickness;
// 				pRight->m_Texture		= pParentNode->m_Texture;
// 				pRight->m_type			= NodeType_Remainder;
// 
// 				// 右节点
// 				Component* pLeft		= new Component;																
// 				pLeft->m_x				= pParentNode->m_x;				// 父节点左下角  + 小板长度 + 锯缝
// 				pLeft->m_y				= pParentNode->m_y;				// 父节点左下角 
// 				pLeft->m_RealLength		= pParentNode->m_RealLength;	// 父节点长度 
// 				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// 小板宽度
// 				pLeft->m_Thickness		= pParentNode->m_Thickness;
// 				pLeft->m_Texture		= pParentNode->m_Texture;
// 				pLeft->m_type			= NodeType_Remainder;
// 
// 				// 父节点一分为二, 父节点类型从余料变为组合板
// 				pParentNode->m_type = NodeType_CombinePanel;
// 				pParentNode->AddChild(pLeft);
// 				pParentNode->AddChild(pRight);
// 
// 				// 竖切一刀，右节点再一分为二，变为需要的小板和余料
// 
// 				// 设置小板,在小板链表中删除该小板
// 				pPlaceCpn->m_pParent = pLeft;
// 				pPlaceCpn->m_x = pLeft->m_x;
// 				pPlaceCpn->m_y = pLeft->m_y;
// 				pPlaceCpn->m_type = NodeType_NeededComponent;
// 
// 				// 删除已排样的板件和最低轮廓线
// 				ComponentList::iterator it_cpn;
// 				it_cpn = find(m_ComponentList.begin(), m_ComponentList.end(), pPlaceCpn);
// 				m_ComponentList.erase(it_cpn);
// 
// 				vector<Outline>::iterator it_line;
// 				it_line = find(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), lowest_line);
// 				m_LowestOutlineList.erase(it_line);
// 
// 
// 
// 				// 另一块余料
// 				Component* pSecondRight		= new Component;
// 				pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + m_BaseInfo.m_SawKerfWidth;			// 父节点左下角 + 小板宽度 + 锯缝
// 				pSecondRight->m_y			= pLeft->m_y;															// 父节点左下角 
// 				pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - m_BaseInfo.m_SawKerfWidth;	// 
// 				pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;	// 父节点宽度 - 小板宽度 - 锯缝
// 				pSecondRight->m_Thickness	= pLeft->m_Thickness;
// 				pSecondRight->m_Texture		= pLeft->m_Texture;
// 				pSecondRight->m_type		= NodeType_Remainder;
// 
// 				// 左节点添加子节点，类型变为组合板
// 				pLeft->m_type = NodeType_CombinePanel;
// 				pLeft->AddChild(pPlaceCpn);
// 				pLeft->AddChild(pSecondRight);
// 
// 
// 				// 更新轮廓线，两个右节点，增加两条轮廓线
// 				Outline line1, line2;
// 				line1.SetOutline(pRight->m_x, pRight->m_x+pRight->m_RealLength, pRight->m_y, pRight->m_y + pRight->m_RealWidth, pRight);
// 				line2.SetOutline(pSecondRight->m_x, pSecondRight->m_x+pSecondRight->m_RealLength, pSecondRight->m_y, pSecondRight->m_y + pSecondRight->m_RealWidth, pSecondRight);
// 
// 				if (line2.IsLegal() == true)
// 					m_LowestOutlineList.push_back(line2);
// 
// 				if (line1.IsLegal() == true)
// 					m_OutlineList.push_back(line1);
// 			}
// 
// 
// 			// 排序
// 			sort(m_LowestOutlineList.begin(), m_LowestOutlineList.end(), CompareOutlineGreater);
// 			sort(m_OutlineList.begin(), m_OutlineList.end(), CompareOutlineGreater);
// 
// 			int a = 0;
// 			//BuildOutlineList(pPanel);
// 		}
// 		else
// 		{
// 			// 新建大板
// 			pPanel = new Panel;
// 			pPanel->m_RealLength = m_fPanelLength;		// 设置大板长
// 			pPanel->m_RealWidth = m_fPanelWidth;		// 设置大板宽
// 			pPanel->m_Material = m_strMaterial;			// 设置大板材质
// 			pPanel->m_Thickness = m_fThickness;			// 设置大板厚度
// 			pPanel->m_type = NodeType_Remainder;		// 设置大板节点类型
// 
// 
// 			m_PanelList.push_back(pPanel);
// 
// 			// 建立大板最低轮廓线链表
// 			BuildOutlineList(pPanel);
// 		}
// 
// 	}


	// 更新最低轮廓线链表

	// 排样完成

	return 0;
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		初始化轮廓线链表
//	
//	param:
//		
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
int CSolution::BuildOutlineList(Panel* pParent)
{
	// 清空轮廓线链表
	m_OutlineList.clear();
 	m_LowestOutlineList.clear();

	// 获取余料，创建轮廓线
	vector<Node*> list;
	pParent->GetAllLeafNodes(list, NodeType_Remainder);	

	// 创建轮廓线
	int i, nCount;
	nCount = list.size();

	// 无余料
	if(nCount == 0)
		return 0;

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		Outline line;
		float start_x = pCpn->m_x;
		float end_x = pCpn->m_x + pCpn->m_RealLength;
		float start_y = pCpn->m_y;
		float end_y = pCpn->m_y + pCpn->m_RealWidth;
		
		// 设置轮廓线
		line.SetOutline(start_x, end_x, start_y, end_y, pCpn);
		m_OutlineList.push_back(line);
	}

	// 从小到大排序
	sort(m_OutlineList.begin(), m_OutlineList.end(), CompareOutlineGreater); 

	// 选出最低轮廓线
	int erase_id = 0;
	Outline first_line = m_OutlineList.at(0);
	nCount = m_OutlineList.size();

	for (i = 1; i < nCount; i++)
	{
		Outline ln = m_OutlineList.at(i);

		if (ln.m_start_y == first_line.m_start_y)
		{
			erase_id = i;
		}
		else
		{
			break;
		}
	}

	// 将最低轮廓线元素插入最低轮廓线链表，并从轮廓线链表中移除
	for (i = 0; i <= erase_id; i++)
	{
		Outline ln = m_OutlineList.at(0);
		m_LowestOutlineList.push_back(ln);
		m_OutlineList.erase(m_OutlineList.begin());
	}


	return 0;
}


// 初始化余料线链表
// int CSolution::BuildRemainderList(Panel* pParent)
// {
// 	// 清空余料线链表
// 	m_RemainderList.clear();
// 
// 	// 获取余料，创建轮廓线
// 	vector<Node*> list;
// 	pParent->GetAllLeafNodes(list, NodeType_Remainder);	
// 
// 	int i, nCount;
// 	nCount = list.size();
// 
// 	// Node*转为Component*
// 	for(i = 0; i < nCount; i++)
// 	{
// 		Component* pNode = static_cast<Component*>(list[i]);
// 		m_RemainderList.push_back(pNode);
// 	}
// 
// 	return 0;
// }


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		解决方案内部重新优化,思路：
//			1、对备份的优化方案中的每一块大板重新排布，得到最大余料
//			2、从后面的大板中查找是否有足够小的余料，可以排到当前大板中
//			3、以此类推，到最后一块板
//	
//	param:
//		
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/

//#define  TEST_FLAG		2

void CSolution::ReOptimizeEveryPanel(int Org)
{
	vector<Panel*>::iterator it_panel_begin, it_panel_end;

	it_panel_begin = m_PanelList.begin();
	it_panel_end = m_PanelList.end();


	for (vector<Panel*>::iterator  it_panel = it_panel_begin; it_panel != it_panel_end; it_panel++)
	{
		bool bBetterFlag = false;
		Panel* pCurpanel = *it_panel;
		vector<Component*> CurCpnList, NewCpnList;

		// 拷贝一份大板 一份小板，
		Panel* pNewPanel = new Panel(*pCurpanel);

		pCurpanel->GetAllNeededComponent(CurCpnList);
		ALGORITHM_API::CopyComponentList(CurCpnList, NewCpnList);

		/************************************************************************/
		/*                          开始重拍                                    */
		/************************************************************************/
	

//#if	(TEST_FLAG == CutDir_Horizon)

		// 新的小板排序 -- 贪心算法-全部横切
		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Horizon, Org) == 0)
		{
			// 新的大板比旧大板余料大，在后续的大板中找小板，插到当前板中
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// 交换
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}

		// 新大板恢复成原始状态
		pNewPanel->Recover(NewCpnList);

//#elif (TEST_FLAG == CutDir_Vertical)



		// 新的小板排序 -- 贪心算法-全部竖切

		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Vertical, Org) == 0)
		{
			// 新的大板比旧大板余料大，在后续的大板中找小板，插到当前板中
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// 交换
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}
		
		// 新大板恢复成原始状态
		pNewPanel->Recover(NewCpnList);

//#elif  (TEST_FLAG == CutDir_Default)



		// 新的小板排序 -- 贪心算法-全部默认切
		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Default, Org) == 0)
		{
			// 新的大板比旧大板余料大，在后续的大板中找小板，插到当前板中
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// 交换
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}
		
		// 新大板恢复成原始状态
		pNewPanel->Recover(NewCpnList);

//#else

		vector<Component*> new_list, cur_list;

		// 新的小板排序 -- 贪心算法-全部随机切
		if (ALGORITHM_API::LayoutOnePanel_Greedy(pNewPanel, m_BaseInfo, NewCpnList, CutDir_Random, Org) == 0)
		{
			// 新的大板比旧大板余料大，在后续的大板中找小板，插到当前板中
			if (pNewPanel->IsBetterThan(pCurpanel) == true)
			{
				// 交换
				
				*it_panel = pNewPanel;
				pNewPanel = pCurpanel;
				pCurpanel = *it_panel;

				bBetterFlag = true;
			}
		}

//#endif
		

		pCurpanel->GetAllNeededComponent(cur_list);
		pNewPanel->GetAllNeededComponent(new_list);

		// 释放重排大板
		Node* pTmp = pNewPanel;
		pNewPanel->DeleteTree(&pTmp);
		pNewPanel = NULL;



		/************************************************************************/
		/*              查看后面的板有没有可以塞进来的                          */
		/************************************************************************/
		
		if (bBetterFlag == true)
		{
			vector<Panel*>::iterator it_left_panel;
			Component* pRmd = pCurpanel->GetLargestRemainder();

			for (it_left_panel = it_panel + 1; it_left_panel != it_panel_end; it_left_panel++)
			{
				Panel* pLeftPanel = *it_left_panel;

				vector<Component*> left_cpn_list;
				pLeftPanel->GetAllNeededComponent(left_cpn_list);

				for (int i_cpn = 0; i_cpn < left_cpn_list.size(); i_cpn++)
				{
					Component* pCpn = left_cpn_list.at(i_cpn);

					// 包含、插入
					if (pRmd->Containable(pCpn) == true)
					{
						Panel::RecoverOneComponent(pCpn);
						ALGORITHM_API::KnifeOneRemainder(pRmd, pCpn, 0, m_BaseInfo.m_SawKerfWidth, Org);

						return;
					}
					else if (pCpn->IsRotatable() == true)
					{
						Component tmp_cpn;
						tmp_cpn.m_RealLength = pCpn->m_RealWidth;
						tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

						if (pRmd->Containable(&tmp_cpn) == true)
						{
							// 旋转后塞入
							pCpn->ClockwiseRotate90();

							Panel::RecoverOneComponent(pCpn);
							ALGORITHM_API::KnifeOneRemainder(pRmd, pCpn, 0, m_BaseInfo.m_SawKerfWidth, Org);
							return;
						}

					}
				}
			}
		}
	}
}


// 面积从大到小排序
bool DescendingUtilization( Panel* pfirst,  Panel* psecond) 
{                 
	float u1 = pfirst->GetUtilization();
	float u2 = psecond->GetUtilization();

	if (u1 > u2)
	{
		return true;
	}
	else
	{
		return false;
	}

}


/*---------------------------------------*/
//	函数说明：
//		对大板列表按照优化率进行排序
//		余料板优先，原料板排在后面
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
void CSolution::SortPanelList(void)
{
	// 将大板列表一分为二

	vector<Panel*> RemainderList, RawMaterialList;

	for (int i_panel = 0; i_panel < m_PanelList.size(); i_panel++)
	{
		Panel* pPanel = m_PanelList.at(i_panel);

		if (pPanel->m_PanelType == PanelType_RawMaterial)
		{
			RawMaterialList.push_back(pPanel);
		}
		else
		{
			RemainderList.push_back(pPanel);
		}

	}

	// 清空原列表
	m_PanelList.clear();

	// 余料表排序
	sort(RemainderList.begin(), RemainderList.end(), DescendingUtilization);

	// 原料表排序
	sort(RawMaterialList.begin(), RawMaterialList.end(), DescendingUtilization);

	// 插入原链表
	m_PanelList.insert(m_PanelList.end(), RemainderList.begin(), RemainderList.end());
	m_PanelList.insert(m_PanelList.end(), RawMaterialList.begin(), RawMaterialList.end());

	

}
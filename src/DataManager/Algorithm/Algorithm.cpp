#include "stdafx.h"
#include "Algorithm.h"
#include "../BaseDataType/CommonData/CommonData.h"
#include "../BaseDataType/Outline/Outline.h"
#include <algorithm>



// 板件排序，长度优先，面积次之
int ALGORITHM_API::SortComponentList_Random(vector<Component*>& ComponentList)
{
	// 随机排序;
	vector<Component*> new_list;

	srand((unsigned)time(0)); 

	// 从原始板件中随机选取小板，插入到新链表中，
	while(ComponentList.size() > 0)
	{
		int nCount = ComponentList.size();
		int sel_id = rand() % nCount;

		Component* pCpn = ComponentList.at(sel_id);
		new_list.push_back(pCpn);
		ComponentList.erase(ComponentList.begin() + sel_id);
	}

	// 重新赋值
	ComponentList = new_list;
	return 0;
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
bool ALGORITHM_API::MatchSuitableComponentNOutline2(vector<Component*>& ComponentList,vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList, int &nCpnID, int &nOutlineID)
{
match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = ComponentList.size();
	int i_outline, nOutlineSize = LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = LowestOutlineList.at(i_outline);

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
		LowestOutlineList.clear();

		// 后面还有轮廓线，继续查找，没有则直接返回false
		if (OutlineList.size() > 0)
		{
			// 选出最低轮廓线
			int i, erase_id = 0;
			Outline first_line = OutlineList.at(0);
			int nCount = OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = OutlineList.at(i);

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
				Outline ln = OutlineList.at(0);
				LowestOutlineList.push_back(ln);
				OutlineList.erase(OutlineList.begin());
			}

			// 重新查找
			goto match_loop;
		}
	}



	return bMatchFlag;
}

// 从小到大排序
bool ALGORITHM_API::CompareOutlineGreater(const Outline& first, const Outline& second) 
{              
	return first < second;	
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
int ALGORITHM_API::BuildOutlineList(Panel* pParent, vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList)
{
	// 清空轮廓线链表
	OutlineList.clear();
	LowestOutlineList.clear();

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
		OutlineList.push_back(line);
	}

	// 从小到大排序
	sort(OutlineList.begin(), OutlineList.end(), CompareOutlineGreater); 

	// 选出最低轮廓线
	int erase_id = 0;
	Outline first_line = OutlineList.at(0);
	nCount = OutlineList.size();

	for (i = 1; i < nCount; i++)
	{
		Outline ln = OutlineList.at(i);

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
		Outline ln = OutlineList.at(0);
		LowestOutlineList.push_back(ln);
		OutlineList.erase(OutlineList.begin());
	}


	return 0;
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
int ALGORITHM_API::LayoutOnePanel_LowestOutline(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Outline> OutlineList, LowestOutlineList;
	
	// 指针判断
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// 排序 长度优先 面积次之
	SortComponentList_Random(SrcComponentList);

	BuildOutlineList(pSrcPanel, OutlineList, LowestOutlineList);



	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;
		int nCpnID, nOutlineID;

		bMatchable = MatchSuitableComponentNOutline2(SrcComponentList, OutlineList, LowestOutlineList, nCpnID, nOutlineID);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = SrcComponentList.at(nCpnID);				
			Outline lowest_line = LowestOutlineList.at(nOutlineID);
			Component* pParentNode = lowest_line.m_pParent;

			// 板件ID
// 			pPlaceCpn->m_CpnID = nCpnID;
// 			nCpnID++;

			int cut_dir;

			// 选择真正切割方式
			switch(CutStyle)
			{
			case CutDir_Horizon:
				cut_dir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				cut_dir = CutDir_Vertical;
				break;
			case CutDir_Random:
				cut_dir = rand()%2;
				break;
			default:
				break;
			}

			// 排样
			ALGORITHM_API::KnifeOneRemainder(pParentNode, pPlaceCpn, cut_dir, Info.m_SawKerfWidth, Org);

			// 重建最低轮廓线
			BuildOutlineList(pSrcPanel, OutlineList, LowestOutlineList);

			// 删除已排样的板件和最低轮廓线
			vector<Component*>::iterator it_cpn;
			it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPlaceCpn);
			SrcComponentList.erase(it_cpn);
		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// 排完了
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// 排完了

	return 0;
}



/*---------------------------------------------------------*/
//	函数说明：
//		在大板中优化排样
//
//	参数：
//		Panel * pSrcPanel					--	要排样的大板
//		BaseInfo & base_info				--	排样所需的基础信息
//		vector<Component*> & SrcCpnList		--	小板列表
//		int cut_style						--	切割方式
//
//	返回值:
//		0		--	大板排完了,没有小板
//		1		--	大板排完了,还有小板
//		其他	--	出错
/*---------------------------------------------------------*/
int ALGORITHM_API::LayoutOnePanel_Greedy(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Component*> RemainderList;
	
	// 指针判断
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// 建立余料链表
	BuildRemainderList(pSrcPanel, RemainderList);

	// 如果启用反面信息优先排列
	if (Info.m_bDownerFaceFirst == true)
	{
		vector<Component*> DownerInfoCpnList;
		vector<Component*>::iterator it;
		for (it = SrcComponentList.begin(); it != SrcComponentList.end();)
		{
			Component* pCpn = *it;

			if (pCpn->HaveDownerFaceHole() == true || pCpn->HaveDownerFaceSlot() == true)
			{
				// 从原链表删除，插入反面信息链表
				it = SrcComponentList.erase(it);
				DownerInfoCpnList.push_back(pCpn);
			}
			else
			{
				it++;
			}
		}

		// 插在头部
		SrcComponentList.insert(SrcComponentList.begin(), DownerInfoCpnList.begin(), DownerInfoCpnList.end());
	}


	// 还有小板需要排样
	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;					// 匹配成功标志
		bool bRotateFlag = false;					// 旋转标志
		int nRecommendCutDir = CutDir_Horizon;		// 建议切割方式
		Component* pPerfectMatchComponent = NULL, *pPerfectMatchRemainder = NULL;	// 最佳匹配的余料和小板

		bMatchable = MatchSuitableComponentNRemaider(RemainderList, SrcComponentList, pPerfectMatchComponent, bRotateFlag, nRecommendCutDir, pPerfectMatchRemainder, Info);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = pPerfectMatchComponent;		
			Component* pParentNode = pPerfectMatchRemainder;

			// 是否旋转
			if (bRotateFlag == true)
			{
				pPlaceCpn->ClockwiseRotate90();
			}

			// 选择真正切割方式
			switch(CutStyle)
			{
			case CutDir_Horizon:
				nRecommendCutDir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				nRecommendCutDir = CutDir_Vertical;
				break;
			case CutDir_Random:
				nRecommendCutDir = rand()%2;
				break;
			default:
				break;
			}

			// 排样
			ALGORITHM_API::KnifeOneRemainder(pPerfectMatchRemainder, pPerfectMatchComponent, nRecommendCutDir, Info.m_SawKerfWidth, Org);

			// 删除已排样的板件和余料
			vector<Component*>::iterator it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPerfectMatchComponent);
			SrcComponentList.erase(it_cpn);

			vector<Component*>::iterator it_rmd = find(RemainderList.begin(), RemainderList.end(), pPerfectMatchRemainder);
			RemainderList.erase(it_rmd);

			// 重新建立余料链表
			BuildRemainderList(pSrcPanel, RemainderList);

			// 余料排序
			SortRemainderList_LengthFirst(RemainderList);

		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// 排完了
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// 排完了
}


/*---------------------------------------*/
//	函数说明：
//		将一块余料切割成一块需要的小板，如果有剩余，
//		可以留下0~2块余料
//		
//		注：左节点为所需小板或包含所需小板的余料节点，
//			右节点为余料节点，与坐标方向无关
//				
//
//	参数：
//		Component* pParentNode	--	待切割的板件指针 
//		Component* pPlaceCpn	--	需要的小板指针
//		int CutDir				--	切割方向	0:横切	1:竖切
//		float SawKerfWidth		--	刀片宽度	
//		
//	返回值:
//
//
/*---------------------------------------*/
bool ALGORITHM_API::KnifeOneRemainder(Component* pParentNode, Component* pPlaceCpn, int CutDir, float SawKerfWidth, int Org)
{
	// 判断要切割的小板是否合法
	if (pPlaceCpn->IsLegal() == false)
	{
		return false;
	}


	if (CutDir == CutDir_Vertical)
	{

		// 第一刀，竖切一刀，父节点一分为二
		Component* pLeft = new Component;
		Component* pRight = new Component;

		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// 左下角

			// 左节点
			pLeft->m_x				= pParentNode->m_x;				// 父节点的x
			pLeft->m_y				= pParentNode->m_y;				// 父节点的y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// 小板的长度
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// 父节点宽度
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// 右节点
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// 父节点左下角 + 小板长度 + 锯缝
			pRight->m_y				= pParentNode->m_y;														// 父节点的y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// 父节点宽度
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_LeftTop:			// 左上角

			// 左节点
			pLeft->m_x				= pParentNode->m_x;				// 父节点的x
			pLeft->m_y				= pParentNode->m_y;				// 父节点的y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// 小板的长度
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// 父节点宽度
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// 右节点
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// 父节点左下角 + 小板长度 + 锯缝
			pRight->m_y				= pParentNode->m_y;														// 父节点的y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// 父节点宽度
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// 右下角

			// 左节点
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// 父节点的x + 父节点的长度 - 小板长度
			pLeft->m_y				= pParentNode->m_y;														// 父节点的y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// 小板的长度
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// 父节点宽度
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// 右节点
			pRight->m_x				= pParentNode->m_x;			// 父节点的x
			pRight->m_y				= pParentNode->m_y;														// 父节点的y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// 父节点宽度
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_RightTop:		// 右上角

			// 左节点
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// 父节点的x + 父节点的长度 - 小板长度
			pLeft->m_y				= pParentNode->m_y;														// 父节点的y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// 小板的长度
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// 父节点宽度
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// 右节点
			pRight->m_x				= pParentNode->m_x;			// 父节点的x
			pRight->m_y				= pParentNode->m_y;														// 父节点的y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// 父节点宽度
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		default:						// 默认左下角

			// 左节点
			pLeft->m_x				= pParentNode->m_x;
			pLeft->m_y				= pParentNode->m_y;
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// 右节点
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// 父节点左下角 + 小板长度 + 锯缝
			pRight->m_y				= pParentNode->m_y;
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pRight->m_RealWidth		= pParentNode->m_RealWidth;
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
				
		}
		



		// 父节点一分为二, 父节点类型从余料变为组合板
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}



		// 第二刀 横切一刀，左节点再一分为二，变为需要的小板和余料
		Component* pSecondRight = new Component;
		switch(Org)
		{
			case LayoutOrg_LeftBottom:		// 左下角

				// 设置小板
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;
				pPlaceCpn->m_y = pLeft->m_y;
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// 另一块余料
				pSecondRight->m_x			= pLeft->m_x;													// 父节点的x
				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;			// 父节点的y + 小板宽度 + 锯缝
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// 父节点的长
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;
			case LayoutOrg_LeftTop:			// 左上角

				// 设置小板
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;												// 父节点的x
				pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// 父节点的y + 父节点宽度 - 小板的宽度
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// 另一块余料
				pSecondRight->m_x			= pLeft->m_x;													// 父节点的x
				pSecondRight->m_y			= pLeft->m_y;													// 父节点的y
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// 父节点的长
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;						
			case LayoutOrg_RightBottom:		// 右下角

				// 设置小板
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;	// 父节点的x
				pPlaceCpn->m_y = pLeft->m_y;	// 父节点的y
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// 另一块余料
				pSecondRight->m_x			= pLeft->m_x;													// 父节点的x
				pSecondRight->m_y			= pLeft->m_y  + pPlaceCpn->m_RealWidth + SawKerfWidth;			// 父节点的y + 小板的宽度 + 锯缝
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// 父节点的长
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;




				break;
			case LayoutOrg_RightTop:		// 右上角

				// 设置小板
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;												// 父节点的x
				pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// 父节点的y + 父节点宽度 - 小板的宽度
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// 另一块余料
				pSecondRight->m_x			= pLeft->m_x;													// 父节点的x
				pSecondRight->m_y			= pLeft->m_y;													// 父节点的y
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// 父节点的长
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;


				break;
			default:						// 默认左下角

				// 设置小板,在小板链表中删除该小板
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;
				pPlaceCpn->m_y = pLeft->m_y;
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// 另一块余料
				pSecondRight->m_x			= pLeft->m_x;			
				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;			// 父节点左下角 + 小板宽度 + 锯缝
				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// 父节点宽度 - 小板宽度 - 锯缝
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;
		}


		// 左节点添加子节点，类型变为组合板
		pLeft->m_type = NodeType_CombinePanel;

		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}



	}
	else
	{
		// 第一刀，横切一刀，父节点一分为二

		Component* pLeft = new Component;	
		Component* pRight = new Component;

		switch(Org)
		{
			case LayoutOrg_LeftBottom:		// 左下角

				// 左节点													
				pLeft->m_x				= pParentNode->m_x;				// 父节点左下角 
				pLeft->m_y				= pParentNode->m_y;				// 父节点左下角 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// 父节点长度 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// 小板宽度
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// 右节点
				pRight->m_x				= pParentNode->m_x;																// 父节点左下角 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// 父节点左下角 + 小板宽度
				pRight->m_RealLength	= pParentNode->m_RealLength;													// 父节点长度
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;

				break;

			case LayoutOrg_LeftTop:			// 左上角

				// 左节点																
				pLeft->m_x				= pParentNode->m_x;				// 父节点x  
				pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;				// 父节点y + 父节点宽度 - 小板宽度
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// 父节点长度 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// 小板宽度
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// 右节点
				pRight->m_x				= pParentNode->m_x;														// 父节点x
				pRight->m_y				= pParentNode->m_y ;													// 父节点y
				pRight->m_RealLength	= pParentNode->m_RealLength;											// 父节点长度
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;

			case LayoutOrg_RightBottom:		// 右下角

				// 左节点															
				pLeft->m_x				= pParentNode->m_x;				// 父节点左下角 
				pLeft->m_y				= pParentNode->m_y;				// 父节点左下角 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// 父节点长度 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// 小板宽度
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// 右节点
				pRight->m_x				= pParentNode->m_x;														// 父节点左下角 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// 父节点左下角 + 小板宽度
				pRight->m_RealLength	= pParentNode->m_RealLength;											// 父节点长度
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;

			case LayoutOrg_RightTop:		// 右上角
				// 左节点															
				pLeft->m_x				= pParentNode->m_x;														// 父节点x  
				pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;	// 父节点y + 父节点宽度 - 小板宽度
				pLeft->m_RealLength		= pParentNode->m_RealLength;											// 父节点长度 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;												// 小板宽度
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// 右节点
				pRight->m_x				= pParentNode->m_x;														// 父节点x
				pRight->m_y				= pParentNode->m_y ;													// 父节点y
				pRight->m_RealLength	= pParentNode->m_RealLength;											// 父节点长度
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;

				break;

			default:						// 默认左下角

				// 左节点															
				pLeft->m_x				= pParentNode->m_x;				// 父节点左下角 
				pLeft->m_y				= pParentNode->m_y;				// 父节点左下角 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// 父节点长度 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// 小板宽度
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// 右节点
				pRight->m_x				= pParentNode->m_x;																// 父节点左下角 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// 父节点左下角 + 小板宽度
				pRight->m_RealLength	= pParentNode->m_RealLength;													// 父节点长度
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// 父节点宽度 - 小板宽度 - 锯缝
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;
		}


		// 父节点一分为二, 父节点类型从余料变为组合板
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}
		// 竖切一刀，右节点再一分为二，变为需要的小板和余料

		Component* pSecondRight = new Component;
		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// 左下角

			// 设置小板,在小板链表中删除该小板
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// 父节点的x
			pPlaceCpn->m_y = pLeft->m_y;		// 父节点的y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// 另一块余料
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// 父节点x + 小板宽度 + 锯缝
			pSecondRight->m_y			= pLeft->m_y;													// 父节点y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// 父节点宽度 - 小板宽度 - 锯缝
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			
			break;
		case LayoutOrg_LeftTop:			// 左上角

			// 设置小板,在小板链表中删除该小板
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// 父节点x
			pPlaceCpn->m_y = pLeft->m_y;		// 父节点y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// 另一块余料
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// 父节点x + 小板宽度 + 锯缝
			pSecondRight->m_y			= pLeft->m_y;													// 父节点y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// 父节点宽度 - 小板宽度 - 锯缝
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// 右下角

			// 设置小板,在小板链表中删除该小板
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;	// 父节点x + 父节点长度 - 小板长度
			pPlaceCpn->m_y = pLeft->m_y;													// 父节点y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// 另一块余料
			pSecondRight->m_x			= pLeft->m_x;													// 父节点x 
			pSecondRight->m_y			= pLeft->m_y;													// 父节点y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// 父节点宽度 - 小板宽度 - 锯缝
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		case LayoutOrg_RightTop:		// 右上角

			// 设置小板,在小板链表中删除该小板
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;		// 父节点x + 父节点长度 - 小板长度
			pPlaceCpn->m_y = pLeft->m_y;		// 父节点y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// 另一块余料
			pSecondRight->m_x			= pLeft->m_x;													// 父节点x 
			pSecondRight->m_y			= pLeft->m_y;													// 父节点y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// 父节点宽度 - 小板宽度 - 锯缝
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			

			break;
		default:						// 默认左下角

			// 设置小板,在小板链表中删除该小板
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// 父节点的x
			pPlaceCpn->m_y = pLeft->m_y;		// 父节点的y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// 另一块余料
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// 父节点x + 小板宽度 + 锯缝
			pSecondRight->m_y			= pLeft->m_y;													// 父节点y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// 父节点长度 - 小板长度 - 锯缝
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// 父节点宽度 - 小板宽度 - 锯缝
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			

			break;
		}


		

		// 左节点添加子节点，类型变为组合板
		pLeft->m_type = NodeType_CombinePanel;
		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}
	}

	return true;
}



/*--------------------------------------------------------------------------------------*/
//	purpose:
//		初始化余料线链表
//
//	param:
//		
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::BuildRemainderList(Panel* pParent, vector<Component*>& RemainderList)
{
	// 清空余料线链表
	RemainderList.clear();

	// 获取余料，创建轮廓线
	vector<Node*> list;
	pParent->GetAllLeafNodes(list, NodeType_Remainder);	

	int i, nCount;
	nCount = list.size();

	// Node*转为Component*
	for(i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(list[i]);
		RemainderList.push_back(pNode);
	}

	return 0;
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

bool ALGORITHM_API::MatchSuitableComponentNRemaider(vector<Component*>& RemainderList, vector<Component*>& ComponentList, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder, BaseInfo& base_info)
{
	int i_remainder, nRemainderNum = RemainderList.size();
	int i_cpn, nLeftCpnSize = ComponentList.size();

	bool bMatchFlag = false;
	bool bRotateFlag = false;
	float min_remain_length, min_remain_width, min_dist;

	if (base_info.m_bDownerFaceFirst == true)
	{

		for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
		{
			Component* pRmd = RemainderList.at(i_remainder);

			for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
			{
				Component* pCpn = ComponentList.at(i_cpn);

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

						// 遇到更匹配的 并且不是最佳有反面信息且当前板件无反面信息
						if (	(remaim_length < min_dist || remain_width < min_dist) 
							&& !(pPerfectMatchComponent->HaveDownerFaceInfo() ==true && pCpn->HaveDownerFaceInfo() == false))
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

							// 遇到更匹配的 并且不是最佳有反面信息且当前板件无反面信息
							if (	(remaim_length < min_dist || remain_width < min_dist) 
								&& !(pPerfectMatchComponent->HaveDownerFaceInfo() ==true && pCpn->HaveDownerFaceInfo() == false))
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
	}
	else
	{

		for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
		{
			Component* pRmd = RemainderList.at(i_remainder);

			for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
			{
				Component* pCpn = ComponentList.at(i_cpn);

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
	}


	return bMatchFlag;

}


// 判断谁比较小
bool ALGORITHM_API::ComponentCompareSmaller(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) < (*p2);	
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		从大到小板件排序，长度优先，面积次之
//	
//	param:
//		void
//
//	return:
//		0 -- 成功
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::SortRemainderList_LengthFirst(vector<Component*>& RemainderList)
{
	//  板件从大到小排序
	sort(RemainderList.begin(), RemainderList.end(), ComponentCompareSmaller);

	return 0;
}







/*******************************************************************************************/

// 拷贝小板链表
void ALGORITHM_API::CopyComponentList(vector<Component*>& SrcList, vector<Component*>& DstList)
{
	int i_cpn, nComponentCount;

	DstList.clear();
	nComponentCount = SrcList.size();
	for(i_cpn = 0; i_cpn < nComponentCount; i_cpn++)
	{
		Component* pCpn = SrcList.at(i_cpn);
		Component* pCopy = static_cast<Component*>(pCpn->CopyNode());


		DstList.push_back(pCopy);
	}
}

// 判断谁比较小
bool ComponentCompareArea(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return p1->IsAreaLargeThan(*p2);	
}


/*---------------------------------------*/
//	函数说明：
//		面积大优先遍历树形结构
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
void ALGORITHM_API::TravelNeedComponent_AreaLargeFirst(Component* pCpn, vector<Component*>& cpn_list)
{
	int i, nCount = pCpn->m_ChildrenList.size();

	// 先根遍历 从叶子加点开始遍历
	if (pCpn->m_type == NodeType_NeededComponent)
	{
		cpn_list.push_back(pCpn);
	}

	// 收集子节点
	vector<Component*> tmp_list;

	for(i = 0; i < nCount; i++)
	{
		Component* pTmpCpn = static_cast<Component*>(pCpn->m_ChildrenList.at(i));
		tmp_list.push_back(pTmpCpn);
	}

	// 并按面积排序
	sort(tmp_list.begin(), tmp_list.end(), ComponentCompareArea);


	for(i = 0; i < nCount; i++)
	{
		Component* pNode = tmp_list.at(i);
		TravelNeedComponent_AreaLargeFirst(pNode, cpn_list);
	}

	

	return ;
}

/*---------------------------------------------------*/
/*				数学相关计算接口					 */
/*---------------------------------------------------*/

/*---------------------------------------*/
//	函数说明：
//		获取两PointInfo点之间距离
//
//
//	参数：
//
//
//
//
//	返回值:
//		距离
//
/*---------------------------------------*/
float ALGORITHM_API::GetPointsDistance(PointInfo p1, PointInfo p2)
{
	float dx, dy, dis_point;
	dx = p1.x - p2.x;
	dy = p1.y - p2.y;

	// 求两点距离
	dis_point = sqrt(dx*dx + dy*dy);

	return dis_point;
}


#define  COINCIDENT_DIST		(0.2)		// 重合距离判断

// 过滤重合的点
vector<PointInfo> ALGORITHM_API::FiltrateCoincidentPoints(vector<PointInfo>& oldpoint)
{
	// 过滤重合点
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

	for (int i = 0; i < old_point_size; i++)
	{
		// 选点
		PointInfo cur_point, next_point;
		cur_point = oldpoint.at(i);
		next_point = oldpoint.at((i + 1) % old_point_size);

		// 取值
		float cur_x, cur_y, next_x, next_y, dx, dy;
		dx = cur_point.x - next_point.x;
		dy = cur_point.y - next_point.y;

		// 求两点距离
		float dis_point = sqrt(dx*dx + dy*dy);

		// 当前点与下一点有重合点
		if (dis_point < COINCIDENT_DIST)
		{
			//2016 yuanzb  最后一点与第一点重合，且最后一点不是圆弧终点，删掉
			if (i == old_point_size-1)
			{
				if (cur_point.sign == 2 || cur_point.sign == 3)	// 圆弧终点，添加
				{
					filtrationpoint.push_back(cur_point);
				}

				continue;
			}

			// 需要合并的
			PointInfo new_one;
			new_one.x = cur_point.x;
			new_one.y = cur_point.y;
			if (cur_point.sign  == 2 && next_point.sign ==1)			// 圆弧尾点连圆弧起点，改为中间点
			{
				new_one = cur_point;
				new_one.sign = 3;	// 圆弧连接点
			}
			else if (cur_point.sign == 1 && next_point.sign == 2)	// 圆弧起尾点重合，删除两点，换成直线点
			{
				new_one = cur_point;
				new_one.sign = 0;
				new_one.r = 0;
			}
			else if (cur_point.sign == 2 && next_point.sign == 0)	// 圆弧尾点接直线，删除直线点
			{
				new_one = cur_point;
			}
			else if (cur_point.sign == 0 && next_point.sign == 1)	// 直线接圆弧起点
			{
				new_one = next_point;
			}
			else													// 直线接直线，直接删了第二个点
			{
				new_one = cur_point;
			}

			// 判断是否已添加了相同坐标的点
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, new_one);

				// 重合
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && new_one.sign == 1)	// 直线和圆弧起点重合
					{
						// 删除最后一个点
						filtrationpoint.pop_back();
						filtrationpoint.push_back(new_one);
					}
				}
				else
				{
					// 不重合，添加
					filtrationpoint.push_back(new_one);
					i++;
				}

			}
			else
			{
				// 不重合，添加
				filtrationpoint.push_back(new_one);
				i++;
			}
		}
		else
		{
			// 判断是否已添加了相同坐标的点
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

				// 重合
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && cur_point.sign == 1)	// 直线和圆弧起点重合
					{
						// 删除最后一个点
						filtrationpoint.pop_back();
						filtrationpoint.push_back(cur_point);
					}
				}
				else
				{
					// 不重合，添加
					filtrationpoint.push_back(cur_point);
				}
			}
			else
			{
				filtrationpoint.push_back(cur_point);
			}
		}
	}

	// 最后检查一下最后一个点是否为直线且与第一个点重合,异形点数量一定大于等于3，否则必定异形数据错误
	int nCount;
	float dist;
	PointInfo first_pnt, last_pnt;

	nCount = filtrationpoint.size();
	first_pnt = filtrationpoint.at(0);
	last_pnt = filtrationpoint.at(nCount-1);

	dist = ALGORITHM_API::GetPointsDistance(first_pnt, last_pnt);
	if (dist < COINCIDENT_DIST)
	{
		if (last_pnt.sign == 0)
		{
			filtrationpoint.pop_back();
		}
	}

	return filtrationpoint;
}


// 过滤重合的点,保证图形闭合最后一个点是直线也要添加，
vector<PointInfo> ALGORITHM_API::FiltrateCoincidentPoints2(vector<PointInfo>& oldpoint)
{
	// 过滤重合点
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

	for (int i = 0; i < old_point_size; i++)
	{
		// 选点
		PointInfo cur_point, next_point;
		cur_point = oldpoint.at(i);
		next_point = oldpoint.at((i + 1) % old_point_size);

		// 取值
		float cur_x, cur_y, next_x, next_y, dx, dy;
		dx = cur_point.x - next_point.x;
		dy = cur_point.y - next_point.y;

		// 求两点距离
		float dis_point = sqrt(dx*dx + dy*dy);

		// 当前点与下一点有重合点
		if (dis_point < COINCIDENT_DIST)
		{
			//2016 yuanzb  最后一点直接添加
			if (i == old_point_size-1)
			{
				// 判断是否已添加了相同坐标的点
				int nNewCount = filtrationpoint.size();
				if (nNewCount > 0)
				{
					float d;
					PointInfo prev_new_one;

					prev_new_one = filtrationpoint.at(nNewCount-1);
					d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

					// 重合
					if (d < COINCIDENT_DIST)
					{
						// 不添加, 跳过
						continue;
					}
					else
					{
						// 不重合，添加

						// 最后一个点为圆弧起点是，直接改为直线点
						if (cur_point.sign == 1)
						{
							cur_point.sign = 0;
						}


						filtrationpoint.push_back(cur_point);
						continue;
					}
				}
				else
				{
					filtrationpoint.push_back(cur_point);
					continue;
				}
			}

			// 需要合并的
			PointInfo new_one;
  			new_one.x = cur_point.x;
			new_one.y = cur_point.y;
			if (cur_point.sign  == 2 && next_point.sign ==1)			// 圆弧尾点连圆弧起点，改为中间点
			{
				new_one = cur_point;
				new_one.sign = 3;	// 圆弧连接点
			}
			else if (cur_point.sign == 1 && next_point.sign == 2)	// 圆弧起尾点重合，删除两点，换成直线点
			{
				new_one = cur_point;
				new_one.sign = 0; 
				new_one.r = 0;
			}
			else if (cur_point.sign == 3 && next_point.sign == 2)	// 圆弧连接点与终点重合，保留尾点
			{
				new_one = next_point;

				// 出现尾点半径为0的情况，以连接点的半径为准
				if (cur_point.r > next_point.r)
				{
					new_one.r = cur_point.r;
				}

			}
			else if (cur_point.sign == 2 && next_point.sign == 0)	// 圆弧尾点接直线，删除直线点
			{
				new_one = cur_point;
			}
			else if (cur_point.sign == 0 && next_point.sign == 1)	// 直线接圆弧起点 
			{
				new_one = next_point;
			}
			else if (cur_point.sign == 1 && next_point.sign == 3)	// 圆弧起点与连接点重合，删除连接点
			{
				new_one = cur_point;
			}
			else													// 直线接直线，直接删了第二个点
			{
				new_one = cur_point;
			}



			// 判断是否已添加了相同坐标的点
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, new_one);

				// 重合
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && new_one.sign == 1)	// 直线和圆弧起点重合
					{
						// 删除最后一个点
						filtrationpoint.pop_back();
						filtrationpoint.push_back(new_one);
					}
				}
				else
				{
					// 不重合，添加
					filtrationpoint.push_back(new_one);
					//i++;
				}

			}
			else
			{
				// 不重合，添加
				filtrationpoint.push_back(new_one);
				//i++;
			}
		}
		else
		{
			// 判断是否已添加了相同坐标的点
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

				// 重合
				if (d < COINCIDENT_DIST)
				{
					// 不添加
				}
				else
				{
					// 不重合，添加
					filtrationpoint.push_back(cur_point);
				}
			}
			else
			{
				filtrationpoint.push_back(cur_point);
			}
		}
	}

	return filtrationpoint;
}

/*---------------------------------------*/
//	函数说明：
//			旋转点
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
void ALGORITHM_API::rotatepoint(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x - x0;
	float mid_y = y - y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x + x0;
	y = rotate_y + y0; 
}




/*---------------------------------------*/
//	函数说明：
//		获取圆心
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
void ALGORITHM_API::getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float len_beginToend = sqrt((beginx - endx)*(beginx - endx) + (beginy - endy)*(beginy - endy));
	float dis_x = (beginx - endx)/len_beginToend*radius;
	float dis_y = (beginy - endy)/len_beginToend*radius;

	float mid_x = endx + dis_x;
	float mid_y = endy + dis_y;

	float mid_x_move = mid_x - endx;
	float mid_y_move = mid_y - endy;

	float arc = (3.1415926-2*asin((0.5*len_beginToend)/radius))/2;

	center_x = mid_x_move*cos(arc) - mid_y_move*sin(arc);
	center_y = mid_x_move*sin(arc) + mid_y_move*cos(arc);

	center_x = center_x + endx;
	center_y = center_y + endy;
}

/*---------------------------------------*/
//	函数说明：
//		两点取圆心,改进了getCircleCenter的错误
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
void ALGORITHM_API::GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float dx = beginx - endx;
	float dy = beginy - endy;
	float chord_len = sqrt(dx*dx + dy*dy);
	float sita = acos(0.5*chord_len/radius);	// 角度
	float new_x, new_y;

	// 顺时针旋转，指向圆心的向量， 若大于180度时需要逆时针旋转
	if (sign == 1)
		ClockwiseRotate(dx, dy, new_x, new_y, sita);
	else
		CouterClockwiseRotate(dx, dy, new_x, new_y, sita);

	// 向量的规范化
	float normal_new_x, normal_new_y;
	NormalVector(new_x, new_y, normal_new_x, normal_new_y);

	// 尾点指向圆心的向量，准确值
	float radius_x, radius_y;
	radius_x = normal_new_x * radius;
	radius_y = normal_new_y * radius;

	// 尾点+指向圆心向量， 求得圆心坐标。
	center_x = endx + radius_x;
	center_y = endy + radius_y;

}

/*---------------------------------------*/
//	函数说明：
//		逆时针旋转
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
void ALGORITHM_API::CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

/*---------------------------------------*/
//	函数说明：
//		顺时针旋转
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
void ALGORITHM_API::ClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) + mid_y*sin(arc);
	float rotate_y = -mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

/*---------------------------------------*/
//	函数说明：
//		向量规范化
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
void ALGORITHM_API::NormalVector(float x0,float y0,float & x,float & y)
{
	float len = sqrt(x0*x0 + y0*y0);

	x = x0/len;
	y = y0/len;

}

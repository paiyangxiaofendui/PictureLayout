/*--------------------------------------------------------------------------------------------------------------------*/
//	Complex.cpp   -- 复合图形类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.7.12
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Complex.h"
#include "../CommonData.h"
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"
#include "PolyLine.h"
#include "Arc.h"
#include "../Layer/Layer.h"



Cad_Complex::Cad_Complex()
{
	m_FigureType = FigureType_Complex;
}


Cad_Complex::~Cad_Complex()
{

}






/*-----------------------------------------------------------------------*/
//	purpose:
//		计算平行的图形
//		根据基本图形链表，依次平行，
//		1、若基本图形有相交，根据相交点修改前后基本图形的尾点和起点
//		2、若不相交，则用圆弧插补点连接基本图形
//		
//
//	param：
//		int dir -- 方向
//		float dist -- 距离
//
//	return:
//		vector<ShapePoint*> -- 平行后的形状点
/*-----------------------------------------------------------------------*/
Figure* Cad_Complex::CalParallelFigure(int dir, float dist)
{
	BOOL ClosedFlag = FALSE;			// 图形闭合
	BOOL InterFlag = FALSE;
	BOOL IntersectWithPrevFig = FALSE;
	FPoint inter_pnt;
	Figure* pCurFig, *pCurParallelFig;
	Figure* pNextFig, *pNextParallelFig;
	ShapePoint* pCurFigEndPnt, *pCurParallelFigStartPnt, *pCurParallelFigEndPnt, *pNextParallelFigStartPnt;
	Layer ParallelLayer;

	// 判断是否闭合
	ClosedFlag = IsClosed();

	// 求出基本图形链表
	CalComplexFigureList();

	// 依次平行
	int i, nCount;
	nCount = m_ComplexFigureList.size();

	if (nCount == 1)			// 只有一个基本图形	直接求平行图形即可
	{
		pCurFig = m_ComplexFigureList.at(0);

		// 平行
		pCurParallelFig = pCurFig->CalParallelFigure(dir, dist);

		return pCurParallelFig;
	}
	else					// 多个基本图形		需平行，相交，再连接
	{

		for (i = 0; i < nCount; i++)
		{
			if (i == nCount-1)	// 最后一个图形
			{
				pCurFig = m_ComplexFigureList.at(i);
				pCurParallelFig = pCurFig->CalParallelFigure(dir, dist);

				// 添加当前平行图形
				ParallelLayer.AddFigure(pCurParallelFig);

				// 这里需要判断是否与上一图形相交，并处理当前平行图形的起点
				pCurParallelFigStartPnt = pCurParallelFig->GetHeadShapePoint();
				if (IntersectWithPrevFig == TRUE)
				{
					pCurParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
				}

				if(ClosedFlag == TRUE)	// 闭合，需要判断与第一个图形是否相交
				{
					pNextParallelFig = ParallelLayer.GetFigure(0);	// 直接取第一个平行图形


					// 相交
					InterFlag = GeneralInterface::CalFigureIntersect(pCurParallelFig, pNextParallelFig, inter_pnt);
					if (InterFlag == TRUE)
					{
						// 相交则修改平行图形的形状点

						pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	起点
						pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	终点

						pCurParallelFigEndPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
						pNextParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);

						// 设置与上一图形相交标志，与相交点
						IntersectWithPrevFig = TRUE;
					}
					else
					{
						// 设为不相交
						IntersectWithPrevFig = FALSE;

						// 不相交则用圆弧插补
						pCurFigEndPnt = pCurFig->GetTailShapePoint();						//	当前图形尾点 == 圆心
						pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	当前平行图形尾点 == 圆弧起点
						pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	下一平行图形首点 == 圆弧终点

						Cad_PolyLine* pLinkArc = new Cad_PolyLine;
						pLinkArc->m_ShapePointList = GeneralInterface::ArcInterpolate(pCurFigEndPnt, pCurParallelFigEndPnt, pNextParallelFigStartPnt, dist);

						// 更新图形信息
						pLinkArc->UpdateShapePointInfo();
						ParallelLayer.AddFigure(pLinkArc);
					}
				}
// 				else					// 不闭合，直接平行添加
// 				{
// 					ParallelLayer.AddFigure(pCurParallelFig);
// 				}

			}
			else
			{
				pCurFig = m_ComplexFigureList.at(i);
				pNextFig = m_ComplexFigureList.at(i+1);

				// 平行
				pCurParallelFig = pCurFig->CalParallelFigure(dir, dist);
				pNextParallelFig = pNextFig->CalParallelFigure(dir, dist);

				// 添加当前平行图形
				ParallelLayer.AddFigure(pCurParallelFig);

				// 这里需要判断是否与上一图形相交，并处理当前平行图形的起点
				pCurParallelFigStartPnt = pCurParallelFig->GetHeadShapePoint();
				if (IntersectWithPrevFig == TRUE)
				{
					pCurParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
				}

				// 相交
				InterFlag = GeneralInterface::CalFigureIntersect(pCurParallelFig, pNextParallelFig, inter_pnt);
				if (InterFlag == TRUE)
				{
					// 相交则修改平行图形的形状点

					pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	起点
					pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	终点

					pCurParallelFigEndPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);
					pNextParallelFigStartPnt->SetXY(inter_pnt.m_x, inter_pnt.m_y);

					// 设置与上一图形相交标志，与相交点
					IntersectWithPrevFig = TRUE;

				}
				else
				{
					// 设为不相交
					IntersectWithPrevFig = FALSE;

					// 不相交则用圆弧插补
					pCurFigEndPnt = pCurFig->GetTailShapePoint();						//	当前图形尾点 == 圆心
					pCurParallelFigEndPnt = pCurParallelFig->GetTailShapePoint();		//	当前平行图形尾点 == 圆弧起点
					pNextParallelFigStartPnt = pNextParallelFig->GetHeadShapePoint();	//	下一平行图形首点 == 圆弧终点

					// 添加连接弧
					Cad_PolyLine* pLinkArc = new Cad_PolyLine;
					pLinkArc->m_ShapePointList = GeneralInterface::ArcInterpolate(pCurFigEndPnt, pCurParallelFigEndPnt, pNextParallelFigStartPnt, dist);

					// 更新图形信息
					pLinkArc->UpdateShapePointInfo();
					ParallelLayer.AddFigure(pLinkArc);
				}


				//ParallelLayer.AddFigure(pCurParallelFig);
			}
		}
	}

	// 先连接图形,在计算图形相关信息
	ParallelLayer.LinkFigureList();

	Figure* pFinalFig = ParallelLayer.GetFigure(0);
	pFinalFig->CalFigureRect();
	pFinalFig->UpdateShapePointInfo();
	pFinalFig->UpdateOutlineInfo();
	

	return pFinalFig;
}



/*----------------------------------------------------------*/
//	purpose:
//		反向形状点链表，因为复合图形的原因，会把形状点的属性做一定的调整
//		先调转，再把最后的插到最前面
//		
//	param:
//		void
//
//	return:
//		void
/*----------------------------------------------------------*/
void Cad_Complex::ReverseShapePointList(void)
{
	// 形状点的图形属性，先调转，再把最后的插到最前面
 	int i, nCount;
 	ShapePoint* pPnt = NULL;
	vector<int> type_vector;
	vector<int>::iterator type_it;

	nCount = GetShapePointNum();
	for (i = nCount-1; i >=0; i--)
	{
		pPnt = GetShapePoint(i);
		type_vector.push_back(pPnt->m_FigureType);
	}
	
	// 将最后一个图形类型插到最前面
	int end_type = type_vector.back();
	type_vector.pop_back();
	type_vector.insert(type_vector.begin(), end_type);
	
	GeneralInterface::ReverseShapePointList(&m_ShapePointList);

	for (i = 0; i < nCount; i++)
	{
		pPnt = GetShapePoint(i);
		pPnt->m_FigureType = type_vector[i];
	}

	UpdateShapePointInfo();
}


// 更新所有形状点信息
void Cad_Complex::UpdateShapePointInfo(void)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;

	nCount = m_ShapePointList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_ShapePointList.begin() + i;
		(*it)->m_PntID = i;
		(*it)->m_pFigure = this;
	}
}



/*----------------------------------------------------------*/
//	purpose:
//		计算形状点轮廓线信息
//		1、先计算出图形链表，各自完善的形状点
//		2、对所有形状点，按照异形录入手册的规则进行处理，
//		3、把相应的形状点信息赋值给复合图形的形状点链表
//
//	param:
//		void
//
//	return:
//		void
/*----------------------------------------------------------*/
void Cad_Complex::CalShapePointOutlineInfo(void)
{
	int i,j, n, nCount;
	int nLastDelIndex = -1;								//yuanzb 2016-9-29 因为图形间的连接弧可能距离很近导致判断重合，删多了会导致计算点不匹配，所以添加变量最后一个删除的索引，避免多次删除图形间的连接点导致数量不匹配  
	int nComplexFigNum, nTmpListTotalNum, nTmpListNum;	// 复合图形数量，tmplist形状点数量总和，tmplist删除针对直线接圆弧、圆弧接圆弧、圆弧接直线，重合形状点后的数量
	Figure* pFig = NULL;
	ShapePoint* pPnt = NULL;
	ShapePoint* pSrcPnt = NULL;
	vector<ShapePoint*> tmp_list;

	ShapePoint* pCurPnt = NULL;
	ShapePoint* pNextPnt = NULL;

	// 1、计算图形链表
	CalComplexFigureList();

	nComplexFigNum = m_ComplexFigureList.size();
	for (i = 0; i < nComplexFigNum; i++)
	{
		pFig = m_ComplexFigureList[i];
		pFig->CalShapePointOutlineInfo();

		n = pFig->GetShapePointNum();
		for (j = 0; j < n; j++)
		{
			pPnt = new ShapePoint;
			pSrcPnt = pFig->GetShapePoint(j);

			*pPnt = *pSrcPnt;

			tmp_list.push_back(pPnt);
		}
	}

	// 2、按照异形录入规则处理所有形状点
	vector<ShapePoint*>::iterator del_it;
	nTmpListTotalNum = nTmpListNum = tmp_list.size();
	for (i = 0; i < nTmpListNum-2; i++)
	{
		j = i+1;
		pCurPnt = tmp_list[i];
		pNextPnt = tmp_list[j];

		if (pCurPnt->IsCloseTo(pNextPnt) == TRUE && nLastDelIndex != i)
		{
			if (pCurPnt->m_FigureType == FigureType_PolyLine && pNextPnt->m_FigureType == FigureType_Arc)
			{
				// 记录上一次删除的索引
				nLastDelIndex = i;	

				// 直线接圆弧，删除直线点
				del_it = tmp_list.begin() + i;

				tmp_list.erase(del_it);
				i--;
				j--;
				nTmpListNum = tmp_list.size();
			}
			else if (pCurPnt->m_FigureType == FigureType_Arc && pNextPnt->m_FigureType == FigureType_PolyLine)
			{
				// 记录上一次删除的索引
				nLastDelIndex = i;	

				// 圆弧接直线，删除直线点
				del_it = tmp_list.begin() + j;

				tmp_list.erase(del_it);
				i--;
				j--;
				nTmpListNum = tmp_list.size();
			}
			else if (pCurPnt->m_FigureType == FigureType_Arc && pNextPnt->m_FigureType == FigureType_Arc)
			{
				// 记录上一次删除的索引
				nLastDelIndex = i;	

				// 圆弧接圆弧，删除前一点，后一点sign改为3
				//pNextPnt->m_sign = Sign_ArcLink;

				//del_it = tmp_list.begin() + i;

				pCurPnt->m_sign = Sign_ArcLink;
				del_it = tmp_list.begin() + j;


				tmp_list.erase(del_it);
				i--;
				j--;
				nTmpListNum = tmp_list.size();
			}
		}
	}

	// 3、按顺序赋值给复合图形的形状点链表
	nCount = GetShapePointNum();
	n = tmp_list.size();

	// 判断形状点数量是否匹配
	if (n != nCount)
	{
		if(nCount != nTmpListTotalNum-nComplexFigNum+1)
		{
			AfxMessageBox("复合图形形状点信息计算不匹配！");
		}
		else
		{
			AfxMessageBox("删除了部分重合点！");
		}
	}


	// 头尾形状点对group赋值
	tmp_list[0]->m_group = Group_Start;
	tmp_list[nCount-1]->m_group = Group_End;

	for (i = 0; i < nCount; i++)
	{
		pSrcPnt = GetShapePoint(i);
		pPnt = tmp_list[i];

		pSrcPnt->m_radius	= pPnt->m_radius;
		pSrcPnt->m_sign		= pPnt->m_sign;		
		pSrcPnt->m_dir		= pPnt->m_dir;		
		pSrcPnt->m_cut		= pPnt->m_cut;		
		pSrcPnt->m_type		= pPnt->m_type;		
		pSrcPnt->m_group	= pPnt->m_group;	
		pSrcPnt->m_side		= pPnt->m_side;	
	}


}




// 清空复合图形链表
void Cad_Complex::ClearComplexFigureList()
{
	GeneralInterface::ClearFigureList(&m_ComplexFigureList);
}

// 计算图形链表
void Cad_Complex::CalComplexFigureList(void)
{
	int i, nArcNum, nCount, start_id, end_id;
	int cur_type, next_type;
	ShapePoint *pNext;
	Figure* pFig = NULL;

	// 清空源链表
	ClearComplexFigureList();

	nCount = GetShapePointNum();
	if (nCount < 2)
		return ;
	else if (nCount = 2)
	{
		start_id = 0;
		end_id = 1;
	}

	start_id = 0;
	pNext = GetShapePoint(1);
	cur_type = pNext->m_FigureType;

	// 中间的图形
	nCount = GetShapePointNum();
	for (i = 2; i < nCount; i++)
	{
		end_id = i-1;
		pNext = GetShapePoint(i);
		next_type = pNext->m_FigureType;

		if (cur_type != next_type)	// 图形类型不同，构造一个图形
		{
			pFig = GetFigureFromShapePoint(start_id, end_id, cur_type);
			pFig->CalOutlineList();
			AddFigure(pFig);

			cur_type = next_type;
			start_id = i-1;
			end_id = i;
		}
		else if (cur_type == FigureType_Arc)
		{
			// 圆弧够三个点就可以添加了

			nArcNum = end_id - start_id;
			if (nArcNum == 2)
			{
				pFig = GetFigureFromShapePoint(start_id, end_id, cur_type);
				pFig->CalOutlineList();
				AddFigure(pFig);

				cur_type = next_type;
				start_id = i-1;
				end_id = i;
			}
			else
				end_id++;
		}
		else
		{
			end_id++;
		}
	}

	// 最后一个图形
	pFig = GetFigureFromShapePoint(start_id, end_id, cur_type);
	pFig->CalOutlineList();
	AddFigure(pFig);

}


// 获取图形
Figure* Cad_Complex::GetFigureFromShapePoint(int start_id, int end_id, int type)
{
	int i, nShpPntCount;
	Figure* pFig = NULL;
	Cad_Arc* pArc = NULL;
	ShapePoint* pPnt = NULL;
	ShapePoint* pNewPnt = NULL;

	nShpPntCount = end_id - start_id + 1;

	// 新建图形
	switch(type)
	{
	case FigureType_PolyLine:
		pFig = new Cad_PolyLine;
		break;
	case FigureType_Arc:
		pFig = new Cad_Arc;
		break;
	default:
		break;
	}

	// 添加形状点
	for (i = start_id; i <= end_id; i++ )
	{
		pNewPnt = new ShapePoint;
		pPnt = GetShapePoint(i);

		*pNewPnt = *pPnt;
		pFig->AddShapePoint(pNewPnt);
	}

	// 更新一些必要信息
	switch(type)
	{
	case FigureType_Arc:
		pArc = dynamic_cast<Cad_Arc*>(pFig);

		if (pArc == NULL)
		{
			AfxMessageBox("动态转换失败！");
			pArc = static_cast<Cad_Arc*>(pFig);
		}


		pArc->CalCenterRadiusInfo();
		break;
	default:
		break;
	}


	return pFig;
}

// 添加一个图形
int Cad_Complex::AddFigure(Figure* pFigure)	
{
	if (pFigure != NULL)
	{
		m_ComplexFigureList.push_back(pFigure);
		UpdateFigureInfo();
		return 0;
	}

	return -1;
}


// 更新所有图形信息
void Cad_Complex::UpdateFigureInfo(void)
{
	int i, nCount;
	vector<Figure*>::iterator it;

	nCount = m_ComplexFigureList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_ComplexFigureList.begin() + i;
		(*it)->m_FigureID = i;
		(*it)->m_pLayer = NULL;
	}
}

// 计算轮廓线	
void Cad_Complex::CalOutlineList(void)
{
	// 先清空
	ClearOutlineList();

	// 计算图形链表，
	CalComplexFigureList();

	// 连接各个图形的轮廓线
	int i, j, nFigCount, nOutlineCount;
	Figure* pFig = NULL;

	nFigCount = m_ComplexFigureList.size();
	for (i = 0; i < nFigCount; i++)
	{
		pFig = m_ComplexFigureList[i];

		nOutlineCount = pFig->GetOutlineNum();
		for (j = 0; j < nOutlineCount; j++)
		{
			FPoint* pPnt = new FPoint;
			*pPnt = *(pFig->GetOutline(j));
			AddOutline(pPnt);
		}
	}

}
/*--------------------------------------------------------------------------------------------------------------------*/
//	GeneralInterface.cpp    -- 公共借口类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.7.12
//	备注：	数据操作公共接口
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "GeneralInterface.h"
#include "../DataManager/Layer/Layer.h"
#include "../DataManager/Figure/Arc.h"
#include "../DataManager/Figure/Complex.h"
#include "../DataManager/Point/ShapePoint.h"
#include "../DataManager/CommonData.h"




/*--------------------------------------------------------------------------------------*/
/*                                 单例类相关操作										*/
/*--------------------------------------------------------------------------------------*/
// 清空图形链表
void GeneralInterface::ClearLayerList(vector<Layer*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<Layer*>::iterator it;

		// 释放内存
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			Layer* pLayer = *it;

			delete pLayer;
			pLayer = NULL;
		}

		// 清空容器
		pList->clear();
	}
}


/*--------------------------------------------------------------------------------------*/
/*                                 图形链表相关操作                                     */
/*--------------------------------------------------------------------------------------*/


// 平行图形链表
BOOL GeneralInterface::ParallelFigureList(vector<Figure*>* pSrcList, vector<Figure*>* pDstList, int dir, float dist)
{
	int i, nCount;

	nCount = pSrcList->size();
	for (i = 0; i < nCount; i++)
	{
		Figure* pFig = pSrcList->at(i);
		Figure* pParallelFig = pFig->CalParallelFigure(dir, dist);

		if (pParallelFig != NULL)
			pDstList->push_back(pParallelFig);
	}

	return TRUE;
}



// 清空图形链表
void GeneralInterface::ClearFigureList(vector<Figure*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<Figure*>::iterator it;

		// 释放内存
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			Figure* pFig = *it;

			delete pFig;
			pFig = NULL;
		}

		// 清空容器
		pList->clear();
	}
}


// 图形链表排序 -- 按矩形大小
void GeneralInterface::SortFigureListByRect(vector<Figure*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		CRect pre_rect, cur_rect;
		vector<Figure*>::iterator it;
		vector<Figure*> tmp_list;

		// 释放内存
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			Figure* pFig = *it;
			Figure* pHeadFig = NULL;

			pFig->CalFigureRect();

			if (i == 0)
			{
				pre_rect = pFig->m_FigureRect;
				pre_rect.NormalizeRect();

				tmp_list.push_back(pFig);
			}
			else
			{
				cur_rect = pFig->m_FigureRect;
				cur_rect.NormalizeRect();

				pHeadFig = tmp_list.at(0);
				pre_rect = pHeadFig->m_FigureRect;

				int cur_width = cur_rect.Width();
				int cut_height = cur_rect.Height();
				int pre_width = pre_rect.Width();
				int pre_height = pre_rect.Height();
				

				// 当前矩形大于之前的矩形，插在前面
				if (cur_width >= pre_width && cut_height >= pre_height)
				{
					tmp_list.insert(tmp_list.begin(), pFig);
				}
				else
				{
					tmp_list.push_back(pFig);
				}
			}
		}

		pList->clear();
		*pList = tmp_list;

	}
}

// 对圆弧插补若干个点
vector<ShapePoint*> GeneralInterface::ArcInterpolate(ShapePoint* center, ShapePoint* start, ShapePoint* end, float r)
{
	float dx, dy, chord_len;
	float center_x, center_y, start_x, start_y, end_x, end_y;	// 原始线段点,平行后的线段点 
	float vec_x,  vec_y;	// 插补时用的向量
	float second_Interpolate_x, second_Interpolate_y;	// 线段连接的圆弧插补点

	ShapePoint* pStartPnt = new ShapePoint;									
	ShapePoint* pEndPnt = new ShapePoint;
	ShapePoint* pSecondInterPnt = new ShapePoint;
	vector<ShapePoint*> InterpolateVector;

	// 变量初始化
	*pStartPnt = *start;
	*pEndPnt = *end;

	start->GetXY(start_x, start_y);
	end->GetXY(end_x, end_y);
	center->GetXY(center_x, center_y);

	// 圆弧弦向量
	dx = end_x - start_x;
	dy = end_y - start_y;

	// 弦长
	chord_len = sqrt(dx*dx + dy*dy);

	// 长短弦进行处理
	if(chord_len < ARC_INTERPOLATE_VALUE)
	{
		/*-------------压入起点------------*/
		InterpolateVector.push_back(pStartPnt);


		if (chord_len > ARC_INTERPOLATE_MIN_VALUE)
		{
			/*-------------弦中点------------*/

			second_Interpolate_x = start_x + dx/2.0;
			second_Interpolate_y = start_y + dy/2.0;

			// 圆心指向弦中点的向量
			vec_x = second_Interpolate_x - center_x;
			vec_y = second_Interpolate_y - center_y;

			// 向量规范化后,乘以半径得到指向圆弧中点的向量
			NormalVector(vec_x, vec_y, vec_x, vec_y);
			vec_x *= r;
			vec_y *= r;

			// 求得圆弧中点坐标
			second_Interpolate_x = center_x + vec_x;
			second_Interpolate_y = center_y + vec_y;

			// 压入中点
			pSecondInterPnt->SetXY(second_Interpolate_x, second_Interpolate_y);
			InterpolateVector.push_back(pSecondInterPnt);
		}

		/*-------------压入尾点------------*/
		InterpolateVector.push_back(pEndPnt);
	}
	else
	{
		// 1、 求圆弧中点
		second_Interpolate_x = start_x + dx/2.0;
		second_Interpolate_y = start_y + dy/2.0;

		// 圆心指向弦中点的向量
		vec_x = second_Interpolate_x - center_x;
		vec_y = second_Interpolate_y - center_y;

		// 向量规范化后,乘以半径得到指向圆弧中点的向量
		NormalVector(vec_x, vec_y, vec_x, vec_y);
		vec_x *= r;
		vec_y *= r;

		// 求得圆弧中点坐标
		second_Interpolate_x = center_x + vec_x;
		second_Interpolate_y = center_y + vec_y;
		pSecondInterPnt->SetXY(second_Interpolate_x, second_Interpolate_y);

		// 2、圆弧求离散点
		Cad_Arc TmpArc;

		TmpArc.AddShapePoint(pStartPnt);
		TmpArc.AddShapePoint(pSecondInterPnt);
		TmpArc.AddShapePoint(pEndPnt);

		TmpArc.m_OutlineSpace = 0.5;
		TmpArc.CalCenterRadiusInfo();
		TmpArc.CalOutlineList();

		// 3、离散点转插补点
		int i, nCount;

		nCount = TmpArc.GetOutlineNum();
		for (i = 0; i < nCount; i++)
		{
			ShapePoint* pTmpShpPnt = new ShapePoint;
			FPoint* pTmpOutline = TmpArc.GetOutline(i);

			pTmpShpPnt->SetXY(pTmpOutline->m_x, pTmpOutline->m_y);
			InterpolateVector.push_back(pTmpShpPnt);
		}
	}

	return InterpolateVector;
}

// 计算两图形是否相交
BOOL GeneralInterface::CalFigureIntersect(Figure* pSrcFig, Figure* pDstFig, FPoint& inter_pnt)
{
	BOOL rt;
	BOOL LineIntersectFlag = TRUE;			// 折线与折线相交，第一次用直线相交
	int i_src, i_dst, nSrcNum, nDstNum;
	FPoint *pSrcCur, *pSrcNext, *pDstCur, *pDstNext;

	if(pSrcFig == NULL || pDstFig == NULL)
		return FALSE;

	// 计算轮廓点，用轮廓点链表进行线段相交测试
	pSrcFig->CalOutlineList();
	pDstFig->CalOutlineList();

	// 判断是否相交
	nSrcNum = pSrcFig->GetOutlineNum();
	nDstNum = pDstFig->GetOutlineNum();

	for(i_src = nSrcNum-2; i_src >= 0; i_src--)
	{
		for (i_dst = 0; i_dst < nDstNum-1; i_dst++)
		{
			pSrcCur = pSrcFig->GetOutline(i_src);
			pSrcNext = pSrcFig->GetOutline(i_src+1);

			pDstCur = pDstFig->GetOutline(i_dst);
			pDstNext = pDstFig->GetOutline(i_dst+1);

			if(pSrcFig->m_FigureType == FigureType_PolyLine && pDstFig->m_FigureType == FigureType_PolyLine && LineIntersectFlag == TRUE)
			{
				LineIntersectFlag = FALSE;	// 只交一次，如果这次不相交，说明很可能出错了，用线段相交

				float flag_angle = Angle2Radian(OBTUSE_ANGLE_VALUE);
				float angle = GeneralInterface::CalVectorAngle(pSrcNext->m_x - pSrcCur->m_x, pSrcNext->m_y - pSrcCur->m_y, pDstNext->m_x - pDstCur->m_x, pDstNext->m_y - pDstCur->m_y);

				if (angle > flag_angle)	 // 向量夹角为钝角，则线段夹角为锐角
				{
					rt = CalSegmenIntersect2(pSrcCur->m_x, pSrcCur->m_y, pSrcNext->m_x, pSrcNext->m_y, pDstCur->m_x, pDstCur->m_y, pDstNext->m_x, pDstNext->m_y, inter_pnt.m_x, inter_pnt.m_y);	// 调函数
				}
				else					// 向量夹角为锐角，则线段夹角为钝角
				{
					rt = CalLineIntersect(pSrcCur->m_x, pSrcCur->m_y, pSrcNext->m_x, pSrcNext->m_y, pDstCur->m_x, pDstCur->m_y, pDstNext->m_x, pDstNext->m_y, inter_pnt.m_x, inter_pnt.m_y);	// 调函数
				}
			}
			else
			{
				rt = CalSegmenIntersect2(pSrcCur->m_x, pSrcCur->m_y, pSrcNext->m_x, pSrcNext->m_y, pDstCur->m_x, pDstCur->m_y, pDstNext->m_x, pDstNext->m_y, inter_pnt.m_x, inter_pnt.m_y);

				//2017-5-23  yuanzb  第一个图形的头交了第二个图形的尾，这种情况即使相交也不算相交,此处求的相交是第一个图形的尾交第二个图形的头部
				FPoint* pDstFigTailPnt = pDstFig->GetTailShapePoint();

				if (rt == TRUE && inter_pnt.IsCloseTo(pDstFigTailPnt) == TRUE)
				{
					rt = FALSE;
				}

			}
			
			
			if (rt == TRUE)
				return TRUE;
		}
	}

	return rt;
}


/*--------------------------------------------------------------------------------------*/
/*                                 形状点链表相关操作                                   */
/*--------------------------------------------------------------------------------------*/


// 移动形状点链表
void GeneralInterface::MoveShapePointList(vector<ShapePoint*>* pList, float rel_x, float rel_y)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;

	nCount = pList->size();
	for (i = 0; i < nCount; i++)
	{
		it = pList->begin() + i;

		(*it)->m_x += rel_x;
		(*it)->m_y += rel_y;
	}
}


// 反向形状点容器
void GeneralInterface::ReverseShapePointList(vector<ShapePoint*>* pList)
{
	int i, nCount;
	vector<ShapePoint*>::iterator it;
	vector<ShapePoint*> tmp_vector;

	nCount = pList->size();
	for (i = nCount-1; i >=0; i--)
	{
		it = pList->begin() + i;
		tmp_vector.push_back(*it);
	}

	pList->clear();
	*pList = tmp_vector;
}

// 清空形状点链表
void GeneralInterface::ClearShapePointList(vector<ShapePoint*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<ShapePoint*>::iterator it;

		// 释放内存
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			ShapePoint* pPnt = *it;

			delete pPnt;
			pPnt = NULL;
		}

		// 清空容器
		pList->clear();
	}
}


// 连接形状点判断
BOOL GeneralInterface::LinkShapePoint_Judge(ShapePoint* pSrcPnt, ShapePoint* pDstPnt, int* pStyle)
{
	Figure* pSrcFig, *pDstFig;
	int src_type, dst_type;


	// 点判断
	if (pSrcPnt == NULL || pDstPnt == NULL || pSrcPnt == pDstPnt || pStyle == NULL)
	{
		return FALSE;
	}

	// 源、目标图形
	pSrcFig = pSrcPnt->m_pFigure;
	pDstFig = pDstPnt->m_pFigure;

	src_type = pSrcFig->m_FigureType;
	dst_type = pDstFig->m_FigureType;

	// 图形判断
	if (pSrcFig == NULL || pDstFig == NULL || pSrcFig == pDstFig)
	{
		return FALSE;
	}

	// 图形类型判断
	if (src_type == FigureType_Circle || dst_type == FigureType_Circle)
	{
		return FALSE;
	}

	// 是否头尾形状点
	if (pSrcPnt != pSrcFig->GetHeadShapePoint() && pSrcPnt != pSrcFig->GetTailShapePoint())
	{
		return FALSE;
	}
	
	// 是否头尾形状点
	if (pDstPnt != pDstFig->GetHeadShapePoint() && pDstPnt != pDstFig->GetTailShapePoint())
	{
		return FALSE;
	}

	// 连接方式
	if (pSrcPnt == pSrcFig->GetHeadShapePoint())
	{
		if (pDstPnt == pDstFig->GetHeadShapePoint())
			*pStyle = Link_Head2Head;
		else
			*pStyle = Link_Head2Tail;
	}
	else
	{
		if (pDstPnt == pDstFig->GetHeadShapePoint())
			*pStyle = Link_Tail2Head;
		else
			*pStyle = Link_Tail2Tail;
	}

	return TRUE;
}

// 图形尾连接图形头
int GeneralInterface::LinkTail2Head(Figure* pSrcFig, Figure* pDstFig)
{
	int i, nSrcCount, nDstCount;
	int nSrcID, nDstID;
	ShapePoint *pTail = NULL, *pHead = NULL;
	ShapePoint* pNewPnt = NULL;
	Layer* pLayer = NULL;

	// 1、基本判断
	if (pSrcFig == NULL || pDstFig == NULL || pSrcFig == pDstFig )
	{
		return -1;
	}

	pLayer = pSrcFig->m_pLayer;
	pTail = pSrcFig->GetTailShapePoint();
	pHead = pDstFig->GetHeadShapePoint();
	nSrcID = pSrcFig->m_FigureID;
	nDstID = pDstFig->m_FigureID;


	// 2、新建复合图形，删除重合点
	nSrcCount = pSrcFig->GetShapePointNum();
	nDstCount = pDstFig->GetShapePointNum();

	// 添加形状点
	Cad_Complex* pComplex = new Cad_Complex;

	// 原图形
	for (i = 0; i < nSrcCount-1; i++)
	{
		pNewPnt = new ShapePoint;

		*pNewPnt = *(pSrcFig->GetShapePoint(i));
		pComplex->AddShapePoint(pNewPnt);
	}

	// 连接点
	pNewPnt = new ShapePoint;

	*pNewPnt = *pTail;
	pComplex->AddShapePoint(pNewPnt);

	if (pTail->IsCloseTo(pHead) == FALSE)
	{
		pNewPnt = new ShapePoint;

		*pNewPnt = *pHead;
		pNewPnt->m_FigureType = pTail->m_FigureType;
		pComplex->AddShapePoint(pNewPnt);
	}

	// 目标图形的形状点
	for (i = 1; i < nDstCount; i++)
	{
		pNewPnt = new ShapePoint;

		*pNewPnt = *(pDstFig->GetShapePoint(i));
		pComplex->AddShapePoint(pNewPnt);
	}

	// 3、删除两个图形，插入图层。

	if (nSrcID > nDstID)
	{
		pLayer->DeleteFigure(nSrcID);
		pLayer->DeleteFigure(nDstID);
	}
	else
	{
		pLayer->DeleteFigure(nDstID);
		pLayer->DeleteFigure(nSrcID);
	}

	pComplex->CalOutlineList();
	pComplex->UpdateShapePointInfo();
	pLayer->AddFigureToHead(pComplex);


	return 0;
}


// 连接图形 
// 返回 0 : 成功 其他：失败
int GeneralInterface::LinkFigure(Figure* pSrcFig, Figure* pDstFig)
{
	int rt;
	int src_type = pSrcFig->m_FigureType;
	int dst_type = pDstFig->m_FigureType;
	ShapePoint* pSrcPnt, * pDstPnt;
	ShapePoint* pSrcHead, * pSrcTail, *pDstHead, *pDstTail;


	// 图形判断
	if (pSrcFig == NULL || pDstFig == NULL || pSrcFig == pDstFig)
	{
		return -1;
	}

	// 图形类型判断
	if (src_type == FigureType_Circle || dst_type == FigureType_Circle)
	{
		return -1;
	}

	// 判断连接方式

	pSrcHead = pSrcFig->GetHeadShapePoint();
	pSrcTail = pSrcFig->GetTailShapePoint();
	pDstHead = pDstFig->GetHeadShapePoint();
	pDstTail = pDstFig->GetTailShapePoint();


	if (pSrcTail->IsCloseTo(pDstHead) == TRUE)		// src尾连dst头
	{
		pSrcPnt = pSrcFig->GetTailShapePoint();
		pDstPnt = pDstFig->GetHeadShapePoint();
	}	
	else if (pSrcHead->IsCloseTo(pDstHead) == TRUE)		// src头连dst头 =  反向后的src尾连dst头
	{
		pSrcFig->ReverseShapePointList();
		pSrcPnt = pSrcFig->GetTailShapePoint();
		pDstPnt = pDstFig->GetHeadShapePoint();
	}
	else if (pSrcHead->IsCloseTo(pDstTail) == TRUE)			// src头连dst尾 = dst尾连src头
	{
		pSrcPnt = pDstFig->GetTailShapePoint();
		pDstPnt = pSrcFig->GetHeadShapePoint();
	}
	else if (pSrcTail->IsCloseTo(pDstTail) == TRUE)		// src尾连dst尾 = src尾连反向后的dst头
	{
		pDstFig->ReverseShapePointList();
		pSrcPnt = pSrcFig->GetTailShapePoint();
		pDstPnt = pDstFig->GetHeadShapePoint();
	}		
	else
		return -1;

	rt = LinkShapePoint(pSrcPnt, pDstPnt);

	return rt;
}

/*-----------------------------------------------------------------------*/
//	purpose:
//		连接形状点，将两个基本图形连接成一个复合图形，目前只允许头尾连接
//		将重合的形状点删掉
//
//	param：
//		ShapePoint* pSrcPnt -- 源形状点
//		ShapePoint* pDstPnt -- 目标形状点
//
//	return:
//		int -- 0 成功
/*-----------------------------------------------------------------------*/
int GeneralInterface::LinkShapePoint(ShapePoint* pSrcPnt, ShapePoint* pDstPnt)	
{
	int rt;
	Figure* pSrcFig, *pDstFig;
//	int style;

	// 1、合法性判断及方式判断
// 	rt = LinkShapePoint_Judge(pSrcPnt, pDstPnt, &style);
// 	if (rt == TRUE)
// 	{
// 		Figure* pSrcFig, *pDstFig;
// 
// 		if (style == Link_Head2Tail)
// 		{
// 			// 源、目标图形 调换，只处理尾头连接
// 			pSrcFig = pDstPnt->m_pFigure;
// 			pDstFig = pSrcPnt->m_pFigure;
// 		}
// 		else if (style == Link_Tail2Head)
// 		{
// 			pSrcFig = pSrcPnt->m_pFigure;
// 			pDstFig = pDstPnt->m_pFigure;
// 		}
// 		else if (style == Link_Head2Head)
// 		{
// 			pSrcFig->ReverseShapePointList();
// 			pSrcPnt = pSrcFig->GetTailShapePoint();
// 			pDstPnt = pDstFig->GetHeadShapePoint();
// 		}
// 		else
// 			return -1;	// 暂时不处理头头，尾尾相连
//
		// 图形尾连接图形头
// 		rt = LinkTail2Head(pSrcFig, pDstFig);
// 
// 		return rt;
//	}
//	return -1;

	// 图形尾连接图形头
	pSrcFig = pSrcPnt->m_pFigure;
	pDstFig = pDstPnt->m_pFigure;
	rt = LinkTail2Head(pSrcFig, pDstFig);

	return rt;
}


/*--------------------------------------------------------------------------------------*/
/*                                轮廓线链表相关操作                                    */
/*--------------------------------------------------------------------------------------*/
// 清空轮廓线链表
void GeneralInterface::ClearOutlineList(vector<FPoint*>* pList)
{
	if (pList != NULL)
	{
		int i, nCount;
		vector<FPoint*>::iterator it;

		// 释放内存
		nCount = pList->size();
		for (i = 0; i < nCount; i++)
		{
			it = pList->begin() + i;
			FPoint* pPnt = *it;

			delete pPnt;
			pPnt = NULL;
		}

		// 清空容器
		pList->clear();
	}
}


// 移动形状点链表
void GeneralInterface::MoveOutlineList(vector<FPoint*>* pList, float rel_x, float rel_y)
{
	int i, nCount;
	vector<FPoint*>::iterator it;

	nCount = pList->size();
	for (i = 0; i < nCount; i++)
	{
		it = pList->begin() + i;

		(*it)->m_x += rel_x;
		(*it)->m_y += rel_y;
	}
}



/*--------------------------------------------------------------------------------------*/
/*                                数学计算相关操作                                     */
/*--------------------------------------------------------------------------------------*/
// 获取极坐标系中的角度，弧度表示
float GeneralInterface::GetRadianFromXY(float dx, float dy)	
{
	float angle;

	if (dx == 0.0)
	{
		if (dy > 0.0)
		{
			angle = (float)(PI/2);
		}
		else
		{
			angle = (float)(-PI/2);
		}
	}
	else if (dx < 0.0)
	{
		angle = (float)(atan(dy/dx) + PI);
	}
	else
	{
		angle = atan(dy/dx);
	}

	if (angle < 0.0)
		angle += (float)(2*PI);

	return angle;
}


/*----------------------------------------------------------*/
//	purpose:
//		指定点到直线的距离
//		斜截式：y = kx + b   代入(x1，y1)  =》  b = y1 - kx1 
//		一般式：Ax + By + C = 0
//		A = k  B = -1  C = b 
//		点到直线公式|Ax + By + C|/根号(A*A + B*B)
//
//	param:
//		float start_x	--	起点x
//		float star_y	--	起点y
//		float end_x		--	终点x
//		float end_y		--	终点y
//		float x			--  定点x
//		float y			--	定点y
//
//	return:
//		float	--	距离
/*----------------------------------------------------------*/
float GeneralInterface::GetDistanceFromPoint2Line(float start_x, float start_y, float end_x, float end_y, float x, float y)
{
	float dx, dy, k, dist, b;

	dx = end_x - start_x;
	dy = end_y - start_y;

	if (dx == 0)		// 垂直x轴
	{
		dist = abs(x - start_x);

	}
	else if (dy == 0)	// 垂直y轴
	{
		dist = abs(y - start_y);
	}
	else 
	{
		k = dy/dx;
		b = start_y - k*start_x;
		dist = abs(k*x + -1*y + b) / sqrt(1 + k*k);
	}

	return	dist;
}



/*----------------------------------------------------------------------*/
//	purpose:
//		计算两向量夹角，用点乘，a·b = |a||b|cosΘ
//
//	param：
//		dx1	-- 向量1x偏移
//		dy1	-- 向量1y偏移
//		dx2	-- 向量2x偏移
//		dy2	-- 向量2y偏移
//
//	return:
//		angle -- 角度
/*----------------------------------------------------------------------*/
float GeneralInterface::CalVectorAngle(float dx1, float dy1, float dx2, float dy2)
{
	float angle, dot_product, module_product;

	dot_product = dx1*dx2 + dy1*dy2;	// 点积
	module_product = sqrt(dx1*dx1 + dy1*dy1) * sqrt(dx2*dx2 + dy2*dy2);	// 模积
	angle = acos(dot_product/module_product);

	return angle;
}

/*----------------------------------------------------------*/
//	purpose:
//		平移一条线段
//		根据方向，通过向量旋转方式平移线段
//
//	param:
//		float start_x	--	起点x
//		float star_y	--	起点y
//		float end_x		--	终点x
//		float end_y		--	终点y
//		int dir			--	方向	0：左边	1：右边
//		float dist		--	距离	
//		float new_start_x	--	新起点x
//		float new_star_y	--	新起点y
//		float new_end_x		--	新终点x
//		float new_end_y		--	新终点y
//
//	return:
//		TRUE	--	成功
//		FALSE	--	失败
/*----------------------------------------------------------*/
BOOL GeneralInterface::ParallelSegment(float start_x, float start_y, float end_x, float end_y, int dir, float dist,
									   float & new_start_x, float & new_start_y, float & new_end_x, float & new_end_y)
{
	float dx, dy, len;
	float vec_x, vec_y, normal_vec_x, normal_vec_y;
	float arc = PI/2.0;	// 90度

	// 求出向量
	dx = end_x - start_x;
	dy = end_y - start_y;
	len = sqrt(dx*dx + dy*dy);

	// 判断向量是否为0
	if (len < CLOSE_VALUE)	// 重合
	{
		start_x	= new_start_x;
		start_y	= new_start_y;
		end_x	= new_end_x;
		end_y	= new_end_y;

		return FALSE;
	}

	// 旋转向量
	if (dir == 0)	// 左边 逆时针90度
	{
		CouterClockwiseRotate(dx, dy, vec_x, vec_y, arc);
	}
	else			// 顺时针90度
	{
		ClockwiseRotate(dx, dy, vec_x, vec_y, arc);
	}

	// 向量规范化后乘以平行距离
	NormalVector(vec_x, vec_y, normal_vec_x, normal_vec_y);
	normal_vec_x *= dist;
	normal_vec_y *= dist;	

	// 原始坐标加上平行向量就得到平行后的线段了
	new_start_x	= start_x	+ normal_vec_x;
	new_start_y	= start_y	+ normal_vec_y;
	new_end_x	= end_x		+ normal_vec_x;
	new_end_y	= end_y		+ normal_vec_y;

	return TRUE;
}


// 逆时针旋转
void GeneralInterface::CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

// 顺时针旋转
void GeneralInterface::ClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) + mid_y*sin(arc);
	float rotate_y = -mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

// 向量规范化
void GeneralInterface::NormalVector(float x0,float y0,float & x,float & y)
{
	float len = sqrt(x0*x0 + y0*y0);

	x = x0/len;
	y = y0/len;

}

// 两点取圆心
void GeneralInterface::GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float dx = beginx - endx;
	float dy = beginy - endy;
	float chord_len = sqrt(dx*dx + dy*dy);
	float sita = acos(0.5*chord_len/radius);	// 角度
	float angle = Radian2Angle(sita);
	float new_x, new_y;

	// 顺时针旋转，指向圆心的向量， 若大于180度时需要逆时针旋转
	ClockwiseRotate(dx, dy, new_x, new_y, sita);

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


/*----------------------------------------------------------*/
//	purpose:
//		求线段相交
//
//	param:
//		float start_x	--	起点x
//		float start_y	--	起点y
//		float end_x		--	终点x
//		float end_y		--	终点y
//		float center_x	--	圆心x
//		float center_y	--	圆心y
//		float mid_x		--	中点x
//		float mid_y		--	中点y
//
//	return:
//		TRUE	--	正确
//		FALSE	--	错误
/*----------------------------------------------------------*/
BOOL GeneralInterface::CalArcMidPoint(float start_x, float start_y, float end_x, float end_y, float center_x, float center_y, float r, float &mid_x, float &mid_y)
{
	float dx, dy, chord_len;
	float vec_x,  vec_y;	// 插补时用的向量
	float second_Interpolate_x, second_Interpolate_y;

	// 圆弧弦向量
	dx = end_x - start_x;
	dy = end_y - start_y;

	// 弦长
	chord_len = sqrt(dx*dx + dy*dy);
	if (chord_len < 0)
		return FALSE;

	/*-------------弦中点------------*/

	second_Interpolate_x = start_x + dx/2.0;
	second_Interpolate_y = start_y + dy/2.0;

	// 圆心指向弦中点的向量
	vec_x = second_Interpolate_x - center_x;
	vec_y = second_Interpolate_y - center_y;

	// 向量规范化后,乘以半径得到指向圆弧中点的向量
	NormalVector(vec_x, vec_y, vec_x, vec_y);
	vec_x *= r;
	vec_y *= r;

	// 求得圆弧中点坐标
	second_Interpolate_x = center_x + vec_x;
	second_Interpolate_y = center_y + vec_y;

	mid_x = second_Interpolate_x;
	mid_y = second_Interpolate_y;
	
	return TRUE;
}


/*----------------------------------------------------------*/
//	purpose:
//		求线段相交
//
//	param:
//		float start_x1	--	第一条线段起点x
//		float start_y1	--	第一条线段起点y
//		float end_x1	--	第一条线段终点x
//		float end_y1	--	第一条线段终点y
//		float start_x2	--	第二条线段起点x
//		float start_y2	--	第二条线段起点y
//		float end_x2	--	第二条线段终点x
//		float end_y2	--	第二条线段终点y
	
//		float &inter_x	--	交点x
//		float &inter_y	--	交点y
//
//	return:
//		TRUE	--	相交
//		FALSE	--	不相交
/*----------------------------------------------------------*/

#define   ACCURACY			0.1				// 精度
#define   ACCURACYTOW		0.00000001
BOOL GeneralInterface::CalSegmenIntersect(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y)
{

	if(fabs(start_x1 - end_x1) < ACCURACYTOW && fabs (start_y1 - end_y1) < ACCURACYTOW || fabs(start_x2 - end_x2) < ACCURACYTOW && fabs(start_y2 - end_y2) < ACCURACYTOW)
	{
		return FALSE;
	}

	if(fabs( start_x1 - end_x1) > ACCURACYTOW && fabs(start_y1 - end_y1) < ACCURACYTOW)	
	{
		if(fabs( start_x2 - end_x2) > ACCURACYTOW && fabs(start_y2- end_y2)<ACCURACYTOW )
		{
			if(fabs(start_y1-start_y2)<ACCURACYTOW &&(!((start_x2> (start_x1+ACCURACY)&&start_x2> (end_x1+ACCURACY)&&end_x2>(start_x1+ACCURACY)&&end_x2> (end_x1+ACCURACY))||((start_x2+ACCURACY)< start_x1&&(start_x2+ACCURACY)< end_x1&&(end_x2+ACCURACY)<start_x1&&(end_x2+ACCURACY)<end_x1))))
			{ 
				if(!(start_x2> start_x1&&start_x2> end_x1&&start_x2> end_x2||start_x2< start_x1&&start_x2<end_x1&&start_x2< end_x2))

				{
					inter_x=start_x2;
					inter_y=start_y2;
					return TRUE;
				}
				if(!(start_x1> start_x2&&start_x1> end_x1&&start_x1> end_x2||start_x1< start_x2&&start_x1<end_x1&&start_x1< end_x2))

				{
					inter_x=start_x1;
					inter_y=start_y1;
					return TRUE;
				}
				if(!(end_x2> start_x1&&end_x2> end_x1&&end_x2> start_x2||end_x2< start_x1&&end_x2<end_x1&&end_x2< start_x2))

				{
					inter_x=end_x2;
					inter_y=end_y2;
					return TRUE;
				}if(!(end_x1> start_x1&&end_x1> start_x2&&end_x1> end_x2||end_x1< start_x1&&end_x1<start_x2&&end_x1< end_x2))

				{
					inter_x=end_x1;
					inter_y=end_x2;
					return TRUE;
				}
			}
			else
				return FALSE;

		}
		else
		{
			float xpoint=( start_x2- end_x2)/(start_y2- end_y2)*end_y1+(end_x2*start_y2-start_x2*end_y2)/(start_y2- end_y2);
			inter_y=end_y1;
			inter_x=xpoint;
			if((end_x2<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x2||end_x2>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x2)&&(end_x1<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x1||end_x1>=(inter_x-ACCURACY)&&(inter_x>+ACCURACY)>=start_x1))
				return TRUE;
			else
				return FALSE;


		}
	}
	if(fabs( start_x1- end_x1)<ACCURACYTOW &&fabs(start_y1- end_y1)>ACCURACYTOW )
	{
		if(fabs( start_x2- end_x2)<ACCURACYTOW &&fabs(start_y2- end_y2)>ACCURACYTOW )
		{ 
			if(fabs(start_x1-start_x2)<ACCURACYTOW &&(!((start_y2> (start_y1+ACCURACY)&&start_y2> (end_y1+ACCURACY)&&end_y2>(start_y1+ACCURACY)&&end_y2> (end_y1+ACCURACY))||((start_y2+ACCURACY)< start_y1&&(start_y2+ACCURACY)< end_y1&&(end_y2+ACCURACY)<start_y1&&(end_y2+ACCURACY)<end_y1))))
			{ 
				if(!(start_y2> start_y1&&start_y2> end_y1&&start_y2> end_y2||start_y2< start_y1&&start_y2<end_y1&&start_y2< end_y2))

				{
					inter_x=start_x2;
					inter_y=start_y2;
					return TRUE;
				}
				if(!(start_y1> start_y2&&start_y1> end_y1&&start_y1> end_y2||start_y1< start_y2&&start_y1<end_y1&&start_y1< end_y2))

				{
					inter_x=start_x1;
					inter_y=start_y1;
					return TRUE;
				}
				if(!(end_y2> start_y1&&end_y2> end_y1&&end_y2> start_y2||end_y2< start_y1&&end_y2<end_y1&&end_y2< start_y2))

				{
					inter_x=end_x2;
					inter_y=end_y2;
					return TRUE;
				}if(!(end_y1> start_y1&&end_y1> start_y2&&end_y1> end_y2||end_y1< start_y1&&end_y1<start_y2&&end_y1< end_y2))

				{
					inter_x=end_x1; inter_y=end_x2; return TRUE;
				}
			}
			else
				return FALSE;
		}
		else
		{
			float ypoint=( start_y2- end_y2)/(start_x2- end_x2)*end_x1+(end_y2*start_x2-start_y2*end_x2)/(start_x2- end_x2); 
			inter_x=end_x1;
			inter_y=ypoint;
			if((end_x2<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x2||end_x2>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x2)&&(end_x1<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x1||end_x1>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x1))
				return TRUE;
			else
				return FALSE;
		}
	}
	else
	{
		float k=-( start_y1- end_y1)/( start_x1- end_x1);
		float b=-start_y1-k* start_x1;
		if((k* start_x2+b+start_y2)<0&&(k* end_x2+b+end_y2)>0||(k* start_x2+b+start_y2)>0&&(k* end_x2+b+end_y2)<0)
		{
			inter_x=(-(start_y2+b)*(start_x2-end_x2)+start_x2*(start_y2-end_y2))/(k*(start_x2-end_x2)+(start_y2-end_y2));
			inter_y=-inter_x*k-b;

			if((end_x1<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x1||end_x1>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x1)&&(end_y1<=(inter_y+ACCURACY)&&(inter_y-ACCURACY)<=start_y1||end_y1>=(inter_y-ACCURACY)&&(inter_y+ACCURACY)>=start_y1))
				return TRUE;
			else
				return FALSE;
		}

		if(fabs(( start_x1- end_x1)*( start_y2- end_y2)-( start_x2- end_x2)*( start_y1- end_y1))>ACCURACYTOW)
		{

			inter_x=(-(start_y2+b)*(start_x2-end_x2)+start_x2*(start_y2-end_y2))/(k*(start_x2-end_x2)+(start_y2-end_y2));
			inter_y=-inter_x*k-b;

			if((end_x2<=(inter_x+ACCURACY)&&(inter_x-ACCURACY)<=start_x2||end_x2>=(inter_x-ACCURACY)&&(inter_x+ACCURACY)>=start_x2)&&(end_y2<=(inter_y+ACCURACY)&&(inter_y-ACCURACY)<=start_y2||end_y2>=(inter_y-ACCURACY)&&(inter_y+ACCURACY)>=start_y2))
				return TRUE;
			else
				return FALSE;

		}
		if(fabs(( start_x1- end_x1)*( start_y2- end_y2)-( start_x2- end_x2)*( start_y1- end_y1))<ACCURACYTOW&&fabs(k* start_x2+b+start_y2)<ACCURACYTOW)
		{ 	if(!((start_y2> (start_y1+ACCURACY)&&start_y2> (end_y1+ACCURACY)&&end_y2>(start_y1+ACCURACY)&&end_y2> (end_y1+ACCURACY))||((start_y2+ACCURACY)< start_y1&&(start_y2+ACCURACY)< end_y1&&(end_y2+ACCURACY)<start_y1&&(end_y2+ACCURACY)<end_y1)))
		{
			if(!(start_y2> start_y1&&start_y2> end_y1&&start_y2> end_y2||start_y2< start_y1&&start_y2<end_y1&&start_y2< end_y2))

			{
				inter_x=start_x2;
				inter_y=start_y2;
				return TRUE;
			}
			if(!(start_y1> start_y2&&start_y1> end_y1&&start_y1> end_y2||start_y1< start_y2&&start_y1<end_y1&&start_y1< end_y2))

			{
				inter_x=start_x1;
				inter_y=start_y1;
				return TRUE;
			}
			if(!(end_y2> start_y1&&end_y2> end_y1&&end_y2> start_y2||end_y2< start_y1&&end_y2<end_y1&&end_y2< start_y2))

			{
				inter_x=end_x2;
				inter_y=end_y2;
				return TRUE;
			}if(!(end_y1> start_y1&&end_y1> start_y2&&end_y1> end_y2||end_y1< start_y1&&end_y1<start_y2&&end_y1< end_y2))

			{
				inter_x=end_x1;
				inter_y=end_x2;
				return TRUE;
			}
		}
		}

		else return FALSE;
	}


	return FALSE;
}





//计算交叉乘积(P1-P0)x(P2-P0)
double xmult(point p1,point p2,point p0){
	return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
}

//判点是否在线段上,包括端点
int dot_online_in(point p,point l1,point l2){
	return zero(xmult(p,l1,l2))&&(l1.x-p.x)*(l2.x-p.x)<eps&&(l1.y-p.y)*(l2.y-p.y)<eps;
}

//判两点在线段同侧,点在线段上返回0
int same_side(point p1,point p2,point l1,point l2){
	return xmult(l1,p1,l2)*xmult(l1,p2,l2)>eps;
}

//判两直线平行
int parallel(point u1,point u2,point v1,point v2){
	return zero((u1.x-u2.x)*(v1.y-v2.y)-(v1.x-v2.x)*(u1.y-u2.y));
}

//判三点共线
int dots_inline(point p1,point p2,point p3){
	return zero(xmult(p1,p2,p3));
}

//判两线段相交,包括端点和部分重合
int intersect_in(point u1,point u2,point v1,point v2){
	if (!dots_inline(u1,u2,v1)||!dots_inline(u1,u2,v2))
		return !same_side(u1,u2,v1,v2)&&!same_side(v1,v2,u1,u2);
	return dot_online_in(u1,v1,v2)||dot_online_in(u2,v1,v2)||dot_online_in(v1,u1,u2)||dot_online_in(v2,u1,u2);
}

//计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
point intersection(point u1,point u2,point v1,point v2)
{
	point ret=u1;
	double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
		/((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
	ret.x+=(u2.x-u1.x)*t;
	ret.y+=(u2.y-u1.y)*t;
	return ret;
}


 BOOL GeneralInterface::CalSegmenIntersect2(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y)
{
	point u1,u2,v1,v2;
	point ans;

	u1.x = start_x1;
	u1.y = start_y1;

	u2.x = end_x1;
	u2.y = end_y1;

	v1.x = start_x2;
	v1.y = start_y2;

	v2.x = end_x2;
	v2.y = end_y2;

	if (parallel(u1,u2,v1,v2) || !intersect_in(u1,u2,v1,v2))
	{
		return FALSE;
	}
	else
	{
		ans = intersection(u1,u2,v1,v2);
		inter_x = ans.x;
		inter_y = ans.y;
		return TRUE;
	}

	return FALSE;
}




 /*----------------------------------------------------------------------*/
 //	purpose:
 //		计算两直线相交点
 //
 //	param：
 //		(start_x1, start_y1)	-- 第一条直线起点
 //		(end_x1, end_y1)	-- 第一条直线终点
 //		(start_x2, start_y2)	-- 第二条直线起点
 //		(end_x2, end_y2)	-- 第二条直线终点
 //		(inter_x, inter_y) -- 交点
 //
 //	return:
 //		TRUE -- 无交点
 //		FALSE  -- 有交点
 /*----------------------------------------------------------------------*/

#define		EQUAL_VALUE		0.01
 BOOL GeneralInterface::CalLineIntersect(float start_x1, float start_y1, float end_x1, float end_y1, float start_x2, float start_y2, float end_x2, float end_y2, float& inter_x, float& inter_y)
 {
	 float k1, k2, dx1, dy1, dx2, dy2;

	 dx1 = end_x1 - start_x1;
	 dy1 = end_y1 - start_y1;
	 dx2 = end_x2 - start_x2;
	 dy2 = end_y2 - start_y2;

	 if (abs(dx1) < EQUAL_VALUE)		// 第一条直线垂直Y轴
	 {
		 if (abs(dx2) < EQUAL_VALUE)  
			 return FALSE;
		 else
		 {
			 k2 = dy2/dx2;
			 inter_x = start_x1;
			 inter_y = k2*(inter_x - start_x2)+ start_y2 ;

			 return TRUE;
		 }
	 }
	 else if (abs(dx2) < EQUAL_VALUE)	// 第二条直线垂直Y轴 
	 {
		 k1 = dy1/dx1;
		 inter_x = start_x2;
		 inter_y = k1*(inter_x - start_x1)+ start_y1 ;

		 return TRUE;
	 }
	 else
	 {
		 k1 = dy1/dx1;
		 k2 = dy2/dx2;

		 if(k1 == k2)		// 平行，但需要判断是否重合 
		 {
			 float dist = GetDistanceFromPoint2Line(start_x1, start_y1, end_x1, end_y1, start_x2, start_y2);
			 if (dist < CLOSE_VALUE)	// 点在直线上
			 {
				 inter_x = start_x2;
				 inter_y = start_y2;

				 return TRUE;
			 }
			 else
			 {
				 return FALSE;
			 }
		 }
		 else
		 {
			 inter_x = (start_y2 - start_y1 - (k2*start_x2 - k1*start_x1))/(k1 - k2);
			 inter_y = k1*(inter_x - start_x1) + start_y1;

			 return TRUE;
		 }
	 }
 }

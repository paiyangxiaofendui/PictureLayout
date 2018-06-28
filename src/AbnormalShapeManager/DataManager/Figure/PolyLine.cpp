#include "stdafx.h"
#include "PolyLine.h"
#include "../CommonData.h"
#include "../Layer/Layer.h"
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"
#include "Arc.h"


Cad_PolyLine::Cad_PolyLine()
{
	m_FigureType = FigureType_PolyLine;
}

Cad_PolyLine::~Cad_PolyLine()
{

}


/*-----------------------------------------------------------------------*/
//	purpose:
//		计算平行的形状点链表
//		1、平行后的图形一定为组合图形
//		2、每两个点平行出一条直线，相交则取相交点，不相交则插入圆弧连接，
//
//	param：
//		int dir -- 方向
//		float dist -- 距离
//
//	return:
//		vector<ShapePoint*> -- 平行后的形状点
/*-----------------------------------------------------------------------*/
Figure* Cad_PolyLine::CalParallelFigure(int dir, float dist)
{
	BOOL rt = FALSE;
	BOOL ClosedFlag = FALSE;			// 图形闭合
	BOOL IsInterWithPrevSeg = FALSE;	// 与上一条线段相交
	BOOL IsInterWithNextSeg = FALSE;	//与下一条线段相交
	int i, nCount;
	float inter_x = 0, inter_y = 0;		// 交点坐标
	float pre_x, pre_y, cur_x, cur_y, next_x, next_y;			// 原始线段点
	float new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y;		// 平行后的线段点
	float dx, dy, mid_Interpolate_x, mid_Interpolate_y;			// 线段连接的圆弧插补点

	ShapePoint *pNewHead;								// 头尾点
	ShapePoint* pPrePnt, *pCurPnt, *pNextPnt;							// 原始点
	ShapePoint* pNewPrePnt, *pNewCurPnt1, *pNewCurPnt2;					// 平行后的点
	vector<ShapePoint*> ParallelShapePointList;							// 平行的形状点链表
	Cad_PolyLine* pParallelPolyLine = new Cad_PolyLine;					// 平行图形

	// 判断是否闭合
	ClosedFlag = IsClosed();

	// 判断形状点数是否合法
	nCount = GetShapePointNum();
	if (nCount < 2)
	{
		return NULL;
	}

	for (i = 1; i < nCount; i++)
	{
		if (i == nCount-1)	// 最后一点
		{
			if (ClosedFlag == TRUE)	// 闭合的当前线段与第一段线段相交
			{
				pPrePnt = GetShapePoint(i-1);
				pCurPnt = GetShapePoint(i);
				pNextPnt = GetShapePoint((i+2)%nCount);

				pPrePnt->GetXY(pre_x, pre_y);
				pCurPnt->GetXY(cur_x, cur_y);
				pNextPnt->GetXY(next_x, next_y);

				// 平行后添加折线
				rt = GeneralInterface::ParallelSegment(pre_x, pre_y, cur_x, cur_y, dir, dist, new_pre_x, new_pre_y, new_cur_x1, new_cur_y1);
				if (rt == FALSE)
				{
					AfxMessageBox("有重合点，线段平行出错！");
				}

				// 与上一条线段之前是否有交点，有的话首点为相交点
				if (IsInterWithPrevSeg == TRUE)
				{
					new_pre_x = inter_x;
					new_pre_y = inter_y;
					IsInterWithPrevSeg = FALSE;
				}
				else
				{
					// 第一个点平行点
					pNewPrePnt = new ShapePoint(new_pre_x, new_pre_y);
					pParallelPolyLine->AddShapePoint(pNewPrePnt);
					pNewPrePnt = NULL;
				}



				rt = GeneralInterface::ParallelSegment(cur_x, cur_y, next_x, next_y, dir, dist, new_cur_x2, new_cur_y2, new_next_x, new_next_y);
				if (rt == FALSE)
				{
					AfxMessageBox("有重合点，线段平行出错！");
				}

				// 判断与下一段线是否相交，超过120度，直线平行，小于120度线段平行
				float flag_angle = Angle2Radian(OBTUSE_ANGLE_VALUE);
				float angle = GeneralInterface::CalVectorAngle(new_cur_x1 - new_pre_x, new_cur_y1 - new_pre_y, new_next_x - new_cur_x2, new_next_y - new_cur_y2);
				 
				if (angle > flag_angle)	 // 锐角
				{
					IsInterWithNextSeg = GeneralInterface::CalSegmenIntersect2(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// 调函数
				}
				else					// 钝角
				{
					IsInterWithNextSeg = GeneralInterface::CalLineIntersect(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// 调函数
				}

				if (IsInterWithNextSeg == TRUE)
				{
					new_cur_x1 = inter_x;
					new_cur_y1 = inter_y;

					IsInterWithPrevSeg = TRUE;	// 下一段直线起点为交点
					IsInterWithNextSeg = FALSE;

					// 第二个点平行点
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);


					// 与下一条线段相交则添加到图层，到此为止
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);

					pNewCurPnt1 = NULL;

					// 闭合图形且最后一条线段与第一条线段相交，第一条线段起点需要改变坐标
					pNewHead = pParallelPolyLine->GetHeadShapePoint();
					pNewHead->SetXY(new_cur_x1, new_cur_y1);

				}
				else	// 不与下一条线段相交，则需要添加连接的圆弧，圆心为当前点，起点为当前线段的尾点，终点为下一线段的起点，半径dist，方向为逆时针
				{
					// 第二个点平行点
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);

					// 圆弧尾点
					pNewCurPnt2 = new ShapePoint(new_cur_x2, new_cur_y2);

#if 0
					// 添加连接圆弧，圆心半径，起终点求圆弧，直接在圆弧上插补3个点拟合

					// 弦
					dx = new_cur_x2 - new_cur_x1;
					dy = new_cur_y2 - new_cur_y1;

					// 弦中点
					mid_Interpolate_x = new_cur_x1 + dx/2.0;
					mid_Interpolate_y = new_cur_y1 + dy/2.0;

					// 圆心指向弦中点的向量
					float mid_vec_x = mid_Interpolate_x - cur_x;
					float mid_vec_y = mid_Interpolate_y - cur_y;

					// 向量规范化后,乘以半径得到指向圆弧中点的向量
					GeneralInterface::NormalVector(mid_vec_x, mid_vec_y, mid_vec_x, mid_vec_y);
					mid_vec_x *= dist;
					mid_vec_y *= dist;

					// 求得圆弧中点坐标
					mid_Interpolate_x = cur_x + mid_vec_x;
					mid_Interpolate_y = cur_y + mid_vec_y;

					// 圆弧中点
					ShapePoint* pMidPnt = new ShapePoint(mid_Interpolate_x, mid_Interpolate_y);


					pParallelPolyLine->AddShapePoint(pMidPnt);
					pMidPnt = NULL;

#else				// 添加连接弧

					Cad_PolyLine LinkArc;
					LinkArc.m_ShapePointList = GeneralInterface::ArcInterpolate(pCurPnt, pNewCurPnt1, pNewCurPnt2, dist);

					// 圆弧首尾点与平行点重合，所以不加，添加中间插补点即可
					int index, nLinkArcShpPntNum;
					nLinkArcShpPntNum = LinkArc.GetShapePointNum();
					for (index = 1; index < nLinkArcShpPntNum-1; index++)
					{
						ShapePoint* pArcPnt = LinkArc.GetShapePoint(index);
						ShapePoint* pInterPoint = new ShapePoint(pArcPnt->m_x, pArcPnt->m_y);

						pParallelPolyLine->AddShapePoint(pInterPoint);
					}

#endif
					pParallelPolyLine->AddShapePoint(pNewCurPnt2);
					pNewCurPnt2 = NULL;

					// 添加后置空，避免对后面造成影响
					pNewCurPnt1 = NULL;

				}
			}
			else					// 非闭合图形
			{
				pPrePnt = GetShapePoint(i-1);
				pCurPnt = GetShapePoint(i);

				pPrePnt->GetXY(pre_x, pre_y);
				pCurPnt->GetXY(cur_x, cur_y);

				// 平行
				rt = GeneralInterface::ParallelSegment(pre_x, pre_y, cur_x, cur_y, dir, dist, new_pre_x, new_pre_y, new_cur_x1, new_cur_y1);
				if (rt == FALSE)
				{
					AfxMessageBox("有重合点，线段平行出错！");
				}

				// 与上一条线段之前是否有交点，有的话首点为相交点
				if (IsInterWithPrevSeg == TRUE)
				{
					new_pre_x = inter_x;
					new_pre_y = inter_y;
					IsInterWithPrevSeg = FALSE;

					// 与上一段圆弧相交不应该再添加相交点
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);
					pNewCurPnt1 = NULL;
				}
				else
				{
					pNewPrePnt = new ShapePoint(new_pre_x, new_pre_y);
					pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);

					pParallelPolyLine->AddShapePoint(pNewPrePnt);
					pParallelPolyLine->AddShapePoint(pNewCurPnt1);

					pNewPrePnt = NULL;
					pNewCurPnt1 = NULL;

				}
			}
		}
		else		// 中间点
		{
			pPrePnt = GetShapePoint(i-1);
			pCurPnt = GetShapePoint(i);
			pNextPnt = GetShapePoint(i+1);

			pPrePnt->GetXY(pre_x, pre_y);
			pCurPnt->GetXY(cur_x, cur_y);
			pNextPnt->GetXY(next_x, next_y);

			// 平行后添加折线
			rt = GeneralInterface::ParallelSegment(pre_x, pre_y, cur_x, cur_y, dir, dist, new_pre_x, new_pre_y, new_cur_x1, new_cur_y1);
			if (rt == FALSE)
			{
				AfxMessageBox("有重合点，线段平行出错！");
			}

			// 与上一条线段之前是否有交点，有的话首点为相交点， 但不应该添加首点
			if (IsInterWithPrevSeg == TRUE)
			{
				new_pre_x = inter_x;
				new_pre_y = inter_y;
				IsInterWithPrevSeg = FALSE;
			}
			else
			{
				// 第一个点平行点
				pNewPrePnt = new ShapePoint(new_pre_x, new_pre_y);
				pParallelPolyLine->AddShapePoint(pNewPrePnt);
				pNewPrePnt = NULL;
			}
			
			


			rt = GeneralInterface::ParallelSegment(cur_x, cur_y, next_x, next_y, dir, dist, new_cur_x2, new_cur_y2, new_next_x, new_next_y);
			if (rt == FALSE)
			{
				AfxMessageBox("有重合点，线段平行出错！");
			}

			// 判断与下一段线是否相交，超过120度，直线平行，小于120度线段平行
			float flag_angle = Angle2Radian(OBTUSE_ANGLE_VALUE);
			float angle = GeneralInterface::CalVectorAngle(new_cur_x1 - new_pre_x, new_cur_y1 - new_pre_y, new_next_x - new_cur_x2, new_next_y - new_cur_y2);

			if (angle > flag_angle)	 // 锐角
			{
				IsInterWithNextSeg = GeneralInterface::CalSegmenIntersect2(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// 调函数
			}
			else					// 钝角
			{
				IsInterWithNextSeg = GeneralInterface::CalLineIntersect(new_pre_x, new_pre_y, new_cur_x1, new_cur_y1, new_cur_x2, new_cur_y2, new_next_x, new_next_y, inter_x, inter_y);	// 调函数
			}
			if (IsInterWithNextSeg == TRUE)
			{
				new_cur_x1 = inter_x;
				new_cur_y1 = inter_y;

				IsInterWithPrevSeg = TRUE;	// 下一段直线起点为交点
				IsInterWithNextSeg = FALSE;

				// 第二个点平行点
				pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);


				// 与下一条线段相交则添加到图层，到此为止
				pParallelPolyLine->AddShapePoint(pNewCurPnt1);
				pNewCurPnt1 = NULL;

			}
			else	// 不与下一条线段相交，则需要添加连接的圆弧，圆心为当前点，起点为当前线段的尾点，终点为下一线段的起点，半径dist，方向为逆时针
			{
				// 第二个点平行点
				pNewCurPnt1 = new ShapePoint(new_cur_x1, new_cur_y1);
				pParallelPolyLine->AddShapePoint(pNewCurPnt1);		// 当前点的平行点



				// 添加连接圆弧，圆心半径，起终点求圆弧，直接在圆弧上插补3个点拟合
#if 0
				// 弦
				dx = new_cur_x2 - new_cur_x1;
				dy = new_cur_y2 - new_cur_y1;

				// 弦中点
				mid_Interpolate_x = new_cur_x1 + dx/2.0;
				mid_Interpolate_y = new_cur_y1 + dy/2.0;

				// 圆心指向弦中点的向量
				float mid_vec_x = mid_Interpolate_x - cur_x;
				float mid_vec_y = mid_Interpolate_y - cur_y;

				// 向量规范化后,乘以半径得到指向圆弧中点的向量
				GeneralInterface::NormalVector(mid_vec_x, mid_vec_y, mid_vec_x, mid_vec_y);
				mid_vec_x *= dist;
				mid_vec_y *= dist;

				// 求得圆弧中点坐标
				mid_Interpolate_x = cur_x + mid_vec_x;
				mid_Interpolate_y = cur_y + mid_vec_y;

				// 圆弧中点
				ShapePoint* pMidPnt = new ShapePoint(mid_Interpolate_x, mid_Interpolate_y);
				pParallelPolyLine->AddShapePoint(pMidPnt);
				pMidPnt = NULL;
#else
				pNewCurPnt2 = new ShapePoint(new_cur_x2, new_cur_y2);

				Cad_PolyLine LinkArc;
				LinkArc.m_ShapePointList = GeneralInterface::ArcInterpolate(pCurPnt, pNewCurPnt1, pNewCurPnt2, dist);

				// 圆弧首尾点与平行点重合，所以不加，添加中间插补点即可
				int index, nLinkArcShpPntNum;
				nLinkArcShpPntNum = LinkArc.GetShapePointNum();
				for (index = 1; index < nLinkArcShpPntNum-1; index++)
				{
					ShapePoint* pArcPnt = LinkArc.GetShapePoint(index);
					ShapePoint* pInterPoint = new ShapePoint(pArcPnt->m_x, pArcPnt->m_y);

					pParallelPolyLine->AddShapePoint(pInterPoint);
				}

				delete pNewCurPnt2;
				pNewCurPnt2 = NULL;
#endif

				// 添加后置空，避免对后面造成影响
				pNewCurPnt1 = NULL;

			}
		}
	}

	// 更新相应信息
	pParallelPolyLine->CalFigureRect();
	pParallelPolyLine->UpdateShapePointInfo();
	pParallelPolyLine->UpdateOutlineInfo();

	return pParallelPolyLine;
}
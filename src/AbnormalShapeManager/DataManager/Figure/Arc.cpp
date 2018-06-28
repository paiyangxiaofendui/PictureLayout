#include "stdafx.h"
#include "Arc.h"
#include "../CommonData.h"
#include <math.h>
#include "../Point/ShapePoint.h"
#include "../../GeneralInterface/GeneralInterface.h"

Cad_Arc::Cad_Arc()
{
	m_FigureID = 0;
	m_FigureType = FigureType_Arc;
	m_pLayer = NULL;

	m_CenterX = 0.0;
	m_CenterY = 0.0;
	m_radius = 0.0;
	m_StartAngle = 0.0;
	m_EndAngle = 0.0;


}

Cad_Arc::~Cad_Arc()
{

}


/*----------------------------------------------------------*/
//	purpose:
//		移动图形
//		
//	param:
//		float rel_x -- 相对坐标x
//		float rel_y -- 相对坐标y
//
//	return:
//		void
/*----------------------------------------------------------*/
void Cad_Arc::MoveFigure(float rel_x, float rel_y)
{
	// 移动形状点
	GeneralInterface::MoveShapePointList(&m_ShapePointList, rel_x, rel_y);

	// 重新计算圆心半径
	CalCenterRadiusInfo();

	// 重新生成轮廓点
	CalOutlineList();

	// 重新计算矩形
	CalFigureRect();
}


/*-----------------------------------------------------------------------*/
//	purpose:
//		计算平行的形状点链表
//		根据圆心，半径，外扩或者内缩
//		
//
//	param：
//		int dir -- 方向
//		float dist -- 距离
//
//	return:
//		vector<ShapePoint*> -- 平行后的形状点
/*-----------------------------------------------------------------------*/
Figure*  Cad_Arc::CalParallelFigure(int dir, float dist)
{
	int arc_dir = GetDirection();
	float new_radius;
	float vec_start_x, vec_start_y, vec_mid_x, vec_mid_y, vec_end_x, vec_end_y;
	ShapePoint *pStart, *pMid, *pEnd;
	ShapePoint *pNewStart, *pNewMid, *pNewEnd;
	Cad_Arc* pParallelArc = new Cad_Arc;

	// 赋初值，以第一点为原点
	pStart	= GetShapePoint(0);
	pMid	= GetShapePoint(1);
	pEnd	= GetShapePoint(2);

	// 求圆心指向各点向量
	vec_start_x = pStart->m_x - m_CenterX;
	vec_start_y = pStart->m_y - m_CenterY;
	vec_mid_x	= pMid->m_x - m_CenterX;
	vec_mid_y	= pMid->m_y - m_CenterY;
	vec_end_x	= pEnd->m_x - m_CenterX;
	vec_end_y	= pEnd->m_y - m_CenterY;

	// 规范化向量
	GeneralInterface::NormalVector(vec_start_x, vec_start_y, vec_start_x, vec_start_y);
	GeneralInterface::NormalVector(vec_mid_x, vec_mid_y, vec_mid_x, vec_mid_y);
	GeneralInterface::NormalVector(vec_end_x, vec_end_y, vec_end_x, vec_end_y);

	// 计算新半径
	if (arc_dir == Dir_CouterClockwise)	// 逆时针
	{
		if (dir == Dir_Left)	//	左 == 内缩
			new_radius = m_radius - dist;
		else					// 右 == 外扩
			new_radius = m_radius + dist;

		if (new_radius <= 0.0)
		{
			AfxMessageBox("圆弧半径内缩过小！");
		}
	}
	else
	{
		if (dir == Dir_Left)	//	左 == 外扩
			new_radius = m_radius + dist;
		else					// 右 == 内缩
			new_radius = m_radius - dist;

		if (new_radius <= 0.0)
		{
			AfxMessageBox("圆弧半径内缩过小！");
		}
	}

	// 长度为新半径的向量
	vec_start_x *= new_radius;
	vec_start_y *= new_radius;
	vec_mid_x	*= new_radius;
	vec_mid_y	*= new_radius;
	vec_end_x	*= new_radius;
	vec_end_y	*= new_radius;

	// 添加新形状点
	pNewStart = new ShapePoint(vec_start_x + m_CenterX, vec_start_y + m_CenterY);
	pNewMid = new ShapePoint(vec_mid_x + m_CenterX, vec_mid_y + m_CenterY);
	pNewEnd = new ShapePoint(vec_end_x + m_CenterX, vec_end_y + m_CenterY);

	pParallelArc->AddShapePoint(pNewStart);
	pParallelArc->AddShapePoint(pNewMid);
	pParallelArc->AddShapePoint(pNewEnd);

	// 计算圆心半径信息 用于同步
	pParallelArc->CalCenterRadiusInfo();
	pParallelArc->UpdateShapePointInfo();
	pParallelArc->UpdateOutlineInfo();

	return pParallelArc;
}


// 反向形状点容器
void Cad_Arc::ReverseShapePointList()
{
	GeneralInterface::ReverseShapePointList(&m_ShapePointList);
	CalCenterRadiusInfo();
}



// 计算形状点轮廓线信息
void Cad_Arc::CalShapePointOutlineInfo(void)
{
	int dir;
	float angle, r, negative_r_angle;
	ShapePoint* pPnt = NULL;

	// 获取方向、圆心角
	dir = GetDirection();
	angle = GetCentralAngle();
	negative_r_angle = PI;

	// G代码中>180度的圆弧r为负，但应该在出G代码中处理，不在这里做处理 刀补中暂时未对超过
// 	if (angle > negative_r_angle)
// 		r = -m_radius;
// 	else
		r = m_radius;

	// 第一个点
	pPnt = GetShapePoint(0);

	pPnt->m_radius = r;
	pPnt->m_sign = Sign_ArcStart;
	pPnt->m_dir = dir;
	pPnt->m_cut = Cut_KnifeDown;
	pPnt->m_type = Type_Regular;
	

	pPnt = GetShapePoint(1);
	pPnt->m_radius = r;
	pPnt->m_sign = Sign_Ignore;
	pPnt->m_dir = dir;
	pPnt->m_cut = Cut_KnifeDown;
	pPnt->m_type = Type_Regular;


	pPnt = GetShapePoint(2);
	pPnt->m_radius = r;
	pPnt->m_sign = Sign_ArcEnd;
	pPnt->m_dir = dir;
	pPnt->m_cut = Cut_KnifeDown;
	pPnt->m_type = Type_Regular;

}



// 计算轮廓线	
void Cad_Arc::CalOutlineList(void)
{
	int i, num, dir;
	float x, y;
	float L, space, Radian, dRadian;
	FPoint *pNewPnt = NULL;

	// 清空轮廓线链表
	ClearOutlineList();

	// 计算一下圆心半径等信息，确保数据一致
	CalCenterRadiusInfo();

	space = m_OutlineSpace;	// m_OutlineSpace mm为轮廓点间距
	dir = GetDirection();

	if (dir == Dir_CouterClockwise)	// 逆时针，微分弧为正
	{
		if (m_EndAngle > m_StartAngle)
		{
			Radian = m_EndAngle - m_StartAngle;
		}
		else
		{
			Radian = (float)(m_EndAngle + 2*PI - m_StartAngle);
		}
	}
	else	// 顺时针，微分弧为负
	{
		if (m_EndAngle > m_StartAngle)
		{
			Radian = (float)(m_EndAngle - m_StartAngle - 2*PI);
		}
		else
		{
			Radian = m_EndAngle - m_StartAngle; 
		}
	}

	L = abs(m_radius * Radian);

	num = (int)(L/space + 1);

	dRadian = Radian / num;

	// 第一个点
	pNewPnt = new FPoint;
	x = m_CenterX + m_radius *cos(m_StartAngle);
	y = m_CenterY + m_radius *sin(m_StartAngle);

	pNewPnt->SetXY(x, y);
	AddOutline(pNewPnt);
	pNewPnt = NULL;

	// 中间点
	for (i = 1; i < num; i++)
	{
		pNewPnt = new FPoint;
		x = m_CenterX + m_radius *cos(m_StartAngle + i * dRadian);
		y = m_CenterY + m_radius *sin(m_StartAngle + i * dRadian);

		pNewPnt->SetXY(x, y);
		AddOutline(pNewPnt);
		pNewPnt = NULL;
	}

	// 结尾点
	pNewPnt = new FPoint;
	x = m_CenterX + m_radius *cos(m_EndAngle);
	y = m_CenterY + m_radius *sin(m_EndAngle);

	pNewPnt->SetXY(x, y);
	AddOutline(pNewPnt);
	pNewPnt = NULL;
}


// 获取顺逆时针方向
int Cad_Arc::GetDirection(void)
{
	int nCount;
	float sign;
	float x1, y1, x2, y2;
	float dx1, dy1, dx2, dy2;
	ShapePoint p0, p1, p2;

	nCount = m_ShapePointList.size();
	if (nCount != 3)
		return -1;

	// 赋初值，以第一点为原点
	p0 = *(m_ShapePointList[0]);
	p1 = *(m_ShapePointList[1]);
	p2 = *(m_ShapePointList[2]);

	p1 = p1 - p0;
	p2 = p2 - p0;

	// xy坐标
	x1 = p1.m_x;
	y1 = p1.m_y;
	x2 = p2.m_x;
	y2 = p2.m_y; 

	// xy偏移
	dx1 = p1.m_x;
	dy1 = p1.m_y;
	dx2 = p2.m_x - p1.m_x;
	dy2 = p2.m_y - p1.m_y;

	// 判断斜率是否相等
	sign = dy2*dx1 - dx2*dy1;
	if (sign == 0.0)	//平行
	{
		return Dir_Parallel;
	}
	else if (sign > 0)
	{
		return Dir_CouterClockwise;
	}
	else
	{
		return Dir_Clockwise;
	}
}

// 获取圆心角
float Cad_Arc::GetCentralAngle(void)
{
	int dir;
	float angle;

	angle = 0;
	dir = GetDirection();
	if (dir == Dir_Clockwise)
	{
		if (m_StartAngle > m_EndAngle)
		{
			angle = m_StartAngle - m_EndAngle;
		}
		else	
		{
			angle = m_StartAngle + 2*PI - m_EndAngle;
		}
	}
	else if (Dir_CouterClockwise)
	{
		if (m_EndAngle > m_StartAngle)
		{
			angle = m_EndAngle - m_StartAngle;
		}
		else	
		{
			angle = m_EndAngle + 2*PI - m_StartAngle;
		}
	}

	return angle;
}




// 圆心半径起始结束角度，求三个形状点,dxf导入的都是逆时针，看是否超过360度
void Cad_Arc::CenterRadius2ShapePoint(void)
{
	float x, y, angle;
	ShapePoint* pPnt = NULL;

	if (m_StartAngle > m_EndAngle)	// 需加360度
	{
		// 第一个点
		angle = m_StartAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// 第二个点
		angle = (float)((m_StartAngle + m_EndAngle + 2*PI)/2);
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// 第三个点
		angle = m_EndAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);
	}
	else
	{
		// 第一个点
		angle = m_StartAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// 第二个点
		angle = (m_StartAngle + m_EndAngle)/2;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);

		// 第三个点
		angle = m_EndAngle;
		x = m_radius * cos(angle) + m_CenterX;
		y = m_radius * sin(angle) + m_CenterY;

		pPnt = new ShapePoint(x, y);
		AddShapePoint(pPnt);
	}
}

// 通过形状点计算圆心半径
void Cad_Arc::CalCenterRadiusInfo()
{
	int dir;
	float Cx, Cy, r, s_angle,e_angle;
	
	dir = GetDirection();
	CalCenterRadius(&Cx, &Cy, &r, &s_angle, &e_angle);

	m_CenterX = Cx;
	m_CenterY = Cy;
	m_radius = r;
	m_StartAngle = s_angle;
	m_EndAngle = e_angle;
}



// 计算圆心和半径 ，采用圆的一般公式 x^2+y^2+Dx+Ey+F=0 (D^2+E^2-4F>0) 三点代入方程求解
//（X+D/2）^2+（Y+E/2）^2=(D^2+E^2-4F)/4
int Cad_Arc::CalCenterRadius(float* pCx, float* pCy, float* r, float* s_angle, float* e_angle)
{
	int nCount;
	float sign, D, E;
	float numerator, denominator;	// 分子，分母
	float x1, y1, x2, y2;
	float dx1, dy1, dx2, dy2;
	ShapePoint p0, p1, p2;

	nCount = m_ShapePointList.size();
	if (nCount != 3)
		return -1;

	// 赋初值，以第一点为原点
	p0 = *(m_ShapePointList[0]);
	p1 = *(m_ShapePointList[1]);
	p2 = *(m_ShapePointList[2]);

	p1 = p1 - p0;
	p2 = p2 - p0;

	// xy坐标
	x1 = p1.m_x;
	y1 = p1.m_y;
	x2 = p2.m_x;
	y2 = p2.m_y; 

	// xy偏移
	dx1 = p1.m_x;
	dy1 = p1.m_y;
	dx2 = p2.m_x - p1.m_x;
	dy2 = p2.m_y - p1.m_y;

	// 判断斜率是否相等
	sign = dy2*dx1 - dx2*dy1;
	if (sign == 0.0)	//平行
	{
		AfxMessageBox("圆弧数据出错，无法计算！");
		return -1;
	}
	else				
	{
		// 代入求D的化简结果
		numerator = y1*(x2*x2 + y2*y2) - y2*(x1*x1 + y1*y1);
		denominator = x1*y2 - x2*y1;

		D = numerator/denominator;

		// 其中一个y有可能为0
		if (abs(y1) > abs(y2) )
			E = -(x1*x1 + y1*y1 + D*x1)/y1;
		else
			E = -(x2*x2 + y2*y2 + D*x2)/y2;

		// 
		float dx, dy, start_angle, end_angle;
		float center_x, center_y, radius;

		// 求得圆心(此处圆心是相对第一个形状点的坐标，后面需加上第一个形状点的绝对坐标才是真正的绝对坐标),半径是正确的 
		center_x = -(D/2);
		center_y = -(E/2);

		dx = x1 - center_x;
		dy = y1 - center_y;
		radius = sqrt(dx*dx + dy*dy);

		// 真正的绝对坐标
		center_x += p0.m_x;
		center_y += p0.m_y;

// 		if (sign > 0)	// 逆时针
// 		{
			dx = m_ShapePointList[0]->m_x - center_x;
			dy = m_ShapePointList[0]->m_y - center_y;
			start_angle = GeneralInterface::GetRadianFromXY(dx, dy);

			dx = m_ShapePointList[2]->m_x - center_x;
			dy = m_ShapePointList[2]->m_y - center_y;
			end_angle = GeneralInterface::GetRadianFromXY(dx, dy);
// 		}
// 		else			// 顺时针
// 		{
// 			dx = m_ShapePointList[0]->m_x - center_x;
// 			dy = m_ShapePointList[0]->m_y - center_y;
// 			start_angle = GeneralInterface::GetRadianFromXY(dx, dy);
// 
// 			dx = m_ShapePointList[2]->m_x - center_x;
// 			dy = m_ShapePointList[2]->m_y - center_y;
// 			end_angle = GeneralInterface::GetRadianFromXY(dx, dy);
// 		}



		// 返回值
		if (pCx != NULL)
			*pCx = center_x;	

		if (pCy != NULL)
			*pCy = center_y;	

		if (r != NULL)
			*r = radius;

		if (s_angle != NULL)
			*s_angle = start_angle;

		if (e_angle != NULL)
			*e_angle = end_angle;
	}

	return 0;
}

// 设置圆心半径
void Cad_Arc::SetCenterRadius(float pCx, float pCy, float r, float s_angle, float e_angle)
{
	m_CenterX = pCx;
	m_CenterY = pCy;
	m_radius = r;
	m_StartAngle = s_angle;
	m_EndAngle = e_angle;
}
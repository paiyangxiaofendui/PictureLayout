/*--------------------------------------------------------------------------------------------------------------------*/
//	Circle.cpp   -- 圆类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.7.19
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Circle.h"
#include "../CommonData.h"
#include "../Point/ShapePoint.h"


Cad_Circle::Cad_Circle()
{
	m_CenterX = 0;
	m_CenterY = 0;
	m_radius = 0;
}


Cad_Circle::~Cad_Circle()
{

}

// 计算轮廓线		
void Cad_Circle::CalOutlineList(void)		
{
	int i, num, dir;
	float x, y;
	float L, space, Radian, dRadian;
	FPoint *pNewPnt = NULL;

	// 清空轮廓线链表
	ClearOutlineList();

	space = OUTLINE_SPACE;	// 10mm为一个点
	dir = GetDirection();

	if (dir == Dir_CouterClockwise)	// 逆时针，微分弧为正
	{
		Radian = (float)(2*PI );
	}
	else	// 顺时针，微分弧为负
	{
		Radian = (float)(-2*PI );
	}

	L = abs(m_radius * Radian);

	num = (int)(L/space + 1);

	dRadian = Radian / num;

	// 第一个点
	pNewPnt = new FPoint;
	x = m_CenterX + m_radius *cos(0.0f);
	y = m_CenterY + m_radius *sin(0.0f);

	pNewPnt->SetXY(x, y);
	AddOutline(pNewPnt);
	pNewPnt = NULL;

	// 中间点
	for (i = 1; i < num-1; i++)
	{
		pNewPnt = new FPoint;
		x = m_CenterX + m_radius *cos(i * dRadian);
		y = m_CenterY + m_radius *sin(i * dRadian);

		pNewPnt->SetXY(x, y);
		AddOutline(pNewPnt);
		pNewPnt = NULL;
	}

	// 结尾点
	pNewPnt = new FPoint;
	*pNewPnt = *(GetHeadShapePoint());

	AddOutline(pNewPnt);
	pNewPnt = NULL;
}


// 获取顺逆时针方向
int Cad_Circle::GetDirection(void)
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


// 圆心半径起始结束角度，求三个形状点,dxf导入的都是逆时针，看是否超过360度
void Cad_Circle::CenterRadius2ShapePoint(void)
{
	float x, y, angle;
	ShapePoint* pPnt = NULL;

	// 第一个点
	angle = 0.0f;
	x = m_radius + m_CenterX;
	y = m_CenterY;

	pPnt = new ShapePoint(x, y);
	AddShapePoint(pPnt);

	// 第二个点
	angle = (float)(PI/2);
	x = m_CenterX;
	y = m_radius + m_CenterY;

	pPnt = new ShapePoint(x, y);
	AddShapePoint(pPnt);

	// 第三个点
	angle = (float)(PI);
	x = -m_radius + m_CenterX;
	y = m_CenterY;

	pPnt = new ShapePoint(x, y);
	AddShapePoint(pPnt);
}


// 计算圆心和半径 ，采用圆的一般公式 x^2+y^2+Dx+Ey+F=0 (D^2+E^2-4F>0) 三点代入方程求解
//（X+D/2）^2+（Y+E/2）^2=(D^2+E^2-4F)/4
int Cad_Circle::CalCenterRadius(float* pCx, float* pCy, float* r)
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
		return -1;
	}
	else				
	{
		// 代入求D的化简结果
		numerator = y1*(x2*x2 + y2*y2) - y2*(x1*x1 + y1*y1);
		denominator = x1*y2 - x2*y1;

		D = numerator/denominator;

		// 其中一个y有可能为0
		if (y1 != 0.0)
			E = -(x1*x1 + y1*y1 + D*x1)/y1;
		else
			E = -(x2*x2 + y2*y2 + D*x2)/y2;

		// 
		float dx, dy;
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

		// 返回值
		if (pCx != NULL)
			*pCx = center_x;	

		if (pCy != NULL)
			*pCy = center_y;	

		if (r != NULL)
			*r = radius;
	}

	return 0;
}

// 通过形状点计算圆心半径
void Cad_Circle::UpdateCenterRadius()
{
	int dir;
	float Cx, Cy, r;

	dir = GetDirection();
	CalCenterRadius(&Cx, &Cy, &r);

	m_CenterX = Cx;
	m_CenterY = Cy;
	m_radius = r;
}

// 设置圆心半径
void Cad_Circle::SetCenterRadius(float pCx, float pCy, float r)
{
	m_CenterX = pCx;
	m_CenterY = pCy;
	m_radius = r;
}

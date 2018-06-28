/*--------------------------------------------------------------------------------------------------------------------*/
//	ShapePoint.h      -- 形状点类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "ShapePoint.h"
#include "../CommonData.h"

// 构造
ShapePoint::ShapePoint()
{
	m_FigureType = FigureType_Unknow;		// 图形类型

	m_radius = 0;				// 圆弧的半径，直线点为0
	m_sign = 0;					// 0：直线点 1:圆弧起点 2：圆弧终点 3：两段圆弧连接点 -1：无需处理的点
	m_dir = 0;					// 0：直线点 1：逆时针 2：顺时针
	m_cut = 0;					// 0：不起刀 1：需要起刀 2：表示重新下刀
	m_type = 0;					// 0：常规 1：CNC锣铣（小刀旋转打大孔）
	m_group = Group_Regular;	// 0：常规(图形中的点) 1：图形开始  2：图形结束
	m_side = Side_Out;			// 1：外切  2：内切   3：中心切
	
	m_x = 0;
	m_y = 0;
}


// 构造
ShapePoint::ShapePoint(float x, float y)
{
	m_FigureType = FigureType_Unknow;		// 图形类型

	m_radius = 0;		// 圆弧的半径，直线点为0
	m_sign = 0;			// 0：直线点 1:圆弧起点 2：圆弧终点 3：两段圆弧连接点 -1：无需处理的点
	m_dir = 0;			// 0：直线点 1：逆时针 2：顺时针
	m_cut = 0;			// 0：不起刀 1：需要起刀 2：表示重新下刀
	m_type = 0;			// 0：常规 1：CNC锣铣（小刀旋转打大孔）
	m_group = Group_Regular;	// 0：常规(图形中的点) 1：图形开始  2：图形结束
	m_side = Side_Out;			// 1：外切  2：内切   3：中心切

	m_x = x;
	m_y = y;
}

// 析构
ShapePoint::~ShapePoint()
{

}


// +操作符重载
ShapePoint ShapePoint::operator+(ShapePoint& pnt)
{
	m_x += pnt.m_x;
	m_y += pnt.m_y;

	return *this;
}

// -操作符重载
ShapePoint ShapePoint::operator-(ShapePoint& pnt)			
{
	m_x -= pnt.m_x;
	m_y -= pnt.m_y;

	return *this;
}


// =操作符重载
ShapePoint& ShapePoint::operator = (const ShapePoint& pnt)
{
	m_x = pnt.m_x;
	m_y = pnt.m_y;
	m_PntID = pnt.m_PntID;	
	m_pFigure = pnt.m_pFigure;

	m_FigureType = pnt.m_FigureType;
	m_radius = pnt.m_radius;		
	m_sign = pnt.m_sign;			
	m_dir = pnt.m_dir;			
	m_cut = pnt.m_cut;			
	m_type = pnt.m_type;	
	m_group = pnt.m_group;			
	m_side = pnt.m_side;	

	return *this;
}
/*--------------------------------------------------------------------------------------------------------------------*/
//	ShapePoint.h      -- 形状点类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef		ShapePoint_H
#define		ShapePoint_H

#include "FPoint.h"

class Figure;

class AFX_EXT_CLASS ShapePoint : public FPoint
{
public:
	ShapePoint();
	ShapePoint(float x, float y);
	~ShapePoint();


public:
	// 操作符重载
	ShapePoint operator+(ShapePoint& pnt);			// +操作符重载
	ShapePoint operator-(ShapePoint& pnt);			// -操作符重载
	ShapePoint& operator = (const ShapePoint& pnt);	// =操作符重载


public:
	int m_FigureType;

public:
	float m_radius;		// 圆弧的半径，直线点为0
	int m_sign;			// 0：直线点 1:圆弧起点 2：圆弧终点 3：两段圆弧连接点 -1：无需处理的点
	int m_dir;			// 0：直线点 1：逆时针 2：顺时针
	int m_cut;			// 0：不起刀 1：需要起刀 2：表示重新下刀
	int m_type;			// 0：常规 1：CNC锣铣（小刀旋转打大孔）
	int m_group;		// 0：常规(图形中的点) 1：图形开始  2：图形结束
	int m_side;			// 1：外切  2：内切   3：中心切
};
#endif
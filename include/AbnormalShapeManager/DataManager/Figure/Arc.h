/*--------------------------------------------------------------------------------------------------------------------*/
//	Arc.h      -- 圆弧类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.12
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef	__Arc_H__
#define __Arc_H__

#include "Figure.h"

class AFX_EXT_CLASS Cad_Arc : public Figure
{
public:
	Cad_Arc();
	~Cad_Arc();


public:
	// 图形相关
	virtual void MoveFigure(float rel_x, float rel_y);					// 移动图形
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// 计算平行图形

public:
	// 形状点链表相关操作
	virtual void ReverseShapePointList(void);	// 反向形状点链表

public:
	// 轮廓线链表相关操作
	virtual void CalOutlineList(void);			// 计算轮廓线		

public:
	virtual void CalShapePointOutlineInfo();	// 计算形状点的轮廓信息

public:
	int GetDirection(void);						// 获取顺逆时针方向
	float GetCentralAngle(void);				// 获取圆心角

public:
	void CalCenterRadiusInfo();					// 通过形状点计算圆心半径信息，用于数据同步更新使用
	void CenterRadius2ShapePoint(void);			// 通过圆心半径计算形状点，一般在读取dxf信息后使用
	int CalCenterRadius(float* pCx, float* pCy, float* r, float* s_angle, float* e_angle);		// 通过形状点计算圆心半径
	void SetCenterRadius(float pCx, float pCy, float r, float s_angle, float e_angle);			// 设置圆心半径

public:
	float m_CenterX;
	float m_CenterY;
	float m_radius;
	float m_StartAngle;		// 第一个形状点的角度
	float m_EndAngle;		// 最后一个形状点的角度

};

#endif
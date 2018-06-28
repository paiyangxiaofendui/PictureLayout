/*--------------------------------------------------------------------------------------------------------------------*/
//	Circle.h   -- 圆类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.19
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef	__Circle_H__
#define __Circle_H__

#include "Figure.h"

class AFX_EXT_CLASS Cad_Circle : public Figure
{
public:
	Cad_Circle();
	~Cad_Circle();

public:
	// 轮廓线链表相关操作
	virtual void CalOutlineList(void);			// 计算轮廓线		

public:
	int GetDirection(void);						// 获取顺逆时针方向
	void UpdateCenterRadius();					// 通过形状点计算圆心半径
 	void CenterRadius2ShapePoint(void);			// 通过圆心半径计算形状点
 	int CalCenterRadius(float* pCx, float* pCy, float* r);		// 通过形状点计算圆心半径
	void SetCenterRadius(float pCx, float pCy, float r);

public:
	float m_CenterX;
	float m_CenterY;
	float m_radius;
};

#endif
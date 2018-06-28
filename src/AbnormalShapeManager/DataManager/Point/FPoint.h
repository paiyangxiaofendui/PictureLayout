/*--------------------------------------------------------------------------------------------------------------------*/
//	FPoint.h     -- 点基类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		FPoint_H
#define		FPoint_H

class Figure;

class AFX_EXT_CLASS FPoint
{
public:
	FPoint();
	FPoint(float x, float y);
	~FPoint();

public:
	void SetXY(float x, float y);		// 设置XY坐标
	void GetXY(float& x, float& y);		// 获取XY坐标

public:
	BOOL IsCloseTo(FPoint& pnt);		// 很靠近，近似重合
	BOOL IsCloseTo(FPoint* pPnt);		// 很靠近，近似重合
	float DistanceTo(FPoint& pnt);		// 距离点
	float DistanceTo(FPoint* pPnt);		// 距离点

public:
	// 操作符重载
	FPoint operator+(FPoint& pnt);			// +操作符重载
	FPoint operator-(FPoint& pnt);			// -操作符重载
	FPoint& operator = (const FPoint& pnt);	// =操作符重载

public:
	float m_x;
	float m_y;
	int	m_PntID;	

	Figure* m_pFigure;
};


#endif

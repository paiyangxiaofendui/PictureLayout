/*--------------------------------------------------------------------------------------------------------------------*/
//	Complex.h  -- 复合图形类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.12
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		__Complex_H__
#define		__Complex_H__

#include "Figure.h"

class AFX_EXT_CLASS Cad_Complex : public Figure
{
public:
	Cad_Complex();
	~Cad_Complex();

public:
	// 图形
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// 计算平行图形

public:
	// 形状点链表操作
	virtual void ReverseShapePointList(void);				// 反向形状点链表
	virtual void UpdateShapePointInfo(void);				// 更新所有形状点信息


public:
	virtual void CalShapePointOutlineInfo(void);			// 计算形状点轮廓线信息

public:
	void ClearComplexFigureList();		// 清空复合图形链表
	void CalComplexFigureList(void);	// 计算复合图形链表
	Figure* GetFigureFromShapePoint(int start_id, int end_id, int type);


	int AddFigure(Figure* pFigure);		// 添加一个图形
	void UpdateFigureInfo(void);		// 更新所有图形信息

public:
	// 轮廓线链表相关操作
	virtual void CalOutlineList(void);						// 计算轮廓线		


public:
	vector<Figure*> m_ComplexFigureList;

};

#endif
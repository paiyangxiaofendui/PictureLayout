/*--------------------------------------------------------------------------------------------------------------------*/
//	Figure.h     -- 图形类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		Figure_H
#define		Figure_H

#include <vector>
using namespace std;

class Layer;
class ShapePoint;
class FPoint;

class AFX_EXT_CLASS Figure
{
public:
	Figure();
	virtual ~Figure();

public:
	// 图形相关
	virtual void MoveFigure(float rel_x, float rel_y);		// 移动图形

	virtual BOOL IsClosed();				// 判断图形是否闭合

	virtual float CalFigureLen(void);		// 计算图形长度
	virtual BOOL CalFigureRect();			// 计算图形矩形
	virtual BOOL CalFigureRect(int &max_x, int& max_y, int& min_x, int& min_y);				// 计算图形矩形
	virtual BOOL CalFigureRect(float &max_x, float& max_y, float& min_x, float& min_y);		// 计算图形矩形
	virtual BOOL CalShapePointRect(float &max_x, float& max_y, float& min_x, float& min_y);	// 计算图形形状点矩形
	virtual BOOL CalOutlineRect(float &max_x, float& max_y, float& min_x, float& min_y);	// 计算图形轮廓点矩形

public:
	virtual void CalShapePointOutlineInfo(void);			// 计算形状点轮廓线信息
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// 计算平行形状点链表

public:
	// 形状点相关操作
	virtual int AddShapePoint(ShapePoint* pShapePoint);		// 添加一个形状点
	virtual int DeleteShapePoint(int ShapePoint_id);		// 删除一个形状点
	virtual ShapePoint* GetShapePoint(int ShapePoint_id);	// 获取形状点数量
	virtual int GetShapePointNum(void);						// 获取形状点数量
	virtual ShapePoint* GetHeadShapePoint(void);			// 获取第一个形状点
	virtual ShapePoint* GetTailShapePoint(void);			// 获取最后一个形状点
	virtual void UpdateShapePointInfo(void);				// 更新所有形状点信息
	
	// 形状点链表相关操作
	virtual void ReverseShapePointList(void);				// 反向形状点链表
	virtual void ClearShapePointList(void);					// 清空形状点链表
	


public:
	// 轮廓线相关操作
	virtual int AddOutline(FPoint* pPnt);					// 添加一个轮廓线
	virtual FPoint* GetOutline(int Point_id);				// 获取轮廓线数量
	virtual int DeleteOutline(int Pnt_id);					// 删除一个轮廓线
	virtual int GetOutlineNum(void);						// 获取轮廓线点数量
	virtual void UpdateOutlineInfo(void);					// 更新所有轮廓线信息

	// 轮廓线链表相关操作
	virtual void CalOutlineList(void);						// 计算轮廓线		
	virtual void ClearOutlineList(void);					// 清空轮廓线链表

public:
	int m_FigureID;			// 图形号
	int m_FigureType;		// 图形类型
	Layer* m_pLayer;		// 图层
	float m_OutlineSpace;	// 轮廓点间距

public:
	vector<ShapePoint*> m_ShapePointList;	// 形状点链表
	vector<FPoint*> m_OutlineList;			// 轮廓线链表

public:
	// 选中相关
	BOOL	m_bSelect;
	CRect	m_FigureRect;

	BOOL IsSelect(void);
	void Select(void);
	void UnSelect(void);

};	



#endif
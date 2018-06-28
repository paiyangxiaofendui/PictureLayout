/*--------------------------------------------------------------------------------------------------------------------*/
//	Layer.h     -- 图层类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		Layer_H
#define		Layer_H

#include <vector>
using namespace std;

class AFX_EXT_CLASS Figure;

class Layer
{
public:
	Layer();
	~Layer();

public:
	// 图形相关操作
	int AddFigure(Figure* pFigure);				// 添加一个图形
	int AddFigureToHead(Figure* pFigure);		// 添加一个图形在最前面
	int GetFigureNum(void);						// 添加一个图形
	Figure* GetFigure(int figure_id);			// 添加一个图形
	int DeleteFigure(int figure_id);			// 删除一个图形
	void UpdateFigureInfo(void);				// 更新所有图形信息

	void LinkFigureList(void);					// 连接图形

	void SortFigureListByArcFirst();			// 图形链表排序，圆弧优先
	void SortFigureListByRect();				// 图形链表排序 -- 按矩形大小	

public:
	int m_LayerID;

	vector<Figure*> m_FigureList;
};



#endif
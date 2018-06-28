/*--------------------------------------------------------------------------------------------------------------------*/
//	CSingleton.h    -- 单例类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	单例类
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		CSingleton_H
#define		CSingleton_H

#include <vector>
using namespace std;

class Layer;
class Figure;

class AFX_EXT_CLASS CAbnormalToolsSingleton
{
public:
	static CAbnormalToolsSingleton* GetSingleton(void);

public:
	// 图层相关操作
	int NewLayer(void);										// 新键一个图层
	int AddLayer(Layer* pLayer);							// 添加一个图层
	int DeleteLayer(int layer_id);							// 删除一个图层
	Layer* GetLayer(int layer_id);							// 获取图层
	int GetLayerNum(void);									// 获取图层数
	void UpdateLayerInfo(void);								// 更新所有图形信息

	void ClearLayerList(void);								// 清空图层链表
	void ClearAllData(void);								// 清空全部数据

public:
	// 对整体数据进行操作
	int GetFigureNum();										// 获取图形数
	void SortFigure();										// 图形排序
	void UpdateFigureInfo();								// 更新图形信息
	void LinkFigureList();									// 连接图层中的图形
	void CollectAllFigures(vector<Figure*>& FigureList);	// 收集所有的图形
	void CheckIfAllFiguresLegal(void);						// 检测是否所有图形都合法 删除非复合以及不闭合图形图形

public:
	vector<Layer*> m_LayerList;

public:
	// 界面选中图形

	vector<Figure*> m_SelectFigureList;
	vector<Figure*>* GetSelectFigureList();


	void SelectOneFigure(Figure* pFig);					// 选择一个图形
	void UnSelectOneFigure(Figure* pFig);				// 取消选择一个图形
	void SelectAllFigure(void);							// 选择全部图形
	void UnSelectAllFigure(void);						// 取消选择全部图形
	void MoveSelectFigure(float rel_x, float rel_y);	// 移动选中图形
	void DelSelectFigure(void);							// 删除选中图形


private:
	CAbnormalToolsSingleton();
	~CAbnormalToolsSingleton();
	static CAbnormalToolsSingleton* m_pSingleton;
};



#endif
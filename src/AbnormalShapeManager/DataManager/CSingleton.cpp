/*--------------------------------------------------------------------------------------------------------------------*/
//	CSingleton.cpp    -- 单例类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "CSingleton.h"
#include "Layer/Layer.h"
#include "Figure/Figure.h"
#include "../GeneralInterface/GeneralInterface.h"
#include <algorithm>
#include "CommonData.h"


// 单例实体初始化
CAbnormalToolsSingleton* CAbnormalToolsSingleton::m_pSingleton = NULL;

CAbnormalToolsSingleton* CAbnormalToolsSingleton::GetSingleton(void)
{
	if (m_pSingleton == NULL)
		m_pSingleton = new CAbnormalToolsSingleton;
	
	return m_pSingleton;
}

// 构造
CAbnormalToolsSingleton::CAbnormalToolsSingleton()
{

}

// 析构
CAbnormalToolsSingleton::~CAbnormalToolsSingleton()
{
	delete m_pSingleton;
}


// 图层相关操作

// 新键一个图层
int CAbnormalToolsSingleton::NewLayer(void)
{
	Layer* pNewLayer = new Layer;
	return AddLayer(pNewLayer);
}

// 添加一个图层
int CAbnormalToolsSingleton::AddLayer(Layer* pLayer)
{
	if (pLayer != NULL)
	{
		m_LayerList.push_back(pLayer);
		return 0;
	}
	
	return -1;
}

// 删除一个图层
int CAbnormalToolsSingleton::DeleteLayer(int layer_id)
{
	int nCount;
	vector<Layer*>::iterator it;

	// 数量判断
	nCount = m_LayerList.size();
	if (layer_id >= nCount)
		return -1;

	it = m_LayerList.begin() + layer_id;
	if (it != m_LayerList.end())
	{
		m_LayerList.erase(it);
		return 0;
	}

	return -1;
}

// 获取图层
Layer* CAbnormalToolsSingleton::GetLayer(int layer_id)
{
	int nCount;
	vector<Layer*>::iterator it;

	// 数量判断
	nCount = m_LayerList.size();
	if (layer_id >= nCount)
		return NULL;

	it = m_LayerList.begin() + layer_id;
	if (it != m_LayerList.end())
	{
		return *(it);
	}

	return NULL;
}

// 获取图层数
int CAbnormalToolsSingleton::GetLayerNum(void)
{
	return m_LayerList.size();
}

// 更新所有图形信息
void CAbnormalToolsSingleton::UpdateLayerInfo(void)
{
	int i, nCount;
	vector<Layer*>::iterator it;

	nCount = m_LayerList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_LayerList.begin() + i;
		(*it)->m_LayerID = i;
	}
}

// 清空图层链表
void CAbnormalToolsSingleton::ClearLayerList(void)
{
	GeneralInterface::ClearLayerList(&m_LayerList);
}

// 清空全部数据
void CAbnormalToolsSingleton::ClearAllData(void)		
{
	ClearLayerList();

	m_SelectFigureList.clear();
}



/*--------------------------------------------------------------------------------------*/
/*                                整体数据进行操作										*/
/*--------------------------------------------------------------------------------------*/

// 获取图形数
int CAbnormalToolsSingleton::GetFigureNum()
{
	int i, nLayerCount, nFigCount = 0;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		nFigCount += pLayer->GetFigureNum(); 
	}

	return nFigCount;
}


// 图形排序 用于图形连接前使用
void CAbnormalToolsSingleton::SortFigure()
{
	int i, nLayerCount;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		pLayer->SortFigureListByArcFirst(); 
	}

}

// 更新图形信息
void CAbnormalToolsSingleton::UpdateFigureInfo()
{
	int i, nLayerCount;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		pLayer->UpdateFigureInfo(); 
	}
}

// 连接图层中的图形
void CAbnormalToolsSingleton::LinkFigureList()
{
	int i, nLayerCount;

	nLayerCount = m_LayerList.size();
	for (i = 0; i < nLayerCount; i++)
	{
		Layer* pLayer = GetLayer(i);
		pLayer->LinkFigureList(); 
	}
}

// 收集所有的图形，准备写xml文件或返回轮廓点信息字符串，这里要根据矩形大小排序,矩形大的排在前面
void CAbnormalToolsSingleton::CollectAllFigures(vector<Figure*>& FigureList)
{
	int i_fig, i_layer, nLayerNum, nFigNum;

	nLayerNum = GetLayerNum();
	for (i_layer = 0; i_layer < nLayerNum; i_layer++)
	{
		Layer* pLayer = GetLayer(i_layer);

		pLayer->SortFigureListByRect();	// 排序，确保外围轮廓线在最前面

		nFigNum = pLayer->GetFigureNum();
		for (i_fig = 0; i_fig < nFigNum; i_fig++)
		{
			Figure* pFig = pLayer->GetFigure(i_fig);

			pFig->CalShapePointOutlineInfo();	// 计算轮廓线形状点信息

			FigureList.push_back(pFig);
		}
	}
}


// 检测是否所有图形都合法 删除非复合以及不闭合图形图形
void CAbnormalToolsSingleton::CheckIfAllFiguresLegal(void)
{
	int i_fig, i_layer, nLayerNum, nFigNum;

	nLayerNum = GetLayerNum();
	for (i_layer = 0; i_layer < nLayerNum; i_layer++)
	{
		Layer* pLayer = GetLayer(i_layer);

		nFigNum = pLayer->GetFigureNum();
		for (i_fig = 0; i_fig < nFigNum; i_fig++)
		{
			Figure* pFig = pLayer->GetFigure(i_fig);

			if (pFig->IsClosed() == FALSE)
			{
				// 不闭合
// 				pLayer->DeleteFigure(pFig->m_FigureID);
// 				i_fig = 0;
// 				nFigNum = pLayer->GetFigureNum();
				AfxMessageBox("检测到有不闭合的图形，将图形移到中间，用Ctrl+A全选图形，放大，查看小矩形，请检查修改dxf文件后重新导入，否则会出错！");
			}
			else if (pFig->m_FigureType != FigureType_Complex)
			{
				// 未连接的单独的图形
// 				pLayer->DeleteFigure(pFig->m_FigureID);
// 				i_fig = 0;
// 				nFigNum = pLayer->GetFigureNum();
				AfxMessageBox("检测到有不闭合的图形，将图形移到中间，用Ctrl+A全选图形，放大，查看小矩形，请检查修改dxf文件后重新导入，否则会出错！");
			}
			else if (pFig->CalFigureLen() < FIGURE_MIN_LEN)
			{
				AfxMessageBox("检测到有小于10mm的图形，将图形移到中间，用Ctrl+A全选图形，放大，查看小矩形，请检查是否真的需要，图形过小可能会导致出错！");
			}
		}
	}
}



/*--------------------------------------------------------------------------------------*/
/*                                图形选择相关操作										*/
/*--------------------------------------------------------------------------------------*/


/*----------------------------------------------------------*/
//	purpose:
//		获取选中图形链表
//		
//	param:
//		
//
//	return:
//		vector<Figure*>* -- 链表
/*----------------------------------------------------------*/
vector<Figure*>* CAbnormalToolsSingleton::GetSelectFigureList()
{
	return &m_SelectFigureList;
}

/*----------------------------------------------------------*/
//	purpose:
//		选择一个图形
//		
//	param:
//		Figure* pFig -- 图形指针
//
//	return:
//		void
/*----------------------------------------------------------*/
void CAbnormalToolsSingleton::SelectOneFigure(Figure* pFig)
{
	if (pFig != NULL)
	{
		pFig->Select();		// 选中图形

		// 未添加过，则添加进入选中图形链表
		vector<Figure*>::iterator result = std::find(m_SelectFigureList.begin(), m_SelectFigureList.end(), pFig); 
		if ( result == m_SelectFigureList.end( ))
			m_SelectFigureList.push_back(pFig);
	}
}

// 取消选择一个图形
void CAbnormalToolsSingleton::UnSelectOneFigure(Figure* pFig)
{
	if (pFig != NULL)
	{
		pFig->UnSelect();
 		vector<Figure*>::iterator result = std::find(m_SelectFigureList.begin(), m_SelectFigureList.end(), pFig); 

		if ( result != m_SelectFigureList.end( )) //找到
		{
			m_SelectFigureList.erase(result);
		}
	}
}

// 选择全部图形
void CAbnormalToolsSingleton::SelectAllFigure(void)
{
	int i, j, nLayerNum, nFigNum;
	Layer* pLayer = NULL;
	Figure* pFig = NULL;

	nLayerNum = GetLayerNum();
	for (i = 0; i < nLayerNum; i++)
	{
		pLayer = GetLayer(i);
		nFigNum = pLayer->GetFigureNum();

		for (j = 0; j < nFigNum; j++)
		{
			pFig = pLayer->GetFigure(j);
			SelectOneFigure(pFig);
		}
	}
}

// 取消选择全部图形
void CAbnormalToolsSingleton::UnSelectAllFigure(void)
{
	int i, j, nLayerNum, nFigNum;
	Layer* pLayer = NULL;
	Figure* pFig = NULL;

	nLayerNum = GetLayerNum();
	for (i = 0; i < nLayerNum; i++)
	{
		pLayer = GetLayer(i);
		nFigNum = pLayer->GetFigureNum();

		for (j = 0; j < nFigNum; j++)
		{
			pFig = pLayer->GetFigure(j);
			UnSelectOneFigure(pFig);
		}
	}

	m_SelectFigureList.clear();
}

// 移动选中图形
void CAbnormalToolsSingleton::MoveSelectFigure(float rel_x, float rel_y)
{
	int i, nFigNum;
	Figure* pFig = NULL;

	nFigNum = m_SelectFigureList.size();
	for (i = 0; i < nFigNum; i++)
	{
		pFig = m_SelectFigureList.at(i);
		pFig->MoveFigure(rel_x, rel_y);
		pFig->CalFigureRect();
	}
}


// 删除选中图形
void CAbnormalToolsSingleton::DelSelectFigure(void)
{
	int i, nFigNum;
	Figure* pFig = NULL;
	Layer* pLayer = NULL;

	nFigNum = m_SelectFigureList.size();
	for (i = 0; i < nFigNum; i++)
	{
		pFig = m_SelectFigureList.at(i);
		pLayer = pFig->m_pLayer;

		pLayer->DeleteFigure(pFig->m_FigureID);
		pLayer->UpdateFigureInfo();
	}
}
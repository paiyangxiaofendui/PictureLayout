/*--------------------------------------------------------------------------------------------------------------------*/
//	CSingleton.cpp    -- 单例类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Layer.h"

#include "../Figure/Figure.h"
#include "../../GeneralInterface/GeneralInterface.h"
#include "../Point/ShapePoint.h"
#include "../CommonData.h"

Layer::Layer()
{
	m_LayerID = 0;
}

Layer::~Layer()
{

}

	
// 图形相关操作

// 添加一个图形
int Layer::AddFigure(Figure* pFigure)
{
	if (pFigure != NULL)
	{
		m_FigureList.push_back(pFigure);
		UpdateFigureInfo();
		return 0;
	}

	return -1;
}


// 添加一个图形在最前面
int Layer::AddFigureToHead(Figure* pFigure)
{
	if (pFigure != NULL)
	{
		m_FigureList.insert(m_FigureList.begin(), pFigure);
		UpdateFigureInfo();
		return 0;
	}

	return -1;
}

// 添加一个图形
int Layer::GetFigureNum(void)
{
	return m_FigureList.size();
}

// 获取一个图形
Figure* Layer::GetFigure(int figure_id)
{
	int nCount;
	vector<Figure*>::iterator it;

	// 数量判断
	nCount = m_FigureList.size();
	if (figure_id >= nCount)
		return NULL;

	it = m_FigureList.begin() + figure_id;
	if (it != m_FigureList.end())
	{
		return *(it);
	}

	return NULL;
}


// 删除一个图形
int Layer::DeleteFigure(int figure_id)
{
	int nCount;
	vector<Figure*>::iterator it;

	// 数量判断
	nCount = m_FigureList.size();
	if (figure_id >= nCount)
		return -1;

	it = m_FigureList.begin() + figure_id;
	if (it != m_FigureList.end())
	{
		// 释放内存
		delete (*it);

		m_FigureList.erase(it);
		UpdateFigureInfo();			// 更新图形信息

		return 0;
	}


	return -1;
}

// 更新所有图形信息
void Layer::UpdateFigureInfo(void)
{
	int i, nCount;
	vector<Figure*>::iterator it;

	nCount = m_FigureList.size();
	for (i = 0; i < nCount; i++)
	{
		it = m_FigureList.begin() + i;
		(*it)->m_FigureID = i;				// 更新图形ID
		(*it)->m_pLayer = this;				// 更新图层指针

		(*it)->CalFigureRect();				// 计算图形矩形
	}


}

// 连接图形
void Layer::LinkFigureList(void)
{

// 循环标签
loop_label:
	int i = 0;
	int j = 0;
	int nCount = GetFigureNum();

	for (; i < nCount-1; i++)
	{
		for (j = i+1; j < nCount; j++)
		{
			Figure* pSrc, *pDst;
			
			pSrc = GetFigure(i);
			pDst = GetFigure(j);

			if (GeneralInterface::LinkFigure(pSrc, pDst) == 0)
				goto loop_label;
		}
	}
}

// 图形排序，圆弧优先
void Layer::SortFigureListByArcFirst()
{
	int i, nCount;
	Figure* pFig;
	vector<Figure*>  tmp_list;

	nCount = GetFigureNum();
	for (i = 0; i < nCount; i++)
	{
		pFig = GetFigure(i);

		if (pFig->m_FigureType == FigureType_Arc)
			tmp_list.insert(tmp_list.begin(), pFig);
		else
			tmp_list.push_back(pFig);
	}

	m_FigureList.clear();
	m_FigureList.insert(m_FigureList.begin(),tmp_list.begin(), tmp_list.end());

	UpdateFigureInfo();
}

// 图形链表排序 -- 按矩形大小	
void Layer::SortFigureListByRect()
{
	GeneralInterface::SortFigureListByRect(&m_FigureList);
	UpdateFigureInfo();
}
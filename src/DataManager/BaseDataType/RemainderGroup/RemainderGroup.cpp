/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderGroup.cpp      -- 余料组类实现文件
//	
//	作者：	yuanzb
//	时间：	2017.9.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "RemainderGroup.h"



RemainderGroup::RemainderGroup(void)
{
	m_Thickness = 0;			
}


RemainderGroup::~RemainderGroup(void)
{
	// 清空大板链表，释放大板内存
	for (int j = 0; j < m_RemainderItemList.size(); j++)
	{
		RemainderItem* pItem =  m_RemainderItemList.at(j);
		delete pItem;
	}

	m_RemainderItemList.clear();
}


/*---------------------------------------*/
//	函数说明：
//		添加余料
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void RemainderGroup::AddRemainder(RemainderItem* pAddItem)
{
	m_RemainderItemList.push_back(pAddItem);

	UpdateRemainderItemID();

}

/*---------------------------------------*/
//	函数说明：
//		更新信息
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void RemainderGroup::UpdateRemainderItemID()
{
	int nCount = m_RemainderItemList.size();

	for (int i = 0; i < nCount; i++)
	{
		RemainderItem* pItem = m_RemainderItemList.at(i);

		pItem->m_GroupID = m_GroupID;
		pItem->m_ItemID = i;
	}
}

/*---------------------------------------*/
//	函数说明：
//		删除一个余料项
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void RemainderGroup::DelRemainder(int id)
{
	RemainderItem* pitem = m_RemainderItemList.at(id);
	delete pitem;

	m_RemainderItemList.erase(m_RemainderItemList.begin() + id);

	UpdateRemainderItemID();

}

iterator
iterator
iterator
iterator
/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderManager.cpp      -- 余料管理器类实现文件
//	
//	作者：	yuanzb
//	时间：	2017.9.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "RemainderManager.h"



RemainderManager::RemainderManager(void)
{			

}


RemainderManager::~RemainderManager(void)
{

}


/*---------------------------------------*/
//	函数说明：
//		更新余料组ID
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
void RemainderManager::UpdateRemainderGroupID()
{
	int nCount = m_RemainderGroupList.size();

	for (int i = 0; i < nCount; i++)
	{
		RemainderGroup* pGroup = m_RemainderGroupList.at(i);

		pGroup->m_GroupID = i;
		pGroup->UpdateRemainderItemID();

	}
}


/*---------------------------------------*/
//	函数说明：
//		添加余料组
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
void RemainderManager::AddRemainderGroup(RemainderGroup* pAddGroup)
{
	m_RemainderGroupList.push_back(pAddGroup);
	UpdateRemainderGroupID();
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
void RemainderManager::AddRemainderItem(RemainderItem* pAddItem)
{
	// 保护
	if (pAddItem == NULL)
	{
		return;
	}

	RemainderItem* pItem = FindRemainder(pAddItem->m_Material, pAddItem->m_Thickness);

	if (pItem != NULL)
	{
		// 找到，插入
		int group_id = pItem->m_GroupID;

		RemainderGroup* pGroup = m_RemainderGroupList.at(group_id);
		pGroup->AddRemainder(pAddItem);
	}
	else
	{
		//未找到，新建

		RemainderGroup* pNewGroup = new RemainderGroup;

		pNewGroup->m_Material = pAddItem->m_Material;
		pNewGroup->m_Thickness = pAddItem->m_Thickness;

		AddRemainderGroup(pNewGroup);
		pNewGroup->AddRemainder(pAddItem);

	}

	UpdateRemainderGroupID();

}

/*---------------------------------------*/
//	函数说明：
//		减少一个余料
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
void RemainderManager::SubRemainderItem(RemainderItem* pDelItem)
{
	if (pDelItem->m_nCount > 1)	// 有多
	{
		pDelItem->m_nCount--;
	}
	else	// 只剩下一件
	{
		int group_id = pDelItem->m_GroupID;
		int item_id = pDelItem->m_ItemID;

		RemainderGroup* pGroup = m_RemainderGroupList.at(group_id);

		pGroup->DelRemainder(item_id);

		if (pGroup->m_RemainderItemList.size() == 0)
		{
			DelRemainderGroup(group_id);
		}
	}

	UpdateRemainderGroupID();
}


/*---------------------------------------*/
//	函数说明：
//
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
void RemainderManager::DelRemainderGroup(int id)
{
	RemainderGroup* pGroup = m_RemainderGroupList.at(id);
	delete pGroup;

	m_RemainderGroupList.erase(m_RemainderGroupList.begin() + id);


	UpdateRemainderGroupID();
}


/*---------------------------------------*/
//	函数说明：
//		寻找余料
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
RemainderItem* RemainderManager::FindRemainder(CString Material, float Thickness)
{
	vector<RemainderGroup*>::iterator it_group;

	for (it_group = m_RemainderGroupList.begin(); it_group != m_RemainderGroupList.end(); it_group++)
	{
		RemainderGroup* pGroup = *it_group;

		if (pGroup->m_Material == Material && pGroup->m_Thickness == Thickness)
		{


			return pGroup->m_RemainderItemList.at(0);
		}
	}

	return NULL;

}



// 获取
void RemainderManager::GetAllItems(vector<RemainderItem*>& item_list)
{
	for (vector<RemainderGroup*>::iterator it_group = m_RemainderGroupList.begin(); it_group != m_RemainderGroupList.end(); it_group++)
	{
		RemainderGroup* pGroup = *it_group;

		for (vector<RemainderItem*>::iterator it_item = pGroup->m_RemainderItemList.begin(); it_item != pGroup->m_RemainderItemList.end(); it_item++)
		{
			item_list.push_back(*it_item);
		}
	}
}
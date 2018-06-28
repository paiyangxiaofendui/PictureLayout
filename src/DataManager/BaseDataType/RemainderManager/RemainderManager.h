/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderManager.h      -- 余料管理器类声明文件
//	
//	作者：	yuanzb
//	时间：	2017.9.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_RemainderManager_H
#define		_RemainderManager_H

#include <vector>
#include "../RemainderGroup/RemainderGroup.h"
#include "../RemainderItem/RemainderItem.h"

using namespace std;


class AFX_EXT_CLASS RemainderManager
{
public:
	RemainderManager(void);
	virtual ~RemainderManager(void);

public:
	// 接口
	void AddRemainderItem(RemainderItem* pAddItem);
	void AddRemainderGroup(RemainderGroup* pAddGroup);
	void SubRemainderItem(RemainderItem* pDelItem);
	void DelRemainderGroup(int id);
	RemainderItem* FindRemainder(CString Material, float Thickness);
	void UpdateRemainderGroupID();

	void GetAllItems(vector<RemainderItem*>& item_list);



public:
	vector<RemainderGroup*>	m_RemainderGroupList;			// 余料组列表

};


#endif

/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderGroup.h      -- 余料组类声明文件
//	
//	作者：	yuanzb
//	时间：	2017.9.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_RemainderGroup_H
#define		_RemainderGroup_H

#include <vector>
#include "../RemainderItem/RemainderItem.h"

using namespace std;


class AFX_EXT_CLASS RemainderGroup
{
public:
	RemainderGroup(void);
	virtual ~RemainderGroup(void);

public:
	// 公共接口

	void AddRemainder(RemainderItem* pAddItem);
	void DelRemainder(int id);
	void UpdateRemainderItemID();


public:
	int m_GroupID;
	float	m_Thickness;							//	厚
	CString m_Material;								//	材质
	vector<RemainderItem*>	m_RemainderItemList;	//	余料项列表




};


#endif

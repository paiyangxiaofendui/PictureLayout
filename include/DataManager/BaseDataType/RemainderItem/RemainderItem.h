/*--------------------------------------------------------------------------------------------------------------------*/
//	RemainderItem.h      -- 余料项类声明文件
//	
//	作者：	yuanzb
//	时间：	2017.9.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_RemainderItem_H
#define		_RemainderItem_H


class Component;


class AFX_EXT_CLASS RemainderItem
{
public:
	RemainderItem(void);
	virtual ~RemainderItem(void);


public:
	int m_GroupID;			// 余料组ID
	int m_ItemID;		// 余料ID

	CString m_Material;		// 材质
	float m_Thickness;		// 厚
	float m_Length;			// 长		
	float m_Width;			// 宽		
	int m_nCount;			// 数量
	
};


#endif

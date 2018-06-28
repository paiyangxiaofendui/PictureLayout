/*--------------------------------------------------------------------------------------------------------------------*/
//	Node.h      -- 点基类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Node_H
#define		_Node_H

#include <vector>
using namespace std;

class AFX_EXT_CLASS Node
{
public:
	Node(void);
	virtual ~Node(void);



public:
	// 接口
	virtual void AddChild(Node* pNode);									// 添加子节点
	virtual void GetAllLeafNodes(vector<Node*>& list, int type);		// 获取该类型的节点
	virtual Node* CopyNode(void);										// 拷贝节点

	virtual void DeleteTree(Node** ppRoot);								// 释放树
	virtual void DeleteAllChildren(Node* pRoot);						// 释放所有节点，但保留根节点

public:
	int m_type;							// 0：余料（子节点）  1：要开的小板（子节点）   2：组合板（父节点）
	Node* m_pParent;					// 父节点指针
	vector<Node*> m_ChildrenList;		// 子节点指针链表
	
};


#endif

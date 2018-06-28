/*--------------------------------------------------------------------------------------------------------------------*/
//	Node.cpp     -- 点基类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Node.h"



Node::Node(void)
{
	m_pParent = NULL;
}


Node::~Node(void)
{

}


// 添加子节点
void Node::AddChild(Node* pNode)
{
	if (pNode != NULL)
	{
		m_ChildrenList.push_back(pNode);
		pNode->m_pParent = this;
	}
}

//获取该类型的节点
void Node::GetAllLeafNodes(vector<Node*>& list, int type)
{
	int i, nCount = m_ChildrenList.size();

	// 先根遍历 从叶子加点开始遍历
	if (m_type == type)
	{
		list.push_back(this);
	}


	for(i = 0; i < nCount; i++)
	{
		Node* pNode = m_ChildrenList.at(i);
		pNode->GetAllLeafNodes(list, type);

	}

	return;
}


// 拷贝节点
Node* Node::CopyNode(void)
{
	Node* pNode = new Node;

	return pNode;
}


// 后根遍历，释放树
void Node::DeleteTree(Node** ppRoot)
{
	if (ppRoot != NULL)
	{
		Node* pRoot = *ppRoot;
		if (pRoot != NULL)
		{
			int i, nCount = pRoot->m_ChildrenList.size();

			// 递归释放子节点
			for(i = 0; i < nCount; i++)
			{
				Node* pNode = pRoot->m_ChildrenList.at(i);
				DeleteTree(&pNode);
			}

			delete pRoot;
			*ppRoot = NULL;
		}
	}
}

// 释放所有节点，但保留根节点
void Node::DeleteAllChildren(Node* pRoot)
{
	int i, nCount;

	nCount = pRoot->m_ChildrenList.size();
	for (i = 0; i < nCount; i++)
	{
		Node* pNode = pRoot->m_ChildrenList.at(i);

		DeleteTree(&pNode);
	}

	pRoot->m_ChildrenList.clear();
}


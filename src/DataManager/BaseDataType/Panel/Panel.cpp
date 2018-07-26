/*--------------------------------------------------------------------------------------------------------------------*/
//	Panel.cpp      -- 大板类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.11.9
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Panel.h"
#include <algorithm>


#include "../CommonData/CommonData.h"
#include "../../Algorithm/Algorithm.h"
#include "../CSingleton/CSingleton.h"

using namespace std;

// 构造
Panel::Panel(void)
{
	m_PanelType = PanelType_RawMaterial;
	m_OrgLen = 0;  
	m_OrgWidth = 0;  

	m_OriginX = 0;  
	m_OriginY = 0;   
	m_type = NodeType_Remainder;		// 初始化为余料
}

// 构造
Panel::Panel(Panel& panel)
{
	// panel
	m_PanelType = panel.m_PanelType ;
	m_OrgLen	= panel.m_OrgLen	;
	m_OrgWidth	= panel.m_OrgWidth	;
	m_OriginX	= panel.m_OriginX	;
	m_OriginY	= panel.m_OriginY	;


	// component
	m_type		= NodeType_Remainder;
	m_RealLength = panel.m_RealLength;
	m_RealWidth = panel.m_RealWidth;
	m_Thickness = panel.m_Thickness;
	m_Material = panel.m_Material;
	
	m_PanelID =  panel.m_PanelID;
	m_x =  panel.m_x;
	m_y =  panel.m_y;


}


// 析构
Panel::~Panel(void)
{

}

// 获取大板利用率
float Panel::GetUtilization(void)
{
	float utilization;
	float panel_area = 0, cpn_area = 0, remain_area = 0;
	vector<Node*> RemainderList;

	panel_area = m_OrgLen*m_OrgWidth;//GetArea();

	//GetAllLeafNodes(RemainderList, NodeType_Remainder);

// 	for (vector<Node*>::iterator it = RemainderList.begin(); it != RemainderList.end(); it++)
// 	{
// 		Component* pRemainder = static_cast<Component*>(*it);
// 		remain_area += pRemainder->GetArea();
// 	}
// 
// 	utilization = 1.0f - remain_area/panel_area;


	GetAllLeafNodes(RemainderList, NodeType_NeededComponent);

	for (vector<Node*>::iterator it = RemainderList.begin(); it != RemainderList.end(); it++)
	{
		Component* pCpn = static_cast<Component*>(*it);
		cpn_area += pCpn->GetArea();
	}

	// 加上修边
	CSingleton* pSingleton = CSingleton::GetSingleton();
// 	float DeburringWidth = pSingleton->m_BaseInfo.m_DeburringWidth;
// 	float DeburringArea = m_OrgLen*DeburringWidth + (m_OrgWidth-DeburringWidth)*DeburringWidth;


	utilization = (cpn_area /*+ DeburringArea*/) / panel_area;


	return utilization;
}

// 获取小板数量
int Panel::GetComponentNum(void)
{
	vector<Node*> ComponentList;
	GetAllLeafNodes(ComponentList, NodeType_NeededComponent);

	return ComponentList.size();
}

/*---------------------------------------*/
//	函数说明：
//		获取小板
//
//
//	参数：
//
//
//	返回值:
//
//
/*---------------------------------------*/
Component* Panel::GetComponent(int NumInPanel)
{
	int nCount;
	vector<Node*> node_list;
	GetAllLeafNodes(node_list, NodeType_NeededComponent);

	nCount = node_list.size();
	for(int i = 0; i < nCount; i++)
	{
		Component* pCpn = static_cast<Component*>(node_list.at(i));

		if (pCpn->m_NumberInPanel == NumInPanel)
		{
			return pCpn;
		}
	}

	return NULL;
}



// 距离中心点从远到近排序
bool  ComponentCompareCenterFarer(const Component* pfirst, const Component* psecond) 
{                 
// 	Component* p1 = const_cast<Component*>(pfirst);
// 	Component* p2 = const_cast<Component*>(psecond);
// 
// 
// 	return p1->m_fDist2PanelCenter < p2->m_fDist2PanelCenter;	
	return true;
}


//  获取所有的小板
void Panel::GetAllNeededComponent_Sorted(vector<Component*>& cpn_list)
{
	// 初始化基础信息
	CSingleton* pSingleton = CSingleton::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;

	vector<Node*> node_list;
	vector<Component*> small_cpn_list;
	BOOL meet_large_cpn = FALSE;

	// 找到最大的小板，决定小板的长宽
	int min_len, min_width;
	Component* pLargestCpn = GetLargestNeedComponent();

	// 没有小板
	if (pLargestCpn == NULL)
	{
		return; 
	}

	min_len = pLargestCpn->m_RealLength <	300 ?    pLargestCpn->m_RealLength :  300;
	min_width = pLargestCpn->m_RealWidth < 300 ?    pLargestCpn->m_RealWidth :  300;




	// 先序收集子节点
	GetAllLeafNodes(node_list, NodeType_NeededComponent);


	// 从前往后找小板，把小板逆序放到最后面
	int i, nCount = node_list.size();
	for (i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(node_list.at(i));

		if (meet_large_cpn == FALSE &&
			(pNode->m_RealLength < min_len || pNode->m_RealWidth < min_width))
		{
			small_cpn_list.push_back(pNode);
		}
		else
		{
			meet_large_cpn = TRUE;
			cpn_list.push_back(pNode);
		}
	}

	
	cpn_list.insert(cpn_list.end(), small_cpn_list.rbegin(), small_cpn_list.rend());


	// 对小板根据离大板中心点远近进行排序

// 	m_fCenterX = m_OrgLen/2.0;
// 	m_fCenterY = m_OrgWidth/2.0;
// 
// 	nCount = small_cpn_list.size();
// 	for (i = 0; i < nCount; i++)
// 	{
// 		Component* pNode = small_cpn_list.at(i);
// 
// 		pNode->m_fCenterX = pNode->m_x + pNode->m_RealLength/2.0;
// 		pNode->m_fCenterY = pNode->m_y + pNode->m_RealWidth/2.0;
// 
// 		float dx = pNode->m_fCenterX - m_fCenterX;
// 		float dy = pNode->m_fCenterY - m_fCenterY;
// 
// 		pNode->m_fDist2PanelCenter = sqrt(dx*dx + dy*dy);
// 	}
// 
// 	sort(small_cpn_list.begin(), small_cpn_list.end(), ComponentCompareCenterFarer);


//	cpn_list.insert(cpn_list.end(), small_cpn_list.begin(), small_cpn_list.end());

}





/*---------------------------------------*/
//	函数说明：
//		面积大优先  获取所有的小板
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
void Panel::GetAllNeedComponent_AreaLargeFirst(vector<Component*>& cpn_list)
{

	vector<Component*> small_cpn_list;
	vector<Component*> node_list;

	ALGORITHM_API::TravelNeedComponent_AreaLargeFirst(this, node_list);

	int i, nCount = node_list.size();
	for (i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(node_list.at(i));

		cpn_list.push_back(pNode);
	}




	return;
}


// 获取所有的余料
// void Panel::GetAllRemainder(vector<Component*>& cpn_list)
// {
// 	vector<Node*> node_list;
// 
// 	GetAllLeafNodes(node_list, NodeType_Remainder);
// 
// 	int i, nCount = node_list.size();
// 	for (i = 0; i < nCount; i++)
// 	{
// 		Component* pNode = static_cast<Component*>(node_list.at(i));
// 
// 		cpn_list.push_back(pNode);
// 	}
// }


// 将大板还原为
void Panel::Recover(vector<Component*>& cpn_list)
{
	vector<Node*> node_list;

	GetAllLeafNodes(node_list, NodeType_NeededComponent);

	// 收集小板，切断与大板的联系
	int i, nCount = node_list.size();
	for (i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(node_list.at(i));
		Node* pParent = pNode->m_pParent;
		
		pNode->m_pParent = NULL;
		cpn_list.push_back(pNode);

		//vector<Node*>::iterator it = find(pParent->m_ChildrenList.begin(), pParent->m_ChildrenList.end(), pNode);
		vector<Node*>::iterator it = pParent->m_ChildrenList.begin();
		pParent->m_ChildrenList.erase(it);

	}

	//释放内存
	DeleteAllChildren(this);

	// 初始的
	m_type = NodeType_Remainder;
	m_ChildrenList.clear();

}


/*---------------------------------------*/
//	函数说明：
//		将小板还原
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
void Panel::RecoverOneComponent(Component* pCpn)
{
	Node* pParent = pCpn->m_pParent;

	pCpn->m_pParent = NULL;

	//vector<Node*>::iterator it = find(pParent->m_ChildrenList.begin(), pParent->m_ChildrenList.end(), pCpn);
	vector<Node*>::iterator it = pParent->m_ChildrenList.begin();
	pParent->m_ChildrenList.erase(it);
}

/*---------------------------------------*/
//	函数说明：
//		是否比另一块大板好
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
bool Panel::IsBetterThan(Panel* pDstPanel)
{
	if(pDstPanel != NULL)
	{
		Component* pCurLargestRemainder = this->GetLargestRemainder();
		Component* pDstLargestRemainder = pDstPanel->GetLargestRemainder();

		return pCurLargestRemainder->IsAreaLargeThan(*pDstLargestRemainder);
	}

	return false;
}


/*---------------------------------------*/
//	函数说明：
//		是否有反面信息
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
bool  Panel::HaveDownerFaceInfo(void)
{
	vector<Node*> node_list;

	// 获取所有小板, 先序收集子节点
	GetAllLeafNodes(node_list, NodeType_NeededComponent);

	// 依次判断是否有反面信息
	int nCount = node_list.size();

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = node_list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		if (pCpn->HaveDownerFaceInfo() == true)
		{
			return true;
		}
	}

	return false;
}

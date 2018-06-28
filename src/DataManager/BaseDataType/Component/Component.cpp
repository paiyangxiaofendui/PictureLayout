/*--------------------------------------------------------------------------------------------------------------------*/
//	Component.cpp       -- 小板类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.11.9
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "Component.h"
#include "../CommonData/CommonData.h"
#include <algorithm>

using namespace std;

enum
{
	Length_First,
	Width_First,
	Area_First,
	Special_1
};


#define  Component_Sort_Priority		Length_First

Component::Component(void)
{
	m_CpnID = 0;				
	m_NumberInPanel = 0;	
	m_BarCode = "";			
	m_strCabinetName = "";	
	m_strComponentName = "";	
	m_Material = "";			
	m_Texture = TextureType_NO_TEXTURE;			
	m_Thickness = 0;		
	m_RealLength = 0;		
	m_RealWidth = 0;		
	m_x = 0;				
	m_y = 0;
	m_fXLabelCenter = 0;
	m_fYLabelCenter = 0;

	m_type = NodeType_Remainder;

	//m_bRotated = false;
	m_nRotatedAngle = 0;
	m_nOtherShapeID = 0;
	m_nKnifeDownPos = 0;

	m_aHasSideHole[0] = m_aHasSideHole[1] = m_aHasSideHole[2] = m_aHasSideHole[3] = false;

// 	m_fDist2PanelCenter = 0;
// 	m_fCenterX = 0;
// 	m_fCenterY = 0;
}

Component::~Component(void)
{

}

// 拷贝节点
Node* Component::CopyNode(void)
{
	Component* pCpn = new Component;

	// Node信息
	pCpn->m_type					= this->m_type;								// 0：余料（子节点）  1：要开的小板（子节点）   2：组合板（父节点）

	// Component信息
	pCpn->m_CpnID					= this->m_CpnID				;				// 板件ID		唯一标识， 为了解决小板切多次问题
	pCpn->m_NumberInPanel			= this->m_NumberInPanel		;				// 板内序号		第几块排的小板
	pCpn->m_BarCode					= this->m_BarCode			;				// 条码
	pCpn->m_strCabinetName			= this->m_strCabinetName	;				// 柜体名称
	pCpn->m_strComponentName		= this->m_strComponentName	;				// 板件名称
	pCpn->m_Material				= this->m_Material			;				// 材料
	pCpn->m_Texture					= this->m_Texture			;				// 纹理			0：无纹理  1：横纹 2：竖纹	
	pCpn->m_Thickness				= this->m_Thickness			;				// 厚度
	pCpn->m_RealLength				= this->m_RealLength		;				// 真实长
	pCpn->m_RealWidth				= this->m_RealWidth			;				// 真实宽
	pCpn->m_x						= this->m_x					;				// x坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
	pCpn->m_y						= this->m_y					;				// y坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
	pCpn->m_fXLabelCenter           = this->m_fXLabelCenter;                    // 标签中点x坐标 相对大板的原点，直角坐标系，第一象限，左下角为原点
	pCpn->m_fYLabelCenter           = this->m_fYLabelCenter;                    // 标签中点y坐标 相对大板的原点，直角坐标系，第一象限，左下角为原点

	pCpn->m_nRotatedAngle			= this->m_nRotatedAngle		;				// 旋转角度
	pCpn->m_nOtherShapeID			= this->m_nOtherShapeID		;				// 异形ID
	pCpn->m_vOutlinePoint			= this->m_vOutlinePoint		;				// 轮廓点信息
	pCpn->m_nKnifeDownPos			= this->m_nKnifeDownPos		;				// 下刀点
	pCpn->m_vUpperFaceHole			= this->m_vUpperFaceHole	;				// 上表面孔
	pCpn->m_vUpperFaceSlot			= this->m_vUpperFaceSlot	;				// 上表面槽
	pCpn->m_vDownerFaceHole			= this->m_vDownerFaceHole	;				// 下表面孔
	pCpn->m_vDownerFaceSlot			= this->m_vDownerFaceSlot	;				// 下表面槽


	pCpn->m_strOrderID				= this->m_strOrderID			;           // 订单号
	pCpn->m_strCabinetID			= this->m_strCabinetID			;			// 柜号

	//pCpn->m_afBanding[4]			= this->m_afBanding[4]			;           // 封边
	memcpy(pCpn->m_afBanding, this->m_afBanding, sizeof(this->m_afBanding));	   

	pCpn->m_bSlotFlipped			= this->m_bSlotFlipped			;           // 槽翻转
	pCpn->m_bVHoleFlipped			= this->m_bVHoleFlipped			;           // 孔翻转
	pCpn->m_strCustomerInfo			= this->m_strCustomerInfo		;			// 客户信息
	pCpn->m_strJoinedStore			= this->m_strJoinedStore		;			// 加盟店
	pCpn->m_strSlottingFlag			= this->m_strSlottingFlag		;			// 拉槽标识
	pCpn->m_strReminder				= this->m_strReminder			;           // 助记号
	pCpn->m_strDrilling				= this->m_strDrilling			;           // 钻孔
	pCpn->m_strOrderType			= this->m_strOrderType			;			// 订单类型
	pCpn->m_strReverseSideBarcode	= this->m_strReverseSideBarcode	;			// 反面条码
	pCpn->m_fProductLength			= this->m_fProductLength		;			// 成品长度
	pCpn->m_fProductWidth			= this->m_fProductWidth			;			// 成品宽度
	pCpn->m_fProductThickness		= this->m_fProductThickness		;			// 成品厚度
	pCpn->m_strCustomerAddress		= this->m_strCustomerAddress	;			// 客户地址
	pCpn->m_strHoleSlotFlag			= this->m_strHoleSlotFlag		;			// 钻槽标识




	return pCpn;
}


// 能否包含节点
bool Component::Containable(Component* pCpn)
{
	float cur_len = m_RealLength;
	float cur_width = m_RealWidth;

	if (pCpn != NULL)
	{
		if (cur_len >= pCpn->m_RealLength && cur_width >= pCpn->m_RealWidth)
		{
			return true;
		}
	}

	return false;
}

// 获取所有的余料
 void Component::GetAllRemainder(vector<Component*>& list)
 {
	 vector<Node*> tmp_list;
	 GetAllLeafNodes(tmp_list, NodeType_Remainder);	

	 int nCount = tmp_list.size();

	 for(int i = 0; i < nCount; i++)
	 {
		 Node* pNode = tmp_list.at(i);
		 Component* pCpn = dynamic_cast<Component*>(pNode);

		 list.push_back(pCpn);
	 }

 }



 // 获取所有面积大于area的余料
 void Component::GetAllRemainder(vector<Component*>& list, float area, float min_len)
 {
	 vector<Node*> tmp_list;
	 GetAllLeafNodes(tmp_list, NodeType_Remainder);	

	 int nCount = tmp_list.size();

	 for(int i = 0; i < nCount; i++)
	 {
		 Node* pNode = tmp_list.at(i);
		 Component* pCpn = dynamic_cast<Component*>(pNode);

		 float cur_area = pCpn->m_RealLength*pCpn->m_RealWidth;
		 if ((cur_area > area)				&&
			 (pCpn->m_RealLength > min_len) &&
			 (pCpn->m_RealWidth > min_len))
		 {
			 list.push_back(pCpn);
		 }

	 }
 }


 // 获取所有需要的小板
void Component::GetAllNeededComponent(vector<Component*>& list)
{
	vector<Node*> tmp_list;
	GetAllLeafNodes(tmp_list, NodeType_NeededComponent);	

	int nCount = tmp_list.size();

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = tmp_list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		list.push_back(pCpn);
	}
}

// 面积从大到小排序
bool  Component::ComponentCompareAreaLess(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return p1->IsAreaLargeThan(*p2);	
}


// 获取最大的余料
Component* Component::GetLargestRemainder(void)
{
	vector<Component*> list;

	GetAllRemainder(list);

	if (list.size() == 0)
		return NULL;

	// 从大到小排序
	sort(list.begin(), list.end(), ComponentCompareAreaLess);

	return list.front();
}

// 获取最大的小板
Component* Component::GetLargestNeedComponent(void)				
{
	vector<Component*> list;

	GetAllNeededComponent(list);

	if (list.size() == 0)
		return NULL;

	// 从大到小排序
	sort(list.begin(), list.end(), ComponentCompareAreaLess);

	return list.front();
}




// 长度优先 面积次之
bool Component::operator> (const Component& dst_cpn) const
{
#if (Component_Sort_Priority == Length_First)

	if (this->m_RealLength > dst_cpn.m_RealLength)			// 长度优先
	{
		return true;
	}
	else if (this->m_RealLength == dst_cpn.m_RealLength)		// 面积次之
	{
		float src_area = this->m_RealLength * this->m_RealWidth;
		float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

		if (src_area > dst_area)
		{
			return true;
		}
	}

#elif	(Component_Sort_Priority == Width_First)

	if (this->m_RealWidth > dst_cpn.m_RealWidth)			// 长度优先
	{
		return true;
	}
	else if (this->m_RealWidth == dst_cpn.m_RealWidth)		// 面积次之
	{
		float src_area = this->m_RealLength * this->m_RealWidth;
		float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

		if (src_area > dst_area)
		{
			return true;
		}
	}
#elif	(Component_Sort_Priority == Area_First)
	float src_area = this->m_RealLength * this->m_RealWidth;
	float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;
	
	if (src_area > dst_area)
	{
		return true;
	}
	else if (src_area == dst_area)		// 面积次之
	{
		if (this->m_RealWidth > dst_cpn.m_RealWidth)			// 长度优先
		{
			return true;
		}
	}

#elif	(Component_Sort_Priority == Special_1)

	float src_area = this->m_RealLength * this->m_RealWidth +　2 * (this->m_RealLength + this->m_RealWidth);
	float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth +　2 * (dst_cpn.m_RealLength + dst_cpn.m_RealWidth);
	
	if (src_area > dst_area)
	{
		return true;
	}


#endif

	return false;
}

// <操作符重载
bool Component::operator< (const Component& dst_cpn) const
{
	if (this->m_RealLength < dst_cpn.m_RealLength)			// 长度优先
	{
		return true;
	}
	else if (this->m_RealLength == dst_cpn.m_RealLength)		// 面积次之
	{
		float src_area = this->m_RealLength * this->m_RealWidth;
		float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

		if (src_area < dst_area)
		{
			return true;
		}
	}

	return false;
}

bool Component::operator== (const Component& dst_cpn) const		// ==操作符重载
{
	if (this->m_RealLength == dst_cpn.m_RealLength && this->m_RealWidth == dst_cpn.m_RealWidth)			// 长度优先
	{
		return true;
	}

	return false;
}



bool Component::IsAreaLargeThan(const Component& dst_cpn) const
{
	float src_area = this->m_RealLength * this->m_RealWidth;
	float dst_area = dst_cpn.m_RealLength * dst_cpn.m_RealWidth;

	if (src_area > dst_area)
	{
		return true;
	}

	return false;
}











// 旋转小板
void Component::ClockwiseRotate90(void)
{
	float org_len = m_RealLength;
	float org_width = m_RealWidth;

	m_RealLength = org_width;
	m_RealWidth	= org_len ;

	//m_bRotated = !m_bRotated;
	m_nRotatedAngle -= 90;                     //顺时针为负
	m_nRotatedAngle = m_nRotatedAngle % 360;
}

// 逆时针旋转90度
void Component::CouterClockwiseRotate90(void)
{
	if(IsRotatable() == true)
	{
		float org_len = m_RealLength;
		float org_width = m_RealWidth;

		m_RealLength = org_width;
		m_RealWidth	= org_len ;

		//m_bRotated = !m_bRotated;
		m_nRotatedAngle += 90;              //逆时针为正
		m_nRotatedAngle = m_nRotatedAngle % 360;
	}

}


// 能否旋转
bool Component::IsRotatable(void)
{
	if(m_Texture == TextureType_NO_TEXTURE)
		return true;
	else
		return false;
}


// 是否合法
bool Component::IsLegal(void)
{
	if (m_RealLength > 0 && m_RealWidth > 0)
	{
		return true;
	}

	return false;
}


// 获取板件面积
float Component::GetArea(void)
{
	float area = m_RealLength * m_RealWidth;

	if (area > 0)
		return area;
	else
		return 0;
}

// 获取板件面积包含锯缝
float  Component::GetAreaContainKerf(void)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	float half_kerf = pSingleton->m_BaseInfo.m_SawKerfWidth/2;
	float area = (m_RealLength+half_kerf) * (m_RealWidth+half_kerf);

	if (area > 0)
		return area;
	else
		return 0;
}

/*---------------------------------------*/
//	函数说明：
//	计算矩形轮廓点
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
void Component::CalRectOutlinePoint(void)
{
	m_vOutlinePoint.clear();

	PointInfo p1;

	p1.x	= 0;
	p1.y	= m_RealWidth;
	p1.r	= 0;
	p1.sign	= 0;
	p1.dir	= 0;;
	p1.cut	= 0;;
	p1.side	= 0;;
	p1.type	= 0;;
	p1.group = 1;

	PointInfo p2;

	p2.x = 0;
	p2.y = 0;
	p2.r	= 0;
	p2.sign	= 0;
	p2.dir	= 0;;
	p2.cut	= 0;;
	p2.side	= 0;;
	p2.type	= 0;;
	p2.group = 0;


	PointInfo p3;

	p3.x = m_RealLength;
	p3.y = 0;
	p3.r	= 0;
	p3.sign	= 0;
	p3.dir	= 0;;
	p3.cut	= 0;;
	p3.side	= 0;;
	p3.type	= 0;;
	p3.group = 0;

	PointInfo p4;

	p4.x = m_RealLength;
	p4.y = m_RealWidth;
	p4.r	= 0;
	p4.sign	= 0;
	p4.dir	= 0;;
	p4.cut	= 0;;
	p4.side	= 0;;
	p4.type	= 0;;
	p4.group = 0;

	PointInfo p5;

	p5.x	= 0;
	p5.y	= m_RealWidth;
	p5.r	= 0;
	p5.sign	= 0;
	p5.dir	= 0;;
	p5.cut	= 0;;
	p5.side	= 0;;
	p5.type	= 0;;
	p5.group = 2;

	m_vOutlinePoint.push_back(p1);
	m_vOutlinePoint.push_back(p2);
	m_vOutlinePoint.push_back(p3);
	m_vOutlinePoint.push_back(p4);
	m_vOutlinePoint.push_back(p5);
}



/*---------------------------------------*/
//	函数说明：
//		是否有孔
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
bool Component::HaveDownerFaceHole(void)
{
	if (m_vDownerFaceHole.size() > 0)
		return true;

	return false;
}



/*---------------------------------------*/
//	函数说明：
//		是否有槽
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
bool  Component::HaveDownerFaceSlot(void)
{

	if (m_vDownerFaceSlot.size() > 0)
		return true;

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
bool  Component::HaveDownerFaceInfo(void)
{
	if (m_vDownerFaceHole.size() > 0 || m_vDownerFaceSlot.size() > 0)
		return true;

	return false;
}
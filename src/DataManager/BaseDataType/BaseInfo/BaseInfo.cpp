#include "stdafx.h"
#include "BaseInfo.h"

#if 1

BaseInfo::BaseInfo(void)
{

	m_SawKerfWidth						= 6;		// 锯缝 用于排样
	m_PanelLength						= 2440;		// 大板长度		
	m_PanelWidth						= 1220;		// 大板宽度		
	m_DeburringWidth					= 3;		// 修边距离 4边都修		

	m_UpHeight							= 45;		// 安全高度
	m_DownHeight						= 0.2;		// 下刀深度
	m_distance1							= 30;		// 下刀偏移1
	m_distance2							= 20;		// 下刀偏移2
	m_MinLength							= 300;		// 最小大板参数 - 最小长度
	m_MinWidth							= 300;		// 最小大板参数 - 最小宽度
	m_MinArea							= 90000;	// 最小大板参数 - 最小面积

	m_FirstSectionOPTimes				= 1;		// 第一阶段优化次数
	m_FirstSectionOPMethod				= 0;		// 第一阶段优化方法
	m_SecondSectionOPTimes				= 0;		// 第二阶段优化次数
	m_ThirdSectionOPTimes				= 0;		// 第三阶段优化次数
	m_ThirdSectionOAccptableUtilization	= 0;		// 第三阶可接受优化率
	m_LayoutOrg							= 0;		// 排样原点
	m_bDownerFaceFirst					= true;	// 反面信息优先排列		
}


BaseInfo::~BaseInfo(void)
{

}

#endif
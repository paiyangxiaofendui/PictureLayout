

#ifndef		_BaseInfo_H_
#define		_BaseInfo_H_



//基础信息
class AFX_EXT_CLASS BaseInfo
{
public:
	BaseInfo(void);
	~BaseInfo(void);

public:
	// 开料相关的信息
	
	float m_SawKerfWidth;						// 锯缝 用于排样
	float m_PanelLength;						// 大板长度		
	float m_PanelWidth;							// 大板宽度		
	float m_DeburringWidth;						// 修边距离 4边都修		

	float m_UpHeight;							//安全高度
	float m_DownHeight;							//下刀深度
	float m_org_offset;
	float m_distance1;							//下刀偏移1
	float m_distance2;							//下刀偏移2
	float m_MinLength;							//最小大板参数 - 最小长度
	float m_MinWidth;							//最小大板参数 - 最小宽度
	float m_MinArea;							//最小大板参数 - 最小面积

public:
	// 优化相关参数

	int m_FirstSectionOPTimes;					// 第一阶段优化次数
	int m_FirstSectionOPMethod;					// 第一阶段优化方法
	int m_SecondSectionOPTimes;					// 第二阶段优化次数
	int m_ThirdSectionOPTimes;					// 第三阶段优化次数
	float m_ThirdSectionOAccptableUtilization;	// 第三阶可接受优化率

	int m_LayoutOrg;							// 排样原点
	bool m_bDownerFaceFirst;					// 有反面信息优先排列



};


#endif
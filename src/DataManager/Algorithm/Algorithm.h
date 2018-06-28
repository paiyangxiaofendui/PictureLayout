#pragma once
#include "../BaseDataType/CommonData/CommonData.h"
#include "../BaseDataType/BaseInfo/BaseInfo.h"
#include "../BaseDataType/Component/Component.h"
#include "../BaseDataType/Panel/Panel.h"
#include "../BaseDataType/Outline/Outline.h"


// 算法接口 提供多种材质优化排版、多快板优化排版、单块板优化排版、余料优化排版
class AFX_EXT_CLASS ALGORITHM_API
{
public:
	// 贪心算法

	
	//	在大板中优化排样
	static int LayoutOnePanel_Greedy(Panel* pSrcPanel, BaseInfo& base_info, vector<Component*>& SrcCpnList, int cut_style, int Org);

	// 初始化余料线链表
	static int BuildRemainderList(Panel* pParent, vector<Component*>& RemainderList);

	// 匹配合适的小板和余料
	static bool MatchSuitableComponentNRemaider(vector<Component*>& RemainderList, vector<Component*>& ComponentList, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder, BaseInfo& base_info);

	// 对余料进行从大到小排序
	static int SortRemainderList_LengthFirst(vector<Component*>& RemainderList);

	// 比较第一块是否小于第二块板
	static bool ComponentCompareSmaller(const Component* pfirst, const Component* psecond) ;

	// 切割方式
	static bool KnifeOneRemainder(Component* pRemainder, Component* pComponent, int cut_dir, float kerf_width, int Org);

public:
	// 最低轮廓线算法
	static int LayoutOnePanel_LowestOutline(Panel* pSrcPanel, BaseInfo& base_info, vector<Component*>& SrcCpnList, int cut_style, int Org);

	// 建立最低轮廓线链表
	static int BuildOutlineList(Panel* pParent, vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList);

	// 从小到大排序
	static bool CompareOutlineGreater(const Outline& first, const Outline& second) ;

	// 板件随机排序
	static int SortComponentList_Random(vector<Component*>& ComponentList);

	// 匹配合适的小板和轮廓线
	static bool MatchSuitableComponentNOutline2(vector<Component*>& ComponentList,vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList, int &nCpnID, int &nOutlineID);

public:
	// 一些数据操作

	// 拷贝小板链表
	static  void CopyComponentList(vector<Component*>& SrcList, vector<Component*>& DstList);

public:
	// 一些树形操作
	static void TravelNeedComponent_AreaLargeFirst(Component* pCpn, vector<Component*>& cpn_list);



public:
	// 数学相关计算函数
	static float GetPointsDistance(PointInfo p1, PointInfo p2);									// 两点间距离
	static vector<PointInfo> FiltrateCoincidentPoints(vector<PointInfo>& oldpoint);				// 不要最后一个直线点
	static vector<PointInfo> FiltrateCoincidentPoints2(vector<PointInfo>& oldpoint);			// 要最后一个直线点
	static void rotatepoint(float x0,float y0,float & x,float & y,float arc);					// 旋转点
	static void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);	// 获取圆心 此函数有错，现已弃用



	static void CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc);	//		逆时针旋转
	static void ClockwiseRotate(float x0,float y0,float & x,float & y,float arc);		//		顺时针旋转
	static void NormalVector(float x0,float y0,float & x,float & y);					//		向量规范化
	static void GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);	// 两点取圆心,改进了getCircleCenter的错误


};
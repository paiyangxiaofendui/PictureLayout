/*--------------------------------------------------------------------------------------------------------------------*
///	CSolution.h    --  解决方案类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.11.9
//	备注：	同种材质，同种厚度的小板，排在n块大板上
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_CSolution_H_ 
#define		_CSolution_H_

#include "../BaseInfo/BaseInfo.h"
#include "../Outline/Outline.h"
#include "../CommonData/CommonData.h"

#include <vector>
using namespace std;

class Panel;
class Component;

class AFX_EXT_CLASS CSolution
{
public:
	CSolution();
	virtual  ~CSolution();



public:
	// 优化率相关接口 
	int GetPanelNum(void);				// 获取大板数量
	Panel* GetPanel(int panel_num);		// 获取大板


public:
	// 操作接口
	//int LayoutOptimization(void);						// 排样优化	长度优先排序 + 最低轮廓线 + 随机横竖切割 
	int LayoutOptimization_RandomSortCut(int CutStyle, int Org);			// 排样优化	随机排序 + 最低轮廓线 + 随机横竖切割 

public:
	int LayoutOptimization_MinWaste(int CutStyle, int Org);				// 排样优化	余料剩余最小，长或宽最大利用优先
//	bool MatchSuitableComponentNRemaider(Panel* pPanel, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder);			// 匹配合适的小板和轮廓线
//	int BuildRemainderList(Panel* pParent);				// 初始化余料线链表
//	int SortRemainderList_LengthFirst(void);			// 板件排序，长度优先，面积次之



	int SortComponentList_LengthFirst(void);				// 板件排序，长度优先，面积次之
	int SortComponentList_Random(void);						// 板件排序，长度优先，面积次之



	void CheckComponentList(void);						// 检查板件大小是否超出。
	int BuildOutlineList(Panel* pParent);				// 初始化轮廓线链表
	int PlaceOneComponent(Component* pCpn);				// 摆放一块小板
	bool MatchSuitableComponentNOutline(int &nCpnID, int &nOutlineID);			// 匹配合适的小板和轮廓线
	bool MatchSuitableComponentNOutline2(int &nCpnID, int &nOutlineID);			// 匹配合适的小板和轮廓线

public:
	// 解决方案内部重新优化
	void ReOptimizeEveryPanel(int Org);			// 解决方案内部重新优化


	// 对大板列表按照优化率进行排序
	void SortPanelList(void);


public:
	int	m_SlnID;						// 解决方案号					
	float m_fThickness;					// 板件厚度						
	CString m_strMaterial;				// 板件材料						

	BaseInfo m_BaseInfo;				// 基础信息						

	vector<Panel*> m_PanelList;			// 大板列表 优化方案			
	vector<Component*> m_ComponentList;	// 小板列表 原始板件


	// 最低轮廓线算法使用
	vector<Outline>	m_OutlineList;			// 轮廓线链表
	vector<Outline>	m_LowestOutlineList;	// 最低轮廓线链表
};

#endif
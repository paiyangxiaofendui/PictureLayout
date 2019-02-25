/*--------------------------------------------------------------------------------------------------------------------*/
//	Component.h      -- 小板类声明文件
//	
//	作者：	yuanzb
//	时间：	2019.11.9
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Component_H_
#define		_Component_H_

#include "../Node/Node.h"
#include "../CommonData/CommonData.h"
#include "../CSingleton/CSingleton.h"


enum
{
	Length_First,
	Width_First,
	Area_First,
	Special_1,
	MaxSide_First
};


#define  Component_Sort_Priority		MaxSide_First

// 小板信息
class AFX_EXT_CLASS Component : public Node
{
public:
	Component(void);
	virtual ~Component(void);


public:
	int			m_CpnID;					// 板件ID	唯一标识， 为了解决小板切多次问题
	int			m_NumberInPanel;			// 板内序号 第几块排的小板
	int			m_IndexInSameCpn;			// 同一块小板中的序号，只有第一块显示，其他不显示
	CString 	m_BarCode;					// 条码
	CString     m_strCabinetName;			// 柜体名称
	CString     m_strComponentName;			// 板件名称
	CString     m_Material;					// 材料
	int         m_Texture;					// 纹理	0：无纹理  1：横纹 2：竖纹	
	float       m_Thickness;				// 厚度
	float       m_RealLength;				// 真实长
	float       m_RealWidth;				// 真实宽
	float       m_x;						// x坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
	float       m_y;						// y坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
	float       m_fXLabelCenter;            // 标签中点x坐标 相对小板的顶点，直角坐标系，第一象限，左下角为原点
	float       m_fYLabelCenter;            // 标签中点y坐标 相对小板的顶点，直角坐标系，第一象限，左下角为原点

	int m_nRotatedAngle;                    // 板件相对于原始输入旋转了多少度，顺时针为负，逆时针为正
	int m_nOtherShapeID;					// 异形ID
	vector<PointInfo> m_vOutlinePoint;		// 轮廓点信息
	int m_nKnifeDownPos;                    // 下刀点
	vector<AllVec> m_vUpperFaceHole;        // 上表面孔
	vector<AllSlot> m_vUpperFaceSlot;       // 上表面槽
	vector<AllVec> m_vDownerFaceHole;       // 下表面孔
	vector<AllSlot> m_vDownerFaceSlot;      // 下表面槽


	CString m_strOrderID;               //订单号
	CString m_strCabinetID;             //柜号
	float m_afBanding[4];               //封边
	bool m_bSlotFlipped;                //槽翻转
	bool m_bVHoleFlipped;                //孔翻转
	CString m_strCustomerInfo;          //客户信息
	CString m_strJoinedStore;           //加盟店
	CString m_strSlottingFlag;          //拉槽标识
	CString m_strReminder;              //助记号
	CString m_strDrilling;              //钻孔
	CString m_strOrderType;             //订单类型
	CString m_strReverseSideBarcode;    //反面条码
	float m_fProductLength;             //成品长度
	float m_fProductWidth;              //成品宽度
	float m_fProductThickness;          //成品厚度
	CString m_strCustomerAddress;       //客户地址
	CString m_strHoleSlotFlag;          //钻槽标识
	CString m_strPanelClass;            //板件类型,"Door"为门板，"Panel"为普通板
	CString m_strSendingDate;           //发货日期
	bool m_aHasSideHole[4];             //是否有侧孔（左、前、右、后）

	//float m_fDist2PanelCenter;			// 距离大板中心的距离
	//float m_fCenterX;					// 大板中心X
	//float m_fCenterY;					// 大板中心Y



public:
	// 节点相关接口
	virtual Node* CopyNode(void);										// 拷贝节点
	virtual bool Containable(Component* pCpn);							// 能否包含节点
	virtual void GetAllRemainder(vector<Component*>& list);				// 获取所有的余料
	virtual void GetAllRemainder(vector<Component*>& list, float area, float min_len);	// 获取所有面积大于area的余料
	virtual void GetAllNeededComponent(vector<Component*>& list);		// 获取所有需要的小板
		
	virtual Component* GetLargestRemainder(void);						// 获取最大的余料
	virtual Component* GetLargestNeedComponent(void);					// 获取最大的小板




public:
	// 操作符重载
	bool operator> (const Component& dst_cpn) const;		// >操作符重载
	bool operator< (const Component& dst_cpn) const;		// <操作符重载
	bool operator== (const Component& dst_cpn) const;		// ==操作符重载

	// 比较相关接口
	bool IsAreaLargeThan(const Component& dst_cpn) const;

	static bool  ComponentCompareAreaLess(const Component* pfirst, const Component* psecond) ;


public:
	// 操作接口
	bool IsLegal(void);						// 是否合法
	bool IsRotatable(void);					// 能否旋转
	void ClockwiseRotate90(void);			// 顺时针旋转90度
	void CouterClockwiseRotate90(void);		// 逆时针旋转90度

	float GetArea(void);					// 获取板件面积
	float GetAreaContainKerf(void);			// 获取板件面积包含锯缝

	void CalRectOutlinePoint(void);			// 计算矩形轮廓点

public:
	// 虚函数
	virtual bool HaveDownerFaceHole(void);			// 是否有反面孔
	virtual bool HaveDownerFaceSlot(void);			// 是否有反面槽
	virtual bool HaveDownerFaceInfo(void);			// 是否有反面信息

};

typedef vector<Component*> ComponentList;

#endif

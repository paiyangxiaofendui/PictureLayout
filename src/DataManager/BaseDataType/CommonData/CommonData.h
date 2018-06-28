#ifndef		_CommonData_h_
#define		_CommonData_h_


#pragma once
#include <vector>
#include <map>

using namespace std;

// 全局宏定义
#define		NEW_ENCRYPT_BASE64		1

#define HG_EDITION 0
#define YW_EDITION 1

#define CUR_EDITION HG_EDITION
















// 板件的节点类型
typedef enum NodeType
{
	NodeType_Remainder = 0,			// 余料								叶子节点
	NodeType_NeededComponent,		// 需要的小板						叶子节点
	NodeType_CombinePanel			// 组合板 由余料和需要的小板组成	父节点
}NodeType;

// 纹理方向
typedef enum TextureType
{
	TextureType_NO_TEXTURE = 0,			// 无纹理
	TextureType_H_TEXTURE,				// 横纹
	TextureType_V_TEXTURE				// 竖纹
}TextureType;

// 切割方向
typedef enum CutDir
{
	CutDir_Horizon = 0,					// 横切
	CutDir_Vertical,					// 竖切
	CutDir_Random,						// 随机
	CutDir_Default						// 默认

}CutDir;

// 小板排样原点
typedef enum LayoutOrg
{
	LayoutOrg_LeftBottom = 0,			// 左下角
	LayoutOrg_LeftTop,					// 左上角
	LayoutOrg_RightBottom,				// 右下角
	LayoutOrg_RightTop					// 右上角

}LayoutOrg;


// 大板类型  原料、余料

typedef enum PanelType
{
	PanelType_RawMaterial = 0,			// 原料
	PanelType_Remainder,				// 余料

}PanelType;





// 文件输入小板
struct ComponentInputItem
{
	ComponentInputItem()
		:m_fLength(0.0)
		,m_fWidth(0.0)
		,m_fThickness(0.0)
		,m_nCount(0)
		,m_eRotateType(0)
		,m_bSlotFlipped(false)
		,m_bVHoleFlipped(false)
		,m_fProductLength(0.0)
		,m_fProductWidth(0.0)
		,m_fProductThickness(0.0)
	{
		m_strTexture = _T("无纹理");
		m_afBanding[0] = 0.0;
		m_afBanding[1] = 0.0;
		m_afBanding[2] = 0.0;
		m_afBanding[3] = 0.0;
		m_aHasSideHole[0] = m_aHasSideHole[1] = m_aHasSideHole[2] = m_aHasSideHole[3] = false;
	}

	UINT m_uSerialID;                    //序列号，用于标识各项
	CString m_strOrderID;               //订单号
	CString m_strCabinetID;             //柜号
	CString m_strBarcode;				// 条码
	CString m_strCabinetName;			// 柜体名
	CString m_strPanelName;				// 板件名
	CString m_strMaterial;				// 材质
	float m_fLength;					// 开料长
	float m_fWidth;						// 开料宽
	float m_fThickness;					// 开料厚度
	int m_nCount;						// 需切数量
	CString m_strTexture;				// 纹理
	float m_afBanding[4];               //封边
	CString m_strOtherShapeIDAndParam;	//异形参数
	int m_eRotateType;					//旋转
	CString	m_strUpperFaceSlotInfo;		// 正面槽信息
	bool m_bSlotFlipped;                //槽翻转
	CString	m_strDownerFaceSlotInfo;	// 反面槽信息
	CString	m_strUpperFaceHoleInfo;		// 正面孔信息
	bool m_bVHoleFlipped;                //孔翻转
	CString	m_strDownerFaceHoleInfo;	// 反面孔信息
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
	CString m_strOtherShapeID;			// 异形ID
	CString m_strCustomerAddress;       //客户地址
	CString m_strHoleSlotFlag;          //钻槽标识
	CString	m_strAbnormalInfo;			// 异形点信息
	CString m_strPanelClass;            //板件类型,"Door"为门板，"Panel"为普通板
	CString m_strSendingDate;           //发货日期
	bool m_aHasSideHole[4];             //是否有侧孔（左、前、右、后）
};

class CSolution;
class Panel;

struct PanelViewingParam
{
	CSolution* m_pSolution;
	Panel* m_pPanel;
	bool m_bChecked;
};

typedef struct 
{
// 	RemainderInputItem()
// 		: m_nXLen(0)
// 		, m_nYLen(0)
// 		, m_fThickness(0.0)
// 		, m_nCount(0)
// 	{}

	int m_nXLen;
	int m_nYLen;
	float m_fThickness;
	CString m_strMaterial;
	int m_nCount;
}RemainderInputItem;


// 导出NC相关数据结构

#define  NEW_PROGRAM	(1)		// 新程序 修改了数据结构



// 划一刀
struct CutPath
{
	float _x1;
	float _y1;
	float _x2;
	float _y2;
};

// 贴条码
struct P_Info
{
	string _info;	// 编号
	string _content;	// 内容
};
// 
struct Remove_Line
{
	float begin_x;
	float begin_y;
	float end_x;
	float end_y;
};
// 位置
struct pos
{
	float x,y;	// 绝对坐标
	int _kn;	// 刀号
};
// 
struct changexy
{
	float _size;
	float _x;
	float _y;
};
// 大板节点
struct PanelNode
{
	float _x,_y;			// 标记目前剩余板件在原材料的位置
	float _height,_width;	// 切割零件的大小
	PanelNode *lchild;		// 切割后的左子树
	PanelNode *rchild;		// 切割后的右子树
	int _num;				// 叠切
};
// 刀类型
struct KnifeType
{
	int kt;						// 刀类型 
	float _diameter;			// 直径
	string _tgcodeb;			// 刀头代码
	string _tgcodee;			// 刀尾
	float _feedrate;			// 空跑速度
	float _spindlespeed;		//
	float _endspeed;			// 结束速度
	float _overspend;			// 结束速度
	float _facespeed;			// 接触料面速度
	float _kx;					// 刀盒x位置
	float _ky;					// 刀盒y位置
	float _length;				// 分割界限，如果比刀长度小，则分几次开料，如_kinfelength = 30 _length = 20 则第一次切到高度为10（切了20厚），第二次切刀高度-0.1(切穿)
	int _kn;					// 刀号
	string _coorsys;			// 坐标系
	float _kinfelength;			// 有效长度，最深能打多深
};

// 普通组件
struct p_Component
{
	float _height,_width;		// 该零件的大小
	float _x,_y;				// 该零件在组合零件中的位置
	string _info;			// 零件编码
	int _dir;					// 0表示无纹理，1表示横纹，2表示竖纹
	//	float label_x;				// 贴标x坐标，相对板件左上角
	//	float label_y;				// 贴标y坐标，相对板件左上角
	//	float label_width;			// 贴标宽度
	//	float label_height;			// 贴标高度
};

// 叠切的组件
struct p_ComponentA
{
	float _height,_width;		//	该零件的大小
	float _x,_y;				//	该零件在组合零件中的位置
	string _info;			//	零件编码
	int _dir;					//	0表示无纹理，1表示横纹，2表示竖纹
	int _num;					//	叠切的数量
};

// 组合零件
struct p_Components
{
	float _height,_width;		// 组合零件的长宽
	vector<p_Component> _coms;	// 零件容器 可能有多个
};

// 组件坐标
struct Coordinate
{
	p_Components _onecoms;		// 组合零件 可能存在多个零件的在同一块组中
	float _x,_y;				// 对应零件在原材料中的坐标
};

struct gcodeCoor //对于每块小板的坐标，大小，条码信息等数据，用于导出G代码和画出示意图
{
	float _x,_y;				// x，y坐标
	string _info;			// 条码
	float _height,_width;		// 大板宽、高
	int _dir;					// 1：横纹  2：竖纹
	int pos;					// 落刀点号

	// 新增唯一板内ID
	int m_NumInPanel;

	//	float label_x;				// 贴标x坐标，相对板件左上角
	//	float label_y;				// 贴标y坐标，相对板件左上角
	//	float label_width;			// 贴标宽度
	//	float label_height;			// 贴标高度
};

// 原始点信息
struct PointInfo
{
	float x;	// x坐标，第四象限相对异形板原点坐标
	float y;	// y坐标，第四象限相对异形板原点坐标
	float r;	// 半径，直线点为 0，圆弧点为半径值
	int sign;	// 0：直线点  1：圆弧起点 2：圆弧终点
	int dir;	// 0：直线 1：逆时针 2：顺时针
	int cut;	// 0:不起不落 1：起刀 2：重新落刀

	int group;	//  1：起点 2：终点
	int side;	// 加工方向 1：外切（外轮廓） 2：内切（内轮廓） 3：中心点
	int type;	// 铣型类型?0：非CNC加工线 1：CNC加工线

};

// 刀补点信息
struct DPointInfo
{
	float x;	// x坐标，第四象限相对异形板原点坐标
	float y;	// y坐标，第四象限相对异形板原点坐标
	float r;	// 半径，直线点为 0，圆弧点为半径值
	int sign;	// 0：直线点	1：圆弧起点 2：圆弧终点
	int dir;	// 0：直线		1：逆时针	2：顺时针
	int cut;	// 0: 不起不落	1：起刀		2：罗刀


#if (NEW_PROGRAM == 1)

	int group;	//  1：起点 2：终点
	int side;	// 加工方向 1：外切（外轮廓） 2：内切（内轮廓） 3：中心点
	int type;	// 铣型类型 0：非CNC加工线 1：CNC加工线
#endif


	int posold;
	int posnew;
};

// 所有的原始点
typedef struct AllPoint
{
	// 新增唯一板内ID 取代条码作为判断标准
	int m_NumInPanel;

	string panel_info;			// 板件编号
	vector<PointInfo> point_info;
}AllPoint;

// 所有的刀补点
typedef struct DAllPoint
{
	string panel_info;
	vector<DPointInfo> point_info;
}DAllPoint;

// 所有的槽
typedef struct AllSlot
{
	AllSlot():m_bDisabled(false){}
	string panel_info;
	float _x;		// 槽的长方体中心位置
	float _y;		// 槽的长方体中心位置
	float _width;	// 宽度 x方向长度
	float _height;	// 高度	y方向长度
	float _depth;	// 槽的深度
	int _dir;		// 槽的方向 0：竖的  1：横的

	// 新增唯一板内ID 取代条码作为判断标准
	int m_NumInPanel;

	bool m_bDisabled; // 是否被禁用，被禁用的不参与导出NC码
}AllSlot;

// 孔
typedef struct AllVec
{
	AllVec():m_bDisabled(false){}
	string panel_info;	// 板号
	float _x;			// 相对小板中心点坐标
	float _y;			// 相对小板中心点坐标
	float _r;			// 半径
	float _depth;		// 深度

	// 新增唯一板内ID 取代条码作为判断标准
	int m_NumInPanel;

	bool m_bDisabled; // 是否被禁用，被禁用的不参与导出NC码
}AllVec;

// 现以不用
struct GcodeParam
{
	string header;
	string ender;
	float prospeech;
	float cutspeech;
	float endspeech;
	float overspeech;
	float upheight;		// 安全高度
	float downheight;	// 下刀深度
	float distance1;	// 下刀偏移1，安全高度-》对刀原点
	float distance2;	// 下刀偏移2，对刀原点-》下刀深度
	float org_offset;	// 对刀原点偏移，为了防止爆边，可能在将对刀原点向上增加1mm
	string strRinget;
};

// 小板尺寸
struct OptimizeParam
{
	float minwidth;
	float minheight;
	float minsize;	
	float minrate;		// 最小利用率
	int _knife_type;
};

// 条码信息
struct BarString
{
	string bar_code;	// 条码编号
	string print_info;
};

struct Info_dir
{
	int _dir;
	string _info;
};


// 贴标位置
typedef struct
{
	float x;
	float y;
}PointFloat;



// 排钻刀打垂直孔
typedef struct 
{
	std::string panel_info;						// 板件条码
	float _pwidth;								// 板宽
	float _pheight;								// 板高
	float _x;									// x
	float _y;									// y
	float _r;									// r半径
	float _depth;								// 深度
	float _diameter;							// 直径
	float src_x;								// 板件原点x
	float src_y;								// 板件原点y
	std::string _tgcodeb;						// 刀具起始代码
	std::string _tgcodee;						// 刀具结束代码
	std::string _coorsys;						// 坐标系偏移
	int _feedrate;								// 速度
	int _spindlespeed;							// 速度
	int _overspeed;								// 速度
	int _endspeed;								// 速度
	int _facespeed;								// 速度

	std::vector<float> _kx;						// 刀x数组
	std::vector<float> _ky;						// 刀y数组
	std::vector<int> _kn;						// 刀号数组
	std::vector<int> _samedownkn;				// 双刀下
	std::vector<std::string> BeginCodeList;		// 开始代码列表
	std::vector<std::string> EndCodeList;		// 结束代码列表

	int _thiskn;								// 这把刀号
	float _thiskx;								// 这把刀号
	float _thisky;								// 这把刀号
	bool _kdown;								// 这把刀下
}KnifeTypeAndVecA;

// 刀用途
typedef enum 
{
	KnifeType_Spindle_Hole_Slot_Material = 0,		// 主轴孔槽料		0
	KnifeType_Spindle_Hole_Slot,					// 主轴孔槽			1
	KnifeType_Spindle_Hole,							// 主轴孔			2
	KnifeType_Spindle_Slot,							// 主轴槽			3
	KnifeType_Spindle_Material,						// 主轴料			4
	KnifeType_Spindle_Slot_Material,				// 主轴槽料			5
	KnifeType_Drill_Hole,							// 排钻孔			6
	KnifeType_Multi_knife_Hole,						// 多刀头钻孔		7

}enumKnifeType;

// 刀打垂直孔，单独
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _r;
	float _depth;
	float _diameter;
	std::string _tgcodeb;
	std::string _tgcodee;
	int _feedrate;
	int _spindlespeed;
	float _srcx;
	float _srcy;
}KnifeTypeAndVec;

// 刀开槽
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _width;
	float _height;
	float _depth;
	int _dir;
	float _diameter;
	std::string _tgcodeb;
	std::string _tgcdoee;
	int _feedrate;
	int _spindlespeed;
	float _srcx;
	float _srcy;
}KnifeTypeAndSlot;

// 槽
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _width;
	float _height;
	float _depth;
	int _dir;
	float _srcx;
	float _srcy;
}AllSlotK;

// 孔
typedef struct 
{
	std::string panel_info;
	float _pwidth;
	float _pheight;
	float _x;
	float _y;
	float _r;
	float _depth;
	float _srcx;
	float _srcy;
}AllVecK;

class KnifeClass;


typedef  struct Panel_Knife
{
	PanelNode * _panel;
	KnifeClass * _kc;
	vector<Coordinate> _coor;
	CStringW big_file;
}Panel_Knife;

typedef struct print_info
{
	CString panel_no;
	CString com_no;
	CString customer_info;
	CString bar_code;
	CString com_width;
	CString com_height;
	CString com_depth;
	CString com_name;
	CString com_matrial;
	CString cst_no;
	CString cst_name;			//柜体名称（产品名称）
	CString edge_info;
	CString product_width;
	CString product_height;
	CString other_com;			//异型
	CString slot_com;			//开槽
	CString punching_com;		//打孔
	CString double_com;			//双面
	CString franchisee;			//加盟店
	CString ordertype;			//订单类型
	CString rebar_code;			//反面条码
	CString customer_address;
	CString zc_identify;
	CString panel_dir;
	CString panel_reversal;
	int     nRotatedAngle;
	bool    bIsDoorPanel;       //是否门板
	CString m_strSendingDate;   //发货日期
	bool m_aHasSideHole[4];     //是否有侧孔（左、前、右、后）
}print_info;



// 客户信息结构体
class ClientInfo
{
public:
	CString		m_ClientName;
	CString		m_PhoneNumber;
	CString		m_QQNumber;
	CString		m_Address;
	
	CString		m_MacAdress;
	CString		m_BaseBoardSerialNo;
	CString		m_CPUSerialNo;
	CString		m_HDSerialNo;

	int			m_OutputNCTimes;
	long		m_RegisterTime;	


};

struct SourceFilePreProccesParam
{
	SourceFilePreProccesParam()
		:b_upvecImportEnable(true)
		,b_downvecImportEnable(true)
		,b_upsoltImportEnable(true)
		,b_downsoltImportEnable(true)
		,b_othershapeImportEnable(true)
		,i_comnum(1)
		,i_comchange(0)
		,f_changex(0.0)
		,f_changey(0.0)
		,f_width(0.0)
		,f_height(0.0)
		,f_Deburringwidth(0.0)
		,bReserveDeepHole(false)
		,bReserveDeepSlot(false)
	{}

	bool b_upvecImportEnable;//正面孔
	bool b_downvecImportEnable;//反面孔
	bool b_upsoltImportEnable;//正面槽
	bool b_downsoltImportEnable;//反面槽
	bool b_othershapeImportEnable;//异形过滤


	int i_comnum;//切割数量
	int i_comchange;//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
	float f_changex;//孔槽偏移
	float f_changey;//孔槽偏移

	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	std::map<float,float> vecAdder;

	//孔位过滤（找到指定孔直径的孔，并移除）
	vector<float> _cxy;

	float f_width;//原料长度
	float f_height;//原料宽度
	float f_Deburringwidth; //修边距离

	bool bReserveDeepHole;
	bool bReserveDeepSlot;
};


// 恩德CSV文件信息
typedef struct 
{
	CString strInfo_A;					//	A		//订单号
	CString strInfo_B;					//	B		//柜号
	CString strInfo_C;					//	C		//条码
	CString strInfo_D;					//	D		//产品名称
	CString strInfo_E;					//	E		//成品名称
	CString strInfo_F;					//	F		//材质
	CString strInfo_G;					//	G		//长
	CString strInfo_H;					//	H		//宽
	CString strInfo_I;					//	I		//厚度
	CString strInfo_J;					//	J		//切割数量
	CString strInfo_K;					//	K		//纹理方向
	CString strInfo_L;					//	L		//封长1
	CString strInfo_M;					//	M		//封宽1
	CString strInfo_N;					//	N		//封长2
	CString strInfo_O;					//	O		//封宽2
	CString strInfo_P;					//	P		//是否旋转
	CString strInfo_Q;					//	Q		//槽翻转
	CString strInfo_R;					//	R		//孔翻转
	CString strInfo_S;					//	S		//客户信息
	CString strInfo_T;					//	T		//加盟店
	CString strInfo_U;					//	U		//拉槽标识

	// 必须，不能改
	CString strDirInfo_V;				//	V
	CString strZplInfo_W;				//	W
	CString strZplInfo2_X;				//	X
	CString strBmpInfo_Y;				//	Y
	CString strLabNum_Z;				//	Z

	CString strInfo_AA;				//	AA			//助记号
	CString strInfo_AB;				//	AB			//钻孔
	CString strInfo_AC;				//	AC			//订单类型
	CString strInfo_AD;				//	AD			//反面条码
	CString strInfo_AE;				//	AE			//成品长度
	CString strInfo_AF;				//	AF			//成品宽度
	CString strInfo_AG;				//	AG			//成品厚度
	CString strInfo_AH;				//	AH			//异型ID
	CString strInfo_AI;				//	AI			//客户地址
	CString strInfo_AJ;				//	AJ			//钻槽标识







}EnDeCsvInfo;	





typedef struct 
{
	CString noProcessSmallPanelOne1;	//订单号
	CString noProcessSmallPanelOne2;	//柜号
	CString noProcessSmallPanelOne3;	//条码
	CString noProcessSmallPanelOne4;	//产品名称
	CString noProcessSmallPanelOne5;	//成品名称
	CString noProcessSmallPanelOne6;	//材质
	CString noProcessSmallPanelOne7;	//长
	CString noProcessSmallPanelOne8;	//宽
	CString noProcessSmallPanelOne9;	//厚度
	CString noProcessSmallPanelOne10;	//切割数量
	CString noProcessSmallPanelOne11;	//纹理方向
	CString noProcessSmallPanelOne12;	//封长1
	CString noProcessSmallPanelOne13;	//封宽1
	CString noProcessSmallPanelOne14;	//封长2
	CString noProcessSmallPanelOne15;	//封宽2
	CString noProcessSmallPanelOne16;	//是否旋转
	CString noProcessSmallPanelOne17;	//槽翻转
	CString noProcessSmallPanelOne18;	//孔翻转
	CString noProcessSmallPanelOne19;	//客户信息
	CString noProcessSmallPanelOne20;	//加盟店
	CString noProcessSmallPanelOne21;	//拉槽标识
	CString noProcessSmallPanelOne22;	//助记号
	CString noProcessSmallPanelOne23;	//钻孔
	CString noProcessSmallPanelOne24;	//订单类型
	CString noProcessSmallPanelOne25;	//反面条码
	CString noProcessSmallPanelOne26;	//成品长度
	CString noProcessSmallPanelOne27;	//成品宽度
	CString noProcessSmallPanelOne28;	//成品厚度
	CString noProcessSmallPanelOne29;	//异型ID
	CString noProcessSmallPanelOne30;	//客户地址
	CString noProcessSmallPanelOne31;	//钻槽标识


}PrintInfo;


#endif

/*--------------------------------------------------------------------------------------------------------------------*/
//	CommonData.h     --	 公共数据声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		CommonData_H
#define		CommonData_H

#define		PI							(3.1415926535898)
#define		CLOSE_VALUE					(0.1)						// 判断两点重叠的距离
#define		OUTLINE_SPACE				(3)							// 圆/弧轮廓线距离mm
#define		ARC_INTERPOLATE_VALUE		(0.5)						// 连接弧弦长超过0.5mm多插补两个点
#define		ARC_INTERPOLATE_MIN_VALUE	(0.3)						// 连接弧弦长超过0.3mm才会插补中间点，否则通过eps判定为相交，不需要插补

#define		Angle2Radian(a)				((a)*PI/180.0)				// 角度转弧度
#define		Radian2Angle(r)				((r)*180.0/PI)				// 弧度转角度

#define		OBTUSE_ANGLE_VALUE			(100)						// 刻意把钝角调大是为了避免90度角的判断出错，减少直线间的圆弧连接  2017-8-30 按照严格的刀补方式，不相交的就用圆弧连接
																	// 向量夹角与线段夹角互补，夹角为锐角，线段相交，圆弧连接。	夹角为钝角，平行直线相交
#define		FIGURE_MIN_LEN				(10.0)						// 最小图形长度


#define		eps							(0.2)
#define		zero(x)						(((x)>0?(x):-(x))<eps)
struct point{double x,y;};





typedef	enum
{
	FigureType_Unknow = 0,
	FigureType_PolyLine,
	FigureType_Arc,
	FigureType_Circle,
	FigureType_Complex,

}FigureType;

// 圆弧方向
enum 
{
	Dir_Parallel,			// 平行，错误，直线
	Dir_CouterClockwise ,	// 逆时针
	Dir_Clockwise ,			// 顺时针
};

// 连接方式
enum
{
	Link_Head2Head,
	Link_Head2Tail,
	Link_Tail2Head,
	Link_Tail2Tail,
};


// 平行方向
enum 
{
	Dir_Left,				// 左边
	Dir_Right ,				// 右边
};



// 轮廓数据枚举定义

// 0：直线点 1:圆弧起点 2：圆弧终点 3：两段圆弧连接点 -1：无需处理的点
typedef enum 
{
	Sign_Ignore = -1,
	Sign_Line,
	Sign_ArcStart,
	Sign_ArcEnd,
	Sign_ArcLink

}OutlineSign;

// 0：直线点 1：逆时针 2：顺时针
typedef enum 
{
	Dir_Line = 0,
	Dir_ArcCouterClockWise,
	Dir_ArcClockWise,

}OutlineDir;

// 0：不起刀 1：需要起刀 2：表示重新下刀
typedef enum 
{
	Cut_KnifeDown = 0,
	Cut_KnifeUp,
	Cut_KnifeRedown,

}OutlineCut;

// 0：常规 1：CNC锣铣（小刀旋转打大孔）
typedef enum 
{
	Type_Regular= 0,
	Type_CncCircle,

}OutlineType;

// 0：常规(图形中的点) 1：图形开始  2：图形结束
typedef enum 
{
	Group_Regular = 0,	// 0：常规(图形中的点)
	Group_Start,		// 1：图形开始
	Group_End,			// 2：图形结束

}OutlineGroup;

// 1：外切  2：内切   3：中心切
typedef enum 
{
	Side_Out = 1,		// 1：外切
	Side_In,			// 2：内切 
	Side_Center,		// 3：中心切

}OutlineSide;

#endif
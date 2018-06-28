#ifndef		_OtherShapeHelper_H_
#define		_OtherShapeHelper_H_

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/KnifeDll/KnifeClass.h"
#include "../../../include/TinyXml/tinyxml.h"

#include "../UIData/UIData.h"

#include <string>
#include <vector>

using namespace std;

using namespace TinyXml;

typedef std::map<std::string,std::string>	OutlineParamPoint;		//异形点（公式形式）
typedef std::vector<OutlineParamPoint>		OutlineParamShape;		//异形（公式型式）
typedef std::map<int,OutlineParamShape>		OutlineParamShapeDict;	//异形字典（公式型式）

#ifndef PARAM_POINT
#define PARAM_POINT
struct param_point
{
	std::string _name;
	std::string _value;
};
#endif

//异形内部使用点信息
#ifndef POINT_FLOAT
#define POINT_FLOAT
class OtherPointFloat
{
public:
	OtherPointFloat()
	{
		x = 0.0f;
		y = 0.0f;
	}

	OtherPointFloat(IN const OtherPointFloat &point)
	{
		x = point.x;
		y = point.y;
	}

	OtherPointFloat(IN float _x, IN float _y)
	{
		x = _x;
		y = _y;
	}

	OtherPointFloat operator+(IN const OtherPointFloat& point) const
	{
		return OtherPointFloat(x + point.x,
			y + point.y);
	}

	OtherPointFloat operator-(IN const OtherPointFloat& point) const
	{
		return OtherPointFloat(x - point.x,
			y - point.y);
	}

	BOOL Equals(IN const OtherPointFloat& point) 
	{
		float mindiff = 0.0001f;	//误差范围

		if ( -mindiff < (x - point.x) && (x - point.x) < mindiff )	//X重合
		{
			if ( ( -mindiff < (y - point.y) && (y - point.y) < mindiff ) )	//Y重合
			{
				return true;
			}
		}
		return false;
	}

public:

	float x;
	float X() const { return x; }
	void X(float val) { x = val; }
	float y;
	float Y() const { return y; }
	void Y(float val) { y = val; }
};
#endif

//异形内部使用线信息
#ifndef INFO_LINE
#define INFO_LINE
class InfoLine
{
public:
	InfoLine();
	InfoLine(IN const InfoLine& _line);

	InfoLine(IN const PointInfo& _begin, IN const PointInfo& _end);

	InfoLine(IN const PointInfo& _begin, IN const PointInfo& _end, float _r, int _dir,int _groupID,int _side,bool _visable);

	InfoLine operator=(IN const InfoLine& _line) const;

public:
	PointInfo begin;
	PointInfo end;
	float r;
	int dir;
	int groupID;
	int side;
	bool visable;
};
#endif


class OtherShapeHelper
{
public:

	OtherShapeHelper();

public:

	// 获得异形PointInfo 
	// x:50.0,y:30.0;  => {[ x:50.0 , y:30.0 ]}
	static string GetPointInfoStringFromPointInfo(vector<PointInfo> PointInfos);

	// 获得异形PointInfo 
	//  {[ x:50.0 , y:30.0 ]} => x:50.0,y:30.0;
	static std::vector<PointInfo> GetPointInfoFromPointInfoString(std::string PointInfo);

	//通过长宽构建轮廓
	static std::vector<PointInfo> getPointInfoRectangle(const std::string rotate, std::string& panelWidth,std::string& panelHeight );

	//////////////////////////////////////////////////////////////////////////
	// 构造点
	static PointInfo GenerateOnePointInfo( std::string str_x,std::string str_y,std::string str_r,std::string str_sign,std::string str_dir,std::string str_cut,std::string str_group,std::string str_side );

	//旋转板件（会改变板件长宽）
	static std::vector<PointInfo> getPointInfoRotate(const std::vector<PointInfo> pointInfos,const std::string rotate , std::string panelWidth,std::string panelHeight );

	//关于顶点旋转90度
	static std::vector<PointInfo> RotatePointInfoBackDegree90( std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen);
	//关于center旋转angle度
	static std::vector<PointInfo> RotatePointInfoDegree90(std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen);
	static std::vector<PointInfo> RotatePointInfoDegree180(std::vector<PointInfo> pointInfos, float fOrgXLen, float fOrgYLen);

	// 异形string 参数列表
	// ID:2016;gapWidth:10;gapHeight:20;   =>   { ID : 2016 , gapWidth : 10 , gapHeight : 20 }
	static std::vector<param_point> GetParamPointsFromOtherShapeString( std::string otherShapeString );

	// 异形string 参数列表
	// ID:2016;gapWidth:10;gapHeight:20;   =>   { ID : 2016 , PW : 50 , PD : 50 , gapWidth : 10 , gapHeight : 20 }
	static std::vector<param_point> GetParamPointsFromOtherShapeString( std::string otherShapeString, std::string panelWidth, std::string panelHeight );

	//根据长度整理参数的排序函数
	static bool sortFunctionByStrLen(const param_point & m1, const param_point & m2) ;
	//根据长度整理参数的排序函数, 参数按照长度，长到短
	static void sortParam_pointByStrLen(std::vector<param_point> & pa_p) ;

	//异形参数列表PW判断
	static bool isParamPointHasPanelWidth( const std::vector<param_point>& pa_p );
	//异形参数列表PD判断
	static bool isParamPointHasPanelHeight( const std::vector<param_point>& pa_p );

	//异形参数列表 插入数据
	static void PushParamPoint( std::vector<param_point>& pa_p, std::string name, std::string value );
	//异形参数列表 更新数据
	static void SetParamPoint( std::vector<param_point>& pa_p, std::string name, std::string value );
	//异形参数列表 读取数据
	static std::string GetParamPoint( std::vector<param_point>& pa_p, std::string name );

	//公共异形专用 参数列表旋转
	static void RotateOtherShapeParamPoint(std::vector<param_point>& pa_p,std::string rotate);
	//公共异形专用 异形旋转
	static std::vector<PointInfo> RotateOtherShapePointInfos(std::vector<PointInfo> m_pointInfos,std::string changeType ,std::string m_panelwidth, std::string m_panelheight);

	// 用异形string参数，替换公共异形参数点阵，计算参数点阵到普通点阵
	// ID:2016;PW:50;PD:60;gapWidth:10;gapHeight:20; + {[x:"PD-gapWidth",y:"PW-gapHeight"]} => {[ x:50.0 , y:30.0 ]}
	static std::vector<PointInfo> GetPointInfoFromPanelOutLineAndParamPoint(std::vector<std::map<std::string,std::string>> panelOutline, std::vector<param_point> pa_p);

	// 获得公共异形参数点阵
	// ID:2016;...  => {[x:"PD-gapWidth",y:"PW-gapHeight"]}
	static std::vector<std::map<std::string,std::string>> GetOtherShapePanelOutLineListFromOtherShapeID(std::string otherShapeID);

	//异形string中获取 ID数据
	static std::string GetOtherShapeId( const std::string& otherShapeID );
	//异形参数列表中获取 ID数据
	static std::string GetOtherShapeId( const std::vector<param_point>& pa_p );

	//把点从第四象限移到第一象限
	static void GetPointsToYPlus(std::vector<PointInfo>& pointInfos, const std::string rotate, std::string m_panelwidth, std::string m_panelheight);

	// 获得所有公共异形参数点阵
	// ID:2016;...  => {[x:"PD-gapWidth",y:"PW-gapHeight"]}
	static OutlineParamShapeDict GetOtherShapePanelOutLineList();

	// 异形string 转点阵
	// ID:2016;gapWidth:10;gapHeight:20;   =>    {[ x:50.0 , y:30.0 ]}
	static std::vector<PointInfo> GetPointInfoFromOtherShapeString( std::string otherShapeString, std::string panelWidth, std::string panelHeight);

	//异形参数列表PW PD删除
	static void removeParamPoint_PanelWidthPanelHeight( std::vector<param_point>& pa_p );

	// 参数列表 转 异形string 
	// { ID : 2016 , gapWidth : 10 , gapHeight : 20 }   =>   ID:2016;gapWidth:10;gapHeight:20;
	static std::string GetOtherShapeStringFromParamPoints( std::vector<param_point> pa_p );

	//通过点获得线段数组
	static std::vector<InfoLine> GetInfoLineFromPointInfo(const std::vector<PointInfo>& pointInfos );

	//复制点信息
	static void clonePointInfo(PointInfo& newPointInfo,const PointInfo& oldPointInfo);

	//根据线段在PDC上绘图
	static void DrawLinesInPDC( CDC* pDC, const std::vector<InfoLine>& lines,CPoint offsetPoint, float _scale );

	//比对点是否重合
	static bool isPointFloatEquals(float p1_x ,float p1_y, float p2_x , float p2_y);

	static CRect buildCRect(RectF rect);

	//通过两点以及圆弧和方向，得到圆弧线组成的参数（ rect,beginAngle,endAngle 输出参数）
	static void getArc(OtherPointFloat first, OtherPointFloat last,float radius,int sign,RectF& rect,double& beginAngle,double& sweepAngle);

	//角坐标两点追及问题，圆形圆弧上两点，某一点沿角坐标正方向追及另一点点需要移动的角度数
	static void getAngularCoordinate_DegreeCatchShort( OtherPointFloat center, OtherPointFloat one, OtherPointFloat other, float radius, double &beginAngle, double &sweepAngle );

	//角坐标两点追及问题，圆形圆弧上两点，一点沿角坐标正方向追及第二点需要移动的角度数
	static void getAngularCoordinate_DegreeCatch( OtherPointFloat center, OtherPointFloat first, OtherPointFloat last, float radius, double &beginAngle, double &sweepAngle );

	//两点算角度  
	static double getRadFrom2Point(OtherPointFloat pointCenter,OtherPointFloat pointDis) ;
	
	//弧度转角度  
	static double getAngleFromRad(double rad) ;

	//得到圆弧的圆心
	static void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);

	//生成圆心，sign=0圆心在线段方向的左手边，sign=1圆心在线段方向的右手边
	// ...................
	// .......last........
	// ......./|\.........
	// .sign=1.|..........
	// .@......|...@......
	// ........|...sign=2.
	// ........|..........
	// ........|..........
	// ......first........
	// ...................
	static OtherPointFloat getCircleCenter(OtherPointFloat first, OtherPointFloat last , float radius , int sign);

	//清除sign等于-1的点
	static std::vector<PointInfo> ClearUnnessaryPointInfo(const std::vector<PointInfo> pointInfos);

	// 获得异形PointInfo清除sign -1点 
	static std::vector<PointInfo> ClearPointInfoSignNegative( std::vector<PointInfo> PointInfos );

	static std::vector<PointInfo> MoveOutlinePoints(std::vector<PointInfo> vOutlinePoint, float fXOffset, float fYOffset);

	//获得分组
	static std::vector<std::vector<InfoLine>> GetInfoLineGroup(const std::vector<InfoLine> infoLine);

	const static int ArcSignLeft;
	const static int ArcSignRight;
private:
	static bool isLoadPanelOutlineList;
	static TinyXml::TiXmlDocument docPanelOutlineList;
};

vector<vector<PointInfo>> SplitAllVertexToGroup(vector<PointInfo>& vAllOutlineVertex);

OutlineLine GenLineByVertex(PointInfo& vertex, PointInfo& vertexNext);

//int GetLineType(int nSign);

void CalcMillingTrackByOutlineLines(vector<OutlineLine>& vOneSegmentLines, float fKnifeDia, vector<OutlineLine>& vOneSegmentLines_MillingTrack);

void OutlineGroupRotateClockwise90(vector<vector<OutlineLine>>& vMillingOutlineGroup, float fOrgXLen, float fOrgYLen);

void SloveZeroLine(vector<OutlineLine>& vLine);

//LineType GetLineType(int nLineSign);

#endif
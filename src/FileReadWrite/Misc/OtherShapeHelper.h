#ifndef		_OtherShapeHelper_H_
#define		_OtherShapeHelper_H_

#include "../../../include/KnifeDll/KnifeClass.h"
//#include "../../../include/TinyXml/tinyxml.h"
#include "../Misc/XmlHandlePlus.h"

#include <string>
#include <vector>

using namespace std;

#ifndef PARAM_POINT
#define PARAM_POINT
struct param_point
{
	std::string _name;
	std::string _value;
};
#endif

class AFX_EXT_CLASS OtherShapeHelper
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

	static std::vector<PointInfo> GetPointInfoReverse( const std::vector<PointInfo> pointInfos );

	//获得翻转后轮廓（左右翻转）
	static std::vector<PointInfo> getPointInfoBack(const std::vector<PointInfo> pointInfos,const std::string& panelWidth,const std::string& panelHeight);
private:
	static TinyXml::TiXmlDocument docPanelOutlineList;
	static bool isLoadPanelOutlineList;
};

#endif
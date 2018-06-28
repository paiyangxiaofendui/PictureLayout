#pragma once

//#include <vector>
#include <string>
//#include <map>
//#include "KnifeClass.h"

using namespace std;
 
#ifndef PARAM_POINT
#define PARAM_POINT
struct param_point
{
	string _name;
	string _value;
};
#endif // PARAM_POINT


//struct p_show
//{
//	float _x;
//	float _y;
//};

class YHInfoTools
{
public:
	YHInfoTools(void);

	//////////////////////////////////////////////////////////////////////////
	//工具函数
public:
	//(用数字替换字母)例如：PW+PD-PK
	static string replace_allstr(string str,const string old_value,const string new_value);

	////翻转后的点位信息
	//static void getOtherAllPoint(std::vector<AllPoint>& otherallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width);

	////得到反面点位信息
	//static void getAllDPoint(std::vector<DAllPoint>& rdallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width,float _OtherKerfWidth);

	////得到点位信息
	//static void getAllPoint(std::vector<AllPoint>& rallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width);

	////得到反面槽信息函数
	//static void getdAllSlot(std::vector<AllSlot>& rdallslot,CString slotinfo,CString rotateinfo,CString panelinfo);

	////得到反面孔信息
	//static void getdAllVec(std::vector<AllVec>& rdallvec,CString vecinfo,CString panelinfo);

	////得到正面槽信息
	//static void getAllSlot(std::vector<AllSlot>& rallslot,CString slotinfo,CString rotateinfo,CString panelinfo);

	////得到正面孔信息
	//static void getAllVec(std::vector<AllVec>& rallvec,CString vecinfo,CString panelinfo);

	////获得材料（厚度-材质）中的厚度
	//static float GetThinFromThinMatrial(std::string thin_matrial);

	////获得材料（厚度-材质）中的材质
	//static std::string GetMatrialFromThinMatrial(std::string thin_matrial);

	////获得优化参数
	//static OptimizeParam YHInfoTools::GetOptimizParamFromFile();


	//////根据长度整理参数的排序函数
	//static bool sort_baseStrLen(const param_point & m1, const param_point & m2) ;

	//////根据长度整理参数的排序函数, 参数按照长度，长到短
	//static void sortParam_pointByStrLen(std::vector<param_point> & pa_p) ;

	////根据传入数组，清理所有条码中存在的# %加数字
	//static void BarCodeFixer(std::vector<KnifeClass *> &_knifeclasses);


};


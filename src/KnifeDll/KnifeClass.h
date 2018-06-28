#ifndef		_KnifeClass_H_
#define		_KnifeClass_H_



#pragma once
#include <string>
#include <vector>
#include <map>
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"
using namespace std;

class Panel;
class Component;



class __declspec(dllexport) KnifeClass
{
public:
	KnifeClass(void);
	~KnifeClass(void);

	void setKerfwidth(float kerfwidth){m_kerfwidth = kerfwidth;}
	void setDeburringwidth(float deburringwidth){m_deburringwidth = deburringwidth;}
	void setPanelHeight(float panelheight){m_panelheight = panelheight;}
	void setPanelWidth(float panelwidth){m_panelwidth = panelwidth;}
	void setThickness(float thickness){_thickness = thickness;}
	void setThinMatrial(string ThinMatrial){_thin_matrial = ThinMatrial;}
	void setPanelMatrial(string Matrial){m_material = Matrial;}
	void setOptimizeParam(OptimizeParam optimizeparam){m_optimizeparam = optimizeparam;}

	string getPanelMatrial(){return m_material;}
	float getThickness(){return _thickness;}



	void SetLeftParam(float leftthin,float spanelsize,float leftsp){m_leftthin = leftthin;m_spanelsize = spanelsize;m_leftsp = leftsp;}

	void setKnifeType(std::vector<KnifeType> Knifetype);
	void setvecchangexy(float _x,float _y){m_vecchangex = _x;m_vecchangey = _y;}
	void setvecchangexy(vector<float> cxy);

	// 网络验证
	static bool InternetChecking(void);

	// 访问http网页
	static bool PostHttpPage(int port,const std::string & hostName, const std::string & pathName, const std::string & postData);

public:
	// 导出NC相关函数
	bool OutputdvsGcodem(	Panel* panel,			string file,			string file1,				int cutprint,
							bool doublecut,			bool usecoor,			int coortype,				int PanelNum,
							string str1,			string str2,			vector<P_Info> _pinfo,		string grouptype,
							string file_change,		int dir,				vector<float> _df,			string f_header,
							string f_ender,			int _merge,				string _mainktgcodeb,		string _mainktgcodee,
							string _vecktgcodeb,	string _vecktgcodee,	int total_check_panel_num,	BOOL panel_comments);

	bool OutputGcode(		Panel* panel,			std::string file,		std::string file1,			int cutprint,
							bool doublecut,			bool usecoor,			int coortype,				int PanelNum,
							string str1,			string str2,			vector<P_Info> _pinfo,		string grouptype,
							string file_change,		string _mainktgcodeb,	string _mainktgcodee,		string _vecktgcodeb,
							string _vecktgcodee,	int _vecnochange,		bool cutcore,				string node_dvcut,
							bool LabelInReverseSide, int total_check_panel_num ,  int decimal_place,	BOOL panel_comments);

	


	vector<gcodeCoor> pathoptimize(vector<Component*>& CpnList);

	vector<PointInfo> outdirpoint(vector<PointInfo> oldpoint,float dis,int & _pos,std::string panel_info);



	void OutputLabel(Panel* panel,					string CurCutFileName,			string CurPrintFileName,
					int cutprint,					int PanelNum,					string NextPrintFileName,	
					string NextCutFileName,			vector<P_Info> _pinfo,			ofstream& wfile,
					vector<gcodeCoor>& _newcoor,	string node_dvcut,				bool LabelInReverseSide);



	void GetShortestPath(std::vector<pos> & poses,pos & beginpos,std::vector<pos> & newposes);
	void GetChangePath(std::vector<pos> poses,std::vector<pos> & newposes);



	std::string ChangeGroup(int _x,int _y,std::string _changestr);
	std::string ChangeGroup(std::vector<int> _kn,std::string _changestr);


	std::string replace_allstr(std::string str,const std::string old_value,const std::string new_value);

	vector<gcodeCoor> rollsetoptimize(vector<gcodeCoor> _newcoor);
	bool Line_intersection(float begin_px1,float begin_py1,float end_px1,float end_py1,float begin_px2,float begin_py2,float end_px2,float end_py2);

	void PrintNumber2Array(char* str, int str_len,int decimal_place, float val);


public:
	// 新增函数

	// 判断谁比较小
	static bool ComponentCompareID(const Component* pfirst, const Component* psecond);


	// 计算公式值。 原来是引用Construction的，导致引入大量的外部库，不能接受
	static float  calcFormula(std::string formula);

	// 坐标转换，将新算法的数据（平面直角坐标系第一象限）转换成原有雕刻机的坐标格式（平面直角坐标系第四象限、屏幕坐标系）
	void TransformMachiningInfoToOld_UpperFace(Panel* pPanel, vector<Component*>& CpnList, vector<gcodeCoor>& NewCoorList, 
												vector<AllPoint>& AllPointList, vector<AllSlot>& AllSlotList, vector<AllVec>& AllVecHoleList,
												map<int,PointFloat>&	LabelPositionMap, vector<CutPath>& CutPathList, vector<CutPath>& PreCutPathList);

	void TransformMachiningInfoToOld_DownerFace(Panel* pPanel, vector<Component*>& CpnList, vector<gcodeCoor>& NewCoorList,
												vector<AllSlot>& DownerAllSlotList, vector<AllVec>& DownerAllVecHoleList);


	// 计算一块大板的所有小板下刀点
	 void SetKnifeDownPos(Panel* pPanel);



public:
	float m_kerfwidth;									// 锯缝宽度
	float m_deburringwidth;								// 修边宽度
	float _thickness;									// 厚度，，什么的厚度？？？
	std::string _thin_matrial;							// 薄料
	std::string m_material;								// 材质
	vector<p_Component> m_com;							// 零件容器
	vector<AllPoint> m_allpoint;						// 所有的原始点
	vector<DAllPoint> m_disallpoint;					// 所有的刀补点
	vector<AllPoint> m_otherallpoint;					// 其它的原始点
	vector<AllSlot> m_allslot;							// 正面的槽
	vector<AllVec> m_allvec;							// 正面的孔
	vector<AllSlot> m_dallslot;							// 刀补的槽
	vector<AllVec> m_dallvec;							// 反面的孔
	float m_vecchangex;
	float m_vecchangey;
	vector<float> m_cxy;
	float m_panelwidth;									// 板宽
	float m_panelheight;								// 板高


	vector<CutPath> m_PreCutPath;						// 切一刀，用于最后一块板修边
	vector<CutPath> m_cutpath;							// 切一刀，用于余料切割
	map<Panel *,vector<gcodeCoor>> m_handnewcoor;		// 优化切割得出的G代码
	GcodeParam m_gcodeparam;							// G代码参数
	std::vector<KnifeType> m_knifetype;					// 刀类型
	OptimizeParam m_optimizeparam;						// 优化参数
	map<PanelNode *,int> m_orderpanel;					// 板件序号
	int m_max_merge_num;								// 最大排样板数
	std::string m_cut_id;								// 刀号
	bool opstop;										// 停止优化
	//int kc_num;
	float m_leftthin;								
	float m_spanelsize;
	float m_leftsp;


public:
	CString m_strOrderNum;
	static int m_nOutputTBNum; // 恩德输出文件序号
	static CList<CString> m_TBStringList;

public:
	// 恩德贴标
	static void ResetOutputTBNum(void);			// 初始化恩德输出贴标序号
	void SetInputOrderNum(CString str);			// 设置订单号
	CString GetInputOrderNum(void);				// 获取订单号

	// 小板下两刀
	bool m_bSmallPanelCutTwiceEnable;			// 判断小板的标准长度
	float m_SmallPanelStandardSize;				// 判断小板的标准长度
	float m_SmallPanelSecondSpeed;				// 小板第二次的速度
	float m_SmallPanelReserveHeight;			// 小板预留高度切两次时使用

	void InitSmallPanelCutTwicePara(void);		// 初始化小板下两刀参数
	void SetSmallPanelCutTwicePara(bool enable, float small_size, float reserve_height, float second_speed);		// 设置小板下两刀参数
	void GetSmallPanelCutTwicePara(bool& enable, float& small_size, float& reserve_height, float& second_speed);	// 获取小板下两刀参数



	// 导出G代码时，切割一块正常板件
	void OutputGCode_KnifeOnePanel(vector<PointInfo>& newpoint, int pos, GcodeParam& gcodeparam, vector<PointInfo>& points , std::ofstream& wfile, 
		int k_l, float src_x,  float src_y, std::string& info ,float& knift_lenght, int& coortype, bool& no_cutpoint, bool KnifeOffset,
		float& knife_spindlespeed , float& oldknife_spindlespeed, float& knife_feedrate, float& knife_endspeed, float& knife_overspend , float& knife_facespeed,  int decimal_place);			

	// 导出G代码时，切割一块板小板，需要两次下刀
	void OutputGCode_KnifeOneSmallPanel(vector<PointInfo>& newpoint, int pos, GcodeParam& gcodeparam, vector<PointInfo>& points , std::ofstream& wfile, 
		int k_l, float src_x,  float src_y, std::string& info ,float& knift_lenght, int& coortype, bool& no_cutpoint, bool KnifeOffset,
		float& knife_spindlespeed , float& oldknife_spindlespeed, float& knife_feedrate, float& knife_endspeed, float& knife_overspend , float& knife_facespeed,  int decimal_place);	


	// 板件分割界限，大板开两次
	float m_ThicknessThresholdSize;				// 厚度界限
	float m_ThicknessThresholdSecondSpeed;		// 超过了厚度界限的速度
	float m_ThicknessThresholdReserveHeight;	// 超过了厚度界限的预留高度，切两次时使用

	void InitThicknessThresholdPara(void);		// 初始化厚度界限参数
	void SetThicknessThresholdPara(float threshol_size, float reserve_height, float second_speed);		// 设置厚度界限参数
	void GetThicknessThresholdPara(float& threshol_size, float& reserve_height, float& second_speed);	// 获取厚度界限参数


	// 异形刀径，暂时使用
	float m_AbnormalKnifeWidth;					// 异形刀径宽度，预设，默认用于开料

	void InitAbnormalKnifeWidth(void);			// 初始化异形刀径
	void SetAbnormalKnifeWidth(float width);	// 设置异形刀径
	void GetAbnormalKnifeWidth(float& width);	// 获取异形刀径

	float getPanelWidth(){return m_panelwidth;}
	float getPanelHeight(){return m_panelheight;}

#if (NEW_PROGRAM == 1)

public:
	// 贴标位置
	float m_LabelWidth;	// 贴标宽
	float LabelWidth() const { return m_LabelWidth; }
	void LabelWidth(float val) { m_LabelWidth = val; }

	float m_LabelHeight;	// 贴标高
	float LabelHeight() const { return m_LabelHeight; }
	void LabelHeight(float val) { m_LabelHeight = val; }

	map<int,PointFloat>	m_LabelPosition;	// 贴标位置结构
	map<int,PointFloat> LabelPosition() const { return m_LabelPosition; }
	void LabelPosition(map<int,PointFloat> val) { m_LabelPosition = val; }
#endif

public:
	// 少刀无法打孔、拉槽，文本提示所需变量  
	bool m_bReportMsg;				// 信息提示使能

	void EnaleMsgReport();			// 使能信息提示
	void UnaleMsgReport();			// 关闭信息提示

};
#endif
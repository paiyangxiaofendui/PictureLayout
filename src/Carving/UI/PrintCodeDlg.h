#pragma once
#include <vector>
#include <map>
#include "ImageListCtrl.h"
//#include "SelectPrintDlg.h"
#include "afxwin.h"
#include "../Resource.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

using namespace TinyXml;

// CPrintCodeDlg dialog
extern class CPrintDialog;
struct code_all_info
{
	std::vector<CString> m_codeinfo;
	CString m_def;
	float m_width;
	float m_height;
	CString s_width;
	CString s_height;
	CString m_cstno;
};

struct panel_picture
{
	float _width;
	float _height;
	float left_edge;
	float right_edge;
	float up_edge;
	float down_edge;
	std::string s_width;
	std::string s_height;
	int _dir;
	int nRotatedAngle;
};

struct recode_print
{
	std::map<std::string,std::string> printtext;
	std::string code_info;
	std::string logo_path;
	panel_picture _picture;
	std::string code_info2;
};

struct xml_module
{
	std::string _x;
	std::string _y;
	std::string _fontsize;
	std::string _fontname;
	std::string _frontname;
	std::string _leftstr;
	std::string _rightstr;
	std::string _used;
};

struct def_pos
{
	int x;
	int y;
};



class CPrintCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintCodeDlg)

public:
	CPrintCodeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrintCodeDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_PRINTCODEDLG };
	std::vector<std::vector<std::string>> m_pall_cominfo;
	CImageList image_list;
	wstring ANSIToUnicode(const string& str);
	void Wchar_tToString(std::string& szDst, wchar_t *wchar);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDaYinAnNiu();
	CImageListCtrl m_codelist;
	std::map<int,code_all_info> m_list_info;

	afx_msg void OnPaint();

	void BuildCodeBmp();

	//通过路径获得图片尺寸
	void getImageWidthHeight(CString path , int& image_width, int& image_height);

	void Paint_image(CDC* pDC, CString path ,int rotate, int pos_x, int pos_y );

	//生成板件图片
	void Paint_PanelImage(CDC* pDC, int offset_x, int offset_y,std::string s_width, std::string s_height, CString fontName,int fontSize,
		CString edge_img_width_0,CString edge_img_width_1,CString edge_img_height_0,CString edge_img_height_1,
		CString arrow_img_path,CString shape_img_path);

	void BuildOneCodeImage( CString sourceImagePath, CString destImagePath, /*hg3d::CXmlNode * print_settings*/TiXmlElement* elmPrintSetting, recode_print& printInfomation,bool dash=false);

public:
	BOOL PrintPic();
	void OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight);


	//长内容分段截取扩展（源字符串 前后分别截取n个字符长度）
	void BuildCodeExtend(std::map<std::string,std::string>& dst,const std::string keyname);

	void TransitionPrint(TiXmlElement* elmPrintSetting);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnExportPanelA4();			//打印标签，大板是大图
	int MyBarcode(CDC* cdc,int x1,int y1,int y2,int codetype,int ipen,const char * cCode);
	int GetBarcodeLen(int ipen,const char * cCode);
	afx_msg void OnDaoChuAnNiu();

	void ExportPanelPic(bool isPanelUseBigPic);

	void ScaleSaveImage(double scale, CString srcFile, CString dstFile );

	CComboBox m_comboScaling;
//	afx_msg void OnCbnEditchangeBqcombo();
	afx_msg void OnCbnSelchangeBqcombo();
	double getDaoChuBiaoQianBiLi();
	void sortFile(CString file_path ,CString prefix_name);

public:
	//大板信息
	int m_panelPictureNumCount;				//大板图片数量
	std::vector<int> m_panelNumList;		//大板关联序号
	std::vector<CString> m_panelSmallPicPathList;	//大板小图片地址
	std::vector<CString> m_panelBigPicPathList;	//大板大图片地址
	bool m_panelNCCode;						//是否生成了大板NC码

	//小板信息
	std::vector<recode_print> m_print_info;	//小板打印用数据（使用）
	std::vector<CString> m_print_path;		//小板打印用目录（使用）
	std::vector<print_info> m_info;			//小板打印信息（输入）

	std::vector<int> m_vComponentLableID;
};

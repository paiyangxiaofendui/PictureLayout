#ifndef			_ABTDialog_h_ 
#define			_ABTDialog_h_


#include "Resource.h "

#include <string>
using namespace std;

// 绘图GDI+
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")



// TinyXml库
#include "../../include/tinyxml/tinyxml.h"
#ifdef DEBUG
#pragma  comment(lib, "../../lib/tinyxmld.lib")
#pragma  message(".......................................当前为debug版本")
#else
#pragma  comment(lib, "../../lib/tinyxml.lib")
#pragma  message(".......................................当前为release版本")
#endif




// 外部类声明

class Graphics;
class Figure;

#define Program_2_LiJun					(0)			// 给李俊的程序，需要保存xml和截图

#define		MIN_MOVE_STEP				(2)			// 最小移动步长 单位：像素


#define		ZOOM_STEP					(30)		// 每次缩放的步长
#define		ZOOM_MAX_RANGE				(5000)		// 最大缩小，板件宽高为5000mm
#define		ZOOM_MIN_RANGE				(10)		// 最大放大，板件宽高为50mm



#define		BK_CLOLOR					(Color::White)
#define		CROSS_CLOLOR				(Color::LightGray)
#define		OUTLINE_CLOLOR_UNSELECT		(Color::Blue)
#define		OUTLINE_CLOLOR_SELECT		(Color::Red)
#define		SHAPEPOINT_CLOLOR			(Color::Red)
#define		FIGURE_RECT_CLOLOR			(Color::Gray)


class AFX_EXT_CLASS ABTDialog : public CDialog
{
public:
	ABTDialog(CWnd* pParentWnd = NULL);
	~ABTDialog();

private:
	enum {IDD = IDD_ABT_DLG};

private:
	// 菜单资源
	CMenu m_Menu2;
	HACCEL m_hAccel;

public:
	// 绘图使用

	CRect m_ShowRect;			//界面上绘画板件的区域
	CPoint m_ShowRectCenter;	//界面上绘画板件的区域中心
	int m_nPanelWidth;			//板件纵向尺寸
	int m_nPanelLength;			//板件横向尺寸

	BOOL m_ShowShapePointFlag;
	BOOL m_ShowOutlineFlag;
	BOOL m_ShowFigStartDirFlag;


public:
	// XML数据 
	string m_strXmlNode;
	void SaveXml(CString FilePath);
	void SaveJPG(CString FilePath);

public:
	void DrawWindowBackground(Gdiplus::Graphics* g);
	void ShowRect_DrawBackground(Gdiplus::Graphics* g, CRect& rt);
	void ShowRect_DrawCross(Gdiplus::Graphics* g, CRect& rt);


	void ShowRect_DrawFgure(Gdiplus::Graphics* g);			// 画图形
	void ShowRect_DrawFgureRect(Gdiplus::Graphics* g);		// 画选中图形矩形
	void ShowRect_DrawFgureDir(Gdiplus::Graphics* g);		// 画图形方向
	void ShowRect_DrawShapePoint(Gdiplus::Graphics* g);		// 画形状点
	void ShowRect_DrawPolygon(Gdiplus::Graphics* g);		// 画点

	void ShowRect_TransCoorFromCad2Window(float cx, float cy, int& wx, int& wy);		// CAD->窗口 坐标转换 
	void ShowRect_TransCoorFromCad2Window(float cx, float cy, float& wx, float& wy);	// CAD->窗口 坐标转换 
	void ShowRect_TransCoorFromCad2Window(CRect cad_rt, CRect& win_rt);					// CAD->窗口 坐标转换

public:
	 TinyXml::TiXmlElement* m_pPanelOutlineNode;
	 CString m_strPanelOutlineNode;


public:
	// 判断图形选中
	BOOL JudgeIfFigureSelect(Figure* pFig, CPoint pt);
	void SelectFigure(CPoint pt);


public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOpenDxf();
	afx_msg void OnBnClickedBtnReverseFigure();
	afx_msg void OnBnClickedBtnSaveXml();



	afx_msg void OnBnClickedOk();
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnMove2StandardPosition();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnScreenCapture();
	afx_msg void OnAccelSelectAllFigure();
	afx_msg void OnUpdateAccelSelectAllFigure(CCmdUI *pCmdUI);
	afx_msg void OnSelectAllFigure();
	afx_msg void OnBnClickedButtonParallel();
	afx_msg void OnDelFigure();
	afx_msg void OnShowShapePoint();
	afx_msg void OnShowOutline();
	afx_msg void OnUpdateShowShapePoint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowOutline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowFigureStartDir(CCmdUI *pCmdUI);
	afx_msg void OnShowFigureStartDir();
	afx_msg void OnMenuImportRequirement();
};

#endif


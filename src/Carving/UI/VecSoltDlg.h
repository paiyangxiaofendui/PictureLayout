#pragma once
#include "../Resource.h"
//#include <vector>
//#include "OtherShape.h"
//#include "hgCst/Construction.h"
//#include "hg3d/Utils.h"
// CVecSoltDlg dialog
//struct SOLT
//{
//	float _x;
//	float _y;
//	float _width;
//	float _height;
//	float _depth;
//	int _direction;
//};
//struct VEC
//{
//	float _x;
//	float _y;
//	float _r;
//	float _depth;
//};
//
//struct _show
//{
//	float _x;
//	float _y;
//};


class Component;
/************************************************************************/
/* 板件预览窗口（预览这个板件正反面孔槽以及异形）                       */
/************************************************************************/
class CVecSoltDlg : public CDialogEx//画出板件槽和孔信息
{
	DECLARE_DYNAMIC(CVecSoltDlg)

public:
	CVecSoltDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVecSoltDlg();

// Dialog Data
	enum { IDD = IDD_VECSOLTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//std::string m_solt;
	//std::string m_vec;
	//std::string m_othershape;
	//float m_width;
	//float m_height;
	//vector<AllPoint> m_allpoint;
	//std::vector<_show> showps;
	//std::string m_ra;
	//std::string m_dsolt;
	//std::string m_dvec;
	//std::vector<PointInfo> m_pointInfos;

public:
	//static std::vector<SOLT> TraniferSolt(std::string m_solt);
	//static std::vector<VEC> TraniferVec(std::string m_vec);
	//static std::vector<SOLT> TraniferdSolt(std::string m_dsolt);
	//static std::vector<VEC> TraniferdVec(std::string m_dvec);
private:
	//void getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);
	//std::vector<SOLT> v_solt;
	//std::vector<VEC> v_vec;
	//std::vector<SOLT> v_dsolt;
	//std::vector<VEC> v_dvec;
	//std::string replace_allstr(std::string str,const std::string old_value,const std::string new_value);
	//float _scale;
	//int move_x;
	//int move_y;

	//bool m_click;
	//int m_coldx;
	//int m_coldy;
public:
	afx_msg void OnPaint();
	//afx_msg void OnBnClickedButton2();
	//afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButton3();
	//afx_msg void OnBnClickedButton4();
	//afx_msg void OnBnClickedButton5();
	//afx_msg void OnBnClickedButton6();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	//std::map<std::string,std::string> GetItem() const { return m_item; }
	//void SetItem(std::map<std::string,std::string> val) { m_item = val; }
	void SetComponent(Component* pComponent) { m_pComponent = pComponent;}

private:
	//std::map<std::string,std::string> m_item;

protected:
	CRect GetDrawArea();
	void DrawDiagram();
	bool DeltaScale(float fDeltaScale);
	void SetScale(float fTargetScale);
	void ResetScaleAndPos();

	Component* m_pComponent;
	CBitmap m_bmpDiagram;

	float m_fViewingScale;
	POINT m_ptDistanceBetweenViewingAndDiagramCenter;
	HCURSOR m_hCursorHand;
	bool m_bDragging;
	POINT m_ptDragStartPos;
};

#pragma once
#include "../resource.h"
#include "../UIData/UIData.h"


// CDlgRequirement_PreCombine 对话框

class CDlgRequirement_PreCombine : public CDialog
{
	DECLARE_DYNAMIC(CDlgRequirement_PreCombine)

public:
	CDlgRequirement_PreCombine(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRequirement_PreCombine();

// 对话框数据
	enum { IDD = IDD_DLG_REQUIREMENT_PRECOMBINE };

	void SetPreCombine(vector<ComponentInputItem>& vPreCombineComponentItem);
	//int GetItemCount();
	vector<PreCombineComponent> GetPreCombine(){return m_vPreCombineComponent;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	CRect GetPanelViewRect();
	CRect GetComponentUIRect(PreCombineComponent& thePreCombineComponent);
	float GetScale();
	void RotateSelectedComponent();
	void RemoveSelectedComponent();
	bool IsAttachMode();
	void MovingComponent_OnMouseMove(CPoint point);

	//vector<vector<ComponentInputItem>> m_vPreCombineItem;
	vector<PreCombineComponent> m_vPreCombineComponent;
	int m_nSelectedItem;
	CPoint m_ptDragStartPos;
	PointFloat m_ptPreCombineComponentDragStartPos;
	bool m_bDragging;
};

#pragma once


#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "listboxclipboard.h"
#include "./UICommon/DialogChildBase.h"
#include "./UICommon/PngButton.h"

// CDlgResult 对话框

class CDlgTotalResult;
class Component;
class PanelViewingParam;

enum RemainderCutingSettingState{CUTTING_SETTING_STATE_START, CUTTING_SETTING_STATE_END};

class CDlgResult : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESULT };

	void ResetResultDlg();
	void RefreshOptimizeResult();
	void StopDrawing();
	vector<Panel*> GetPanels(bool bChecked);
	bool IsPanelChecked(Panel* pPanel);
	PanelViewingParam* GetSelectedItemViewingParam();
	void ResetPastingState();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnRefreshPanelView(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMenuResetMachiningOrder();
	afx_msg void OnMenuCutComponent();
	afx_msg void OnMenuCopyComponent();
	afx_msg void OnMenuPasteComponent();
	afx_msg void OnMenuRemoveComponent();
	afx_msg void OnMenuRemainderCutting();
	afx_msg void OnMenuRotatePastingComponent();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBtnExportDxf();
	afx_msg void OnBtnPrintCurDiagram();
	afx_msg void OnBtnPrintCurLabel();
	afx_msg void OnBtnExportCurMaterialList1();
	afx_msg void OnBtnExportCurMaterialList2();
	afx_msg void OnLbnSelchangeClipBoard();
	afx_msg LRESULT OnShowOrHideClipboard(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	CRect GetPanelViewRect();
	//void DrawDiagram(PanelViewingParam* param);
	void DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, PanelViewingParam& param);
	void DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color = Color::Red);
	//void DrawArc(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Pen& pen);
	void DrawMachiningInfo(Graphics& g, CRect rcPanel, float fScale, Component* pComponent);
	void DrawDetail(Graphics& g, CRect rcComponent, float fScale, Component* pComponent);
	void DrawRemainderCuting(Graphics& g, CRect rcPanel, float fScale, Panel& thePanel);
	//void DrawDiagram();
	CRect GetComponentUIRect(float fPanelXLen, float fPanelYLen, float fComponentX, float fComponentY, float fComponentXLen, float fComponentYLen);
	float GetDrawingScale(float fPanelXLen, float fPanelYLen);

	void AddComponentToClipboard(Component* pComponent);
	//void ShowClipboard();
	void ShowOrHideClipboard();
	void DrawComponentToPos(Component* pComponent, CPoint ptComponent, bool bDrawOKIcon, Panel* pPanel);
	bool IsPosCanPasteComponent(Panel* pPanel, Component* pComponent, PointFloat ptComponentUILeftTop);
	PointFloat ConvertUIPosToPanelPos(Panel* pPanel, CPoint ptInUI);
	CPoint ConvertPanelPosToUIPos(Panel* pPanel, PointFloat ptInPanel);
	void CancelPasting();
	bool IsAttachMode();
	//CPoint GetAttachUIPos_Component(CPoint ptMouseInUI, float fComponentXLen, float fComponentYLen);
	PointFloat GetAttachPanelPos_Component(CPoint ptMouseInUI, float fComponentXLen, float fComponentYLen);
	void RotatePastingComponent();
	void PastingComponent_OnMouseMove(CPoint point);

	void DrawRemainderCuttingToPos(CutPath& theRemainderCutting, Panel* pPanel);
	void StopRemainderCutting();
	PointFloat GetAttachPanelPos_RemainderCutting_Start(CPoint ptMouseInUI);
	PointFloat GetAttachPanelPos_RemainderCutting_End(CPoint ptMouseInUI, PointFloat ptStartInPanel);

	CDlgTotalResult* m_pDlgTotalResult;
	//bool m_bDrawPanel;
	//Image* m_pImg;
	CPoint m_ptRClicked;

	CListBoxClipBoard m_lbClipBoard;
	Component* m_pPastingComponent;
	bool m_bNeedToEraseOldPastingComponent;
	CPoint m_ptOldPastingComponent;
	bool m_bOldPastingComponentOK;
	PointFloat m_ptfAttachPosInPanel;

	CutPath* m_pRemainderCutting;
	bool m_bNeedToEraseOldRemainderCutting;
	int m_nRemainderCuttingSettingState;

	CPngButton m_btnExportDxf;
	CPngButton m_btnPrintCurDiagram;
	CPngButton m_btnPrintCurLabel;
	CPngButton m_btnPrintCurMaterialList1;
	CPngButton m_btnPrintCurMaterialList2;
};

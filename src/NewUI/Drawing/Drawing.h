#pragma once
#include "../Work/WorkDef.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../UIData/UIData.h"

#define A4_TEMPLATE_X 1188
#define A4_TEMPLATE_Y 840

class Panel;
class Component;

struct DrawingPanelParam
{
	DrawingPanelParam()
		:m_bDrawMachiningInfo(false)
		,m_color(RGB(0,0,0))
		,m_bDrawDeberredPanel(false)
	{
	}

	int m_nGap;
	bool m_bDrawMachiningInfo;
	DWORD m_color;
	bool m_bDrawDeberredPanel;
};

bool DrawPanelInfoToDC(HDC hdc, Panel* pPanel, float org_scale, int rightmove, int bottommove, int num, /*CFont * font*/PanelDiagramPrintingStyle eStyle);

void DrawPanelDiagramToA4(HDC hdc, Panel* pPanel, CRect rcDrawingArea, int nPanelID, PanelDiagramPrintingStyle eStyle = PanelDiagramPrintingStyle::PRINT_STYLE_A4_1);

void DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color = Color::Black);

void DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, DrawingPanelParam& param);

void DrawComponent(CDC* pDC, Component* pComponent, CRect rcDrawArea, DrawingPanelParam& param);

void DrawMachiningInfo(Graphics& g, CRect rcComponent, float fScale, Component* pComponent);

void DrawMachiningOrder(CDC* pDC, Panel* pPanel, CRect rcDrawArea, int nGap, int nTextSize);

void DrawPanelSummary(CDC* pDC, Panel* pPanel, CRect rcDrawArea, int nTextSize, int nPanelID);

void DrawBarcodeList(CDC* pDC, Panel* pPanel, CRect rcDrawArea, int nTextSize);

void DrawComponentInPanel(CDC* pDC, Component* pComponent, CRect rcDrawArea, DrawingPanelParam& param, float fPanelXLen, float fPanelYLen);

void DrawOutlineLineInPanel(CDC* pDC, vector<vector<OutlineLine>>& vMillingOutlineGroup, float fComponentX, float fComponentY, CRect rcDrawArea, DrawingPanelParam& param, float fPanelXLen, float fPanelYLen);

void DrawPanelDiagram_ComponentLabel(HDC hdc, int nImageXLen, int nImageYLen, Panel* pPanel, int nComponentIDInPanel);
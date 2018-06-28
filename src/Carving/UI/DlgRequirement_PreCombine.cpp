// DlgRequirement_PreCombine.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "DlgRequirement_PreCombine.h"
//#include "afxdialogex.h"
#include "../Drawing/Drawing.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../DataProcess/DataProcess.h"
#include "../Misc/ProgramMisc.h"
#include "../Misc/OtherShapeHelper.h"

#define GAP 10
#define ATTACH_DISTANCE 50

// CDlgRequirement_PreCombine 对话框

IMPLEMENT_DYNAMIC(CDlgRequirement_PreCombine, CDialog)

CDlgRequirement_PreCombine::CDlgRequirement_PreCombine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRequirement_PreCombine::IDD, pParent)
	, m_nSelectedItem(-1)
	, m_bDragging(false)
{

}

CDlgRequirement_PreCombine::~CDlgRequirement_PreCombine()
{
}

void CDlgRequirement_PreCombine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRequirement_PreCombine, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDOK, &CDlgRequirement_PreCombine::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRequirement_PreCombine 消息处理程序
void CDlgRequirement_PreCombine::SetPreCombine(vector<ComponentInputItem>& vPreCombineComponentItem)
{
	//m_vPreCombineItem.push_back(vPreCombineComponentItem);
	//m_vPreCombineComponent = vPreCombineComponentItem;
	float fX = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
	float fY = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
	for(int i = 0; i < vPreCombineComponentItem.size(); i++)
	{
		PreCombineComponent item;
		item.m_Item = vPreCombineComponentItem[i];
		item.m_fX_Left = fX;
		item.m_fY_Bottom = fY;

		vector<PointInfo> vAllOutlineVertex = OtherShapeHelper::GetPointInfoFromPointInfoString(item.m_Item.m_strAbnormalInfo.GetBuffer());
		vector<vector<PointInfo>> vOutlineVertexGroup = SplitAllVertexToGroup(vAllOutlineVertex);

		for(int k = 0; k < vOutlineVertexGroup.size(); k++)
		{
			vector<PointInfo>& vOneSegmentOutlineVertex = vOutlineVertexGroup[/*vOutlineVertexGroup.size()-1*/k];

			vector<OutlineLine> vOneSegmentLines;
			vector<OutlineLine> vOneSegmentLines_MillingTrack;
			for(int j = 0; j < vOneSegmentOutlineVertex.size()-1; j++)
			{
				PointInfo& vertex = vOneSegmentOutlineVertex[j];
				PointInfo& vertexNext = vOneSegmentOutlineVertex[j+1];

				OutlineLine line = GenLineByVertex(vertex, vertexNext);

				vOneSegmentLines.push_back(line);
			}
			SloveZeroLine(vOneSegmentLines);
			//if(vOneSegmentLines.size() == 0)
			//	continue;
			CalcMillingTrackByOutlineLines(vOneSegmentLines, CSingleon::GetSingleton()->m_BaseInfo.m_SawKerfWidth, vOneSegmentLines_MillingTrack);
			SloveZeroLine(vOneSegmentLines_MillingTrack);

			item.m_vMillingOutlineGroup.push_back(vOneSegmentLines_MillingTrack);
		}


		m_vPreCombineComponent.push_back(item);

		fX += vPreCombineComponentItem[i].m_fLength + CSingleon::GetSingleton()->m_BaseInfo.m_SawKerfWidth;

		if(fX >= (CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength - CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth) - CSingleon::GetSingleton()->m_BaseInfo.m_SawKerfWidth)
		{
			fX = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
			fY = fY + CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth / 5.0;
		}
	}
}

//int CDlgRequirement_PreCombine::GetItemCount()
//{
//	return m_vPreCombineItem.size();
//}

CRect CDlgRequirement_PreCombine::GetPanelViewRect()
{
	CRect rcRet;

	CRect rcClient, rcBtnReset;
	GetClientRect(rcClient);
	GetDlgItem(IDOK)->GetWindowRect(rcBtnReset);
	ScreenToClient(rcBtnReset);

	rcRet.left = rcClient.left + 10;
	rcRet.top = rcClient.top + 10;
	rcRet.right = rcClient.right - 10;
	rcRet.bottom = rcBtnReset.top - 10;

	return rcRet;
}

void CDlgRequirement_PreCombine::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient, rcDrawArea;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);

	//Graphics g(dcMem.m_hDC);
	COLORREF colBK = GetSysColor(CTLCOLOR_DLG);

	//g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
	dcMem.FillSolidRect(rcClient, colBK);

	rcDrawArea = GetPanelViewRect();

	dcMem.FillSolidRect(rcDrawArea, RGB(255, 255, 255));

	Panel panel;
	panel.m_OrgLen = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength;
	panel.m_OrgWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth;
	panel.m_RealLength = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength - CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth * 2;
	panel.m_RealWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth - CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth * 2;
	panel.m_OriginX = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
	panel.m_OriginY = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
	DrawingPanelParam param;
	param.m_nGap = GAP;
	param.m_bDrawDeberredPanel = true;
	DrawPanel(&dcMem, &panel, rcDrawArea, param);

	for(int i = 0; i < m_vPreCombineComponent.size(); i++)
	{
		PreCombineComponent& thePreCombineComponent = m_vPreCombineComponent[i];
		Component* pTheComponent = ConvertOneInputInfoToOneComponent(thePreCombineComponent.m_Item);
		vector<vector<OutlineLine>> vMillingOutlineGroup = thePreCombineComponent.m_vMillingOutlineGroup;
		int nRotateTimes = thePreCombineComponent.m_nRotatedAngleWhenCombine / 90;
		for(int j = 0; j < abs(nRotateTimes); j++)
		{
			if(nRotateTimes < 0)
			{
				OutlineGroupRotateClockwise90(vMillingOutlineGroup, pTheComponent->m_RealLength, pTheComponent->m_RealWidth);
				ComponentRotateClockwise90(pTheComponent);
			}
		}
		
		pTheComponent->m_x = thePreCombineComponent.m_fX_Left;
		pTheComponent->m_y = thePreCombineComponent.m_fY_Bottom;

		DrawingPanelParam param;
		param.m_nGap = GAP;
		if(thePreCombineComponent.m_nState == PreCombineComponent::bs_Hovered || thePreCombineComponent.m_nState == PreCombineComponent::bs_Down)
			param.m_color = RGB(50, 50, 150);
		if(i == m_nSelectedItem)
			param.m_color = RGB(0, 0, 255);
		DrawComponentInPanel(&dcMem, pTheComponent, rcDrawArea, param, panel.m_OrgLen, panel.m_OrgWidth);
		DrawOutlineLineInPanel(&dcMem, vMillingOutlineGroup, pTheComponent->m_x, pTheComponent->m_y, rcDrawArea, param, panel.m_OrgLen, panel.m_OrgWidth);

		delete pTheComponent;
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}

BOOL CDlgRequirement_PreCombine::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}

void CDlgRequirement_PreCombine::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_nSelectedItem != -1 && m_nSelectedItem < m_vPreCombineComponent.size() && m_bDragging)
	{
		MovingComponent_OnMouseMove(point);
	}
	else
	{
		for(int i = 0; i < m_vPreCombineComponent.size(); i++)
		{
			PreCombineComponent& thePreCombineComponent = m_vPreCombineComponent[i];
			CRect rcComponentInUI = GetComponentUIRect(thePreCombineComponent);
			if (rcComponentInUI.PtInRect(point) && thePreCombineComponent.GetButtonState() != PreCombineComponent::bs_Down)
				thePreCombineComponent.SetButtonState(PreCombineComponent::bs_Hovered);
			else if(!rcComponentInUI.PtInRect(point))
				thePreCombineComponent.SetButtonState(PreCombineComponent::bs_Normal);
		}
		InvalidateRect(GetPanelViewRect());
	}


	CDialog::OnMouseMove(nFlags, point);
}

void CDlgRequirement_PreCombine::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bool bSelectedOneItem = false;
	for(int i = 0; i < m_vPreCombineComponent.size(); i++)
	{
		PreCombineComponent& thePreCombineComponent = m_vPreCombineComponent[i];
		CRect rcComponentInUI = GetComponentUIRect(thePreCombineComponent);
		if (rcComponentInUI.PtInRect(point))
		{
			thePreCombineComponent.SetButtonState(PreCombineComponent::bs_Down);
			m_nSelectedItem = i;
			m_ptDragStartPos = point;
			m_ptPreCombineComponentDragStartPos.x = thePreCombineComponent.m_fX_Left;
			m_ptPreCombineComponentDragStartPos.y = thePreCombineComponent.m_fY_Bottom;
			m_bDragging = true;

			bSelectedOneItem = true;
			break;
		}
	}

	if(!bSelectedOneItem)
	{
		m_nSelectedItem = -1;
		m_bDragging = false;
	}
	
	InvalidateRect(GetPanelViewRect());

	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgRequirement_PreCombine::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if(m_nSelectedDragRect != -1 && m_nSelectedDragRect < m_vPreCombineComponent.size())
	//{
	//	PreCombineComponent& thePreCombineComponent = m_vPreCombineComponent[m_nSelectedDragRect];
	//	thePreCombineComponent.SetButtonState(PreCombineComponent::bs_Hovered);
	//	m_nSelectedDragRect = -1;
	//	InvalidateRect(GetPanelViewRect());
	//}
	m_bDragging = false;
	CDialog::OnLButtonUp(nFlags, point);
}

CRect CDlgRequirement_PreCombine::GetComponentUIRect(PreCombineComponent& thePreCombineComponent)
{
	//CRect rcRet;

	int nPanelLen = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength;
	int nPanelWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth;

	float fComponentLen = thePreCombineComponent.m_Item.m_fLength;
	float fComponentWidth = thePreCombineComponent.m_Item.m_fWidth;
	if((abs(thePreCombineComponent.m_nRotatedAngleWhenCombine) / 90) % 2 == 1)
	{
		float fTmp = fComponentWidth;
		fComponentWidth = fComponentLen;
		fComponentLen = fTmp;
	}

	CRect rcDrawArea = GetPanelViewRect();

	float fScale = ::GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-GAP, rcDrawArea.Height()-GAP);
	CRect rcPanelArea(GAP/2, GAP/2, rcDrawArea.Width()-GAP/2, rcDrawArea.Height()-GAP/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	CRect rcComponent(thePreCombineComponent.m_fX_Left*fScale, \
		(nPanelWidth - (thePreCombineComponent.m_fY_Bottom + fComponentWidth))*fScale, \
		(thePreCombineComponent.m_fX_Left + fComponentLen)*fScale, \
		(nPanelWidth - thePreCombineComponent.m_fY_Bottom)*fScale);
	rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

	return rcComponent;
}

float CDlgRequirement_PreCombine::GetScale()
{
	int nPanelLen = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength;
	int nPanelWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth;

	CRect rcDrawArea = GetPanelViewRect();

	float fScale = ::GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-GAP, rcDrawArea.Height()-GAP);

	return fScale;
}

void CDlgRequirement_PreCombine::OnBnClickedOk()
{

	CDialog::OnOK();
}

BOOL CDlgRequirement_PreCombine::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_SPACE)
		{
			RotateSelectedComponent();
			return TRUE;
		}
		else if(pMsg->wParam == VK_DELETE)
		{
			RemoveSelectedComponent();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgRequirement_PreCombine::RotateSelectedComponent()
{
	if(m_nSelectedItem != -1 && m_nSelectedItem < m_vPreCombineComponent.size())
	{
		PreCombineComponent& thePreCombineComponent = m_vPreCombineComponent[m_nSelectedItem];
		thePreCombineComponent.m_nRotatedAngleWhenCombine = GetRegularAngle(thePreCombineComponent.m_nRotatedAngleWhenCombine - 90);
		InvalidateRect(GetPanelViewRect());
	}
}

void CDlgRequirement_PreCombine::RemoveSelectedComponent()
{
	if(m_nSelectedItem != -1 && m_nSelectedItem < m_vPreCombineComponent.size())
	{
		vector<PreCombineComponent>::iterator itr = m_vPreCombineComponent.begin();
		advance(itr, m_nSelectedItem);
		m_vPreCombineComponent.erase(itr);
		m_nSelectedItem = -1;
		InvalidateRect(GetPanelViewRect());
	}
}

bool CDlgRequirement_PreCombine::IsAttachMode()
{
	return !IsKeyPressed(VK_CONTROL);
}

void CDlgRequirement_PreCombine::MovingComponent_OnMouseMove(CPoint point)
{
	if(m_nSelectedItem != -1 && m_nSelectedItem < m_vPreCombineComponent.size() && m_bDragging)
	{
		PreCombineComponent& thePreCombineComponent = m_vPreCombineComponent[m_nSelectedItem];
		//CRect rc = GetComponentUIRect(thePreCombineComponent);
		//rc.OffsetRect(point.x - m_ptDragStartPos.x, point.y - m_ptDragStartPos.y);
		//m_vpDragableRect[m_nSelectedDragRect]->SetRect(rc.left, rc.top, rc.right, rc.bottom);

		PointFloat ptfMovingPosInPanel;
		if(!IsAttachMode())
		{
			float fScale = GetScale();
			ptfMovingPosInPanel.x = m_ptPreCombineComponentDragStartPos.x + (point.x - m_ptDragStartPos.x)/fScale;
			ptfMovingPosInPanel.y = m_ptPreCombineComponentDragStartPos.y - ((point.y - m_ptDragStartPos.y)/fScale);
		}
		else
		{
			float fScale = GetScale();
			ptfMovingPosInPanel.x = m_ptPreCombineComponentDragStartPos.x + (point.x - m_ptDragStartPos.x)/fScale;
			ptfMovingPosInPanel.y = m_ptPreCombineComponentDragStartPos.y - ((point.y - m_ptDragStartPos.y)/fScale);

			Panel panel;
			panel.m_OrgLen = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength;
			panel.m_OrgWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth;
			panel.m_RealLength = CSingleon::GetSingleton()->m_BaseInfo.m_PanelLength - CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth * 2;
			panel.m_RealWidth = CSingleon::GetSingleton()->m_BaseInfo.m_PanelWidth - CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth * 2;
			panel.m_OriginX = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
			panel.m_OriginY = CSingleon::GetSingleton()->m_BaseInfo.m_DeburringWidth;
			ptfMovingPosInPanel = GetAttachPos_PreCombineComponent(&panel, m_vPreCombineComponent, m_nSelectedItem, ptfMovingPosInPanel, CSingleon::GetSingleton()->m_BaseInfo.m_SawKerfWidth, ATTACH_DISTANCE);/*GetAttachPanelPos_PreCombineComponent(point, m_pPastingComponent->m_RealLength, m_pPastingComponent->m_RealWidth)*/;
		}
		thePreCombineComponent.m_fX_Left = ptfMovingPosInPanel.x;
		thePreCombineComponent.m_fY_Bottom = ptfMovingPosInPanel.y;

		//m_ptDragStartPos = point;
		InvalidateRect(GetPanelViewRect());
	}

}
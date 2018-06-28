// DlgResult.cpp : 实现文件
//

#include "stdafx.h"
#include "../Carving.h"
#include "DlgResult.h"
#include "afxdialogex.h"
#include "DlgTotalResult.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"
#include "../MainFrm.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/FileReadWrite/DxfReadWrite/DxfReadWrite.h"



#include "ParamDlg.h"
#include "DlgSetMachiningOrder.h"
#include "../Drawing/Drawing.h"
#include "PanelPrintDlg.h"
#include "SelectPrintDlg.h"
#include "TiebiaoEdgeDlg.h"
#include "ParamSettingDlg.h"
#include "SelectPathDlg.h"
#include "../DataProcess/DataProcess.h"
// CDlgResult 对话框

#define PI 3.14159
#define KNIFE_DOWN_CIRCLE_DIA 10
#define DRAW_PANEL_GAP 50
#define TOTAL_RESULT_DLG_WIDTH 440
#define CLIPBOARD_WIDTH 150
#define CLIPBOARD_ITEM_HEIGHT 150
#define ATTACH_DISTANCE 50

IMPLEMENT_DYNAMIC(CDlgResult, CDialogChildBase)

CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CDialogChildBase(CDlgResult::IDD, pParent)
	, m_pDlgTotalResult(NULL)
	//, m_bDrawPanel(false)
	//, m_pImg(NULL)
	, m_pPastingComponent(NULL)
	, m_bNeedToEraseOldPastingComponent(false)
	, m_pRemainderCutting(NULL)
	, m_bNeedToEraseOldRemainderCutting(false)
{

}

CDlgResult::~CDlgResult()
{
	if(m_pPastingComponent)
		delete m_pPastingComponent;

	if(m_pRemainderCutting)
		delete m_pRemainderCutting;
}

void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIPBOARD, m_lbClipBoard);
	DDX_Control(pDX, IDC_BUTTON_EXPORT_DXF, m_btnExportDxf);
	DDX_Control(pDX, IDC_BUTTON_PRINT_CUR_DIAGRAM, m_btnPrintCurDiagram);
	DDX_Control(pDX, IDC_BUTTON_PRINT_CUR_LABEL, m_btnPrintCurLabel);
	DDX_Control(pDX, IDC_BUTTON_EXPORT_CUR_MATERIAL_LIST1, m_btnPrintCurMaterialList1);
	DDX_Control(pDX, IDC_BUTTON_EXPORT_CUR_MATERIAL_LIST2, m_btnPrintCurMaterialList2);
}


BEGIN_MESSAGE_MAP(CDlgResult, CDialogChildBase)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_REFRESH_PANEL_VIEW, &CDlgResult::OnRefreshPanelView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_RESET_MACHINING_ORDER, &CDlgResult::OnMenuResetMachiningOrder)
	ON_COMMAND(ID_MENU_CUT_COMPONENT, &CDlgResult::OnMenuCutComponent)
	ON_COMMAND(ID_MENU_COPY_COMPONENT, &CDlgResult::OnMenuCopyComponent)
	ON_COMMAND(ID_MENU_PASTE_COMPONENT, &CDlgResult::OnMenuPasteComponent)
	ON_COMMAND(ID_MENU_REMOVE_COMPONENT, &CDlgResult::OnMenuRemoveComponent)
	ON_COMMAND(ID_MENU_REMAINDER_CUTTING, &CDlgResult::OnMenuRemainderCutting)
	ON_WM_RBUTTONUP()
	//ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_DXF, &CDlgResult::OnBtnExportDxf)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_CUR_DIAGRAM, &CDlgResult::OnBtnPrintCurDiagram)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_CUR_LABEL, &CDlgResult::OnBtnPrintCurLabel)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_CUR_MATERIAL_LIST1, &CDlgResult::OnBtnExportCurMaterialList1)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_CUR_MATERIAL_LIST2, &CDlgResult::OnBtnExportCurMaterialList2)
	ON_LBN_SELCHANGE(IDC_LIST_CLIPBOARD, &CDlgResult::OnLbnSelchangeClipBoard)
	ON_MESSAGE(WM_SHOW_OR_HIDE_CLIPBOARD, &CDlgResult::OnShowOrHideClipboard)
	ON_COMMAND(ID_MENU_ROTATE_PASTING_COMPONENT, &CDlgResult::OnMenuRotatePastingComponent)
END_MESSAGE_MAP()


// CDlgResult 消息处理程序


BOOL CDlgResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// TODO:  在此添加额外的初始化
	m_pDlgTotalResult = new CDlgTotalResult(this);
	m_pDlgTotalResult->Create(CDlgTotalResult::IDD, this);
	m_pDlgTotalResult->MoveWindow(100, 100, 100, 500);
	m_pDlgTotalResult->ShowWindow(SW_SHOW);

	m_lbClipBoard.ShowWindow(SW_HIDE);

	m_btnExportDxf.LoadImageFromRes(IDB_PNG_BTN_EXPORT_DXF_NORMAL, IDB_PNG_BTN_EXPORT_DXF_HOVER, IDB_PNG_BTN_EXPORT_DXF_HOVER, IDB_PNG_BTN_EXPORT_DXF_DISABLE, 0);
	m_btnExportDxf.SetWordOffset(40, CPngButton::LEFT);
	m_btnPrintCurDiagram.LoadImageFromRes(IDB_PNG_BTN_PRINT_DIAGRAM_NORMAL, IDB_PNG_BTN_PRINT_DIAGRAM_HOVER, IDB_PNG_BTN_PRINT_DIAGRAM_HOVER, IDB_PNG_BTN_PRINT_DIAGRAM_DISABLE, 0);
	m_btnPrintCurDiagram.SetWordOffset(40, CPngButton::LEFT);
	m_btnPrintCurLabel.LoadImageFromRes(IDB_PNG_BTN_PRINT_LABEL_NORMAL, IDB_PNG_BTN_PRINT_LABEL_HOVER, IDB_PNG_BTN_PRINT_LABEL_HOVER, IDB_PNG_BTN_PRINT_LABEL_DISABLE, 0);
	m_btnPrintCurLabel.SetWordOffset(40, CPngButton::LEFT);
	m_btnPrintCurMaterialList1.LoadImageFromRes(IDB_PNG_BTN_EXCEL_NORMAL, IDB_PNG_BTN_EXCEL_HOVER, IDB_PNG_BTN_EXCEL_HOVER, IDB_PNG_BTN_EXCEL_DISABLE, 0);
	m_btnPrintCurMaterialList1.SetWordOffset(40, CPngButton::LEFT);
	m_btnPrintCurMaterialList2.LoadImageFromRes(IDB_PNG_BTN_EXCEL_NORMAL, IDB_PNG_BTN_EXCEL_HOVER, IDB_PNG_BTN_EXCEL_HOVER, IDB_PNG_BTN_EXCEL_DISABLE, 0);
	m_btnPrintCurMaterialList2.SetWordOffset(40, CPngButton::LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgResult::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pDlgTotalResult && m_pDlgTotalResult->GetSafeHwnd())
	{
		CRect rcTotalResult(cx-TOTAL_RESULT_DLG_WIDTH, 0, cx, cy);
		m_pDlgTotalResult->MoveWindow(rcTotalResult);

		CRect rcClipboard(cx- TOTAL_RESULT_DLG_WIDTH - CLIPBOARD_WIDTH, 0, cx - TOTAL_RESULT_DLG_WIDTH, cy);
		m_lbClipBoard.MoveWindow(rcClipboard);

		CRect rcPanelView = GetPanelViewRect();
		CButton* aBtn[5];
		aBtn[0] = &m_btnExportDxf;
		aBtn[1] = &m_btnPrintCurDiagram;
		aBtn[2] = &m_btnPrintCurLabel;
		aBtn[3] = &m_btnPrintCurMaterialList1;
		aBtn[4] = &m_btnPrintCurMaterialList2;

		CRect rcClient;
		GetClientRect(rcClient);
		int nBtnBottom = rcClient.bottom - 10;
		int nPos = rcPanelView.right - 30;
		for(int i = 4; i >= 0; i--)
		{

			CRect rcBtn;
			aBtn[i]->GetWindowRect(rcBtn);
			//ScreenToClient(rcBtn);
			rcBtn.SetRect(nPos - rcBtn.Width(), nBtnBottom - rcBtn.Height(), nPos, nBtnBottom);
			aBtn[i]->MoveWindow(rcBtn);
			nPos -= rcBtn.Width();
		}

		Invalidate();
		//InvalidateRect(GetPanelViewRect());
	}
}


void CDlgResult::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient, rcDrawArea;
	GetClientRect(rcClient);
	//GetDrawArea(rcDrawArea);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);


	Graphics g(dcMem.m_hDC);
	COLORREF colBK = RGB(255, 255, 224);//GetSysColor(CTLCOLOR_DLG);//GetBkColor(dc.m_hDC);

	g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());


	//Gdiplus::Font font(L"微软雅黑", 18, FontStyleRegular, UnitPixel);//宋体
	//SolidBrush brush(Color::Black);

	rcDrawArea = GetPanelViewRect();


	//if(m_bDrawPanel && m_pImg)
	//{
	//	RectF rfDes((REAL)rcDrawArea.left, (REAL)rcDrawArea.top, (REAL)rcDrawArea.Width(), (REAL)rcDrawArea.Height());
	//	g.DrawImage(m_pImg, rfDes, 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight(), UnitPixel);
	//}
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		DrawPanel(&dcMem, pParam->m_pPanel, rcDrawArea, *pParam);
	}



	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}


BOOL CDlgResult::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

CRect CDlgResult::GetPanelViewRect()
{
	CRect rcRet;

	CRect rcClient, rcDlgTotalResult, rcClipboard;
	GetClientRect(rcClient);
	m_pDlgTotalResult->GetWindowRect(rcDlgTotalResult);
	m_lbClipBoard.GetWindowRect(rcClipboard);
	ScreenToClient(rcDlgTotalResult);
	ScreenToClient(rcClipboard);

	rcRet.left = rcClient.left + 10;
	rcRet.top = rcClient.top + 10;
	rcRet.right = (m_lbClipBoard.IsWindowVisible() ? rcClipboard.left : rcDlgTotalResult.left) - 10;
	rcRet.bottom = rcClient.bottom - 50;

	return rcRet;
}

LRESULT CDlgResult::OnRefreshPanelView(WPARAM wparam, LPARAM lparam)
{
	//m_bDrawPanel = true;
	//DrawDiagram((PanelViewingParam*)wparam);
	InvalidateRect(GetPanelViewRect());
	
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	bool bEnable = true;
	if(!pParam || !pParam->m_pPanel)
		bEnable = false;

	m_btnExportDxf.EnableWindow(bEnable);
	m_btnPrintCurDiagram.EnableWindow(bEnable);
	m_btnPrintCurLabel.EnableWindow(bEnable);
	m_btnPrintCurMaterialList1.EnableWindow(bEnable);
	m_btnPrintCurMaterialList2.EnableWindow(bEnable);

	return 0;
}

//void CDlgResult::StopDrawing()
//{
//	m_bDrawPanel = false;
//}

//void CDlgResult::DrawDiagram(PanelViewingParam* param)
//{
//	CRect rcDrawArea;
//	rcDrawArea = GetPanelViewRect();
//
//	if(m_pImg)
//	{
//		::delete m_pImg;
//		m_pImg = NULL;
//	}
//	m_pImg = ::new Bitmap(rcDrawArea.Width(), rcDrawArea.Height());
//	Graphics* g = Graphics::FromImage(m_pImg);
//	g->SetSmoothingMode(SmoothingModeHighQuality);
//
//
//	Gdiplus::Font font(L"宋体", 18, FontStyleRegular/*, UnitPixel*/);
//	StringFormat sf;
//	SolidBrush brush(Color::Red);
//
//
//	Panel& thePanel = *(param->m_pPanel);
//	int nPanelLen = /*2000*//*thePanel.m_RealLength*/thePanel.m_OrgLen;
//	int nPanelWidth = /*1000*//*thePanel.m_RealWidth*/thePanel.m_OrgWidth;
//	
//	//float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-50, rcDrawArea.Height()-50);
//	float fScale = GetDrawingScale(nPanelLen, nPanelWidth);
//	CRect rcPanelArea(50/2, 50/2, rcDrawArea.Width()-50/2, rcDrawArea.Height()-50/2);
//
//	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
//		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
//		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
//		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);
//
//	g->FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());
//
//	vector<Component*> vAllComponent;
//	FindAllComponentInPanel(thePanel, vAllComponent);
//
//	for(int i = 0; i < vAllComponent.size(); i++)
//	{
//		Component& theComponent = *(vAllComponent[i]);
//		CRect rcComponent(theComponent.m_x*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - theComponent.m_y)*fScale);
//		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);
//
//		g->FillRectangle(&SolidBrush(Color(255, 255, 224)), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
//		g->DrawRectangle(&Pen(Color::Black, 1), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
//		CString str;
//		str.Format("%d", theComponent.m_NumberInPanel);
//
//		//g->DrawString(AnsiToUnicode(str).c_str(), -1, &font, PointF((rcComponent.left + rcComponent.right)/2, (rcComponent.top + rcComponent.bottom)/2), &sf, &brush);
//		RectF rfText(rcComponent.left, rcComponent.top, rcComponent.Width(), rcComponent.Height());
//		g->DrawString(AnsiToUnicode(str).c_str(), -1, &font, rfText, &sf, &brush);
//
//		vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
//		for(int j = 1; j < vOutlinePoint.size(); j++)
//		{
//			PointInfo& vertexStart = vOutlinePoint[j - 1];
//			PointInfo& vertexEnd = vOutlinePoint[j];
//
//			DrawOneLine(*g, rcComponent, fScale, vertexStart, vertexEnd);
//		}
//
//		DrawMachiningInfo(*g, rcComponent, fScale, &theComponent);
//
//	}
//
//	g->DrawRectangle(&Pen(Color::Black, 2), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());
//
//	delete g;
//
//	InvalidateRect(rcDrawArea);
//}

//void CDlgResult::DrawDiagram()
//{
//
//}

void CDlgResult::DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, PanelViewingParam& param)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);


	Panel& thePanel = *(param.m_pPanel);
	int nPanelLen = /*2000*//*thePanel.m_RealLength*/thePanel.m_OrgLen;
	int nPanelWidth = /*1000*//*thePanel.m_RealWidth*/thePanel.m_OrgWidth;

	//float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-50, rcDrawArea.Height()-50);
	float fScale = GetDrawingScale(nPanelLen, nPanelWidth);
	CRect rcPanelArea(DRAW_PANEL_GAP/2, DRAW_PANEL_GAP/2, rcDrawArea.Width()-DRAW_PANEL_GAP/2, rcDrawArea.Height()-DRAW_PANEL_GAP/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	g.FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		//g.FillRectangle(&SolidBrush(Color(180, 255, 255, 224)), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());

		Region regionComponent;
		GetComponentRegion(theComponent.m_vOutlinePoint, rcComponent, fScale, regionComponent);
		//GraphicsPath pathComponent;
		//vector<PointInfo> vOutterOutlinePoint;
		//GetOutterOutlineVertex(theComponent.m_vOutlinePoint, vOutterOutlinePoint);
		//GetOutlinePath(vOutterOutlinePoint, rcComponent, fScale, pathComponent);
		//Region regionComponent(&pathComponent);
		//g.FillPath(&SolidBrush(Color(180, 255, 255, 224)), &pathComponent);
		g.FillRegion(&SolidBrush(Color(180, 255, 255, 224)), &regionComponent);

		//g.DrawRectangle(&Pen(Color::Black, 1), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
		//CString str;
		//str.Format("%d", theComponent.m_NumberInPanel);

		////g.DrawString(AnsiToUnicode(str).c_str(), -1, &font, PointF((rcComponent.left + rcComponent.right)/2, (rcComponent.top + rcComponent.bottom)/2), &sf, &brush);
		//RectF rfText(rcComponent.left, rcComponent.top, rcComponent.Width(), rcComponent.Height());
		//g.DrawString(AnsiToUnicode(str).c_str(), -1, &font, rfText, &sf, &brush);

		vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
		for(int j = 1; j < vOutlinePoint.size(); j++)
		{
			PointInfo& vertexStart = vOutlinePoint[j - 1];
			PointInfo& vertexEnd = vOutlinePoint[j];

			DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd);
		}

		DrawMachiningInfo(g, rcComponent, fScale, &theComponent);

		DrawDetail(g, rcComponent, fScale, &theComponent);

	}

	CParamDlg* param_dlg;
	param_dlg = ((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam;
	if (param_dlg->m_CheckRemainderLibrary == TRUE)
	{
		CSingleon* pSingleton = CSingleon::GetSingleton();
		vector<Component*> vRemainderComponent;
		thePanel.GetAllRemainder(vRemainderComponent, pSingleton->m_BaseInfo.m_MinArea, pSingleton->m_BaseInfo.m_MinLength);

		for(int i = 0; i < vRemainderComponent.size(); i++)
		{
			Component& theComponent = *(vRemainderComponent[i]);
			CRect rcComponent(theComponent.m_x*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - theComponent.m_y)*fScale);
			rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

			theComponent.CalRectOutlinePoint();
			Region regionComponent;
			GetComponentRegion(theComponent.m_vOutlinePoint, rcComponent, fScale, regionComponent);

			g.FillRegion(&SolidBrush(Color(255, 200,255,200)), &regionComponent);



			vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
			for(int j = 1; j < vOutlinePoint.size(); j++)
			{
				PointInfo& vertexStart = vOutlinePoint[j - 1];
				PointInfo& vertexEnd = vOutlinePoint[j];

				DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd);
			}

			DrawDetail(g, rcComponent, fScale, &theComponent);
		}
	}

	DrawRemainderCuting(g, rcPanel, fScale, thePanel);

	g.DrawRectangle(&Pen(Color::Black, 2), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());
}

void CDlgResult::RefreshOptimizeResult()
{
	m_pDlgTotalResult->RefreshOptimizeResult();
}

void CDlgResult::DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color)
{
	if(vertexStart.group == OUTLINE_GROUP_TYPE_END)
		return;

	Pen pen(color, 1);
	pen.SetDashStyle(DashStyleDash);

	if(GetLineType(vertexStart.sign) == STRAIGHT_LINE)
	{

		INT p1_x = rcPanel.left + vertexStart.x * fScale;
		INT p1_y = rcPanel.top + (rcPanel.Height() - vertexStart.y * fScale);
		INT p2_x = rcPanel.left + vertexEnd.x * fScale;
		INT p2_y = rcPanel.top + (rcPanel.Height() - vertexEnd.y * fScale);
		g.DrawLine(&pen, p1_x, p1_y, p2_x, p2_y);
	}
	else if(GetLineType(vertexStart.sign) == CIRCULAR_ARC)
	{
		//DrawArc(g, rcPanel, fScale, vertexStart, vertexEnd, pen);
		CString strTmp;
		//vertexStart.m_nDir = OutlineVertex::ANTI_CLOCKWISE;
		int eArcDir = vertexEnd.dir;
		float fXa = vertexStart.x, fYa = vertexStart.y, fXb = vertexEnd.x, fYb = vertexEnd.y, fR = /*vertexStart.r*/vertexEnd.r;
		//strTmp.Format("fXa = %f, fYa = %f, fXb = %f, fYb = %f, fR = %f\n", fXa, fYa, fXb, fYb, fR);
		//OutputDebugString(strTmp);
		float fDx = fXa - fXb;
		//strTmp.Format("fDx = %f\n", fDx);
		//OutputDebugString(strTmp);
		float fDy = fYa - fYb;
		//strTmp.Format("fDy = %f\n", fDy);
		//OutputDebugString(strTmp);

		float fCenterX1;
		float fCenterX2;
		float fCenterY1;
		float fCenterY2;
		if(IsFloatEqual(fDy, 0, 0.1))
		{
			fCenterX1 = (fXa + fXb) / 2;
			fCenterX2 = (fXa + fXb) / 2;

			fCenterY1 = fYa - sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
			fCenterY2 = fYa + sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
		}
		else
		{
			float fD = fDx / fDy;
			//strTmp.Format("fD = %f\n", fD);
			//OutputDebugString(strTmp);
			float fa = 1 + fD*fD;
			float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
			float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
			//strTmp.Format("fa = %f, fb = %f, fc = %f\n", fa, fb, fc);
			//OutputDebugString(strTmp);
			//strTmp.Format("fb*fb - 4*fa*fc = %f\n", fb*fb - 4*fa*fc);
			//OutputDebugString(strTmp);
			fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
			fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
		}
		//strTmp.Format("fCenterX1 = %f, fCenterY1 = %f\n", fCenterX1, fCenterY1);
		//OutputDebugString(strTmp);
		//strTmp.Format("fCenterX2 = %f, fCenterY2 = %f\n", fCenterX2, fCenterY2);
		//OutputDebugString(strTmp);

		//SolidBrush brush(Color::Red);
		//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX1*fScale), (REAL)(rcPanel.bottom -fCenterY1*fScale), (REAL)3, (REAL)3);
		//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX2*fScale), (REAL)(rcPanel.bottom -fCenterY2*fScale), (REAL)3, (REAL)3);

		//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXa*fScale), (REAL)(rcPanel.top +fYa*fScale), (REAL)3, (REAL)3);
		//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXb*fScale), (REAL)(rcPanel.top +fYb*fScale), (REAL)3, (REAL)3);


		PointF vectorStart(fXa-fCenterX1, fYa-fCenterY1);
		PointF vectorEnd(fXb-fCenterX1, fYb-fCenterY1);
		float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
		float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
		float fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
		float fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);

		//strTmp.Format(L"fCos = %f, fSin = %f\n", fCos, fSin);
		//OutputDebugString(strTmp);
		//bool bAngleLessThan180 = true;
		//if(fSin < 0 || (IsFloatEqual(fSin, 0) && fCos < 0))
		//	bAngleLessThan180 = false;


		PointF ptTheCenter;
		if(((-fSin) >= 0 && /*vertexStart.dir*/eArcDir == ANTI_CLOCKWISE) || ((-fSin) < 0 && /*vertexStart.dir*/eArcDir == CLOCKWISE))
		{
			ptTheCenter.X = fCenterX2;
			ptTheCenter.Y = fCenterY2;
		}
		else
		{
			ptTheCenter.X = fCenterX1;
			ptTheCenter.Y = fCenterY1;
		}

		//SolidBrush brush(Color::Red);
		//g.FillRectangle(&brush, (REAL)(rcPanel.left +ptTheCenter.X*fScale), (REAL)(rcPanel.bottom -ptTheCenter.Y*fScale), (REAL)3, (REAL)3);

		RectF rc(rcPanel.left, rcPanel.top, 100, 100);
		rc.X = rcPanel.left + (ptTheCenter.X - /*vertexStart.r*/fR)*fScale;
		rc.Y = rcPanel.top + (rcPanel.Height() - (ptTheCenter.Y + /*vertexStart.r*/fR)*fScale);
		rc.Width = (/*vertexStart.r*/fR * 2)*fScale;
		rc.Height = (/*vertexStart.r*/fR * 2)*fScale;
		float fArc = acos(fCos);
		float fAng = fArc*360/(PI*2);

		PointF vectorDrawingStart;
		if(/*vertexStart.dir*/eArcDir == CLOCKWISE)
		{
			vectorDrawingStart.X = vertexStart.x - ptTheCenter.X;
			vectorDrawingStart.Y = vertexStart.y - ptTheCenter.Y;
		}
		else
		{
			vectorDrawingStart.X = vertexEnd.x - ptTheCenter.X;
			vectorDrawingStart.Y = vertexEnd.y - ptTheCenter.Y;
		}

		float fCosDrawingStart = vectorDrawingStart.X/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
		float fSinDrawingStart = vectorDrawingStart.Y/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
		float fAngDrawingStart = (acos(fCosDrawingStart))*360/(PI*2);
		if(fSinDrawingStart >= 0)
			fAngDrawingStart = 360 - fAngDrawingStart;

		g.DrawArc(&pen, rc, fAngDrawingStart, fAng);

		//RectF rfVertex;
		//rfVertex.X = rcPanel.left + vertexStart.x*fScale-3;
		//rfVertex.Y = rcPanel.bottom - vertexStart.y*fScale-3;
		//rfVertex.Width = 6;
		//rfVertex.Height = 6;
		//strTmp.Format("rfVertex.X = %f, rfVertex.Y = %f\n", rfVertex.X, rfVertex.Y);
		//OutputDebugString(strTmp);
		//g.FillRectangle(&SolidBrush(Color::Blue), rfVertex);

		//rfVertex.X = rcPanel.left + vertexEnd.x*fScale-3;
		//rfVertex.Y = rcPanel.bottom - vertexEnd.y*fScale-3;
		//rfVertex.Width = 6;
		//rfVertex.Height = 6;
		//g.FillRectangle(&SolidBrush(Color::Blue), rfVertex);
	}
}

//void CDlgResult::DrawArc(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Pen& pen)
//{
//	CString strTmp;
//	//vertexStart.m_nDir = OutlineVertex::ANTI_CLOCKWISE;
//	int eArcDir = vertexEnd.dir;
//	float fXa = vertexStart.x, fYa = vertexStart.y, fXb = vertexEnd.x, fYb = vertexEnd.y, fR =/* vertexStart.r*/vertexEnd.r;
//	//strTmp.Format(L"fXa = %f, fYa = %f, fXb = %f, fYb = %f, fR = %f\n", fXa, fYa, fXb, fYb, fR);
//	//OutputDebugString(strTmp);
//	float fDx = fXa - fXb;
//	//strTmp.Format(L"fDx = %f\n", fDx);
//	//OutputDebugString(strTmp);
//	float fDy = fYa - fYb;
//	//strTmp.Format(L"fDy = %f\n", fDy);
//	//OutputDebugString(strTmp);
//	float fD = fDx / fDy;
//	//strTmp.Format(L"fD = %f\n", fD);
//	//OutputDebugString(strTmp);
//	float fa = 1 + fD*fD;
//	float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
//	float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
//	//strTmp.Format(L"fa = %f, fb = %f, fc = %f\n", fa, fb, fc);
//	//OutputDebugString(strTmp);
//	float fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
//	float fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
//	float fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
//	float fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
//
//	strTmp.Format("fCenterX1 = %f, fCenterY1 = %f\n", fCenterX1, fCenterY1);
//	OutputDebugString(strTmp);
//	strTmp.Format("fCenterX2 = %f, fCenterY2 = %f\n", fCenterX2, fCenterY2);
//	OutputDebugString(strTmp);
//	SolidBrush brush(Color::Red);
//	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX1*fScale), (REAL)(rcPanel.bottom -fCenterY1*fScale), (REAL)3, (REAL)3);
//	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX2*fScale), (REAL)(rcPanel.bottom -fCenterY2*fScale), (REAL)3, (REAL)3);
//
//	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXa*fScale), (REAL)(rcPanel.top +fYa*fScale), (REAL)3, (REAL)3);
//	//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXb*fScale), (REAL)(rcPanel.top +fYb*fScale), (REAL)3, (REAL)3);
//
//
//	PointF vectorStart(fXa-fCenterX1, fYa-fCenterY1);
//	PointF vectorEnd(fXb-fCenterX1, fYb-fCenterY1);
//	float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
//	float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
//	float fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
//	float fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);
//
//	//strTmp.Format(L"fCos = %f, fSin = %f\n", fCos, fSin);
//	//OutputDebugString(strTmp);
//	//bool bAngleLessThan180 = true;
//	//if(fSin < 0 || (IsFloatEqual(fSin, 0) && fCos < 0))
//	//	bAngleLessThan180 = false;
//
//
//	PointF ptTheCenter;
//	if(((-fSin) >= 0 && /*vertexStart.dir*/eArcDir == ANTI_CLOCKWISE) || ((-fSin) < 0 && /*vertexStart.dir*/eArcDir == CLOCKWISE))
//	{
//		ptTheCenter.X = fCenterX2;
//		ptTheCenter.Y = fCenterY2;
//	}
//	else
//	{
//		ptTheCenter.X = fCenterX1;
//		ptTheCenter.Y = fCenterY1;
//	}
//
//	RectF rc(rcPanel.left, rcPanel.top, 100, 100);
//	rc.X = rcPanel.left + (ptTheCenter.X - vertexStart.r)*fScale;
//	rc.Y = rcPanel.top + (rcPanel.Height() - (ptTheCenter.Y + vertexStart.r)*fScale);
//	rc.Width = (vertexStart.r * 2)*fScale;
//	rc.Height = (vertexStart.r * 2)*fScale;
//	float fArc = acos(fCos);
//	float fAng = fArc*360/(PI*2);
//
//	PointF vectorDrawingStart;
//	if(/*vertexStart.dir*/eArcDir == CLOCKWISE)
//	{
//		vectorDrawingStart.X = vertexStart.x - ptTheCenter.X;
//		vectorDrawingStart.Y = vertexStart.y - ptTheCenter.Y;
//	}
//	else
//	{
//		vectorDrawingStart.X = vertexEnd.x - ptTheCenter.X;
//		vectorDrawingStart.Y = vertexEnd.y - ptTheCenter.Y;
//	}
//
//	float fCosDrawingStart = vectorDrawingStart.X/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
//	float fSinDrawingStart = vectorDrawingStart.Y/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
//	float fAngDrawingStart = (acos(fCosDrawingStart))*360/(PI*2);
//	if(fSinDrawingStart >= 0)
//		fAngDrawingStart = 360 - fAngDrawingStart;
//
//	g.DrawArc(&pen, rc, fAngDrawingStart, fAng);
//}

void CDlgResult::DrawMachiningInfo(Graphics& g, CRect rcPanel, float fScale, Component* pComponent)
{
	SolidBrush brush(Color::Blue);//填充的颜色
	SolidBrush brushLightBlue(Color::LightBlue);
	SolidBrush brushDisable(Color(100, 100, 100));
	SolidBrush brushDisable_Light(Color(190, 190, 190));

	for(vector<AllVec>::iterator itr = pComponent->m_vUpperFaceHole.begin(); itr != pComponent->m_vUpperFaceHole.end(); itr++)
	{
		float x = itr->_x * fScale;
		float y = (pComponent->m_RealWidth - itr->_y) * fScale;	
		g.FillEllipse(itr->m_bDisabled ? &brushDisable : &brush, rcPanel.left + (x - (itr->_r * fScale) / 2), rcPanel.top + (y - (itr->_r * fScale) / 2), itr->_r * fScale, itr->_r * fScale);
	}

	for(vector<AllSlot>::iterator itr = pComponent->m_vUpperFaceSlot.begin(); itr != pComponent->m_vUpperFaceSlot.end(); itr++)
	{
		CRect rcSlot;
		rcSlot.left = (itr->_x - (itr->_width / 2)) * fScale;
		rcSlot.top = (pComponent->m_RealWidth - (itr->_y + (itr->_height / 2))) * fScale;
		rcSlot.right = (itr->_x + (itr->_width / 2)) * fScale;
		rcSlot.bottom = (pComponent->m_RealWidth - (itr->_y - (itr->_height / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->m_angle*/itr->_dir == 0 ? 90 : 0);
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(itr->m_bDisabled ? &brushDisable : &brush, rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

	for(vector<AllVec>::iterator itr = pComponent->m_vDownerFaceHole.begin(); itr != pComponent->m_vDownerFaceHole.end(); itr++)
	{
		float x = itr->_x * fScale;
		float y = (pComponent->m_RealWidth - itr->_y) * fScale;	
		g.FillEllipse(itr->m_bDisabled ? &brushDisable_Light : &brushLightBlue, rcPanel.left + (x - (itr->_r * fScale) / 2), rcPanel.top + (y - (itr->_r * fScale) / 2), itr->_r * fScale, itr->_r * fScale);
	}

	for(vector<AllSlot>::iterator itr = pComponent->m_vDownerFaceSlot.begin(); itr != pComponent->m_vDownerFaceSlot.end(); itr++)
	{
		CRect rcSlot;
		rcSlot.left = (itr->_x - (itr->_width / 2)) * fScale;
		rcSlot.top = (pComponent->m_RealWidth - (itr->_y + (itr->_height / 2))) * fScale;
		rcSlot.right = (itr->_x + (itr->_width / 2)) * fScale;
		rcSlot.bottom = (pComponent->m_RealWidth - (itr->_y - (itr->_height / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->m_angle*/itr->_dir == 0 ? 90 : 0);
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(itr->m_bDisabled ? &brushDisable_Light : &brushLightBlue, rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	vector<PointInfo> vOutterOutlinePoint;
	GetOutterOutlineVertex(pComponent->m_vOutlinePoint, vOutterOutlinePoint);
	if(vOutterOutlinePoint.size() == 0)
		return;
	int nKnifeDownVertexIDStart = pComponent->m_nKnifeDownPos % (vOutterOutlinePoint.size()-1);
	int nKnifeDownVertexIDEnd = (pComponent->m_nKnifeDownPos + 1) % (vOutterOutlinePoint.size()-1);
	PointInfo& vertexStart = vOutterOutlinePoint[nKnifeDownVertexIDStart];
	PointInfo& vertexEnd = vOutterOutlinePoint[nKnifeDownVertexIDEnd];
	float fXKnifeDown, fYKnifeDown;
	float fXKnifeDown_UI, fYKnifeDown_UI;
	float fEdgeLen = GetPointDistance(vertexStart.x, vertexStart.y, vertexEnd.x, vertexEnd.y);
	fXKnifeDown = vertexEnd.x + pMainFrame->m_pDlgBaseParam->m_distance2 * (vertexStart.x - vertexEnd.x)/ fEdgeLen;
	fYKnifeDown = vertexEnd.y + pMainFrame->m_pDlgBaseParam->m_distance2 * (vertexStart.y - vertexEnd.y)/ fEdgeLen;
	fXKnifeDown_UI = rcPanel.left + (fXKnifeDown * fScale);
	fYKnifeDown_UI = rcPanel.top + (pComponent->m_RealWidth - fYKnifeDown)* fScale;
	g.FillEllipse(&SolidBrush(Color::Red), (INT)(fXKnifeDown_UI - KNIFE_DOWN_CIRCLE_DIA/2), (INT)(fYKnifeDown_UI - KNIFE_DOWN_CIRCLE_DIA/2), KNIFE_DOWN_CIRCLE_DIA ,KNIFE_DOWN_CIRCLE_DIA);
	//g.DrawLine(&Pen(Color::Blue), rcPanel.left + (vertexStart.x * fScale), rcPanel.top + (pComponent->m_RealWidth - vertexStart.y)* fScale, \
	//	rcPanel.left + (vertexEnd.x * fScale), rcPanel.top + (pComponent->m_RealWidth - vertexEnd.y)* fScale);

	//RectF rfLabel(rcPanel.left+(pComponent->m_fXLabelCenter-15)*fScale, \
	//	rcPanel.top+(pComponent->m_RealWidth - pComponent->m_fYLabelCenter-15)*fScale, \
	//	30*fScale, \
	//	30*fScale);
	//g.FillRectangle(&SolidBrush(Color::Red), rfLabel);
}

void CDlgResult::DrawDetail(Graphics& g, CRect rcComponent, float fScale, Component* pComponent)
{
	Gdiplus::Font font18(L"Segoe UI", 20, FontStyleRegular, UnitPixel);
	Gdiplus::Font font11(L"Segoe UI", 11, FontStyleRegular, UnitPixel);
	StringFormat sf;
	sf.SetLineAlignment(StringAlignmentCenter);
	SolidBrush brushRed(Color::Red);
	SolidBrush brushBlack(Color::Black);

	CString strOrder, strSize, strDetail;
	strOrder.Format("%d", pComponent->m_NumberInPanel);
	strSize.Format(_T("%s*%s"), GetFloatString(pComponent->m_RealLength, 1), GetFloatString(pComponent->m_RealWidth, 1));
	strDetail = strSize + _T("\n") + pComponent->m_BarCode;

	RectF rfMeasureDetail , rfMeasureOrder;	
	g.MeasureString(AnsiToUnicode(strDetail).c_str(), -1, &font11, PointF(0, 0), &rfMeasureDetail);
	g.MeasureString(AnsiToUnicode(strOrder).c_str(), -1, &font18, PointF(0, 0), &rfMeasureOrder);
	
	if(/*!pComponent->m_bRotated*/((pComponent->m_nRotatedAngle / 90) % 2) == 0)//板件平行于原板板件
	{
		RectF rfTotalText(rcComponent.left + (rcComponent.Width()-(rfMeasureOrder.Width+rfMeasureDetail.Width))/2, \
			rcComponent.top + (rcComponent.Height()-rfMeasureDetail.Height)/2, \
			rfMeasureOrder.Width+rfMeasureDetail.Width, \
			rfMeasureDetail.Height);

		RectF rfOrder, rfDetail;
		rfOrder = rfTotalText;
		g.DrawString(AnsiToUnicode(strOrder).c_str(), -1, &font18, rfOrder, &sf, &brushRed);
		rfDetail = rfTotalText;
		rfDetail.Offset(rfMeasureOrder.Width, 0);
		rfDetail.Width -= rfMeasureOrder.Width;
		g.DrawString(AnsiToUnicode(strDetail).c_str(), -1, &font11, rfDetail, &sf, &brushBlack);
	}
	else//板件垂直于原板板件
	{
		RectF rfTotalText(rcComponent.left + (rcComponent.Width()-(rfMeasureOrder.Width+rfMeasureDetail.Height))/2, \
			rcComponent.top + (rcComponent.Height()-rfMeasureDetail.Width)/2, \
			rfMeasureOrder.Width+rfMeasureDetail.Height, \
			rfMeasureDetail.Width);


		RectF rfOrder, rfDetail;
		rfOrder = rfTotalText;
		sf.SetLineAlignment(StringAlignmentCenter);
		g.DrawString(AnsiToUnicode(strOrder).c_str(), -1, &font18, rfOrder, &sf, &brushRed);
		rfDetail = rfTotalText;
		rfDetail.Offset(rfMeasureOrder.Width, 0);
		rfDetail.Width -= rfMeasureOrder.Width;

		POINT ptTargetCenter;
		ptTargetCenter.x = (rfDetail.GetLeft() + rfDetail.GetRight()) / 2;
		ptTargetCenter.y = (rfDetail.GetTop() + rfDetail.GetBottom()) / 2;

		g.TranslateTransform(ptTargetCenter.x, ptTargetCenter.y);
		g.RotateTransform(90);
		g.TranslateTransform(-ptTargetCenter.x, -ptTargetCenter.y);

		RectF rfStr(ptTargetCenter.x - rfDetail.Height/2, ptTargetCenter.y - rfDetail.Width/2, rfDetail.Height, rfDetail.Width);
		//g.FillRectangle(&brushBlack, rfStr);
		g.SetSmoothingMode(SmoothingModeHighQuality);
		g.DrawString(AnsiToUnicode(strDetail).c_str(), -1, &font11, rfStr, &sf, &brushBlack);

		g.ResetTransform();
	}
}

void CDlgResult::DrawRemainderCuting(Graphics& g, CRect rcPanel, float fScale, Panel& thePanel)
{
	SolidBrush brush(Color::Blue);
	for(int i = 0; i < thePanel.m_vCutPath.size(); i++)
	{
		CutPath& theCutPath = thePanel.m_vCutPath[i];
		float fXCenter = (theCutPath._x1 + theCutPath._x2) / 2;
		float fYCenter = (theCutPath._y1 + theCutPath._y2) / 2;
		float fLen = GetPointDistance(theCutPath._x1, theCutPath._y1, theCutPath._x2, theCutPath._y2);
		float fDia = GetKerf();

		CRect rcSlot;
		rcSlot.left = (fXCenter - (fLen / 2)) * fScale;
		rcSlot.top = (thePanel.m_OrgWidth - (fYCenter + (fDia / 2))) * fScale;
		rcSlot.right = (fXCenter + (fLen / 2)) * fScale;
		rcSlot.bottom = (thePanel.m_OrgWidth - (fYCenter - (fDia / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->_dir == 0 ? 90 : 0*/-GetLineAngle(theCutPath._x1, theCutPath._y1, theCutPath._x2, theCutPath._y2));
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(&brush, rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

}

void CDlgResult::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	//else
	//{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(!pParam || !pParam->m_pPanel || !pParam->m_pSolution)
			return;
		Panel* pThePanel = pParam->m_pPanel;

		Component* pTheClickedComponent = NULL;
		vector<Component*> vAllComponent;
		FindAllComponentInPanel(*pThePanel, vAllComponent);
		for(int i = 0; i < vAllComponent.size(); i++)
		{
			Component& theComponent = *(vAllComponent[i]);
			CRect rcComponentUIRect = GetComponentUIRect(pThePanel->m_OrgLen, pThePanel->m_OrgWidth, theComponent.m_x, theComponent.m_y, theComponent.m_RealLength, theComponent.m_RealWidth);
			if(rcComponentUIRect.PtInRect(point))
			{
				//CString strTmp;
				//strTmp.Format(_T("%d"), theComponent.m_NumberInPanel);
				//AfxMessageBox(strTmp);
				float fXClickedInComponent, fYClickedInComponent;
				float fScale = GetDrawingScale(pThePanel->m_OrgLen, pThePanel->m_OrgWidth);
				fXClickedInComponent = (point.x - rcComponentUIRect.left) / fScale;
				fYClickedInComponent = (-(point.y - rcComponentUIRect.bottom)) / fScale;
				vector<PointInfo> vOutterOutlinePoint;
				GetOutterOutlineVertex(theComponent.m_vOutlinePoint, vOutterOutlinePoint);
				theComponent.m_nKnifeDownPos = GetKnifeDownPosByClickedPos(vOutterOutlinePoint, fXClickedInComponent, fYClickedInComponent);

				//CString strTmp;
				//for(int j = 0; j < theComponent.m_vOutlinePoint.size(); j++)
				//{
					//PointInfo& pt = theComponent.m_vOutlinePoint[j];
					//strTmp.Format(_T("x=%.1f y=%.1f r=%.1f sign=%d dir=%d cut=%d group=%d side=%d type=%d\n"), pt.x, pt.y, pt.r, pt.sign, pt.dir, pt.cut, pt.group, pt.side, pt.type);
					//OutputDebugString(strTmp);
				//}
				//strTmp.Format(_T("KnifeDown=%d\n"), theComponent.m_nKnifeDownPos);
				//OutputDebugString(strTmp);

				CRect rcPanelViewRect;
				rcPanelViewRect = GetPanelViewRect();
				//InvalidateRect(rcPanelViewRect);
				PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam/*0*/, 0);
				break;
			}
		}
	//}

	CDialog::OnLButtonDblClk(nFlags, point);
}

CRect CDlgResult::GetComponentUIRect(float fPanelXLen, float fPanelYLen, float fComponentX, float fComponentY, float fComponentXLen, float fComponentYLen)
{
	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

	//float fScale = GetScale(fPanelXLen, fPanelYLen, rcDrawArea.Width()-50, rcDrawArea.Height()-50);
	float fScale = GetDrawingScale(fPanelXLen, fPanelYLen);

	CRect rcPanelArea(DRAW_PANEL_GAP/2, DRAW_PANEL_GAP/2, rcDrawArea.Width()-DRAW_PANEL_GAP/2, rcDrawArea.Height()-DRAW_PANEL_GAP/2);

	CRect rcPanel((rcDrawArea.Width()-fPanelXLen*fScale)/2, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2, \
		(rcDrawArea.Width()-fPanelXLen*fScale)/2 + fPanelXLen*fScale, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2 + fPanelYLen*fScale);

	CRect rcComponentUI(\
		fComponentX*fScale, \
		(fPanelYLen - (fComponentY + fComponentYLen))*fScale, \
		(fComponentX + fComponentXLen)*fScale, \
		(fPanelYLen - fComponentY)*fScale);
	rcComponentUI.OffsetRect(rcPanel.left, rcPanel.top);
	//rcComponentUI.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	return rcComponentUI;
}

float CDlgResult::GetDrawingScale(float fPanelXLen, float fPanelYLen)
{
	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

	float fScale = GetScale(fPanelXLen, fPanelYLen, rcDrawArea.Width()-50, rcDrawArea.Height()-50);

	return fScale;
}

vector<Panel*> CDlgResult::GetPanels(bool bChecked)
{
	return m_pDlgTotalResult->GetPanels(bChecked);
}

bool CDlgResult::IsPanelChecked(Panel* pPanel)
{
	return m_pDlgTotalResult->IsPanelChecked(pPanel);
}

void CDlgResult::OnMenuResetMachiningOrder()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Panel* pThePanel = pParam->m_pPanel;
	CDlgSetMachiningOrder dlg;
	dlg.SetPanel(pThePanel);
	if(dlg.DoModal() == IDOK)
	{
		ResetMachiningOrderByMap(pThePanel, dlg.GetResettedOrderMap());
		CalcKnifeDownPosInPanel(pThePanel);
		PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam, 0);
	}

}

void CDlgResult::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pPastingComponent)
	{
		CancelPasting();
	}
	else if(m_pRemainderCutting)
	{
		StopRemainderCutting();
	}
	else
	{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(!pParam || !pParam->m_pPanel)
			return;

		if(GetPanelViewRect().PtInRect(point))
		{
			CMenu menu, *pPopup;  
			menu.LoadMenu(IDR_MENU_RESULT_DLG);  
			pPopup = menu.GetSubMenu(0);  
			CPoint myPoint;  
			//ClientToScreen(&myPoint);  
			GetCursorPos(&myPoint); 
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this); 

			m_ptRClicked = myPoint;
			ScreenToClient(&m_ptRClicked);
		}
	}

	//CDialog::OnRButtonUp(nFlags, point);
}

void CDlgResult::OnMenuCutComponent()
{
	//Component* pComponent = GetSelectedComponent();
	//AddComponentToClipboard(pComponent);
	//RemoveComponentFromPanel(pComponent, pPanel);

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Component* pTheComponent = GetClickedComponent(m_ptRClicked, GetPanelViewRect(), DRAW_PANEL_GAP, pParam->m_pPanel);
	if(pTheComponent == NULL)
		return;

	Component* pCopiedComponent = (Component*) pTheComponent->CopyNode();
	AddComponentToClipboard(pCopiedComponent);

	RemoveComponentFromPanel(pTheComponent, pParam->m_pPanel);
	PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam, 0);
	m_pDlgTotalResult->UpdateList();
}

void CDlgResult::OnMenuCopyComponent()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Component* pTheComponent = GetClickedComponent(m_ptRClicked, GetPanelViewRect(), DRAW_PANEL_GAP, pParam->m_pPanel);
	if(pTheComponent == NULL)
		return;

	Component* pCopiedComponent = (Component*) pTheComponent->CopyNode();

	AddComponentToClipboard(pCopiedComponent);
}

void CDlgResult::OnMenuPasteComponent()
{
	if(m_lbClipBoard.GetCurSel() == -1)
		return;

	//m_pPastingComponent = (Component*)(((Component*)(m_lbClipBoard.GetItemData(m_lbClipBoard.GetCurSel())))->CopyNode());
	m_pPastingComponent = (Component*)(m_lbClipBoard.GetComponent(m_lbClipBoard.GetCurSel())->CopyNode());


	//m_lbClipBoard.DeleteString(m_lbClipBoard.GetCurSel());

	//ShowOrHideClipboard();

	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	PastingComponent_OnMouseMove(ptCursor);
}

void CDlgResult::OnMenuRemoveComponent()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Component* pTheComponent = GetClickedComponent(m_ptRClicked, GetPanelViewRect(), DRAW_PANEL_GAP, pParam->m_pPanel);
	if(pTheComponent == NULL)
		return;

	//AddComponentToClipboard(pTheComponent);
	if(MessageBox(_T("确定删除板件？"), NULL, MB_OKCANCEL) == IDOK)
	{
		RemoveComponentFromPanel(pTheComponent, pParam->m_pPanel);
		PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam, 0);
		m_pDlgTotalResult->UpdateList();
	}
}

void CDlgResult::AddComponentToClipboard(Component* pComponent)
{
	//m_vClipboardItem.push_back(pComponent);
	m_lbClipBoard.AddItem(pComponent);
	m_lbClipBoard.SetCurSel(m_lbClipBoard.GetCount()-1);
	m_lbClipBoard.SetItemHeight(m_lbClipBoard.GetCount()-1, CLIPBOARD_ITEM_HEIGHT);
	ShowOrHideClipboard();
}

//void CDlgResult::ShowClipboard()
//{
//	m_lbClipBoard.ShowWindow(SW_SHOW);
//	Invalidate();
//}

void CDlgResult::ShowOrHideClipboard()
{
	if(m_lbClipBoard.GetCount() > 0 && !m_lbClipBoard.IsWindowVisible())
	{
		m_lbClipBoard.ShowWindow(SW_SHOW);
		Invalidate();
	}
	else if(m_lbClipBoard.GetCount() == 0 && m_lbClipBoard.IsWindowVisible())
	{
		m_lbClipBoard.ShowWindow(SW_HIDE);
		Invalidate();
	}
}

void CDlgResult::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	PastingComponent_OnMouseMove(point);

	if(m_pRemainderCutting)
	{
		if(m_bNeedToEraseOldRemainderCutting)
		{
			DrawRemainderCuttingToPos(*m_pRemainderCutting, pParam->m_pPanel);
		}

		PointFloat ptPosInPanel;
		if(!IsAttachMode())
			ptPosInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, point);
		else
		{
			if(m_nRemainderCuttingSettingState == CUTTING_SETTING_STATE_START)
				ptPosInPanel = GetAttachPanelPos_RemainderCutting_Start(point);
			else
			{
				PointFloat ptStartInPanel;
				ptStartInPanel.x = m_pRemainderCutting->_x1;
				ptStartInPanel.y = m_pRemainderCutting->_y1;
				ptPosInPanel = GetAttachPanelPos_RemainderCutting_End(point, ptStartInPanel);
			}
		}

		if(m_nRemainderCuttingSettingState == CUTTING_SETTING_STATE_START)
		{		
			m_pRemainderCutting->_x1 = m_pRemainderCutting->_x2 = ptPosInPanel.x;
			m_pRemainderCutting->_y1 = m_pRemainderCutting->_y2 = ptPosInPanel.y;
		}
		else
		{
			m_pRemainderCutting->_x2 = ptPosInPanel.x;
			m_pRemainderCutting->_y2 = ptPosInPanel.y;
		}
		
		DrawRemainderCuttingToPos(*m_pRemainderCutting, pParam->m_pPanel);
		m_bNeedToEraseOldRemainderCutting = true;
	}

	CDialog::OnMouseMove(nFlags, point);
}

//void CDlgResult::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//
//	CDialog::OnLButtonUp(nFlags, point);
//}

void CDlgResult::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pRemainderCutting)
	{
		if(m_nRemainderCuttingSettingState == CUTTING_SETTING_STATE_START)
		{
			m_nRemainderCuttingSettingState = CUTTING_SETTING_STATE_END;
		}
		else
		{
			PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
			if(!pParam || !pParam->m_pPanel)
				return;

			pParam->m_pPanel->m_vCutPath.push_back(*m_pRemainderCutting);
			StopRemainderCutting();
		}
	}
	else if(m_pPastingComponent)
	{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(pParam && pParam->m_pPanel)
		{
			//if(IsPosCanPasteComponent(pParam->m_pPanel, m_pPastingComponent, point))
			if(pParam->m_pPanel->m_Material.CompareNoCase(m_pPastingComponent->m_Material) == 0 \
				&& IsFloatEqual(pParam->m_pPanel->m_Thickness, m_pPastingComponent->m_Thickness))
			{
				PointFloat ptComponentLeftTop;
				if(!IsAttachMode())
					ptComponentLeftTop = ConvertUIPosToPanelPos(pParam->m_pPanel, /*point*/m_ptOldPastingComponent);
				else
					ptComponentLeftTop = m_ptfAttachPosInPanel;
				PointFloat ptComponentLeftBottom;
				ptComponentLeftBottom.x = ptComponentLeftTop.x;
				ptComponentLeftBottom.y = ptComponentLeftTop.y - m_pPastingComponent->m_RealWidth;
				if(InsertComponentToPanelRoot(pParam->m_pPanel, m_pPastingComponent, ptComponentLeftBottom))
				{
					m_lbClipBoard.DeleteString(m_lbClipBoard.GetCurSel());
					m_lbClipBoard.SetCurSel(0);

					m_pPastingComponent = NULL;
					m_bNeedToEraseOldPastingComponent = false;

					ShowOrHideClipboard();
					CalcKnifeDownPosInPanel(pParam->m_pPanel);
					InvalidateRect(GetPanelViewRect());
					m_pDlgTotalResult->UpdateList();
				}
			}
			else
			{
				MessageBox(_T("材质厚度不一致，无法放入板件！"));
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgResult::DrawComponentToPos(Component* pComponent, CPoint ptComponent, bool bDrawOKIcon, Panel* pPanel)
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	float fScale = GetDrawingScale(pParam->m_pPanel->m_OrgLen, pParam->m_pPanel->m_OrgWidth);

	CRect rcComponent(ptComponent.x, ptComponent.y, ptComponent.x + pComponent->m_RealLength*fScale, ptComponent.y + pComponent->m_RealWidth*fScale);
	
	CDC* pDC = GetDC();

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, rcComponent.Width(), rcComponent.Height());
	dcMem.SelectObject(&bmpMem);

	dcMem.Rectangle(0, 0, rcComponent.Width(), rcComponent.Height());

	//float fXClickedInPanel, fYClickedInPanel;
	//CRect rcPanelUIRect = GetComponentUIRect(pParam->m_pPanel->m_OrgLen, pParam->m_pPanel->m_OrgWidth, 0, 0, pParam->m_pPanel->m_OrgLen, pParam->m_pPanel->m_OrgWidth);

	//fXClickedInPanel = (ptComponent.x - rcPanelUIRect.left) / fScale;
	//fYClickedInPanel = (-(ptComponent.y - rcPanelUIRect.bottom)) / fScale;
	//CPoint ptInPanel(fXClickedInPanel, fYClickedInPanel);
	//CSingleon* pSingleton = CSingleon::GetSingleton();
	//if(!IsComponentOverlap(pParam->m_pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_SawKerfWidth) \
	//	&& IsComponentInPanelClipped(pParam->m_pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_DeburringWidth))
	DrawingPanelParam param;
	param.m_nGap = 0;
	DrawComponent(&dcMem, pComponent, CRect(0, 0, rcComponent.Width(), rcComponent.Height()), param);

	Image* pImageFlag = NULL;
	if(/*IsPosCanPasteComponent(pParam->m_pPanel, pComponent, ptComponent)*/bDrawOKIcon)
		pImageFlag = LoadPngImgFromRes(IDB_PNG_FLAG_YES);
	else
		pImageFlag = LoadPngImgFromRes(/*IDB_PNG_FLAG_NO*/IDB_PNG_EXCLAMATION);
	Graphics g(dcMem.m_hDC);
	RectF rfImgCorrect(((float)rcComponent.Width() - pImageFlag->GetWidth())/2, ((float)rcComponent.Height()-pImageFlag->GetHeight())/2, pImageFlag->GetWidth(), pImageFlag->GetHeight());
	g.DrawImage(pImageFlag, rfImgCorrect, 0, 0, pImageFlag->GetWidth(), pImageFlag->GetHeight(), UnitPixel);
	delete pImageFlag;

	//int nOldMode = pDC->SetROP2(R2_XORPEN);
	pDC->BitBlt(ptComponent.x, ptComponent.y, rcComponent.Width(), rcComponent.Height(), &dcMem, 0, 0, SRCINVERT);
	//pDC->SetROP2(nOldMode);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();

	this->ReleaseDC(pDC);
}

bool CDlgResult::IsPosCanPasteComponent(Panel* pPanel, Component* pComponent, PointFloat ptComponentUILeftTop)
{
	bool bRet = false;

	PointFloat ptInPanel = /*ConvertUIPosToPanelPos(pPanel, ptComponentUILeftTop)*/ptComponentUILeftTop;
	CSingleon* pSingleton = CSingleon::GetSingleton();
	if(pPanel->m_Material.CompareNoCase(pComponent->m_Material) == 0 \
		&& IsFloatEqual(pPanel->m_Thickness, pComponent->m_Thickness) \
		&&!IsComponentOverlap(pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_SawKerfWidth) \
		&& IsComponentInPanelClipped(pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_DeburringWidth))
	{
		return bRet = true;
	}

	return bRet;
}

PointFloat CDlgResult::ConvertUIPosToPanelPos(Panel* pPanel, CPoint ptInUI)
{
	float fScale = GetDrawingScale(pPanel->m_OrgLen, pPanel->m_OrgWidth);

	float fXClickedInPanel, fYClickedInPanel;
	CRect rcPanelUIRect = GetComponentUIRect(pPanel->m_OrgLen, pPanel->m_OrgWidth, 0, 0, pPanel->m_OrgLen, pPanel->m_OrgWidth);

	fXClickedInPanel = (ptInUI.x - rcPanelUIRect.left) / fScale;
	fYClickedInPanel = (-(ptInUI.y - rcPanelUIRect.bottom)) / fScale;
	PointFloat ptInPanel;
	ptInPanel.x = fXClickedInPanel;
	ptInPanel.y = fYClickedInPanel;

	return ptInPanel;
}


void CDlgResult::OnBtnPrintCurDiagram()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	vector<int> check_num;//大板数据	
	check_num.push_back(m_pDlgTotalResult->GetCurSelPanelItemID());

	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	file_path = file_path + _T("\\picture");

	int _panelno = 0;
	CPanelPrintDlg dlg;


	for (int i = 0;i < check_num.size();i++)
	{
		CString src_file_name,file_name;
		src_file_name = "design.bmp";
		file_name.Format("design-%d.bmp",i);

		CImage image;
		image.Load(file_path + "\\" + src_file_name);

		//CFont font;  
		//font.CreatePointFont(200, _T("Arial"));

		//DrawPanelInfoToDC(image.GetDC(), (Panel*)(pParam->m_pPanel), 2.2, 10, 10, check_num.at(i)+1, PanelDiagramPrintingStyle::PRINT_STYLE_A4_1);
		CRect rcDrawingArea;
		rcDrawingArea.SetRect(0, 0, A4_TEMPLATE_X, A4_TEMPLATE_Y);
		DrawPanelDiagramToA4(image.GetDC(), (Panel*)(pParam->m_pPanel), rcDrawingArea, check_num.at(i)+1);


		image.Save(file_path + "\\" + file_name);
		image.ReleaseDC();
		image.Destroy();

		//CreatePictureA4(current_pk,check_num.at(i));
	}
	dlg.m_panelNum = check_num.size();

	dlg.DoModal();
}

void CDlgResult::OnBtnPrintCurLabel()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	int index = m_pDlgTotalResult->GetCurSelPanelItemID();

	CSelectPrintDlg* pSelectPrintDlg = new CSelectPrintDlg;
	//pSelectPrintDlg->_printinfo = getPrintInfo();

	//pSelectPrintDlg->m_pkcoor = m_picturedlg->_picturecoor;
	pSelectPrintDlg->Create(IDD_DIALOG_SELECT_PRINT,NULL);
	pSelectPrintDlg->ShowWindow(SW_SHOW);
	pSelectPrintDlg->setPanelListCheck(index+1);
	pSelectPrintDlg->setPanelCheck(index + 1);
	pSelectPrintDlg->OnBtnPrintSmallPanel();
	pSelectPrintDlg->DestroyWindow();
	delete pSelectPrintDlg;
}

void CDlgResult::OnBtnExportDxf()
{
	// 导出当前板件的dxf文件
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		CString strFilePath;

		CString filter = "dxf 文件(*.dxf)|*.dxf|所有文件 (*.*)|*.*||";
		//CString strDefFileName = GetFileNameInPath(m_strOpenedFile) + _T(".dxf");
		CFileDialog fileDlg (FALSE, _T("dxf"), _T("001.dxf")/*strDefFileName*/,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
		//CString strDefDir = SelectPathDlg().DefaultSavePath();
		//fileDlg.m_ofn.lpstrInitialDir = strDefDir;

		if ( fileDlg.DoModal() == IDOK)
		{
			strFilePath = fileDlg.GetPathName();

			CString DownerFilePath = strFilePath;
			DownerFilePath.Replace(".dxf", "@W.dxf");

			if (DxfReadWrite::OutputUpperFaceDxf(pParam->m_pPanel, strFilePath) == true 
				&& DxfReadWrite::OutputDownerFaceDxf(pParam->m_pPanel, DownerFilePath) == true)
			{
				AfxMessageBox("文件保存成功！");
			}
			else
			{
				AfxMessageBox("文件保存失败！");
			}
		}
	}
}




void CDlgResult::OnBtnExportCurMaterialList1()
{

	::SendMessage(GetParent()->GetSafeHwnd(), WM_USER_EXPORT_MATERIAL_LIST, 0, 1);
	//SelectPathDlg pathDlg;
	//CString pathName = pathDlg.SelectFileDlgDoModel(
	//	"保存",
	//	"csv",
	//	ReadName,
	//	pathDlg.DefaultSavePath(),
	//	DlgSave
	//	);

	//if( pathName != "" )
	//{

	//	ParamSettingDlg settings;
	//	settings.loadSettings();
	//	TiebiaoEdgeDlg tiebiao;
	//	tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());


	//	Panel_Knife panelKnife = m_picturedlg->_picturecoor[panelNum];	//找到当前板件
	//	vector<Coordinate> _coors = panelKnife._coor;
	//	PanelNode* rootNode = panelKnife._panel;
	//	vector<gcodeCoor> handcoor = panelKnife._kc->getNewcoor(rootNode);

	//	//所有小板数据
	//	std::vector<std::map<std::string,std::string>> items = m_matrialdlg->GetMatrialItems();
	//	std::vector<std::map<std::string,std::string>> resultArr;

	//	//通过小板ID找到所有小板参数
	//	for (int c = 0;c < handcoor.size();c++)
	//	{
	//		gcodeCoor coor = handcoor.at(c);
	//		int coor_pos = coor.pos;
	//		std::string coor_info = coor._info;

	//		for (int i = 0;i<items.size();i++)
	//		{
	//			if (coor_info.compare(items[i]["info"]) == 0 )
	//			{
	//				std::map<std::string,std::string> oneitem = items[i];
	//				oneitem["num"] = "1"		;
	//				oneitem["pointInfo"] = ""	;
	//				oneitem["slotInfo"] = ""	;
	//				oneitem["dslotInfo"] = ""	;
	//				oneitem["vecInfo"] = ""		;
	//				oneitem["dvecInfo"] = ""	;
	//				oneitem["OtherPoints"] = ""	;

	//				oneitem["edgeLen1"] = tiebiao.findCharByEdge(atof(items[i]["edgeLen1"].c_str())).c_str();//"封长1");
	//				oneitem["edgeWidth1"] = tiebiao.findCharByEdge(atof(items[i]["edgeWidth1"].c_str())).c_str();//"封宽1");
	//				oneitem["edgeLen2"] = tiebiao.findCharByEdge(atof(items[i]["edgeLen2"].c_str())).c_str();//"封长2");
	//				oneitem["edgeWidth2"] = tiebiao.findCharByEdge(atof(items[i]["edgeWidth2"].c_str())).c_str();//"封宽2");

	//				resultArr.push_back(oneitem);
	//				break;
	//			}
	//		}
	//	}


	//	//按照EXCEL的定义生成excel
	//	std::vector<std::string> title = m_matrialdlg->ShowTitleListString();
	//	std::vector<std::string> dataCol = m_matrialdlg->NameList();

	//	OnWriteDataCSV(pathName, title, dataCol, resultArr);

	//	MessageBox("保存完成");
	//}


	//PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	//if(!pParam || !pParam->m_pPanel)
	//	return;

	//CString tempReadName = GetProjectName();
	////tempReadName.Replace("雕刻机对接-","");

	//SelectPathDlg pathDlg;
	//CString pathName = pathDlg.SelectFileDlgDoModel(
	//	"保存",
	//	"csv",
	//	/*"移出板件-" + */tempReadName,
	//	pathDlg.DefaultSavePath(),
	//	DlgSave
	//	);

	//if( pathName != "" )
	//{
	//	ParamSettingDlg settings;
	//	settings.loadSettings();
	//	TiebiaoEdgeDlg tiebiao;
	//	tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());

	//	//所有选中的板件(把雕刻机不擅长加工的板件变成料单)
	//	//vector<Panel*> vUncheckedPanel = m_pDlgResult->GetPanels(false);
	//	vector<Panel*> vUncheckedPanel;
	//	vUncheckedPanel.push_back(pParam->m_pPanel);
	//	int nComponentInputItemCount = m_vComponentInputItem.size();
	//	int* aExportCount = new int[nComponentInputItemCount];
	//	for(int i = 0; i < nComponentInputItemCount; i++)
	//		aExportCount[i] = 0;

	//	for(int i = 0; i < vUncheckedPanel.size(); i++)
	//	{
	//		vector<Component*> vAllComponent;
	//		FindAllComponentInPanel((Component&)(*(vUncheckedPanel[i])), vAllComponent);
	//		for(int j = 0; j < vAllComponent.size(); j++)
	//		{
	//			int nIndexOfTheComponent = -1;
	//			for(int k = 0; k < nComponentInputItemCount; k++)
	//			{
	//				if(m_vComponentInputItem[k].m_strBarcode.CompareNoCase(vAllComponent[j]->m_BarCode) == 0)
	//				{
	//					nIndexOfTheComponent = k;
	//					break;
	//				}
	//			}
	//			if(nIndexOfTheComponent != -1)
	//				aExportCount[nIndexOfTheComponent]++;
	//		}

	//	}

	//	//所有小板数据
	//	std::vector<std::map<std::string,std::string>> resultArr;
	//	for(int i = 0; i < nComponentInputItemCount; i++)
	//	{
	//		if(aExportCount[i] > 0)
	//		{
	//			map<std::string,std::string> mapOneExportItem = m_pDlgRequirement->GetMatrialItemsOne(i);
	//			CString strNum;
	//			strNum.Format("%d", aExportCount[i]);
	//			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//			mapOneExportItem["num"] = strNum;
	//			mapOneExportItem["pointInfo"] = ""	;
	//			//mapOneExportItem["slotInfo"]	 = ""	;
	//			//mapOneExportItem["dslotInfo"] = ""	;
	//			//mapOneExportItem["vecInfo"] = ""		;
	//			//mapOneExportItem["dvecInfo"] = ""	;
	//			//mapOneExportItem["OtherPoints"] = ""	;
	//			//mapOneExportItem["edgeLen1"] = tiebiao.findCharByEdge(atof(items[i]["edgeLen1"].c_str())).c_str();//"封长1");
	//			//mapOneExportItem["edgeWidth1"] = tiebiao.findCharByEdge(atof(items[i]["edgeWidth1"].c_str())).c_str();//"封宽1");
	//			//mapOneExportItem["edgeLen2"] = tiebiao.findCharByEdge(atof(items[i]["edgeLen2"].c_str())).c_str();//"封长2");
	//			//mapOneExportItem["edgeWidth2"] = tiebiao.findCharByEdge(atof(items[i]["edgeWidth2"].c_str())).c_str();//"封宽2");
	//			resultArr.push_back(mapOneExportItem);
	//		}

	//	}

	//	delete[] aExportCount;

	//	std::vector<std::string> title;
	//	for(int i = 0; i < sizeof(g_aRequirementListTitle) / sizeof(char*); i++)
	//	{
	//		title.push_back(g_aRequirementListTitle[i]);
	//	}
	//	std::vector<std::string> dataCol;
	//	for(int i = 0; i < sizeof(g_aRequirementListTitle_ENU) / sizeof(char*); i++)
	//	{
	//		dataCol.push_back(g_aRequirementListTitle_ENU[i]);
	//	}
	//	//按照EXCEL的定义生成excel
	//	OnWriteDataCSV(pathName, title, dataCol, resultArr);

	//	MessageBox("保存完成");
	//}
}

void CDlgResult::OnBtnExportCurMaterialList2()
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_USER_EXPORT_MATERIAL_LIST, 1, 1);
}

PanelViewingParam* CDlgResult::GetSelectedItemViewingParam()
{
	return m_pDlgTotalResult->GetSelectedItemViewingParam();
}

void CDlgResult::OnLbnSelchangeClipBoard()
{
	// TODO: 在此添加控件通知处理程序代码
	CancelPasting();
}

void CDlgResult::CancelPasting()
{
	if(m_pPastingComponent)
	{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(pParam && pParam->m_pPanel)
		{
			if(m_bNeedToEraseOldPastingComponent)
			{
				DrawComponentToPos(m_pPastingComponent, m_ptOldPastingComponent, m_bOldPastingComponentOK, pParam->m_pPanel);
			}
		}

		delete m_pPastingComponent;
		m_pPastingComponent = NULL;
		m_bNeedToEraseOldPastingComponent = false;

		//Invalidate();
	}
}

bool CDlgResult::IsAttachMode()
{
	return !IsKeyPressed(VK_CONTROL);
}

PointFloat CDlgResult::GetAttachPanelPos_Component(CPoint ptMouseInUI, float fComponentXLen, float fComponentYLen)
{
	//CPoint ptRet = ptMouseInUI;
	PointFloat ptfRet;

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		CSingleon* pSingleton = CSingleon::GetSingleton();
		float fKerf = pSingleton->m_BaseInfo.m_SawKerfWidth;

		PointFloat ptMouseInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptMouseInUI);
		PointFloat ptAttachPosInPanel = GetAttachPos_Component(pParam->m_pPanel, ptMouseInPanel, fComponentXLen, fComponentYLen, fKerf/* + 1*/, ATTACH_DISTANCE);
		//ptRet = ConvertPanelPosToUIPos(pParam->m_pPanel, ptAttachPosInPanel);
		ptfRet = ptAttachPosInPanel;
	}

	return ptfRet;
}

CPoint CDlgResult::ConvertPanelPosToUIPos(Panel* pPanel, PointFloat ptInPanel)
{
	CPoint ptRet;

	float fPanelXLen = pPanel->m_OrgLen;
	float fPanelYLen = pPanel->m_OrgWidth;

	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

	float fScale = GetDrawingScale(fPanelXLen, fPanelYLen);

	CRect rcPanelArea(DRAW_PANEL_GAP/2, DRAW_PANEL_GAP/2, rcDrawArea.Width()-DRAW_PANEL_GAP/2, rcDrawArea.Height()-DRAW_PANEL_GAP/2);

	CRect rcPanel((rcDrawArea.Width()-fPanelXLen*fScale)/2, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2, \
		(rcDrawArea.Width()-fPanelXLen*fScale)/2 + fPanelXLen*fScale, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2 + fPanelYLen*fScale);

	ptRet.x = ptInPanel.x * fScale + rcPanel.left;
	ptRet.y = (fPanelYLen - ptInPanel.y)*fScale + rcPanel.top;

	return ptRet;
}

void CDlgResult::OnMenuRemainderCutting()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	m_pRemainderCutting = new CutPath;
	m_nRemainderCuttingSettingState = CUTTING_SETTING_STATE_START;
	m_bNeedToEraseOldRemainderCutting = false;

	CPoint ptCursor;  
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	PointFloat fpCursorInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptCursor);
	m_pRemainderCutting->_x1 = m_pRemainderCutting->_x2 = fpCursorInPanel.x;
	m_pRemainderCutting->_y1 = m_pRemainderCutting->_y2 = fpCursorInPanel.y;
}

void CDlgResult::DrawRemainderCuttingToPos(CutPath& theRemainderCutting, Panel* pPanel)
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	PointFloat ptStartInPanel, ptEndInPanel;
	CPoint ptStartInUI, ptEndInUI;
	ptStartInPanel.x = theRemainderCutting._x1;
	ptStartInPanel.y = theRemainderCutting._y1;
	ptEndInPanel.x = theRemainderCutting._x2;
	ptEndInPanel.y = theRemainderCutting._y2;

	ptStartInUI = ConvertPanelPosToUIPos(pPanel, ptStartInPanel);
	ptEndInUI = ConvertPanelPosToUIPos(pPanel, ptEndInPanel);

	CDC* pDC = GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	int nOldMode = pDC->SetROP2(R2_XORPEN);
	//pDC->MoveTo(ptStartInUI);
	//pDC->LineTo(ptStartInUI.x+50 ,ptStartInUI.y+50);

	pDC->MoveTo(ptStartInUI.x - 5, ptStartInUI.y);
	pDC->LineTo(ptStartInUI.x + 5 ,ptStartInUI.y);
	pDC->MoveTo(ptStartInUI.x, ptStartInUI.y - 5);
	pDC->LineTo(ptStartInUI.x, ptStartInUI.y + 5);

	if(ptEndInUI != ptStartInUI)
	{
		pDC->MoveTo(ptStartInUI.x, ptStartInUI.y);
		pDC->LineTo(ptEndInUI.x, ptEndInUI.y);

		pDC->MoveTo(ptEndInUI.x - 5, ptEndInUI.y);
		pDC->LineTo(ptEndInUI.x + 5 ,ptEndInUI.y);
		pDC->MoveTo(ptEndInUI.x, ptEndInUI.y - 5);
		pDC->LineTo(ptEndInUI.x, ptEndInUI.y + 5);
	}
	
	pDC->SetROP2(nOldMode);
	pDC->SelectObject(pOldPen);
	
	pen.DeleteObject();

	this->ReleaseDC(pDC);
}

void CDlgResult::StopRemainderCutting()
{
	delete m_pRemainderCutting;
	m_pRemainderCutting = NULL;
	m_bNeedToEraseOldRemainderCutting = false;
	Invalidate();
}

PointFloat CDlgResult::GetAttachPanelPos_RemainderCutting_Start(CPoint ptMouseInUI)
{
	PointFloat ptRet;

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		float fKerf = GetKerf();

		PointFloat ptMouseInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptMouseInUI);
		PointFloat ptAttachPosInPanel = GetAttachPos_RemainderCutting_Start(pParam->m_pPanel, ptMouseInPanel, fKerf/* + 1*/, ATTACH_DISTANCE);
		ptRet = ptAttachPosInPanel;
	}

	return ptRet;
}

PointFloat CDlgResult::GetAttachPanelPos_RemainderCutting_End(CPoint ptMouseInUI, PointFloat ptStartInPanel)
{
	PointFloat ptRet;

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		float fKerf = GetKerf();

		PointFloat ptMouseInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptMouseInUI);
		PointFloat ptAttachPosInPanel = GetAttachPos_RemainderCutting_End(pParam->m_pPanel, ptMouseInPanel, fKerf/* + 1*/, ATTACH_DISTANCE, ptStartInPanel);
		ptRet = ptAttachPosInPanel;
	}

	return ptRet;
}

void CDlgResult::ResetPastingState()
{
	if(m_pPastingComponent)
		delete m_pPastingComponent;
	m_pPastingComponent = NULL;

	m_lbClipBoard.EmptyClipBoard();
	ShowOrHideClipboard();
}

void CDlgResult::ResetResultDlg()
{
	RefreshOptimizeResult();
	ResetPastingState();
}

LRESULT CDlgResult::OnShowOrHideClipboard(WPARAM wparam, LPARAM lparam)
{
	ShowOrHideClipboard();
	return 0;
}

BOOL CDlgResult::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)  
	{
		RotatePastingComponent();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgResult::RotatePastingComponent()
{
	if(m_pPastingComponent)
	{
		CancelPasting();
		m_lbClipBoard.ItemRotate90(m_lbClipBoard.GetCurSel());
		PostMessage(WM_COMMAND,ID_MENU_PASTE_COMPONENT);
	}

}

void CDlgResult::PastingComponent_OnMouseMove(CPoint point)
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	if(m_pPastingComponent)
	{
		if(m_bNeedToEraseOldPastingComponent)
		{
			DrawComponentToPos(m_pPastingComponent, m_ptOldPastingComponent, m_bOldPastingComponentOK, pParam->m_pPanel);
		}

		CPoint ptCur;
		if(!IsAttachMode())
			ptCur = point;
		else
		{
			//ptCur = GetAttachUIPos_Component(point, m_pPastingComponent->m_RealLength, m_pPastingComponent->m_RealWidth);
			m_ptfAttachPosInPanel = GetAttachPanelPos_Component(point, m_pPastingComponent->m_RealLength, m_pPastingComponent->m_RealWidth);
			ptCur = ConvertPanelPosToUIPos(pParam->m_pPanel, m_ptfAttachPosInPanel);
		}

		bool bPosOK = IsPosCanPasteComponent(pParam->m_pPanel, m_pPastingComponent, IsAttachMode() ? m_ptfAttachPosInPanel : ConvertUIPosToPanelPos(pParam->m_pPanel, ptCur));
		DrawComponentToPos(m_pPastingComponent, ptCur, bPosOK, pParam->m_pPanel);
		m_ptOldPastingComponent = ptCur;
		m_bOldPastingComponentOK = bPosOK;
		m_bNeedToEraseOldPastingComponent = true;
	}
}

void CDlgResult::OnMenuRotatePastingComponent()
{
	RotatePastingComponent();
}
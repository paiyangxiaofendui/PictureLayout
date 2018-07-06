// DlgResult.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgResult.h"
#include "afxdialogex.h"
#include "DlgTotalResult.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"
#include "DlgSetMachiningOrder.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/FileReadWrite/DxfReadWrite/DxfReadWrite.h"
#include "../../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"



#include "../Drawing/Drawing.h"
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
	, m_pPastingComponent(NULL)
	, m_bNeedToEraseOldPastingComponent(false)
	, m_pRemainderCutting(NULL)
	, m_bNeedToEraseOldRemainderCutting(false)
{

	m_width				= 1.0;	
	m_height			= 1.0;	
	m_x_space			= 1.0;
	m_y_space			= 1.0;
	m_left_offset		= 1.0;
	m_right_offset		= 1.0;
	m_top_offset		= 1.0;
	m_bottom_offset		= 1.0;

	m_arranging_origin = 1;
}

CDlgResult::~CDlgResult()
{
	if(m_pPastingComponent)
		delete m_pPastingComponent;

	if(m_pRemainderCutting)
		delete m_pRemainderCutting;

	CSingleon* pSingleton = CSingleon::GetSingleton();
	if (pSingleton != NULL)
	{
		pSingleton->ClearAllData();
	}

}

void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIPBOARD, m_lbClipBoard);

	DDX_Text(pDX, IDC_EDIT_NEW_LEN, m_width);
	DDX_Text(pDX, IDC_EDIT_NEW_WIDTH, m_height);

	DDX_Text(pDX, IDC_EDIT_NEW_X_SPACE, m_x_space);				 
	DDX_Text(pDX, IDC_EDIT_NEW_Y_SPACE, m_y_space);				 
																 
	DDX_Text(pDX, IDC_EDIT_NEW_LEFT_OFFSET, m_left_offset);			 
	DDX_Text(pDX, IDC_EDIT_NEW_RIGHT_OFFSET, m_right_offset);			 
																 
	DDX_Text(pDX, IDC_EDIT_NEW_TOP_OFFSET, m_top_offset);
	DDX_Text(pDX, IDC_EDIT_NEW_BOTTOM_OFFSET, m_bottom_offset);


	DDX_Control(pDX, IDC_COMBO_LAYOUT_ORIGIN, control_arranging_origin);
	DDX_CBIndex(pDX, IDC_COMBO_LAYOUT_ORIGIN, m_arranging_origin);

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
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_LBN_SELCHANGE(IDC_LIST_CLIPBOARD, &CDlgResult::OnLbnSelchangeClipBoard)
	ON_MESSAGE(WM_SHOW_OR_HIDE_CLIPBOARD, &CDlgResult::OnShowOrHideClipboard)
	ON_COMMAND(ID_MENU_ROTATE_PASTING_COMPONENT, &CDlgResult::OnMenuRotatePastingComponent)

	ON_BN_CLICKED(IDC_BUTTON_READ_HGO, &CDlgResult::OnOpenSolution)
	ON_BN_CLICKED(IDC_BUTTON_LAYOUT, &CDlgResult::OnOpenSolution)
	ON_BN_CLICKED(IDC_BUTTON_READ_PIC_INFO, &CDlgResult::OnOpenSourcePicInfo)


	
END_MESSAGE_MAP()


// CDlgResult 消息处理程序


BOOL CDlgResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pDlgTotalResult = new CDlgTotalResult(this);
	m_pDlgTotalResult->Create(CDlgTotalResult::IDD, this);
	m_pDlgTotalResult->MoveWindow(100, 100, 100, 500);
	m_pDlgTotalResult->ShowWindow(SW_SHOW);

	m_lbClipBoard.ShowWindow(SW_HIDE);

	// 初始化
	control_arranging_origin.Clear();
	control_arranging_origin.InsertString(0,_T("左下角"));
	control_arranging_origin.InsertString(1,_T("左上角"));
	control_arranging_origin.InsertString(2,_T("右下角"));
	control_arranging_origin.InsertString(3,_T("右上角"));


	control_arranging_origin.SetCurSel(1);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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

	rcDrawArea = GetPanelViewRect();

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














#if 1



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


		Invalidate();
	}
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


	return 0;
}

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
		

		g.FillRegion(&SolidBrush(Color(180, 255, 255, 224)), &regionComponent);

		


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
		

		CString strTmp;
		

		int eArcDir = vertexEnd.dir;
		float fXa = vertexStart.x, fYa = vertexStart.y, fXb = vertexEnd.x, fYb = vertexEnd.y, fR = /*vertexStart.r*/vertexEnd.r;
		

		float fDx = fXa - fXb;
		

		float fDy = fYa - fYb;
		


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
			

			float fa = 1 + fD*fD;
			float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
			float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
			

			fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
			fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
		}
		


		


		PointF vectorStart(fXa-fCenterX1, fYa-fCenterY1);
		PointF vectorEnd(fXb-fCenterX1, fYb-fCenterY1);
		float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
		float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
		float fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
		float fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);

		


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

		

	}
}

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
	
	if(((pComponent->m_nRotatedAngle / 90) % 2) == 0)//板件平行于原板板件
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

			float fXClickedInComponent, fYClickedInComponent;
			float fScale = GetDrawingScale(pThePanel->m_OrgLen, pThePanel->m_OrgWidth);
			fXClickedInComponent = (point.x - rcComponentUIRect.left) / fScale;
			fYClickedInComponent = (-(point.y - rcComponentUIRect.bottom)) / fScale;
			vector<PointInfo> vOutterOutlinePoint;
			GetOutterOutlineVertex(theComponent.m_vOutlinePoint, vOutterOutlinePoint);
			theComponent.m_nKnifeDownPos = GetKnifeDownPosByClickedPos(vOutterOutlinePoint, fXClickedInComponent, fYClickedInComponent);



			CRect rcPanelViewRect;
			rcPanelViewRect = GetPanelViewRect();
			//InvalidateRect(rcPanelViewRect);
			PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam/*0*/, 0);
			break;
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

CRect CDlgResult::GetComponentUIRect(float fPanelXLen, float fPanelYLen, float fComponentX, float fComponentY, float fComponentXLen, float fComponentYLen)
{
	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

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
			

			GetCursorPos(&myPoint); 
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this); 

			m_ptRClicked = myPoint;
			ScreenToClient(&m_ptRClicked);
		}
	}

	

}

void CDlgResult::OnMenuCutComponent()
{
	


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


	m_pPastingComponent = (Component*)(m_lbClipBoard.GetComponent(m_lbClipBoard.GetCurSel())->CopyNode());




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

	m_lbClipBoard.AddItem(pComponent);
	m_lbClipBoard.SetCurSel(m_lbClipBoard.GetCount()-1);
	m_lbClipBoard.SetItemHeight(m_lbClipBoard.GetCount()-1, CLIPBOARD_ITEM_HEIGHT);
	ShowOrHideClipboard();
}


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
				//	CalcKnifeDownPosInPanel(pParam->m_pPanel);
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


void CDlgResult::ClearAllData()
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();
	ResetResultDlg();
}



void CDlgResult::OnOpenSolution()
{
	// TODO: 在此添加命令处理程序代码


	CSingleon* pSingleton = CSingleon::GetSingleton();

	CString strTmp;

	strTmp = _T("hgo");
	CString filter = "hgo 文件(*.hgo)|*.hgo|所有文件 (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hgo"), _T("*.hgo"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);


//	CString strDefDir = SelectPathDlg().DefaultLoadPath();
//	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	if ( fileDlg.DoModal() == IDOK)
	{

		// 先清除上一次的数据
		//pSingleton->ClearAllData();

		ClearAllData();
	


		m_vComponentInputItem.clear();

		CString strOpenedFile = fileDlg.GetPathName();
		//m_vOpenedFile.clear();
		//m_vOpenedFile.push_back(strOpenedFile);

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName.CompareNoCase(strTmp) == 0)
		{
			if(HgyReadWrite::ReadHgy(strOpenedFile) != true)
			{
				AfxMessageBox("优化文件出错!");
			}
			else
			{
				//GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

				m_vComponentInputItem = pSingleton->m_vBackupComponentInputItem;

				for(int i = 0; i < m_vComponentInputItem.size(); i++)
					m_vComponentInputItem[i].m_uSerialID = CSingleon::GetSingleton()->m_uComponentInputItemSerialID++;

				// 检查板件是否有超出尺寸范围的，删掉
				//CheckComponentList(m_vComponentInputItem);
			}

		}

		// 计算小板
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}


		// 自动计算优化参数
// 		COPSetDlg dlgOptimizeSetting;
// 
// 		dlgOptimizeSetting.AutoSetOpSettings(nCpnNum);


		// 切换视图
//		SwitchView(VIEW_RESULT);

// 		if (m_pDlgResult != NULL)
// 			m_pDlgResult->ResetResultDlg();
// 
// 		if (m_pDlgRequirement != NULL)
// 			m_pDlgRequirement->SetRequirement(m_vComponentInputItem);

		// 重置窗口
		ResetResultDlg();

		// 备份输入板件组
		pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);
	}
}



/**  排样优化入口
	@param[in]		
	@param[out]		
	@return			void
	@warning		
	@note			
	@see            
	*/
void  CDlgResult::OnLayout()
{
	// 获取排样数据
	UpdateData(TRUE);


	CSingleon* pSingleton = CSingleon::GetSingleton();
	BaseInfo& info = pSingleton->m_BaseInfo;

	
	info.m_PanelLength		=		m_width ;			
	info.m_PanelWidth		=		m_height;			
	info.m_x_space			=		m_x_space;			
	info.m_y_space			=		m_y_space;			
	info.m_left_offset		=		m_left_offset;		
	info.m_right_offset		=		m_right_offset;	
	info.m_top_offset		=		m_top_offset;		
	info.m_bottom_offset	=		m_bottom_offset;	

	info.m_LayoutOrg = m_arranging_origin;



	// 拷贝需要优化的板件，不直接处理需要优化的板件，避免原始数据遭到破坏
	vector<ComponentInputItem> vOptimizeComponent = m_vComponentInputItem;

	// 检测板件超出
	CheckAndDeleteOverSizeComponentList(vOptimizeComponent);

}



/** 检查板件大小是否超出并删除超长板件
 *	@param[in]		
 *	@param[out]		
 *	@return			void
 *  @warning		
 *	@note			
 *	@see            
 */
void CDlgResult::CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;
	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "无纹理")
		{
			if(pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 旋转后，再次判断
				if (pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth 
					|| pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth
					|| pCpn.m_fLength <= 0
					|| pCpn.m_fWidth <= 0)
				{
					// 还是超出，删除
					bOverSize = true;
				}
			}
		}
		else if(pCpn.m_strTexture == "横纹")
		{
			if (pCpn.m_fLength > base_info.m_PanelLength - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  base_info.m_PanelWidth - 2*base_info.m_DeburringWidth 
				|| pCpn.m_fWidth > base_info.m_PanelLength - 2*base_info.m_DeburringWidth
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// 直接删除
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// 报错
			strMsg += "删除超出范围板件，板件号：" + pCpn.m_strBarcode + "\n";

			// 删除
			it = vComponentInputItem.erase(it);
		}
		else
		{
			it++;
		}
	}

	// 有信息
	if (strMsg.IsEmpty() != true)
	{
		AfxMessageBox(strMsg);
	}


}


void CDlgResult::OnOptimize()
{
	// TODO: 在此添加命令处理程序代码
	

}








#endif

#define  INCH_TO_MM		(25.4)		// 英寸转毫米 

/**  读取图片文件并排样
	@param[in]		
	@param[out]		
	@return			void
	@warning		
	@note			
	@see            
	*/
void CDlgResult::OnOpenSourcePicInfo()
{

	//CParamDlg* param_dlg;
	//param_dlg = (((CMainFrame*)AfxGetMainWnd())->m_pDlgBaseParam);

	//CSingleon* pSingleton = CSingleon::GetSingleton();

	//CString filter = "xml 文件(*.xml)|*.xml|所有文件 (*.*)|*.*||";
	//CFileDialog fileDlg (true, _T("xml"), _T("*.xml"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
	//CString strDefDir = SelectPathDlg().DefaultLoadPath();
	//fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	//fileDlg.m_ofn.lpstrTitle = _T("导入排样源数据(xml)");




	//if ( fileDlg.DoModal() == IDOK)
	//{

	//	// 先清除上一次的数据
	//	ClearAllData();
	//	m_vComponentInputItem.clear();

	//	m_strOpenedFile = fileDlg.GetPathName();

	//	int Which = m_strOpenedFile.ReverseFind('.');  
	//	CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
	//	strExtName.MakeLower();

	//	if (strExtName == "xml")
	//	{

	//		SourceFilePreProccesParam param;
	//		param.b_upvecImportEnable    = param_dlg->VecInput();//正面孔
	//		param.b_downvecImportEnable		= param_dlg->DVecInput();//反面孔
	//		param.b_upsoltImportEnable		= param_dlg->SoltInput();//正面槽
	//		param.b_downsoltImportEnable = param_dlg->DSoltInput();//反面槽
	//		param.b_othershapeImportEnable	= param_dlg->OthershapeInput();//异形过滤
	//		param.i_comnum	= param_dlg->Editnum();//切割数量
	//		param.i_comchange = param_dlg->Comchange();//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
	//		param.f_changex = param_dlg->MoveX();//孔槽偏移
	//		param.f_changey = param_dlg->MoveY();//孔槽偏移	
	//		param.vecAdder = param_dlg->GetVecAdder();//孔位偏差（找到指定孔直径的孔，其直径加一个值）		
	//		param._cxy = param_dlg->GetVecFilter();//孔位过滤（找到指定孔直径的孔，并移除）
	//		param.bReserveDeepHole = param_dlg->ReverseDeepHole();
	//		param.bReserveDeepSlot = param_dlg->ReverseDeepSlot();


	//		// 文档
	//		TiXmlDocument* doc = new TiXmlDocument;
	//		doc->LoadFile(m_strOpenedFile.GetBuffer());

	//		// 根节点
	//		TiXmlElement *pRootElement = doc->RootElement();
	//		if (pRootElement == NULL)
	//		{
	//			AfxMessageBox("空文件");
	//			//return false;
	//		}

	//		//循环读取图片信息
	//		for (TiXmlElement* pPicSetElem = pRootElement->FirstChildElement("图片集"); pPicSetElem != NULL; pPicSetElem = (TiXmlElement*)(pPicSetElem->NextSibling()))
	//		{
	//			for (TiXmlElement* pCurPic = pPicSetElem->FirstChildElement("图片"); pCurPic != NULL; pCurPic = (TiXmlElement*)(pCurPic->NextSibling()))
	//			{
	//				string pic_path = pCurPic->Attribute("路径");
	//				int	num = stoi(pCurPic->Attribute("数量"));

	//				// 计算图片长宽

	//				const wchar_t* pwc = HGCode::char_Gb2312_To_Unicode(pic_path.c_str());
	//				Image tmp_img(pwc);


	//				UINT w	= tmp_img.GetWidth();
	//				UINT h	= tmp_img.GetHeight();
	//				UINT hr = tmp_img.GetHorizontalResolution();		// dpi 每英寸多少个像素点
	//				UINT vr = tmp_img.GetVerticalResolution();			// dpi

	//				float w_inch = (1.0*w)/hr;
	//				float h_inch = (1.0*h)/vr;

	//				float w_mm = w_inch*INCH_TO_MM;
	//				float h_mm = h_inch*INCH_TO_MM;


	//				// 形成一条数据
	//				ComponentInputItem componentInputItem;

	//				componentInputItem.m_strBarcode = pic_path.c_str();
	//				componentInputItem.m_fLength	= w_mm;
	//				componentInputItem.m_fWidth		= h_mm;
	//				componentInputItem.m_nCount		= num;
	//				componentInputItem.m_strTexture = "横纹";

	//				m_vComponentInputItem.push_back(componentInputItem);

	//			}
	//		}





	//		// 计算小板
	//		int nCpnNum = 0;
	//		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
	//		{
	//			ComponentInputItem& item = *it;
	//			if (item.m_nCount > 0)
	//			{
	//				nCpnNum += item.m_nCount;
	//				it++;
	//			}
	//			else
	//			{
	//				it = m_vComponentInputItem.erase(it);
	//			}
	//		}

	//		CString str;
	//		str.Format("%d", nCpnNum);
	//		str += "块小板";
	//		AfxMessageBox(str);


	//	}


	//}



	//// 备份输入板件组
	//pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);




	//ResetResultDlg();
}

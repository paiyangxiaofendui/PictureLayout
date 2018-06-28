// DlgSetMachiningOrder.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "DlgSetMachiningOrder.h"
#include "afxdialogex.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../Drawing/Drawing.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"

#define GAP 10
// CDlgSetMachiningOrder 对话框

IMPLEMENT_DYNAMIC(CDlgSetMachiningOrder, CDialog)

CDlgSetMachiningOrder::CDlgSetMachiningOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetMachiningOrder::IDD, pParent)
	, m_pPanel(NULL)
{

}

CDlgSetMachiningOrder::~CDlgSetMachiningOrder()
{
}

void CDlgSetMachiningOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetMachiningOrder, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgSetMachiningOrder::OnReset)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CDlgSetMachiningOrder::OnBnClickedOk)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgSetMachiningOrder 消息处理程序

void CDlgSetMachiningOrder::OnReset()
{
	m_mapResettedOrder_tmp.clear();
	InvalidateRect(GetPanelViewRect());
	GetDlgItem(IDOK)->EnableWindow(FALSE);
}

void CDlgSetMachiningOrder::OnPaint()
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

	DrawingPanelParam param;
	param.m_nGap = GAP;
	DrawPanel(&dcMem, m_pPanel, rcDrawArea, param);
	DrawSettingOrder(&dcMem, m_pPanel, rcDrawArea, m_mapResettedOrder_tmp);


	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}


BOOL CDlgSetMachiningOrder::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

CRect CDlgSetMachiningOrder::GetPanelViewRect()
{
	CRect rcRet;

	CRect rcClient, rcBtnReset;
	GetClientRect(rcClient);
	GetDlgItem(IDC_BUTTON_RESET)->GetWindowRect(rcBtnReset);
	ScreenToClient(rcBtnReset);

	rcRet.left = rcClient.left + 10;
	rcRet.top = rcClient.top + 10;
	rcRet.right = rcBtnReset.left - 10;
	rcRet.bottom = rcClient.bottom - 50;

	return rcRet;
}

void CDlgSetMachiningOrder::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nClickedComponentOrgID = GetClickedComponentID(point);
	if(nClickedComponentOrgID != -1)
	{
		bool bExist = false;
		for(map<int, int>::iterator itr = m_mapResettedOrder_tmp.begin(); itr != m_mapResettedOrder_tmp.end(); itr++)
		{
			if(itr->first == nClickedComponentOrgID)
			{
				bExist = true;
				break;
			}
		}
		if(!bExist)
		{
			int nOrder = m_mapResettedOrder_tmp.size() + 1;
			m_mapResettedOrder_tmp.insert(make_pair(nClickedComponentOrgID, nOrder));
			InvalidateRect(GetPanelViewRect());

			if(m_mapResettedOrder_tmp.size() == GetComponentCountInPanel(*m_pPanel))
				GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgSetMachiningOrder::DrawSettingOrder(CDC* pDC, Panel* pPanel, CRect rcDrawArea, map<int, int>& m_mapResettedOrder_tmp)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	Gdiplus::Font fontText(L"Arial", 10, FontStyleRegular/*, UnitPixel*/);
	StringFormat sfCenter;
	sfCenter.SetAlignment(StringAlignmentCenter);
	sfCenter.SetLineAlignment(StringAlignmentCenter);
	SolidBrush brush(Color::Black);


	Panel& thePanel = *(pPanel);
	int nPanelLen = thePanel.m_OrgLen;
	int nPanelWidth =thePanel.m_OrgWidth;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-GAP, rcDrawArea.Height()-GAP);
	CRect rcPanelArea(GAP/2, GAP/2, rcDrawArea.Width()-GAP/2, rcDrawArea.Height()-GAP/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);
	//g.FillRectangle(&SolidBrush(Color(255, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		if(m_mapResettedOrder_tmp.find(theComponent.m_NumberInPanel) != m_mapResettedOrder_tmp.end())
		{
			int nSetID = m_mapResettedOrder_tmp[theComponent.m_NumberInPanel];
			CString strSetID;
			strSetID.Format(_T("%d"), nSetID);

			RectF rfText(rcComponent.left - 50, rcComponent.top - 50, rcComponent.Width() + 100, rcComponent.Height() + 100);
			g.DrawString(AnsiToUnicode(strSetID).c_str(), -1, &fontText, rfText, &sfCenter, &brush);
		}
	}
}

void CDlgSetMachiningOrder::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mapResettedOrder = m_mapResettedOrder_tmp;
	CDialog::OnOK();
}

int CDlgSetMachiningOrder::GetClickedComponentID(CPoint point)
{
	int nRet = -1;


	Panel& thePanel = *(m_pPanel);
	int nPanelLen = thePanel.m_OrgLen;
	int nPanelWidth =thePanel.m_OrgWidth;

	CRect rcDrawArea = GetPanelViewRect();

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-GAP, rcDrawArea.Height()-GAP);
	CRect rcPanelArea(GAP/2, GAP/2, rcDrawArea.Width()-GAP/2, rcDrawArea.Height()-GAP/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		Region regionComponent;
		GetComponentRegion(theComponent.m_vOutlinePoint, rcComponent, fScale, regionComponent);
		Point pt;
		pt.X = point.x;
		pt.Y = point.y;
		if(regionComponent.IsVisible(pt))
		{
			nRet = theComponent.m_NumberInPanel;
			break;
		}
		//if(rcComponent.PtInRect(point))
		//{
		//	nRet = theComponent.m_NumberInPanel;
		//	break;
		//}
	}


	return nRet;
}

BOOL CDlgSetMachiningOrder::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	ArrangeUI();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDlgSetMachiningOrder::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ArrangeUI();
	Invalidate();
}

void CDlgSetMachiningOrder::ArrangeUI()
{
	if(GetDlgItem(IDOK) != NULL)
	{
		CRect rcBtnOrg, rcBtn, rcClient;
		GetClientRect(rcClient);
		GetDlgItem(IDOK)->GetWindowRect(rcBtnOrg);
		ScreenToClient(rcBtn);
		rcBtn.SetRect(rcClient.right - 200, rcClient.bottom - 32, rcClient.right - 200 + rcBtnOrg.Width(), rcClient.bottom - 32 + rcBtnOrg.Height());
		GetDlgItem(IDOK)->MoveWindow(rcBtn);
		rcBtn.SetRect(rcClient.right - 100, rcClient.bottom - 32, rcClient.right - 100 + rcBtnOrg.Width(), rcClient.bottom - 32 + rcBtnOrg.Height());
		GetDlgItem(IDCANCEL)->MoveWindow(rcBtn);

		rcBtn.SetRect(rcClient.right - 100, rcClient.top + 32, rcClient.right - 100 + rcBtnOrg.Width(), rcClient.top + 32 + rcBtnOrg.Height());
		GetDlgItem(IDC_BUTTON_RESET)->MoveWindow(rcBtn);
	}
}
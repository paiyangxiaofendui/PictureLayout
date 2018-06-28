// VecSoltDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "VecSoltDlg.h"
//#include "afxdialogex.h"
//#include "OtherShape.h"
//#include "hgCst/Construction.h"
//#include "hg3d/Utils.h"
//#include "YHInfoTools.h"
//#include "HGTools.h"
// CVecSoltDlg dialog

//#include "OtherShapeHelper.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../Drawing/Drawing.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"

#define SCALE_DELTA_PRE_SCROLL 0.05
#define MIN_SCALE 0.5
#define MAX_SCALE 3.0

IMPLEMENT_DYNAMIC(CVecSoltDlg, CDialogEx)

CVecSoltDlg::CVecSoltDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVecSoltDlg::IDD, pParent)
	, m_pComponent(NULL)
	, m_fViewingScale(1.0)
	, m_hCursorHand(NULL)
	, m_bDragging(false)
{
	m_ptDistanceBetweenViewingAndDiagramCenter.x = m_ptDistanceBetweenViewingAndDiagramCenter.y = 0;
	m_hCursorHand = AfxGetApp()->LoadStandardCursor(IDC_HAND);
}

CVecSoltDlg::~CVecSoltDlg()
{
}

void CVecSoltDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVecSoltDlg, CDialogEx)
	//ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_BUTTON2, &CVecSoltDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON1, &CVecSoltDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON3, &CVecSoltDlg::OnBnClickedButton3)
	//ON_BN_CLICKED(IDC_BUTTON4, &CVecSoltDlg::OnBnClickedButton4)
	//ON_BN_CLICKED(IDC_BUTTON5, &CVecSoltDlg::OnBnClickedButton5)
	//ON_BN_CLICKED(IDC_BUTTON6, &CVecSoltDlg::OnBnClickedButton6)
	//ON_WM_MOUSEWHEEL()
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	//ON_WM_LBUTTONDOWN()

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PANEL, &CPanelBrowserDlg::OnLvnItemchangedListPanel)
	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(IDC_BUTTON_READ, &CPanelBrowserDlg::OnBnClickedButtonRead)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CVecSoltDlg message handlers


BOOL CVecSoltDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//if (m_item["OtherPoints"].compare("") == 0)
	//{
	//	//TODO: 错误检测及记录
	//	MessageBox("读不到此板件的轮廓信息请核实");
	//	return TRUE;
	//}

	//m_width = atof(m_item["width"].c_str());
	//m_height = atof(m_item["height"].c_str());
	//m_solt = m_item["slotInfo"];
	//m_vec = m_item["vecInfo"];
	//m_dsolt = m_item["dslotInfo"];
	//m_dvec = m_item["dvecInfo"];

	////获取异形信息
	//m_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(m_item["OtherPoints"]);

	//AllPoint allpoint;
	//allpoint.panel_info = "1";//panelinfo.GetBuffer();
	//allpoint.point_info = m_pointInfos;

	//m_allpoint.clear();
	//m_allpoint.push_back(allpoint);


	//v_solt = TraniferSolt(m_solt);		//解构正面槽
	//v_vec = TraniferVec(m_vec);			//解构正面孔
	//v_dsolt = TraniferdSolt(m_dsolt);	//解构反面槽
	//v_dvec = TraniferdVec(m_dvec);		//解构反面孔

	////计算合适的缩放比例
	//CRect clientRect;
	//GetClientRect(clientRect);

	//_scale = HGTools::getMaxScale(m_width + 80,m_height * 2 + 80,clientRect.Width(),clientRect.Height());

	//move_x = 10;
	//move_y = 10;

	////ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//std::string CVecSoltDlg::replace_allstr(std::string str,const std::string old_value,const std::string new_value)
//{
//	while(true)   
//	{                                                                                                                                                              
//		std::string::size_type pos(0);     
//		if((pos=str.find(old_value))!=string::npos)
//			str.replace(pos,old_value.length(),new_value);
//		else
//			break;
//	}
//	return str;  
//}

void CVecSoltDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	

//	HDC hdc = this->GetDC()->GetSafeHdc();
//	CDC* pDC=CDC::FromHandle(hdc);
//
//	CBrush   *brush_old=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,255)));   
//	CBrush   *brush_new=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(0,0,255))); 
//	CBrush   *brush_yellow=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,0))); 
//
//
//
//	CRect rect;
//	this->GetClientRect(&rect);
//	pDC->SelectObject(brush_old);  
//	pDC->BeginPath();
//	pDC->MoveTo(0,0);
//	pDC->LineTo(rect.Width(),0);
//	pDC->LineTo(rect.Width(),rect.Height());
//	pDC->LineTo(0,rect.Height());
//	pDC->LineTo(0,0);
//	pDC->EndPath();
//	pDC->FillPath();
//
//	try
//	{	
//		//画正反面异形
//		std::vector<InfoLine> lines1 = OtherShapeHelper::GetInfoLineFromPointInfo(m_pointInfos);
//		OtherShapeHelper::DrawLinesInPDC(pDC, lines1 , CPoint(move_x,move_y) ,_scale );
//
//		std::vector<PointInfo> backInfo = OtherShapeHelper::getPointInfoBack(m_pointInfos,m_item["width"],m_item["height"]);	
//		std::vector<InfoLine> lines2 = OtherShapeHelper::GetInfoLineFromPointInfo(backInfo);
//		OtherShapeHelper::DrawLinesInPDC(pDC,lines2,CPoint(move_x,move_y + 10 + m_height * _scale),_scale);
//	}
//	catch (std::logic_error* e)
//	{
//		CString mm;
//		mm.Format("%s:%s",m_item["info"].c_str(),e->what());
//		MessageBox(mm);
//	}
//	catch (...)
//	{
//		MessageBox("绘制板件轮廓出错");
//	}
//	
//	
//
//#if 1
//
//	//画正面槽
//	for (int i = 0;i < v_solt.size();i++)
//	{
//		float s_x = v_solt.at(i)._x;
//		float s_y = v_solt.at(i)._y;
//		float s_width = v_solt.at(i)._width;
//		float s_height = v_solt.at(i)._height;
//		float s_depth = v_solt.at(i)._depth;
//		int s_direction = v_solt.at(i)._direction;
//		if (s_direction == 0)
//		{
//			s_y = s_y - s_width/2.0;
//		}
//		else
//		{
//			s_x = s_x - s_width/2.0;
//		}
//
//		int pen_width = 1;
//		if (s_height * _scale > 1)
//		{
//			pen_width = (int)(s_height * _scale);
//		}
//		CPen pen(PS_SOLID,1,RGB(0,0,0));
//		CPen pen_1(PS_SOLID,pen_width,RGB(255,0,0));
//		pDC->SelectObject(&pen_1);
//		if (s_direction == 0)
//		{
//			pDC->MoveTo((int)(s_x*_scale)+move_x,(int)(s_y*_scale)+move_y);
//			pDC->LineTo((int)(s_x*_scale)+move_x,(int)((s_y + s_width)*_scale)+move_y);
//		}
//		else if (s_direction == 1)
//		{
//			pDC->MoveTo((int)(s_x*_scale)+move_x,(int)(s_y*_scale)+move_y);
//			pDC->LineTo((int)((s_x + s_width)*_scale)+move_x,(int)(s_y*_scale)+move_y);
//		}
//		pDC->SelectObject(&pen);
//	}
//
//	//画反面槽
//	for (int i = 0;i < v_dsolt.size();i++)
//	{
//		float s_x = v_dsolt.at(i)._x;
//		float s_y = v_dsolt.at(i)._y;
//		float s_width = v_dsolt.at(i)._width;
//		float s_height = v_dsolt.at(i)._height;
//		float s_depth = v_dsolt.at(i)._depth;
//		int s_direction = v_dsolt.at(i)._direction;
//		if (s_direction == 0)
//		{
//			s_y = s_y - s_width/2.0;
//		}
//		else
//		{
//			s_x = s_x - s_width/2.0;
//		}
//
//		int pen_width = 1;
//		if (s_height * _scale > 1)
//		{
//			pen_width = (int)(s_height * _scale);
//		}
//		CPen pen(PS_SOLID,1,RGB(0,0,0));
//		CPen pen_1(PS_SOLID,pen_width,RGB(255,0,0));
//		pDC->SelectObject(&pen_1);
//		if (s_direction == 0)
//		{
//			pDC->MoveTo((int)(s_x*_scale)+move_x,(int)(s_y*_scale)+move_y + m_height * _scale + 10);
//			pDC->LineTo((int)(s_x*_scale)+move_x,(int)((s_y + s_width)*_scale)+move_y + m_height * _scale + 10);
//		}
//		else if (s_direction == 1)
//		{
//			pDC->MoveTo((int)(s_x*_scale)+move_x,(int)(s_y*_scale)+move_y + m_height * _scale + 10);
//			pDC->LineTo((int)((s_x + s_width)*_scale)+move_x,(int)(s_y*_scale)+move_y + m_height * _scale + 10);
//		}
//		pDC->SelectObject(&pen);
//	}
//
//	//画正面孔
//	for (int i = 0;i < v_vec.size();i++)
//	{
//		float v_x = v_vec.at(i)._x;
//		float v_y = v_vec.at(i)._y;
//		float v_r = v_vec.at(i)._r;
//
//		CPen pen(PS_SOLID,1,RGB(0,0,0));
//		CPen pen_1(PS_SOLID,1,RGB(0,0,255));
//		pDC->SelectObject(&pen_1);
//		pDC->SelectObject(brush_new);
//
//		CRect rect;
//		rect.left = (int)((v_x - v_r)* _scale) + move_x;
//		rect.top = (int)((v_y - v_r)* _scale) + move_y;
//		rect.right = (int)((v_x + v_r)* _scale) + move_x;
//		rect.bottom = (int)((v_y + v_r)* _scale) + move_y;
//
//
//		pDC->Ellipse(rect);
//
//		pDC->SelectObject(&pen);
//		pDC->SelectObject(brush_old);
//
//	}
//
//	//画反面孔
//	for (int i = 0;i < v_dvec.size();i++)
//	{
//		float v_x = v_dvec.at(i)._x;
//		float v_y = v_dvec.at(i)._y;
//		float v_r = v_dvec.at(i)._r;
//
//		CPen pen(PS_SOLID,1,RGB(0,0,0));
//		CPen pen_1(PS_SOLID,1,RGB(0,0,255));
//		pDC->SelectObject(&pen_1);
//		pDC->SelectObject(brush_new);
//
//		CRect rect;
//		rect.left = (int)((v_x - v_r)* _scale) + move_x;
//		rect.top = (int)((v_y - v_r)* _scale) + move_y + m_height * _scale + 10;
//		rect.right = (int)((v_x + v_r)* _scale) + move_x;
//		rect.bottom = (int)((v_y + v_r)* _scale) + move_y + m_height * _scale + 10;
//
//
//		pDC->Ellipse(rect);
//
//		pDC->SelectObject(&pen);
//		pDC->SelectObject(brush_old);
//
//	}
//
//#endif

	CRect rcClient, rcDrawArea;
	GetClientRect(rcClient);
	rcDrawArea = GetDrawArea();

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);


	Graphics g(dcMem.m_hDC);
	COLORREF colBK = RGB(255, 255, 255);//GetSysColor(CTLCOLOR_DLG);//GetBkColor(dc.m_hDC);

	g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());

	//g.FillRectangle(&SolidBrush(Color::Black), rcDrawArea.left, rcDrawArea.top, rcDrawArea.Width(), rcDrawArea.Height());


	//Image* pPanelImg = m_panelDrawer.GetImg();
	//if(pPanelImg)
	//{
	//	CRect rcImg(rcDrawArea.left - ((int)pPanelImg->GetWidth() - (int)rcDrawArea.Width()) / 2, rcDrawArea.top - ((int)pPanelImg->GetHeight() - (int)rcDrawArea.Height()) / 2, \
	//		rcDrawArea.left - ((int)pPanelImg->GetWidth() - (int)rcDrawArea.Width()) / 2 + pPanelImg->GetWidth(), rcDrawArea.top - ((int)pPanelImg->GetHeight() - (int)rcDrawArea.Height()) / 2 + pPanelImg->GetHeight());
	//	rcImg.OffsetRect(m_ptDistanceBetweenViewingAndDiagramCenter.x, m_ptDistanceBetweenViewingAndDiagramCenter.y);
	//	CRect rcDrawAreaAndImageInterset;
	//	if(rcDrawAreaAndImageInterset.IntersectRect(rcImg, rcDrawArea))
	//	{
	//		RectF rfDes((REAL)rcDrawAreaAndImageInterset.left, (REAL)rcDrawAreaAndImageInterset.top, (REAL)rcDrawAreaAndImageInterset.Width(), (REAL)rcDrawAreaAndImageInterset.Height());

	//		g.DrawImage(pPanelImg, rfDes, rcDrawAreaAndImageInterset.left - rcImg.left, rcDrawAreaAndImageInterset.top - rcImg.top, rcDrawAreaAndImageInterset.Width(), rcDrawAreaAndImageInterset.Height(), UnitPixel);
	//	}
	//}

	if(m_bmpDiagram.GetSafeHandle())
	{
		BITMAP bmp;
		m_bmpDiagram.GetBitmap(&bmp);
		CRect rcImg(rcDrawArea.left - ((int)bmp.bmWidth - (int)rcDrawArea.Width()) / 2, \
			rcDrawArea.top - ((int)bmp.bmHeight - (int)rcDrawArea.Height()) / 2, \
			rcDrawArea.left - ((int)bmp.bmWidth - (int)rcDrawArea.Width()) / 2 + bmp.bmWidth, \
			rcDrawArea.top - ((int)bmp.bmHeight - (int)rcDrawArea.Height()) / 2 + bmp.bmHeight);
		rcImg.OffsetRect(m_ptDistanceBetweenViewingAndDiagramCenter.x, m_ptDistanceBetweenViewingAndDiagramCenter.y);
		CRect rcDrawAreaAndImageInterset;
		if(rcDrawAreaAndImageInterset.IntersectRect(rcImg, rcDrawArea))
		{
			//RectF rfDes((REAL)rcDrawAreaAndImageInterset.left, (REAL)rcDrawAreaAndImageInterset.top, (REAL)rcDrawAreaAndImageInterset.Width(), (REAL)rcDrawAreaAndImageInterset.Height());
			CRect rcDes;
			/*rcDes.SetRect((REAL)rcDrawAreaAndImageInterset.left, \
				(REAL)rcDrawAreaAndImageInterset.top, \
				rcDrawAreaAndImageInterset.left + rcDrawAreaAndImageInterset.Width(), \
				rcDrawAreaAndImageInterset.top + rcDrawAreaAndImageInterset.Height());*/
			rcDes.CopyRect(rcDrawAreaAndImageInterset);

			CDC dcBmp;
			dcBmp.CreateCompatibleDC(&dc);
			CBitmap* pBmpOld = dcBmp.SelectObject(&m_bmpDiagram);
			//dcBmp.FillSolidRect(0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 0, 0));

			//g.DrawImage(pPanelImg, rfDes, rcDrawAreaAndImageInterset.left - rcImg.left, rcDrawAreaAndImageInterset.top - rcImg.top, rcDrawAreaAndImageInterset.Width(), rcDrawAreaAndImageInterset.Height(), UnitPixel);
			dcMem.BitBlt(rcDes.left, rcDes.top, rcDes.Width(), rcDes.Height(), &dcBmp, rcDrawAreaAndImageInterset.left - rcImg.left, rcDrawAreaAndImageInterset.top - rcImg.top, SRCCOPY);

			dcBmp.SelectObject(pBmpOld);
			dcBmp.DeleteDC();
		}
	}

	//g.DrawLine(&Pen(Color::Gray), PointF(rcClient.left, rcClient.bottom-STATUS_BAR_HEIGHT), PointF(rcClient.right, rcClient.bottom-STATUS_BAR_HEIGHT));
	//g.DrawLine(&Pen(Color::White), PointF(rcClient.left, rcClient.bottom-STATUS_BAR_HEIGHT+1), PointF(rcClient.right, rcClient.bottom-STATUS_BAR_HEIGHT+1));
	//Gdiplus::Font font(L"微软雅黑", 13, FontStyleRegular, UnitPixel);//宋体
	//SolidBrush brush(Color::Black);



	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();

}

//void CVecSoltDlg::getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
//{
//	float len_beginToend = sqrt((beginx - endx)*(beginx - endx) + (beginy - endy)*(beginy - endy));
//	float dis_x = (beginx - endx)/len_beginToend*radius;
//	float dis_y = (beginy - endy)/len_beginToend*radius;
//
//	float mid_x = endx + dis_x;
//	float mid_y = endy + dis_y;
//
//	float mid_x_move = mid_x - endx;
//	float mid_y_move = mid_y - endy;
//
//	float arc = (3.1415926-2*asin((0.5*len_beginToend)/radius))/2;
//
//	center_x = mid_x_move*cos(arc) - mid_y_move*sin(arc);
//	center_y = mid_x_move*sin(arc) + mid_y_move*cos(arc);
//
//	center_x = center_x + endx;
//	center_y = center_y + endy;
//}
//
//std::vector<VEC> CVecSoltDlg::TraniferVec(std::string m_vec)//正面孔信息
//{
//	std::vector<VEC> v_vec;
//	v_vec.clear();
//	std::string str_vec = m_vec;
//
//	int str_num = 0;
//	std::string str_value = "";
//	VEC onevec;
//	for (int i = 0;i < str_vec.size();i++)
//	{
//		char ch = str_vec.at(i);
//		if (ch == ',')
//		{
//			switch(str_num)
//			{
//			case 0:
//				onevec._x = atof(str_value.c_str());
//				break;
//			case 1:
//				onevec._y = atof(str_value.c_str());
//				break;
//			case 2:
//				onevec._r = atof(str_value.c_str())/2;
//				break;
//			case 3:
//				onevec._depth = atof(str_value.c_str());
//				break;
//			default:
//				break;
//			}
//			str_value = "";
//			str_num++;
//		}
//		else if (ch == ';')
//		{
//			onevec._depth = atof(str_value.c_str());
//			v_vec.push_back(onevec);
//			str_num = 0;
//			str_value="";
//		}
//		else
//		{
//			str_value = str_value + ch;
//		}
//	}
//	return v_vec;
//}
//
//std::vector<VEC> CVecSoltDlg::TraniferdVec(std::string m_dvec)//得到反面孔信息
//{
//	std::vector<VEC> v_dvec;
//	v_dvec.clear();
//	std::string str_vec = m_dvec;
//
//	int str_num = 0;
//	std::string str_value = "";
//	VEC onevec;
//	for (int i = 0;i < str_vec.size();i++)
//	{
//		char ch = str_vec.at(i);
//		if (ch == ',')
//		{
//			switch(str_num)
//			{
//			case 0:
//				onevec._x = atof(str_value.c_str());
//				break;
//			case 1:
//				onevec._y = atof(str_value.c_str());
//				break;
//			case 2:
//				onevec._r = atof(str_value.c_str())/2;
//				break;
//			case 3:
//				onevec._depth = atof(str_value.c_str());
//				break;
//			default:
//				break;
//			}
//			str_value = "";
//			str_num++;
//		}
//		else if (ch == ';')
//		{
//			onevec._depth = atof(str_value.c_str());
//			v_dvec.push_back(onevec);
//			str_num = 0;
//			str_value="";
//		}
//		else
//		{
//			str_value = str_value + ch;
//		}
//	}
//	return v_dvec;
//}
//
//std::vector<SOLT> CVecSoltDlg::TraniferdSolt(std::string m_dsolt)//反面槽信息
//{
//	std::vector<SOLT> v_dsolt;
//	v_dsolt.clear();
//	std::string str_slotinfo = m_dsolt;
//
//	int str_num = 0;
//	std::string str_value = "";
//	SOLT oneslot;
//	for (int i = 0;i < str_slotinfo.size();i++)
//	{
//		char ch = str_slotinfo.at(i);
//		if (ch == ',')
//		{
//			switch(str_num)
//			{
//			case 0:
//				oneslot._x = atof(str_value.c_str());
//				break;
//			case 1:
//				oneslot._y = atof(str_value.c_str());
//				break;
//			case 2:
//				oneslot._width = atof(str_value.c_str());
//				break;
//			case 3:
//				oneslot._height = atof(str_value.c_str());
//				break;
//			case 4:
//				oneslot._depth = atof(str_value.c_str());
//				break;
//			case 5:
//				oneslot._direction = atoi(str_value.c_str());
//				break;
//			default:
//				break;
//			}
//			str_value = "";
//			str_num++;
//		}
//		else if (ch == ';')
//		{
//			oneslot._direction = atoi(str_value.c_str());
//			v_dsolt.push_back(oneslot);
//			str_num = 0;
//			str_value="";
//		}
//		else
//		{
//			str_value = str_value + ch;
//		}
//	}
//	return v_dsolt;
//}
//
//std::vector<SOLT> CVecSoltDlg::TraniferSolt(std::string m_solt)//正面槽信息
//{
//	std::vector<SOLT> v_solt;
//	v_solt.clear();
//	std::string str_slotinfo = m_solt;
//
//	int str_num = 0;
//	std::string str_value = "";
//	SOLT oneslot;
//	for (int i = 0;i < str_slotinfo.size();i++)
//	{
//		char ch = str_slotinfo.at(i);
//		if (ch == ',')
//		{
//			switch(str_num)
//			{
//			case 0:
//				oneslot._x = atof(str_value.c_str());
//				break;
//			case 1:
//				oneslot._y = atof(str_value.c_str());
//				break;
//			case 2:
//				oneslot._width = atof(str_value.c_str());
//				break;
//			case 3:
//				oneslot._height = atof(str_value.c_str());
//				break;
//			case 4:
//				oneslot._depth = atof(str_value.c_str());
//				break;
//			case 5:
//				oneslot._direction = atoi(str_value.c_str());
//				break;
//			default:
//				break;
//			}
//			str_value = "";
//			str_num++;
//		}
//		else if (ch == ';')
//		{
//			oneslot._direction = atoi(str_value.c_str());
//			v_solt.push_back(oneslot);
//			str_num = 0;
//			str_value="";
//		}
//		else
//		{
//			str_value = str_value + ch;
//		}
//	}
//	return v_solt;
//}
//
///************************************************************************/
///* 事件                                                                 */
///************************************************************************/
//
////放大
//void CVecSoltDlg::OnBnClickedButton2()
//{
//	
//	_scale = _scale + 0.1;
//	OnPaint();
//}
//
////缩小
//void CVecSoltDlg::OnBnClickedButton1()
//{
//	if (_scale >0.1)
//	{
//		_scale = _scale - 0.1;
//	}
//	OnPaint();
//}
//
////向上
//void CVecSoltDlg::OnBnClickedButton3()
//{
//	
//		move_y = move_y - 10;
//		OnPaint();
//	
//}
//
////向下
//void CVecSoltDlg::OnBnClickedButton4()
//{
//	
//	move_y = move_y + 10;
//	OnPaint();
//}
//
////移动向左
//void CVecSoltDlg::OnBnClickedButton5()
//{
//	
//	move_x = move_x - 10;
//	OnPaint();
//}
//
////移动向右
//void CVecSoltDlg::OnBnClickedButton6()
//{
//	
//	move_x = move_x + 10;
//	OnPaint();
//}

////滚动缩放
//BOOL CVecSoltDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	//float new_Scale = _scale + (float)zDelta/1200.0;
//	//if (new_Scale > 0.1)
//	//{
//	//	_scale = new_Scale;
//	//}
//	//OnPaint();
//	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
//}

////鼠标拖动
//void CVecSoltDlg::OnMouseMove(UINT nFlags, CPoint point)
//{
//	
//	//if (m_click == true)
//	//{
//	//	move_x = move_x + (point.x - m_coldx);
//	//	move_y = move_y + (point.y - m_coldy);
//	//	m_coldx = point.x;
//	//	m_coldy = point.y;
//	//	OnPaint();
//	//}
//	CDialogEx::OnMouseMove(nFlags, point);
//}

////弹起鼠标
//void CVecSoltDlg::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	
//	//m_click = false;
//	CDialogEx::OnLButtonUp(nFlags, point);
//}

////按下鼠标
//void CVecSoltDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	
//	//m_click = true;
//	//m_coldx = point.x;
//	//m_coldy = point.y;
//	CDialogEx::OnLButtonDown(nFlags, point);
//}


void CVecSoltDlg::DrawDiagram()
{
	CRect rcDrawArea= GetDrawArea();
	//PanelDrawingParam param(/*m_bShowVHoleLine*/true, /*m_bShowHHoleLine*/true, (m_eMergeMode == MERGE_MODE_MERGE) ? true : false, m_fViewingScale);
	//m_panelDrawer.DrawDiagram(m_vPanelModel, nPanelModelItem, ePanelFaceID, rcDrawArea.Width() * m_fViewingScale, rcDrawArea.Height() * m_fViewingScale, param);

	//if(m_pImg)
	//{
	//	::delete m_pImg;
	//	m_pImg = NULL;
	//}
	//m_pImg = ::new Bitmap(lDiagramWidth, lDiagramHeigh);
	//Graphics* g = Graphics::FromImage(m_pImg);


	//g->SetSmoothingMode(SmoothingModeAntiAlias);

	//g->Clear(Color::Wheat);
	//float fScale = GetScale(fXLen, fYLen, lDiagramWidth-GAP, lDiagramHeigh-GAP);
	//CRect rcPanelArea(GAP/2, GAP/2, lDiagramWidth-GAP/2, lDiagramHeigh-GAP/2);

	//CRect rcPanel( \
	//	(lDiagramWidth-fXLen*fScale)/2, \
	//	(lDiagramHeigh-fYLen*fScale)/2, \
	//	(lDiagramWidth-fXLen*fScale)/2 + fXLen*fScale, \
	//	(lDiagramHeigh-fYLen*fScale)/2 + fYLen*fScale);

	//g->DrawRectangle(&Pen(Color::Red, 2), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());


	if(m_bmpDiagram.GetSafeHandle())
		m_bmpDiagram.DeleteObject();

	CDC* pDC = GetDC();
	m_bmpDiagram.CreateCompatibleBitmap(pDC, rcDrawArea.Width() * m_fViewingScale, rcDrawArea.Height() * m_fViewingScale);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = memDC.SelectObject(&m_bmpDiagram);
	ReleaseDC(pDC);

	memDC.FillSolidRect(0, 0, rcDrawArea.Width() * m_fViewingScale, rcDrawArea.Height() * m_fViewingScale, RGB(255, 255, 255));

	CRect rcUpperFaceArea(0, 0, rcDrawArea.Width() * m_fViewingScale, rcDrawArea.Height() * m_fViewingScale/2);
	CRect rcDownerFaceArea(0, rcDrawArea.Height() * m_fViewingScale/2, rcDrawArea.Width() * m_fViewingScale, rcDrawArea.Height() * m_fViewingScale);

	DrawingPanelParam param;
	param.m_nGap = 30;
	param.m_bDrawMachiningInfo = true;
	DrawComponent(&memDC, m_pComponent, rcUpperFaceArea, param);

	Component* pComponentDownerFace = (Component*)m_pComponent->CopyNode();
	FlipComponentH(pComponentDownerFace);
	DrawComponent(&memDC, pComponentDownerFace, rcDownerFaceArea, param);
	delete pComponentDownerFace;

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();


	
	InvalidateRect(rcDrawArea);
}

CRect CVecSoltDlg::GetDrawArea()
{
	CRect rcRet;
	GetClientRect(rcRet);
	return rcRet;
}

bool CVecSoltDlg::DeltaScale(float fDeltaScale)
{
	if((IsFloatEqual(m_fViewingScale, MIN_SCALE) || m_fViewingScale < MIN_SCALE) && fDeltaScale < 0.0)
		return false;
	if((IsFloatEqual(m_fViewingScale, MAX_SCALE) || m_fViewingScale > MAX_SCALE) && fDeltaScale > 0.0)
		return false;

	float fTargetScale = 1.0;
	if(m_fViewingScale + fDeltaScale > MAX_SCALE)
		fTargetScale = MAX_SCALE;
	else if(m_fViewingScale + fDeltaScale < MIN_SCALE)
		fTargetScale = MIN_SCALE;
	else
		fTargetScale = m_fViewingScale + fDeltaScale;

	SetScale(fTargetScale);
}

void CVecSoltDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	ResetScaleAndPos();

	Invalidate();
}

BOOL CVecSoltDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

BOOL CVecSoltDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	CRect rcDrawing = GetDrawArea();
	if(rcDrawing.PtInRect(ptCursor))
	{
		SetCursor(m_hCursorHand);
		return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CVecSoltDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcDrawArea = GetDrawArea();
	if(rcDrawArea.PtInRect(point))
	{
		m_bDragging = true;
		m_ptDragStartPos.x = point.x;
		m_ptDragStartPos.y = point.y;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CVecSoltDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDragging = false;
	ReleaseCapture();
	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CVecSoltDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	float fDeltaScale = (((float)zDelta) / 120.0) * SCALE_DELTA_PRE_SCROLL;
	DeltaScale(fDeltaScale);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CVecSoltDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		POINT pt;
		GetCursorPos(&pt);
		if(WindowFromPoint(pt) && WindowFromPoint(pt)->GetSafeHwnd())
			pMsg->hwnd = WindowFromPoint(pt)->GetSafeHwnd();
	}
	else if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
	{
		//if(pMsg->hwnd == m_lcPanel.GetSafeHwnd())
			pMsg->hwnd = GetSafeHwnd();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//bool CVecSoltDlg::DeltaScale(float fDeltaScale)
//{
//	if((IsFloatEqual(m_fViewingScale, MIN_SCALE) || m_fViewingScale < MIN_SCALE) && fDeltaScale < 0.0)
//		return false;
//	if((IsFloatEqual(m_fViewingScale, MAX_SCALE) || m_fViewingScale > MAX_SCALE) && fDeltaScale > 0.0)
//		return false;
//
//	float fTargetScale = 1.0;
//	if(m_fViewingScale + fDeltaScale > MAX_SCALE)
//		fTargetScale = MAX_SCALE;
//	else if(m_fViewingScale + fDeltaScale < MIN_SCALE)
//		fTargetScale = MIN_SCALE;
//	else
//		fTargetScale = m_fViewingScale + fDeltaScale;
//
//	SetScale(fTargetScale);
//}

void CVecSoltDlg::SetScale(float fTargetScale)
{
	m_fViewingScale = fTargetScale;

	DrawDiagram();
}

void CVecSoltDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bDragging)
	{

		m_ptDistanceBetweenViewingAndDiagramCenter.x += point.x - m_ptDragStartPos.x;
		m_ptDistanceBetweenViewingAndDiagramCenter.y += point.y - m_ptDragStartPos.y;

		m_ptDragStartPos.x = point.x;
		m_ptDragStartPos.y = point.y;

		CRect rcDrawArea = GetDrawArea();
		InvalidateRect(rcDrawArea);

		//TRACKMOUSEEVENT tme;  
		//tme.cbSize = sizeof(tme);  
		//tme.hwndTrack = m_hWnd;  
		//tme.dwFlags = TME_LEAVE/*|TME_HOVER*/;  
		////tme.dwHoverTime = 1;  
		//_TrackMouseEvent(&tme);  
		SetCapture();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CVecSoltDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDragging = false;

	CDialogEx::OnMouseLeave();
}

void CVecSoltDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nChar == VK_SPACE)
	{
		ResetScaleAndPos();
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CVecSoltDlg::ResetScaleAndPos()
{
	m_ptDistanceBetweenViewingAndDiagramCenter.x = m_ptDistanceBetweenViewingAndDiagramCenter.y = 0;
	m_fViewingScale = 1.0;

	DrawDiagram();
}
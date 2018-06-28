#include "stdafx.h"
#include "ABTDialog.h"


#include "FileRW/DxfFile.h"
#include "DataManager/CSingleton.h"
#include "DataManager/Layer/Layer.h"
#include "DataManager/Figure/Complex.h"
#include "DataManager/Figure/Arc.h"
#include "DataManager/Point/ShapePoint.h"
#include "DataManager/CommonData.h"
#include "GeneralInterface/GeneralInterface.h"
#include "ParallelDialog.h"

#include <io.h>

#include "shlwapi.h"  
#pragma comment(lib,"shlwapi.lib") 

int	FIGURE_SELECT_DISTANCE	=	3;		// 鼠标选中图形的距离	像素

int	SHOW_RECT_CAD_WIDTH		=	2000;	// 真正的CAD尺寸		2000mm宽
int	SHOW_RECT_CAD_HEIGHT	=	2000;	// 真正的CAD尺寸		2000mm高

int	SHOW_RECT_WINDOW_WIDTH	=	1200	;	// 窗口尺寸				500像素宽
int	SHOW_RECT_WINDOW_HEIGHT	=	1200	;	// 窗口尺寸				500像素高

int SHOW_RECT_WINDOW_OFFSET_X = -550;	// 窗口偏移X方向
int SHOW_RECT_WINDOW_OFFSET_Y = -550;	// 窗口偏移Y方向


ABTDialog::ABTDialog(CWnd* pParentWnd)
	: CDialog(ABTDialog::IDD,  pParentWnd)
{

	m_ShowShapePointFlag = TRUE;
	m_ShowOutlineFlag = TRUE;
	m_ShowFigStartDirFlag = TRUE;
}

ABTDialog::~ABTDialog()
{

}

BEGIN_MESSAGE_MAP(ABTDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_OPEN_DXF, &ABTDialog::OnBnClickedBtnOpenDxf)
	ON_BN_CLICKED(IDC_BTN_REVERSE_FIGURE, &ABTDialog::OnBnClickedBtnReverseFigure)
	ON_BN_CLICKED(IDC_BTN_SAVE_XML, &ABTDialog::OnBnClickedBtnSaveXml)
	ON_BN_CLICKED(IDOK, &ABTDialog::OnBnClickedOk)
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_MOVE_2_STANDARD_POSITION, &ABTDialog::OnBnClickedBtnMove2StandardPosition)
	ON_BN_CLICKED(IDCANCEL, &ABTDialog::OnBnClickedCancel)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BTN_SCREEN_CAPTURE, &ABTDialog::OnBnClickedBtnScreenCapture)
	ON_COMMAND(ID_SELECT_ALL_FIGURE, &ABTDialog::OnSelectAllFigure)

	// 快捷方式
	ON_COMMAND(ID_ACCEL_SELECT_ALL_FIGURE, &ABTDialog::OnAccelSelectAllFigure)

	ON_UPDATE_COMMAND_UI(ID_ACCEL_SELECT_ALL_FIGURE, &ABTDialog::OnUpdateAccelSelectAllFigure)
	ON_COMMAND(ID_SELECT_ALL_FIGURE, &ABTDialog::OnSelectAllFigure)
	ON_BN_CLICKED(IDC_BUTTON_PARALLEL, &ABTDialog::OnBnClickedButtonParallel)
	ON_COMMAND(ID_DEL_FIGURE, &ABTDialog::OnDelFigure)
	ON_COMMAND(ID_SHOW_SHAPE_POINT, &ABTDialog::OnShowShapePoint)
	ON_COMMAND(ID_SHOW_OUTLINE, &ABTDialog::OnShowOutline)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHAPE_POINT, &ABTDialog::OnUpdateShowShapePoint)
	ON_UPDATE_COMMAND_UI(ID_SHOW_OUTLINE, &ABTDialog::OnUpdateShowOutline)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FIGURE_START_DIR, &ABTDialog::OnUpdateShowFigureStartDir)
	ON_COMMAND(ID_SHOW_FIGURE_START_DIR, &ABTDialog::OnShowFigureStartDir)
	ON_COMMAND(ID_MENU_IMPORT_REQUIREMENT, &ABTDialog::OnMenuImportRequirement)
END_MESSAGE_MAP()


BOOL ABTDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}


BOOL ABTDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	// 菜单初始化
	m_Menu2.LoadMenuA(IDR_MENU2);VK_UP;
	SetMenu(&m_Menu2);

	// 快捷键
	m_hAccel= ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// 绘图初始化

	m_ShowRect.SetRect(0 + SHOW_RECT_WINDOW_OFFSET_X, 0 + SHOW_RECT_WINDOW_OFFSET_Y, SHOW_RECT_WINDOW_WIDTH+ SHOW_RECT_WINDOW_OFFSET_X, SHOW_RECT_WINDOW_HEIGHT+ SHOW_RECT_WINDOW_OFFSET_Y);
	m_ShowRectCenter.SetPoint(m_ShowRect.left + m_ShowRect.Width()/2, m_ShowRect.top + m_ShowRect.Height()/2);




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ABTDialog::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CRect rcClient;
	GetClientRect(rcClient);
	CPaintDC dc(this);
	CDC dcMem;
	CBitmap bmpMem;
	dcMem.CreateCompatibleDC(&dc);
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);
	Gdiplus::Graphics g(dcMem.m_hDC);

	DrawWindowBackground(&g);
	ShowRect_DrawBackground(&g, m_ShowRect);
	ShowRect_DrawCross(&g, m_ShowRect);

	if (m_ShowOutlineFlag == TRUE)
		ShowRect_DrawFgure(&g);

	if (m_ShowShapePointFlag == TRUE)
		ShowRect_DrawShapePoint(&g);

	if (m_ShowFigStartDirFlag == TRUE)
		ShowRect_DrawFgureDir(&g);

	ShowRect_DrawFgureRect(&g);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();

}



// 画窗口背景
void ABTDialog::DrawWindowBackground(Gdiplus::Graphics* g)
{
	// 窗口背景
	CRect rcClient;
	GetClientRect(rcClient);
	COLORREF colBK = GetSysColor(CTLCOLOR_DLG);

	g->FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
}


// 画矩形背景
void ABTDialog::ShowRect_DrawBackground(Gdiplus::Graphics* g, CRect& rt)
{
	// 画图背景
	g->FillRectangle(&SolidBrush(BK_CLOLOR), m_ShowRect.left, m_ShowRect.top, m_ShowRect.Width(), m_ShowRect.Height());
}

// 画十字
void ABTDialog::ShowRect_DrawCross(Gdiplus::Graphics* g, CRect& rt)
{
	// 中间十字线
	int mid_y, mid_x;
	int width = m_ShowRect.Width();
	int height = m_ShowRect.Height();
	CPoint LeftTop(m_ShowRect.left, m_ShowRect.top);



	mid_x = m_ShowRectCenter.x;
	mid_y = m_ShowRectCenter.y;

	Gdiplus::Pen pen(CROSS_CLOLOR, 1);
	Gdiplus::Point P1(LeftTop.x,			mid_y);
	Gdiplus::Point P2(LeftTop.x + width,	mid_y);
	Gdiplus::Point P3(mid_x,				LeftTop.y);
	Gdiplus::Point P4(mid_x,				LeftTop.y + height);

	g->DrawLine(&pen, P1, P2);
	g->DrawLine(&pen, P3, P4);

}

// 画图形 轮廓线
void ABTDialog::ShowRect_DrawFgure(Gdiplus::Graphics* g)
{
	int i,j, i_layer, nLayerCount,nFigCount, nOutlintCount;;
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = NULL;
	Figure* pFig = NULL;
	FPoint* pPnt = NULL;

	nLayerCount = pSingleton->GetLayerNum();
	for (i_layer = 0; i_layer < nLayerCount; i_layer++)
	{
		pLayer = pSingleton->GetLayer(i_layer);

		nFigCount = pLayer->GetFigureNum();
		for (i = 0; i < nFigCount; i++)
		{
			pFig = pLayer->GetFigure(i);
			nOutlintCount = pFig->GetOutlineNum();

			// 画点数组
			Gdiplus::Point *arrDrawPnt = NULL;
			arrDrawPnt = new Gdiplus::Point[nOutlintCount];

			for (j = 0; j < nOutlintCount; j++)
			{
				int draw_x, draw_y;

				pPnt = pFig->GetOutline(j);	
				ShowRect_TransCoorFromCad2Window(pPnt->m_x, pPnt->m_y, draw_x, draw_y);
				arrDrawPnt[j].X = draw_x;
				arrDrawPnt[j].Y = draw_y;
			}

			//设置画笔
			Gdiplus::Pen pen(OUTLINE_CLOLOR_UNSELECT, 1);
			if (pFig->IsSelect() == TRUE)
			{
				pen.SetColor(OUTLINE_CLOLOR_SELECT);
				pen.SetWidth(1);
			}
			else
			{
				pen.SetColor(OUTLINE_CLOLOR_UNSELECT);
				pen.SetWidth(1);
			}

			g->DrawLines(&pen, arrDrawPnt, nOutlintCount);

			// 释放
			delete[] arrDrawPnt;
			arrDrawPnt = NULL;
		}
	}
}


// 画选中图形矩形
void ABTDialog::ShowRect_DrawFgureRect(Gdiplus::Graphics* g)
{
	int i,j, nLayerCount, nFigCount;
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = NULL;
	Figure* pFig = NULL;

	nLayerCount = pSingleton->GetLayerNum();
	for(i = 0; i < nLayerCount; i++)
	{
		pLayer = pSingleton->GetLayer(i);
		nFigCount = pLayer->GetFigureNum();

		for (j = 0; j < nFigCount; j++)
		{
			pFig = pLayer->GetFigure(j);
			if (pFig->IsSelect() == TRUE)
			{
				CRect win_rect;
				pFig->CalFigureRect();
				ShowRect_TransCoorFromCad2Window(pFig->m_FigureRect, win_rect);

				//设置画笔
				Gdiplus::Pen pen(FIGURE_RECT_CLOLOR, 1);

				Rect rt(win_rect.left, win_rect. top, win_rect.Width(), win_rect.Height());

				rt.Inflate(5,5);
				g->DrawRectangle(&pen , rt);
			}
		}
	}
}



// 画图形方向
void ABTDialog::ShowRect_DrawFgureDir(Gdiplus::Graphics* g)
{
	int i, i_layer ,nLayerCount, nFigCount;
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = pSingleton->GetLayer(0);
	Figure* pFig = NULL;
	ShapePoint* pPnt = NULL;

	nLayerCount = pSingleton->GetLayerNum();
	for (i_layer = 0; i_layer < nLayerCount; i_layer++)
	{
		pLayer = pSingleton->GetLayer(i_layer);

		nFigCount = pLayer->GetFigureNum();
		for (i = 0; i < nFigCount; i++)
		{
			pFig = pLayer->GetFigure(i);

			// 画图形方向
			int x1, y1, x2, y2;
			ShapePoint* pFirstPnt = pFig->GetShapePoint(0);
			ShapePoint* pSecondPnt = pFig->GetShapePoint(1);

			ShowRect_TransCoorFromCad2Window(pFirstPnt->m_x, pFirstPnt->m_y, x1, y1);
			ShowRect_TransCoorFromCad2Window(pSecondPnt->m_x, pSecondPnt->m_y, x2, y2);


			AdjustableArrowCap lineCap(6, 6, TRUE);
			Pen redArrowPen(Color::Green, 1);
			redArrowPen.SetCustomEndCap(&lineCap);
			g->DrawLine(&redArrowPen, x1, y1, x2, y2);
		}
	}
}

// 画形状点
void ABTDialog::ShowRect_DrawShapePoint(Gdiplus::Graphics* g)
{
	int i,j, i_layer ,nLayerCount, nFigCount, nShapePointCount;;
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	Layer* pLayer = pSingleton->GetLayer(0);
	Figure* pFig = NULL;
	ShapePoint* pPnt = NULL;

	nLayerCount = pSingleton->GetLayerNum();
	for (i_layer = 0; i_layer < nLayerCount; i_layer++)
	{
		pLayer = pSingleton->GetLayer(i_layer);
		nFigCount = pLayer->GetFigureNum();
		for (i = 0; i < nFigCount; i++)
		{
			pFig = pLayer->GetFigure(i);
			nShapePointCount = pFig->GetShapePointNum();

			for (j = 0; j < nShapePointCount; j++)
			{
				int draw_x, draw_y, r = 6;
				SolidBrush  br(SHAPEPOINT_CLOLOR);

				pPnt = pFig->GetShapePoint(j);
				ShowRect_TransCoorFromCad2Window(pPnt->m_x, pPnt->m_y, draw_x, draw_y);
				g->FillEllipse(&br, draw_x - r/2, draw_y - r/2, r, r);
			}
		}
	}
}


/*----------------------------------------------------------*/
//	purpose:
//		CAD->窗口 坐标转换 
//		
//	param:
//		float cx	--	x坐标 	 cad	
//		float cy	--	y坐标	 cad
//		int& wx		--	x坐标	 窗口
//		int& wy		--	y坐标	 窗口
//
//	return:
//		void
/*----------------------------------------------------------*/
void ABTDialog::ShowRect_TransCoorFromCad2Window(float cx, float cy, int& wx, int& wy)	
{
	wx = (int)(cx * SHOW_RECT_WINDOW_WIDTH / SHOW_RECT_CAD_WIDTH + m_ShowRectCenter.x);
	wy = (int)(-1 * cy * SHOW_RECT_WINDOW_HEIGHT / SHOW_RECT_CAD_HEIGHT + m_ShowRectCenter.y);
}


/*----------------------------------------------------------*/
//	purpose:
//		CAD->窗口 坐标转换 
//		
//	param:
//		float cx	--	x坐标 	 cad	
//		float cy	--	y坐标	 cad
//		float& wx	--	x坐标	 窗口
//		float& wy	--	y坐标	 窗口
//
//	return:
//		void
/*----------------------------------------------------------*/
void ABTDialog::ShowRect_TransCoorFromCad2Window(float cx, float cy, float& wx, float& wy)
{
	wx = (cx * SHOW_RECT_WINDOW_WIDTH / SHOW_RECT_CAD_WIDTH + m_ShowRectCenter.x);
	wy = (-1 * cy * SHOW_RECT_WINDOW_HEIGHT / SHOW_RECT_CAD_HEIGHT + m_ShowRectCenter.y);
}


/*----------------------------------------------------------*/
//	purpose:
//		CAD->窗口 矩形坐标转换 
//		
//	param:
//		CRect cad_rt	--	矩形 	 cad	
//		CRect& win_rt	--	矩形	 窗口
//
//	return:
//		void
/*----------------------------------------------------------*/
void ABTDialog::ShowRect_TransCoorFromCad2Window(CRect cad_rt, CRect& win_rt)
{
	win_rt.left = (int)(cad_rt.left * SHOW_RECT_WINDOW_WIDTH  / SHOW_RECT_CAD_WIDTH + m_ShowRectCenter.x);
	win_rt.right = (int)(cad_rt.right * SHOW_RECT_WINDOW_WIDTH  / SHOW_RECT_CAD_WIDTH + m_ShowRectCenter.x);
	win_rt.top = (int)(-1 * cad_rt.top * SHOW_RECT_WINDOW_HEIGHT / SHOW_RECT_CAD_HEIGHT + m_ShowRectCenter.y);
	win_rt.bottom = (int)(-1 * cad_rt.bottom * SHOW_RECT_WINDOW_HEIGHT / SHOW_RECT_CAD_HEIGHT + m_ShowRectCenter.y);

	win_rt.NormalizeRect();
}



 void ABTDialog::OnBnClickedBtnOpenDxf()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 CFileDialog fileDlg (TRUE, _T("dxf"), _T("*.dxf"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, _T(""), NULL);

	 if ( fileDlg.DoModal() == IDOK)
	 {
		 CString file_path = fileDlg.GetPathName();
		 DxfFile dxf_file;
		
		 // 文件数据-》内存数据管理模块
		 int  nCount;
		 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

		 // 先清空所有数据
		 pSingleton->ClearAllData();

		 // 新建图层
		 nCount = pSingleton->GetLayerNum();
		 if (nCount == 0)
			 pSingleton->NewLayer();

		 // 读取图形信息
		 if ( dxf_file.Load(file_path) == -1)
		 {
			 // 出错
			 return;
		 }
		
		 dxf_file.ReadFigureData();

		 // 排序、连接、更新图形信息
		 pSingleton->SortFigure();
		 pSingleton->LinkFigureList();
		 pSingleton->UpdateFigureInfo();
		 pSingleton->CheckIfAllFiguresLegal();

		 // 检查是否有图形
		 if (pSingleton->GetFigureNum() == 0)
		 {
			 AfxMessageBox("没有从dxf文件中读取到任何图形信息！");
		 }

		 // 自动移动到基准点
		 OnBnClickedBtnMove2StandardPosition();


		 // 重绘
		 InvalidateRect(&m_ShowRect);

	 }
 }


 void ABTDialog::OnBnClickedBtnReverseFigure()
 {
	 // TODO: 在此添加控件通知处理程序代码

	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	 vector<Figure*>* pList = pSingleton->GetSelectFigureList();

	 if (pList->size() > 0)
	 {
		 for(int i = 0; i < pList->size(); i++)
		 {
			 Figure* pFig = pList->at(i);

			 pFig->ReverseShapePointList();
			 pFig->CalOutlineList();
		 }

		 // 重绘
		 InvalidateRect(&m_ShowRect);
	 }
 }


 void ABTDialog::SaveXml(CString FilePath)
 {
	 vector<Figure*> Figurelist;

	 // 1、计算每一个图层中的每一个图形的形状点的轮廓信息
	 int nFigNum;
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

	 pSingleton->CollectAllFigures(Figurelist);
	 nFigNum = Figurelist.size();
	 if (nFigNum == 0)
	 {
		 AfxMessageBox("没有数据!");
		 return ;
	 }
	 else if (nFigNum > 1)
	 {
		 // 对形状点的Cut进行处理

		 int i;
		 Figure* pFig = NULL;
		 ShapePoint* pPnt = NULL;

		 // 最后一个图形结束的形状点
		 i = nFigNum - 1;
		 pFig = Figurelist[i];
		 pPnt = pFig->GetTailShapePoint();
		 pPnt->m_cut = Cut_KnifeUp;

		 for (i = nFigNum-2; i > 0; i--)
		 {
			 pFig = Figurelist[i];

			 pPnt = pFig->GetHeadShapePoint();
			 pPnt->m_cut = Cut_KnifeRedown;

			 pPnt = pFig->GetTailShapePoint();
			 pPnt->m_cut = Cut_KnifeUp;
		 }

		 i = 0;
		 pFig = Figurelist[i];
		 pPnt = pFig->GetHeadShapePoint();
		 pPnt->m_cut = Cut_KnifeRedown;
	 }


	 // Xml文档初始化等操作
	 TinyXml::TiXmlDocument doc(FilePath);

	 string strCategory = _T("Category");
	 string strStartSerial = _T("StartSerial");
	 string strName = _T("Name");
	 string strStyleId = _T("StyleId");

	 string strCategoryVal = _T("自定义异形");
	 string strStartSerialVal = _T("10000");
	 string strNameVal = _T("板1");
	 string strStyleIdVal = _T("10001");

	 CString strX;
	 CString strY;
	 CString strR;
	 CString strSign;
	 CString strDir;
	 CString strCut;
	 CString strType;
	 CString strA;
	 CString strGroup;
	 CString strSide;

	 TinyXml::TiXmlElement PanelOutlineNode(_T("PanelOutline"));

	 PanelOutlineNode.SetAttribute(strCategory, strCategoryVal);
	 PanelOutlineNode.SetAttribute(strStartSerial, strStartSerialVal);
	 PanelOutlineNode.SetAttribute(strName, strNameVal);
	 PanelOutlineNode.SetAttribute(strStyleId, strStyleIdVal);

	 // 写轮廓点信息
	 int i_fig;
	 nFigNum = Figurelist.size();
	 for (i_fig = nFigNum-1; i_fig >= 0; i_fig--)
	 {
		 int nShpPntNum;
		 Figure* pFig = Figurelist[i_fig];

		 nShpPntNum = pFig->GetShapePointNum();
		 for (int i = 0; i < nShpPntNum; i++)
		 {
			 TinyXml::TiXmlElement VectorElem(_T("Vector"));
			 ShapePoint* pCurPnt = pFig->GetShapePoint(i);

			 if (pCurPnt->m_sign == Sign_Ignore)
			 {
				 // 圆弧中心点,不是头尾点，插补若干个点
				 if (pCurPnt->m_FigureType == FigureType_Arc 
					 && pCurPnt->m_PntID > 0 
					 && pCurPnt->m_PntID < nShpPntNum-1)
				 {
					 int i_outline, nOutlineNum;
					 FPoint* pOutline = NULL;
					 Cad_Arc* pArc = new Cad_Arc;
					 ShapePoint* pStartPnt = new ShapePoint;
					 ShapePoint* pMidPnt = new ShapePoint;
					 ShapePoint* pEndPnt = new ShapePoint;

					 *pStartPnt = *(pFig->GetShapePoint(i-1));
					 *pMidPnt = *pCurPnt;
					 *pEndPnt = *(pFig->GetShapePoint(i+1));

					 pArc->AddShapePoint(pStartPnt);
					 pArc->AddShapePoint(pMidPnt);
					 pArc->AddShapePoint(pEndPnt);
					 pArc->CalOutlineList();

					 nOutlineNum = pArc->GetOutlineNum();
					 for (i_outline = 1; i_outline < nOutlineNum-1; i_outline++)
					 {
						 pOutline = pArc->GetOutline(i_outline);

						 strX.Format(		"%f",	pOutline->m_x);
						 strY.Format(		"%f",	pOutline->m_y);
						 strR.Format(		"%f",	pCurPnt->m_radius);
						 strSign.Format(	"%d",	Sign_Ignore);		// 忽略
						 strDir.Format(		"%d",	pCurPnt->m_dir);
						 strCut.Format(		"%d",	Cut_KnifeDown);		// 不起不落
						 strType.Format(	"%d",	pCurPnt->m_type);
						 strGroup.Format(	"%d",	Group_Regular);		// 常规点
						 strSide.Format(	"%d",	pCurPnt->m_side);

						 VectorElem.SetAttribute(_T("X"),		strX);
						 VectorElem.SetAttribute(_T("Y"),		strY);
						 VectorElem.SetAttribute(_T("R"),		strR);
						 VectorElem.SetAttribute(_T("Sign"),	strSign);
						 VectorElem.SetAttribute(_T("Dir"),		strDir);
						 VectorElem.SetAttribute(_T("Cut"),		strCut);
						 VectorElem.SetAttribute(_T("Type"),	strType);
						 VectorElem.SetAttribute(_T("Group"),	strGroup);
						 VectorElem.SetAttribute(_T("Side"),	strSide);

						 PanelOutlineNode.InsertEndChild(VectorElem);
					 }

					 delete pArc;
					 pArc = NULL;
				 }
				 else
					 continue;
			 }
			 else
			 {
				 strX.Format(		"%f",	pCurPnt->m_x);
				 strY.Format(		"%f",	pCurPnt->m_y);
				 strR.Format(		"%f",	pCurPnt->m_radius);
				 strSign.Format(	"%d",	pCurPnt->m_sign);
				 strDir.Format(		"%d",	pCurPnt->m_dir);
				 strCut.Format(		"%d",	pCurPnt->m_cut);
				 strType.Format(	"%d",	pCurPnt->m_type);
				 strGroup.Format(	"%d",	pCurPnt->m_group);
				 strSide.Format(	"%d",	pCurPnt->m_side);

				 VectorElem.SetAttribute(_T("X"),		strX);
				 VectorElem.SetAttribute(_T("Y"),		strY);
				 VectorElem.SetAttribute(_T("R"),		strR);
				 VectorElem.SetAttribute(_T("Sign"),	strSign);
				 VectorElem.SetAttribute(_T("Dir"),		strDir);
				 VectorElem.SetAttribute(_T("Cut"),		strCut);
				 VectorElem.SetAttribute(_T("Type"),	strType);
				 VectorElem.SetAttribute(_T("Group"),	strGroup);
				 VectorElem.SetAttribute(_T("Side"),	strSide);

				 PanelOutlineNode.InsertEndChild(VectorElem);
			 }
		 }
	 }

	 // 将节点插入文档
	 doc.InsertEndChild(PanelOutlineNode);

	 // 保存
	 doc.SaveFile();

	// AfxMessageBox("文件保存成功！");
 }


 void ABTDialog::OnBnClickedBtnSaveXml()
 {
	 // TODO: 在此添加控件通知处理程序代码

	 std::string path;
	 BOOL isOpen = FALSE;											//是否打开(否则为保存)  
	 CString defaultDir = _T("");									//默认打开的文件路径  
	 CString fileName = _T("test.xml");								//默认打开的文件名  
	 CString filter = _T("文件 (*.xml; *.XML)|*.xml; *.XML||");		//文件过虑的类型  
	 CFileDialog fileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);  
	 vector<Figure*> Figurelist;

	 if(fileDlg.DoModal() == IDOK)
	 {
		 CString file_path = fileDlg.GetPathName();
		 path = file_path.GetBuffer();
	 }
	 else
	 {
		 return;
	 }

	 // 1、计算每一个图层中的每一个图形的形状点的轮廓信息
	 int nFigNum;
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

	 pSingleton->CollectAllFigures(Figurelist);
	 nFigNum = Figurelist.size();
	 if (nFigNum == 0)
	 {
		 AfxMessageBox("没有数据!");
		 return ;
	 }
	 else if (nFigNum > 1)
	 {
		 // 对形状点的Cut进行处理

		 int i;
		 Figure* pFig = NULL;
		 ShapePoint* pPnt = NULL;

		 // 最后一个图形结束的形状点
		 i = nFigNum - 1;
		 pFig = Figurelist[i];
		 pPnt = pFig->GetTailShapePoint();
		 pPnt->m_cut = Cut_KnifeUp;

		 for (i = nFigNum-2; i > 0; i--)
		 {
			 pFig = Figurelist[i];

			 pPnt = pFig->GetHeadShapePoint();
			 pPnt->m_cut = Cut_KnifeRedown;

			 pPnt = pFig->GetTailShapePoint();
			 pPnt->m_cut = Cut_KnifeUp;
		 }

		 i = 0;
		 pFig = Figurelist[i];
		 pPnt = pFig->GetHeadShapePoint();
		 pPnt->m_cut = Cut_KnifeRedown;
	 }


	 // Xml文档初始化等操作
	 TinyXml::TiXmlDocument doc(path);

	 string strCategory = _T("Category");
	 string strStartSerial = _T("StartSerial");
	 string strName = _T("Name");
	 string strStyleId = _T("StyleId");

	 string strCategoryVal = _T("自定义异形");
	 string strStartSerialVal = _T("10000");
	 string strNameVal = _T("板1");
	 string strStyleIdVal = _T("10001");

	 CString strX;
	 CString strY;
	 CString strR;
	 CString strSign;
	 CString strDir;
	 CString strCut;
	 CString strType;
	 CString strA;
	 CString strGroup;
	 CString strSide;

	 TinyXml::TiXmlElement PanelOutlineNode(_T("PanelOutline"));

	 PanelOutlineNode.SetAttribute(strCategory, strCategoryVal);
	 PanelOutlineNode.SetAttribute(strStartSerial, strStartSerialVal);
	 PanelOutlineNode.SetAttribute(strName, strNameVal);
	 PanelOutlineNode.SetAttribute(strStyleId, strStyleIdVal);

	 // 写轮廓点信息
	 int i_fig;
	 nFigNum = Figurelist.size();
	 for (i_fig = nFigNum-1; i_fig >= 0; i_fig--)
	 {
	 	int nShpPntNum;
	 	Figure* pFig = Figurelist[i_fig];
	 
	 	nShpPntNum = pFig->GetShapePointNum();
	 	for (int i = 0; i < nShpPntNum; i++)
	 	{
	 		 TinyXml::TiXmlElement VectorElem(_T("Vector"));
	 		 ShapePoint* pCurPnt = pFig->GetShapePoint(i);

	 		 if (pCurPnt->m_sign == Sign_Ignore)
			 {
				 // 圆弧中心点,不是头尾点，插补若干个点
				 if (pCurPnt->m_FigureType == FigureType_Arc 
					 && pCurPnt->m_PntID > 0 
					 && pCurPnt->m_PntID < nShpPntNum-1)
				 {
					 int i_outline, nOutlineNum;
					 FPoint* pOutline = NULL;
					 Cad_Arc* pArc = new Cad_Arc;
					 ShapePoint* pStartPnt = new ShapePoint;
					 ShapePoint* pMidPnt = new ShapePoint;
					 ShapePoint* pEndPnt = new ShapePoint;

					 *pStartPnt = *(pFig->GetShapePoint(i-1));
					 *pMidPnt = *pCurPnt;
					 *pEndPnt = *(pFig->GetShapePoint(i+1));
					
					 pArc->AddShapePoint(pStartPnt);
					 pArc->AddShapePoint(pMidPnt);
					 pArc->AddShapePoint(pEndPnt);
					 pArc->CalOutlineList();

					 nOutlineNum = pArc->GetOutlineNum();
					 for (i_outline = 1; i_outline < nOutlineNum-1; i_outline++)
					 {
						 pOutline = pArc->GetOutline(i_outline);

						 strX.Format(		"%f",	pOutline->m_x);
						 strY.Format(		"%f",	pOutline->m_y);
						 strR.Format(		"%f",	pCurPnt->m_radius);
						 strSign.Format(	"%d",	Sign_Ignore);		// 忽略
						 strDir.Format(		"%d",	pCurPnt->m_dir);
						 strCut.Format(		"%d",	Cut_KnifeDown);		// 不起不落
						 strType.Format(	"%d",	pCurPnt->m_type);
						 strGroup.Format(	"%d",	Group_Regular);		// 常规点
						 strSide.Format(	"%d",	pCurPnt->m_side);

						 VectorElem.SetAttribute(_T("X"),		strX);
						 VectorElem.SetAttribute(_T("Y"),		strY);
						 VectorElem.SetAttribute(_T("R"),		strR);
						 VectorElem.SetAttribute(_T("Sign"),	strSign);
						 VectorElem.SetAttribute(_T("Dir"),		strDir);
						 VectorElem.SetAttribute(_T("Cut"),		strCut);
						 VectorElem.SetAttribute(_T("Type"),	strType);
						 VectorElem.SetAttribute(_T("Group"),	strGroup);
						 VectorElem.SetAttribute(_T("Side"),	strSide);

						 PanelOutlineNode.InsertEndChild(VectorElem);
					 }

					 delete pArc;
					 pArc = NULL;
				 }
				 else
					 continue;
			 }
			 else
			 {
				 strX.Format(		"%f",	pCurPnt->m_x);
				 strY.Format(		"%f",	pCurPnt->m_y);
				 strR.Format(		"%f",	pCurPnt->m_radius);
				 strSign.Format(	"%d",	pCurPnt->m_sign);
				 strDir.Format(		"%d",	pCurPnt->m_dir);
				 strCut.Format(		"%d",	pCurPnt->m_cut);
				 strType.Format(	"%d",	pCurPnt->m_type);
				 strGroup.Format(	"%d",	pCurPnt->m_group);
				 strSide.Format(	"%d",	pCurPnt->m_side);

				 VectorElem.SetAttribute(_T("X"),		strX);
				 VectorElem.SetAttribute(_T("Y"),		strY);
				 VectorElem.SetAttribute(_T("R"),		strR);
				 VectorElem.SetAttribute(_T("Sign"),	strSign);
				 VectorElem.SetAttribute(_T("Dir"),		strDir);
				 VectorElem.SetAttribute(_T("Cut"),		strCut);
				 VectorElem.SetAttribute(_T("Type"),	strType);
				 VectorElem.SetAttribute(_T("Group"),	strGroup);
				 VectorElem.SetAttribute(_T("Side"),	strSide);

				 PanelOutlineNode.InsertEndChild(VectorElem);
			 }
	 	}
	 }
	 
	 // 将节点插入文档
	 doc.InsertEndChild(PanelOutlineNode);
	 
	 // 保存
	 doc.SaveFile();
	 
	 AfxMessageBox("文件保存成功！");
	 
	 // 重绘
	 InvalidateRect(&m_ShowRect);
 }


 void ABTDialog::OnBnClickedOk()
 {
	 // TODO: 在此添加控件通知处理程序代码

	 int nFigNum;
	 vector<Figure*> Figurelist;
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

#if (Program_2_LiJun == 0)

	 // 1、计算每一个图层中的每一个图形的形状点的轮廓信息
	 
	 pSingleton->CollectAllFigures(Figurelist);
	 nFigNum = Figurelist.size();
	 if (nFigNum == 0)
	 {
		 AfxMessageBox("没有数据!");
		 return ;
	 }
	 else if (nFigNum > 1)
	 {
		 // 对形状点的Cut进行处理

		 int i;
		 Figure* pFig = NULL;
		 ShapePoint* pPnt = NULL;

		 // 最后一个图形结束的形状点
		 i = nFigNum - 1;
		 pFig = Figurelist[i];
		 pPnt = pFig->GetTailShapePoint();
		 pPnt->m_cut = Cut_KnifeUp;

		 for (i = nFigNum-2; i > 0; i--)
		 {
			 pFig = Figurelist[i];

			 pPnt = pFig->GetHeadShapePoint();
			 pPnt->m_cut = Cut_KnifeRedown;

			 pPnt = pFig->GetTailShapePoint();
			 pPnt->m_cut = Cut_KnifeUp;
		 }

		 i = 0;
		 pFig = Figurelist[i];
		 pPnt = pFig->GetHeadShapePoint();
		 pPnt->m_cut = Cut_KnifeRedown;
	 }

	 // 写轮廓点信息
	 int i_fig;
	 nFigNum = Figurelist.size();
	 for (i_fig = nFigNum-1; i_fig >= 0; i_fig--)
	 {
		 int nShpPntNum;
		 Figure* pFig = Figurelist[i_fig];

		 nShpPntNum = pFig->GetShapePointNum();
		 for (int i = 0; i < nShpPntNum; i++)
		 {
			 ShapePoint* pCurPnt = pFig->GetShapePoint(i);
			 if (pCurPnt->m_sign == Sign_Ignore)
			 {
				 // 圆弧中心点,不是头尾点，插补若干个点
				 if (pCurPnt->m_FigureType == FigureType_Arc 
					 && pCurPnt->m_PntID > 0 
					 && pCurPnt->m_PntID < nShpPntNum-1)
				 {
					 int i_outline, nOutlineNum;
					 FPoint* pOutline = NULL;
					 Cad_Arc* pArc = new Cad_Arc;
					 ShapePoint* pStartPnt = new ShapePoint;
					 ShapePoint* pMidPnt = new ShapePoint;
					 ShapePoint* pEndPnt = new ShapePoint;

					 *pStartPnt = *(pFig->GetShapePoint(i-1));
					 *pMidPnt = *pCurPnt;
					 *pEndPnt = *(pFig->GetShapePoint(i+1));

					 pArc->AddShapePoint(pStartPnt);
					 pArc->AddShapePoint(pMidPnt);
					 pArc->AddShapePoint(pEndPnt);
					 pArc->CalOutlineList();

					 nOutlineNum = pArc->GetOutlineNum();
					 for (i_outline = 1; i_outline < nOutlineNum-1; i_outline++)
					 {
						 pOutline = pArc->GetOutline(i_outline);

						 CString str;
						 str.Format("X:%f,Y:%f,R:%f,Sign:%d,Dir:%d,Cut:%d,Type:%d,Group:%d,Side:%d;",
							 pOutline->m_x,
							 pOutline->m_y,
							 pCurPnt->m_radius,
							 Sign_Ignore,			// 忽略
							 pCurPnt->m_dir,
							 Cut_KnifeDown,			// 不起不落
							 pCurPnt->m_type,
							 Group_Regular,			// 常规点
							 pCurPnt->m_side
							 );

						 m_strPanelOutlineNode += str;
					 }

					 delete pArc;
					 pArc = NULL;
				 }
				 else
					 continue;
			 }
			 else
			 {
				 CString str;
				 str.Format("X:%f,Y:%f,R:%f,Sign:%d,Dir:%d,Cut:%d,Type:%d,Group:%d,Side:%d;",
					 pCurPnt->m_x,
					 pCurPnt->m_y,
					 pCurPnt->m_radius,
					 pCurPnt->m_sign,
					 pCurPnt->m_dir,
					 pCurPnt->m_cut,
					 pCurPnt->m_type,
					 pCurPnt->m_group,
					 pCurPnt->m_side
					 );

				 m_strPanelOutlineNode += str;
			 }
		 }
	 }

#else

	// 保存XML到cdata\Arrangement\产品
	int nPos;
	const int nBufSize = 512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);
	CString XmlFilePath, JpgFilePath;
	CString str;
	
	 //获取当前执行文件的路径。
	GetModuleFileName(NULL,chBuf,nBufSize);

	XmlFilePath = chBuf;
	nPos = XmlFilePath.ReverseFind(_T('\\')); 
	JpgFilePath = XmlFilePath = XmlFilePath.Left(nPos+1);  
	
	// XML文件路径
	XmlFilePath +=  "cdata";
	
	// 一级
	str = XmlFilePath;  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//创建目录,已有的话不影响  

		if (!PathIsDirectory(str))
			AfxMessageBox(_T(str + "目录创建失败"));
	} 

	// 二级
	XmlFilePath += "\\Arrangement";
	str = XmlFilePath;  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//创建目录,已有的话不影响  
		if (!PathIsDirectory(str))
			AfxMessageBox(_T(str + "目录创建失败"));
	} 

	// 三级
	XmlFilePath += "\\产品";
	str = XmlFilePath;  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//创建目录,已有的话不影响  
		if (!PathIsDirectory(str))
			AfxMessageBox(_T(str + "目录创建失败"));
	} 
	


	// JPG路径
	// 一级
	JpgFilePath +=  "pdata";
	str = JpgFilePath;  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//创建目录,已有的话不影响  
		if (!PathIsDirectory(str))
			AfxMessageBox(_T(str + "目录创建失败"));
	}  

	// 二级
	JpgFilePath +=  "\\PanelStyle";
	str = JpgFilePath;  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//创建目录,已有的话不影响  
		if (!PathIsDirectory(str))
			AfxMessageBox(_T(str + "目录创建失败"));
	}  


	// 文件名
	XmlFilePath += "\\test.xml";
	JpgFilePath +=  "\\test.jpg";

	// 保存

	SaveXml(XmlFilePath);
	SaveJPG(JpgFilePath);

#endif


	 // 删除所有数据
	 if (pSingleton != NULL)
	 {
		 pSingleton->ClearAllData();
	 }


	 CDialog::OnOK();
 }


 void ABTDialog::OnRButtonUp(UINT nFlags, CPoint point)
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值

	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	 
	 pSingleton->UnSelectAllFigure();

	 InvalidateRect(m_ShowRect);

	 CDialog::OnRButtonUp(nFlags, point);
 }


 void ABTDialog::OnLButtonUp(UINT nFlags, CPoint point)
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值
// 	 CString pos;
// 
// 	 pos.Format("x:%d   y:%d", point.x, point.y);
// 	 AfxMessageBox(pos);


	 // 检测是否有选中图形
	 SelectFigure(point);


	 CDialog::OnLButtonUp(nFlags, point);
 }


 void ABTDialog::SelectFigure(CPoint point)
 {
	 int i, j, nLayerCount, nFigCount;
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	 Layer* pLayer = NULL;
	 Figure* pFig = NULL;
	 ShapePoint* pPnt = NULL;
	 CRect fig_cad_rect, fig_win_rect;

	 nLayerCount = pSingleton->GetLayerNum();

	 for (i = 0; i < nLayerCount; i++)
	 {
		 pLayer = pSingleton->GetLayer(i);
		 nFigCount = pLayer->GetFigureNum();

		 for (j = 0; j < nFigCount; j++)
		 {
			 pFig = pLayer->GetFigure(j);
		//	 pFig->CalFigureRect();			没必要每次都算，只要移动后计算即可
			 fig_cad_rect = pFig->m_FigureRect;
			 ShowRect_TransCoorFromCad2Window(fig_cad_rect, fig_win_rect);
			 fig_win_rect.InflateRect(FIGURE_SELECT_DISTANCE, FIGURE_SELECT_DISTANCE);	// 扩大矩形，方便选中

			 if (fig_win_rect.PtInRect(point))
			 {
				 if (JudgeIfFigureSelect(pFig, point) == TRUE)
				 {
					 pSingleton->SelectOneFigure(pFig);

				 }
			 }
		 }


		 InvalidateRect(m_ShowRect);
	 }

 }



 /*----------------------------------------------------------*/
 //	purpose:
 //		判断图形是否被选中，鼠标点在窗口的坐标和图形轮廓点在
 //		窗口的坐标之间的距离小于选中距离FIGURE_SELECT_DISTANCE
 //		即视为选中
 //		
 //	param:
 //		Figure* pFig -- 图形指针
 //		CPoint point -- 鼠标点在窗口的坐标
 //
 //	return:
 //		TRUE	:	选中
 //		FALSE	:	不选中
 /*----------------------------------------------------------*/
 BOOL ABTDialog::JudgeIfFigureSelect(Figure* pFig, CPoint point)
 {
	 if (pFig != NULL)
	 {
		int j, nOutlintCount;

		nOutlintCount = pFig->GetOutlineNum();
		for (j = 1; j < nOutlintCount; j++)
		{

			 int pre_draw_x, pre_draw_y, cur_draw_x, cur_draw_y;
			 float dist;
			 FPoint* pCurPnt, *pPrePnt;
		
			 pPrePnt = pFig->GetOutline(j-1);	
			 ShowRect_TransCoorFromCad2Window(pPrePnt->m_x, pPrePnt->m_y, pre_draw_x, pre_draw_y);

			 pCurPnt = pFig->GetOutline(j);	
			 ShowRect_TransCoorFromCad2Window(pCurPnt->m_x, pCurPnt->m_y, cur_draw_x, cur_draw_y);

			dist = GeneralInterface::GetDistanceFromPoint2Line((float)pre_draw_x, (float)pre_draw_y, (float)cur_draw_x, (float)cur_draw_y, (float)point.x, (float)point.y);

			//dx = (float)(point.x - draw_x);
			//dy = (float)(point.y - draw_y);

			//dist = sqrt(dx*dx + dy*dy);

			 if (dist < FIGURE_SELECT_DISTANCE)
				 return TRUE;
		}
	 }
	 
	 return FALSE;
	
 }

 // 将图形移动到第四象限
 void ABTDialog::OnBnClickedBtnMove2StandardPosition()
 {
	 // TODO: 在此添加控件通知处理程序代码

	 float rel_x = 0, rel_y = 0;
	 float maxx = 0, maxy = 0, minx = 0, miny = 0;
	 int i, j, nLayerCount, nFigCount;
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	 Layer* pLayer = NULL;
	 Figure* pFig = NULL;
	 ShapePoint* pPnt = NULL;
	 CRect fig_cad_rect;

	 nLayerCount = pSingleton->GetLayerNum();
	 if (nLayerCount > 0)
	 {

		 // 计算图形矩形
		 for (i = 0; i < nLayerCount; i++)
		 {
			 pLayer = pSingleton->GetLayer(i);
			 nFigCount = pLayer->GetFigureNum();

			 for (j = 0; j < nFigCount; j++)
			 {
				 float tmp_maxx, tmp_maxy, tmp_minx, tmp_miny;

				 pFig = pLayer->GetFigure(j);

				 // 初始化
				 if (i == 0 && j == 0)
				 {
					 pFig->CalFigureRect(maxx, maxy, minx, miny);
				 }
				 else
				 {
					// 
					pFig->CalFigureRect(tmp_maxx, tmp_maxy, tmp_minx, tmp_miny);

					if (tmp_maxx > maxx)
						maxx = tmp_maxx;

					if (tmp_minx < minx)
						minx = tmp_minx;

					if (tmp_maxy > maxy)
						maxy = tmp_maxy;

					if (tmp_miny < miny)
						miny = tmp_miny;
				 }
			 }
		 }



		 // 矩形左上角移动到原点位置
		 rel_x = 0 - minx;
		 rel_y = 0 - maxy;

		 for (i = 0; i < nLayerCount; i++)
		 {
			 pLayer = pSingleton->GetLayer(i);
			 nFigCount = pLayer->GetFigureNum();

			 for (j = 0; j < nFigCount; j++)
			 {
				 pFig = pLayer->GetFigure(j);
				 pFig->MoveFigure(rel_x, rel_y);
			 }
		 }


		 // 刷新
		 InvalidateRect(m_ShowRect);
	 }
	
 }


 void ABTDialog::OnBnClickedCancel()
 {
	 // TODO: 在此添加控件通知处理程序代码

	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();
	if (pSingleton != NULL)
	{
		pSingleton->ClearAllData();
	}

	 CDialog::OnCancel();
 }


 BOOL ABTDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值

	if (zDelta > 0)
	{
		// 放大
		SHOW_RECT_CAD_WIDTH	 = SHOW_RECT_CAD_WIDTH - ZOOM_STEP  > ZOOM_MIN_RANGE ?  SHOW_RECT_CAD_WIDTH - ZOOM_STEP : ZOOM_MIN_RANGE;
		SHOW_RECT_CAD_HEIGHT = SHOW_RECT_CAD_HEIGHT - ZOOM_STEP > ZOOM_MIN_RANGE ?  SHOW_RECT_CAD_HEIGHT - ZOOM_STEP : ZOOM_MIN_RANGE;	
	}
	else
	{
		// 缩小
		SHOW_RECT_CAD_WIDTH	 = SHOW_RECT_CAD_WIDTH + ZOOM_STEP < ZOOM_MAX_RANGE ?  SHOW_RECT_CAD_WIDTH + ZOOM_STEP : ZOOM_MAX_RANGE;
		SHOW_RECT_CAD_HEIGHT = SHOW_RECT_CAD_HEIGHT + ZOOM_STEP < ZOOM_MAX_RANGE ?  SHOW_RECT_CAD_HEIGHT + ZOOM_STEP : ZOOM_MAX_RANGE;	
	}

	InvalidateRect(m_ShowRect);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
 }


 BOOL ABTDialog::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: 在此添加专用代码和/或调用基类
	 static CPoint LBtnDnPnt, LBtnUpPnt;
	 static BOOL LBtnDnPntEnable = FALSE, LBtnUpPntEnable = FALSE;
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

	 if (pMsg->message == WM_KEYDOWN)
	 {
		  switch (pMsg->wParam)
		  {
		  case VK_SPACE:
			  SHOW_RECT_CAD_WIDTH = 2000;
			  SHOW_RECT_CAD_HEIGHT = 2000;
			  InvalidateRect(m_ShowRect);
			  break;
		  default:
			  break;
		  }
	 }

	 // 左键按下
	 if (pMsg->message == WM_LBUTTONDOWN)
	 {
		 LBtnDnPnt = pMsg->pt;			// 左键按下的屏幕坐标
		 ScreenToClient(&LBtnDnPnt);	// 左键按下的窗口客户区坐标

		 if(m_ShowRect.PtInRect(LBtnDnPnt) == TRUE)	// 点在绘图矩形区域内
			LBtnDnPntEnable = TRUE;
	 }

	 // 左键抬起
	 if (pMsg->message == WM_LBUTTONUP)
	 {
		 LBtnUpPnt = pMsg->pt;			// 左键按下的屏幕坐标
		 ScreenToClient(&LBtnUpPnt);	// 左键按下的窗口客户区坐标

		 if(m_ShowRect.PtInRect(LBtnUpPnt) == TRUE)
			 LBtnUpPntEnable = TRUE;

		 if (LBtnDnPntEnable == TRUE && LBtnUpPntEnable == TRUE)
		 {
			 // 移动
			 
			 CPoint Rel = LBtnUpPnt - LBtnDnPnt;
			 float dx = LBtnUpPnt.x - LBtnDnPnt.x;
			 float dy = -1*(LBtnUpPnt.y - LBtnDnPnt.y);		// 客户区坐标和CAD坐标y轴是相反的
			 float len = sqrt(dx*dx + dy*dy);

			 if(len > MIN_MOVE_STEP)	// 大于10个像素点，就判定为要移动
			 {
				pSingleton->MoveSelectFigure(dx, dy);
				InvalidateRect(m_ShowRect);
			 }
		 }

		 // 重置
		 LBtnDnPntEnable = LBtnUpPntEnable = FALSE;
	 }

	 if (pMsg->message == WM_KEYDOWN)
	 {
		 switch(pMsg->wParam)
		 {
		 case VK_DELETE:
			 pSingleton->DelSelectFigure();
			 pSingleton->UnSelectAllFigure();
			 InvalidateRect(m_ShowRect);
			 break;
		 case  'A' :
			if (GetAsyncKeyState(VK_CONTROL))
			{
				OnSelectAllFigure();
			}
			break;
		 default:
			break;
		 }
	 }



// 	 if(m_hAccel   !=   NULL)   
// 	 {   
// 		 if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
// 
// 			 return   TRUE;  
// 
// 	 }    


	 return CDialog::PreTranslateMessage(pMsg);
 }




 void Screen(char filename[], int client_x, int client_y, int width, int height)
 {
	 CDC *pDC;//屏幕DC
	 pDC = CDC::FromHandle(GetDC(NULL));//获取当前整个屏幕DC
	 int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	 int Width = width /*pDC->GetDeviceCaps(HORZRES)*/;
	 int Height = height /*pDC->GetDeviceCaps(VERTRES)*/;

	 // 	printf("当前屏幕色彩模式为%d位色彩n", BitPerPixel);
	 // 	printf("屏幕宽度：%dn", Width);
	 // 	printf("屏幕高度：%dn", Height);

	 CDC memDC;//内存DC
	 memDC.CreateCompatibleDC(pDC);

	 CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	 memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	 oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	 memDC.BitBlt(0, 0, Width, Height, pDC, client_x, client_y, SRCCOPY);//复制屏幕图像到内存DC

	 //以下代码保存memDC中的位图到文件
	 BITMAP bmp;
	 memBitmap.GetBitmap(&bmp);//获得位图信息

	 FILE *fp = fopen(filename, "w+b");

	 BITMAPINFOHEADER bih = {0};//位图信息头
	 bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	 bih.biCompression = BI_RGB;
	 bih.biHeight = bmp.bmHeight;//高度
	 bih.biPlanes = 1;
	 bih.biSize = sizeof(BITMAPINFOHEADER);
	 bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	 bih.biWidth = bmp.bmWidth;//宽度

	 BITMAPFILEHEADER bfh = {0};//位图文件头
	 bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	 bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	 bfh.bfType = (WORD)0x4d42;

	 fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头

	 fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头

	 byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据

	 GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
		 (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据

	 fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据

	 delete [] p;

	 fclose(fp);

	 memDC.SelectObject(oldmemBitmap);
 }



 void ABTDialog::SaveJPG(CString FilePath)
 {
	 int left, top, width, height;
	 CRect win_rect, client_rt;
	 GetWindowRect(&win_rect);
	 GetClientRect(&client_rt);

	 width = m_ShowRect.Width()/2 ;
	 height =  m_ShowRect.Height()/2;
	 left = win_rect.left + m_ShowRect.left + width;
	 top = win_rect.top + m_ShowRect.top + height;

	 Screen(FilePath.GetBuffer(), left, top, width, height);
 }

 void ABTDialog::OnBnClickedBtnScreenCapture()
 {
	 // TODO: 在此添加控件通知处理程序代码
#if 1

	 CString file_path;
	 BOOL isOpen = FALSE;											//是否打开(否则为保存)  
	 CString defaultDir = _T("");									//默认打开的文件路径  
	 CString fileName = _T("test.jpg");								//默认打开的文件名  
	 CString filter = _T("文件 (*.jpg; *.JPG)|*.jpg; *.JPG||");		//文件过虑的类型  
	 CFileDialog fileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);  

	 if(fileDlg.DoModal() == IDOK)
	 {
		 file_path = fileDlg.GetPathName();
	 }
	 else
	 {
		 return;
	 }

	 SaveJPG(file_path);


// 	 int left, top, width, height;
// 	 CRect win_rect, client_rt;
// 	 GetWindowRect(&win_rect);
// 	 GetClientRect(&client_rt);
// 	 
// 	 width = m_ShowRect.Width()/2 ;
// 	 height =  m_ShowRect.Height()/2 + 10;
// 	 left = win_rect.left + m_ShowRect.left + width;
// 	 top = win_rect.top + m_ShowRect.top + height;
// 
// 
// 
// 	 Screen(file_path.GetBuffer(), left, top, width, height);
#else
	 
	 float center_x, center_y;
	 GetCircleCenter(100, -100, 0, 0 ,100, 0, center_x, center_y);

#endif

 }


 void ABTDialog::OnAccelSelectAllFigure()
 {
	 // TODO: 在此添加命令处理程序代码

	 AfxMessageBox("quanxuan !");
 }


 void ABTDialog::OnUpdateAccelSelectAllFigure(CCmdUI *pCmdUI)
 {
	 // TODO: 在此添加命令更新用户界面处理程序代码
 }


 void ABTDialog::OnSelectAllFigure()
 {
	 // TODO: 在此添加命令处理程序代码
	CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

	pSingleton->SelectAllFigure();
	InvalidateRect(m_ShowRect);

 }

 void ABTDialog::OnBnClickedButtonParallel()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 ParallelDialog dlg;

	 dlg.DoModal();

	 InvalidateRect(m_ShowRect);
 }


 void ABTDialog::OnDelFigure()
 {
	 // TODO: 在此添加命令处理程序代码
	 CAbnormalToolsSingleton* pSingleton = CAbnormalToolsSingleton::GetSingleton();

	 pSingleton->DelSelectFigure();
	 InvalidateRect(m_ShowRect);
 }


 void ABTDialog::OnShowShapePoint()
 {
	 // TODO: 在此添加命令处理程序代码
 }


 void ABTDialog::OnShowOutline()
 {
	 // TODO: 在此添加命令处理程序代码
 }


 void ABTDialog::OnUpdateShowShapePoint(CCmdUI *pCmdUI)
 {
	 // TODO: 在此添加命令更新用户界面处理程序代码
	 if (m_ShowShapePointFlag == TRUE)
		m_ShowShapePointFlag = FALSE;
	 else
		 m_ShowShapePointFlag = TRUE;

	 pCmdUI->SetCheck(0);
	 pCmdUI->Enable(FALSE);
	 InvalidateRect(m_ShowRect);
 }


 void ABTDialog::OnUpdateShowOutline(CCmdUI *pCmdUI)
 {
	 // TODO: 在此添加命令更新用户界面处理程序代码
	 if (m_ShowOutlineFlag == TRUE)
		 m_ShowOutlineFlag = FALSE;
	 else
		 m_ShowOutlineFlag = TRUE;

	 pCmdUI->SetCheck(m_ShowOutlineFlag);
	 InvalidateRect(m_ShowRect);
 }


 void ABTDialog::OnUpdateShowFigureStartDir(CCmdUI *pCmdUI)
 {
	 // TODO: 在此添加命令更新用户界面处理程序代码
	 if (m_ShowFigStartDirFlag == TRUE)
		 m_ShowFigStartDirFlag = FALSE;
	 else
		 m_ShowFigStartDirFlag = TRUE;

	 pCmdUI->SetCheck(m_ShowFigStartDirFlag);
	 InvalidateRect(m_ShowRect);
 }


 void ABTDialog::OnShowFigureStartDir()
 {
	 // TODO: 在此添加命令处理程序代码
 }


 void ABTDialog::OnMenuImportRequirement()
 {
	 // TODO: 在此添加命令处理程序代码
	 CString info = "1、必须是闭合的圆弧和直线！（图形必须打散）;\n2、格式是cad的dxf格式;\n3、导入后必须点界面的移到基准点;\n4、板件有多大，请把图画多大。圆弧多大请画多大，一切以图形为准;\n5、绿色针头为轮廓方向，外轮廓的方向为逆时针，内轮廓方向为顺时针（板件形状为外轮廓，其余则为内轮廓）";
		 
	 AfxMessageBox(info);


 }

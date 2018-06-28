#include "stdafx.h"
#include "Drawing.h"
#include "../Misc/ProgramMisc.h"
#include "../Misc/Misc.h"

#define PI 3.14159

void DrawPanelDiagramToA4(HDC hdc, Panel* pPanel, CRect rcDrawingArea, int nPanelID, PanelDiagramPrintingStyle eStyle)
{
	CDC dc;
	dc.Attach(hdc);

	int nTextSize = 10, nTextSize_Barcode = 8;
	switch(eStyle)
	{
	case PRINT_STYLE_A4_1:
		nTextSize = 19;
		nTextSize_Barcode = 14;
		break;
	case PRINT_STYLE_A4_4:
		nTextSize = 10;
		nTextSize_Barcode = 8;
		break;
	case PRINT_STYLE_LABEL:
		nTextSize = 8;
		nTextSize_Barcode = 8;
		break;
	}

	CRect rcDrawingArea_Panel;
	rcDrawingArea_Panel.CopyRect(rcDrawingArea);
	if(eStyle != PRINT_STYLE_LABEL)
		rcDrawingArea_Panel.bottom -= rcDrawingArea.Height()/2;
	else
		rcDrawingArea_Panel.bottom -= rcDrawingArea.Height() * 1.0/6.0;

	DrawingPanelParam param;
	param.m_nGap = 0;
	param.m_bDrawMachiningInfo = false;

	DrawPanel(&dc, pPanel, rcDrawingArea_Panel, param);
	DrawMachiningOrder(&dc, pPanel, rcDrawingArea_Panel, 0, nTextSize);

	CRect rcDrawingAreaSummary;
	if(eStyle != PRINT_STYLE_LABEL)
		rcDrawingAreaSummary.SetRect(rcDrawingArea.left, rcDrawingArea.top + rcDrawingArea.Height()/2+10, rcDrawingArea.right, rcDrawingArea.bottom);
	else
		rcDrawingAreaSummary.SetRect(rcDrawingArea.left, rcDrawingArea_Panel.bottom + 3, rcDrawingArea.right, rcDrawingArea.bottom);
	DrawPanelSummary(&dc, pPanel, rcDrawingAreaSummary, nTextSize, nPanelID);
	
	if(eStyle != PRINT_STYLE_LABEL)
	{
		CRect rcDrawingAreaBarcodeList;
		rcDrawingAreaBarcodeList.SetRect(rcDrawingArea.left, rcDrawingArea.top + rcDrawingArea.Height()/2+10+30, rcDrawingArea.right, rcDrawingArea.bottom);
		DrawBarcodeList(&dc, pPanel, rcDrawingAreaBarcodeList, nTextSize_Barcode);
	}

	dc.Detach();
}

// 打印大板标签
bool DrawPanelInfoToDC(HDC hdc, Panel* pPanel, float org_scale, int rightmove, int bottommove, int num, /*CFont* font*/PanelDiagramPrintingStyle eStyle)
{
	float scale = org_scale * 1.2;	//2016-8-18 yuanzb 大图时显示标签，将图缩小，腾出右边的空间

	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	int nTextSize;
	switch(eStyle)
	{
	case PRINT_STYLE_A4_1:
		nTextSize = 19;
		break;
	case PRINT_STYLE_A4_4:
		nTextSize = 10;
		break;
	case PRINT_STYLE_LABEL:
		nTextSize = 8;
		break;
	default:
		nTextSize = 10;
		break;
	}
	Gdiplus::Font fontText(L"Arial", nTextSize, FontStyleRegular/*, UnitPixel*/);
	Gdiplus::Font fontComponentInfo(L"Times New Roman", (eStyle == PRINT_STYLE_A4_1) ? 14 : 8, FontStyleRegular/*, UnitPixel*/);
	StringFormat sf;
	StringFormat sfCenter;
	sfCenter.SetAlignment(StringAlignmentCenter);
	sfCenter.SetLineAlignment(StringAlignmentCenter);
	SolidBrush brush(Color::Black);


	int nPanelLen = /*pPanel->m_RealLength*/pPanel->m_OrgLen;
	int nPanelWidth = /*pPanel->m_RealWidth*/pPanel->m_OrgWidth;

	float fScale = 1.0 /(org_scale * 1.2);

	CRect rcPanel(rightmove, \
		bottommove, \
		rightmove + nPanelLen*fScale, \
		bottommove + nPanelWidth*fScale);

	//g.FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(*pPanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (/*pPanel->m_RealWidth*/nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (/*pPanel->m_RealWidth*/nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		//g->FillRectangle(&SolidBrush(Color(255, 255, 224)), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
		//g.DrawRectangle(&Pen(Color::Black, 1), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
		CString str;
		str.Format("%d", theComponent.m_NumberInPanel);

		////g->DrawString(AnsiToUnicode(str).c_str(), -1, &font, PointF((rcComponent.left + rcComponent.right)/2, (rcComponent.top + rcComponent.bottom)/2), &sf, &brush);
		RectF rfText(rcComponent.left - 50, rcComponent.top - 50, rcComponent.Width() + 100, rcComponent.Height() + 100);
		g.DrawString(AnsiToUnicode(str).c_str(), -1, &fontText, rfText, &sfCenter, &brush);

		vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
		if(vOutlinePoint.size() > 0)
		{
			for(int j = 1; j < vOutlinePoint.size(); j++)
			{
				PointInfo& vertexStart = vOutlinePoint[j - 1];
				PointInfo& vertexEnd = vOutlinePoint[j];

				DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd);
			}
		}
		else
		{
			g.DrawRectangle(&Pen(Color::Black, 1), rcComponent.left, rcComponent.top, rcComponent.Width(), rcComponent.Height());
		}

		//DrawMachiningInfo(*g, rcComponent, fScale, &theComponent);

	}

	g.DrawRectangle(&Pen(Color::Black, 2), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	CString str_num, strThicknessAndMeterial;
	strThicknessAndMeterial.Format(_T("%s-%s"), GetFloatString(pPanel->m_Thickness, 1), pPanel->m_Material);
	str_num.Format("【第%d块板】  【板数:%d】  【%s】", num, vAllComponent.size(), strThicknessAndMeterial);
	g.DrawString(AnsiToUnicode(str_num).c_str(), -1, &fontText, PointF(rightmove, bottommove + /*pPanel->m_RealWidth*/nPanelWidth * fScale + 10), &sf, &brush);

	//int font_width = 20;
	int info_num_per_column = 10;
	int info_width = 400 * ((eStyle == PRINT_STYLE_A4_1) ? 1 : 0.5);
	int info_height = 30 * ((eStyle == PRINT_STYLE_A4_1) ? 1 : 0.5);

	if(eStyle != PRINT_STYLE_LABEL)
	{
		for(int i = 0; i < vAllComponent.size(); i++)
		{
			Component& theComponent = *(vAllComponent[i]);
			CString strComponentInfo;
			strComponentInfo.Format(_T("(%d)%s*%s %s"), theComponent.m_NumberInPanel, GetFloatString(theComponent.m_RealLength, 0), GetFloatString(theComponent.m_RealWidth, 0), theComponent.m_BarCode);

			PointF ptText;
			ptText.X = rightmove + ((theComponent.m_NumberInPanel - 1)/info_num_per_column)*info_width;
			ptText.Y = bottommove + /*pPanel->m_RealWidth*/nPanelWidth * fScale + 10 + 40 * ((eStyle == PRINT_STYLE_A4_1) ? 1 : 0.5) + ((theComponent.m_NumberInPanel - 1)%info_num_per_column) * info_height;
			g.DrawString(AnsiToUnicode(strComponentInfo).c_str(), -1, &fontComponentInfo, ptText, &sf, &brush);

		}
	}

	return true;
}

void DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color)
{
	if(vertexStart.group == OUTLINE_GROUP_TYPE_END)
		return;

	Pen pen(color, 1);
	//pen.SetDashStyle(DashStyleDash);

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
		//vertexStart.m_nDir = OutlineVertex::ANTI_CLOCKWISE;
		int eArcDir = vertexEnd.dir;
		float fXa = vertexStart.x, fYa = vertexStart.y, fXb = vertexEnd.x, fYb = vertexEnd.y, fR = /*vertexStart.r*/vertexEnd.r;
		//strTmp.Format(L"fXa = %f, fYa = %f, fXb = %f, fYb = %f, fR = %f\n", fXa, fYa, fXb, fYb, fR);
		//OutputDebugString(strTmp);
		float fDx = fXa - fXb;
		//strTmp.Format(L"fDx = %f\n", fDx);
		//OutputDebugString(strTmp);
		float fDy = fYa - fYb;
		//strTmp.Format(L"fDy = %f\n", fDy);
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
			//strTmp.Format(L"fD = %f\n", fD);
			//OutputDebugString(strTmp);
			float fa = 1 + fD*fD;
			float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
			float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
			//strTmp.Format(L"fa = %f, fb = %f, fc = %f\n", fa, fb, fc);
			//OutputDebugString(strTmp);
			fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
			fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
		}

		strTmp.Format("fCenterX1 = %f, fCenterY1 = %f\n", fCenterX1, fCenterY1);
		OutputDebugString(strTmp);
		strTmp.Format("fCenterX2 = %f, fCenterY2 = %f\n", fCenterX2, fCenterY2);
		OutputDebugString(strTmp);
		//SolidBrush brush(Color::Black);
		//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX1*fScale), (REAL)(rcPanel.top +fCenterY1*fScale), (REAL)3, (REAL)3);
		//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX2*fScale), (REAL)(rcPanel.top +fCenterY2*fScale), (REAL)3, (REAL)3);

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

void DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, DrawingPanelParam& param)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	//Gdiplus::Font font(L"宋体", 18, FontStyleRegular/*, UnitPixel*/);
	//StringFormat sf;
	//SolidBrush brush(Color::Red);


	Panel& thePanel = *(pPanel);
	int nPanelLen = thePanel.m_OrgLen;
	int nPanelWidth =thePanel.m_OrgWidth;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-param.m_nGap, rcDrawArea.Height()-param.m_nGap);
	CRect rcPanelArea(param.m_nGap/2, param.m_nGap/2, rcDrawArea.Width()-param.m_nGap/2, rcDrawArea.Height()-param.m_nGap/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	g.FillRectangle(&SolidBrush(Color(255, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		//g.FillRectangle(&SolidBrush(Color(255, 255, 224)), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
		//g.DrawRectangle(&Pen(Color::Black, 1), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());
		//CString str;
		//str.Format("%d", theComponent.m_NumberInPanel);

		//RectF rfText(rcComponent.left, rcComponent.top, rcComponent.Width(), rcComponent.Height());
		//g.DrawString(AnsiToUnicode(str).c_str(), -1, &font, rfText, &sf, &brush);

		vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
		for(int j = 1; j < vOutlinePoint.size(); j++)
		{
			PointInfo& vertexStart = vOutlinePoint[j - 1];
			PointInfo& vertexEnd = vOutlinePoint[j];

			DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd, Color(GetRValue(param.m_color), GetGValue(param.m_color), GetBValue(param.m_color)));
		}

		//DrawMachiningInfo(*g, rcComponent, fScale, &theComponent);

	}

	g.DrawRectangle(&Pen(Color(GetRValue(param.m_color), GetGValue(param.m_color), GetBValue(param.m_color)), 2), \
		(INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	if(param.m_bDrawDeberredPanel)
	{
		CRect rcDeburredPanel(pPanel->m_OriginX*fScale, (nPanelWidth - (pPanel->m_OriginY + pPanel->m_RealWidth))*fScale, \
			(pPanel->m_OriginX + pPanel->m_RealLength)*fScale, (nPanelWidth - pPanel->m_OriginY)*fScale);
		rcDeburredPanel.OffsetRect(rcPanel.left, rcPanel.top);

		Pen penDeburring(Color(GetRValue(param.m_color), GetGValue(param.m_color), GetBValue(param.m_color)), 1);
		penDeburring.SetDashStyle(DashStyleDash);
		g.DrawRectangle(&penDeburring, \
			(INT)rcDeburredPanel.left, (INT)rcDeburredPanel.top, (INT)rcDeburredPanel.Width(), (INT)rcDeburredPanel.Height());
	}
}

void DrawComponent(CDC* pDC, Component* pComponent, CRect rcDrawArea, DrawingPanelParam& param)
{
	int nComponentXLen = pComponent->m_RealLength;
	int nComponentYLen = pComponent->m_RealWidth;

	Graphics g(pDC->GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeHighQuality);

	float fScale = GetScale(nComponentXLen, nComponentYLen, rcDrawArea.Width()-param.m_nGap, rcDrawArea.Height()-param.m_nGap);

	CRect rcPanelArea(param.m_nGap/2, param.m_nGap/2, rcDrawArea.Width()-param.m_nGap/2, rcDrawArea.Height()-param.m_nGap/2);

	CRect rcPanel((rcDrawArea.Width()-nComponentXLen*fScale)/2, \
		(rcDrawArea.Height()-nComponentYLen*fScale)/2, \
		(rcDrawArea.Width()-nComponentXLen*fScale)/2 + nComponentXLen*fScale, \
		(rcDrawArea.Height()-nComponentYLen*fScale)/2 + nComponentYLen*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	//g.FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	vector<PointInfo>& vOutlinePoint = pComponent->m_vOutlinePoint;
	Color color(GetRValue(param.m_color), GetGValue(param.m_color), GetBValue(param.m_color));
	if(vOutlinePoint.size() > 0)
	{
		for(int j = 1; j < vOutlinePoint.size(); j++)
		{
			PointInfo& vertexStart = vOutlinePoint[j - 1];
			PointInfo& vertexEnd = vOutlinePoint[j];

			DrawOneLine(g, rcPanel, fScale, vertexStart, vertexEnd, color);
		}
	}
	else
	{
		g.DrawRectangle(&Pen(color, 1), rcPanel.left, rcPanel.top, rcPanel.Width(), rcPanel.Height());
	}

	if(param.m_bDrawMachiningInfo)
	{
		DrawMachiningInfo(g, rcPanel, fScale, pComponent);
	}
}

void DrawMachiningInfo(Graphics& g, CRect rcComponent, float fScale, Component* pComponent)
{
	SolidBrush brush(Color::Blue);//填充的颜色

	for(vector<AllVec>::iterator itr = pComponent->m_vUpperFaceHole.begin(); itr != pComponent->m_vUpperFaceHole.end(); itr++)
	{
		float x = itr->_x * fScale;
		float y = (pComponent->m_RealWidth - itr->_y) * fScale;	
		g.FillEllipse(&brush, rcComponent.left + (x - (itr->_r * fScale) / 2), rcComponent.top + (y - (itr->_r * fScale) / 2), itr->_r * fScale, itr->_r * fScale);
	}

	for(vector<AllSlot>::iterator itr = pComponent->m_vUpperFaceSlot.begin(); itr != pComponent->m_vUpperFaceSlot.end(); itr++)
	{
		CRect rcSlot;
		rcSlot.left = (itr->_x - (itr->_width / 2)) * fScale;
		rcSlot.top = (pComponent->m_RealWidth - (itr->_y + (itr->_height / 2))) * fScale;
		rcSlot.right = (itr->_x + (itr->_width / 2)) * fScale;
		rcSlot.bottom = (pComponent->m_RealWidth - (itr->_y - (itr->_height / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcComponent.left, (rcSlot.top+rcSlot.bottom)/2 + rcComponent.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->m_angle*/itr->_dir == 0 ? 90 : 0);
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(&brush, rcComponent.left + rcSlot.left, rcComponent.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}


	//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//vector<PointInfo> vOutterOutlinePoint;
	//GetOutterOutlineVertex(pComponent->m_vOutlinePoint, vOutterOutlinePoint);
	//if(vOutterOutlinePoint.size() == 0)
	//	return;
	//int nKnifeDownVertexIDStart = pComponent->m_nKnifeDownPos % (vOutterOutlinePoint.size()-1);
	//int nKnifeDownVertexIDEnd = (pComponent->m_nKnifeDownPos + 1) % (vOutterOutlinePoint.size()-1);
	//PointInfo& vertexStart = vOutterOutlinePoint[nKnifeDownVertexIDStart];
	//PointInfo& vertexEnd = vOutterOutlinePoint[nKnifeDownVertexIDEnd];
	//float fXKnifeDown, fYKnifeDown;
	//float fXKnifeDown_UI, fYKnifeDown_UI;
	//float fEdgeLen = GetPointDistance(vertexStart.x, vertexStart.y, vertexEnd.x, vertexEnd.y);
	//fXKnifeDown = vertexEnd.x + pMainFrame->m_pDlgBaseParam->m_distance2 * (vertexStart.x - vertexEnd.x)/ fEdgeLen;
	//fYKnifeDown = vertexEnd.y + pMainFrame->m_pDlgBaseParam->m_distance2 * (vertexStart.y - vertexEnd.y)/ fEdgeLen;
	//fXKnifeDown_UI = rcComponent.left + (fXKnifeDown * fScale);
	//fYKnifeDown_UI = rcComponent.top + (pComponent->m_RealWidth - fYKnifeDown)* fScale;
	//g.FillEllipse(&SolidBrush(Color::Red), (INT)(fXKnifeDown_UI - KNIFE_DOWN_CIRCLE_DIA/2), (INT)(fYKnifeDown_UI - KNIFE_DOWN_CIRCLE_DIA/2), KNIFE_DOWN_CIRCLE_DIA ,KNIFE_DOWN_CIRCLE_DIA);
}

void DrawMachiningOrder(CDC* pDC, Panel* pPanel, CRect rcDrawArea, int nGap, int nTextSize)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	Gdiplus::Font fontText(L"Arial", nTextSize, FontStyleRegular);
	StringFormat sfCenter;
	sfCenter.SetAlignment(StringAlignmentCenter);
	sfCenter.SetLineAlignment(StringAlignmentCenter);
	SolidBrush brush(Color::Black);


	Panel& thePanel = *(pPanel);
	int nPanelLen = thePanel.m_OrgLen;
	int nPanelWidth =thePanel.m_OrgWidth;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-nGap, rcDrawArea.Height()-nGap);
	CRect rcPanelArea(nGap/2, nGap/2, rcDrawArea.Width()-nGap/2, rcDrawArea.Height()-nGap/2);

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

		int nSetID = theComponent.m_NumberInPanel;
		CString strSetID;
		strSetID.Format(_T("%d"), nSetID);

		RectF rfText(rcComponent.left - 50, rcComponent.top - 50, rcComponent.Width() + 100, rcComponent.Height() + 100);
		g.DrawString(AnsiToUnicode(strSetID).c_str(), -1, &fontText, rfText, &sfCenter, &brush);
	}
}

void DrawPanelSummary(CDC* pDC, Panel* pPanel, CRect rcDrawArea, int nTextSize, int nPanelID)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Font fontText(L"Arial", nTextSize, FontStyleRegular);
	RectF rfText(rcDrawArea.left, rcDrawArea.top, rcDrawArea.Width(), rcDrawArea.Height());
	StringFormat sf;
	SolidBrush brush(Color::Black);

	CString str_num, strThicknessAndMeterial;
	strThicknessAndMeterial.Format(_T("%s-%s"), GetFloatString(pPanel->m_Thickness, 1), pPanel->m_Material);
	str_num.Format("【第%d块板】  【板数:%d】  【%s】", nPanelID, GetComponentCountInPanel(*pPanel), strThicknessAndMeterial);
	g.DrawString(AnsiToUnicode(str_num).c_str(), -1, &fontText, rfText, &sf, &brush);
}

void DrawBarcodeList(CDC* pDC, Panel* pPanel, CRect rcDrawArea, int nTextSize)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Font fontText(L"Times New Roman", nTextSize, FontStyleRegular);
	StringFormat sf;
	SolidBrush brush(Color::Black);

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(*pPanel, vAllComponent);

	int info_num_per_column = 10;
	int info_width;
	int info_height;
	if(vAllComponent.size() <= 20)
	{
		info_width = rcDrawArea.Width() / 2;
		info_height = rcDrawArea.Height() / info_num_per_column;
	}
	else
	{
		info_width = rcDrawArea.Width() / 3;
		info_height = rcDrawArea.Height() / info_num_per_column;
	}

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CString strComponentInfo;
		if(vAllComponent.size() <= 20)
			strComponentInfo.Format(_T("(%d)%s*%s %s"), theComponent.m_NumberInPanel, GetFloatStringTrimming(theComponent.m_RealLength, 1), GetFloatStringTrimming(theComponent.m_RealWidth, 1), theComponent.m_BarCode);
		else
			strComponentInfo.Format(_T("(%d)%s"), theComponent.m_NumberInPanel, theComponent.m_BarCode);

		PointF ptText;
		ptText.X = rcDrawArea.left + ((theComponent.m_NumberInPanel - 1)/info_num_per_column)*info_width;
		ptText.Y = rcDrawArea.top + ((theComponent.m_NumberInPanel - 1)%info_num_per_column) * info_height;
		g.DrawString(AnsiToUnicode(strComponentInfo).c_str(), -1, &fontText, ptText, &sf, &brush);

	}
}

void DrawComponentInPanel(CDC* pDC, Component* pComponent, CRect rcDrawArea, DrawingPanelParam& param, float fPanelXLen, float fPanelYLen)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	int nPanelLen = fPanelXLen;
	int nPanelWidth = fPanelYLen;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-param.m_nGap, rcDrawArea.Height()-param.m_nGap);
	CRect rcPanelArea(param.m_nGap/2, param.m_nGap/2, rcDrawArea.Width()-param.m_nGap/2, rcDrawArea.Height()-param.m_nGap/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	//g.FillRectangle(&SolidBrush(Color(255, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	//vector<Component*> vAllComponent;
	//FindAllComponentInPanel(thePanel, vAllComponent);

	//for(int i = 0; i < vAllComponent.size(); i++)
	//{
		Component& theComponent = *pComponent;
		CRect rcComponent(theComponent.m_x*fScale, (nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
		for(int j = 1; j < vOutlinePoint.size(); j++)
		{
			PointInfo& vertexStart = vOutlinePoint[j - 1];
			PointInfo& vertexEnd = vOutlinePoint[j];

			DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd, Color(GetRValue(param.m_color), GetGValue(param.m_color), GetBValue(param.m_color)));
		}

		//DrawMachiningInfo(*g, rcComponent, fScale, &theComponent);

	//}

	//g.DrawRectangle(&Pen(Color::Black, 2), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());
}

void DrawOutlineLineInPanel(CDC* pDC, vector<vector<OutlineLine>>& vMillingOutlineGroup, float fComponentX, float fComponentY, CRect rcDrawArea, DrawingPanelParam& param, float fPanelXLen, float fPanelYLen)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	int nPanelLen = fPanelXLen;
	int nPanelWidth = fPanelYLen;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-param.m_nGap, rcDrawArea.Height()-param.m_nGap);
	CRect rcPanelArea(param.m_nGap/2, param.m_nGap/2, rcDrawArea.Width()-param.m_nGap/2, rcDrawArea.Height()-param.m_nGap/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	rcPanel.OffsetRect(rcDrawArea.left, rcDrawArea.top);

	//Component& theComponent = *pComponent;
	CRect rcComponent(fComponentX*fScale, (nPanelWidth - (fComponentY + 100))*fScale, (fComponentX + 100)*fScale, (nPanelWidth - fComponentY)*fScale);
	rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

	for(int i = 0; i < vMillingOutlineGroup.size(); i++)
	{
		for(int j = 0; j < vMillingOutlineGroup[i].size(); j++)
		{
			OutlineLine& line = vMillingOutlineGroup[i][j];

			PointInfo vertexStart, vertexEnd;
			vertexStart.x = line.m_fStartX;
			vertexStart.y = line.m_fStartY;
			vertexStart.sign = line.m_nLineType == STRAIGHT_LINE ? 0 : 1;
			vertexEnd.x = line.m_fEndX;
			vertexEnd.y = line.m_fEndY;
			vertexEnd.r = line.m_fR;
			vertexEnd.dir = line.m_nDir;

			DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd, Color(GetRValue(param.m_color), GetGValue(param.m_color), GetBValue(param.m_color)));
		}
	}
}

void DrawPanelDiagram_ComponentLabel(HDC hdc, int nImageXLen, int nImageYLen, Panel* pPanel, int nComponentIDInPanel)
{
	CDC dc;
	dc.Attach(hdc);

	dc.FillSolidRect(0, 0, nImageXLen, nImageYLen, RGB(252, 252, 252));

	CRect rcDrawArea;
	rcDrawArea.SetRect(0, 0, nImageXLen, nImageYLen);
	DrawingPanelParam param;
	param.m_nGap = 0;
	DrawPanel(&dc, pPanel, rcDrawArea, param);

	Graphics g(dc.m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	Panel& thePanel = *(pPanel);
	int nPanelLen = thePanel.m_OrgLen;
	int nPanelWidth =thePanel.m_OrgWidth;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-param.m_nGap, rcDrawArea.Height()-param.m_nGap);
	CRect rcPanelArea(param.m_nGap/2, param.m_nGap/2, rcDrawArea.Width()-param.m_nGap/2, rcDrawArea.Height()-param.m_nGap/2);

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
		if(theComponent.m_NumberInPanel == nComponentIDInPanel)
		{
			CRect rcComponent(theComponent.m_x*fScale, (nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (nPanelWidth - theComponent.m_y)*fScale);
			rcComponent.OffsetRect(rcPanel.left, rcPanel.top);
			g.FillRectangle(&SolidBrush(Color(0,0,0)), rcComponent.left, rcComponent.top, rcComponent.Width(), rcComponent.Height());
		}
	}


	dc.Detach();
}
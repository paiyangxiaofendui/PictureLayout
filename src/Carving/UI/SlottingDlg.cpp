// SlottingDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "SlottingDlg.h"
#include "afxdialogex.h"
//#include "hgCst/Construction.h"
//#include "hg3d/Utils.h"
//#include "hg3d/XmlFile.h"
//#include "OtherShape.h"
//#include "OtherShapeHelper.h"
//#include "YHInfoTools.h"
#include "../Misc/Misc.h"
#include "../Drawing/Drawing.h"
#include "../Misc/HGTools.h"
#include "../Misc/OtherShapeHelper.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"

// CSlottingDlg dialog

IMPLEMENT_DYNAMIC(CSlottingDlg, CDialogEx)

CSlottingDlg::CSlottingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSlottingDlg::IDD, pParent)
{

	m_editd = _T("");
	//  m_editdir = _T("");
	m_edith = _T("");
	m_editSlotLength = _T("");
	m_edity = _T("");
	m_editx = _T("");
}

CSlottingDlg::~CSlottingDlg()
{
}

void CSlottingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITD, m_editd);
	DDX_Text(pDX, IDC_EDITH, m_edith);
	DDX_Text(pDX, IDC_EDITW, m_editSlotLength);
	DDX_Text(pDX, IDC_EDITY, m_edity);
	DDX_Text(pDX, IDC_EDITX, m_editx);
	DDX_Control(pDX, IDC_STATICSHOW, m_showpanel);
	DDX_Control(pDX, IDC_COMDIR, m_comdir);
	DDX_Control(pDX, IDC_SOLTLIST, m_soltlist);
}


BEGIN_MESSAGE_MAP(CSlottingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSlottingDlg::OnBnClickedOk)						//确定 保存
	ON_BN_CLICKED(IDC_BUTTON1, &CSlottingDlg::OnBnClickedButton1)			//添加
	ON_NOTIFY(NM_DBLCLK, IDC_SOLTLIST, &CSlottingDlg::OnNMDblclkSoltlist)	//双击删除
	ON_NOTIFY(NM_RCLICK, IDC_SOLTLIST, &CSlottingDlg::OnNMRClickSoltlist)	//右键删除
	ON_COMMAND(ID_MENU_REMOVING_SLOT, &CSlottingDlg::OnRemoveSlot)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSlottingDlg message handlers


void CSlottingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString strTmp;
	for (int i = 0;i < m_soltlist.GetItemCount();i++)
	{
		char c_solt[500];
		m_soltlist.GetItemText(i,0,c_solt,400);
		strTmp = strTmp + c_solt;
	}
	vector<AllSlot> vSlots;
	YHInfoTools::getAllSlot(vSlots, strTmp, "", "");
	for(int i = 0; i < vSlots.size(); i++)
	{
		vSlots[i]._y = m_fComponentYLen - vSlots[i]._y;
		char one_saw_value[100];
		sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;", \
			vSlots[i]._x, vSlots[i]._y, vSlots[i]._width, vSlots[i]._height, vSlots[i]._depth, vSlots[i]._dir);
		m_result += one_saw_value;
	}

	CString Slotx,Sloty,SlotWidth,SlotDepth;
	GetDlgItem(IDC_EDITX)->GetWindowText(Slotx);
	GetDlgItem(IDC_EDITY)->GetWindowText(Sloty);
	GetDlgItem(IDC_EDITH)->GetWindowText(SlotWidth);
	GetDlgItem(IDC_EDITD)->GetWindowText(SlotDepth);
	WriteSlotTemplate(Slotx,Sloty,SlotWidth,SlotDepth);
	CDialogEx::OnOK();
}
void CSlottingDlg::WriteSlotTemplate(CString Slotx,CString Sloty,CString SlotWidth,CString SlotDepth )
{
	//std::string tempPath = _T("kdata");
	//tempPath = hg3d::getFullFileName(tempPath);
	//tempPath = tempPath + "\\SlotTemplate.xml";
	CString strTmp;
	strTmp.Format(_T("kdata\\SlotTemplate%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	// 解密文件
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

	//hg3d::CXmlFile* theFile = new hg3d::CXmlFile();
	TinyXml::TiXmlDocument doc(xmlPath);
	//if (!theFile->Load(tempPath.c_str()))
	bool bLoadSuccess = doc.LoadFile();
	HGTools::deleteFile(xmlPath);
	if(!bLoadSuccess)
	{
		AfxMessageBox("写入SlotTemplate.xml失败",MB_OK);
		return;
	}

	XmlHandler xmlHandler;
	vector<TiXmlElement*> vSlotMessage = xmlHandler.GetChildElms(doc.RootElement());
	//for(int i = 0;i<theFile->GetChildNodes().size();i++)
	for(int i = 0;i<vSlotMessage.size();i++)
	{
		//hg3d::CXmlNode* theNode = theFile->GetChildNodes().at(i);
		TiXmlElement* elmSlotMessage = vSlotMessage[i];
		CString strTheName;
		xmlHandler.GetXmlAttribute(elmSlotMessage, "name", strTheName);
		//std::string theName = theNode->GetAttributeValue("name");
		//if (theName=="Slotx")
		if (strTheName.CompareNoCase("Slotx") == 0)
		{
			//theNode->SetAttributeValue("value",Slotx);
			elmSlotMessage->SetAttribute("value", Slotx);
		}
		//else if (theName=="Sloty")
		if (strTheName.CompareNoCase("Sloty") == 0)
		{
			//theNode->SetAttributeValue("value",Sloty);
			elmSlotMessage->SetAttribute("value", Sloty);
		}
		//else if (theName=="SlotWidth")
		if (strTheName.CompareNoCase("SlotWidth") == 0)
		{
			//theNode->SetAttributeValue("value",SlotWidth);
			//xmlHandler.GetXmlAttribute(elmSlotMessage, "value", SlotWidth);
			elmSlotMessage->SetAttribute("value", SlotWidth);
		}
		//else if (theName=="SlotDepth")
		if (strTheName.CompareNoCase("SlotDepth") == 0)
		{
			//theNode->SetAttributeValue("value",SlotDepth);
			//xmlHandler.GetXmlAttribute(elmSlotMessage, "value", SlotDepth);
			elmSlotMessage->SetAttribute("value", SlotDepth);
		}
	}
	//theFile->Save(tempPath.c_str());
	//保存
	doc.SaveFile(xmlPath);
	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
	//关闭文件
	HGTools::deleteFile(xmlPath);
}
void CSlottingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	drawpanel();
	// Do not call CDialogEx::OnPaint() for painting messages
}


std::string CSlottingDlg::replace_allstr(std::string str,const std::string old_value,const std::string new_value)
{
	while(true)   
	{                                                                                                                                                              
		std::string::size_type pos(0);     
		if((pos=str.find(old_value))!=string::npos)
			str.replace(pos,old_value.length(),new_value);
		else
			break;
	}
	return str;  
}

void CSlottingDlg::drawpanel()
{
	UpdateData(TRUE);

	HDC hdc = m_showpanel.GetDC()->GetSafeHdc();
	CRect rcDrawArea, rcComponentDrawArea;
	m_showpanel.GetWindowRect(rcDrawArea);
	m_showpanel.ScreenToClient(rcDrawArea);
	rcComponentDrawArea = rcDrawArea;
	rcComponentDrawArea.DeflateRect(15, 0, 0, 15);
	CDC* pDC=CDC::FromHandle(hdc);

	CBrush   *brush=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,255))); 
	pDC->SelectObject(brush);  
	pDC->BeginPath();
	pDC->MoveTo(3,3);
	pDC->LineTo(3,210);
	pDC->LineTo(210,210);
	pDC->LineTo(210,3);
	pDC->EndPath();
	pDC->FillPath();


	//for (int i = 0;i < m_showps.size();i++)
	//{
	//	float p_x1 = m_showps.at(i)._x;
	//	float p_y1 = m_showps.at(i)._y;
	//	float p_x2 = m_showps.at((i+1)%m_showps.size())._x;
	//	float p_y2 = m_showps.at((i+1)%m_showps.size())._y;
	//	pDC->MoveTo(p_x1*_scale+5,p_y1*_scale+5);
	//	pDC->LineTo(p_x2*_scale+5,p_y2*_scale+5);
	//}
	vector<PointInfo> vOutlinePoint = OtherShapeHelper::GetPointInfoFromPointInfoString(m_strOutlinePointString.GetBuffer());

	//CRect rcDrawArea;
	//GetDlgItem(IDC_OTHER_SHAPE_IMG)->GetWindowRect(rcDrawArea);
	//ScreenToClient(rcDrawArea);

	int nComponentXLen = m_fComponentXLen;
	int nComponentYLen = m_fComponentYLen;
	//判断是否合法outline点阵


	Graphics g(pDC->GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeHighQuality);

	float fScale = GetScale(nComponentXLen, nComponentYLen, rcComponentDrawArea.Width()-10, rcComponentDrawArea.Height()-10);

	CRect rcPanelArea(10/2, 10/2, rcComponentDrawArea.Width()-10/2, rcComponentDrawArea.Height()-10/2);

	CRect rcPanel((rcComponentDrawArea.Width()-nComponentXLen*fScale)/2, \
		(rcComponentDrawArea.Height()-nComponentYLen*fScale)/2, \
		(rcComponentDrawArea.Width()-nComponentXLen*fScale)/2 + nComponentXLen*fScale, \
		(rcComponentDrawArea.Height()-nComponentYLen*fScale)/2 + nComponentYLen*fScale);

	rcPanel.OffsetRect(rcComponentDrawArea.left, rcComponentDrawArea.top);

	g.FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	//vector<PointInfo>& vOutlinePoint = vOutlinePoint;
	if(vOutlinePoint.size() > 0)
	{
		for(int j = 1; j < vOutlinePoint.size(); j++)
		{
			PointInfo& vertexStart = vOutlinePoint[j - 1];
			PointInfo& vertexEnd = vOutlinePoint[j];

			DrawOneLine(g, rcPanel, fScale, vertexStart, vertexEnd);
		}
	}
	else
	{
		g.DrawRectangle(&Pen(Color::Black, 1), rcPanel.left, rcPanel.top, rcPanel.Width(), rcPanel.Height());
	}


	//this->ReleaseDC(pDC);


	//for (int i = 0;i < m_soltlist.GetItemCount();i++)
	//{
	//	std::string str_x;
	//	std::string str_y;
	//	std::string str_w;
	//	std::string str_h;
	//	std::string str_d;
	//	std::string str_dir;

	//	char c_solt[500];
	//	m_soltlist.GetItemText(i,0,c_solt,400);

	//	std::string str_slotinfo = c_solt;

	//	int str_num = 0;
	//	std::string str_value = "";
	//	AllSlot oneslot;
	//	for (int i = 0;i < str_slotinfo.size();i++)
	//	{
	//		char ch = str_slotinfo.at(i);
	//		if (ch == ',')
	//		{
	//			switch(str_num)
	//			{
	//			case 0:
	//				str_x = str_value;
	//				break;
	//			case 1:
	//				str_y = str_value;
	//				break;
	//			case 2:
	//				str_w = str_value;
	//				break;
	//			case 3:
	//				str_h = str_value;
	//				break;
	//			case 4:
	//				str_d = str_value;
	//				break;
	//			case 5:
	//				str_dir = str_value;
	//				break;
	//			default:
	//				break;
	//			}
	//			str_value = "";
	//			str_num++;
	//		}
	//		else if (ch == ';')
	//		{
	//			str_dir = str_value;
	//			str_num = 0;
	//		}
	//		else
	//		{
	//			str_value = str_value + ch;
	//		}
	//	}

	//	if (str_x != "" && str_y != "" && str_w != "" && str_h != "" && str_d != "" && str_dir != "")
	//	{
	//		float p_x = atof(str_x.c_str());
	//		float p_y = atof(str_y.c_str());
	//		float p_w = atof(str_w.c_str());
	//		float p_h = atof(str_h.c_str());
	//		float p_d = atof(str_d.c_str());

	//		int pen_width;
	//		if (p_h*_scale < 1.0)
	//		{
	//			pen_width = 1;
	//		}
	//		else
	//		{
	//			pen_width = (int)(p_h*_scale);
	//		}
	//		CPen pen(PS_SOLID,1,RGB(0,0,0));
	//		CPen pen_1(PS_SOLID,pen_width,RGB(0,0,0));

	//		pDC->SelectObject(&pen_1);
	//		if (str_dir == "0")
	//		{
	//			pDC->MoveTo((int)(p_x*_scale)+5,(int)((p_y - p_w/2.0)*_scale)+5);
	//			pDC->LineTo((int)(p_x*_scale)+5,(int)((p_y + p_w/2.0)*_scale)+5);
	//		}
	//		else if (str_dir == "1")
	//		{
	//			pDC->MoveTo((int)((p_x - p_w/2.0)*_scale)+5,(int)(p_y*_scale)+5);
	//			pDC->LineTo((int)((p_x + p_w/2.0)*_scale)+5,(int)(p_y*_scale)+5);
	//		}
	//		pDC->SelectObject(&pen);
	//	}

	//}
	
	//for(vector<AllSlot>::iterator itr = pComponent->m_vUpperFaceSlot.begin(); itr != pComponent->m_vUpperFaceSlot.end(); itr++)
	vector<AllSlot> vSlot = GetSlotByListCtrl();
	for (vector<AllSlot>::iterator itr = vSlot.begin(); itr != vSlot.end(); itr++)
	{
		//float fSlotX, fSlotY, fSlotLen, fSlotDia, fSlotAngle;
		CRect rcSlot;
		rcSlot.left = (itr->_x - (itr->_width / 2)) * fScale;
		rcSlot.top = (itr->_y - (itr->_height / 2)) * fScale;
		rcSlot.right = (itr->_x + (itr->_width / 2)) * fScale;
		rcSlot.bottom = (itr->_y + (itr->_height / 2)) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->m_angle*/itr->_dir == 0 ? 90 : 0);
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(&SolidBrush(Color::Blue), rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

	AdjustableArrowCap lineCap(4, 5, true);
	Pen penArrow(Color::Black, 1);
	penArrow.SetCustomEndCap(&lineCap);
	g.DrawLine(&penArrow, rcDrawArea.left + 15, rcDrawArea.top + 15, rcDrawArea.left + 15, rcDrawArea.bottom);
	g.DrawLine(&penArrow, rcDrawArea.left + 15, rcDrawArea.top + 15, rcDrawArea.right, rcDrawArea.top + 15);

	Gdiplus::Font font10(L"Segoe UI", 11, FontStyleRegular, UnitPixel);
	StringFormat sf;
	SolidBrush brushBlack(Color::Black);
	g.DrawString(L"Y", -1, &font10, PointF(rcDrawArea.left, rcDrawArea.bottom-15), &sf, &brushBlack);
	g.DrawString(L"X", -1, &font10, PointF(rcDrawArea.right-15, rcDrawArea.top), &sf, &brushBlack);

	CString strComponentSize;
	sf.SetAlignment(StringAlignmentCenter);
	RectF rfSize(rcDrawArea.left, rcDrawArea.bottom-30, rcDrawArea.Width(), 15);
	strComponentSize.Format(_T("%s * %s"), GetFloatString(m_fComponentXLen, 1), GetFloatString(m_fComponentYLen, 1));
	g.DrawString(AnsiToUnicode(strComponentSize).c_str(), -1, &font10, rfSize, &sf, &brushBlack);
}

BOOL CSlottingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//添加数据
	m_comdir.AddString("宽度方向");
	m_comdir.AddString("长度方向");
	if (m_editdir=="0")
	{
		m_comdir.SetCurSel(1);
	}
	else
		m_comdir.SetCurSel(0);
	m_soltlist.InsertColumn(0,_T("开槽信息"),0,140);

	if(m_editdir =="0")
	{
		m_editSlotLength = GetFloatString(m_fComponentXLen, 1);
		m_editx = GetFloatString(m_fComponentXLen/2, 1);
	}
	else
	{
		m_editSlotLength = GetFloatString(m_fComponentYLen, 1);
		m_edity = GetFloatString(m_fComponentYLen/2, 1);
	}
	UpdateData(FALSE);
	//std::string result_value = m_strpoint.GetBuffer();

	//vector<param_point> pa_p;
	//std::string str_info = result_value;
	//std::string str = "";
	//std::string str_name;
	//std::string str_value;
	//for (int i = 0;i < str_info.size();i++)
	//{
	//	char ch = str_info.at(i);
	//	if(ch == ':')
	//	{
	//		str_name = str;
	//		str = "";
	//	}
	//	else if (ch == ';')
	//	{
	//		str_value = str;
	//		str = "";
	//		param_point p;
	//		p._name = str_name;
	//		p._value = str_value;
	//		if (str_name == "" || str_value == "")
	//		{
	//			return false;
	//		}
	//		pa_p.push_back(p);
	//	}
	//	else
	//	{
	//		str = str + ch;
	//	}
	//}

	//std::string m_stypeid,m_panelwidth,m_panelheight;

	//for (int i = 0;i < pa_p.size();i++)
	//{
	//	if (pa_p.at(i)._name == "ID")
	//	{
	//		m_stypeid = pa_p.at(i)._value;
	//	}
	//	else if (pa_p.at(i)._name == "PW")
	//	{
	//		m_panelwidth = pa_p.at(i)._value;
	//	}
	//	else if (pa_p.at(i)._name == "PD")
	//	{
	//		m_panelheight = pa_p.at(i)._value;
	//	}
	//}

	//for (int i = 1;i < pa_p.size();i++)
	//{
	//	std::string pa_name1 = pa_p.at(i)._name;
	//	for (int j = 0;j < i;j++)
	//	{
	//		std::string pa_name = pa_p.at(j)._name;
	//		if (pa_name.length() < pa_name1.length())
	//		{
	//			std::swap(pa_p[i],pa_p[j]);
	//		}
	//	}
	//}

	//OtherShapeHelper::sortParam_pointByStrLen(pa_p);

	//vector<point_vector> m_allpoint;
	//m_allpoint.clear();

	//hg3d::TString strConfigFile1 = _T("kdata");
	//strConfigFile1 = hg3d::getFullFileName(strConfigFile1);
	//strConfigFile1 += _T("\\PanelOutlineList.xml");

	//hg3d::CXmlFile* doc1 = new hg3d::CXmlFile();
	//if (!doc1->Load(strConfigFile1.c_str()))
	//{
	//	AfxMessageBox("读取PanelOutlineList.xml失败",MB_OK);
	//	return false;
	//}

	//for (int i = 0;i < doc1->GetChildNodes().size();i++)
	//{
	//	hg3d::CXmlNode* child_node = doc1->GetChildNodes().at(i);
	//	std::string str_styleid = child_node->GetAttributeValue("StyleId");
	//	if (strcmp(str_styleid.c_str(),m_stypeid.c_str()) == 0)
	//	{
	//		for (int j = 0;j < child_node->GetChildNodes().size();j++)
	//		{
	//			point_vector pos_i;

	//			std::string str_x = child_node->GetChildNodes().at(j)->GetAttributeValue("X");
	//			std::string str_y = child_node->GetChildNodes().at(j)->GetAttributeValue("Y");

	//			pos_i._x = str_x;
	//			pos_i._y = str_y;
	//			m_allpoint.push_back(pos_i);

	//		}
	//	}
	//}



	//_scale = 200.0/atof(m_panelwidth.c_str()) < 200.0/atof(m_panelheight.c_str()) ? 200.0/atof(m_panelwidth.c_str()) : 200.0/atof(m_panelheight.c_str());

	//for (int i = 0;i < m_allpoint.size();i++)
	//{
	//	p_show p_s;
	//	std::string s_x = m_allpoint.at(i)._x;
	//	std::string s_y = m_allpoint.at(i)._y;

	//	for (int k = 0;k < pa_p.size();k++)
	//	{
	//		std::string p_name = pa_p.at(k)._name;
	//		std::string p_value = pa_p.at(k)._value;

	//		s_x = replace_allstr(s_x,p_name,p_value);
	//		s_y = replace_allstr(s_y,p_name,p_value);
	//	}

	//	hg3d::Construction cst;

	//	p_s._x = cst.calcFormula(s_x);
	//	p_s._y = -1*cst.calcFormula(s_y);
	//	m_showps.push_back(p_s);
	//}
	vector<AllSlot> vPreExistSlots;
	YHInfoTools::getAllSlot(vPreExistSlots, m_strPreExistSlots, "", "");
	for(int i = 0; i < vPreExistSlots.size(); i++)
	{
		char one_saw_value[100];
		sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;", \
			vPreExistSlots[i]._x, m_fComponentYLen - vPreExistSlots[i]._y,  vPreExistSlots[i]._width,  vPreExistSlots[i]._height,  vPreExistSlots[i]._depth,  vPreExistSlots[i]._dir);
		m_soltlist.InsertItem(m_soltlist.GetItemCount(), one_saw_value);
	}

	drawpanel();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSlottingDlg::OnBnClickedButton1()
{
	
	UpdateData(TRUE);
	if (m_comdir.GetCurSel() == 0)
	{
		m_editdir = "0";
	}
	else
	{
		m_editdir = "1";
	}
	CString insert_result;
	insert_result = m_editx + "," + m_edity + "," + m_editSlotLength + "," + m_edith + "," + m_editd + "," + m_editdir + ";";

	m_soltlist.InsertItem(0,insert_result);

	drawpanel();
}


void CSlottingDlg::OnNMDblclkSoltlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nItem = m_soltlist.GetNextItem(-1,LVIS_SELECTED);

	if (nItem < 0)
	{
		return;
	}
	else
	{
		m_soltlist.DeleteItem(nItem);
	}
	*pResult = 0;
}


void CSlottingDlg::OnNMRClickSoltlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CMenu menu, *pPopup;  
	menu.LoadMenu(IDR_MENU_ADD_OR_REMOVE_SLOT);  
	pPopup = menu.GetSubMenu(0);  
	CPoint myPoint;  
	ClientToScreen(&myPoint);  
	GetCursorPos(&myPoint); 
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this); 
	*pResult = 0;
}


void CSlottingDlg::OnRemoveSlot()//删除选中的开槽设置
{
	
	int nItem = m_soltlist.GetNextItem(-1,LVIS_SELECTED);
	if (nItem < 0)
	{
		return;
	}
	else
	{
		m_soltlist.DeleteItem(nItem);
		m_showpanel.Invalidate();
		Invalidate();
	}
}

vector<AllSlot> CSlottingDlg::GetSlotByListCtrl()
{
	vector<AllSlot> vSlot;

	for (int i = 0;i < m_soltlist.GetItemCount();i++)
	{
		std::string str_x;
		std::string str_y;
		std::string str_w;
		std::string str_h;
		std::string str_d;
		std::string str_dir;

		char c_solt[500];
		m_soltlist.GetItemText(i,0,c_solt,400);

		std::string str_slotinfo = c_solt;

		int str_num = 0;
		std::string str_value = "";
		AllSlot oneslot;
		for (int i = 0;i < str_slotinfo.size();i++)
		{
			char ch = str_slotinfo.at(i);
			if (ch == ',')
			{
				switch(str_num)
				{
				case 0:
					str_x = str_value;
					break;
				case 1:
					str_y = str_value;
					break;
				case 2:
					str_w = str_value;
					break;
				case 3:
					str_h = str_value;
					break;
				case 4:
					str_d = str_value;
					break;
				case 5:
					str_dir = str_value;
					break;
				default:
					break;
				}
				str_value = "";
				str_num++;
			}
			else if (ch == ';')
			{
				str_dir = str_value;
				str_num = 0;
			}
			else
			{
				str_value = str_value + ch;
			}
		}

		if (str_x != "" && str_y != "" && str_w != "" && str_h != "" && str_d != "" && str_dir != "")
		{
			oneslot._x = atof(str_x.c_str());
			oneslot._y = atof(str_y.c_str());
			oneslot._width = atof(str_w.c_str());
			oneslot._height = atof(str_h.c_str());
			oneslot._depth = atof(str_d.c_str());
			oneslot._dir = atoi(str_dir.c_str());
			vSlot.push_back(oneslot);
		}
	}

	return vSlot;
}
// PrintSetDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "PrintSetDlg.h"
#include "afxdialogex.h"
#include "PrintCodeDlg.h"
//#include "hg3d/XmlFile.h"
//#include "hg3d/Utils.h"
//#include "hg3d/hgtypes.h"
//#include "hg3d/XmlNode.h"
#include "../../../include/QRCode/qrcode/qrencode.h"
#include "PrintUserSetDlg.h"
#include "../Misc/Misc.h"
#include "../Misc/HGTools.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Misc/ProgramMisc.h"
#include "../Work/WorkDef.h"

// CPrintSetDlg dialog

IMPLEMENT_DYNAMIC(CPrintSetDlg, CDialogEx)

CPrintSetDlg::CPrintSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrintSetDlg::IDD, pParent)
{

	//  m_typecom = _T("");
}

CPrintSetDlg::~CPrintSetDlg()
{
}

void CPrintSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_MODULESHOW, m_moduleshow);
	DDX_Control(pDX, IDC_MODULESHOW, m_moduleshow);
	DDX_Control(pDX, IDC_COMBO1, m_modulesel);
	DDX_Control(pDX, IDC_CHECK1, m_showsole);
	//  DDX_Control(pDX, IDC_COMBO2, m_comtype);
	//  DDX_CBString(pDX, IDC_COMBO2, m_typecom);
}


BEGIN_MESSAGE_MAP(CPrintSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPrintSetDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPrintSetDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrintSetDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CPrintSetDlg::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CPrintSetDlg::OnCbnSelchangeCombo2)
//	ON_CBN_SELENDCANCEL(IDC_COMBO2, &CPrintSetDlg::OnCbnSelendcancelCombo2)
ON_BN_CLICKED(IDC_BUTTON2, &CPrintSetDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPrintSetDlg message handlers


void CPrintSetDlg::OnBnClickedOk()
{
	
	int com_index = m_modulesel.GetCurSel();

	//hg3d::TString strConfigFile = _T("picture");
	//strConfigFile = hg3d::getFullFileName(strConfigFile);
	//strConfigFile += _T("\\printcode.xml");

	CString strTmp;
	strTmp.Format(_T("picture\\printcode%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;
	XmlHandler xmlHandler;
	if(!xmlHandler.LoadFile(strHGE))
	{
		AfxMessageBox("读取printcode.xml失败",MB_OK);
		return;
	}


	//hg3d::CXmlFile* doc = new hg3d::CXmlFile();
	//if (!doc->Load(strConfigFile.c_str()))
	//{
	//	AfxMessageBox("读取printcode.xml失败",MB_OK);
	//	return;
	//}
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	vector<TiXmlElement*> vCodeModule = xmlHandler.GetChildElms(elmRoot);

	//for (int i = 0;i < doc->GetChildNodes().size();i++)
	for (int i = 0;i < vCodeModule.size();i++)
	{
		//hg3d::CXmlNode* child_node = doc->GetChildNodes().at(i);
		TiXmlElement* elmCodeModule = vCodeModule[i];
		//if (child_node != NULL)
		{
			int nTmp;
			xmlHandler.GetXmlAttribute(elmCodeModule, "Index", nTmp);
			//if (atoi(child_node->GetAttributeValue("Index").c_str()) == com_index)
			if (nTmp == com_index)
			{
				//child_node->SetAttributeValue("Used","1");
				elmCodeModule->SetAttribute("Used","1");
			}
				/*hg3d::CXmlNode* child_node1 = child_node->GetChildByAttribute("Name","加盟店");
				hg3d::CXmlNode* child_node2 = child_node->GetChildByAttribute("Name","Logo");
				hg3d::CXmlNode* child_node3 = child_node->GetChildByAttribute("Name","产品名称");
				hg3d::CXmlNode* child_node4 = child_node->GetChildByAttribute("Name","客户信息");
				hg3d::CXmlNode* child_node5 = child_node->GetChildByAttribute("Name","开槽");

				if (((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel() == 1)
				{
					if (child_node1 != NULL)
						child_node1->SetAttributeValue("Used","1");
					if (child_node2 != NULL)
						child_node2->SetAttributeValue("Used","0");
					if (child_node3 != NULL)
						child_node3->SetAttributeValue("Used","0");
					if (child_node4 != NULL)
						child_node4->SetAttributeValue("Used","0");
				}
				if (((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel() == 2)
				{
					if (child_node1 != NULL)
						child_node1->SetAttributeValue("Used","0");
					if (child_node2 != NULL)
						child_node2->SetAttributeValue("Used","0");
					if (child_node3 != NULL)
						child_node3->SetAttributeValue("Used","1");
					if (child_node4 != NULL)
						child_node4->SetAttributeValue("Used","0");
				}
				if (((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel() == 3)
				{
					if (child_node1 != NULL)
						child_node1->SetAttributeValue("Used","0");
					if (child_node2 != NULL)
						child_node2->SetAttributeValue("Used","0");
					if (child_node3 != NULL)
						child_node3->SetAttributeValue("Used","0");
					if (child_node4 != NULL)
						child_node4->SetAttributeValue("Used","1");
				}
				if (((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel() == 4)
				{
					if (child_node1 != NULL)
						child_node1->SetAttributeValue("Used","0");
					if (child_node2 != NULL)
						child_node2->SetAttributeValue("Used","1");
					if (child_node3 != NULL)
						child_node3->SetAttributeValue("Used","0");
					if (child_node4 != NULL)
						child_node4->SetAttributeValue("Used","0");
				}
				if (m_showsole.GetCheck() == true && child_node5 != NULL)
				{
					child_node5->SetAttributeValue("Used","1");
				}
				if (m_showsole.GetCheck() != true && child_node5 != NULL)
				{
					child_node5->SetAttributeValue("Used","0");
				}

			}*/
			else
			{
				//child_node->SetAttributeValue("Used","0");
				elmCodeModule->SetAttribute("Used","0");
			}
		}
	}
	//doc->Save(strConfigFile.c_str());

	CString strXML = HGTools::getXmlPathFromHgxPath(strHGE);
	xmlHandler.GetDoc()->SaveFile(strXML);
	encrypt_base64(strXML.GetBuffer(), strHGE.GetBuffer());
	HGTools::deleteFile(strXML);

	CDialogEx::OnOK();
}


BOOL CPrintSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//hg3d::CXmlFile* doc = new hg3d::CXmlFile();
	//if (!doc->Load(strConfigFile.c_str()))
	//{
	//	AfxMessageBox("读取printcode.xml失败",MB_OK);
	//	return false;
	//}
	CString strTmp;
	strTmp.Format(_T("picture\\printcode%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;
	XmlHandler xmlHandler;
	if(!xmlHandler.LoadFile(strHGE))
	{
		AfxMessageBox("读取printcode.xml失败",MB_OK);
		return false;
	}

	((CComboBox * )GetDlgItem(IDC_COMBO2))->InsertString(0,"请选择");
	((CComboBox * )GetDlgItem(IDC_COMBO2))->InsertString(1,"加盟店");
	((CComboBox * )GetDlgItem(IDC_COMBO2))->InsertString(2,"产品名称");
	((CComboBox * )GetDlgItem(IDC_COMBO2))->InsertString(3,"客户信息");
	((CComboBox * )GetDlgItem(IDC_COMBO2))->InsertString(4,"Logo");

	((CComboBox * )GetDlgItem(IDC_COMBO2))->SetCurSel(0);

	std::string user_index = "0";

	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	vector<TiXmlElement*> vCodeModule = xmlHandler.GetChildElms(elmRoot);
	//for (int i = 0;i < doc->GetChildNodes().size();i++)
	for (int i = 0;i < vCodeModule.size();i++)
	{
		CString str_index;
		str_index.Format("%d",i);
		//hg3d::CXmlNode* child_node = doc->GetChildByAttribute("Index",str_index.GetBuffer());
		TiXmlElement* elmCodeModule = xmlHandler.GetIndicatedElement(elmRoot, "CodeModule", "Index", str_index.GetBuffer());
		str_index.ReleaseBuffer();
		//if (child_node != NULL)
		if (elmCodeModule != NULL)
		{
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmCodeModule, "Name", strTmp);
			//m_modulesel.InsertString(i,(LPCTSTR)child_node->GetAttributeValue("Name").c_str());
			m_modulesel.InsertString(i,strTmp);
			int nTmp;
			xmlHandler.GetXmlAttribute(elmCodeModule, "Used", nTmp);
			//if (child_node->GetAttributeValue("Used") == "1")
			if (nTmp == 1)
			{
				//user_index = child_node->GetAttributeValue("Index");
				xmlHandler.GetXmlAttribute(elmCodeModule, "Index", strTmp);
				user_index = strTmp;
			}
			//if (child_node->GetAttributeValue("Used") == "1")
			if (nTmp == 1)
			{
				//hg3d::CXmlNode* child_node1 = child_node->GetChildByAttribute("Name","加盟店");
				//hg3d::CXmlNode* child_node2 = child_node->GetChildByAttribute("Name","产品名称");
				//hg3d::CXmlNode* child_node3 = child_node->GetChildByAttribute("Name","客户信息");
				//hg3d::CXmlNode* child_node4 = child_node->GetChildByAttribute("Name","Logo");
				//hg3d::CXmlNode* child_node5 = child_node->GetChildByAttribute("Name","开槽");


				//if (child_node1 != NULL && child_node1->GetAttributeValue("Used") == "1")
				int nTmp = 0;
				TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(elmCodeModule, "Parameter", "Name", "加盟店");
				xmlHandler.GetXmlAttribute(elmTmp, "Used", nTmp);
				if(elmTmp != NULL && nTmp == 1)
					((CComboBox * )GetDlgItem(IDC_COMBO2))->SetCurSel(1);

				//if (child_node2 != NULL && child_node2->GetAttributeValue("Used") == "1")
				nTmp = 0;
				elmTmp = xmlHandler.GetIndicatedElement(elmCodeModule, "Parameter", "Name", "产品名称");
				xmlHandler.GetXmlAttribute(elmTmp, "Used", nTmp);
				if(elmTmp != NULL && nTmp == 1)
					((CComboBox * )GetDlgItem(IDC_COMBO2))->SetCurSel(2);

				//if (child_node3 != NULL && child_node3->GetAttributeValue("Used") == "1")
				nTmp = 0;
				elmTmp = xmlHandler.GetIndicatedElement(elmCodeModule, "Parameter", "Name", "客户信息");
				xmlHandler.GetXmlAttribute(elmTmp, "Used", nTmp);
				if(elmTmp != NULL && nTmp == 1)
					((CComboBox * )GetDlgItem(IDC_COMBO2))->SetCurSel(3);

				//if (child_node4 != NULL && child_node4->GetAttributeValue("Used") == "1")
				nTmp = 0;
				elmTmp = xmlHandler.GetIndicatedElement(elmCodeModule, "Parameter", "Name", "Logo");
				xmlHandler.GetXmlAttribute(elmTmp, "Used", nTmp);
				if(elmTmp != NULL && nTmp == 1)
					((CComboBox * )GetDlgItem(IDC_COMBO2))->SetCurSel(4);

				//if (child_node5 != NULL && child_node5->GetAttributeValue("Used") == "1")
				nTmp = 0;
				elmTmp = xmlHandler.GetIndicatedElement(elmCodeModule, "Parameter", "Name", "开槽");
				xmlHandler.GetXmlAttribute(elmTmp, "Used", nTmp);
				if(elmTmp != NULL && nTmp == 1)
					m_showsole.SetCheck(1);
				else
					m_showsole.SetCheck(0);
					
			}
		}
	}

	m_modulesel.SetCurSel(atoi(user_index.c_str()));

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPrintSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//hg3d::TString strConfigFile = _T("picture");
	//strConfigFile = hg3d::getFullFileName(strConfigFile);
	//strConfigFile += _T("\\printcode.xml");

	//hg3d::CXmlFile* doc = new hg3d::CXmlFile();
	//if (!doc->Load(strConfigFile.c_str()))
	//{
	//	AfxMessageBox("读取printcode.xml失败",MB_OK);
	//	return;
	//}
	CString strTmp;
	strTmp.Format(_T("picture\\printcode%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;
	XmlHandler xmlHandler;
	if(!xmlHandler.LoadFile(strHGE))
	{
		AfxMessageBox("读取printcode.xml失败",MB_OK);
		return;
	}

	//hg3d::CXmlNode * print_node = NULL;
	TiXmlElement* elmPrintNode = NULL;
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	vector<TiXmlElement*> vCodeModule = xmlHandler.GetChildElms(elmRoot);

	//for (int i = 0;i < doc->GetChildNodes().size();i++)
	for (int i = 0;i < vCodeModule.size();i++)
	{
		//hg3d::CXmlNode* child_node = doc->GetChildNodes().at(i);
		TiXmlElement* elmCodeModule = vCodeModule[i];
		/*std::string _used = child_node->GetAttributeValue("Used");
		if (_used == "1")
		{
			print_node = child_node;
			m_modulesel.SetCurSel(i);
			break;
		}*/
		//std::string _index = child_node->GetAttributeValue("Index");
		int nIndex;
		xmlHandler.GetXmlAttribute(elmCodeModule, "Index", nIndex);
		//if (atoi(_index.c_str()) == m_modulesel.GetCurSel())
		if (nIndex == m_modulesel.GetCurSel())
		{
			//print_node = child_node;
			elmPrintNode = elmCodeModule;
			break;
		}
	}

	//if (print_node == NULL)
	if (elmPrintNode == NULL)
	{
		AfxMessageBox("未读取到可以使用的打印模板",MB_OK);
		return;
	}

	//std::string s_f = print_node->GetAttributeValue("pictureName");
	//CString strTmp;
	xmlHandler.GetXmlAttribute(elmPrintNode, "pictureName", strTmp);
	std::string s_f = strTmp;

	bool bOutputDoorLabel = false, bOutputSendingDate = false;
	TiXmlElement* elmIsDoorPanel = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "门板标记");
	if (elmIsDoorPanel != NULL && strcmp(elmIsDoorPanel->Attribute("Used"), "1") == 0)
		bOutputDoorLabel = true;
	TiXmlElement* elmSendingDate = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "发货日期");
	if (elmSendingDate != NULL && strcmp(elmSendingDate->Attribute("Used"), "1") == 0)
		bOutputSendingDate = true;

	CPen pen(PS_SOLID,1,RGB(0,0,0));
	CPen pen_1(PS_DASH,1,RGB(0,0,0));
	CPen pen_2(PS_SOLID,2,RGB(0,0,0));

	std::vector<recode_print> m_print_info;
	m_print_info.clear();
	std::vector<print_info> m_info;
	print_info one_info;

	one_info.panel_no = "88";
	one_info.com_no = "88";
	one_info.customer_info = "客户信息";
	one_info.bar_code = "160808A001";
	one_info.com_width = "502";
	one_info.com_height = "503";
	one_info.com_depth = "18";
	one_info.com_name = "活动层板";
	one_info.com_matrial = "颗粒板90度空间";
	one_info.cst_no = "88";
	one_info.cst_name = "产品名称";//柜体名称（产品名称）
	one_info.edge_info = "0.5 1.5 1.0 2.0";
	one_info.product_width = "500";
	one_info.product_height = "500";
	one_info.other_com = "异21"; //异型
	one_info.slot_com = "槽460.5"; //开槽
	one_info.punching_com = "钻"; //打孔
	one_info.double_com = "双"; //双面
	one_info.franchisee = "加盟店"; //加盟店
	one_info.customer_address = "广州中山一路";
	one_info.rebar_code = "160808B001";
	one_info.m_strSendingDate = "5/16";

	m_info.push_back(one_info);

	for (int i = 0;i < m_info.size();i++)
	{
		recode_print _r_print;
		_r_print.code_info = m_info.at(i).bar_code;

		CString orderNameString;
		orderNameString.Format("%06d-%d",_ttoi(m_info.at(i).panel_no),_ttoi(m_info.at(i).com_no));
		orderNameString = orderNameString.Right(8);
		_r_print.printtext["加工顺序"] = orderNameString.GetBuffer();

		//_r_print.printtext["加工顺序"] = m_info.at(i).panel_no + "-" + m_info.at(i).com_no;
		_r_print.printtext["客户信息"] = m_info.at(i).customer_info;
		_r_print.printtext["条码编号"] = m_info.at(i).bar_code;
		_r_print.printtext["板件尺寸"] = m_info.at(i).com_width + "*" + m_info.at(i).com_height + "*" + m_info.at(i).com_depth;
		_r_print.printtext["板件名称"] = m_info.at(i).com_name;
		_r_print.printtext["板件材料"] = m_info.at(i).com_matrial;
		_r_print.printtext["柜号"] = m_info.at(i).cst_no;
		_r_print.printtext["异型"] = m_info.at(i).other_com;
		_r_print.printtext["开槽"] = m_info.at(i).slot_com;
		_r_print.printtext["助记号"] = m_info.at(i).punching_com;
		_r_print.printtext["双面"] = m_info.at(i).double_com;
		_r_print.printtext["加盟店"] = m_info.at(i).franchisee;
		_r_print.printtext["产品名称"] = m_info.at(i).cst_name;
		_r_print.printtext["客户地址"] = m_info.at(i).customer_address;

		CString z_identify,c_identify,zc_identify;
		zc_identify = m_info.at(i).zc_identify;
		z_identify = zc_identify.Left(zc_identify.Find("#"));
		c_identify = zc_identify.Right(zc_identify.GetLength() - zc_identify.Find("#") - 1);
		_r_print.printtext["钻标识"] = z_identify;
		_r_print.printtext["槽标识"] = c_identify;
		if(bOutputDoorLabel)
			_r_print.printtext["门板标记"] = "[门]";
		if(bOutputSendingDate)
			_r_print.printtext["发货日期"] = m_info.at(i).m_strSendingDate;

		_r_print._picture.s_width = m_info.at(i).product_width.GetBuffer();
		_r_print._picture.s_height = m_info.at(i).product_height.GetBuffer();
		_r_print._picture._width = atoi(_r_print._picture.s_width.c_str());
		_r_print._picture._height = atoi(_r_print._picture.s_height.c_str());

		CString _def = m_info.at(i).edge_info;
		std::string str_def = _def.GetBuffer();
		std::vector<std::string> defs;
		std::string str_tmpdef;
		for (int i = 0;i < str_def.size();i++)
		{
			if (str_def.at(i) != ' ')
			{
				str_tmpdef.push_back(str_def.at(i));
			}
			else
			{
				defs.push_back(str_tmpdef);
				str_tmpdef.clear();
			}
			if (i == str_def.size() - 1)
			{
				defs.push_back(str_tmpdef);
			}
		}

		float f_def1 = atof(defs.at(0).c_str());
		float f_def2 = atof(defs.at(1).c_str());
		float f_def3 = atof(defs.at(2).c_str());
		float f_def4 = atof(defs.at(3).c_str());



		_r_print._picture.down_edge = f_def1;
		_r_print._picture.right_edge = f_def2;
		_r_print._picture.up_edge = f_def3;
		_r_print._picture.left_edge = f_def4;

		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_path = ExeFile;
		file_path = file_path.Left(file_path.ReverseFind('\\'));
		file_path = file_path + _T("\\picture\\logo.bmp");

		_r_print.logo_path = file_path;
		m_print_info.push_back(_r_print);
	}

	for (int i = 0;i < m_print_info.size();i++)
	{
		CDC* pDC=CDC::FromHandle(m_moduleshow.GetDC()->GetSafeHdc());

		CRect picture_rect;
		m_moduleshow.GetClientRect(&picture_rect);

		CBrush   *brush_picture1=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,255))); 
		CBrush   *brush_picture=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,255))); 
		pDC->SelectObject(brush_picture);  
		pDC->BeginPath();
		pDC->MoveTo(0,0);
		pDC->LineTo(0,picture_rect.Height());
		pDC->LineTo(picture_rect.Width(),picture_rect.Height());
		pDC->LineTo(picture_rect.Width(),0);
		pDC->EndPath();
		pDC->FillPath();
		pDC->SelectObject(brush_picture1);

		//int _paperWidth = atoi(print_node->GetAttributeValue("paperWidth").c_str());
		//int _paperHeight = atoi(print_node->GetAttributeValue("paperHeight").c_str());
		int _paperWidth;
		int _paperHeight;
		xmlHandler.GetXmlAttribute(elmPrintNode, "paperWidth", _paperWidth);
		xmlHandler.GetXmlAttribute(elmPrintNode, "paperHeight", _paperHeight);

		m_paperWidth = _paperWidth;
		m_paperHeight = _paperHeight;

		CRect rcClient;
		m_moduleshow.GetClientRect(rcClient);
		int src_x = (rcClient.Width() - _paperWidth)/2;
		int src_y = (rcClient.Height() - _paperHeight)/2;

		m_srcx = src_x;
		m_srcy = src_y;

		pDC->SelectObject(pen_1);
		pDC->Rectangle(src_x-2,src_y-2,_paperWidth+2+src_x,_paperHeight+2+src_y);
		pDC->SelectObject(pen);

		CFont oldfont;
		oldfont.CreateFont(15, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("宋体"));
		//打印logo
		//hg3d::CXmlNode * logo_node = print_node->GetChildByAttribute("Name","Logo");
		//if (logo_node != NULL && logo_node->GetAttributeValue("Used") == "1")
		int nUsed = 0;
		TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "Logo");
		xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
		if(elmTmp != NULL && nUsed == 1)
		{
			//std::string logo_x = logo_node->GetAttributeValue("X");
			//std::string logo_y = logo_node->GetAttributeValue("Y");
			//std::string logo_size = logo_node->GetAttributeValue("FontSize");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
			std::string logo_x = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
			std::string logo_y = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
			std::string logo_size = strTmp;

			std::string _logopath = m_print_info.at(i).logo_path;

			CImage image_logo;
			image_logo.Load(_logopath.c_str());

			CDC* pLogoDC = CDC::FromHandle(image_logo.GetDC());

			for (int L_x = 0;L_x < atoi(logo_size.c_str())*2;L_x++)
			{
				for (int L_y = 0;L_y < atoi(logo_size.c_str());L_y++)
				{
					pDC->SetPixel(L_x+atoi(logo_x.c_str())+src_x,L_y+atoi(logo_y.c_str())+src_y,pLogoDC->GetPixel(L_x,L_y));
				}
			}
			image_logo.ReleaseDC();
		}
		//打印一维码
		//hg3d::CXmlNode * onecode_node = print_node->GetChildByAttribute("Name","一维码");
		//if (onecode_node != NULL && onecode_node->GetAttributeValue("Used") == "1")
		nUsed = 0;
		elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "一维码");
		xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
		if(elmTmp != NULL && nUsed == 1)
		{
			//std::string code_x = onecode_node->GetAttributeValue("X");
			//std::string code_y = onecode_node->GetAttributeValue("Y");
			//std::string code_size = onecode_node->GetAttributeValue("FontSize");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
			std::string code_x = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
			std::string code_y = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
			std::string code_size = strTmp;

			//std::string str_bar_code = onecode_node->GetAttributeValue("Value");
			xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
			std::string str_bar_code = strTmp;

			MyBarcode(pDC,atoi(code_x.c_str())+src_x,atoi(code_y.c_str())+m_srcy,atoi(code_y.c_str())+m_srcy+20,39,atoi(code_size.c_str()),str_bar_code.c_str());
		}
		//打印反面一维码
		if (m_print_info.at(i).code_info != "")
		{
			//hg3d::CXmlNode * onecode_node2 = print_node->GetChildByAttribute("Name","一维码2");
			//if (onecode_node2 != NULL && onecode_node2->GetAttributeValue("Used") == "1")
			nUsed = 0;
			elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "一维码2");
			xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
			if(elmTmp != NULL && nUsed == 1)
			{
				//std::string code_x = onecode_node2->GetAttributeValue("X");
				//std::string code_y = onecode_node2->GetAttributeValue("Y");
				//std::string code_size = onecode_node2->GetAttributeValue("FontSize");
				//std::string str_bar_code = onecode_node2->GetAttributeValue("Value");
				CString strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
				std::string code_x = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
				std::string code_y = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
				std::string code_size = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
				std::string str_bar_code = strTmp;

				MyBarcode(pDC,atoi(code_x.c_str())+m_srcx,atoi(code_y.c_str())+m_srcy,atoi(code_y.c_str())+m_srcy+20,39,atoi(code_size.c_str()),str_bar_code.c_str());
			}
		}
		//打印二维码
		//hg3d::CXmlNode * ercode_node = print_node->GetChildByAttribute("Name","二维码");
		//if (ercode_node != NULL && ercode_node->GetAttributeValue("Used") == "1")
		nUsed = 0;
		elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "二维码");
		xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
		if(elmTmp != NULL && nUsed == 1)
		{
			//std::string code_x = ercode_node->GetAttributeValue("X");
			//std::string code_y = ercode_node->GetAttributeValue("Y");
			//std::string code_size = ercode_node->GetAttributeValue("FontSize");
			//std::string str_bar_code = ercode_node->GetAttributeValue("Value");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
			std::string code_x = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
			std::string code_y = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
			std::string code_size = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
			std::string str_bar_code = strTmp;


			char* szSourceSring = new char[str_bar_code.length()+1];
			for (int q = 0;q < str_bar_code.length();q++)
			{
				*(szSourceSring + q) = str_bar_code.at(q);
			}
			*(szSourceSring + str_bar_code.length()) = '\0';
			unsigned char*	pSourceData;
			QRcode*			pQRC;

			//int code_pixel = atoi(code_size.c_str());
			int code_pixel = 1;
			float code_plus = atof(code_size.c_str());

			if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
			{
				int unWidth = pQRC->width;

				int p_x = 0;
				int p_y = 0;

				pSourceData = pQRC->data;
				for (int c_i = 0;c_i < unWidth;c_i++)
				{
					for (int c_j = 0;c_j < unWidth;c_j++)
					{
						for (int c_k = 0;c_k < code_pixel;c_k++)
						{
							p_x = c_i * code_pixel + c_k;
							for(int c_m = 0;c_m < code_pixel;c_m++)
							{
								p_y = c_j * code_pixel + c_m;

								if (*pSourceData & 1)
								{
									pDC->SetPixel(p_y+atoi(code_x.c_str())+src_x,p_x+atoi(code_y.c_str())+src_y,RGB(0,0,0));
								}
							}

						}
						pSourceData++;
					}
				}
				pDC->StretchBlt(atoi(code_x.c_str())+src_x,atoi(code_y.c_str())+src_y,40*code_plus,40*code_plus,pDC,atoi(code_x.c_str())+src_x,atoi(code_y.c_str())+src_y,unWidth,unWidth,SRCCOPY);
				QRcode_free(pQRC);
			}
			if (szSourceSring != NULL)
			{
				delete szSourceSring;
				szSourceSring = NULL;
			}
		}
		//打印反面二维码
		//hg3d::CXmlNode * ercode_node2 = print_node->GetChildByAttribute("Name","二维码2");
		//if (ercode_node2 != NULL && ercode_node2->GetAttributeValue("Used") == "1")
		nUsed = 0;
		elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "二维码2");
		xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
		if(elmTmp != NULL && nUsed == 1)
		{
			//std::string code_x = ercode_node2->GetAttributeValue("X");
			//std::string code_y = ercode_node2->GetAttributeValue("Y");
			//std::string code_size = ercode_node2->GetAttributeValue("FontSize");
			//std::string str_bar_code = ercode_node2->GetAttributeValue("Value");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
			std::string code_x = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
			std::string code_y = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
			std::string code_size = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
			std::string str_bar_code = strTmp;

			char* szSourceSring = new char[str_bar_code.length()+1];
			for (int q = 0;q < str_bar_code.length();q++)
			{
				*(szSourceSring + q) = str_bar_code.at(q);
			}
			*(szSourceSring + str_bar_code.length()) = '\0';
			unsigned char*	pSourceData;
			QRcode*			pQRC;

			//int code_pixel = atoi(code_size.c_str());
			int code_pixel = 1;
			float code_plus = atof(code_size.c_str());

			if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
			{
				int unWidth = pQRC->width;

				int p_x = 0;
				int p_y = 0;

				pSourceData = pQRC->data;
				for (int c_i = 0;c_i < unWidth;c_i++)
				{
					for (int c_j = 0;c_j < unWidth;c_j++)
					{
						for (int c_k = 0;c_k < code_pixel;c_k++)
						{
							p_x = c_i * code_pixel + c_k;
							for(int c_m = 0;c_m < code_pixel;c_m++)
							{
								p_y = c_j * code_pixel + c_m;

								if (*pSourceData & 1)
								{
									pDC->SetPixel(p_y+atoi(code_x.c_str())+src_x,p_x+atoi(code_y.c_str())+src_y,RGB(0,0,0));
								}
							}

						}
						pSourceData++;
					}
				}
				pDC->StretchBlt(atoi(code_x.c_str())+src_x,atoi(code_y.c_str())+src_y,40*code_plus,40*code_plus,pDC,atoi(code_x.c_str())+src_x,atoi(code_y.c_str())+src_y,unWidth,unWidth,SRCCOPY);
				QRcode_free(pQRC);
			}
			if (szSourceSring != NULL)
			{
				delete szSourceSring;
				szSourceSring = NULL;
			}
		}
		//打印板件图片
		//hg3d::CXmlNode * p_picture_node = print_node->GetChildByAttribute("Name","板件图片");
		//if (p_picture_node != NULL && p_picture_node->GetAttributeValue("Used") == "1")
		nUsed = 0;
		elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "板件图片");
		xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
		if(elmTmp != NULL && nUsed == 1)
		{
			pDC->SelectObject(&pen_2);

			//std::string picture_x = p_picture_node->GetAttributeValue("X");
			//std::string picture_y = p_picture_node->GetAttributeValue("Y");
			//std::string picture_fontsize = p_picture_node->GetAttributeValue("FontSize");
			//std::string picture_fontname = p_picture_node->GetAttributeValue("FontName");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
			std::string picture_x = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
			std::string picture_y = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
			std::string picture_fontsize = strTmp;
			xmlHandler.GetXmlAttribute(elmTmp, "FontName", strTmp);
			std::string picture_fontname = strTmp;

			float scale_width,scale_height;
			std::vector<def_pos> poses;

			float _width = m_print_info.at(i)._picture._width;
			float _height = m_print_info.at(i)._picture._height;

			std::string s_width = m_print_info.at(i)._picture.s_width;
			std::string s_height = m_print_info.at(i)._picture.s_height;

			float f_def1 = m_print_info.at(i)._picture.left_edge;
			float f_def2 = m_print_info.at(i)._picture.down_edge;
			float f_def3 = m_print_info.at(i)._picture.right_edge;
			float f_def4 = m_print_info.at(i)._picture.up_edge;

			if (_width > _height)
			{
				scale_width = 50.0;
				scale_height = _height/_width * 50.0;
				if (scale_height < 40.0)
				{
					scale_height = 40.0;
				}
				pDC->Rectangle(atoi(picture_x.c_str())+src_x,atoi(picture_y.c_str())+src_y+(50-scale_height)/2,atoi(picture_x.c_str())+src_x+scale_width,atoi(picture_y.c_str())+src_y+(50-scale_height)/2+scale_height);

				def_pos p1,p2,p3,p4;
				p1.x = atoi(picture_x.c_str())+src_x;
				p1.y = atoi(picture_y.c_str())+src_y+(50-scale_height)/2 + scale_height/2-1;
				p2.x = atoi(picture_x.c_str())+src_x + scale_width/2;
				p2.y = atoi(picture_y.c_str())+src_y+(50-scale_height)/2 + scale_height;
				p3.x = atoi(picture_x.c_str())+src_x + scale_width;
				p3.y = atoi(picture_y.c_str())+src_y+(50-scale_height)/2 + scale_height/2-1;
				p4.x = atoi(picture_x.c_str())+src_x + scale_width/2;
				p4.y = atoi(picture_y.c_str())+src_y+(50-scale_height)/2;

				poses.push_back(p1);
				poses.push_back(p2);
				poses.push_back(p3);
				poses.push_back(p4);
			}
			else
			{
				scale_height = 50.0;
				scale_width = _width/_height * 50.0;
				if (scale_width < 40.0)
				{
					scale_width = 40.0;
				}
				pDC->Rectangle(atoi(picture_x.c_str())+src_x+(50-scale_width)/2,atoi(picture_y.c_str())+src_y,atoi(picture_x.c_str())+src_x+(50-scale_width)/2+scale_width,atoi(picture_y.c_str())+src_y+scale_height);
				def_pos p1,p2,p3,p4;
				p1.x = atoi(picture_x.c_str())+src_x+(50-scale_width)/2;
				p1.y = atoi(picture_y.c_str())+src_y+scale_height/2;
				p2.x = atoi(picture_x.c_str())+src_x+(50-scale_width)/2 + scale_width/2;
				p2.y = atoi(picture_y.c_str())+src_y+scale_height;
				p3.x = atoi(picture_x.c_str())+src_x+(50-scale_width)/2 + scale_width;
				p3.y = atoi(picture_y.c_str())+src_y+scale_height/2;
				p4.x = atoi(picture_x.c_str())+src_x+(50-scale_width)/2 + scale_width/2;
				p4.y = atoi(picture_y.c_str())+src_y;

				poses.push_back(p1);
				poses.push_back(p2);
				poses.push_back(p3);
				poses.push_back(p4);
			}

			CString _swidth;
			CString _sheight;

			_swidth.Format("%s",s_width.c_str());
			_sheight.Format("%s",s_height.c_str());

			CFont arcfont1,arcfont2;
			arcfont1.CreateFont(atoi(picture_fontsize.c_str()), 0, 900, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, (LPCSTR)picture_fontname.c_str());
			arcfont2.CreateFont(atoi(picture_fontsize.c_str()), 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, (LPCSTR)picture_fontname.c_str());

			pDC->SelectObject(&arcfont2);
			pDC->TextOut(poses.at(3).x-4*_swidth.GetLength(),poses.at(3).y-30,_swidth);
			pDC->SelectObject(&arcfont1);
			pDC->TextOut(poses.at(2).x+10,poses.at(2).y+4*_sheight.GetLength(),_sheight);

			CBrush   *brush1=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(0,0,0)));   
			CBrush   *brush=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,255))); 

			pDC->MoveTo(poses.at(2).x+10,poses.at(2).y + scale_height/2);
			pDC->LineTo(poses.at(2).x+25,poses.at(2).y + scale_height/2);

			pDC->MoveTo(poses.at(2).x+10,poses.at(2).y - scale_height/2);
			pDC->LineTo(poses.at(2).x+25,poses.at(2).y - scale_height/2);

			pDC->MoveTo(poses.at(3).x - scale_width/2,poses.at(3).y-30);
			pDC->LineTo(poses.at(3).x - scale_width/2,poses.at(3).y-15);

			pDC->MoveTo(poses.at(3).x + scale_width/2,poses.at(3).y-30);
			pDC->LineTo(poses.at(3).x + scale_width/2,poses.at(3).y-15);

			pDC->SelectObject(brush1);  
			pDC->BeginPath();
			pDC->MoveTo(poses.at(2).x+17,poses.at(2).y + scale_height/2);
			pDC->LineTo(poses.at(2).x+13,poses.at(2).y + scale_height/2-4);
			pDC->LineTo(poses.at(2).x+21,poses.at(2).y + scale_height/2-4);
			pDC->EndPath();
			pDC->FillPath();
			pDC->SelectObject(brush); 

			pDC->SelectObject(brush1);  
			pDC->BeginPath();
			pDC->MoveTo(poses.at(2).x+17,poses.at(2).y - scale_height/2);
			pDC->LineTo(poses.at(2).x+13,poses.at(2).y - scale_height/2+4);
			pDC->LineTo(poses.at(2).x+21,poses.at(2).y - scale_height/2+4);
			pDC->EndPath();
			pDC->FillPath();
			pDC->SelectObject(brush); 

			pDC->SelectObject(brush1);  
			pDC->BeginPath();
			pDC->MoveTo(poses.at(3).x - scale_width/2,poses.at(3).y-23);
			pDC->LineTo(poses.at(3).x - scale_width/2+4,poses.at(3).y-19);
			pDC->LineTo(poses.at(3).x - scale_width/2+4,poses.at(3).y-27);
			pDC->EndPath();
			pDC->FillPath();
			pDC->SelectObject(brush); 

			pDC->SelectObject(brush1);  
			pDC->BeginPath();
			pDC->MoveTo(poses.at(3).x + scale_width/2,poses.at(3).y-23);
			pDC->LineTo(poses.at(3).x + scale_width/2-4,poses.at(3).y-19);
			pDC->LineTo(poses.at(3).x + scale_width/2-4,poses.at(3).y-27);
			pDC->EndPath();
			pDC->FillPath();
			pDC->SelectObject(brush); 


			pDC->MoveTo(poses.at(0).x+5,poses.at(0).y);
			pDC->LineTo(poses.at(2).x-5,poses.at(2).y);
			pDC->MoveTo(poses.at(2).x-5-6,poses.at(0).y-6);
			pDC->LineTo(poses.at(2).x-5,poses.at(2).y);
			pDC->MoveTo(poses.at(2).x-5-6,poses.at(0).y+6);
			pDC->LineTo(poses.at(2).x-5,poses.at(2).y);

			if ((f_def1 - 2.0 > -0.01) && (f_def1 - 2.0 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(0).x-2,poses.at(0).y);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y+8);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush);  
			}
			else if((f_def1 - 1.5 > -0.01) && (f_def1 - 1.5 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(0).x-2,poses.at(0).y);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush); 

				pDC->MoveTo(poses.at(0).x-2,poses.at(0).y);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->MoveTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y+8);
				pDC->MoveTo(poses.at(0).x-10,poses.at(0).y+8);
				pDC->LineTo(poses.at(0).x-2,poses.at(0).y);
			}
			else if((f_def1 - 1.0 > -0.01) && (f_def1 - 1.0 < 0.01))
			{
				pDC->MoveTo(poses.at(0).x-2,poses.at(0).y);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->MoveTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y+8);
				pDC->MoveTo(poses.at(0).x-10,poses.at(0).y+8);
				pDC->LineTo(poses.at(0).x-2,poses.at(0).y);
			}
			else if((f_def1 - 0.5 > -0.01) && (f_def1 - 0.5 < 0.01))
			{
				pDC->MoveTo(poses.at(0).x-2,poses.at(0).y);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y-8);
				pDC->MoveTo(poses.at(0).x-2,poses.at(0).y);
				pDC->LineTo(poses.at(0).x-10,poses.at(0).y+8);
			}

			if ((f_def2 - 2.0 > -0.01) && (f_def2 - 2.0 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(1).x,poses.at(1).y);
				pDC->LineTo(poses.at(1).x-8,poses.at(1).y+8);
				pDC->LineTo(poses.at(1).x+8,poses.at(1).y+8);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush); 
			}
			else if ((f_def2 - 1.5 > -0.01) && (f_def2 - 1.5 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(1).x,poses.at(1).y);
				pDC->LineTo(poses.at(1).x+8,poses.at(1).y+8);
				pDC->LineTo(poses.at(1).x,poses.at(1).y+8);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush); 

				pDC->MoveTo(poses.at(1).x,poses.at(1).y);
				pDC->LineTo(poses.at(1).x-8,poses.at(1).y+8);
				pDC->MoveTo(poses.at(1).x-8,poses.at(1).y+8);
				pDC->LineTo(poses.at(1).x+8,poses.at(1).y+8);
				pDC->MoveTo(poses.at(1).x+8,poses.at(1).y+8);
				pDC->LineTo(poses.at(1).x,poses.at(1).y);

			}
			else if ((f_def2 - 1.0 > -0.01) && (f_def2 - 1.0 < 0.01))
			{
				pDC->MoveTo(poses.at(1).x,poses.at(1).y);
				pDC->LineTo(poses.at(1).x-8,poses.at(1).y+8);
				pDC->MoveTo(poses.at(1).x-8,poses.at(1).y+8);
				pDC->LineTo(poses.at(1).x+8,poses.at(1).y+8);
				pDC->MoveTo(poses.at(1).x+8,poses.at(1).y+8);
				pDC->LineTo(poses.at(1).x,poses.at(1).y);
			}
			else if((f_def2 - 0.5 > -0.01) && (f_def2 - 0.5 < 0.01))
			{
				pDC->MoveTo(poses.at(1).x,poses.at(1).y);
				pDC->LineTo(poses.at(1).x-8,poses.at(1).y+8);
				pDC->MoveTo(poses.at(1).x,poses.at(1).y);
				pDC->LineTo(poses.at(1).x+8,poses.at(1).y+8);
			}


			if ((f_def3 - 2.0 > -0.01) && (f_def3 - 2.0 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(2).x,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y-8);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y+8);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush);  
			}
			else if ((f_def3 - 1.5 > -0.01) && (f_def3 - 1.5 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(2).x,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y+8);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush);

				pDC->MoveTo(poses.at(2).x,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y-8);
				pDC->MoveTo(poses.at(2).x+8,poses.at(2).y-8);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y+8);
				pDC->MoveTo(poses.at(2).x+8,poses.at(2).y+8);
				pDC->LineTo(poses.at(2).x,poses.at(2).y);

			}
			else if((f_def3 - 1.0 > -0.01) && (f_def3 - 1.0 < 0.01))
			{
				pDC->MoveTo(poses.at(2).x,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y-8);
				pDC->MoveTo(poses.at(2).x+8,poses.at(2).y-8);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y+8);
				pDC->MoveTo(poses.at(2).x+8,poses.at(2).y+8);
				pDC->LineTo(poses.at(2).x,poses.at(2).y);
			}
			else if((f_def3 - 0.5 > -0.01) && (f_def3 - 0.5 < 0.01))
			{
				pDC->MoveTo(poses.at(2).x,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y-8);
				pDC->MoveTo(poses.at(2).x,poses.at(2).y);
				pDC->LineTo(poses.at(2).x+8,poses.at(2).y+8);
			}

			if ((f_def4 - 2.0 > -0.01) && (f_def4 - 2.0 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(3).x,poses.at(3).y-2);
				pDC->LineTo(poses.at(3).x-8,poses.at(3).y-10);
				pDC->LineTo(poses.at(3).x+8,poses.at(3).y-10);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush);  
			}
			else if ((f_def4 - 1.5 > -0.01) && (f_def4 - 1.5 < 0.01))
			{
				pDC->SelectObject(brush1);  
				pDC->BeginPath();
				pDC->MoveTo(poses.at(3).x,poses.at(3).y-2);
				pDC->LineTo(poses.at(3).x,poses.at(3).y-10);
				pDC->LineTo(poses.at(3).x+8,poses.at(3).y-10);
				pDC->EndPath();
				pDC->FillPath();
				pDC->SelectObject(brush); 

				pDC->MoveTo(poses.at(3).x,poses.at(3).y-2);
				pDC->LineTo(poses.at(3).x-8,poses.at(3).y-10);
				pDC->MoveTo(poses.at(3).x-8,poses.at(3).y-10);
				pDC->LineTo(poses.at(3).x+8,poses.at(3).y-10);
				pDC->MoveTo(poses.at(3).x+8,poses.at(3).y-10);
				pDC->LineTo(poses.at(3).x,poses.at(3).y-2);

			}
			else if ((f_def4 - 1.0 > -0.01) && (f_def4 - 1.0 < 0.01))
			{
				pDC->MoveTo(poses.at(3).x,poses.at(3).y-2);
				pDC->LineTo(poses.at(3).x-8,poses.at(3).y-10);
				pDC->MoveTo(poses.at(3).x-8,poses.at(3).y-10);
				pDC->LineTo(poses.at(3).x+8,poses.at(3).y-10);
				pDC->MoveTo(poses.at(3).x+8,poses.at(3).y-10);
				pDC->LineTo(poses.at(3).x,poses.at(3).y-2);
			}
			else if((f_def4 - 0.5 > -0.01) && (f_def4 - 0.5 < 0.01))
			{
				pDC->MoveTo(poses.at(3).x,poses.at(3).y-2);
				pDC->LineTo(poses.at(3).x-8,poses.at(3).y-10);
				pDC->MoveTo(poses.at(3).x,poses.at(3).y-2);
				pDC->LineTo(poses.at(3).x+8,poses.at(3).y-10);
			}

			pDC->SelectObject(&pen);
		}


		//打印文字信息
		for (map<std::string,std::string>::iterator iter = m_print_info.at(i).printtext.begin();iter != m_print_info.at(i).printtext.end();iter++)
		{
			std::string _text_name = iter->first;
			//std::string _text_value = iter->second;

			//hg3d::CXmlNode * text_node = print_node->GetChildByAttribute("Name",_text_name.c_str());
			//if (text_node != NULL && text_node->GetAttributeValue("Used") == "1")
			nUsed = 0;
			elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", _text_name.c_str());
			xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
			if(elmTmp != NULL && nUsed == 1)
			{
				//std::string text_x = text_node->GetAttributeValue("X");
				//std::string text_y = text_node->GetAttributeValue("Y");
				//std::string text_fontsize = text_node->GetAttributeValue("FontSize");
				//std::string text_fontname = text_node->GetAttributeValue("FontName");
				//std::string text_leftstr = text_node->GetAttributeValue("LeftStr");
				//std::string text_rightstr = text_node->GetAttributeValue("RightStr");
				//std::string _text_value = text_node->GetAttributeValue("Value");
				CString strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "X", strTmp);
				std::string text_x = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "Y", strTmp);
				std::string text_y = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "FontSize", strTmp);
				std::string text_fontsize = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "FontName", strTmp);
				std::string text_fontname = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "LeftStr", strTmp);
				std::string text_leftstr = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "RightStr", strTmp);
				std::string text_rightstr = strTmp;
				xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
				std::string _text_value = strTmp;

				CFont newfont;

				newfont.CreateFont(atoi(text_fontsize.c_str()), 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_ROMAN, (LPCSTR)text_fontname.c_str());

				pDC->SelectObject(newfont);
				CString str_text;
				str_text.Format("%s%s%s",text_leftstr.c_str(),_text_value.c_str(),text_rightstr.c_str());
				pDC->TextOut(atoi(text_x.c_str())+src_x,atoi(text_y.c_str())+src_y,str_text);
				pDC->SelectObject(oldfont);
				newfont.DeleteObject();
			}
		}

		//打印Logo文本
		DrawLogoTextInLabel(pDC, CRect(src_x, src_y, src_x + _paperWidth, src_y + _paperHeight));

		nUsed = 0;
		elmTmp = xmlHandler.GetIndicatedElement(elmPrintNode, "Parameter", "Name", "排版图");
		xmlHandler.GetXmlAttribute(elmTmp, "Used", nUsed);
		if(elmTmp != NULL && nUsed == 1)
		{
			int nX, nY;
			xmlHandler.GetXmlAttribute(elmTmp, "X", nX);
			xmlHandler.GetXmlAttribute(elmTmp, "Y", nY);
			CString strValue;
			xmlHandler.GetXmlAttribute(elmTmp, "Value", strValue);
			int nXLen = 0, nYLen = 0, nDiagramRotateAngle = 0;
			vector<CString> vSplittedString;
			vSplittedString = SplitStringByChar(strValue, '*');
			nXLen = atoi(vSplittedString[0]);
			nYLen = atoi(vSplittedString[1]);
			nDiagramRotateAngle = atoi(vSplittedString[2]);

			if((nDiagramRotateAngle/90)%2 != 0)
			{
				int nTmp = nXLen;
				nXLen = nYLen;
				nYLen = nTmp;
			}


			CRect rcDiagram;
			rcDiagram.SetRect(nX, nY, nX+nXLen, nY+nYLen);
			rcDiagram.OffsetRect(src_x, src_y);
			pDC->Rectangle(rcDiagram);

			pDC->DrawText("排版图", rcDiagram, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	// Do not call CDialogEx::OnPaint() for painting messages
}


void CPrintSetDlg::OnCbnSelchangeCombo1()
{
	OnPaint();
}


void CPrintSetDlg::OnBnClickedButton1()
{
	

	CDC dc;
	CPrintDialog printDlg(FALSE);
	if(printDlg.DoModal() == IDOK)
	{
		HGLOBAL hDevMode;
		HGLOBAL hDevNames;

		//short paperWidth = pwidth;
		//short paperHeight = pheight;

		printDlg.GetDefaults();
		DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(printDlg.m_pd.hDevMode);
		pDevMode->dmFields = pDevMode->dmFields | DM_PAPERSIZE;
		pDevMode->dmPaperSize = DMPAPER_USER;    

		//pDevMode->dmPaperWidth = paperWidth;
		//pDevMode->dmPaperLength = paperHeight;

		::GlobalUnlock(printDlg.m_pd.hDevMode);

		CPrintInfo Info;

		dc.Attach(printDlg.CreatePrinterDC());
		dc.m_bPrinting = TRUE;
		CString strTitle;
		strTitle.LoadString(AFX_IDS_APP_TITLE);
		DOCINFO di;

		::ZeroMemory (&di, sizeof (DOCINFO));
		di.cbSize = sizeof (DOCINFO);
		di.lpszDocName = "测试打印";

		BOOL bPrintingOK = dc.StartDoc(&di);

		Info.m_rectDraw.SetRect(0,0,dc.GetDeviceCaps(HORZRES),dc.GetDeviceCaps(VERTRES));

		Info.SetMaxPage (1);
		OnPrint(&dc, &Info,"",m_paperWidth,m_paperHeight);
		if (bPrintingOK)
			dc.EndDoc();
		else
			dc.AbortDoc();

		dc.Detach();
	}
}


void CPrintSetDlg::OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight)
{
	CDC* pDC   = pdc;
	CPrintInfo* pInfo = (CPrintInfo *)lParam;

	CFont DataFont;
	DataFont.CreatePointFont(120,"宋体",pDC);

	HDC dcMem;
	dcMem=::CreateCompatibleDC(pDC->m_hDC);
	dcMem=m_moduleshow.GetDC()->GetSafeHdc();

	CRect r = pInfo->m_rectDraw;
	int nVertCenterPos = pDC->GetDeviceCaps (VERTRES) / 2;
	::StretchBlt(pDC->m_hDC, r.left, r.top, r.Width(), r.Height(),
		dcMem,m_srcx,m_srcy,bwidth,bheight,SRCCOPY);

	::DeleteDC(dcMem);
}

void CPrintSetDlg::OnBnClickedCheck1()
{
	
	UpdateData(TRUE);
	OnCbnSelchangeCombo1();
	UpdateData(FALSE);
}


void CPrintSetDlg::OnCbnSelchangeCombo2()
{
	
	OnCbnSelchangeCombo1();
	
}


//void CPrintSetDlg::OnCbnSelendcancelCombo2()
//{
//	
//
//}
int CPrintSetDlg::MyBarcode(CDC* cdc,int x1,int y1,int y2,int codetype,int ipen,const char * cCode)
{
	CString csCode;
	CString LocalBmpPath;
	csCode.Format("%s",cCode);
	Barcode39 code;
	code.Encode39(csCode);
	/*if (39==codetype)
	{
		Barcode39 code;
		code.Encode39(csCode);
	}else 
	{
		Barcode128 code;
		code.Encode128C(csCode);
	}*/

	int iPenW = ipen;
	COLORREF clrBar		=RGB(0,0,0);
	COLORREF clrSpace	=RGB(255,255,255);


	int codelen = iPenW*code.GetEncodeLength();
	HDC hDC = cdc->GetSafeHdc();

	CRect m_SaveRect(x1,y1,x1 + codelen + 5,y2);
	CRect rectCodeText(x1,y2+5,x1 + codelen + 5,y2+15);
	FillRect(hDC,m_SaveRect,CBrush(clrSpace));

	code.DrawBarcode(hDC,x1,y1,y2,y2,clrBar,clrSpace,iPenW);

	return 0;
}

void CPrintSetDlg::OnBnClickedButton2()//修改模版响应函数
{
	
	CString str_edit;
	m_modulesel.GetLBText(m_modulesel.GetCurSel(),str_edit);
	std::string s_edit = str_edit.GetBuffer();

	CPrintUserSetDlg dlg;
	//hg3d::TString strConfigFile = _T("picture");
	//strConfigFile = hg3d::getFullFileName(strConfigFile);
	//strConfigFile += _T("\\printcode.xml");

	//hg3d::CXmlFile* doc = new hg3d::CXmlFile();
	//if (!doc->Load(strConfigFile.c_str()))
	//{
	//	AfxMessageBox("读取printcode.xml失败",MB_OK);
	//	return;
	//}
	CString strTmp;
	strTmp.Format(_T("picture\\printcode%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;
	XmlHandler xmlHandler;
	if(!xmlHandler.LoadFile(strHGE))
	{
		AfxMessageBox("读取printcode.xml失败",MB_OK);
		return;
	}

	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	vector<TiXmlElement*> vCodeModule = xmlHandler.GetChildElms(elmRoot);

	//for (int i = 0;i < doc->GetChildNodes().size();i++)
	for (int i = 0;i < vCodeModule.size();i++)
	{
		//std::string node_name = doc->GetChildNodes().at(i)->GetAttributeValue("Name");
		TiXmlElement* elmCodeModule = vCodeModule[i];
		CString strTmp;
		xmlHandler.GetXmlAttribute(elmCodeModule, "Name", strTmp);
		std::string node_name = strTmp;

		if (strcmp(s_edit.c_str(),node_name.c_str()) == 0)
		{
			//hg3d::CXmlNode * child_node = doc->GetChildNodes().at(i);
			vector<TiXmlElement*> vItem = xmlHandler.GetChildElms(elmCodeModule);
			//for (int j = 0;j < child_node->GetChildNodes().size();j++)
			for (int j = 0;j < vItem.size();j++)
			{
				//if (child_node->GetChildNodes().at(j)->GetAttributeValue("Used") == "1")
				int nUsed = 0;
				xmlHandler.GetXmlAttribute(vItem[j], "Used", nUsed);
				if(nUsed == 1)
				{
					PrintNode onenode;
					//onenode._x = atoi(child_node->GetChildNodes().at(j)->GetAttributeValue("X").c_str());
					//onenode._y = atoi(child_node->GetChildNodes().at(j)->GetAttributeValue("Y").c_str());
					//onenode._fontname = child_node->GetChildNodes().at(j)->GetAttributeValue("FontName");
					//onenode._fontsize = child_node->GetChildNodes().at(j)->GetAttributeValue("FontSize");
					//onenode._leftstr = child_node->GetChildNodes().at(j)->GetAttributeValue("LeftStr");
					//onenode._name = child_node->GetChildNodes().at(j)->GetAttributeValue("Name");
					//onenode._rightstr = child_node->GetChildNodes().at(j)->GetAttributeValue("RightStr");
					//onenode._value = child_node->GetChildNodes().at(j)->GetAttributeValue("Value");
					xmlHandler.GetXmlAttribute(vItem[j], "X", onenode._x);
					xmlHandler.GetXmlAttribute(vItem[j], "Y", onenode._y);
					CString strTmp;
					xmlHandler.GetXmlAttribute(vItem[j], "FontName", strTmp);
					onenode._fontname = strTmp;
					xmlHandler.GetXmlAttribute(vItem[j], "FontSize", strTmp);
					onenode._fontsize = strTmp;
					xmlHandler.GetXmlAttribute(vItem[j], "LeftStr", strTmp);
					onenode._leftstr = strTmp;
					xmlHandler.GetXmlAttribute(vItem[j], "Name", strTmp);
					onenode._name = strTmp;
					xmlHandler.GetXmlAttribute(vItem[j], "RightStr", strTmp);
					onenode._rightstr = strTmp;
					xmlHandler.GetXmlAttribute(vItem[j], "Value", strTmp);
					onenode._value = strTmp;

					dlg.m_allnode.push_back(onenode);
				}
			}
			//dlg.m_endx = dlg.m_endx + atoi(child_node->GetAttributeValue("paperWidth").c_str());
			//dlg.m_endy = dlg.m_endy + atoi(child_node->GetAttributeValue("paperHeight").c_str());
			int nTmp;
			xmlHandler.GetXmlAttribute(elmCodeModule, "paperWidth", nTmp);
			dlg.m_endx = dlg.m_endx + nTmp;
			xmlHandler.GetXmlAttribute(elmCodeModule, "paperHeight", nTmp);
			dlg.m_endy = dlg.m_endy + nTmp;

			break;
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		std::string s_index = "";
		std::string s_Used = "";
		std::string s_paperHeight = "";
		std::string s_paperWidth = "";
		std::string s_pictureName = "";
		//for (int i = 0;i < doc->GetChildNodes().size();i++)
		for (int i = 0;i < vCodeModule.size();i++)
		{
			//std::string node_name = doc->GetChildNodes().at(i)->GetAttributeValue("Name");
			TiXmlElement* elmCodeModule = vCodeModule[i];
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmCodeModule, "Name", strTmp);
			std::string node_name = strTmp;

			if (strcmp(s_edit.c_str(),node_name.c_str()) == 0)
			{
				//hg3d::CXmlNode * node = doc->GetChildNodes().at(i);

				//s_index = node->GetAttributeValue("Index");
				//s_Used = node->GetAttributeValue("Used");
				//s_paperHeight = node->GetAttributeValue("paperHeight");
				//s_paperWidth = node->GetAttributeValue("paperWidth");
				//s_pictureName = node->GetAttributeValue("pictureName");
				xmlHandler.GetXmlAttribute(elmCodeModule, "Index", strTmp);
				s_index = strTmp;
				xmlHandler.GetXmlAttribute(elmCodeModule, "Used", strTmp);
				s_Used = strTmp;
				xmlHandler.GetXmlAttribute(elmCodeModule, "paperHeight", strTmp);
				s_paperHeight = strTmp;
				xmlHandler.GetXmlAttribute(elmCodeModule, "paperWidth", strTmp);
				s_paperWidth = strTmp;
				xmlHandler.GetXmlAttribute(elmCodeModule, "pictureName", strTmp);
				s_pictureName = strTmp;

				//doc->RemoveChild(node);
				elmRoot->RemoveChild(elmCodeModule);
				break;
			}
		}

		//hg3d::CXmlNode * parentnode = hg3d::CXmlNode::NewElementNode("CodeModule");
		//parentnode->SetAttributeValue("Index",s_index);
		//parentnode->SetAttributeValue("Name",s_edit);
		//parentnode->SetAttributeValue("Used",s_Used);
		//parentnode->SetAttributeValue("paperHeight",s_paperHeight);
		//parentnode->SetAttributeValue("paperWidth",s_paperWidth);
		//parentnode->SetAttributeValue("pictureName",s_pictureName);

		//doc->AddChild(parentnode);
		TiXmlElement* elmCodeModuleInsert = new TiXmlElement("CodeModule");
		elmRoot->LinkEndChild(elmCodeModuleInsert);
		elmCodeModuleInsert->SetAttribute("Index",s_index.c_str());
		elmCodeModuleInsert->SetAttribute("Name",s_edit.c_str());
		elmCodeModuleInsert->SetAttribute("Used",s_Used.c_str());
		elmCodeModuleInsert->SetAttribute("paperHeight",s_paperHeight.c_str());
		elmCodeModuleInsert->SetAttribute("paperWidth",s_paperWidth.c_str());
		elmCodeModuleInsert->SetAttribute("pictureName",s_pictureName.c_str());

		for (int j = 0;j < dlg.m_allnode.size();j++)
		{
			CString s_x,s_y;
			s_x.Format("%d",dlg.m_allnode.at(j)._x);
			s_y.Format("%d",dlg.m_allnode.at(j)._y);

			//hg3d::CXmlNode * onenode = hg3d::CXmlNode::NewElementNode("Parameter");
			//onenode->SetAttributeValue("FontName",dlg.m_allnode.at(j)._fontname);
			//onenode->SetAttributeValue("FontSize",dlg.m_allnode.at(j)._fontsize);
			//onenode->SetAttributeValue("LeftStr",dlg.m_allnode.at(j)._leftstr);
			//onenode->SetAttributeValue("Name",dlg.m_allnode.at(j)._name);
			//onenode->SetAttributeValue("RightStr",dlg.m_allnode.at(j)._rightstr);
			//onenode->SetAttributeValue("Value",dlg.m_allnode.at(j)._value);
			//onenode->SetAttributeValue("Used","1");
			//onenode->SetAttributeValue("X",s_x.GetBuffer());
			//onenode->SetAttributeValue("Y",s_y.GetBuffer());

			//parentnode->AddChild(onenode);
			TiXmlElement* elmParam = new TiXmlElement("Parameter");
			elmCodeModuleInsert->LinkEndChild(elmParam);
			elmParam->SetAttribute("FontName",dlg.m_allnode.at(j)._fontname.c_str());
			elmParam->SetAttribute("FontSize",dlg.m_allnode.at(j)._fontsize.c_str());
			elmParam->SetAttribute("LeftStr",dlg.m_allnode.at(j)._leftstr.c_str());
			elmParam->SetAttribute("Name",dlg.m_allnode.at(j)._name.c_str());
			elmParam->SetAttribute("RightStr",dlg.m_allnode.at(j)._rightstr.c_str());
			elmParam->SetAttribute("Value",dlg.m_allnode.at(j)._value.c_str());
			elmParam->SetAttribute("Used","1");
			elmParam->SetAttribute("X",s_x.GetBuffer());
			elmParam->SetAttribute("Y",s_y.GetBuffer());
		}
		//doc->Save(strConfigFile.c_str());
		CString strXML = HGTools::getXmlPathFromHgxPath(strHGE);
		xmlHandler.GetDoc()->SaveFile(strXML);
		encrypt_base64(strXML.GetBuffer(), strHGE.GetBuffer());
		HGTools::deleteFile(strXML);

		OnPaint();
	}
}

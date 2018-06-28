// LeftPanelDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "LeftPanelDlg.h"
#include "afxdialogex.h"
//#include "hg3d/Utils.h"
//#include "hg3d/XmlFile.h"
#include "../Misc/Misc.h"
#include "../Misc/HGTools.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// CLeftPanelDlg dialog

IMPLEMENT_DYNAMIC(CLeftPanelDlg, CDialogEx)

CLeftPanelDlg::CLeftPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLeftPanelDlg::IDD, pParent)
{

	m_edith = 0;
	m_edits = 0;
	m_editw = 0;
}

CLeftPanelDlg::~CLeftPanelDlg()
{
}

void CLeftPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKSHOW, m_checkshow);
	DDX_Text(pDX, IDC_EDITH, m_edith);
	DDX_Text(pDX, IDC_EDITS, m_edits);
	DDX_Text(pDX, IDC_EDITW, m_editw);
}


BEGIN_MESSAGE_MAP(CLeftPanelDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLeftPanelDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLeftPanelDlg message handlers


void CLeftPanelDlg::OnBnClickedOk()
{
	
	UpdateData(TRUE);
	//std::string xml_path = hg3d::getFullFileName("\\kdata\\leftpanel.xml");
	//hg3d::CXmlFile * doc = new hg3d::CXmlFile;
	//doc->Load(xml_path.c_str());
	//hg3d::CXmlNode * child_w = doc->GetChildByAttribute("Name","MinWidth");
	//hg3d::CXmlNode * child_h = doc->GetChildByAttribute("Name","MinHeight");
	//hg3d::CXmlNode * child_s = doc->GetChildByAttribute("Name","MinSize");
	//hg3d::CXmlNode * child_show = doc->GetChildByAttribute("Name","IsShow");
	CString strTmp;
	strTmp.Format(_T("\\kdata\\leftpanel%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;

	XmlHandler xmlHandler;
	xmlHandler.LoadFile(strHGE);
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();

	//if (child_w != NULL)
	//{
	//	CString str_w;
	//	str_w.Format("%d",m_editw);
	//	child_w->SetAttributeValue("Value",str_w.GetBuffer());
	//}
	CString str_w;
	str_w.Format("%d",m_editw);
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "MinWidth", "Value", str_w.GetBuffer());
	str_w.ReleaseBuffer();

	//if (child_h != NULL)
	//{
	//	CString str_h;
	//	str_h.Format("%d",m_edith);
	//	child_h->SetAttributeValue("Value",str_h.GetBuffer());
	//}
	CString str_h;
	str_h.Format("%d",m_edith);
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "MinHeight", "Value", str_h.GetBuffer());
	str_h.ReleaseBuffer();

	//if (child_s != NULL)
	//{
	//	CString str_s;
	//	str_s.Format("%d",m_edits);
	//	child_s->SetAttributeValue("Value",str_s.GetBuffer());
	//}
	CString str_s;
	str_s.Format("%d",m_edits);
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "MinSize", "Value", str_s.GetBuffer());
	str_s.ReleaseBuffer();

	//if (child_show != NULL)
	//{
	//	CString str_show;
	//	str_show.Format("%d",m_checkshow.GetCheck());
	//	child_show->SetAttributeValue("Value",str_show.GetBuffer());
	//	
	//}
	CString str_show;
	str_show.Format("%d",m_checkshow.GetCheck());
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "IsShow", "Value", str_show.GetBuffer());
	str_show.ReleaseBuffer();

	CString strXML = HGTools::getXmlPathFromHgxPath(strHGE);
	xmlHandler.GetDoc()->SaveFile(strXML);
	encrypt_base64( strXML.GetBuffer(), strHGE.GetBuffer());
	//¹Ø±ÕÎÄ¼þ
	HGTools::deleteFile(strXML);

	UpdateData(FALSE);
	CDialogEx::OnOK();
}


BOOL CLeftPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	UpdateData(TRUE);
	//std::string xml_path = hg3d::getFullFileName("\\kdata\\leftpanel.xml");
	//hg3d::CXmlFile * doc = new hg3d::CXmlFile;
	//doc->Load(xml_path.c_str());
	//hg3d::CXmlNode * child_w = doc->GetChildByAttribute("Name","MinWidth");
	//hg3d::CXmlNode * child_h = doc->GetChildByAttribute("Name","MinHeight");
	//hg3d::CXmlNode * child_s = doc->GetChildByAttribute("Name","MinSize");
	//hg3d::CXmlNode * child_show = doc->GetChildByAttribute("Name","IsShow");
	CString strTmp;
	strTmp.Format(_T("\\kdata\\leftpanel%s"), g_szEncyptSubfix);
	CString strHGE = GetModulePath() + strTmp;

	XmlHandler xmlHandler;
	xmlHandler.LoadFile(strHGE);
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();

	//if (child_w != NULL)
	//{
	//	std::string str_w = child_w->GetAttributeValue("Value");
	//	m_editw = atoi(str_w.c_str());
	//}
	TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "MinWidth");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_editw);

	//if (child_h != NULL)
	//{
	//	std::string str_h = child_h->GetAttributeValue("Value");
	//	m_edith = atoi(str_h.c_str());
	//}
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "MinHeight");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_edith);

	//if (child_s != NULL)
	//{
	//	std::string str_s = child_s->GetAttributeValue("Value");
	//	m_edits = atoi(str_s.c_str());
	//}
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "MinSize");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_edits);

	//if (child_show != NULL)
	//{
	//	std::string str_show = child_show->GetAttributeValue("Value");
	//	if (atoi(str_show.c_str()) == 0)
	//	{
	//		m_checkshow.SetCheck(false);
	//	}
	//	else
	//	{
	//		m_checkshow.SetCheck(true);
	//	}
	//}
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "IsShow");
	int nTmp;
	xmlHandler.GetXmlAttribute(elmTmp, "Value", nTmp);
	if (nTmp == 0)
	{
		m_checkshow.SetCheck(false);
	}
	else
	{
		m_checkshow.SetCheck(true);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

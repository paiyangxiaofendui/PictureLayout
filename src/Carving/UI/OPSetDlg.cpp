// OPSetDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "OPSetDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
//#include "XmlHandlePlus.h"
#include "../Misc/Misc.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// COPSetDlg dialog

IMPLEMENT_DYNAMIC(COPSetDlg, CDialogEx)

COPSetDlg::COPSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COPSetDlg::IDD, pParent)
	, m_nEditStep1Count(200)
	, m_nComboStep1Alg(1)
	, m_nEditStep2Count(0)
	, m_nEditStep3Count(0)
	, m_fEditStep3AcceptableUti(0.9)
{
	m_valDownerInfoFirst = FALSE;
}

COPSetDlg::~COPSetDlg()
{
}

void COPSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMKNIFE, m_comknife);
	//DDX_Control(pDX, IDC_COMTYPE, m_comtype);
	//  DDX_Control(pDX, IDC_EDITRATE, m_editrate);
	//  DDX_Control(pDX, IDC_MINH, m_minh);
	//  DDX_Control(pDX, IDC_MINSIZE, m_minsize);
	//  DDX_Control(pDX, IDC_MINW, m_minw);
	//DDX_Text(pDX, IDC_EDITRATE, m_editrate);
	//DDX_Text(pDX, IDC_OP_TIMEOUT, m_op_timeout);
	DDX_Text(pDX, IDC_EDIT_STEP1_COUNT, m_nEditStep1Count);
	DDX_CBIndex(pDX, IDC_COMBO_STEP1_ALG, m_nComboStep1Alg);
	DDX_Text(pDX, IDC_EDIT_STEP2_COUNT, m_nEditStep2Count);
	DDX_Text(pDX, IDC_EDIT_STEP3_COUNT, m_nEditStep3Count);
	DDX_Text(pDX, IDC_EDIT_STEP3_ACCEPTABLE_UTI, m_fEditStep3AcceptableUti);
	DDX_Check(pDX, IDC_CHECK_DOWNER_INFO_FIRST, m_valDownerInfoFirst);
}


BEGIN_MESSAGE_MAP(COPSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &COPSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COPSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// COPSetDlg message handlers

/*---------------------------------------*/
//	函数说明：
//		
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void COPSetDlg::AutoSetOpSettings(int nCpnNum)
{
	// 自动计算优化参数



	if (nCpnNum < 50)
	{
		m_nEditStep1Count = 1000;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 0;
		m_fEditStep3AcceptableUti = 0.95;
	}
	else if (nCpnNum > 50 && nCpnNum < 100)
	{
		m_nEditStep1Count = 1500;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 0;
		m_fEditStep3AcceptableUti = 0.95;
	}
	else if (nCpnNum >= 100 && nCpnNum < 200)
	{
		m_nEditStep1Count = 2000;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 0;
		m_fEditStep3AcceptableUti = 0.95;
	}
	else if (nCpnNum >= 200 && nCpnNum < 300)
	{
		m_nEditStep1Count = 1500;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 0;
		m_fEditStep3AcceptableUti = 0.95;
	}
	else if (nCpnNum >= 300 && nCpnNum < 500)
	{
		m_nEditStep1Count = 1000;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 0;
		m_fEditStep3AcceptableUti = 0.95;
	}
	else if (nCpnNum >= 500 && nCpnNum < 1000)
	{
		m_nEditStep1Count = 500;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 0;
		m_fEditStep3AcceptableUti = 0.95;
	}
	else
	{
		m_nEditStep1Count = 500;
		m_nComboStep1Alg = 1;
		m_nEditStep2Count = 0;
		m_nEditStep3Count = 10;
		m_fEditStep3AcceptableUti = 0.95;
	}


	

	saveSettings();
}


void COPSetDlg::loadSettings()
{
	//配置文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\Op%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
	//解密
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());


	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();
	//关闭文件
	HGTools::deleteFile(xmlPath);
	//加载辅助

	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = doc.RootElement();

	//读取数据
	TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep1Count");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_nEditStep1Count);

	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ComboStep1Alg");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_nComboStep1Alg);

	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep2Count");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_nEditStep2Count);

	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep3Count");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_nEditStep3Count);

	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep3AcceptableUti");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_fEditStep3AcceptableUti);

	// 反面信息优先
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "DownerInfoFirst");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", m_valDownerInfoFirst);

}

void COPSetDlg::saveSettings()
{
	//配置文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\Op%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//解密
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	doc.LoadFile();

	//加载辅助
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = doc.RootElement();

	//保存数据
	//TiXmlElement* elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep1Count");
	//elmTmp->SetAttribute("Value", GetFloatString(m_nEditStep1Count, 0).GetBuffer());
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "EditStep1Count", "Value", GetFloatString(m_nEditStep1Count, 0).GetBuffer());

	//elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ComboStep1Alg");
	//elmTmp->SetAttribute("Value", GetFloatString(m_nComboStep1Alg, 0).GetBuffer());
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "ComboStep1Alg", "Value", GetFloatString(m_nComboStep1Alg, 0).GetBuffer());

	//elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep2Count");
	//elmTmp->SetAttribute("Value", GetFloatString(m_nEditStep2Count, 0).GetBuffer());
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "EditStep2Count", "Value", GetFloatString(m_nEditStep2Count, 0).GetBuffer());

	//elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep3Count");
	//elmTmp->SetAttribute("Value", GetFloatString(m_nEditStep3Count, 0).GetBuffer());
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "EditStep3Count", "Value", GetFloatString(m_nEditStep3Count, 0).GetBuffer());

	//elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "EditStep3AcceptableUti");
	//elmTmp->SetAttribute("Value", GetFloatString(m_fEditStep3AcceptableUti, 2).GetBuffer());
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "EditStep3AcceptableUti", "Value", GetFloatString(m_fEditStep3AcceptableUti, 2).GetBuffer());


	// 反面信息优先
	CString str_DownerInfoFirst = GetFloatString(m_valDownerInfoFirst, 2);
	xmlHandler.SetIndicatedElmAttr(elmRoot, "Param", "Name", "DownerInfoFirst", "Value", str_DownerInfoFirst.GetBuffer());

	//保存
	doc.SaveFile(xmlPath);

	encrypt_base64( xmlPath.GetBuffer(), hgePath.GetBuffer());
	
	//关闭文件
	HGTools::deleteFile(xmlPath);


	// 保存到优化信息






}

void COPSetDlg::OnBnClickedOk()
{
	
	UpdateData(TRUE);

	saveSettings();

	UpdateData(FALSE);
	CDialogEx::OnOK();
}


void COPSetDlg::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}


BOOL COPSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	//m_comtype.InsertString(0,"不分割");

	//m_comknife.InsertString(0,"无限制");
	//m_comknife.InsertString(1,"开长条");
	//m_comknife.InsertString(2,"开短条");

	//m_comtype.SetCurSel(0);
	//m_comknife.SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_STEP1_ALG))->AddString(_T("改进最低轮廓线算法"));
	((CComboBox*)GetDlgItem(IDC_COMBO_STEP1_ALG))->AddString(_T("改进贪心算法"));
	((CComboBox*)GetDlgItem(IDC_COMBO_STEP1_ALG))->AddString(_T("组合算法"));

	loadSettings();
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

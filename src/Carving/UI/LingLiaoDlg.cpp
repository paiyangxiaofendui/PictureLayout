// LingLiaoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LingLiaoDlg.h"
#include "afxdialogex.h"
//#include "HGTools.h"
//#include "XmlHandlePlus.h"
#include "../Misc/HGTools.h"
#include "../Misc/Misc.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// LingLiaoDlg 对话框

IMPLEMENT_DYNAMIC(LingLiaoDlg, CDialogEx)

LingLiaoDlg::LingLiaoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LingLiaoDlg::IDD, pParent)
	, m_CompanyName(_T(""))
	, m_OrderID(_T(""))
	, m_ShiftEmployee(_T(""))
	, m_ShiftDate(_T(""))
	, m_UseEmployee(_T(""))
	, m_UseDate(_T(""))
	, m_ManageEmployee(_T(""))
	, m_OrderStore(_T(""))
	, m_AuditEmployee(_T(""))
	, m_UseMachineName(_T(""))
	, m_OrderMoreInfo(_T(""))
{

}

LingLiaoDlg::~LingLiaoDlg()
{
}

void LingLiaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMPANY_NAME, m_CompanyName);
	DDX_Text(pDX, IDC_ORDER_ID, m_OrderID);
	DDX_Text(pDX, IDC_SHIFT_EMPLOYEE, m_ShiftEmployee);
	DDX_DateTimeCtrl(pDX, IDC_SHIFT_DATE, m_ShiftDate);
	DDX_Text(pDX, IDC_USE_EMPLOYEE, m_UseEmployee);
	DDX_DateTimeCtrl(pDX, IDC_USE_DATE, m_UseDate);
	DDX_Text(pDX, IDC_MANAGE_EMPLOYEE, m_ManageEmployee);
	DDX_Text(pDX, IDC_ORDER_STORE, m_OrderStore);
	DDX_Text(pDX, IDC_AUDIT_EMPLOYEE, m_AuditEmployee);
	DDX_Text(pDX, IDC_USE_MACHINE_NAME, m_UseMachineName);
	DDX_Text(pDX, IDC_ORDER_MOREINFO, m_OrderMoreInfo);
}


BEGIN_MESSAGE_MAP(LingLiaoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LingLiaoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// LingLiaoDlg 消息处理程序


BOOL LingLiaoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	loadSettings();

	UpdateData(FALSE);

	return TRUE;
}


void LingLiaoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	saveSettings();
	CDialogEx::OnOK();
}

void LingLiaoDlg::loadSettings()
{
	//配置文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\ZuiXiaoDaBanCanShu%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
	//解密
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());

	//加载文件
	TinyXml::TiXmlDocument doc(xmlPath);
	bool bLoadSuccess = doc.LoadFile();
	HGTools::deleteFile(xmlPath);

	if(!bLoadSuccess)
		return;

	//加载辅助
	//TinyXml::XmlHandlePlus docHandler(&doc);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = doc.RootElement();

	//CString strTmp;
	TiXmlElement* elmTmp;
	// 公司名称
	//m_CompanyName = docHandler.getParam("CompanyName","宏光软件").c_str();
	strTmp = g_szCompanySimpleName;
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "CompanyName");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_CompanyName = strTmp;
	// 订单编号
	//m_OrderID = docHandler.getParam("OrderID").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "OrderID");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_OrderID = strTmp;
	// 排产人员
	//m_ShiftEmployee = docHandler.getParam("ShiftEmployee").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ShiftEmployee");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_ShiftEmployee = strTmp;
	// 排产日期
	//m_ShiftDate = docHandler.getParam("ShiftDate").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ShiftDate");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_ShiftDate = strTmp;
	// 领料人员
	//m_UseEmployee = docHandler.getParam("UseEmployee").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "UseEmployee");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_UseEmployee = strTmp;
	// 领料日期
	//m_UseDate = docHandler.getParam("UseDate").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "UseDate");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_UseDate = strTmp;
	// 仓管人员
	//m_ManageEmployee = docHandler.getParam("ManageEmployee").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "ManageEmployee");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_ManageEmployee = strTmp;
	// 接单店面
	//m_OrderStore = docHandler.getParam("OrderStore").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "OrderStore");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_OrderStore = strTmp;
	// 审核人员
	//m_AuditEmployee = docHandler.getParam("AuditEmployee").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "AuditEmployee");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_AuditEmployee = strTmp;
	// 开料类型
	//m_UseMachineName = docHandler.getParam("UseMachineName","宏光雕刻机").c_str();
	strTmp.Format(_T("%s雕刻机"), g_szCompanyMostSimpleName);
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "UseMachineName");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_UseMachineName = strTmp;
	// 订单注释
	//m_OrderMoreInfo = docHandler.getParam("OrderMoreInfo").c_str();
	strTmp = "";
	elmTmp = xmlHandler.GetIndicatedElement(elmRoot, "Param", "Name", "OrderMoreInfo");
	xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
	m_OrderMoreInfo = strTmp;
}

void LingLiaoDlg::saveSettings()
{
	//配置文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\ZuiXiaoDaBanCanShu%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
	//解密
	//decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
	//加载文件
	//TinyXml::TiXmlDocument doc(xmlPath);
	//bool bSuccess = doc.LoadFile();
	TinyXml::TiXmlDocument doc;

	//加载辅助
	//TinyXml::XmlHandlePlus docHandler(&doc);
	//docHandler.changeDirectoryTo("/Root/");
	//docHandler.setParamElementName("Param");

	TiXmlElement* elmRoot = new TiXmlElement("Root");
	doc.LinkEndChild(elmRoot);

	// 公司名称
	//docHandler.setParam("CompanyName",m_CompanyName.GetBuffer());
	TiXmlElement* elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "CompanyName");
	elmParam->SetAttribute("Value", m_CompanyName.GetBuffer());
	m_CompanyName.ReleaseBuffer();
	// 订单编号
	//docHandler.setParam("OrderID",m_OrderID.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "OrderID");
	elmParam->SetAttribute("Value", m_OrderID.GetBuffer());
	m_OrderID.ReleaseBuffer();
	// 排产人员
	//docHandler.setParam("ShiftEmployee",m_ShiftEmployee.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "ShiftEmployee");
	elmParam->SetAttribute("Value", m_ShiftEmployee.GetBuffer());
	m_ShiftEmployee.ReleaseBuffer();
	// 排产日期
	//docHandler.setParam("ShiftDate",m_ShiftDate.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "ShiftDate");
	elmParam->SetAttribute("Value", m_ShiftDate.GetBuffer());
	m_ShiftDate.ReleaseBuffer();
	// 领料人员
	//docHandler.setParam("UseEmployee",m_UseEmployee.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "UseEmployee");
	elmParam->SetAttribute("Value", m_UseEmployee.GetBuffer());
	m_UseEmployee.ReleaseBuffer();
	// 领料日期
	//docHandler.setParam("UseDate",m_UseDate.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "UseDate");
	elmParam->SetAttribute("Value", m_UseDate.GetBuffer());
	m_UseDate.ReleaseBuffer();
	// 仓管人员
	//docHandler.setParam("ManageEmployee",m_ManageEmployee.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "ManageEmployee");
	elmParam->SetAttribute("Value", m_ManageEmployee.GetBuffer());
	m_ManageEmployee.ReleaseBuffer();
	// 接单店面
	//docHandler.setParam("OrderStore",m_OrderStore.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "OrderStore");
	elmParam->SetAttribute("Value", m_OrderStore.GetBuffer());
	m_OrderStore.ReleaseBuffer();
	// 审核人员
	//docHandler.setParam("AuditEmployee",m_AuditEmployee.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "AuditEmployee");
	elmParam->SetAttribute("Value", m_AuditEmployee.GetBuffer());
	m_AuditEmployee.ReleaseBuffer();
	// 开料类型
	//docHandler.setParam("UseMachineName",m_UseMachineName.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "UseMachineName");
	elmParam->SetAttribute("Value", m_UseMachineName.GetBuffer());
	m_UseMachineName.ReleaseBuffer();
	// 订单注释
	//docHandler.setParam("OrderMoreInfo",m_OrderMoreInfo.GetBuffer());
	elmParam = new TiXmlElement("Param");
	elmRoot->LinkEndChild(elmParam);
	elmParam->SetAttribute("Name", "OrderMoreInfo");
	elmParam->SetAttribute("Value", m_OrderMoreInfo.GetBuffer());
	m_OrderMoreInfo.ReleaseBuffer();


	//保存
	doc.SaveFile(xmlPath);
	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
	//关闭文件
	HGTools::deleteFile(xmlPath);
}

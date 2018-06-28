// SelMachineDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "../resource.h"
#include "SelMachineDlg.h"
#include "afxdialogex.h"

#include "../Misc/HGTools.h"
#include "../Misc/XmlHandlePlus.h"
#include "../Misc/Misc.h"


#include "../../../include/HG3D/XmlFile.h"
#include "../../../include/HG3D/Utils.h"
#include "../../../include/HG3D/hgtypes.h"
#include "../../../include/HG3D/XmlNode.h"

#include "../../../include/KnifeDll/KnifeClass.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// CSelMachineDlg dialog

IMPLEMENT_DYNAMIC(CSelMachineDlg, CDialogEx)

CSelMachineDlg::CSelMachineDlg(CString title ,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelMachineDlg::IDD, pParent)
	, m_bLabelInReverseSide(false)
{
	_cutdouble = false;
	_coorsysuse = false;
	m_windowTitle = title;
}

CSelMachineDlg::~CSelMachineDlg()
{
}

void CSelMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_TYPE, m_type);
}


BEGIN_MESSAGE_MAP(CSelMachineDlg, CDialogEx)
	ON_BN_CLICKED(IDOK2, &CSelMachineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL3, &CSelMachineDlg::OnBnClickedCancel3)
	ON_NOTIFY(NM_DBLCLK, IDC_TYPE, &CSelMachineDlg::OnNMDblclkType)
END_MESSAGE_MAP()


/************************************************************************/
/* 界面事件                                                             */
/************************************************************************/
// 初始化
BOOL CSelMachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_windowTitle != "")
	{
		SetWindowText(m_windowTitle);
	}

	DWORD dwStyle; 
	dwStyle = m_type.GetExtendedStyle();  
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;
	m_type.SetExtendedStyle(dwStyle);

	m_type.InsertColumn(0,"机器型号",0,165);
	m_type.InsertColumn(1,"标签打印",0,155);

	std::vector<KnifeType> kt;


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hgm文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szEncyptSubfix);
	CString strConfigFileHgm = HGTools::getRelativePath(strTmp);
	CString strConfigFileXml = HGTools::getXmlPathFromHgxPath(strConfigFileHgm);

	// 解密文件
	decrypt_base64(strConfigFileHgm.GetBuffer(), strConfigFileXml.GetBuffer());


#else

	CString strConfigFileHgm = HGTools::getRelativePath("kdata\\knifetype.hgm");
	CString strConfigFileXml = HGTools::getXmlPathFromHgxPath(strConfigFileHgm);

	//HGTools::decryptFile(strConfigFileHgm,strConfigFileXml);

#endif



	



	TinyXml::TiXmlDocument doc(strConfigFileXml.GetBuffer());
	bool bSuccess = doc.LoadFile();
	//TinyXml::XmlHandlePlus docHandler(&doc);

	std::vector<TinyXml::TiXmlElement*> machineArr;
	TinyXml::TiXmlElement* elmRoot = doc.RootElement();

	for(TinyXml::TiXmlElement* elmKnifeType = elmRoot->FirstChildElement("KnifeType"); elmKnifeType != NULL; elmKnifeType = elmKnifeType->NextSiblingElement("KnifeType"))
	{
		CString strIsUsed;
		strIsUsed = elmKnifeType->Attribute("Used");
		if(strIsUsed.CompareNoCase("1") == 0)
		{
			machineArr.push_back(elmKnifeType);
		}
	}
	//std::vector<TinyXml::TiXmlNode*> machineArr = docHandler.findAll("/Knife/KnifeType/",TinyXml::XmlAttrValueCompare("Used","1"));

	XmlHandler xmlHandler;

	for(int i = 0;i<machineArr.size();i++)
	{
		//TinyXml::XmlHandlePlus nodeHandle(machineArr[i]);

		//std::string str_name = nodeHandle.getAttr("Name");
		//std::string str_print = nodeHandle.getAttr("Print");
		//std::string str_coorsysuse = nodeHandle.getAttr("CoorSysUse");
		//std::string str_double = nodeHandle.getAttr("Double");
		//std::string str_grouptype = nodeHandle.getAttr("GroupType");
		CString strName, strPrint;
		int nLabelMachineID = 0;
		xmlHandler.GetXmlAttribute(machineArr[i], "Name", strName);
		xmlHandler.GetXmlAttribute(machineArr[i], "Print", nLabelMachineID);
		if(nLabelMachineID >= 0 && nLabelMachineID < sizeof(g_aLabelMachineName)/sizeof(char*))
			strPrint = g_aLabelMachineName[nLabelMachineID];
		else
			strPrint.Format(_T("%d"), nLabelMachineID);

		int nItem = m_type.InsertItem(i,strName);
		m_type.SetItemText(nItem,1,strPrint);
		m_type.SetItemData(nItem, nLabelMachineID);
	}
	m_combo.SetCurSel(0);
	HGTools::deleteFile(strConfigFileXml);

	return TRUE; 
}

// 确认
void CSelMachineDlg::OnBnClickedOk()
{
	int pos = m_type.GetNextItem(-1,LVIS_SELECTED); 
	if (pos < 0)
	{
		AfxMessageBox("请选中机器型号");
	}
	else
	{
		m_nodename = m_type.GetItemText(pos,0);
		CString strTmp;
		strTmp.Format(_T("%d"), m_type.GetItemData(pos));
		m_printname = strTmp;

		loadSettings();
		CDialogEx::OnOK();
	}
}

// 取消
void CSelMachineDlg::OnBnClickedCancel3()
{
	OnCancel();
}

//双击表单
void CSelMachineDlg::OnNMDblclkType(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//
	//int pos = m_type.GetNextItem(-1,LVIS_SELECTED); 
	//if (pos < 0)
	//{
	//	AfxMessageBox("请选中机器型号");
	//}
	//else
	//{
	//	m_nodename = m_type.GetItemText(pos,0);
	//	m_printname = m_type.GetItemText(pos,1);

	//	loadSettings();
	//	*pResult = 0;
	//	OnOK();
	//}
	OnBnClickedOk();
}

//根据机型信息装载刀库数据
void CSelMachineDlg::loadSettings()
{
	//hg3d::TString strConfigFile = _T("kdata");
	//CString ConfigFilePath = hg3d::getFullFileName(strConfigFile).c_str();

	CString ConfigFilePath = GetModulePath() +  _T("kdata");


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hgm文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szEncyptSubfix);
	CString hgmpath = HGTools::getRelativePath(strTmp);
	CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);

	// 解密文件
	decrypt_base64(hgmpath.GetBuffer(), xmlpath.GetBuffer());


#else

	CString hgmpath = ConfigFilePath + "\\knifetype.hgm";
	CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);

	//HGTools::decryptFile(hgmpath,xmlpath);

#endif









	//TinyXml::TiXmlDocument doc(xmlpath.GetBuffer());
	//doc.LoadFile();
	XmlHandler xmlHandler;
	xmlHandler.LoadFile(xmlpath.GetBuffer());
	HGTools::deleteFile(xmlpath);

	//TinyXml::XmlHandlePlus docHandle(&doc);
	//TinyXml::XmlAttrValueCompare kcompare("Name",m_nodename);
	//找到Name属性为m_nodename 的KnifeType节点
	//std::vector<TinyXml::TiXmlNode*> knifeTypeNodes = docHandle.findAll("/Knife/KnifeType/", kcompare);
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	TiXmlElement* elmKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", m_nodename.c_str());

	//for (int i =0;i<knifeTypeNodes.size();i++)
	if(elmKnifeType != NULL)
	{
		{
			//TinyXml::XmlHandlePlus thisKnifeTpyeNode(knifeTypeNodes.at(i));
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmKnifeType, "CoorSysUse", strTmp);
			std::string str_coorsysuse = /*thisKnifeTpyeNode.getAttr("CoorSysUse")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "Double", strTmp);
			std::string str_double = /*thisKnifeTpyeNode.getAttr("Double")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "CoorType", strTmp);
			std::string str_coortype = /*thisKnifeTpyeNode.getAttr("CoorType")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "CoorType2", strTmp);
			std::string str_coortype2 = /*thisKnifeTpyeNode.getAttr("CoorType2")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "GroupStr", strTmp);
			std::string str_groupstr = /*thisKnifeTpyeNode.getAttr("GroupStr")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "DvCut", strTmp);
			std::string str_dvcut = /*thisKnifeTpyeNode.getAttr("DvCut")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();

			xmlHandler.GetXmlAttribute(elmKnifeType, "PName", strTmp);
			std::string str_Pname = /*thisKnifeTpyeNode.getAttr("PName")*/strTmp.GetBuffer();//标签文件头
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "PLast", strTmp);
			std::string str_Plast = /*thisKnifeTpyeNode.getAttr("PLast")*/strTmp.GetBuffer();//标签后缀
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "CName", strTmp);
			std::string str_Cname = /*thisKnifeTpyeNode.getAttr("CName")*/strTmp.GetBuffer();//nc文件头
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "CLast", strTmp);
			std::string str_Clast = /*thisKnifeTpyeNode.getAttr("CLast")*/strTmp.GetBuffer();//nc文件后缀
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "Pxhx", strTmp);
			std::string str_Pxhx = /*thisKnifeTpyeNode.getAttr("Pxhx")*/strTmp.GetBuffer();//
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmKnifeType, "Cxhx", strTmp);
			std::string str_Cxhx = /*thisKnifeTpyeNode.getAttr("Cxhx")*/strTmp.GetBuffer();//
			strTmp.ReleaseBuffer();

			if (str_Cxhx=="1")
			{
				m_cxhx = "_";
			}else
				m_cxhx = "";
			if (str_Pxhx=="1")
			{
				m_pxhx = "_";
			}else
				m_pxhx = "";
			if (str_coorsysuse == "1") 
			{
				_coorsysuse = true;
			}
			else
			{
				_coorsysuse = false;
			}
			if (str_double == "1")
			{
				_cutdouble = true;
			}
			else
			{
				_cutdouble = false;
			}


			if (str_Pname == "")//标签文件名
			{
				m_pname = g_szCompangMostSimpleNameEng;
			}
			else
			{
				m_pname = str_Pname;
			}


			if (str_Plast == "")//标签后缀
			{
				m_plast = "nc";
			}
			else
			{
				m_plast = str_Plast;
			}

			if (str_Cname == "")//nc文件名
			{
				m_cname = g_szCompangMostSimpleNameEng;
			}
			else
			{
				m_cname = str_Cname;
			}
			m_clast = str_Clast;//nc文件后缀

			_coortype = atoi(str_coortype.c_str());
			_coortype2 = atoi(str_coortype2.c_str());
			_groupstr = str_groupstr;
			m_dvcut = str_dvcut;


			//TinyXml::XmlHandlePlus knifeTypeNode(knifeTypeNodes.at(i));
			//knifeTypeNode.changeDirectoryTo("ParamList/");
			TiXmlElement* elmParamList = xmlHandler.GetChildElm(elmKnifeType, "ParamList");

			//for(int j = 0;j<node->GetChildNodes().size();j++)
			{
				//std::string NodeName = node->GetChildNodes().at(j)->GetName();
				//if (NodeName == "ParamList")
				{
					//hg3d::CXmlNode* ParamListNode = node->GetChildNodes().at(j);
					//for(int k =0;k<ParamListNode->GetChildNodes().size();k++)
					{
						//hg3d::CXmlNode* ParamNode = ParamListNode->GetChildNodes().at(k);
						//if (ParamNode->GetAttributeValue("Name") == "GFirst")//主文件文件头
						TiXmlElement* elmTmp = NULL;
						{
							CString firstname;
							//firstname.Format(_T("%s"),knifeTypeNode.getParam("GFirst").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GFirst");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", firstname);
							firstname.Replace("@","\r\n");							
							firstname.Replace("!","@");	
							m_header = firstname.Left(firstname.Find('#'));
							m_GFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_header.c_str());
							Temp2.Format(_T("%s"),m_GFirst2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_header = Temp1.GetBuffer();
							m_GFirst2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "GLast")//主文件文件尾
						{
							CString lastname;
							//lastname.Format(_T("%s"),knifeTypeNode.getParam("GLast").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GLast");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", lastname);
							lastname.Replace("@","\r\n");							
							lastname.Replace("!","@");
							m_ender = lastname.Left(lastname.Find('#'));
							m_GLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_ender.c_str());
							Temp2.Format(_T("%s"),m_GLast2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_ender = Temp1.GetBuffer();
							m_GLast2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "GBFirst")//反面文件头
						{
							CString firstname;
							//firstname.Format(_T("%s"),knifeTypeNode.getParam("GBFirst").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GBFirst");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", firstname);
							firstname.Replace("@","\r\n");							
							firstname.Replace("!","@");	
							m_GBFirst1 = firstname.Left(firstname.Find('#'));
							m_GBFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);	
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_GBFirst1.c_str());
							Temp2.Format(_T("%s"),m_GBFirst2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_GBFirst1 = Temp1.GetBuffer();
							m_GBFirst2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "GBLast")//主文件反面文件尾
						{
							CString lastname;
							//lastname.Format(_T("%s"),knifeTypeNode.getParam("GBLast").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GBLast");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", lastname);
							lastname.Replace("@","\r\n");							
							lastname.Replace("!","@");
							m_GBLast1 = lastname.Left(lastname.Find('#'));
							m_GBLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_GBLast1.c_str());
							Temp2.Format(_T("%s"),m_GBLast2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_GBLast1 = Temp1.GetBuffer();
							m_GBLast2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "MFirst")//主轴文件头
						{
							CString firstname;
							//firstname.Format(_T("%s"),knifeTypeNode.getParam("MFirst").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "MFirst");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", firstname);
							firstname.Replace("@","\r\n");							
							firstname.Replace("!","@");
							m_MFirst1 = firstname.Left(firstname.Find('#'));
							m_MFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_MFirst1.c_str());
							Temp2.Format(_T("%s"),m_MFirst2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_MFirst1 = Temp1.GetBuffer();
							m_MFirst2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "MLast")//主轴文件尾
						{
							CString lastname;
							//lastname.Format(_T("%s"),knifeTypeNode.getParam("MLast").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "MLast");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", lastname);
							lastname.Replace("@","\r\n");							
							lastname.Replace("!","@");
							m_MLast1 = lastname.Left(lastname.Find('#'));
							m_MLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_MLast1.c_str());
							Temp2.Format(_T("%s"),m_MLast2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_MLast1 = Temp1.GetBuffer();
							m_MLast2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "PZFirst")//排鉆文件头
						{
							CString firstname;
							//firstname.Format(_T("%s"),knifeTypeNode.getParam("PZFirst").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "PZFirst");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", firstname);
							firstname.Replace("@","\r\n");							
							firstname.Replace("!","@");
							m_PZFirst1 = firstname.Left(firstname.Find('#'));
							m_PZFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_PZFirst1.c_str());
							Temp2.Format(_T("%s"),m_PZFirst2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_PZFirst1 = Temp1.GetBuffer();
							m_PZFirst2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "PZLast")//排鉆文件尾
						{
							CString lastname;
							//lastname.Format(_T("%s"),knifeTypeNode.getParam("PZLast").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "PZLast");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", lastname);
							lastname.Replace("@","\r\n");							
							lastname.Replace("!","@");
							m_PZLast1 = lastname.Left(lastname.Find('#'));
							m_PZLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_PZLast1.c_str());
							Temp2.Format(_T("%s"),m_PZLast2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_PZLast1 = Temp1.GetBuffer();
							m_PZLast2 = Temp2.GetBuffer();
						}

						//if (ParamNode->GetAttributeValue("Name") == "SPFirst")//小板文件头
						{
							CString firstname;
							//firstname.Format(_T("%s"),knifeTypeNode.getParam("SPFirst").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SPFirst");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", firstname);
							firstname.Replace("@","\r\n");							
							firstname.Replace("!","@");
							m_SPFirst1 = firstname.Left(firstname.Find('#'));
							m_SPFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_SPFirst1.c_str());
							Temp2.Format(_T("%s"),m_SPFirst2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_SPFirst1 = Temp1.GetBuffer();
							m_SPFirst2 = Temp2.GetBuffer();
						}
						//if (ParamNode->GetAttributeValue("Name") == "SPLast")//小板文件尾
						{
							CString lastname;
							//lastname.Format(_T("%s"),knifeTypeNode.getParam("SPLast").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SPLast");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", lastname);
							lastname.Replace("@","\r\n");							
							lastname.Replace("!","@");
							m_SPLast1 = lastname.Left(lastname.Find('#'));
							m_SPLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
							CString Temp1,Temp2;
							Temp1.Format(_T("%s"),m_SPLast1.c_str());
							Temp2.Format(_T("%s"),m_SPLast2.c_str());
							Temp1.Replace("~","#");
							Temp2.Replace("~","#");
							m_SPLast1 = Temp1.GetBuffer();
							m_SPLast2 = Temp2.GetBuffer();
						}

						//if (ParamNode->GetAttributeValue("Name") == "BTrim1")//工位1修边1
						{  
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("BTrim1").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim1");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_trim1 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "BTrim2")//工位1修边2
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("BTrim2").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim2");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_trim2 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "BTrim3")//工位1修边3
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("BTrim3").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim3");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_trim3 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "BTrim4")//工位1修边4
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("BTrim4").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim4");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_trim4 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FTrim1")//工位1修边1
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("FTrim1").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim1");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_ftrim1 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FTrim2")//工位2修边2
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("FTrim2").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim2");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_ftrim2 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FTrim3")//工位2修边3
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("FTrim3").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim3");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_ftrim3 = atof(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FTrim4")//工位2修边4
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("FTrim4").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim4");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_ftrim4 = atof(theTrim);
						}

						//if (ParamNode->GetAttributeValue("Name") == "BUsedLineX")//用户设置有效边，用于修边偏移（工位1X）
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("BUsedLineX").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BUsedLineX");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_usedLineX1 = atoi(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "BUsedLineY")//用户设置有效边，用于修边偏移（工位1Y）
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("BUsedLineY").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BUsedLineY");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_usedLineY1 = atoi(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FUsedLineX")//用户设置有效边，用于修边偏移（工位2X）
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("FUsedLineX").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FUsedLineX");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_usedLineX2 = atoi(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FUsedLineY")//用户设置有效边，用于修边偏移（工位2Y）
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("FUsedLineY").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FUsedLineY");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_usedLineY2 = atoi(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "EachNCLineExtra")//每行NC代码结尾追加
						{
							//std::string theName = knifeTypeNode.getParam("EachNCLineExtra");
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "EachNCLineExtra");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
							std::string theName = strTmp.GetBuffer();
							strTmp.ReleaseBuffer();
							m_EachNCLineExtra = HGTools::Convert2CString(theName);
						}


						//if (ParamNode->GetAttributeValue("Name") == "IsCobine")//是否合并正反面头尾
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("IsCobine").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsCobine");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_iscombine = atoi(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "IsComLine")//是否连接NC文件
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("IsComLine").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsComLine");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_iscomline = atoi(theTrim);
						}
						//if (ParamNode->GetAttributeValue("Name") == "IsComBAll")//是否合并所有NC文件
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("IsComBAll").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsComBAll");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_iscomball = atoi(theTrim);
						}

						//if (ParamNode->GetAttributeValue("Name") == "LessKnifeChange")//最少换刀设置
						{
							CString theTrim;
							//theTrim.Format(_T("%s"),knifeTypeNode.getParam("LessKnifeChange").c_str());
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "LessKnifeChange");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", theTrim);
							m_lessKnifeChange = atoi(theTrim);
						}

						//if (ParamNode->GetAttributeValue("Name") == "SmallPanelNC")//小板NC文件导出
						{
							//std::string theName = knifeTypeNode.getParam("SmallPanelNC","0");
							strTmp = "0";
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SmallPanelNC");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
							std::string theName = strTmp.GetBuffer();
							strTmp.ReleaseBuffer();
							m_SmallPanelNC = HGTools::Convert2CString(theName);
						}
						//if (ParamNode->GetAttributeValue("Name") == "FileClassifyMove")// 生成NC、贴标文件归整
						{
							//std::string theName = knifeTypeNode.getParam("FileClassifyMove","0");
							strTmp = "0";
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FileClassifyMove");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
							std::string theName = strTmp.GetBuffer();
							strTmp.ReleaseBuffer();
							m_fileClassifyMove = HGTools::Convert2CString(theName);
						}
						//if (ParamNode->GetAttributeValue("Name") == "NC_CommandChange")// NC命令字变更
						{
							//std::string theName = knifeTypeNode.getParam("NC_CommandChange","0");
							strTmp = "0";
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_CommandChange");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
							std::string theName = strTmp.GetBuffer();
							m_NC_CommandChange = HGTools::Convert2CString(theName);
						}
						//if (ParamNode->GetAttributeValue("Name") == "NC_SpaceRemove")// NC坐标空格删除
						{
							//std::string theName = knifeTypeNode.getParam("NC_SpaceRemove","0");
							strTmp = "0";
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_SpaceRemove");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
							std::string theName = strTmp.GetBuffer();
							m_NC_SpaceRemove = HGTools::Convert2CString(theName);
						}
						{
							strTmp = "0";
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_ModifyArcMillingFormat");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", strTmp);
							std::string theName = strTmp.GetBuffer();
							m_NC_ModifyArcMillingFormat = HGTools::Convert2CString(theName);
						}
						{
							elmTmp = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "LabelSide");
							xmlHandler.GetXmlAttribute(elmTmp, "Value", m_bLabelInReverseSide);
						}
					}
				}

			}
			//break;
		}
	}
}

//获得当前机型的所有刀具
std::vector<std::map<std::string,std::string>> CSelMachineDlg::getMachineKnifes()
{
	std::vector<std::map<std::string,std::string>> result;

	//hg3d::TString strConfigFile = _T("kdata");
	//CString ConfigFilePath = hg3d::getFullFileName(strConfigFile).c_str();
	CString ConfigFilePath = GetModulePath() +  _T("kdata");

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hgm文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szEncyptSubfix);
	CString hgmpath = HGTools::getRelativePath(strTmp);
	CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);

	// 解密文件
	decrypt_base64(hgmpath.GetBuffer(), xmlpath.GetBuffer());


#else

	CString hgmpath = ConfigFilePath + "\\knifetype.hgm";
	CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);

	//HGTools::decryptFile(hgmpath,xmlpath);

#endif





	//TinyXml::TiXmlDocument doc(xmlpath.GetBuffer());
	//doc.LoadFile();
	XmlHandler xmlHandler;
	xmlHandler.LoadFile(xmlpath.GetBuffer());
	HGTools::deleteFile(xmlpath);

	//TinyXml::XmlHandlePlus docHandle(&doc);
	//TinyXml::XmlAttrValueCompare kcompare("Name",m_nodename);
	//找到Name属性为m_nodename 的KnifeType节点
	//std::vector<TinyXml::TiXmlNode*> knifeTypeNodes = docHandle.findAll("/Knife/KnifeType/", kcompare);
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	TiXmlElement* elmKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", m_nodename.c_str());

	if (/*knifeTypeNodes.size() == 1*/elmKnifeType != NULL)
	{
		//TinyXml::XmlHandlePlus thisKnifeTpyeNode(knifeTypeNodes.at(0));

		//std::vector<TinyXml::TiXmlNode*> knifes = thisKnifeTpyeNode.findAll("Param");
		vector<TiXmlElement*> vParamElm = xmlHandler.GetChildElms(elmKnifeType, "Param");

		for (int i = 0; i < /*knifes.size()*/vParamElm.size() ; i++)
		{
			//TinyXml::XmlHandlePlus thisKnife(knifes.at(i));
			TiXmlElement* elmParam = vParamElm[i];

			std::map<std::string,std::string> oneKnife;
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmParam, "ChangeCode", strTmp);
			oneKnife["ChangeCode"] = /*thisKnife.getAttr("ChangeCode")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "ChangeCode1", strTmp);
			oneKnife["ChangeCode1"] = /*thisKnife.getAttr("ChangeCode1")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "CoorSys", strTmp);
			oneKnife["CoorSys"] = /*thisKnife.getAttr("CoorSys")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "CoorSys1", strTmp);
			oneKnife["CoorSys1"] = /*thisKnife.getAttr("CoorSys1")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "Diameter", strTmp);
			oneKnife["Diameter"] = /*thisKnife.getAttr("Diameter")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "EndSpeed", strTmp);
			oneKnife["EndSpeed"] = /*thisKnife.getAttr("EndSpeed")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "FeedRate", strTmp);
			oneKnife["FeedRate"] = /*thisKnife.getAttr("FeedRate")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "Index", strTmp);
			oneKnife["Index"] = /*thisKnife.getAttr("Index")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "KNum", strTmp);
			oneKnife["KNum"] = /*thisKnife.getAttr("KNum")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "Kcoor", strTmp);
			oneKnife["Kcoor"] = /*thisKnife.getAttr("Kcoor")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "KnifeLength", strTmp);
			oneKnife["KnifeLength"] = /*thisKnife.getAttr("KnifeLength")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "Length", strTmp);
			oneKnife["Length"] = /*thisKnife.getAttr("Length")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "NearSpeed", strTmp);
			oneKnife["NearSpeed"] = /*thisKnife.getAttr("NearSpeed")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "OverSpeed", strTmp);
			oneKnife["OverSpeed"] = /*thisKnife.getAttr("OverSpeed")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "SpindleSpeed", strTmp);
			oneKnife["SpindleSpeed"] = /*thisKnife.getAttr("SpindleSpeed")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "TypeName", strTmp);
			oneKnife["TypeName"] = /*thisKnife.getAttr("TypeName")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();
			xmlHandler.GetXmlAttribute(elmParam, "Used", strTmp);
			oneKnife["Used"] = /*thisKnife.getAttr("Used")*/strTmp.GetBuffer();
			strTmp.ReleaseBuffer();

			result.push_back(oneKnife);
		}
	}
	else
	{
		MessageBox("加载刀库中的刀出现错误");
	}
	return result;
}

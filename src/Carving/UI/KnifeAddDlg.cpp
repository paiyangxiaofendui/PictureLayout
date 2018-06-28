// KnifeAddDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "KnifeAddDlg.h"
#include "afxdialogex.h"
//#include "EditKDlg.h"
//#include "TheChangeCode.h"
#include "../Misc/HGTools.h"
//using namespace hg3d;
// CKnifeAddDlg dialog

//#include "XmlHandlePlus.h"
#include "../Misc/Misc.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

IMPLEMENT_DYNAMIC(CKnifeAddDlg, CDialogEx)

CKnifeAddDlg::CKnifeAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKnifeAddDlg::IDD, pParent)
	, m_EachNCLineExtra(_T(""))
	, m_smallPanelNC(FALSE)
	, m_fileClassifyMove(FALSE)
	, m_NC_CommandChange(FALSE)
	, m_NC_SpaceRemove(FALSE)
	, m_bModifyArcMillingFormat(false)
{
	m_knifecode = _T("");
	m_knifel = 0.0f;
	m_knifer = 0.0f;
	m_feedrate = _T("");
	m_nearspeed = _T("");
	m_mgcode = _T("");
	m_spindlespeed = _T("");
	m_tooldiameter = _T("");
	m_toollength = _T("");
	m_KnifeLength = _T("");
	m_typename = _T("");
	m_mgcodee = _T("");
	//  m_kcoor = _T("");
	m_endspeech = _T("");
	m_overspeech = _T("");
	m_coorx = _T("");
	m_coory = _T("");
	m_knum = _T("");
	m_ksysa = _T("");
	m_ksysb = _T("");
	m_mgcode2 = _T("");
	m_mgcodee2 = _T("");
	m_groupstr = _T("");
	//m_autop = _T("");
	ischeck = false;
	l_Item = -1;
	t_Item = -1;
}

CKnifeAddDlg::~CKnifeAddDlg()
{
}

void CKnifeAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FEEDRATE, m_feedrate);
	DDX_Text(pDX, IDC_NEARSPEED, m_nearspeed);
	DDX_Text(pDX, IDC_MGCODE, m_mgcode);
	DDX_Text(pDX, IDC_SPINDLESPEED, m_spindlespeed);
	DDX_Text(pDX, IDC_TOOLDIAMETER, m_tooldiameter);
	DDX_Text(pDX, IDC_TOOLLENGTH, m_toollength);
	DDX_Text(pDX, IDC_KLONG, m_KnifeLength);
	DDX_Text(pDX, IDC_TYPENAME, m_typename);
	DDX_Control(pDX, IDC_KNIFELIST, m_knifelist);
	DDX_Control(pDX, IDC_COMTYPE, m_comtype);
	DDX_Text(pDX, IDC_MGCODEE, m_mgcodee);
	//  DDX_Text(pDX, IDC_COOR, m_kcoor);
	DDX_Text(pDX, IDC_ENDSPEECH, m_endspeech);
	DDX_Text(pDX, IDC_OVERSPEECH, m_overspeech);
	DDX_Text(pDX, IDC_COORX, m_coorx);
	DDX_Text(pDX, IDC_COORY, m_coory);
	//  DDX_Control(pDX, IDC_KNUM, m_knum);
	DDX_Text(pDX, IDC_KNUM, m_knum);
	DDX_Text(pDX, IDC_KSYSA, m_ksysa);
	DDX_Text(pDX, IDC_KSYSB, m_ksysb);
	DDX_Text(pDX, IDC_MGCODE2, m_mgcode2);
	//  DDX_Control(pDX, IDC_MGCODEE2, m_mgcodee2);
	DDX_Text(pDX, IDC_MGCODEE2, m_mgcodee2);
	DDX_Control(pDX, IDC_KNIFETYPE, m_knifetype);
	DDX_Control(pDX, IDC_TABCHANGE, m_tabChange);
	DDX_Control(pDX, IDC_COMBO1, m_double);
	//DDX_Control(pDX, IDC_COMBO2, m_coortype);
	DDX_Control(pDX, IDC_COMBO4, m_coorsysuse);
	DDX_Control(pDX, IDC_COMBO5, m_dvcut);
	DDX_Control(pDX, IDC_COMBOBOXEX1, m_coortype);

	DDX_Text(pDX, IDC_BTRIM1, m_btrim1);
	DDX_Text(pDX, IDC_BTRIM2, m_btrim2);
	DDX_Text(pDX, IDC_BTRIM3, m_btrim3);
	DDX_Text(pDX, IDC_BTRIM4, m_btrim4);
	DDX_Text(pDX, IDC_2TRIM1, m_ftrim1);
	DDX_Text(pDX, IDC_2TRIM2, m_ftrim2);
	DDX_Text(pDX, IDC_2TRIM3, m_ftrim3);
	DDX_Text(pDX, IDC_2TRIM4, m_ftrim4);
	DDX_Text(pDX, IDC_PNAME,  m_thepname);
	DDX_Text(pDX, IDC_PLNAME, m_theplast);
	DDX_Text(pDX, IDC_CNAME,  m_thecname);
	DDX_Text(pDX, IDC_CLNAME, m_theclast);
	DDX_Text(pDX, IDC_GROUPSTR, m_groupstr);
	//DDX_Text(pDX, IDC_AUTOP, m_autop);
	DDX_Control(pDX, IDC_COMBO_LABEL_MACHINE, m_comboLabelMachine);
	//DDX_Control(pDX, IDC_GROUPSTR, m_groupstr);
	//DDX_Control(pDX, IDC_ISCBCHECK, m_iscombine);
	//DDX_Control(pDX, IDC_ISCBLINE, m_iscomline);
	DDX_Control(pDX, IDC_ISCBLINE, m_Iscomline);
	DDX_Control(pDX, IDC_ISCOMBALL, m_iscomball);
	DDX_Control(pDX, IDC_ISCBCHECK2, m_combineSet);
	DDX_Control(pDX, IDC_COMBOBOXEX2, m_coortype2);
	DDX_Control(pDX, IDC_USED_LINE_X1, m_usedLineX1);
	DDX_Control(pDX, IDC_USED_LINE_Y1, m_usedLineY1);
	DDX_Control(pDX, IDC_USED_LINE_X2, m_usedLineX2);
	DDX_Control(pDX, IDC_USED_LINE_Y2, m_usedLineY2);
	DDX_Text(pDX, IDC_ADD_CHAR, m_EachNCLineExtra);
	DDX_Check(pDX, IDC_SMALL_PANEL, m_smallPanelNC);
	DDX_Check(pDX, IDC_FILE_CLASSIFY_MOVE, m_fileClassifyMove);
	DDX_Check(pDX, IDC_NC_COMMAND_CHANGE, m_NC_CommandChange);
	DDX_Check(pDX, IDC_NC_POS_SPACE_REMOVE, m_NC_SpaceRemove);
	DDX_Check(pDX, IDC_CHECK_MODIFY_ARC_MILLING_FORMAT, m_bModifyArcMillingFormat);

	DDX_Control(pDX, IDC_LESS_KNIFE_CHANGE, m_lessKnifeChange);
}


BEGIN_MESSAGE_MAP(CKnifeAddDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON4, &CKnifeAddDlg::OnBtnMachineSettingSave)	//机器参数保存
	ON_BN_CLICKED(IDOK, &CKnifeAddDlg::OnBnClickedOk)				//确定


	ON_BN_CLICKED(IDC_BUTTON1, &CKnifeAddDlg::OnBtnAddKnife)		//添加
	ON_BN_CLICKED(IDC_BUTTON2, &CKnifeAddDlg::OnBtnDelKnife)		//删除
	ON_BN_CLICKED(IDC_BUTTON3, &CKnifeAddDlg::OnBtnChangeKnife)		//修改

	ON_NOTIFY(NM_DBLCLK, IDC_KNIFELIST, &CKnifeAddDlg::OnNMDblclkKnifelist)
	ON_CBN_SELCHANGE(IDC_COMTYPE, &CKnifeAddDlg::OnCbnSelchangeComtype)
	ON_NOTIFY(NM_CLICK, IDC_KNIFETYPE, &CKnifeAddDlg::OnNMClickKnifetype)				//点选切换机器
	ON_NOTIFY(NM_DBLCLK, IDC_KNIFETYPE, &CKnifeAddDlg::OnNMDblclkKnifetype)				
	ON_NOTIFY(TCN_SELCHANGING, IDC_TABCHANGE, &CKnifeAddDlg::OnTcnSelchangingTabchange)	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCHANGE, &CKnifeAddDlg::OnTcnSelchangeTabchange)		//切换文件头尾
	ON_NOTIFY(NM_CLICK, IDC_KNIFELIST, &CKnifeAddDlg::OnNMClickKnifelist)				//切换刀具
	ON_CBN_SELCHANGE(IDC_COMBO1, &CKnifeAddDlg::OnCbnSelchangeCombo1)					//单双工位切换
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKnifeAddDlg message handlers


//////////////////////////////////////////////////////////////////////////
//纯数据操作
//加载配置信息
void CKnifeAddDlg::loadSettings()
{
	//从hgx文件目录生成xml文件目录
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szEncyptSubfix);
	CString knifeType_hge = HGTools::getRelativePath(strTmp);
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hge);

#if (NEW_ENCRYPT_BASE64 == 1)
	decrypt_base64(knifeType_hge.GetBuffer(), knifeType_xml.GetBuffer());
#else
	//HGTools::decryptFile(knifeType_hge,knifeType_xml);
#endif

	m_doc.LoadFile(knifeType_xml);
	HGTools::deleteFile(knifeType_xml);

}
//保存配置信息
void CKnifeAddDlg::saveSettings()
{
	//从hgx文件目录生成xml文件目录
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szEncyptSubfix);
	CString knifeType_hge = HGTools::getRelativePath(strTmp);
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hge);

	m_doc.SaveFile(knifeType_xml);

#if (NEW_ENCRYPT_BASE64 == 1)
	encrypt_base64( knifeType_xml.GetBuffer(), knifeType_hge.GetBuffer());
#else
	//HGTools::encryptFile(knifeType_xml,knifeType_hge);
#endif

	HGTools::deleteFile(knifeType_xml);

}
//加载机器名
std::vector<CString> CKnifeAddDlg::getMachineNames()
{
	//TinyXml::XmlHandlePlus docHandle(m_doc);
	//std::vector<TinyXml::TiXmlNode*> pKnifeTypeNodeArr = docHandle.findAll("/Knife/KnifeType");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	vector<TiXmlElement*> vKnifeType = xmlHandler.GetChildElms(elmRoot, "KnifeType");

	std::vector<CString> names;
	//for (int i = 0;i<pKnifeTypeNodeArr.size();i++)
	for (int i = 0;i<vKnifeType.size();i++)
	{
		//TinyXml::XmlHandlePlus nodeHandle(pKnifeTypeNodeArr.at(i));
		//names.push_back(nodeHandle.getAttr("Name").c_str());
		TiXmlElement* elmKnifeType = vKnifeType[i];
		CString strName;
		xmlHandler.GetXmlAttribute(elmKnifeType, "Name", strName);
		names.push_back(strName);
	}
	return names;
}
//加载机器使用状态
std::vector<CString> CKnifeAddDlg::getMachineUseds()
{
	//TinyXml::XmlHandlePlus docHandle(m_doc);
	//std::vector<TinyXml::TiXmlNode*> pKnifeTypeNodeArr = docHandle.findAll("/Knife/KnifeType");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	vector<TiXmlElement*> vKnifeType = xmlHandler.GetChildElms(elmRoot, "KnifeType");

	std::vector<CString> useds;
	//for (int i = 0;i<pKnifeTypeNodeArr.size();i++)
	for (int i = 0;i<vKnifeType.size();i++)
	{
		//TinyXml::XmlHandlePlus nodeHandle(pKnifeTypeNodeArr.at(i));
		//useds.push_back(nodeHandle.getAttr("Used").c_str());
		TiXmlElement* elmKnifeType = vKnifeType[i];
		CString strUsed;
		xmlHandler.GetXmlAttribute(elmKnifeType, "Used", strUsed);
		useds.push_back(strUsed);
	}
	return useds;
}

//////////////////////////////////////////////////////////////////////////
//UI操作
//窗体初始化
BOOL CKnifeAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	loadSettings();

	CRect rect;  
	GetClientRect(&rect);  
	oSize.x = rect.right - rect.left;  
	oSize.y = rect.bottom - rect.top;  
	
	//m_knifelist.InsertColumn(0,"刀库编号",0,100);
	m_knifelist.InsertColumn(0,"",LVCFMT_CENTER,20);
	m_knifelist.InsertColumn(1,"刀号",0,100);
	m_knifelist.InsertColumn(2,"刀直径",0,100);
	m_knifelist.InsertColumn(3,"分切界限",0,100);
	m_knifelist.InsertColumn(4,"换刀代码",0,100);
	m_knifelist.InsertColumn(5,"加工速度",0,100);
	m_knifelist.InsertColumn(6,"下刀速度",0,100);
	m_knifelist.InsertColumn(7,"收尾速度",0,100);
	m_knifelist.InsertColumn(8,"空跑速度",0,100);
	m_knifelist.InsertColumn(9,"多刀坐标",0,100);
	m_knifelist.InsertColumn(10,"打孔类型",0,100);
	m_knifelist.InsertColumn(11,"坐标",0,80);
	m_knifelist.InsertColumn(12,"坐标1",0,80);
	m_knifelist.InsertColumn(13,"换刀代码2",0,100);
	m_knifelist.InsertColumn(14,"刀具长度",0,100);
	m_knifelist.InsertColumn(15,"接近速度",0,100);

	m_comtype.InsertString(0,"主轴孔槽料");
	m_comtype.InsertString(1,"主轴孔槽");
	m_comtype.InsertString(2,"主轴槽料");
	m_comtype.InsertString(3,"主轴孔");
	m_comtype.InsertString(4,"主轴槽");
	m_comtype.InsertString(5,"主轴料");
	m_comtype.InsertString(6,"排钻孔");
	m_comtype.InsertString(7,"多刀头钻孔");
	m_comtype.SetCurSel(0);

	map_comtype[0] = 0;
	map_comtype[1] = 1;
	map_comtype[2] = 5;
	map_comtype[3] = 2;
	map_comtype[4] = 3;
	map_comtype[5] = 4;
	map_comtype[6] = 6;
	map_comtype[7] = 7;

	DWORD dwStyle; 
	dwStyle = m_knifelist.GetExtendedStyle();  
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES ;
	m_knifelist.SetExtendedStyle(dwStyle);


	dwStyle = m_knifetype.GetExtendedStyle();
	dwStyle = dwStyle|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES;
	m_knifetype.SetExtendedStyle(dwStyle);

	m_knifetype.InsertColumn(0,"",LVCFMT_CENTER,20);
	m_knifetype.InsertColumn(1,"机器型号",LVCFMT_LEFT,100);
	//m_knifetype.InsertColumn(2,"是否贴标",LVCFMT_LEFT,80);

	{//机器选中
		std::vector<CString> machineName = getMachineNames();
		std::vector<CString> machineUsed = getMachineUseds();
		for (int i=0;i<machineName.size();i++)
		{
			int node_item = m_knifetype.InsertItem(i,"");
			m_knifetype.SetItemText(node_item,1,machineName.at(i));
			if (machineUsed.at(i) == "1")
			{
				m_knifetype.SetCheck(i,true);
			}
			else
			{
				m_knifetype.SetCheck(i,false);
			}
		}
	}

	{//tab初始化
		m_tabChange.InsertItem(0, _T("总文件头尾"));          
		m_tabChange.InsertItem(1, _T("主轴"));
		m_tabChange.InsertItem(2,_T("排钻"));
		m_tabChange.InsertItem(3,_T("反面头尾"));
		m_tabChange.InsertItem(4,_T("小板头尾"));
		m_tabChange.SetCurSel(0);


		page1.Create(IDD_ChangeCode,&m_tabChange);
		CRect rs;  
		m_tabChange.GetClientRect(&rs);  
		//调整子对话框在父窗口中的位置，根据实际修改  
		rs.top+=23;  
		rs.bottom-=2;  
		rs.left+=3;  
		rs.right-=4;  
		//设置子对话框尺寸并移动到指定位置  
		page1.MoveWindow(&rs);  
		//分别设置隐藏和显示  
		page1.ShowWindow(true);  

		page2.Create(IDD_MAINCODE,&m_tabChange); 
		page2.MoveWindow(&rs);  
		//分别设置隐藏和显示  
		page2.ShowWindow(false);  

		page3.Create(IDD_PZCODE,&m_tabChange); 
		page3.MoveWindow(&rs);  
		//分别设置隐藏和显示  
		page3.ShowWindow(false); 

		page4.Create(IDD_BMAINCODE,&m_tabChange); 
		page4.MoveWindow(&rs);  
		//分别设置隐藏和显示  
		page4.ShowWindow(false); 

		page5.Create(IDD_SPCODE,&m_tabChange); 
		page5.MoveWindow(&rs);  
		//分别设置隐藏和显示  
		page5.ShowWindow(false); 

	}
 	{//初始化coortype 1 2 坐标系下拉框
		RECT rect;
		m_coortype.GetWindowRect(&rect);
		m_coortype.SetWindowPos(NULL,0,0,rect.right - rect.left,200,SWP_NOMOVE|SWP_NOREDRAW);
		m_coortype2.SetWindowPos(NULL,0,0,rect.right - rect.left,200,SWP_NOMOVE|SWP_NOREDRAW);

		//相当于一个特殊数组,但是它保存的是图片
		CImageList* pImageList;      
		pImageList = new CImageList();
		pImageList->Create(32, 32, ILC_COLOR32,4,4); 

		std::vector<CString> bmpPath;
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\0.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\1.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\2.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\3.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\4.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\5.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\6.bmp")));
		bmpPath.push_back(HGTools::getRealPathFromReleativePath(HGTools::getRelativePath("kdata\\7.bmp")));

		std::vector<CBitmap> bmpInstance;
		for (int bmpIndex = 0; bmpIndex < bmpPath.size() ; bmpIndex++)
		{
			CBitmap bInstance;
			bInstance.Attach((HBITMAP)::LoadImage(NULL,bmpPath[bmpIndex],IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
			//bmpInstance.push_back(bInstance);

			pImageList->Add(&bInstance,RGB(150,0,0));
		}

		m_coortype.SetImageList(pImageList);
		m_coortype2.SetImageList(pImageList);

		for(int i=0; i<pImageList->GetImageCount(); i++)
		{
			COMBOBOXEXITEM     cbi ={0};
			cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
				CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
			cbi.iItem = i;
			cbi.iImage = i;
			cbi.iSelectedImage = i;
			cbi.iOverlay = 2;
			cbi.iIndent = 0;

			
			CString str;
			str.Format(_T("%d"), i);
			cbi.pszText = (LPTSTR)(LPCTSTR)str;
			cbi.cchTextMax = str.GetLength();

			
			int nItem;
			nItem = m_coortype.InsertItem(&cbi);
			nItem = m_coortype2.InsertItem(&cbi);
			ASSERT(nItem == i);
		}
		m_coortype.SetCurSel(0);
		m_coortype2.SetCurSel(0);
	}
	{//初始化其他combox	
		m_double.InsertString(0,_T("单工位"));//单双工位设置
		m_double.InsertString(1, _T("双工位 ")); 
		m_double.SetCurSel(0); //设置选中的项
		OnCbnSelchangeCombo1();

		m_coorsysuse.InsertString(0,_T("代码偏移"));//偏移方式设置
		m_coorsysuse.InsertString(1, _T("系统偏移 ")); 
		m_coorsysuse.SetCurSel(0); //设置选中的项

		m_dvcut.InsertString(0,_T("单面"));//单双面打孔设置
		m_dvcut.InsertString(1, _T("双面1")); 
		m_dvcut.InsertString(2, _T("双面2")); 
		m_dvcut.SetCurSel(0); //设置选中的项

		m_combineSet.InsertString(0,_T("不合并"));//偏移方式设置
		m_combineSet.InsertString(1, _T("合并")); 
		m_combineSet.InsertString(2, _T("不合并出反面头尾")); 
		m_combineSet.SetCurSel(0); //设置选中的项


		// 工位1有效X使用哪条边
		m_usedLineMap["上"] = "3";
		m_usedLineMap["下"] = "1";
		m_usedLineMap["左"] = "0";
		m_usedLineMap["右"] = "2";
		m_usedLineMap["3"] = "上";
		m_usedLineMap["1"] = "下";
		m_usedLineMap["0"] = "左";
		m_usedLineMap["2"] = "右";

		m_usedLineX1.InsertString(0,"上");
		m_usedLineX1.InsertString(1,"下");

		// 工位1有效Y使用哪条边
		m_usedLineY1.InsertString(0,"左");
		m_usedLineY1.InsertString(1,"右");

		// 工位2有效X使用哪条边
		m_usedLineX2.InsertString(0,"上");
		m_usedLineX2.InsertString(1,"下");

		// 工位2有效Y使用哪条边
		m_usedLineY2.InsertString(0,"左");
		m_usedLineY2.InsertString(1,"右");
	}

	for(int i = 0; i < sizeof(g_aLabelMachineName)/sizeof(char*); i++)
		m_comboLabelMachine.AddString(g_aLabelMachineName[i]);
	m_comboLabelMachine.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//刀具 修改
void CKnifeAddDlg::EditKnifeList(int theIndex)//修改
{
	UpdateData(TRUE);

	int type_nItem = m_knifetype.GetNextItem(-1,LVIS_SELECTED);
	if (type_nItem < 0)
	{
		AfxMessageBox("请选择机器类型");
		return;
	}
	int list_nItem = m_knifelist.GetNextItem(-1,LVIS_SELECTED);
	CString knife_name = m_knifetype.GetItemText(type_nItem,1);
	std::string s_name = knife_name.GetBuffer();

	m_typename.Format("%d",map_comtype[m_comtype.GetCurSel()]);//对应刀具用途
	std::string str_TypeName = m_typename.GetBuffer();
	std::string str_Diameter = m_tooldiameter.GetBuffer();//对应刀直径
	std::string str_Length = m_toollength.GetBuffer();//对应分切界限
	std::string str_KnifeLength = m_KnifeLength.GetBuffer();//刀长
	CString str_mgcode = m_mgcode;//换刀头工位1
	CString str_mgcodee = m_mgcodee;//换刀尾工位1
	str_mgcode.Replace(_T("\r\n"),_T("@"));
	str_mgcodee.Replace(_T("\r\n"),_T("@"));

	CString str_mgcode2 = m_mgcode2;//换刀头工位2
	CString str_mgcodee2 = m_mgcodee2;//换刀尾工位2
	str_mgcode2.Replace(_T("\r\n"),_T("@"));
	str_mgcodee2.Replace(_T("\r\n"),_T("@"));

	CString mgcode2 = str_mgcode2 + "#" + str_mgcodee2;
	CString mgcode = str_mgcode + "#" + str_mgcodee;
	std::string str_ChangeCode = mgcode.GetBuffer();//换刀代码1
	std::string str_ChangeCode2 = mgcode2.GetBuffer();//换刀代码2
	std::string str_FeedRate = m_feedrate.GetBuffer();//加工速度
	std::string str_NearSpeed = m_nearspeed.GetBuffer();//接近速度
	std::string str_SpindleSpeed = m_spindlespeed.GetBuffer();//
	std::string str_EndSpeed = m_endspeech.GetBuffer();//收尾速度
	std::string str_OverSpeed = m_overspeech.GetBuffer();//空跑速度
	std::string str_kcoorx = m_coorx.GetBuffer();
	std::string str_kcoory = m_coory.GetBuffer();
	std::string str_kcoor = str_kcoorx + "," + str_kcoory;//
	std::string str_knum = m_knum.GetBuffer();
	std::string str_ksysa = m_ksysa.GetBuffer();
	std::string str_ksysb = m_ksysb.GetBuffer();
	
	//更新刀信息
	//TinyXml::XmlHandlePlus knifeTypeParm(&m_doc);
	//knifeTypeParm.changeDirectoryTo("/Knife/KnifeType",TinyXml::XmlAttrValueCompare("Name",s_name));
	//std::vector<TinyXml::TiXmlNode*> knifeTypeNodeArr = knifeTypeParm.findAll("Param/");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", s_name.c_str());
	vector<TiXmlElement*> vParam = xmlHandler.GetChildElms(elmTheKnifeType, "Param");

	//if (knifeTypeNodeArr.size()>theIndex)
	if (vParam.size()>theIndex)
	{
		//TinyXml::XmlHandlePlus nodeHandle(knifeTypeNodeArr.at(theIndex));
		TiXmlElement* elmParam = vParam[theIndex];
		elmParam->SetAttribute("Index","-");
		elmParam->SetAttribute("TypeName",str_TypeName.c_str());
		elmParam->SetAttribute("Diameter",str_Diameter.c_str());
		elmParam->SetAttribute("Length",str_Length.c_str());
		elmParam->SetAttribute("ChangeCode",str_ChangeCode.c_str());
		elmParam->SetAttribute("FeedRate",str_FeedRate.c_str());
		elmParam->SetAttribute("SpindleSpeed",str_SpindleSpeed.c_str());
		elmParam->SetAttribute("EndSpeed",str_EndSpeed.c_str());
		elmParam->SetAttribute("OverSpeed",str_OverSpeed.c_str());
		elmParam->SetAttribute("Kcoor",str_kcoor.c_str());
		elmParam->SetAttribute("KNum",str_knum.c_str());
		elmParam->SetAttribute("CoorSys",str_ksysa.c_str());
		elmParam->SetAttribute("CoorSys1",str_ksysb.c_str());
		elmParam->SetAttribute("ChangeCode1",str_ChangeCode2.c_str());
		elmParam->SetAttribute("KnifeLength",str_KnifeLength.c_str());
		//elmParam->SetAttribute("Used",str_isCheck);
		elmParam->SetAttribute("NearSpeed",str_NearSpeed.c_str());

		//nodeHandle.setAttr("Index","-");
		//nodeHandle.setAttr("TypeName",str_TypeName);
		//nodeHandle.setAttr("Diameter",str_Diameter);
		//nodeHandle.setAttr("Length",str_Length);
		//nodeHandle.setAttr("ChangeCode",str_ChangeCode);
		//nodeHandle.setAttr("FeedRate",str_FeedRate);
		//nodeHandle.setAttr("SpindleSpeed",str_SpindleSpeed);
		//nodeHandle.setAttr("EndSpeed",str_EndSpeed);
		//nodeHandle.setAttr("OverSpeed",str_OverSpeed);
		//nodeHandle.setAttr("Kcoor",str_kcoor);
		//nodeHandle.setAttr("KNum",str_knum);
		//nodeHandle.setAttr("CoorSys",str_ksysa);
		//nodeHandle.setAttr("CoorSys1",str_ksysb);
		//nodeHandle.setAttr("ChangeCode1",str_ChangeCode2);
		//nodeHandle.setAttr("KnifeLength",str_KnifeLength);
		////nodeHandle.setAttr("Used",str_isCheck);
		//nodeHandle.setAttr("NearSpeed",str_NearSpeed);
	}


}

//刀具修改 按钮
void CKnifeAddDlg::OnBtnChangeKnife()//刀库修改响应函数
{

	int t_Item2 = m_knifetype.GetNextItem(-1,LVIS_SELECTED); 
	int l_Item2 = m_knifelist.GetNextItem(-1,LVIS_SELECTED); 
	if (t_Item2>=0)
	{
		std::string machineName = m_knifetype.GetItemText(t_Item,1);

		//更新刀具使用是否选中
		//TinyXml::XmlHandlePlus knifeTypeParm(&m_doc);
		//knifeTypeParm.changeDirectoryTo("/Knife/KnifeType",TinyXml::XmlAttrValueCompare("Name",machineName));
		//std::vector<TinyXml::TiXmlNode*> knifeTypeNodeArr = knifeTypeParm.findAll("Param/");
		XmlHandler xmlHandler;
		TiXmlElement* elmRoot = m_doc.RootElement();
		TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", machineName.c_str());
		vector<TiXmlElement*> vParam = xmlHandler.GetChildElms(elmTheKnifeType, "Param");

		//for (int i = 0;i<knifeTypeNodeArr.size();i++)
		for (int i = 0;i<vParam.size();i++)
		{
			//TinyXml::XmlHandlePlus nodeHandle(knifeTypeNodeArr.at(i));
			TiXmlElement* elmParam = vParam[i];

			int ischeck1 = m_knifelist.GetCheck(i);
			char buffer[10];
			itoa(ischeck1,buffer,10);
			//nodeHandle.setAttr("Used",buffer);
			elmParam->SetAttribute("Used",buffer);
		}
	}
	if (l_Item2 < 0 || t_Item2 < 0)
	{
		AfxMessageBox("请选择修改项目");
		return;
	}
	else
	{
		//更新修改
		EditKnifeList(l_Item2);
		CString machineName = m_knifetype.GetItemText(t_Item,1);
		InputKnifeTypeParamToDialog(machineName);
	}
}

//刀具 添加 按钮
void CKnifeAddDlg::OnBtnAddKnife()
{

	UpdateData(TRUE);

	int type_nItem = m_knifetype.GetNextItem(-1,LVIS_SELECTED);
	if (type_nItem < 0)
	{
		AfxMessageBox("请选择机器类型");
		return;
	}
	CString knife_name = m_knifetype.GetItemText(type_nItem,1);
	std::string s_name = knife_name.GetBuffer();

	CString s_Index;
	s_Index.Format("%d",m_knifelist.GetItemCount());
	std::string str_Index = s_Index.GetBuffer();
	m_typename.Format("%d",map_comtype[m_comtype.GetCurSel()]);
	std::string str_TypeName = m_typename.GetBuffer();
	std::string str_Diameter = m_tooldiameter.GetBuffer();
	std::string str_Length = m_toollength.GetBuffer();
	std::string str_KnifeLength = m_KnifeLength.GetBuffer();
	CString str_mgcode = m_mgcode;
	CString str_mgcodee = m_mgcodee;
	str_mgcode.Replace(_T("\r\n"),_T("@"));
	str_mgcodee.Replace(_T("\r\n"),_T("@"));

	CString str_mgcode2 = m_mgcode2;
	CString str_mgcodee2 = m_mgcodee2;
	str_mgcode2.Replace(_T("\r\n"),_T("@"));
	str_mgcodee2.Replace(_T("\r\n"),_T("@"));

	CString mgcode2 = str_mgcode2 + "#" + str_mgcodee2;
	CString mgcode = str_mgcode + "#" + str_mgcodee;
	std::string str_ChangeCode = mgcode.GetBuffer();
	std::string str_ChangeCode2 = mgcode2.GetBuffer();
	std::string str_FeedRate = m_feedrate.GetBuffer();
	std::string str_NearSpeed = m_nearspeed.GetBuffer();
	std::string str_SpindleSpeed = m_spindlespeed.GetBuffer();

	std::string str_EndSpeed = m_endspeech.GetBuffer();
	std::string str_OverSpeed = m_overspeech.GetBuffer();
	std::string str_kcoorx = m_coorx.GetBuffer();
	std::string str_kcoory = m_coory.GetBuffer();
	std::string str_kcoor = str_kcoorx + "," + str_kcoory;
	std::string str_knum = m_knum.GetBuffer();
	std::string str_ksysa = m_ksysa.GetBuffer();
	std::string str_ksysb = m_ksysb.GetBuffer();



	//添加刀具
	//TinyXml::XmlHandlePlus knifeTypeParm(&m_doc);
	//knifeTypeParm.changeDirectoryTo("/Knife/KnifeType",TinyXml::XmlAttrValueCompare("Name",s_name));
	//std::vector<TinyXml::TiXmlNode*> knifeTypeNodeArr = knifeTypeParm.findAll("Param/");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", s_name.c_str());
	vector<TiXmlElement*> vParam = xmlHandler.GetChildElms(elmTheKnifeType, "Param");

	//算插入节点的Index
	int max_index = 0;
	//for (int i = 0;i<knifeTypeNodeArr.size();i++)
	for (int i = 0;i<vParam.size();i++)
	{
		//TinyXml::XmlHandlePlus nodeHandle(knifeTypeNodeArr.at(i));
		TiXmlElement* elmParam = vParam[i];
		//std::string node_index = nodeHandle.getAttr("Index");
		int nNodeIndex;
		xmlHandler.GetXmlAttribute(elmParam, "Index", nNodeIndex);
		if (max_index < /*atoi(node_index.c_str())*/nNodeIndex)
		{
			max_index = /*atoi(node_index.c_str())*/nNodeIndex;
		}
	}

	{//创建一个并添加到其中
		TinyXml::TiXmlElement* newElement = new TinyXml::TiXmlElement( "Param" );

		int is_check = ischeck;
		CString str_isCheck;
		str_isCheck.Format("%d",is_check);
		newElement->SetAttribute("Index","-");
		newElement->SetAttribute("TypeName",str_TypeName.c_str());
		newElement->SetAttribute("Diameter",str_Diameter.c_str());
		newElement->SetAttribute("Length",str_Length.c_str());
		newElement->SetAttribute("ChangeCode",str_ChangeCode.c_str());
		newElement->SetAttribute("FeedRate",str_FeedRate.c_str());
		newElement->SetAttribute("SpindleSpeed",str_SpindleSpeed.c_str());
		newElement->SetAttribute("EndSpeed",str_EndSpeed.c_str());
		newElement->SetAttribute("OverSpeed",str_OverSpeed.c_str());
		newElement->SetAttribute("Kcoor",str_kcoor.c_str());
		newElement->SetAttribute("KNum",str_knum.c_str());
		newElement->SetAttribute("CoorSys",str_ksysa.c_str());
		newElement->SetAttribute("CoorSys1",str_ksysb.c_str());
		newElement->SetAttribute("ChangeCode1",str_ChangeCode2.c_str());
		newElement->SetAttribute("KnifeLength",str_KnifeLength.c_str());
		newElement->SetAttribute("Used",str_isCheck.GetBuffer());
		str_isCheck.ReleaseBuffer();
		newElement->SetAttribute("NearSpeed",str_NearSpeed.c_str());
		is_check = false;
		//knifeTypeParm.ToNode()->InsertEndChild(newElement);
		elmTheKnifeType->LinkEndChild(newElement);
	}
	CString machineName = s_name.c_str();
	InputKnifeTypeParamToDialog(machineName);
}

//刀具 删除按钮
void CKnifeAddDlg::OnBtnDelKnife()//删除
{
	
	int nItem = m_knifelist.GetNextItem(-1,LVIS_SELECTED);
	int type_nItem = m_knifetype.GetNextItem(-1,LVIS_SELECTED);

	if (nItem < 0)
	{
		return;
	}
	else
	{
		//m_knifelist.DeleteItem(nItem);
		std::string s_name = m_knifetype.GetItemText(type_nItem,1);

		//找到机器
		//TinyXml::XmlHandlePlus knifeTypeHandle(m_doc);
		//knifeTypeHandle.changeDirectoryTo("/Knife/KnifeType",TinyXml::XmlAttrValueCompare("Name",s_name));
		XmlHandler xmlHandler;
		TiXmlElement* elmRoot = m_doc.RootElement();
		TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", s_name.c_str());
		vector<TiXmlElement*> vParam = xmlHandler.GetChildElms(elmTheKnifeType, "Param");

		//删除Index对应节点
		//TinyXml::XmlHandlePlus knifeTypeParm(knifeTypeHandle);
		//std::vector<TinyXml::TiXmlNode*> knifeTypeNodeArr = knifeTypeParm.findAll("Param/");

		//if (knifeTypeNodeArr.size() > nItem)
		if (vParam.size() > nItem)
		{
			//TinyXml::XmlHandlePlus nodeHandle(knifeTypeNodeArr.at(nItem));
			//nodeHandle.remove();
			TiXmlElement* elmParam = vParam[nItem];
			elmTheKnifeType->RemoveChild(elmParam);
		}

		CString machineName = s_name.c_str();
		InputKnifeTypeParamToDialog(machineName);
	}

}





void CKnifeAddDlg::OnBnClickedOk()
{
	//TinyXml::XmlHandlePlus docHandle(&m_doc);

	//std::vector<TinyXml::TiXmlNode*> arr = docHandle.findAll("/Knife/KnifeType");
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	vector<TiXmlElement*> vKnifeType = xmlHandler.GetChildElms(elmRoot, "KnifeType");

	for (int j = 0;j < m_knifetype.GetItemCount();j++)
	{
		//TinyXml::XmlHandlePlus knifenode(arr[j]);
		TiXmlElement* elmKnifeType = vKnifeType[j];


		//刀是否选中使用
		int nKnifeUsed = m_knifetype.GetCheck(j);
		char charUsed[10];
		itoa(nKnifeUsed,charUsed,10);

		//刀名称
		CString cstrMachineName = m_knifetype.GetItemText(j,2);
		std::string strMachineName = m_knifetype.GetItemText(j,1);
		
		std::string strUsed = charUsed;
		//knifenode.setAttr("Used",strUsed);
		elmKnifeType->SetAttribute("Used",strUsed.c_str());
	}
	saveSettings();
	CDialogEx::OnOK();
}




void CKnifeAddDlg::OnNMDblclkKnifelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;	
}


void CKnifeAddDlg::OnCbnSelchangeComtype()
{
}
//加载机器信息
void CKnifeAddDlg::InputMessageToDialog(CString theName)
{
	std::string machineName = theName.GetBuffer();

	//当前机器信息
	//TinyXml::XmlHandlePlus knifeTypeParm(&m_doc);
	//knifeTypeParm.changeDirectoryTo("/Knife/KnifeType",TinyXml::XmlAttrValueCompare("Name",machineName));
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", machineName.c_str());
	{
		CString strTmp;
		//std::string str_CLast = knifeTypeParm.getAttr("CLast");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "CLast", strTmp);
		std::string str_CLast = strTmp;
		//std::string str_CName = knifeTypeParm.getAttr("CName");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "CName", strTmp);
		std::string str_CName = strTmp;
		//std::string str_CoorSysUse = knifeTypeParm.getAttr("CoorSysUse");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "CoorSysUse", strTmp);
		std::string str_CoorSysUse = strTmp;
		//std::string str_CoorType = knifeTypeParm.getAttr("CoorType");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "CoorType", strTmp);
		std::string str_CoorType = strTmp;
		//std::string str_CoorType2 = knifeTypeParm.getAttr("CoorType2");//工位2的坐标系
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "CoorType2", strTmp);
		std::string str_CoorType2 = strTmp;
		//std::string str_DvCut = knifeTypeParm.getAttr("DvCut");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "DvCut", strTmp);
		std::string str_DvCut = strTmp;
		//std::string str_GroupStr = knifeTypeParm.getAttr("GroupStr");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "GroupStr", strTmp);
		std::string str_GroupStr = strTmp;
		//std::string str_PLast = knifeTypeParm.getAttr("PLast");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "PLast", strTmp);
		std::string str_PLast = strTmp;
		//std::string str_PName = knifeTypeParm.getAttr("PName");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "PName", strTmp);
		std::string str_PName = strTmp;
		//std::string str_Double = knifeTypeParm.getAttr("Double");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "Double", strTmp);
		std::string str_Double = strTmp;
		//std::string str_Print = knifeTypeParm.getAttr("Print");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "Print", strTmp);
		std::string str_Print = strTmp;
		//std::string str_Pxhx = knifeTypeParm.getAttr("Pxhx");	//P文件是否有下划线
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "Pxhx", strTmp);
		std::string str_Pxhx = strTmp;
		//std::string str_Cxhx = knifeTypeParm.getAttr("Cxhx");	//P文件是否有下划线
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "Cxhx", strTmp);
		std::string str_Cxhx = strTmp;

		if(str_Cxhx.find("1")!=std::string::npos)//C有下划线
		{		
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKXHX1));
			theCheck->SetCheck(TRUE);
		}else
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKXHX1));
			theCheck->SetCheck(FALSE);
		}
		if(str_Pxhx.find("1")!=std::string::npos)//P有下划线
		{		
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKXHX2));
			theCheck->SetCheck(TRUE);
		}else
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKXHX2));
			theCheck->SetCheck(FALSE);
		}
		if (str_CName.find("@")!=std::string::npos)//开料文件名处理
		{
			int thePos1 = str_CName.find_first_of('@');
			std::string theCname = str_CName.substr(0,thePos1);//nc文件名
			m_thecname.Format(_T("%s"),theCname.c_str());
			CButton* theMCheck = (CButton*)(GetDlgItem(IDC_CMCHECK));
			if (str_CName.find("@Material@")!=std::string::npos)
			{
				if (theMCheck)
				{
					theMCheck->SetCheck(TRUE);
				}
			}
			else
			{
				if (theMCheck)
				{
					theMCheck->SetCheck(FALSE);
				}
			}
			CButton* theTHCheck = (CButton*)(GetDlgItem(IDC_CTHCHECK));
			if (str_CName.find("@Thickness@")!=std::string::npos)
			{
				if (theTHCheck)
				{
					theTHCheck->SetCheck(TRUE);
				}
			}
			else
			{
				if (theTHCheck)
				{
					theTHCheck->SetCheck(FALSE);
				}
			}
		}
		m_theclast.Format(_T("%s"),str_CLast.c_str());	
		m_theplast.Format(_T("%s"),str_PLast.c_str());
		if (str_PName.find("@")!=std::string::npos)//贴标文件名处理
		{
			int thePos1 = str_PName.find_first_of('@');
			std::string thePname = str_PName.substr(0,thePos1);//nc文件名
			m_thepname.Format(_T("%s"),thePname.c_str());
			CButton* theMCheck = (CButton*)(GetDlgItem(IDC_PMCHECK));
			if (str_PName.find("@Material@")!=std::string::npos)
			{
				if (theMCheck)
				{
					theMCheck->SetCheck(TRUE);
				}
			}
			else
			{
				if (theMCheck)
				{
					theMCheck->SetCheck(FALSE);
				}
			}
			CButton* theTHCheck = (CButton*)(GetDlgItem(IDC_PTHCHECK));
			if (str_PName.find("@Thickness@")!=std::string::npos)
			{
				if (theTHCheck)
				{
					theTHCheck->SetCheck(TRUE);
				}
			}
			else
			{
				if (theTHCheck)
				{
					theTHCheck->SetCheck(FALSE);
				}
			}
		}
		m_groupstr.Format(_T("%s"),str_GroupStr.c_str());
		//m_autop.Format(_T("%s"),str_Print.c_str());
		int nLabelMachineID = atoi(str_Print.c_str());
		if(nLabelMachineID < 0 || nLabelMachineID >= sizeof(g_aLabelMachineName)/sizeof(char*))
			nLabelMachineID = 0;
		m_comboLabelMachine.SetCurSel(nLabelMachineID);

		int i_coorsysuse = atoi(str_CoorSysUse.c_str());
		int i_coortype  = atoi(str_CoorType.c_str());
		int i_coortype2  = atoi(str_CoorType2.c_str());
		int i_dvcut     = atoi(str_DvCut.c_str());
		int i_double    = atoi(str_Double.c_str());


		m_coorsysuse.SetCurSel(i_coorsysuse);
		m_coortype.SetCurSel(i_coortype);
		m_coortype2.SetCurSel(i_coortype2);
		m_dvcut.SetCurSel(i_dvcut);
		m_double.SetCurSel(i_double);
	}

	//当前机器信息
	//knifeTypeParm.changeDirectoryTo("ParamList/");
	TiXmlElement* elmParamList = xmlHandler.GetChildElm(elmTheKnifeType, "ParamList");
	{
		//hg3d::CXmlNode* ParamNode = ParamListNode->GetChildNodes().at(k);
		//if (ParamNode->GetAttributeValue("Name") == "GFirst")//主文件文件头
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("GFirst");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GFirst");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString firstname;
			firstname.Format(_T("%s"),theName.c_str());
			page1.ChangeFirst1 = firstname.Left(firstname.Find('#'));
			page1.ChangeFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
			page1.ChangeFirst1.Replace("@","\r\n");
			page1.ChangeFirst2.Replace("@","\r\n");
			page1.ChangeFirst1.Replace("~","#");
			page1.ChangeFirst2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "GLast")//主文件文件尾
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("GLast");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GLast");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString lastname;
			lastname.Format(_T("%s"),theName.c_str());
			page1.ChangeLast1 = lastname.Left(lastname.Find('#'));
			page1.ChangeLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
			page1.ChangeLast1.Replace("@","\r\n");
			page1.ChangeLast2.Replace("@","\r\n");
			page1.ChangeLast1.Replace("~","#");
			page1.ChangeLast2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "GBFirst")//反面主文件文件头
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("GBFirst");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GBFirst");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString firstname;
			firstname.Format(_T("%s"),theName.c_str());
			page4.BMainFirst1 = firstname.Left(firstname.Find('#'));
			page4.BMainFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
			page4.BMainFirst1.Replace("@","\r\n");
			page4.BMainFirst2.Replace("@","\r\n");
			page4.BMainFirst1.Replace("~","#");
			page4.BMainFirst2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "GBLast")//反面主文件文件尾
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("GBLast");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GBLast");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString lastname;
			lastname.Format(_T("%s"),theName.c_str());
			page4.BMainLast1 = lastname.Left(lastname.Find('#'));
			page4.BMainLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
			page4.BMainLast1.Replace("@","\r\n");
			page4.BMainLast2.Replace("@","\r\n");
			page4.BMainLast1.Replace("~","#");
			page4.BMainLast2.Replace("~","#");
		}

		//if (ParamNode->GetAttributeValue("Name") == "SPFirst")//反面主文件文件头
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("SPFirst");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SPFirst");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString firstname;
			firstname.Format(_T("%s"),theName.c_str());
			page5.SpFirst1 = firstname.Left(firstname.Find('#'));
			page5.SpFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
			page5.SpFirst1.Replace("@","\r\n");
			page5.SpFirst2.Replace("@","\r\n");
			page5.SpFirst1.Replace("~","#");
			page5.SpFirst2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "SPLast")//反面主文件文件尾
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("SPLast");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SPLast");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString lastname;
			lastname.Format(_T("%s"),theName.c_str());
			page5.SpLast1 = lastname.Left(lastname.Find('#'));
			page5.SpLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
			page5.SpLast1.Replace("@","\r\n");
			page5.SpLast2.Replace("@","\r\n");
			page5.SpLast1.Replace("~","#");
			page5.SpLast2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "MFirst")//主轴文件头
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("MFirst");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "MFirst");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString firstname;
			firstname.Format(_T("%s"),theName.c_str());
			page2.MainFirst1 = firstname.Left(firstname.Find('#'));
			page2.MainFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
			page2.MainFirst1.Replace("@","\r\n");
			page2.MainFirst2.Replace("@","\r\n");
			page2.MainFirst1.Replace("~","#");
			page2.MainFirst2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "MLast")//主轴文件尾
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("MLast");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "MLast");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString lastname;
			lastname.Format(_T("%s"),theName.c_str());
			page2.MainLast1  = lastname.Left(lastname.Find('#'));
			page2.MainLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
			page2.MainLast1.Replace("@","\r\n");
			page2.MainLast2.Replace("@","\r\n");
			page2.MainLast1.Replace("~","#");
			page2.MainLast2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "PZFirst")//排鉆文件头
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("PZFirst");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "PZFirst");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString firstname;
			firstname.Format(_T("%s"),theName.c_str());
			page3.PzFirst1 = firstname.Left(firstname.Find('#'));
			page3.PzFirst2 = firstname.Right(firstname.GetLength() - firstname.Find('#') - 1);
			page3.PzFirst1.Replace("@","\r\n");
			page3.PzFirst2.Replace("@","\r\n");
			page3.PzFirst1.Replace("~","#");
			page3.PzFirst2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "PZLast")//排鉆文件尾
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("PZLast");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "PZLast");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			CString lastname;
			lastname.Format(_T("%s"),theName.c_str());
			page3.PzLast1  = lastname.Left(lastname.Find('#'));
			page3.PzLast2 = lastname.Right(lastname.GetLength() - lastname.Find('#') - 1);
			page3.PzLast1.Replace("@","\r\n");
			page3.PzLast2.Replace("@","\r\n");
			page3.PzLast1.Replace("~","#");
			page3.PzLast2.Replace("~","#");
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim1")//工位1修边1
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("BTrim1");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim1");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_btrim1.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF1));
			if (m_btrim1.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_btrim1.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim2")//工位1修边2
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("BTrim2");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim2");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_btrim2.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF2));
			if (m_btrim2.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_btrim2.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim3")//工位1修边3
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("BTrim3");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim3");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_btrim3.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF3));
			if (m_btrim3.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_btrim3.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim4")//工位1修边4
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("BTrim4");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim4");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_btrim4.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF4));
			if (m_btrim4.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_btrim4.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim1")//工位2修边1
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("FTrim1");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim1");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_ftrim1.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF1));
			if (m_ftrim1.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_ftrim1.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim2")//工位2修边2
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("FTrim2");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim2");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_ftrim2.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF2));
			if (m_ftrim2.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_ftrim2.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim3")//工位2修边3
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("FTrim3");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim3");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_ftrim3.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF3));
			if (m_ftrim3.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_ftrim3.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim4")//工位2修边4
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("FTrim4");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim4");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;

			m_ftrim4.Format(_T("%s"),theName.c_str());
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF4));
			if (m_ftrim4.Find('-')!=-1)//负
			{ 
				if (theCheck)
				{
					theCheck->SetCheck(TRUE);
				}
				m_ftrim4.Remove('-');
			}
			else//正
			{
				if (theCheck)
				{
					theCheck->SetCheck(FALSE);
				}
			}
		}

		//if (ParamNode->GetAttributeValue("Name") == "BUsedLineX")//用户设置有效边，用于修边偏移（工位1X）
		{
			//setSelectText(m_usedLineX1,m_usedLineMap.at(knifeTypeParm.getParam("BUsedLineX","1")));
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BUsedLineX");
			CString strTmp = "1";
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			string strKey = strTmp;
			setSelectText(m_usedLineX1,m_usedLineMap.at(strKey));
		}
		//if (ParamNode->GetAttributeValue("Name") == "BUsedLineY")//用户设置有效边，用于修边偏移（工位1Y）
		{
			//setSelectText(m_usedLineY1,m_usedLineMap.at(knifeTypeParm.getParam("BUsedLineY","0")));
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BUsedLineY");
			CString strTmp = "0";
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			string strKey = strTmp;
			setSelectText(m_usedLineY1,m_usedLineMap.at(strKey));
		}
		//if (ParamNode->GetAttributeValue("Name") == "FUsedLineX")//用户设置有效边，用于修边偏移（工位2X）
		{
			//setSelectText(m_usedLineX2,m_usedLineMap.at(knifeTypeParm.getParam("FUsedLineX","1")));
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FUsedLineX");
			CString strTmp = "1";
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			string strKey = strTmp;
			setSelectText(m_usedLineX2,m_usedLineMap.at(strKey));
		}
		//if (ParamNode->GetAttributeValue("Name") == "FUsedLineY")//用户设置有效边，用于修边偏移（工位2Y）
		{
			//setSelectText(m_usedLineY2,m_usedLineMap.at(knifeTypeParm.getParam("FUsedLineY","0")));
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FUsedLineY");
			CString strTmp = "0";
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			string strKey = strTmp;
			setSelectText(m_usedLineY2,m_usedLineMap.at(strKey));
		}
		//if (ParamNode->GetAttributeValue("Name") == "EachNCLineExtra")//每行NC代码结尾追加
		{
			//std::string theName = knifeTypeParm.getParam("EachNCLineExtra");
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "EachNCLineExtra");
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", strTmp);
			std::string theName = strTmp;
			m_EachNCLineExtra = HGTools::Convert2CString(theName);
		}



		//if (ParamNode->GetAttributeValue("Name") == "IsCobine")//是否将正面文件头和反面文件头合并
		{
			//std::string theName = knifeTypeParm.getParam("IsCobine");
			//int check1 = atoi(theName.c_str());
			int check1;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsCobine");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_combineSet.SetCurSel(check1);
		}
		//if (ParamNode->GetAttributeValue("Name") == "IsComLine")//是否将NC文件连接起来
		{
			//std::string theName = knifeTypeParm.getParam("IsComLine");
			//int check1 = atoi(theName.c_str());
			int check1;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsComLine");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_Iscomline.SetCheck(check1);
		}
		//if (ParamNode->GetAttributeValue("Name") == "IsComBAll")//是否将NC文件连接起来
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			/*std::string theName = knifeTypeParm.getParam("IsComBAll");
			int check1 = atoi(theName.c_str());*/
			int check1;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsComBAll");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_iscomball.SetCheck(check1);
		}
		//if (ParamNode->GetAttributeValue("Name") == "LessKnifeChange")//最少换刀设置
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			/*std::string theName = knifeTypeParm.getParam("LessKnifeChange");
			int check1 = atoi(theName.c_str());*/
			int check1;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "LessKnifeChange");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_lessKnifeChange.SetCheck(check1);
		}

		//if (ParamNode->GetAttributeValue("Name") == "SmallPanelNC")	//小板NC文件导出
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			//std::string theName = knifeTypeParm.getParam("SmallPanelNC","0");
			//int check1 = atoi(theName.c_str());
			int check1 = 0;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SmallPanelNC");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_smallPanelNC = check1;
		}
		//if (ParamNode->GetAttributeValue("Name") == "FileClassifyMove")	// 生成NC、贴标文件归整
		{
			//std::string theName = knifeTypeParm.getParam("FileClassifyMove","0");
			//int check1 = atoi(theName.c_str());
			int check1 = 0;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FileClassifyMove");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_fileClassifyMove = check1;
		}
		//if (ParamNode->GetAttributeValue("Name") == "NC_CommandChange")	// NC命令字变更
		{
			//std::string theName = knifeTypeParm.getParam("NC_CommandChange","0");
			//int check1 = atoi(theName.c_str());
			int check1 = 0;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_CommandChange");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_NC_CommandChange = check1;
		}
		//if (ParamNode->GetAttributeValue("Name") == "NC_SpaceRemove")	// NC坐标清除
		{
			//std::string theName = knifeTypeParm.getParam("NC_SpaceRemove","0");
			//int check1 = atoi(theName.c_str());
			int check1 = 0;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_SpaceRemove");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_NC_SpaceRemove = check1;
		}
		{
			int check1 = 0;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_ModifyArcMillingFormat");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", check1);
			m_bModifyArcMillingFormat = check1;
		}
		{
			bool bCheck = false;
			TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "LabelSide");
			xmlHandler.GetXmlAttribute(elmTheParam, "Value", bCheck);
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECK_REVERSE_LABEL));
			theCheck->SetCheck(bCheck);
		}
	}

	page1.UpdateData(FALSE);
    page2.UpdateData(FALSE);
	page3.UpdateData(FALSE);
	page4.UpdateData(FALSE);
	page5.UpdateData(FALSE);


	//当前工位是单工位还是双工位
	OnCbnSelchangeCombo1();
	UpdateData(FALSE);
}
//点选切换机器
void CKnifeAddDlg::OnNMClickKnifetype(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	int pos = m_knifetype.GetNextItem(-1,LVIS_SELECTED); 
	t_Item = pos;
	CString str_name = m_knifetype.GetItemText(pos,1);
	//加载机器信息
	InputMessageToDialog(str_name);
	//加载刀库信息
	InputKnifeTypeParamToDialog(str_name);

	
	*pResult = 0;
}

//把文件中配置信息的刀库信息加载到对话框
void CKnifeAddDlg::InputKnifeTypeParamToDialog( CString &str_name )
{
	//当前机器信息
	//TinyXml::XmlHandlePlus knifeTypeParm(&m_doc);
	//knifeTypeParm.changeDirectoryTo("/Knife/KnifeType",TinyXml::XmlAttrValueCompare("Name",str_name.GetBuffer()));
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", str_name.GetBuffer());
	
	//加载刀具
	m_knifelist.DeleteAllItems();
	//std::vector<TinyXml::TiXmlNode*> knifeTypeNodeArr = knifeTypeParm.findAll("Param/");
	vector<TiXmlElement*> vParam = xmlHandler.GetChildElms(elmTheKnifeType, "Param");
	//for (int i = 0;i < knifeTypeNodeArr.size();i++)
	for (int i = 0;i < vParam.size();i++)
	{
		//TinyXml::XmlHandlePlus knifeTypeParm(knifeTypeNodeArr.at(i));
		TiXmlElement* elmTheParam = vParam[i];

		CString strTmp;
		//std::string str_Index = knifeTypeParm.getAttr("Index");
		xmlHandler.GetXmlAttribute(elmTheParam, "Index", strTmp);
		std::string str_Index = strTmp;
		//std::string str_TypeName = knifeTypeParm.getAttr("TypeName");
		xmlHandler.GetXmlAttribute(elmTheParam, "TypeName", strTmp);
		std::string str_TypeName = strTmp;
		//std::string str_Diameter = knifeTypeParm.getAttr("Diameter");
		xmlHandler.GetXmlAttribute(elmTheParam, "Diameter", strTmp);
		std::string str_Diameter = strTmp;
		//std::string str_Length = knifeTypeParm.getAttr("Length");
		xmlHandler.GetXmlAttribute(elmTheParam, "Length", strTmp);
		std::string str_Length = strTmp;
		//std::string str_ChangeCode = knifeTypeParm.getAttr("ChangeCode");
		xmlHandler.GetXmlAttribute(elmTheParam, "ChangeCode", strTmp);
		std::string str_ChangeCode = strTmp;
		//std::string str_FeedRate = knifeTypeParm.getAttr("FeedRate");
		xmlHandler.GetXmlAttribute(elmTheParam, "FeedRate", strTmp);
		std::string str_FeedRate = strTmp;
		//std::string str_SpindleSpeed = knifeTypeParm.getAttr("SpindleSpeed");
		xmlHandler.GetXmlAttribute(elmTheParam, "SpindleSpeed", strTmp);
		std::string str_SpindleSpeed = strTmp;
		//std::string str_EndSpeed = knifeTypeParm.getAttr("EndSpeed");
		xmlHandler.GetXmlAttribute(elmTheParam, "EndSpeed", strTmp);
		std::string str_EndSpeed = strTmp;
		//std::string str_OverSpeed = knifeTypeParm.getAttr("OverSpeed");
		xmlHandler.GetXmlAttribute(elmTheParam, "OverSpeed", strTmp);
		std::string str_OverSpeed = strTmp;
		//std::string str_kcoor = knifeTypeParm.getAttr("Kcoor");
		xmlHandler.GetXmlAttribute(elmTheParam, "Kcoor", strTmp);
		std::string str_kcoor = strTmp;
		//std::string str_knum = knifeTypeParm.getAttr("KNum");
		xmlHandler.GetXmlAttribute(elmTheParam, "KNum", strTmp);
		std::string str_knum = strTmp;
		//std::string str_ksysa = knifeTypeParm.getAttr("CoorSys");
		xmlHandler.GetXmlAttribute(elmTheParam, "CoorSys", strTmp);
		std::string str_ksysa = strTmp;
		//std::string str_ksysb = knifeTypeParm.getAttr("CoorSys1");
		xmlHandler.GetXmlAttribute(elmTheParam, "CoorSys1", strTmp);
		std::string str_ksysb = strTmp;
		//std::string str_ChangeCode2 = knifeTypeParm.getAttr("ChangeCode1");
		xmlHandler.GetXmlAttribute(elmTheParam, "ChangeCode1", strTmp);
		std::string str_ChangeCode2 = strTmp;
		//std::string str_KnifeLength = knifeTypeParm.getAttr("KnifeLength");
		xmlHandler.GetXmlAttribute(elmTheParam, "KnifeLength", strTmp);
		std::string str_KnifeLength = strTmp;
		//std::string str_NearSpeed = knifeTypeParm.getAttr("NearSpeed");
		xmlHandler.GetXmlAttribute(elmTheParam, "NearSpeed", strTmp);
		std::string str_NearSpeed = strTmp;
		//std::string str_used = knifeTypeParm.getAttr("Used");
		xmlHandler.GetXmlAttribute(elmTheParam, "Used", strTmp);
		std::string str_used = strTmp;


		//int nItem = m_knifelist.InsertItem(atoi(str_Index.c_str())-1, (LPCTSTR)(str_TypeName.c_str()));
		int nItem = m_knifelist.InsertItem(i, (LPCTSTR)(str_TypeName.c_str()));
		m_knifelist.SetItemText(nItem,10,(LPCTSTR)(str_TypeName.c_str()));
		m_knifelist.SetItemText(nItem,2,(LPCTSTR)(str_Diameter.c_str()));
		m_knifelist.SetItemText(nItem,3,(LPCTSTR)(str_Length.c_str()));
		m_knifelist.SetItemText(nItem,4,(LPCTSTR)(str_ChangeCode.c_str()));
		m_knifelist.SetItemText(nItem,5,(LPCTSTR)(str_FeedRate.c_str()));
		m_knifelist.SetItemText(nItem,6,(LPCTSTR)(str_SpindleSpeed.c_str()));
		m_knifelist.SetItemText(nItem,7,(LPCTSTR)(str_EndSpeed.c_str()));
		m_knifelist.SetItemText(nItem,8,(LPCTSTR)(str_OverSpeed.c_str()));
		m_knifelist.SetItemText(nItem,9,(LPCTSTR)(str_kcoor.c_str()));
		m_knifelist.SetItemText(nItem,1,(LPCTSTR)(str_knum.c_str()));
		m_knifelist.SetItemText(nItem,11,(LPCTSTR)(str_ksysa.c_str()));
		m_knifelist.SetItemText(nItem,12,(LPCTSTR)(str_ksysb.c_str()));
		m_knifelist.SetItemText(nItem,13,(LPCTSTR)(str_ChangeCode2.c_str()));
		m_knifelist.SetItemText(nItem,14,(LPCTSTR)(str_KnifeLength.c_str()));
		m_knifelist.SetItemText(nItem,15,(LPCTSTR)(str_NearSpeed.c_str()));
		if (str_used=="1")
		{
			m_knifelist.SetCheck(i,true);
		}
		else
			m_knifelist.SetCheck(i,false);
	}

}




void CKnifeAddDlg::OnNMDblclkKnifetype(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nItem = m_knifetype.GetNextItem(-1, LVNI_SELECTED);
	if (nItem < 0)
	{
		return;
	}
	else
	{
		if (m_knifetype.GetItemText(nItem, 2) == "是")
		{
			m_knifetype.SetItemText(nItem,2,"否");
		}
		else
		{
			m_knifetype.SetItemText(nItem,2,"是");
		}
	}
	*pResult = 0;
}


void CKnifeAddDlg::OnTcnSelchangingTabchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	*pResult = 0;  
}

//切换文件头尾
void CKnifeAddDlg::OnTcnSelchangeTabchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	int CurSel = m_tabChange.GetCurSel();  
	switch(CurSel)  
	{  
	case 0:  
		page1.ShowWindow(true); 
		page2.ShowWindow(false);
		page3.ShowWindow(false);
		page4.ShowWindow(false);
		page5.ShowWindow(false);
		break;  
	case 1:  
		page1.ShowWindow(false);  
		page2.ShowWindow(true);
		page3.ShowWindow(false);
		page4.ShowWindow(false);
		page5.ShowWindow(false);
		break;  
	case 2:  
		page1.ShowWindow(false);  
		page2.ShowWindow(false);
		page3.ShowWindow(true);
		page4.ShowWindow(false);
		page5.ShowWindow(false);
		break;  
	case 3:  
		page1.ShowWindow(false);  
		page2.ShowWindow(false);
		page3.ShowWindow(false);
		page4.ShowWindow(true);
		page5.ShowWindow(false);
		break;  
	case 4:  
		page1.ShowWindow(false);  
		page2.ShowWindow(false);
		page3.ShowWindow(false);
		page4.ShowWindow(false);
		page5.ShowWindow(true);
		break;  
	default: ;  
	}  
	*pResult = 0;
}


//切换刀具
void CKnifeAddDlg::OnNMClickKnifelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	UpdateData(TRUE);
	int pos = m_knifelist.GetNextItem(-1,LVIS_SELECTED);
	int nItem = m_knifelist.GetNextItem(-1,LVIS_SELECTED);
	int type_Item = m_knifetype.GetNextItem(-1,LVIS_SELECTED);
	std::string s_name = m_knifetype.GetItemText(type_Item,1);

	if (nItem < 0)
	{
		return;
	}
	else
	{
		CString str_TypeName = m_knifelist.GetItemText(nItem,10);
		CString str_Diameter = m_knifelist.GetItemText(nItem,2);
		CString str_Length = m_knifelist.GetItemText(nItem,3);
		CString str_ChangeCode = m_knifelist.GetItemText(nItem,4);
		CString str_FeedRate = m_knifelist.GetItemText(nItem,5);
		CString str_SpindleSpeed = m_knifelist.GetItemText(nItem,6);
		CString str_EndSpeed = m_knifelist.GetItemText(nItem,7);
		CString str_OverSpeed = m_knifelist.GetItemText(nItem,8);
		CString str_kcoor = m_knifelist.GetItemText(nItem,9);
		CString str_knum = m_knifelist.GetItemText(nItem,1);
		CString str_ksysa = m_knifelist.GetItemText(nItem,11);
		CString str_ksysb = m_knifelist.GetItemText(nItem,12);
		CString str_ChangeCode2 = m_knifelist.GetItemText(nItem,13);
		CString str_KinfeLength = m_knifelist.GetItemText(nItem,14);
		CString str_NearSpeed = m_knifelist.GetItemText(nItem,15);

		std::string s_typename = str_TypeName.GetBuffer();
		int i_typename = 0;
		for (auto iter_map = map_comtype.begin();iter_map != map_comtype.end();iter_map++)
		{
			if (iter_map->second == atoi(s_typename.c_str()))
			{
				i_typename = iter_map->first;
				break;
			}
		}

		CString str_beginChangeCode,str_endChangeCode;
		str_beginChangeCode = str_ChangeCode.Left(str_ChangeCode.Find('#'));
		str_endChangeCode = str_ChangeCode.Right(str_ChangeCode.GetLength() - str_ChangeCode.Find('#') - 1);
		str_beginChangeCode.Replace("@","\r\n");
		str_endChangeCode.Replace("@","\r\n");

		CString str_beginChangeCode2,str_endChangeCode2;
		str_beginChangeCode2 = str_ChangeCode2.Left(str_ChangeCode2.Find('#'));
		str_endChangeCode2 = str_ChangeCode2.Right(str_ChangeCode2.GetLength() - str_ChangeCode2.Find('#') - 1);
		str_beginChangeCode2.Replace("@","\r\n");
		str_endChangeCode2.Replace("@","\r\n");

		CString str_coorx,str_coory;
		str_coorx = str_kcoor.Left(str_kcoor.Find(','));
		str_coory = str_kcoor.Right(str_kcoor.GetLength() - str_kcoor.Find(',') - 1);


		m_typename.Format("%d",i_typename);
		m_comtype.SetCurSel(i_typename);
		m_coorx = str_coorx;
		m_coory = str_coory;
		m_knum = str_knum;
		m_tooldiameter = str_Diameter;
		m_toollength = str_Length;
		m_spindlespeed = str_SpindleSpeed;
		m_feedrate = str_FeedRate;
		m_nearspeed = str_NearSpeed;
		m_endspeech = str_EndSpeed;
		m_overspeech = str_OverSpeed;
		m_mgcode = str_beginChangeCode;
		m_mgcodee = str_endChangeCode;
		m_ksysa = str_ksysa;
		m_ksysb = str_ksysb;
		m_KnifeLength = str_KinfeLength;//刀具长度

		m_mgcode2 = str_beginChangeCode2;
		m_mgcodee2 = str_endChangeCode2;

		l_Item = nItem;
		t_Item = type_Item;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

//机器参数保存
void CKnifeAddDlg::OnBtnMachineSettingSave()
{
	
	UpdateData(TRUE);//将界面数据刷到参数中
	page1.UpdateData(TRUE);
	page2.UpdateData(TRUE);
	page3.UpdateData(TRUE);
	page4.UpdateData(TRUE);
	page5.UpdateData(TRUE);
	int pos = m_knifetype.GetNextItem(-1,LVIS_SELECTED); //机器类型位置
	CString str_name = m_knifetype.GetItemText(pos,1);
	std::string thetypeName;
	thetypeName = str_name.GetBuffer();

	//当前机器信息
	//TinyXml::XmlHandlePlus knifeTypeParm(&m_doc);
	//std::vector<TinyXml::TiXmlNode*> arr = knifeTypeParm.findAll("/Knife/KnifeType/",TinyXml::XmlAttrValueCompare("Name",thetypeName));
	XmlHandler xmlHandler;
	TiXmlElement* elmRoot = m_doc.RootElement();
	TiXmlElement* elmTheKnifeType = xmlHandler.GetIndicatedElement(elmRoot, "KnifeType", "Name", thetypeName.c_str());

	//if (arr.size() == 0)
	if (elmTheKnifeType == NULL)
	{
		return;
	}

	{
		//knifeTypeParm.changeDirectoryTo("/Knife/KnifeType/",TinyXml::XmlAttrValueCompare("Name",thetypeName));

		std::string theMaterial = "@Material@";
		std::string theThickness = "@Thickness@";

		CString strTmp;
		//std::string thePrint = knifeTypeParm.getAttr("Print");
		xmlHandler.GetXmlAttribute(elmTheKnifeType, "Print", strTmp);
		std::string thePrint = strTmp;
		//knifeTypeParm.setAttr("CLast",m_theclast.GetBuffer());//"CLast"
		elmTheKnifeType->SetAttribute("CLast", m_theclast.GetBuffer());
		//knifeTypeParm.setAttr("PLast",m_theplast.GetBuffer());//"PLast"
		elmTheKnifeType->SetAttribute("PLast",m_theplast.GetBuffer());
		std::string theCname = m_thecname.GetBuffer();//"CName"
		CButton* theMCheck = (CButton*)(GetDlgItem(IDC_CMCHECK));
		CButton* theTHCheck = (CButton*)(GetDlgItem(IDC_CTHCHECK));
		CButton* theCXCheck = (CButton*)(GetDlgItem(IDC_CHECKXHX1));
		CButton* thePXCheck = (CButton*)(GetDlgItem(IDC_CHECKXHX2));
		if(theCXCheck->GetCheck())//有下划线
		{
			//knifeTypeParm.setAttr("Cxhx","1");//"Cxhx"
			elmTheKnifeType->SetAttribute("Cxhx","1");//"Cxhx"
		}
		else
		{
			//knifeTypeParm.setAttr("Cxhx","0");//"Cxhx"
			elmTheKnifeType->SetAttribute("Cxhx","0");//"Cxhx"
		}
		if(thePXCheck->GetCheck())//有下划线
		{
			//knifeTypeParm.setAttr("Pxhx","1");//"Cxhx"
			elmTheKnifeType->SetAttribute("Pxhx","1");//"Cxhx"
		}
		else
		{
			//knifeTypeParm.setAttr("Pxhx","0");//"Cxhx"
			elmTheKnifeType->SetAttribute("Pxhx","0");//"Cxhx"
		}
		if (theMCheck->GetCheck())//使用材质,CName
		{
			if (theTHCheck->GetCheck())//使用板厚
			{
				theCname = theCname + theMaterial + theThickness;        
			}
			else//不用板厚
			{
				theCname = theCname + theMaterial;
			}

		}
		else
		{
			if (theTHCheck->GetCheck())//使用板厚
			{
				theCname = theCname + theThickness;
			}
			else
			{
				theCname = theCname;
			}
		}
		//knifeTypeParm.setAttr("CName",theCname);
		elmTheKnifeType->SetAttribute("CName",theCname.c_str());
		theMCheck = (CButton*)(GetDlgItem(IDC_PMCHECK));
		theTHCheck = (CButton*)(GetDlgItem(IDC_PTHCHECK));
		std::string thePname = m_thepname.GetBuffer();//"PName"
		if (theMCheck->GetCheck())//使用材质,CName
		{
			if (theTHCheck->GetCheck())//使用板厚
			{
				thePname = thePname + theMaterial + theThickness;        
			}
			else//不用板厚
			{
				thePname = thePname + theMaterial;
			}

		}
		else
		{
			if (theTHCheck->GetCheck())//使用板厚
			{
				thePname = thePname + theThickness;
			}
			else
			{
				thePname = thePname;
			}
		}
		//knifeTypeParm.setAttr("PName",thePname);
		elmTheKnifeType->SetAttribute("PName",thePname.c_str());

		int i_coorsysuse = m_coorsysuse.GetCurSel();
		int i_coortype  = m_coortype.GetCurSel();
		int i_coortype2 = m_coortype2.GetCurSel();
		int i_dvcut     = m_dvcut.GetCurSel();
		int i_double    = m_double.GetCurSel();

		CString C_coorsysuse;
		CString C_coortype;
		CString C_coortype2;
		CString C_dvcut;
		CString C_double;
		C_coorsysuse.Format(_T("%d"),i_coorsysuse);
		C_coortype.Format(_T("%d"),i_coortype);
		C_coortype2.Format(_T("%d"),i_coortype2);
		C_dvcut.Format(_T("%d"),i_dvcut);
		C_double.Format(_T("%d"),i_double);

		//knifeTypeParm.setAttr("CoorSysUse",C_coorsysuse.GetBuffer());//CoorSysUse
		elmTheKnifeType->SetAttribute("CoorSysUse",C_coorsysuse.GetBuffer());//CoorSysUse
		//knifeTypeParm.setAttr("CoorType",C_coortype.GetBuffer());//CoorType
		elmTheKnifeType->SetAttribute("CoorType",C_coortype.GetBuffer());//CoorType
		//knifeTypeParm.setAttr("CoorType2",C_coortype2.GetBuffer());//CoorType
		elmTheKnifeType->SetAttribute("CoorType2",C_coortype2.GetBuffer());//CoorType
		//knifeTypeParm.setAttr("DvCut",C_dvcut.GetBuffer());//DvCut
		elmTheKnifeType->SetAttribute("DvCut",C_dvcut.GetBuffer());//DvCut
		//knifeTypeParm.setAttr("Double",C_double.GetBuffer());//Double
		elmTheKnifeType->SetAttribute("Double",C_double.GetBuffer());//Double
		//knifeTypeParm.setAttr("GroupStr",m_groupstr.GetBuffer());//"GroupStr"
		elmTheKnifeType->SetAttribute("GroupStr",m_groupstr.GetBuffer());//"GroupStr"
		//knifeTypeParm.setAttr("Print",m_autop.GetBuffer());//"Print"
		//elmTheKnifeType->SetAttribute("Print",m_autop.GetBuffer());//"Print"
		elmTheKnifeType->SetAttribute("Print", m_comboLabelMachine.GetCurSel());//"Print"
		//std::string str_GroupStr = TypeNode->GetAttributeValue("GroupStr");		
		//m_groupstr.Format(_T("%s"),str_GroupStr.c_str());
	}

	//更新机器信息
	{
		//knifeTypeParm.changeDirectoryTo("ParamList/");
		TiXmlElement* elmParamList = xmlHandler.GetChildElm(elmTheKnifeType, "ParamList");
		{
			std::string firstname;
			std::string first1,first2;
			page1.ChangeFirst1.Replace("\r\n","@");
			page1.ChangeFirst2.Replace("\r\n","@");
			page1.ChangeFirst1.Replace("#","~");
			page1.ChangeFirst2.Replace("#","~");
			first1 = page1.ChangeFirst1.GetBuffer();
			first2 = page1.ChangeFirst2.GetBuffer();
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			firstname = first1 + "#" + first2;
			//ParamNode->SetAttributeValue("Value",firstname);
			//knifeTypeParm.setParam("GFirst",firstname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "GFirst", "Value", firstname.c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GFirst");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", firstname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "GLast")//主文件文件尾
		{
			std::string lastname;
			std::string last1,last2;
			page1.ChangeLast1.Replace("\r\n","@");
			page1.ChangeLast2.Replace("\r\n","@");
			page1.ChangeLast1.Replace("#","~");
			page1.ChangeLast2.Replace("#","~");
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			last1 = page1.ChangeLast1.GetBuffer();
			last2 = page1.ChangeLast2.GetBuffer();
			lastname = last1 + "#" + last2;
			//ParamNode->SetAttributeValue("Value",lastname);
			//knifeTypeParm.setParam("GLast",lastname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "GLast", "Value", lastname.c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GLast");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", lastname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "GBFirst")//反面主文件文件头
		{
			std::string firstname;
			std::string first1,first2;
			page4.BMainFirst1.Replace("\r\n","@");
			page4.BMainFirst2.Replace("\r\n","@");
			page4.BMainFirst1.Replace("#","~");
			page4.BMainFirst2.Replace("#","~");
			first1 = page4.BMainFirst1.GetBuffer();
			first2 = page4.BMainFirst2.GetBuffer();
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			firstname = first1 + "#" + first2;
			//ParamNode->SetAttributeValue("Value",firstname);
			//knifeTypeParm.setParam("GBFirst",firstname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "GBFirst", "Value", firstname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GBFirst");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", firstname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "GBLast")//反面主文件文件尾
		{
			std::string lastname;
			std::string last1,last2;
			page4.BMainLast1.Replace("\r\n","@");
			page4.BMainLast2.Replace("\r\n","@");
			page4.BMainLast1.Replace("#","~");
			page4.BMainLast2.Replace("#","~");
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			last1 = page4.BMainLast1.GetBuffer();
			last2 = page4.BMainLast2.GetBuffer();
			lastname = last1 + "#" + last2;
			//ParamNode->SetAttributeValue("Value",lastname);
			//knifeTypeParm.setParam("GBLast",lastname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "GBLast", "Value", lastname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "GBLast");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", lastname.c_str());
			//}
		}

		//if (ParamNode->GetAttributeValue("Name") == "SPFirst")//反面主文件文件头
		{
			std::string firstname;
			std::string first1,first2;
			page5.SpFirst1.Replace("\r\n","@");
			page5.SpFirst2.Replace("\r\n","@");
			page5.SpFirst1.Replace("#","~");
			page5.SpFirst2.Replace("#","~");
			first1 = page5.SpFirst1.GetBuffer();
			first2 = page5.SpFirst2.GetBuffer();
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			firstname = first1 + "#" + first2;
			//ParamNode->SetAttributeValue("Value",firstname);
			//knifeTypeParm.setParam("SPFirst",firstname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "SPFirst", "Value", firstname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SPFirst");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", firstname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "SPLast")//反面主文件文件尾
		{
			std::string lastname;
			std::string last1,last2;
			page5.SpLast1.Replace("\r\n","@");
			page5.SpLast2.Replace("\r\n","@");
			page5.SpLast1.Replace("#","~");
			page5.SpLast2.Replace("#","~");
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			last1 = page5.SpLast1.GetBuffer();
			last2 = page5.SpLast2.GetBuffer();
			lastname = last1 + "#" + last2;
			//ParamNode->SetAttributeValue("Value",lastname);
			//knifeTypeParm.setParam("SPLast",lastname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "SPLast", "Value", lastname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SPLast");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", lastname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "MFirst")//主轴文件头
		{
			std::string firstname;
			std::string first1,first2;
			page2.MainFirst1.Replace("\r\n","@");
			page2.MainFirst2.Replace("\r\n","@");
			page2.MainFirst1.Replace("#","~");
			page2.MainFirst2.Replace("#","~");
			first1 = page2.MainFirst1.GetBuffer();
			first2 = page2.MainFirst2.GetBuffer();
			firstname = first1 + "#" + first2;
			//ParamNode->SetAttributeValue("Value",firstname);
			//knifeTypeParm.setParam("MFirst",firstname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "MFirst", "Value", firstname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "MFirst");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", firstname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "MLast")//主轴文件尾
		{
			std::string lastname;
			std::string last1,last2;
			page2.MainLast1.Replace("\r\n","@");
			page2.MainLast2.Replace("\r\n","@");
			page2.MainLast1.Replace("#","~");
			page2.MainLast2.Replace("#","~");
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			last1 = page2.MainLast1.GetBuffer();
			last2 = page2.MainLast2.GetBuffer();
			lastname = last1 + "#" + last2;
			//ParamNode->SetAttributeValue("Value",lastname);
			//knifeTypeParm.setParam("MLast",lastname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "MLast", "Value", lastname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "MLast");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", lastname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "PZFirst")//排鉆文件头
		{
			std::string firstname;
			std::string first1,first2;
			page3.PzFirst1.Replace("\r\n","@");
			page3.PzFirst2.Replace("\r\n","@");
			page3.PzFirst1.Replace("#","~");
			page3.PzFirst2.Replace("#","~");
			first1 = page3.PzFirst1.GetBuffer();
			first2 = page3.PzFirst2.GetBuffer();
			firstname = first1 + "#" + first2;
			//ParamNode->SetAttributeValue("Value",firstname);
			//knifeTypeParm.setParam("PZFirst",firstname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "PZFirst", "Value", firstname.c_str());
			
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "PZFirst");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", firstname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "PZLast")//排鉆文件尾
		{
			std::string lastname;
			std::string last1,last2;
			page3.PzLast1.Replace("\r\n","@");
			page3.PzLast2.Replace("\r\n","@");
			page3.PzLast1.Replace("#","~");
			page3.PzLast2.Replace("#","~");
			//firstname.Format(_T("%s"),ParamNode->GetAttributeValue("Value").c_str());
			last1 = page3.PzLast1.GetBuffer();
			last2 = page3.PzLast2.GetBuffer();
			lastname = last1 + "#" + last2;
			//ParamNode->SetAttributeValue("Value",lastname);
			//knifeTypeParm.setParam("PZLast",lastname);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "PZLast", "Value", lastname.c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "PZLast");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", lastname.c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim1")//工位1修边1
		{
				CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF1));
				if (theCheck->GetCheck())//负
				{
					m_btrim1 = "-" + m_btrim1;
				}
				//ParamNode->SetAttributeValue("Value",m_btrim1.GetBuffer());
				//knifeTypeParm.setParam("BTrim1",m_btrim1.GetBuffer());
				xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "BTrim1", "Value", m_btrim1.GetBuffer());

				//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim1");
				//if(elmTheParam != NULL)
				//{
				//	elmTheParam->SetAttribute("Value", m_btrim1.GetBuffer());
				//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim2")//工位1修边2
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF2));
			if (theCheck->GetCheck())//负
			{
				m_btrim2 = "-" + m_btrim2;
			}
			//ParamNode->SetAttributeValue("Value",m_btrim2.GetBuffer());
			//knifeTypeParm.setParam("BTrim2",m_btrim2.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "BTrim2", "Value", m_btrim2.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim2");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_btrim2.GetBuffer());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim3")//工位1修边3
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF3));
			if (theCheck->GetCheck())//负
			{
				m_btrim3 = "-" + m_btrim3;
			}
			//ParamNode->SetAttributeValue("Value",m_btrim3.GetBuffer());
			//knifeTypeParm.setParam("BTrim3",m_btrim3.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "BTrim3", "Value", m_btrim3.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim3");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_btrim3.GetBuffer());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BTrim4")//工位1修边4
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECKZF4));
			if (theCheck->GetCheck())//负
			{
				m_btrim4 = "-" + m_btrim4;
			}
			//ParamNode->SetAttributeValue("Value",m_btrim4.GetBuffer());	
			//knifeTypeParm.setParam("BTrim4",m_btrim4.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "BTrim4", "Value", m_btrim4.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BTrim4");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_btrim4.GetBuffer());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim1")//工位2修边1
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF1));
			if (theCheck->GetCheck())//负
			{
				m_ftrim1 = "-" + m_ftrim1;
			}
			//ParamNode->SetAttributeValue("Value",m_ftrim1.GetBuffer());
			//knifeTypeParm.setParam("FTrim1",m_ftrim1.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FTrim1", "Value", m_ftrim1.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim1");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_ftrim1.GetBuffer());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim2")//工位2修边2
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF2));
			if (theCheck->GetCheck())//负
			{
				m_ftrim2 = "-" + m_ftrim2;
			}
			//ParamNode->SetAttributeValue("Value",m_ftrim2.GetBuffer());
			//knifeTypeParm.setParam("FTrim2",m_ftrim2.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FTrim2", "Value", m_ftrim2.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim2");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_ftrim2.GetBuffer());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim3")//工位2修边3
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF3));
			if (theCheck->GetCheck())//负
			{
				m_ftrim3 = "-" + m_ftrim3;
			}
			//ParamNode->SetAttributeValue("Value",m_ftrim3.GetBuffer());
			//knifeTypeParm.setParam("FTrim3",m_ftrim3.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FTrim3", "Value", m_ftrim3.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim3");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_ftrim3.GetBuffer());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FTrim4")//工位2修边4
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_2CHECKZF4));
			if (theCheck->GetCheck())//负
			{
				m_ftrim4 = "-" + m_ftrim4;
			}
			//ParamNode->SetAttributeValue("Value",m_ftrim4.GetBuffer());
			//knifeTypeParm.setParam("FTrim4",m_ftrim4.GetBuffer());
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FTrim4", "Value", m_ftrim4.GetBuffer());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FTrim4");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_ftrim4.GetBuffer());
			//}
		}

		//if (ParamNode->GetAttributeValue("Name") == "BUsedLineX")//用户设置有效边，用于修边偏移（工位1X）
		{
			char test1[50];
			m_usedLineX1.GetLBText(m_usedLineX1.GetCurSel(),test1);
			std::string test = test1;
			//knifeTypeParm.setParam("BUsedLineX",m_usedLineMap.at(test1));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "BUsedLineX", "Value", m_usedLineMap.at(test1).c_str());

			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BUsedLineX");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_usedLineMap.at(test1).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "BUsedLineY")//用户设置有效边，用于修边偏移（工位1Y）
		{
			char test1[50];
			m_usedLineY1.GetLBText(m_usedLineY1.GetCurSel(),test1);
			std::string test = test1;
			//knifeTypeParm.setParam("BUsedLineY",m_usedLineMap.at(test1));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "BUsedLineY", "Value", m_usedLineMap.at(test1).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "BUsedLineY");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_usedLineMap.at(test1).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FUsedLineX")//用户设置有效边，用于修边偏移（工位2X）
		{
			char test1[50];
			m_usedLineX2.GetLBText(m_usedLineX2.GetCurSel(),test1);
			std::string test = test1;
			//knifeTypeParm.setParam("FUsedLineX",m_usedLineMap.at(test1));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FUsedLineX", "Value", m_usedLineMap.at(test1).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FUsedLineX");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_usedLineMap.at(test1).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FUsedLineY")//用户设置有效边，用于修边偏移（工位2Y）
		{
			char test1[50];
			m_usedLineY2.GetLBText(m_usedLineY2.GetCurSel(),test1);
			std::string test = test1;
			//knifeTypeParm.setParam("FUsedLineY",m_usedLineMap.at(test1));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FUsedLineY", "Value", m_usedLineMap.at(test1).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FUsedLineY");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", m_usedLineMap.at(test1).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "EachNCLineExtra")//用户每行NC代码的结尾追加符号
		{
			//knifeTypeParm.setParam("EachNCLineExtra",HGTools::Convert2StdString(m_EachNCLineExtra));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "EachNCLineExtra", "Value", HGTools::Convert2StdString(m_EachNCLineExtra).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "EachNCLineExtra");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", HGTools::Convert2StdString(m_EachNCLineExtra).c_str());
			//	string strTmp = elmTheParam->Attribute("Value");
			//}
		}

		//if (ParamNode->GetAttributeValue("Name") == "IsCobine")//是否将正面文件头和反面文件头合并
		{
			int check1 =m_combineSet.GetCurSel();
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("IsCobine",HGTools::Convert2StdString(test));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "IsCobine", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsCobine");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "IsComLine")//是否nc文件连接起来
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			int check1 = m_Iscomline.GetCheck();
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("IsComLine",HGTools::Convert2StdString(test));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "IsComLine", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsComLine");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "IsComBAll")//是否将正面nc文件合并起来
		{
			//std::string theName = ParamNode->GetAttributeValue("Value");
			int check1 = m_iscomball.GetCheck();
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("IsComBAll",HGTools::Convert2StdString(test));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "IsComBAll", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "IsComBAll");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "LessKnifeChange")//最少换刀设置
		{
			int check1 = m_lessKnifeChange.GetCheck();
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("LessKnifeChange",HGTools::Convert2StdString(test));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "LessKnifeChange", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "LessKnifeChange");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "SmallPanelNC")	//小板NC文件导出
		{
			int check1 = m_smallPanelNC;
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("SmallPanelNC",HGTools::Convert2StdString(test));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "SmallPanelNC", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "SmallPanelNC");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value", HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "FileClassifyMove")	// 生成NC、贴标文件归整
		{
			int check1 = m_fileClassifyMove;
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("FileClassifyMove",HGTools::Convert2StdString(test));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "FileClassifyMove", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "FileClassifyMove");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value",HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "NC_CommandChange")	// NC命令字变更
		{
			int check1 = m_NC_CommandChange;
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("NC_CommandChange",HGTools::Convert2StdString(test));

			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "NC_CommandChange", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_CommandChange");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value",HGTools::Convert2StdString(test).c_str());
			//}
		}
		//if (ParamNode->GetAttributeValue("Name") == "NC_SpaceRemove")	// NC坐标清除
		{
			int check1 = m_NC_SpaceRemove;
			char test[10];
			itoa(check1,test,10);
			//ParamNode->SetAttributeValue("Value",test);
			//knifeTypeParm.setParam("NC_SpaceRemove",HGTools::Convert2StdString(test));

			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "NC_SpaceRemove", "Value", HGTools::Convert2StdString(test).c_str());
			//TiXmlElement* elmTheParam = xmlHandler.GetIndicatedElement(elmParamList, "Param", "Name", "NC_SpaceRemove");
			//if(elmTheParam != NULL)
			//{
			//	elmTheParam->SetAttribute("Value",HGTools::Convert2StdString(test).c_str());
			//}
		}
		{
			int check1 = m_bModifyArcMillingFormat;
			char test[10];
			itoa(check1,test,10);
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "NC_ModifyArcMillingFormat", "Value", HGTools::Convert2StdString(test).c_str());
		}
		{
			CButton* theCheck = (CButton*)(GetDlgItem(IDC_CHECK_REVERSE_LABEL));
			xmlHandler.SetIndicatedElmAttr(elmParamList, "Param", "Name", "LabelSide", "Value", theCheck->GetCheck() ? "1" : "0");
		}
	}
	//saveSettings();
}

//单双工位切换
void CKnifeAddDlg::OnCbnSelchangeCombo1()
{
	
	int double2 = m_double.GetCurSel();
	if (double2 ==0)
	{
		page1.GetDlgItem(IDC_CHANGEDIT2)->ShowWindow(FALSE);
		page1.GetDlgItem(IDC_CHANGEDITL2)->ShowWindow(FALSE);
		page2.GetDlgItem(IDC_MAINEDIT2)->ShowWindow(FALSE);
		page2.GetDlgItem(IDC_MAINEDITL2)->ShowWindow(FALSE);
		page3.GetDlgItem(IDC_PZEDIT2)->ShowWindow(FALSE);
		page3.GetDlgItem(IDC_PZEDITL2)->ShowWindow(FALSE);
		page4.GetDlgItem(IDC_BMAINEDIT2)->ShowWindow(FALSE);
		page4.GetDlgItem(IDC_BMAINEDITL2)->ShowWindow(FALSE);
		page5.GetDlgItem(IDC_SPEDIT2)->ShowWindow(FALSE);
		page5.GetDlgItem(IDC_SPEDITL2)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBOBOXEX2)->EnableWindow(FALSE);
		GetDlgItem(IDC_2CHECKZF1)->EnableWindow(FALSE);
		GetDlgItem(IDC_2CHECKZF2)->EnableWindow(FALSE);
		GetDlgItem(IDC_2CHECKZF3)->EnableWindow(FALSE);
		GetDlgItem(IDC_2CHECKZF4)->EnableWindow(FALSE);
		GetDlgItem(IDC_2TRIM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_2TRIM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_2TRIM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_2TRIM4)->EnableWindow(FALSE);
		m_usedLineX2.EnableWindow(FALSE);
		m_usedLineY2.EnableWindow(FALSE);
	}
	else
	{
		page1.GetDlgItem(IDC_CHANGEDIT2)->ShowWindow(TRUE);
		page1.GetDlgItem(IDC_CHANGEDITL2)->ShowWindow(TRUE);
		page2.GetDlgItem(IDC_MAINEDIT2)->ShowWindow(TRUE);
		page2.GetDlgItem(IDC_MAINEDITL2)->ShowWindow(TRUE);
		page3.GetDlgItem(IDC_PZEDIT2)->ShowWindow(TRUE);
		page3.GetDlgItem(IDC_PZEDITL2)->ShowWindow(TRUE);
		page4.GetDlgItem(IDC_BMAINEDIT2)->ShowWindow(TRUE);
		page4.GetDlgItem(IDC_BMAINEDITL2)->ShowWindow(TRUE);
		page5.GetDlgItem(IDC_SPEDIT2)->ShowWindow(TRUE);
		page5.GetDlgItem(IDC_SPEDITL2)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBOBOXEX2)->EnableWindow(TRUE);
		GetDlgItem(IDC_2CHECKZF1)->EnableWindow(TRUE);
		GetDlgItem(IDC_2CHECKZF2)->EnableWindow(TRUE);
		GetDlgItem(IDC_2CHECKZF3)->EnableWindow(TRUE);
		GetDlgItem(IDC_2CHECKZF4)->EnableWindow(TRUE);
		GetDlgItem(IDC_2TRIM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_2TRIM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_2TRIM3)->EnableWindow(TRUE);
		GetDlgItem(IDC_2TRIM4)->EnableWindow(TRUE);
		m_usedLineX2.EnableWindow(TRUE);
		m_usedLineY2.EnableWindow(TRUE);
	}
}


void CKnifeAddDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
		CDialogEx::OnSize(nType, cx, cy);  
		if (nType==SIZE_RESTORED || nType==SIZE_MAXIMIZED)  
		{  
			float   ratio[2];  
			POINT   newDialogSize;  
			CRect   newRect;  	
			//获取新的客户区的大小  
			GetClientRect(&newRect);  
			newDialogSize.x = newRect.right - newRect.left;  
			newDialogSize.y = newRect.bottom - newRect.top;  
	
			//得现在的对话框与以往对话框的大小比例  
			ratio[0]    = (float)newDialogSize.x / oSize.x;  
			ratio[1]    = (float)newDialogSize.y / oSize.y;  
	
			CRect Rect;  
			int woc;  
	
			//左右上角的数据  
			CPoint OldTLPoint, NewTLPint;  
			CPoint OldBRPoint, NewBRPint;  
	
			//列出所有控件  
			HWND  hwndChild = ::GetWindow(m_hWnd,GW_CHILD);  
	
			while(hwndChild)  
			{  
				//取得ID  
				woc  = ::GetDlgCtrlID(hwndChild);  
				GetDlgItem(woc)->GetWindowRect(Rect);  
				ScreenToClient(Rect);  
	
				OldTLPoint = Rect.TopLeft();  
				NewTLPint.x  = long(OldTLPoint.x*ratio[0]);  
				NewTLPint.y  = long(OldTLPoint.y*ratio[1]);  
	
				OldBRPoint   = Rect.BottomRight();  
				NewBRPint.x  = long(OldBRPoint.x*ratio[0]);  
				NewBRPint.y  = long(OldBRPoint.y*ratio[1]);  
	
				Rect.SetRect(NewTLPint,NewBRPint);  
				GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
				hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);  
			}  
	
			//获取视频窗口父窗口的大小  
			//::GetClientRect(hWnd, &grc);  
			oSize = newDialogSize;      
		}  

	
}

//Combox获得所选项
int CKnifeAddDlg::getSelect2Setting(CComboBox& combox)
{
	CString selectItem;
	combox.GetLBText(combox.GetCurSel(),selectItem);
	return atoi(selectItem);
}

void CKnifeAddDlg::setSelectText(CComboBox& combox, std::string theName )
{
	CString temp;
	temp.Format("%s",theName.c_str());
	int selectItem = atoi(temp);
	for (int i =0;i<combox.GetCount();i++)
	{
		CString comboxString;
		combox.GetLBText(i,comboxString);
		if (comboxString == temp)
		{
			combox.SetCurSel(i);
			break;
		}
	}
	
}

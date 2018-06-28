// PrintCodeDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "PrintCodeDlg.h"
#include "afxdialogex.h"
//#include "rqcode/qrencode.h"
//#include "CommDlg.h"
#include "afxdlgs.h"
//#include "hg3d/XmlFile.h"
//#include "hg3d/Utils.h"
//#include "hg3d/hgtypes.h"
//#include "hg3d/XmlNode.h"
#include "../Misc/Barcode39.h"

//#include "HGCode.h"
//#include "HGTools.h"

//#include "SelectPathDlg.h"
//#include "TiebiaoEdgeDlg.h"
//#include "ParamSettingDlg.h"
#include "../Misc/Misc.h"
#include "ParamSettingDlg.h"
#include "TiebiaoEdgeDlg.h"
#include "SelectPathDlg.h"
#include "../Misc/HGTools.h"



#include "../../../include/QRCode/qrcode/qrencode.h"


#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Misc/ProgramMisc.h"
#include "../Work/WorkDef.h"


// CPrintCodeDlg dialog

IMPLEMENT_DYNAMIC(CPrintCodeDlg, CDialogEx)

CPrintCodeDlg::CPrintCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrintCodeDlg::IDD, pParent)
{
	m_panelPictureNumCount = 0;
	m_panelNCCode = false;
}

CPrintCodeDlg::~CPrintCodeDlg()
{
}

void CPrintCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_codelist);
	DDX_Control(pDX, IDC_BQCOMBO, m_comboScaling);
}


BEGIN_MESSAGE_MAP(CPrintCodeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PANEL_A4, &CPrintCodeDlg::OnExportPanelA4)			//导出标签（大板图片是A4）
	ON_BN_CLICKED(IDC_PANEL_BARCODE, &CPrintCodeDlg::OnDaoChuAnNiu)			//导出标签（大板图片是标签）
	ON_BN_CLICKED(IDC_BUTTON2, &CPrintCodeDlg::OnDaYinAnNiu)				//打印按钮
	ON_BN_CLICKED(IDC_BUTTON1, &CPrintCodeDlg::OnBnClickedButton1)			//关闭按钮
	ON_CBN_SELCHANGE(IDC_BQCOMBO, &CPrintCodeDlg::OnCbnSelchangeBqcombo)	//导出标签比例

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPrintCodeDlg message handlers


void CPrintCodeDlg::OnDaYinAnNiu()
{


	PrintPic();
	
}
BOOL CPrintCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comboScaling.InsertString(0,_T("1:1"));//单双工位设置
	m_comboScaling.InsertString(1, _T("1:2")); 
	m_comboScaling.InsertString(2, _T("1:2.5")); 
	m_comboScaling.SetCurSel(0); //设置选中的项

	CString strConfigFile3 = GetModulePath() +  _T("kdata");


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)
	CString strTmp;
	strTmp.Format(_T("\\PrintModelSelect%s"), g_szEncyptSubfix);
	strConfigFile3 = strConfigFile3 + strTmp;//过滤孔位大小文件
	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile3);
	decrypt_base64(strConfigFile3.GetBuffer(), xmlpath.GetBuffer());

#else

	strConfigFile3 = strConfigFile3 + "\\PrintModelSelect.hgm";//过滤孔位大小文件
	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile3);
	//HGTools::decryptFile(strConfigFile3,xmlpath);


#endif






	XmlHandler xmlHandler;
	xmlHandler.LoadFile(xmlpath.GetBuffer());
	HGTools::deleteFile(xmlpath);

	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();

	if(elmRoot != NULL)
	{
		TiXmlElement* elmParam = xmlHandler.GetChildElm(elmRoot, "");
		CString strTmp;
		xmlHandler.GetXmlAttribute(elmParam, "Size", strTmp);
		std::string str_Select = strTmp;
		m_comboScaling.SetCurSel(atoi(str_Select.c_str()));
	}
	UpdateData(FALSE);

	//数据转换成打印用数据
	//TransitionPrint();
	//生成二维码图片
	BuildCodeBmp();

	return TRUE;
}

//生成条码
void CPrintCodeDlg::OnPaint()
{
	CPaintDC dc(this); 

	CString strConfigFile = GetModulePath() +  _T("picture");


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	CString strTmp;
	strTmp.Format(_T("\\printcode%s"), g_szEncyptSubfix);
	strConfigFile = strConfigFile + strTmp;

	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
	decrypt_base64(strConfigFile.GetBuffer(), xmlpath.GetBuffer());

#else

	strConfigFile = strConfigFile + "\\printcode.hgm";

	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
	//HGTools::decryptFile(strConfigFile,xmlpath);


#endif




	XmlHandler xmlHandler;
	bool bLoadSuccess = xmlHandler.LoadFile(xmlpath.GetBuffer());
	HGTools::deleteFile(xmlpath);
	if (!bLoadSuccess)
	{
		AfxMessageBox("读取printcode.xml失败",MB_OK);
		return;
	}

	TiXmlElement* elmUsed = NULL; 
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	vector<TiXmlElement*> vChildNodes = xmlHandler.GetChildElms(elmRoot, NULL);

	for (int i = 0;i < vChildNodes.size();i++)
	{
		TiXmlElement* elmChild = vChildNodes[i];
		CString strTmp;
		xmlHandler.GetXmlAttribute(elmChild, "Used", strTmp);
		if (strTmp.CompareNoCase(_T("1")) == 0)
		{
			elmUsed = elmChild;
			break;
		}
	}

	if (elmUsed == NULL)
	{
		AfxMessageBox("未读取到可以使用的打印模板",MB_OK);
		return;
	}


	CString file_path = HGTools::getCurExeContainPath();
	file_path = file_path + _T("\\picture");

	//CString strTmp;
	xmlHandler.GetXmlAttribute(elmUsed, "pictureName", strTmp);
	std::string s_f = strTmp;


	CString str_picture_before;
	str_picture_before.Format("%s",s_f.c_str());

	CString cstr_s_f;
	cstr_s_f.Format("%s",s_f.c_str());

	//由前到后读取目录下的文件
	m_codelist.DeleteAllItems();
	m_codelist.SetFolder(280,200,file_path,m_info.size()+m_panelPictureNumCount,str_picture_before);
}


//根据外部传入数据，调换图片顺序，以达到图片排列顺序为大板及其附属小板的显示方式
void CPrintCodeDlg::sortFile(CString file_path ,CString prefix_name)
{
		int totalSize = m_info.size() + m_panelPictureNumCount;
		std::vector<int> origin;
		std::vector<int> newOrder;
		int i =0;
		//初始化原数组
		for (int i =0;i< totalSize;i++ )
		{
			origin.push_back(i);
		}

		int order = 0;
		int m_info_index = 0;
		int m_panel_index = 0;
		while(true)
		{
			//大小版都用完
			if ((m_panel_index >= m_panelNumList.size()) &&
				(m_info_index >= m_info.size()))
			{
				break;
			}
			//大板用完
			else if (m_panel_index >= m_panelNumList.size())
			{
				newOrder.push_back(m_info_index + m_panelPictureNumCount);
				m_info_index++;
			}
			//小板用完
			else if (m_info_index >= m_info.size())
			{
				newOrder.push_back(m_panel_index);
				m_panel_index++;
			}
			//大板序号大于小板序号
			else if (m_panelNumList.at(m_panel_index) > atoi(m_info.at(m_info_index).panel_no))
			{
				newOrder.push_back(m_info_index + m_panelPictureNumCount);
				m_info_index++;
			}
			//大板序号等于小板序号
			else if (m_panelNumList.at(m_panel_index) == atoi(m_info.at(m_info_index).panel_no))
			{
				newOrder.push_back(m_panel_index);
				m_panel_index++;
			}
			//大板序号小于小板序号
			else if (m_panelNumList.at(m_panel_index) < atoi(m_info.at(m_info_index).panel_no))
			{
				newOrder.push_back(m_panel_index);
				m_panel_index++;
			}
			else
			{
				return;
			}
			order++;
		}



		if (newOrder.size() != origin.size())
		{
			return;
		}


		//旧的文件统一改名，以防冲突
		for (int i =0;i<totalSize; i++)
		{
			CString oldName,newTempName;
			oldName.Format("%s-%d.bmp",prefix_name,i);
			newTempName.Format("%s-%d.bmpt",prefix_name,i);
			CFile::Rename(file_path + "\\" + oldName , file_path + "\\" + newTempName);
		}
	
		//所有文件按照规则修改
		for (int i =0;i<totalSize; i++)
		{
			CString oldName,newTempName;
			oldName.Format("%s-%d.bmpt",prefix_name,newOrder.at(i));
			newTempName.Format("%s-%d.bmp",prefix_name,i);
			CFile::Rename(file_path + "\\" + oldName , file_path + "\\" + newTempName);

			//找回大板保存的路径
			if (newOrder.at(i) < m_panelPictureNumCount)
			{
				m_panelSmallPicPathList[newOrder.at(i)] = file_path + "\\" + newTempName;
			}
			else
			{
				m_print_path[newOrder.at(i) - m_panelPictureNumCount] = file_path + "\\" + newTempName;
			}
		}

}

BOOL CPrintCodeDlg::PrintPic()//打印
{ 
	int totalSize = m_info.size() + m_panelPictureNumCount;

	ParamSettingDlg settings;
	settings.loadSettings();

	CDC dc;
	CPrintDialog printDlg(FALSE);
	int theNum = 1;
	if(printDlg.DoModal() == IDOK)
	{
		//准备打印设备
		HGLOBAL hDevMode;
		HGLOBAL hDevNames;
		
		printDlg.GetDefaults();
		DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(printDlg.m_pd.hDevMode);
		pDevMode->dmFields = pDevMode->dmFields | DM_PAPERSIZE;
		pDevMode->dmPaperSize = DMPAPER_USER;    
		
		::GlobalUnlock(printDlg.m_pd.hDevMode);

		CPrintInfo Info;

		dc.Attach(printDlg.CreatePrinterDC());
		dc.m_bPrinting = TRUE;
		CString strTitle;
		strTitle.LoadString(AFX_IDS_APP_TITLE);
		DOCINFO di;
	 
		::ZeroMemory (&di, sizeof (DOCINFO));
		di.cbSize = sizeof (DOCINFO);

		

		//获得配置的printNode
		CString strConfigFile = GetModulePath() +  _T("picture");


		// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

		CString strTmp;
		strTmp.Format(_T("\\printcode%s"), g_szEncyptSubfix);
		strConfigFile = strConfigFile + strTmp;

		CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
		decrypt_base64(strConfigFile.GetBuffer(), xmlpath.GetBuffer());

#else

		strConfigFile = strConfigFile + "\\printcode.hgm";

		CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
		//HGTools::decryptFile(strConfigFile,xmlpath);


#endif





		XmlHandler xmlHandler;
		bool bLoadSuccess = xmlHandler.LoadFile(xmlpath.GetBuffer());
		HGTools::deleteFile(xmlpath);
		if (!bLoadSuccess)
		{
			AfxMessageBox("读取printcode.xml失败",MB_OK);
			return FALSE;
		}

		TiXmlElement* elmUsed = NULL; 
		TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
		vector<TiXmlElement*> vChildNodes = xmlHandler.GetChildElms(elmRoot, NULL);

		for (int i = 0;i < vChildNodes.size();i++)
		{
			TiXmlElement* elmChild = vChildNodes[i];
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmChild, "Used", strTmp);
			if (strTmp.CompareNoCase(_T("1")) == 0)
			{
				elmUsed = elmChild;
				break;
			}
		}

		if (elmUsed == NULL)
		{
			AfxMessageBox("未读取到可以使用的打印模板",MB_OK);
			return FALSE;
		}

		int _paperWidth;
		int _paperHeight;
		xmlHandler.GetXmlAttribute(elmUsed, "paperWidth", _paperWidth);
		xmlHandler.GetXmlAttribute(elmUsed, "paperHeight", _paperHeight);

		//L4Cpp::Log()->warn("\n\nBegin to print\n");

		//打印所有图片
		{		
			CString strTmp;
			xmlHandler.GetXmlAttribute(elmUsed, "pictureName", strTmp);
			std::string s_f = strTmp;

			di.lpszDocName = "样板打印";
			char ExeFile[2000];
			GetModuleFileName(NULL,ExeFile,500);
			CString file_path = ExeFile;
			file_path = file_path.Left(file_path.ReverseFind('\\'));
			file_path = file_path + _T("\\picture");



			//加载所有图片并打印
			for (int p = 0;p < totalSize ;p++)
			{
				CString src_file_name,file_name,file_code;
				src_file_name.Format("%s.bmp",s_f.c_str());
				file_name.Format("%s-%d.bmp",s_f.c_str(),p);

				//AfxMessageBox("1");
				if ((p % settings.Print_per_num()) == 0)//每打印10个睡1s
				{
					Sleep(settings.Print_sleep());
				}

				BOOL bPrintingOK = dc.StartDoc(&di);

				//////////////////////////////////////////////////////////////////////////
				//TODO: 思考问题
				//【1】dc.GetDeviceCaps(HORZRES),dc.GetDeviceCaps(VERTRES) 分别是多少
				//【2】打出来有乱序情况是不是path.Format("%s\\%s.",file_path,file_name); 有问题
				//【3】打印图片模糊 和图片尺寸有关吗？OnPrint(&dc, &Info, path, _paperWidth, _paperHeight);
				//		int _paperWidth = atoi(print_node->GetAttributeValue("paperWidth").c_str());
				//		int _paperHeight = atoi(print_node->GetAttributeValue("paperHeight").c_str());

				Info.m_rectDraw.SetRect(0,0,dc.GetDeviceCaps(HORZRES),dc.GetDeviceCaps(VERTRES));

				CString path;
				path.Format("%s\\%s.",file_path,file_name);

				//记录打印顺序
				//L4Cpp::Log()->warn(path.GetBuffer());

				Info.SetMaxPage (1);
				OnPrint(&dc, &Info, path, _paperWidth, _paperHeight);
				if (bPrintingOK)
					dc.EndDoc();
				else
					dc.AbortDoc();
			}
		}
		dc.Detach();

	 }
	 return TRUE;
}


void CPrintCodeDlg::OnPrint(CDC* pdc,CPrintInfo * lParam,CString strFileName,int bwidth,int bheight)
{
	CDC* pDC   = pdc;
	CPrintInfo* pInfo = (CPrintInfo *)lParam;
 
	CFont DataFont;
	DataFont.CreatePointFont(120,"宋体",pDC);

	HBITMAP hBitmap = (HBITMAP) ::LoadImage(NULL, strFileName, IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);

	HDC dcMem;
	dcMem=::CreateCompatibleDC(pDC->m_hDC);

	HBITMAP hOldBmp=(HBITMAP)::SelectObject(dcMem,hBitmap);
 
	CRect r = pInfo->m_rectDraw;
	int nVertCenterPos = pDC->GetDeviceCaps (VERTRES) / 2;
	::StretchBlt(pDC->m_hDC, r.left, r.top, r.Width(), r.Height(),
	dcMem,0,0,bwidth,bheight,SRCCOPY);
 
 
	::SelectObject(dcMem,hOldBmp);
	::DeleteDC(dcMem);
	::DeleteObject(hBitmap);
}

void CPrintCodeDlg::OnBnClickedButton1()
{
	//this->CloseWindow();
	CDialogEx::OnCancel();
}

//长内容分段截取扩展（源字符串 前后分别截取n个字符长度）
void CPrintCodeDlg::BuildCodeExtend(std::map<std::string,std::string>& dst,const std::string keyname)
{
	if (dst.find(keyname) != dst.end())
	{
		std::string src = dst[keyname];

		int strlen = src.length();
		CString tempValueStr = src.c_str();
		CString tempNameStr;

		//左多少个字符 
		for (int i = 0; i < strlen ; i++)
		{
			tempNameStr.Format("%d_%s",i,keyname.c_str());
			tempValueStr = src.c_str();
			tempValueStr = tempValueStr.Left(i);
	
			dst[tempNameStr.GetBuffer()] = tempValueStr.GetBuffer();
		}

		//右多少个字符 
		for (int i = 0; i < strlen ; i++)
		{
			tempNameStr.Format("%s_%d",keyname.c_str(),i);
			tempValueStr = src.c_str();
			tempValueStr = tempValueStr.Right(i);
	
			dst[tempNameStr.GetBuffer()] = tempValueStr.GetBuffer();
		}
	}
}

void CPrintCodeDlg::TransitionPrint(TiXmlElement* elmPrintSetting)
{
	bool bOutputDoorLabel = false, bOutputSendingDate = false;
	XmlHandler xmlHandler;
	TiXmlElement* elmIsDoorPanel = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "门板标记");
	if (elmIsDoorPanel != NULL && strcmp(elmIsDoorPanel->Attribute("Used"), "1") == 0)
		bOutputDoorLabel = true;
	TiXmlElement* elmSendingDate = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "发货日期");
	if (elmSendingDate != NULL && strcmp(elmSendingDate->Attribute("Used"), "1") == 0)
		bOutputSendingDate = true;

	m_print_info.clear();
	for (int i = 0;i < m_info.size();i++)
	{
		recode_print _r_print;
		_r_print.code_info = m_info.at(i).bar_code.GetBuffer();
		_r_print.code_info2 = m_info.at(i).rebar_code.GetBuffer();



		
		CString orderNameString;
		orderNameString.Format("%06d-%d",_ttoi(m_info.at(i).panel_no),_ttoi(m_info.at(i).com_no));
		orderNameString = orderNameString.Right(8);
		_r_print.printtext["加工顺序"] = orderNameString.GetBuffer();
		BuildCodeExtend(_r_print.printtext,"加工顺序");


		_r_print.printtext["客户信息"] = m_info.at(i).customer_info.GetBuffer();


		_r_print.printtext["条码编号"] = m_info.at(i).bar_code.GetBuffer();
		BuildCodeExtend(_r_print.printtext,"条码编号");
		


		_r_print.printtext["板件尺寸"] = m_info.at(i).com_width + "*" + m_info.at(i).com_height + "*" + m_info.at(i).com_depth;
		_r_print.printtext["板件名称"] = m_info.at(i).com_name.GetBuffer();
		_r_print.printtext["板件材料"] = m_info.at(i).com_matrial.GetBuffer();
		_r_print.printtext["柜号"] = m_info.at(i).cst_no;
		_r_print.printtext["异型"] = m_info.at(i).other_com;
		_r_print.printtext["开槽"] = m_info.at(i).slot_com.Left(2);
		_r_print.printtext["助记号"] = m_info.at(i).punching_com;
		_r_print.printtext["翻转"] = m_info.at(i).panel_reversal == "是" ? "反" : "";
		_r_print.printtext["钻槽标识"] = m_info.at(i).zc_identify;
		if (m_info.at(i).punching_com != "" && m_info.at(i).double_com == "" && m_info.at(i).rebar_code != "")
		{
			_r_print.code_info = _r_print.code_info2;
			_r_print.code_info2 = "";
		}
		if (_r_print.code_info.compare(_r_print.code_info2) == 0)
		{
			_r_print.code_info2 = "";
		}


		_r_print.printtext["双面"] = m_info.at(i).double_com;
		_r_print.printtext["加盟店"] = m_info.at(i).franchisee;
		_r_print.printtext["产品名称"] = m_info.at(i).cst_name;
		_r_print.printtext["订单类型"] = m_info.at(i).ordertype;
		

		_r_print.printtext["客户地址"] = m_info.at(i).customer_address.GetBuffer();
		BuildCodeExtend(_r_print.printtext,"客户地址");
		CString customAddr = m_info.at(i).customer_address;
		int custemAddrIndex = 1;
		do 
		{
			CString name;
			name.Format("客户地址%d",custemAddrIndex++);
			_r_print.printtext[name.GetBuffer()] = customAddr.Left(30);
			customAddr = customAddr.Mid(30);
		} while (customAddr.GetLength() > 0 );
		

		CString z_identify,c_identify,zc_identify;
		zc_identify = m_info.at(i).zc_identify;
		if (zc_identify.Find("#") >= 0)
		{
			z_identify = zc_identify.Left(zc_identify.Find("#"));
			c_identify = zc_identify.Right(zc_identify.GetLength() - zc_identify.Find("#") - 1);
			if (z_identify == "")
			{
				_r_print.printtext["钻标识"] = c_identify;
				_r_print.printtext["槽标识"] = "";
			}
			else
			{
				_r_print.printtext["钻标识"] = z_identify;
				_r_print.printtext["槽标识"] = c_identify;
			}
		}
		else
		{
			_r_print.printtext["钻标识"] = "";
			_r_print.printtext["槽标识"] = "";
		}

		if(bOutputDoorLabel && m_info[i].bIsDoorPanel)
			_r_print.printtext["门板标记"] = "[门]";

		if(bOutputSendingDate)
			_r_print.printtext["发货日期"] = m_info[i].m_strSendingDate;
		

		_r_print._picture.s_width = m_info.at(i).product_width.GetBuffer();
		_r_print._picture.s_height = m_info.at(i).product_height.GetBuffer();
		_r_print._picture._width = atoi(_r_print._picture.s_width.c_str());
		_r_print._picture._height = atoi(_r_print._picture.s_height.c_str());

		CString _dir = m_info.at(i).panel_dir;
		std::string s_dir = _dir.GetBuffer();
		_r_print._picture._dir = atoi(s_dir.c_str());

		CString _def = m_info.at(i).edge_info;
		if (_def == "")
		{
			_r_print._picture.left_edge = 0.0;
			_r_print._picture.down_edge = 0.0;
			_r_print._picture.right_edge = 0.0;
			_r_print._picture.up_edge = 0.0;
		}
		else
		{
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
		}

		_r_print._picture.nRotatedAngle = m_info.at(i).nRotatedAngle;

		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_path = ExeFile;
		file_path = file_path.Left(file_path.ReverseFind('\\'));
		file_path = file_path + _T("\\picture\\logo.bmp");

		_r_print.logo_path = file_path;
		m_print_info.push_back(_r_print);
	}
}
void CPrintCodeDlg::Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	try
	{
		wchar_t * wText = wchar;
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte的运用
		char *psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
		psText = new char[dwNum];
		WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte的再次运用
		szDst = psText;// std::string赋值
		delete []psText;// psText的清除
	}
	catch (...)
	{

	}
}
wstring CPrintCodeDlg::ANSIToUnicode(const string& str)
{
		int len = 0;
		len = str.length();
		 int unicodeLen = ::MultiByteToWideChar( CP_ACP,
			            0,
			            str.c_str(),
			             -1,
			             NULL,
			             0 ); 
		 wchar_t * pUnicode; 
		 pUnicode = new wchar_t[unicodeLen+1]; 
		 memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
		 ::MultiByteToWideChar( CP_ACP,
			          0,
			          str.c_str(),
			          -1,
			          (LPWSTR)pUnicode,
			          unicodeLen ); 
		 wstring rt; 
		 rt = ( wchar_t* )pUnicode;
		 delete pUnicode; 
		 return rt; 
 }
int CPrintCodeDlg::MyBarcode(CDC* cdc,int x1,int y1,int y2,int codetype,int ipen,const char * cCode)
{//x1,y1表示位置，y2表示高度
	CString csCode;
	CString LocalBmpPath;
	csCode.Format("%s",cCode);
	Barcode128 code;
	code.Encode128B(csCode);

	//by JackChen 2016-7-26 11.16.44 老大说换成128B
	//Barcode39 code;
	//code.Encode39(csCode);

	/*if (39==codetype)
	{
		Barcode39 code;
		code.Encode39(csCode);
	}else 
	{
		Barcode128 code;
		code.Encode128C(csCode);
	}*/

	double iPenW = 1.0;
	COLORREF clrBar		=RGB(0,0,0);
	COLORREF clrSpace	=RGB(255,255,255);


	int codelen = iPenW*code.GetEncodeLength();
	HDC hDC = cdc->GetSafeHdc();

	CRect m_SaveRect(x1,y1,x1 + codelen + 5,y2);
	CRect rectCodeText(x1,y2+5,x1 + codelen + 5,y2+15);
	FillRect(hDC,m_SaveRect,CBrush(clrSpace));

	code.DrawBarcode(hDC,x1,y1,y2,y2,clrBar,clrSpace,iPenW);

	/*HFONT hCodeFont = CreateFont(60,0,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("宋体"));
	SelectObject(hDC,hCodeFont);
	DrawText(hDC,csCode,csCode.GetLength(),rectCodeText,DT_TOP|DT_CENTER);*/

	return 0;
}

int CPrintCodeDlg::GetBarcodeLen(int ipen,const char * cCode)
{
	CString csCode;
	CString LocalBmpPath;
	csCode.Format("%s",cCode);
	Barcode39 code;
	code.Encode39(csCode);
	int iPenW = ipen;
	int codelen = iPenW*code.GetEncodeLength();
	return codelen;
}

//TODO: 导出的时候会重新绘图，注意调试会不会造成图片覆盖
void CPrintCodeDlg::OnDaoChuAnNiu()//导出
{
	ExportPanelPic(false);
}



//选择导出标签比例
void CPrintCodeDlg::OnCbnSelchangeBqcombo()
{
	int bqbl = m_comboScaling.GetCurSel();
	CString thebqbl;
	thebqbl.Format("%d",bqbl);


	//hg3d::TString strConfigFile3 = _T("kdata");
	//strConfigFile3 = hg3d::getFullFileName(strConfigFile3);
	//strConfigFile3 += _T("\\PrintModelSelect.xml");//过滤孔位大小文件

	//hg3d::CXmlFile * doc3 = new hg3d::CXmlFile();
	//doc3->Load(strConfigFile3.c_str());
	//if (doc3 != NULL)
	//{
	//	for (int k = 0;k < doc3->GetChildNodes().size();k++)
	//	{
	//		doc3->GetChildNodes().at(k)->SetAttributeValue("Size",thebqbl.GetBuffer());			
	//	}
	//}


	CString strConfigFile3 = GetModulePath() +  _T("kdata");


	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	CString strTmp;
	strTmp.Format(_T("\\PrintModelSelect%s"), g_szEncyptSubfix);
	strConfigFile3 = strConfigFile3 + strTmp;//过滤孔位大小文件

	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile3);
	decrypt_base64(strConfigFile3.GetBuffer(), xmlpath.GetBuffer());


#else


	strConfigFile3 = strConfigFile3 + "\\PrintModelSelect.hgm";//过滤孔位大小文件

	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile3);
	//HGTools::decryptFile(strConfigFile3,xmlpath);

#endif






	XmlHandler xmlHandler;
	xmlHandler.LoadFile(xmlpath.GetBuffer());
	HGTools::deleteFile(xmlpath);

	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();

	if(elmRoot != NULL)
	{
		vector<TiXmlElement*> vChildNodes = xmlHandler.GetChildElms(elmRoot, NULL);

		for (int i = 0;i < vChildNodes.size();i++)
		{
			TiXmlElement* elmChild = vChildNodes[i];
			elmChild->SetAttribute("Size",thebqbl.GetBuffer());
		}
	}

	xmlHandler.SaveFile();

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	encrypt_base64(xmlpath.GetBuffer(), strConfigFile3.GetBuffer());


#else

	//HGTools::encryptFile(xmlpath,strConfigFile3);

#endif





	//关闭文件
	HGTools::deleteFile(xmlpath);
}


//选择导出标签比例
double CPrintCodeDlg::getDaoChuBiaoQianBiLi()
{
	double scale = 1.0;
	CString selectText;
	m_comboScaling.GetLBText(m_comboScaling.GetCurSel(),selectText);
	int indexOfMaohao = selectText.Find(':');
	if (indexOfMaohao != -1)
	{
		CString right = selectText.Right(selectText.GetLength()-indexOfMaohao-1);
		scale = atof(right);
	}
	return scale;
}

//AB钻只有一个，正反条码相同
bool isZhuanSame_BarCodeSameJump(recode_print& pri)
{
	if (strcmp(pri.code_info.c_str(),pri.code_info2.c_str()) == 0)
	{
		std::string cao = pri.printtext["钻槽标识"];
		int indexOfA = cao.find('A');
		int indexOfSharp = cao.find('#');
		int indexOfB = cao.find('B');
		if ((indexOfA == string::npos && indexOfB != string::npos)
			||(indexOfA != string::npos && indexOfB == string::npos))
		{
			return true;
		}
	}
	return false;
}

//数据转换成图片
void CPrintCodeDlg::BuildCodeBmp()
{
	CPaintDC dc(this); // device context for painting

	CString strConfigFile = GetModulePath() +  _T("picture");

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)
	CString strTmp;
	strTmp.Format(_T("\\printcode%s"), g_szEncyptSubfix);
	strConfigFile = strConfigFile + strTmp;

	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
	decrypt_base64(strConfigFile.GetBuffer(), xmlpath.GetBuffer());

#else


	strConfigFile = strConfigFile + "\\printcode.hgm";

	CString xmlpath = HGTools::getXmlPathFromHgxPath(strConfigFile);
	//HGTools::decryptFile(strConfigFile,xmlpath);

#endif







	XmlHandler xmlHandler;
	bool bLoadSuccess = xmlHandler.LoadFile(xmlpath.GetBuffer());
	HGTools::deleteFile(xmlpath);
	if (!bLoadSuccess)
	{
		AfxMessageBox("读取printcode.xml失败",MB_OK);
		return;
	}

	TiXmlElement* elmUsed = NULL; 
	TiXmlElement* elmRoot = xmlHandler.GetDoc()->RootElement();
	vector<TiXmlElement*> vChildNodes = xmlHandler.GetChildElms(elmRoot, NULL);

	for (int i = 0;i < vChildNodes.size();i++)
	{
		TiXmlElement* elmChild = vChildNodes[i];
		CString strTmp;
		xmlHandler.GetXmlAttribute(elmChild, "Used", strTmp);
		if (strTmp.CompareNoCase(_T("1")) == 0)
		{
			elmUsed = elmChild;
			break;
		}
	}

	if (elmUsed == NULL)
	{
		AfxMessageBox("未读取到可以使用的打印模板",MB_OK);
		return;
	}


	CString file_path = HGTools::getCurExeContainPath();
	file_path = file_path + _T("\\picture");

	//CString strTmp;
	xmlHandler.GetXmlAttribute(elmUsed, "pictureName", strTmp);
	std::string s_f = strTmp;

	//多余的文件删一下
	//int totalSize = m_info.size() + m_panelPictureNumCount;
	//for (int i =0;i<totalSize; i++)
	//{
	//	CString newTempName;
	//	newTempName.Format("%s\\%s-%d.bmp",file_path,s_f.c_str(),i);
	//	if (HGTools::isFileExists(newTempName))
	//	{
	//		CFile::Remove(newTempName);
	//	}
	//}

	TransitionPrint(elmUsed);

	m_print_path.clear();
	for (int i = 0;i < m_print_info.size();i++)
	{
		CString src_file_name,file_name,file_code;
		src_file_name.Format("%s.bmp",s_f.c_str());
		file_name.Format("%s-%d.bmp",s_f.c_str(),/*i+m_panelPictureNumCount*/m_vComponentLableID[i]);
		file_code.Format("code.bmp");

		CString srcImage = file_path + "\\" + src_file_name;
		CString destImage = file_path + "\\" + file_name;

		m_print_path.push_back(destImage);
		BuildOneCodeImage(srcImage, destImage, elmUsed, m_print_info.at(i));

	}



	CString cstr_s_f;
	cstr_s_f.Format("%s",s_f.c_str());
	//sortFile(file_path,cstr_s_f);//对文件重新排序，以达到同一块板先大板后小板再到下一块板
}

/************************************************************************/
/* 图片生成tools                                                        */
/************************************************************************/

//通过路径获得图片尺寸
void CPrintCodeDlg::getImageWidthHeight(CString path , int& image_width, int& image_height)
{
	CImage image;
	image.Load(path);
	CDC* pImageDC = CDC::FromHandle(image.GetDC());
	image_width = image.GetWidth();
	image_height = image.GetHeight();
	image.ReleaseDC();
}

//绘制图片到指定位置
void CPrintCodeDlg::Paint_image(CDC* pDC, CString path ,int rotate, int pos_x, int pos_y )
{
	//加载指定图片
	CImage image;
	if(image.Load(path) != S_OK)
		return;
	CDC* pImageDC = CDC::FromHandle(image.GetDC());
	int image_width = image.GetWidth();
	int image_height = image.GetHeight();

	//列 用于画到pdc定位
	int col_begin = 0;	
	int col_index = 0;
	int col_setp = 1;
	int col_end = 0;

	//行 用于画到pdc定位
	int row_begin = 0;
	int row_index = 0;
	int row_setp = 1;
	int row_end = 0;

	int dir = 0;//纵向还是横向 0 等于横向，1等于纵向

	//根据要求指定数据绘制方向
	switch(rotate)
	{
	case 0:
		dir = 0;

		col_begin = pos_x;
		col_index = col_begin;
		col_setp = 1;
		col_end = col_begin + image_width;

		row_begin = pos_y;
		row_index = row_begin;
		row_setp = 1;
		row_end = row_begin + image_height;
		break;

	case 1:
		dir = 1;

		col_begin = pos_x;
		col_index = col_begin;
		col_setp = -1;
		col_end = col_begin + image_width;

		row_begin = pos_y;
		row_index = row_begin;
		row_setp = +1;
		row_end = row_begin - image_height;
		break;

	case 2:
		dir = 0;
		col_begin = pos_x;
		col_index = col_begin;
		col_setp = -1;
		col_end = col_begin - image_height;

		row_begin = pos_y;
		row_index = row_begin;
		row_setp = -1;
		row_end = row_begin - image_width;
		break;

	case 3:
		dir = 1;

		col_begin = pos_x;
		col_index = col_begin;
		col_setp = 1;
		col_end = col_begin - image_width;

		row_begin = pos_y;
		row_index = row_begin;
		row_setp = -1;
		row_end = row_begin + image_height;
		break;

	}
	//加载的图像的每一个点像素，刷到pdc对应的位置上
	int image_x = 0;
	int image_y = 0;
	if (dir == 0)
	{
		row_index = row_begin;
	}
	else
	{
		col_index = col_begin;
	}
	for (int y = 0; y < image_height ; y++)
	{
		if (dir == 0)
		{
			col_index = col_begin;
		} 
		else
		{
			row_index = row_begin;
		}
		for (int x = 0; x < image_width ; x++)
		{
			COLORREF pixel = pImageDC->GetPixel(x,y);
			pDC->SetPixel(col_index, row_index, pixel);
			
			if (dir == 0)
			{
				col_index += col_setp ;
			}
			else
			{
				row_index += row_setp;
			}
		}

		if (dir == 0)
		{
			row_index += row_setp;
		}
		else
		{
			col_index += col_setp ;
		}
		
	}


	image.ReleaseDC();
}

//生成板件图片
void CPrintCodeDlg::Paint_PanelImage(CDC* pDC, int offset_x, int offset_y,std::string s_width, std::string s_height, CString fontName,int fontSize,
	CString edge_img_up,CString edge_img_down,CString edge_img_right,CString edge_img_left,
	CString arrow_img_path,CString shape_img_path)
{
	bool bPanelXLonger = atof(s_width.c_str()) >= atof(s_height.c_str()) ? true : false;
	//////////////////////////////////////////////////////////////////////////
	int temp = 0;

	//缝隙
	int space_width = 3;
	//getImageWidthHeight(HGTools::getRelativePath("ico\\code_space.bmp"),space_width,temp);

	
	// mark 标注 |<  >| 
	int mark_width = 5;
	int mark_height = 16;
	getImageWidthHeight(HGTools::getRelativePath("ico\\code_size_left.bmp"),mark_width,mark_height);
	CString mark_left_img_path = HGTools::getRelativePath("ico\\code_size_left.bmp");
	CString mark_right_img_path = HGTools::getRelativePath("ico\\code_size_right.bmp");

	// edge 标注 △ 等
	int edge_width = 17;
	int edge_height = 9;
	getImageWidthHeight(HGTools::getRelativePath("ico\\code_edge_0.bmp"),edge_width,edge_height);
// 	CString edge_img_0 = HGTools::getRelativePath("ico\\code_edge_0.bmp");
// 	CString edge_img_1 = HGTools::getRelativePath("ico\\code_edge_1.bmp");
// 	CString edge_img_2 = HGTools::getRelativePath("ico\\code_edge_2.bmp");
// 	CString edge_img_3 = HGTools::getRelativePath("ico\\code_edge_3.bmp");
	edge_img_up;
	edge_img_down;
	edge_img_right;
	edge_img_left;

	// arrow 箭头 ← 等
	int arrow_width = 36;
	int arrow_height = 36;	
	getImageWidthHeight(HGTools::getRelativePath("ico\\code_arrow_0.bmp"),arrow_width,arrow_height);
// 	CString arrow_img_0 = HGTools::getRelativePath("ico\\code_arrow_0.bmp");
// 	CString arrow_img_1 = HGTools::getRelativePath("ico\\code_arrow_1.bmp");
// 	CString arrow_img_2 = HGTools::getRelativePath("ico\\code_arrow_2.bmp");
// 	CString arrow_img_3 = HGTools::getRelativePath("ico\\code_arrow_3.bmp");
	arrow_img_path;

	// shape 外轮廓 □ 等
	CImage image;
	image.Load(shape_img_path);
	int shape_width = image.GetWidth();
	int shape_height = image.GetHeight();
	//image.ReleaseDC();

// 	CString shape_img_0 = HGTools::getRelativePath("ico\\code_square_0.bmp");
// 	CString shape_img_1 = HGTools::getRelativePath("ico\\code_square_1.bmp");
// 	CString shape_img_2 = HGTools::getRelativePath("ico\\code_square_2.bmp");
	shape_img_path;

	//panel 板件图片尺寸
	int panel_img_width = space_width + edge_height + shape_width + edge_height + space_width + mark_height + space_width;
	int panel_img_height = space_width + edge_height + shape_height + edge_height + space_width + mark_height + space_width;

	//size 板件长宽文字
	CFont arcfont1,arcfont2;
	arcfont1.CreateFont(fontSize, 0, 900, 0, FW_NORMAL,FALSE, bPanelXLonger ? FALSE : TRUE,  FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, fontName);
	arcfont2.CreateFont(fontSize, 0,   0, 0, FW_NORMAL,FALSE, bPanelXLonger ? TRUE : FALSE,  FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, fontName);


	//////////////////////////////////////////////////////////////////////////

	// mark 坐标计算
	int mark_0_left_pos_x = offset_x + space_width + edge_height;
	int mark_0_left_pos_y = offset_y + space_width;

	int mark_0_text_pos_x = mark_0_left_pos_x + mark_width + space_width;
	int mark_0_text_pos_y = mark_0_left_pos_y + space_width;

	int mark_0_right_pos_x = offset_x + panel_img_width - space_width - mark_height - space_width - edge_height - mark_width;
	int mark_0_right_pos_y = offset_y + space_width;

	

	int mark_1_left_pos_x = offset_x + panel_img_width - space_width - mark_height;
	int mark_1_left_pos_y = offset_y + panel_img_height - space_width - edge_height;

	int mark_1_text_pos_x = mark_1_left_pos_x;
	int mark_1_text_pos_y = mark_1_left_pos_y - mark_width - space_width;
	
	int mark_1_right_pos_x = offset_x + panel_img_width - space_width - mark_height ;	
	int mark_1_right_pos_y = offset_y + space_width + mark_height + space_width + edge_height + mark_width;


	// shape 坐标计算
	int shape_pos_x = offset_x + space_width + edge_height;
	int shape_pos_y = offset_y + panel_img_height - space_width - edge_height - shape_height;

	// edge 坐标计算
	int edge_up_pos_x = shape_pos_x + shape_width/2 - edge_width/2;
	int edge_up_pos_y = shape_pos_y - edge_height;

	int edge_right_pos_x = shape_pos_x + shape_width + edge_height;
	int edge_right_pos_y = shape_pos_y + shape_height/2 - edge_width/2;

	int edge_down_pos_x = shape_pos_x + shape_width/2 + edge_width/2 ;
	int edge_down_pos_y = shape_pos_y + shape_height + edge_height ;

	int edge_left_pos_x = shape_pos_x - edge_height ;
	int edge_left_pos_y = shape_pos_y + shape_height/2 + edge_width/2;
	

	// arrow 坐标计算
	int arrow_pos_x = shape_pos_x + shape_width/2 - arrow_width/2;
	int arrow_pos_y = shape_pos_y + shape_height/2 - arrow_height/2;

	//////////////////////////////////////////////////////////////////////////

	//板件上面标注
	Paint_image(pDC, mark_left_img_path, 0, mark_0_left_pos_x, mark_0_left_pos_y);

	pDC->SelectObject(&arcfont2);
	pDC->TextOut(mark_0_text_pos_x, mark_0_text_pos_y-5, s_width.c_str());

	Paint_image(pDC, mark_right_img_path, 0, mark_0_right_pos_x, mark_0_right_pos_y);

	//if(atof(s_width.c_str()) > atof(s_height.c_str()))
	//{
	//	pDC->MoveTo(mark_0_left_pos_x, mark_0_left_pos_y+15);
	//	pDC->LineTo(mark_0_right_pos_x, mark_0_right_pos_y+15);
	//}

	//板件右面标注
	Paint_image(pDC, mark_left_img_path, 3, mark_1_left_pos_x, mark_1_left_pos_y);

	pDC->SelectObject(&arcfont1);
	pDC->TextOut(mark_1_text_pos_x, mark_1_text_pos_y, s_height.c_str());

	Paint_image(pDC, mark_right_img_path, 3, mark_1_right_pos_x, mark_1_right_pos_y);

	//if(atof(s_height.c_str()) > atof(s_width.c_str()))
	//{
	//	pDC->MoveTo(mark_1_left_pos_x+15, mark_1_left_pos_y);
	//	pDC->LineTo(mark_1_right_pos_x+15, mark_1_right_pos_y);
	//}

	//板件轮廓
	Paint_image(pDC, shape_img_path, 0, shape_pos_x, shape_pos_y);

	//箭头
	Paint_image(pDC, arrow_img_path, 0, arrow_pos_x, arrow_pos_y);

	//封边
	Paint_image(pDC, edge_img_up,		0, edge_up_pos_x,		edge_up_pos_y);;
	Paint_image(pDC, edge_img_right,	1, edge_right_pos_x,	edge_right_pos_y);
	Paint_image(pDC, edge_img_down,		2, edge_down_pos_x,		edge_down_pos_y);
	Paint_image(pDC, edge_img_left,		3, edge_left_pos_x,		edge_left_pos_y);

}

//生成小板标签图片
void CPrintCodeDlg::BuildOneCodeImage( CString sourceImagePath, CString destImagePath, /*hg3d::CXmlNode * print_settings*/TiXmlElement* elmPrintSetting, recode_print& printInfomation,bool dash)
{
	USES_CONVERSION;
	
	CImage image;
	image.Load(sourceImagePath);

	CPen pen(PS_SOLID,1,RGB(0,0,0));
	CPen pen_1(PS_DASH,1,RGB(0,0,0));
	CPen pen_2(PS_SOLID,2,RGB(0,0,0));
	CDC* pDC=CDC::FromHandle(image.GetDC());
	int _paperWidth;
	int _paperHeight;
	elmPrintSetting->Attribute("paperWidth", &_paperWidth);
	elmPrintSetting->Attribute("paperHeight", &_paperHeight);

	if (dash)
	{
		pDC->SelectObject(pen_1);
		pDC->Rectangle(1,1,_paperWidth-2,_paperHeight-2);
	}

	pDC->SelectObject(pen);
	CFont oldfont;
	oldfont.CreateFont(15, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("宋体"));
	
	//打印logo
	//hg3d::CXmlNode * logo_node = print_settings->GetChildByAttribute("Name","Logo");
	XmlHandler xmlHandler;
	TiXmlElement* elmLogo = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "Logo");
	//if (logo_node != NULL && logo_node->GetAttributeValue("Used") == "1")
	if (elmLogo != NULL && strcmp(elmLogo->Attribute("Used"), "1") == 0)
	{
		std::string logo_x = /*logo_node->GetAttributeValue("X")*/elmLogo->Attribute("X");
		std::string logo_y = /*logo_node->GetAttributeValue("Y")*/elmLogo->Attribute("Y");
		std::string logo_size = /*logo_node->GetAttributeValue("FontSize")*/elmLogo->Attribute("FontSize");

		std::string _logopath = printInfomation.logo_path;

		CImage image_logo;
		image_logo.Load(_logopath.c_str());

		CDC* pLogoDC = CDC::FromHandle(image_logo.GetDC());

		for (int L_x = 0;L_x < atoi(logo_size.c_str())*2;L_x++)
		{
			for (int L_y = 0;L_y < atoi(logo_size.c_str());L_y++)
			{
				pDC->SetPixel(L_x+atoi(logo_x.c_str()),L_y+atoi(logo_y.c_str()),pLogoDC->GetPixel(L_x,L_y));
			}
		}
		image_logo.ReleaseDC();
	}
	//打印一维码
	//hg3d::CXmlNode * onecode_node = print_settings->GetChildByAttribute("Name","一维码");
	TiXmlElement* elmOneCode = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "一维码");
	//if (onecode_node != NULL && onecode_node->GetAttributeValue("Used") == "1")
	if (elmOneCode != NULL && strcmp(elmOneCode->Attribute("Used"), "1") == 0)
	{
		std::string code_x = /*onecode_node->GetAttributeValue("X")*/elmOneCode->Attribute("X");
		std::string code_y = /*onecode_node->GetAttributeValue("Y")*/elmOneCode->Attribute("Y");
		std::string code_size = /*onecode_node->GetAttributeValue("FontSize")*/elmOneCode->Attribute("FontSize");

		std::string str_bar_code = printInfomation.code_info;

		MyBarcode(pDC,atoi(code_x.c_str()),atoi(code_y.c_str()),atoi(code_y.c_str())+20,39,atoi(code_size.c_str()),str_bar_code.c_str());
	}
	//打印反面一维码
	if (printInfomation.code_info2 != "")
	{
		//hg3d::CXmlNode * onecode_node2 = print_settings->GetChildByAttribute("Name","一维码2");
		TiXmlElement* elmOneCode2 = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "一维码2");
		//if (onecode_node2 != NULL && onecode_node2->GetAttributeValue("Used") == "1")
		if (elmOneCode2 != NULL && strcmp(elmOneCode2->Attribute("Used"), "1") == 0)
		{
			std::string code_x = /*onecode_node2->GetAttributeValue("X")*/elmOneCode2->Attribute("X");
			std::string code_y = /*onecode_node2->GetAttributeValue("Y")*/elmOneCode2->Attribute("Y");
			std::string code_size = /*onecode_node2->GetAttributeValue("FontSize")*/elmOneCode2->Attribute("FontSize");

			std::string str_bar_code = printInfomation.code_info2;

			MyBarcode(pDC,atoi(code_x.c_str()),atoi(code_y.c_str()),atoi(code_y.c_str())+20,39,atoi(code_size.c_str()),str_bar_code.c_str());
		}
	}
	//打印二维码
	//hg3d::CXmlNode * ercode_node = print_settings->GetChildByAttribute("Name","二维码");
	TiXmlElement* elmErCode = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "二维码");
	//if (ercode_node != NULL && ercode_node->GetAttributeValue("Used") == "1")
	if (elmErCode != NULL && strcmp(elmErCode->Attribute("Used"), "1") == 0)
	{
		std::string code_x = /*ercode_node->GetAttributeValue("X")*/elmErCode->Attribute("X");
		std::string code_y = /*ercode_node->GetAttributeValue("Y")*/elmErCode->Attribute("Y");
		std::string code_size = /*ercode_node->GetAttributeValue("FontSize")*/elmErCode->Attribute("FontSize");

		std::string str_bar_code = printInfomation.code_info;

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
								pDC->SetPixel(p_y+atoi(code_x.c_str()),p_x+atoi(code_y.c_str()),RGB(0,0,0));
							}
						}

					}
					pSourceData++;
				}
			}

			pDC->StretchBlt(atoi(code_x.c_str()),atoi(code_y.c_str()),40*code_plus,40*code_plus,pDC,atoi(code_x.c_str()),atoi(code_y.c_str()),unWidth,unWidth,SRCCOPY);
			QRcode_free(pQRC);
		}
		if (szSourceSring != NULL)
		{
			delete szSourceSring;
			szSourceSring = NULL;
		}
	}
	//打印反面二维码
	if (printInfomation.code_info2 != "")
	{
		//hg3d::CXmlNode * ercode_node2 = print_settings->GetChildByAttribute("Name","二维码2");
		TiXmlElement* elmErCode2 = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "二维码2");
		//if (ercode_node2 != NULL && ercode_node2->GetAttributeValue("Used") == "1")
		if (elmErCode2 != NULL && strcmp(elmErCode2->Attribute("Used"), "1") == 0)
		{
			//跳过AB钻只有一个，同时正反面条码相同
			if (!isZhuanSame_BarCodeSameJump(printInfomation))
			{
				std::string code_x = /*ercode_node2->GetAttributeValue("X")*/elmErCode2->Attribute("X");
				std::string code_y = /*ercode_node2->GetAttributeValue("Y")*/elmErCode2->Attribute("Y");
				std::string code_size = /*ercode_node2->GetAttributeValue("FontSize")*/elmErCode2->Attribute("FontSize");

				std::string str_bar_code = printInfomation.code_info2;



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
										pDC->SetPixel(p_y+atoi(code_x.c_str()),p_x+atoi(code_y.c_str()),RGB(0,0,0));
									}
								}

							}
							pSourceData++;
						}
					}
					pDC->StretchBlt(atoi(code_x.c_str()),atoi(code_y.c_str()),40*code_plus,40*code_plus,pDC,atoi(code_x.c_str()),atoi(code_y.c_str()),unWidth,unWidth,SRCCOPY);

					QRcode_free(pQRC);
				}
				if (szSourceSring != NULL)
				{
					delete szSourceSring;
					szSourceSring = NULL;
				}

			}
		}
	}
	
	//打印板件图片
	//hg3d::CXmlNode * p_picture_node = print_settings->GetChildByAttribute("Name","板件图片");
	TiXmlElement* elmPicture = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "板件图片");
	//if (p_picture_node != NULL && p_picture_node->GetAttributeValue("Used") == "1")
	if (elmPicture != NULL && strcmp(elmPicture->Attribute("Used"), "1") == 0)
	{
		ParamSettingDlg settings;
		settings.loadSettings();
		TiebiaoEdgeDlg tiebiao;
		tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());

		pDC->SelectObject(&pen_2);

		std::string picture_x = /*p_picture_node->GetAttributeValue("X")*/elmPicture->Attribute("X");
		std::string picture_y = /*p_picture_node->GetAttributeValue("Y")*/elmPicture->Attribute("Y");
		std::string picture_fontsize = /*p_picture_node->GetAttributeValue("FontSize")*/elmPicture->Attribute("FontSize");
		std::string picture_fontname = /*p_picture_node->GetAttributeValue("FontName")*/elmPicture->Attribute("FontName");

		//计算包围区域比例
		float _width = printInfomation._picture._width;
		float _height = printInfomation._picture._height;
		float whRate = _width / _height;
		CString panelImgPath = HGTools::getRelativePath("ico\\code_square_0.bmp");
		if (whRate > 1.25)	//宽比长 长很多
		{
			panelImgPath = HGTools::getRelativePath("ico\\code_square_1.bmp");
		}
		else if (whRate < 0.8)	//长比宽 长很多
		{
			panelImgPath = HGTools::getRelativePath("ico\\code_square_2.bmp");
		}
		
		//计算板件摆向
		CString arrowImgPath = HGTools::getRelativePath("ico\\code_arrow_0.bmp");
		TCHAR* a_szArrowDir[] = {"ico\\code_arrow_0.bmp", "ico\\code_arrow_3.bmp", "ico\\code_arrow_2.bmp", "ico\\code_arrow_1.bmp"};
		int nRotatedAngle = GetRegularAngle(printInfomation._picture.nRotatedAngle);
		if(nRotatedAngle < 0)
			nRotatedAngle += 360;
		arrowImgPath = HGTools::getRelativePath(a_szArrowDir[nRotatedAngle/90]);
		//if (printInfomation._picture._dir == 0)
		//{
		//	arrowImgPath = HGTools::getRelativePath("ico\\code_arrow_1.bmp");
		//}
		//else
		//{
		//	arrowImgPath = HGTools::getRelativePath("ico\\code_arrow_0.bmp");
		//}

		//生成图片
		Paint_PanelImage(pDC,
			atoi(picture_x.c_str()) - 8,atoi(picture_y.c_str()) - 32,
			printInfomation._picture.s_width, printInfomation._picture.s_height,
			picture_fontname.c_str(),atoi(picture_fontsize.c_str()),

			HGTools::getRealPathFromReleativePath(HGTools::getRelativePath(tiebiao.findImgPathByEdge(printInfomation._picture.up_edge).c_str())),
			HGTools::getRealPathFromReleativePath(HGTools::getRelativePath(tiebiao.findImgPathByEdge(printInfomation._picture.down_edge).c_str())),
			HGTools::getRealPathFromReleativePath(HGTools::getRelativePath(tiebiao.findImgPathByEdge(printInfomation._picture.right_edge).c_str())),
			HGTools::getRealPathFromReleativePath(HGTools::getRelativePath(tiebiao.findImgPathByEdge(printInfomation._picture.left_edge).c_str())),

			arrowImgPath,
			panelImgPath
			);

		pDC->SelectObject(&pen);
	}

	//打印板件图片
	TiXmlElement* elmDiagram = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", "排版图");
	if (elmDiagram != NULL && strcmp(elmDiagram->Attribute("Used"), "1") == 0)
	{
		float fX, fY;
		CString strValue;
		xmlHandler.GetXmlAttribute(elmDiagram, "X", fX);
		xmlHandler.GetXmlAttribute(elmDiagram, "Y", fY);
		xmlHandler.GetXmlAttribute(elmDiagram, "Value", strValue);

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

		CString strDiagramPath;
		int nLabelID;
		CString strLabelID;
		int nPos = destImagePath.ReverseFind('-');
		int nPos1 = destImagePath.ReverseFind('.');
		strLabelID = destImagePath.Mid(nPos + 1, nPos1 - nPos - 1);
		strDiagramPath.Format(_T("%sTmp\\%s.bmp"), destImagePath.Left(destImagePath.ReverseFind('\\')+1), strLabelID);
		//CImage image;
		//image.Load(strDiagramPath);

		//CDC dcImage;
		//dcImage.Attach(image.GetDC());
		//pDC->BitBlt(fX, fY, image.GetWidth(), image.GetHeight(), &dcImage, 0, 0, SRCCOPY);
		//dcImage.Detach();
		//image.ReleaseDC();

		Graphics g(pDC->m_hDC);
		Bitmap* pBitmap = Bitmap::FromFile(A2W(strDiagramPath));

		int nTmpX = (fX+nXLen/2);
		int nTmpY = (fY+nYLen/2);
		RectF rfDest(nTmpX -(pBitmap->GetWidth()/2.0), nTmpY -(pBitmap->GetHeight()/2.0), pBitmap->GetWidth(), pBitmap->GetHeight());

		g.TranslateTransform(nTmpX, nTmpY);
		g.RotateTransform(-nDiagramRotateAngle);
		g.TranslateTransform(-nTmpX, -nTmpY);
		g.DrawImage(pBitmap, rfDest, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), UnitPixel);
		g.ResetTransform();

		delete pBitmap;
	}

	//打印文字信息
	for (map<std::string,std::string>::iterator iter = printInfomation.printtext.begin();iter != printInfomation.printtext.end();iter++)
	{
		std::string _text_name = iter->first;
		std::string _text_value = iter->second;

		//hg3d::CXmlNode * text_node = print_settings->GetChildByAttribute("Name",_text_name.c_str());
		TiXmlElement* elmText = xmlHandler.GetIndicatedElement(elmPrintSetting, "Parameter", "Name", _text_name.c_str());
		//if (text_node != NULL && text_node->GetAttributeValue("Used") == "1")
		if (elmText != NULL && strcmp(elmText->Attribute("Used"), "1") == 0)
		{
			std::string text_x = /*text_node->GetAttributeValue("X")*/elmText->Attribute("X");
			std::string text_y = /*text_node->GetAttributeValue("Y")*/elmText->Attribute("Y");
			std::string text_fontsize = /*text_node->GetAttributeValue("FontSize")*/elmText->Attribute("FontSize");
			std::string text_fontname = /*text_node->GetAttributeValue("FontName")*/elmText->Attribute("FontName");
			std::string text_leftstr = /*text_node->GetAttributeValue("LeftStr")*/elmText->Attribute("LeftStr");
			std::string text_rightstr = /*text_node->GetAttributeValue("RightStr")*/elmText->Attribute("RightStr");

			CFont newfont;

			newfont.CreateFont(atoi(text_fontsize.c_str()), 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_ROMAN, (LPCSTR)text_fontname.c_str());

			pDC->SelectObject(newfont);
			CString str_text;
			str_text.Format("%s%s%s",text_leftstr.c_str(),_text_value.c_str(),text_rightstr.c_str());
			pDC->TextOut(atoi(text_x.c_str()),atoi(text_y.c_str()),str_text);
			pDC->SelectObject(oldfont);
		}
	}

	//打印Logo文本
	DrawLogoTextInLabel(pDC, CRect(0, 0, _paperWidth, _paperHeight));
	
	//结束，图片保存
	image.Save(destImagePath);
	image.ReleaseDC();
}

void CPrintCodeDlg::ScaleSaveImage(double scale, CString srcFile, CString dstFile )
{
	//原图片
	CImage image;
	image.Load(srcFile);
	CDC* pDC=CDC::FromHandle(image.GetDC());

	//导出图片
	CImage imageTo;
	imageTo.Create((int)(image.GetWidth()*scale),(int)(image.GetHeight()*scale),32);
	CDC* pDCTo = CDC::FromHandle(imageTo.GetDC());

	//按照指定比例缩放
	pDCTo->StretchBlt(0,0,(int)(image.GetWidth()*scale),(int)(image.GetHeight()*scale),pDC,0,0,image.GetWidth(),image.GetHeight(),SRCCOPY);

	//保存图片
	image.ReleaseDC();
	imageTo.Save(dstFile);
	imageTo.ReleaseDC();
}






//导出标签，大板是大图
void CPrintCodeDlg::OnExportPanelA4()
{
	ExportPanelPic(true);
}

void CPrintCodeDlg::ExportPanelPic(bool isPanelUseBigPic)
{
	//int bqbl = m_comboScaling.GetCurSel();
	double the_bl = 1;//比例默认为1：1
	//if (bqbl==1)
	//{
	//	the_bl = 2.5;//比例为1：4
	//}
	the_bl = getDaoChuBiaoQianBiLi();

	CString pathName;

	SelectPathDlg pathDlg;
	pathDlg.CurrentPath(pathDlg.DefaultSavePath());
	if (pathDlg.DoModal() == IDCANCEL)
	{
		return;
	}
	pathName = pathDlg.CurrentPath();
	//pathName = pathName+"\\pic";
	//HGTools::deletePath(pathName);
	//HGTools::createDir(pathName);

	if (!isPanelUseBigPic)
	{//导出样图设置信息
		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_path = ExeFile;
		file_path = file_path.Left(file_path.ReverseFind('\\'));
		file_path = file_path + _T("\\picture");

		//要处理NC二维码情况
		if (m_panelNCCode)
		{
			int picindex = 0;
			for (int i = 0;i < m_panelSmallPicPathList.size();i++)
			{
				if (i%2 == 0)
				{
					CString src_file_name,file_name,file_dst;
					file_dst.Format("%d.bmp",picindex+1);	

					ScaleSaveImage(the_bl, m_panelSmallPicPathList.at(i), pathName + "\\" + file_dst);
				}
				else
				{
					CString src_file_name,file_name,file_dst;
					file_dst.Format("_%d.bmp",picindex+1);	

					ScaleSaveImage(the_bl, m_panelSmallPicPathList.at(i), pathName + "\\" + file_dst);

					picindex++;
				}
			}
		}
		else
		{

			//正常输出当前预览的条码到导出目录
			int picindex = 0;
			for (int i = 0;i < m_panelSmallPicPathList.size();i++)
			{
				CString src_file_name,file_name,file_dst;
				file_dst.Format("A%03d.bmp",picindex+1);	

				ScaleSaveImage(the_bl, m_panelSmallPicPathList.at(i), pathName + "\\" + file_dst);

				picindex++;
			}
		}
	}//导出样图设置信息
	else
	{
		char ExeFile[2000];
		GetModuleFileName(NULL,ExeFile,500);
		CString file_path = ExeFile;
		file_path = file_path.Left(file_path.ReverseFind('\\'));
		file_path = file_path + _T("\\picture");

		{
			//正常输出当前预览的条码到导出目录
			int picindex = 0;
			for (int i = 0;i < m_panelBigPicPathList.size();i++)
			{
				CString src_file_name,file_name,file_dst;
				file_dst.Format("A%03d.bmp",picindex+1);	

				ScaleSaveImage(the_bl, m_panelBigPicPathList.at(i), pathName + "\\" + file_dst);

				picindex++;
			}
		}
	}



	{//导出标签

		for (int i = 0;i < m_print_path.size();i++)
		{
			CString src_file_name,file_name,bsrc_file_name;
			file_name.Format("%s.bmp",m_print_info.at(i).printtext["加工顺序"].c_str());

			//原图片
			CImage image;
			image.Load(m_print_path.at(i));
			CDC* pDC=CDC::FromHandle(image.GetDC());

			//导出图片
			CImage imageTo;
			imageTo.Create((int)(image.GetWidth()*the_bl),(int)(image.GetHeight()*the_bl),32);
			CDC* pDCTo = CDC::FromHandle(imageTo.GetDC());

			//按照指定比例缩放
			pDCTo->StretchBlt(0,0,(int)(image.GetWidth()*the_bl),(int)(image.GetHeight()*the_bl),pDC,0,0,image.GetWidth(),image.GetHeight(),SRCCOPY);

			//保存图片
			image.ReleaseDC();
			imageTo.Save(pathName + "\\" + file_name);
			imageTo.ReleaseDC();
		}
	}
	AfxMessageBox("导出完成");
}

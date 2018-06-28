// InOutDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "InOutDlg.h"
#include "afxdialogex.h"
//#include <Windows.h> 
#include "../Excel/CApplication.h"
#include "../Excel/CWorkbooks.h"
#include "../Excel/CWorkbook.h"
#include "../Excel/CWorksheets.h"
#include "../Excel/CWorksheet.h"
#include "../Excel/CRange.h"
#include "SelectPathDlg.h"
//#include "hg3d/XmlFile.h"
//#include "hg3d/Utils.h"
#include "../Misc/Misc.h"
#include "../Misc/HGTools.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

// CInOutDlg dialog

IMPLEMENT_DYNAMIC(CInOutDlg, CDialogEx)

CInOutDlg::CInOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInOutDlg::IDD, pParent)
{

}

CInOutDlg::~CInOutDlg()
{
}

void CInOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LISTMAP, m_listmap);
	//  DDX_Control(pDX, IDC_COMBO1, m_comlist);
	//  DDX_Control(pDX, IDC_LISTSELECT, m_listselect);
	DDX_Control(pDX, IDC_COMBO1, m_comselect);
}


BEGIN_MESSAGE_MAP(CInOutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInOutDlg::OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
//	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CInOutDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CInOutDlg message handlers


void CInOutDlg::OnBnClickedOk()
{
	CString strTmp;
	strTmp.Format(_T("kdata\\Excel%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//hg3d::CXmlFile * doc = new hg3d::CXmlFile;
	//doc->Load(hg3d::getFullFileName("\\kdata\\Excel.xml").c_str());
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
	TinyXml::TiXmlDocument doc(xmlPath);
	bool bLoadRes = doc.LoadFile();
	HGTools::deleteFile(xmlPath);
	//if (doc != NULL)
	if (bLoadRes)
	{
		XmlHandler xmlHandler;
		vector<TiXmlElement*> vMap = xmlHandler.GetChildElms(doc.RootElement(), "Map");

		//for (int doc_i = 0;doc_i < doc->GetChildNodes().size();doc_i++)
		for(int i = 0; i < vMap.size(); i++)
		{
			//hg3d::CXmlNode * child = doc->GetChildNodes().at(doc_i);
			TiXmlElement* elmMap = vMap[i];
			//if (child != NULL && child->GetName() == "Map")
			//{
				vector<TiXmlElement*> vItem = xmlHandler.GetChildElms(elmMap);
				//for (int doc_j = 0;doc_j < child->GetChildNodes().size();doc_j++)
				for(int j = 0; j < vItem.size(); j++)
				{
					//hg3d::CXmlNode * child_map = child->GetChildNodes().at(doc_j);
					TiXmlElement* elmItem = vItem[j];
					//if (child_map != NULL)
					//{
						//std::string outdata = child_map->GetAttributeValue("Output");
						CString strTmp;
						xmlHandler.GetXmlAttribute(elmItem, "Output", strTmp);
						std::string outdata = strTmp;
						CString s_in;
						s_in.Format("%d",m_inoutmap[atoi(outdata.c_str())]);
						//child_map->SetAttributeValue("Input",s_in.GetBuffer());
						elmItem->SetAttribute("Input", s_in.GetBuffer());
						s_in.ReleaseBuffer();
					//}
				}
			//}
		}
	}
	//doc->Save(hg3d::getFullFileName("\\kdata\\Excel.xml").c_str());
	doc.SaveFile(xmlPath);
	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
	HGTools::deleteFile(xmlPath);
	CDialogEx::OnOK();
}


BOOL CInOutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_comselect.InsertString(0,"请选择");
	m_comselect.InsertString(1,"订单号");
	m_comselect.InsertString(2,"柜号");
	m_comselect.InsertString(3,"条码编号");
	m_comselect.InsertString(4,"产品名称");
	m_comselect.InsertString(5,"成品名称");
	m_comselect.InsertString(6,"材质名称");
	m_comselect.InsertString(7,"开料长度");
	m_comselect.InsertString(8,"开料宽度");
	m_comselect.InsertString(9,"开料厚度");
	m_comselect.InsertString(10,"需切数量");
	m_comselect.InsertString(11,"纹路方向");
	m_comselect.InsertString(12,"封长1");
	m_comselect.InsertString(13,"封宽1");
	m_comselect.InsertString(14,"封长2");
	m_comselect.InsertString(15,"封宽2");
	m_comselect.InsertString(16,"异型");
	m_comselect.InsertString(17,"旋转");
	m_comselect.InsertString(18,"正面槽");
	m_comselect.InsertString(19,"槽翻转");
	m_comselect.InsertString(20,"反面槽");
	m_comselect.InsertString(21,"正面孔");
	m_comselect.InsertString(22,"孔翻转");
	m_comselect.InsertString(23,"反面孔");
	m_comselect.InsertString(24,"客户信息");
	m_comselect.InsertString(25,"加盟店");
	m_comselect.InsertString(26,"拉槽标识");
	m_comselect.InsertString(27,"助记号");
	m_comselect.InsertString(28,"钻孔");
	m_comselect.InsertString(29,"订单类型");
	m_comselect.InsertString(30,"反面条码");
	m_comselect.InsertString(31,"成品长度");
	m_comselect.InsertString(32,"成品宽度");
	m_comselect.InsertString(33,"成品厚度");
	m_comselect.InsertString(34,"异型ID");
	m_comselect.InsertString(35,"客户地址");
	m_comselect.InsertString(36,"钻槽标识");

	CString strTmp;
	strTmp.Format(_T("kdata\\Excel%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
	TinyXml::TiXmlDocument doc(xmlPath);
	bool bLoadRes = doc.LoadFile();
	HGTools::deleteFile(xmlPath);

	//hg3d::CXmlFile * doc = new hg3d::CXmlFile;
	//doc->Load(hg3d::getFullFileName("\\kdata\\Excel.xml").c_str());
	//if (doc != NULL)
	if(bLoadRes)
	{
		XmlHandler xmlHandler;
		vector<TiXmlElement*> vMap = xmlHandler.GetChildElms(doc.RootElement(), "Map");

		//for (int doc_i = 0;doc_i < doc->GetChildNodes().size();doc_i++)
		for(int i = 0; i < vMap.size(); i++)
		{
			//hg3d::CXmlNode * child = doc->GetChildNodes().at(doc_i);
			TiXmlElement* elmMap = vMap[i];

			//if (child != NULL && child->GetName() == "Map")
			//{
				vector<TiXmlElement*> vItem = xmlHandler.GetChildElms(elmMap);
				//for (int doc_j = 0;doc_j < child->GetChildNodes().size();doc_j++)
				for(int j = 0; j < vItem.size(); j++)
				{
					//hg3d::CXmlNode * child_map = child->GetChildNodes().at(doc_j);
					TiXmlElement* elmItem = vItem[j];
					//if (child_map != NULL)
					//{
						//std::string indata = child_map->GetAttributeValue("Input");
						CString strTmp;
						xmlHandler.GetXmlAttribute(elmItem, "Input", strTmp);
						std::string indata = strTmp;
						//std::string outdata = child_map->GetAttributeValue("Output");
						xmlHandler.GetXmlAttribute(elmItem, "Output", strTmp);
						std::string outdata = strTmp;
						m_inoutmap[atoi(outdata.c_str())] = atoi(indata.c_str());
					//}
				}
			//}
		}
	}

	CApplication ExcelApp;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	LPDISPATCH lpDisp = NULL;

	if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return false;
	}

	books.AttachDispatch(ExcelApp.get_Workbooks());



	SelectPathDlg pathDlg;
	CString pathName = pathDlg.SelectFileDlgDoModel(
		"导入开料单(excel)",
		"xls",
		"*",
		pathDlg.DefaultLoadPath(),
		DlgLoad
		);

	if( pathName == "" )
	{
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		return false;
	}

	try
	{
		lpDisp = books.Open(pathName, 
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing, vtMissing, vtMissing);
		book.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		AfxMessageBox(_T("打开文件失败"));
		return false;
	}


	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());

	CString strSheetName = _T("NewSheet");

	try
	{
		//lpDisp = sheets.get_Item(_variant_t(strSheetName));
		lpDisp = sheets.get__Default(_variant_t(1));
		sheet.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		AfxMessageBox(_T("打开文件失败"));
		return false;
	}

	range.AttachDispatch(sheet.get_UsedRange(), true);

	VARIANT varRead = range.get_Value2();
	COleSafeArray olesaRead(varRead);

	VARIANT varItem;
	CString strItem;
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaRead.GetLBound(1, &lFirstLBound);
	olesaRead.GetUBound(1, &lFirstUBound);
	olesaRead.GetLBound(2, &lSecondLBound);
	olesaRead.GetUBound(2, &lSecondUBound);
	long index[2] = {0, 0};

	int i = 1;

	index[0] = i;
	strValue.clear();
	for (long j = lSecondLBound; j <= lSecondUBound; j++)
	{
		index[1] = j;
		olesaRead.GetElement(index, &varItem);
		CString strItem;
		switch (varItem.vt)
		{
		case VT_R8:
			{
				int i_item = (int)varItem.dblVal;
				float f_item = (float)varItem.dblVal;

				if ((float)i_item - f_item > -0.01 && (float)i_item - f_item < 0.01)
					strItem.Format(_T("%d"),i_item);
				else
					strItem.Format(_T("%0.1f"),f_item);
				break;
			}

		case VT_BSTR:
			{
				strItem = varItem.bstrVal;
				break;
			}

		case VT_I4:
			{
				int i_item = (int)varItem.lVal;
				float f_item = (float)varItem.lVal;

				if ((float)i_item - f_item > -0.01 && (float)i_item - f_item < 0.01)
					strItem.Format(_T("%d"),i_item);
				else
					strItem.Format(_T("%0.1f"),f_item);

				//strItem.Format(_T("%0.1f"), (float)varItem.lVal);
				break;
			}

		default:
			{
				break;
			}
		}
		strValue.push_back(strItem);
	}	
	
	m_scrollindex = 0;
	this->SetScrollRange(SB_HORZ,1,strValue.size());
	this->SetScrollPos(SB_HORZ,1);

	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void CInOutDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	int pos = this->GetScrollPos(SB_HORZ);  

	switch (nSBCode)    
	{     
	case SB_LINEUP: 
		if (pos > 1)
			pos -= 1;    
		break;    
	case SB_LINEDOWN:
		if (pos < strValue.size())
			pos  += 1;    
		break;       
	case SB_TOP:    
		pos = 1;    
		break;    
	case SB_BOTTOM:    
		pos = strValue.size();    
		break;        
	case SB_THUMBPOSITION:    
		pos = nPos;    
		break;     
	default:    
		return;    
	}    

	this->SetScrollPos(SB_HORZ,pos);   
	m_scrollindex = pos-1;
	OnPaint();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CInOutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
	HDC hdc = this->GetDC()->GetSafeHdc();
	CDC* pDC=CDC::FromHandle(hdc);

	CBrush   *brush_noclick=CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(255,255,255))); 
	CRect rect;
	this->GetClientRect(&rect);
	pDC->SelectObject(brush_noclick);  
	pDC->BeginPath();
	pDC->MoveTo(0,0);
	pDC->LineTo(rect.Width(),0);
	pDC->LineTo(rect.Width(),rect.Height()-50);
	pDC->LineTo(0,rect.Height()-50);
	pDC->LineTo(0,0);
	pDC->EndPath();
	pDC->FillPath();

	//std::string image1_path = hg3d::getFullFileName("\\ico\\select_top.bmp");
	//std::string image2_path = hg3d::getFullFileName("\\ico\\select_item.bmp");
	std::string image1_path = GetModulePath() + _T("ico\\select_top.bmp");
	std::string image2_path = GetModulePath() + _T("ico\\select_item.bmp");
	CString str_path1,str_path2;
	str_path1.Format("%s",image1_path.c_str());
	str_path2.Format("%s",image2_path.c_str());

	for (int i = m_scrollindex;i < strValue.size();i++)
	{
		int _x = 10 + (i-m_scrollindex)*120;
		int _y = 10;
		CBitmap index_map;
		if (LoadBmpPic(str_path1,index_map) == true)
		{
			CDC SourceDC;
			SourceDC.CreateCompatibleDC(pDC); 
			SourceDC.SelectObject(&index_map);
			pDC->BitBlt(_x,_y,120,20,&SourceDC,0,0,SRCCOPY);
			SourceDC.DeleteDC();
		}
		CFont * font = this->GetFont();
		pDC->SelectObject(font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(_x+2,_y+2,strValue.at(i));

		if (LoadBmpPic(str_path2,index_map) == true)
		{
			CDC SourceDC;
			SourceDC.CreateCompatibleDC(pDC); 
			SourceDC.SelectObject(&index_map);
			pDC->BitBlt(_x,_y+20,120,20,&SourceDC,0,0,SRCCOPY);
			SourceDC.DeleteDC();
		}
		pDC->SetBkMode(TRANSPARENT);
		bool haveselect = false;
		for (auto iter_map = m_inoutmap.begin();iter_map != m_inoutmap.end();iter_map++)
		{
			int in_index = iter_map->second;
			int out_index = iter_map->first;

			if (in_index == i)
			{
				CString newstr;
				m_comselect.GetLBText(out_index+1,newstr);
				pDC->TextOut(_x+2,_y+22,newstr);
				haveselect = true;
				break;
			}
		}
		if (haveselect == false)
			pDC->TextOut(_x+2,_y+22,"请选择");
	}
	

}


BOOL CInOutDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	
	int pos = this->GetScrollPos(SB_HORZ); 
	pos = pos - zDelta/120;
	if (pos > 0 && pos <= strValue.size())
	{
		this->SetScrollPos(SB_HORZ,pos);   
		m_scrollindex = pos-1;
		OnPaint();
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

bool CInOutDlg::LoadBmpPic(const CString strBmpPath, CBitmap&bmp)
{
	HBITMAP  bitmap;  

	try  
	{  
		bmp.Detach();  
		bitmap =(HBITMAP)::LoadImage(NULL,strBmpPath, IMAGE_BITMAP, 0, 0,  
			LR_CREATEDIBSECTION|LR_LOADFROMFILE|LR_DEFAULTSIZE);  

		if(bmp.Attach(bitmap))  
		{  
			return true;  
		}  
	}  
	catch(...)  
	{  
		return false;  
	}  
	return true;  
}

void CInOutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	int p_x = point.x;
	int p_y = point.y;
	m_clickindex = (p_x - 10)/120 + m_scrollindex;
	if (p_y > 30 && p_y < 50 && m_clickindex < strValue.size())
	{

		CRect rc;
		rc.top = 30;
		rc.bottom = 50;
		rc.left = (m_clickindex - m_scrollindex)*120 + 10;
		rc.right = rc.left + 120;

		m_comselect.ShowWindow(SW_SHOW);
		m_comselect.MoveWindow(&rc);
		m_comselect.SetFocus();
		m_comselect.SetCurSel(0);
	}
	else
	{
		m_comselect.ShowWindow(SW_HIDE);
	}
	OnPaint();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CInOutDlg::OnCbnSelchangeCombo1()
{
	
	int key = 0;
	for (auto iter = m_inoutmap.begin();iter != m_inoutmap.end();iter++)
	{
		if (iter->second == m_clickindex)
		{
			m_inoutmap.erase(iter->first);
			break;
		}
		key++;
	}
	m_inoutmap[m_comselect.GetCurSel()-1] = m_clickindex;
}

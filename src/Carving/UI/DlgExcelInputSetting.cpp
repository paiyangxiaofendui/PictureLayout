// DlgExcelInputSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgExcelInputSetting.h"

#include "../Excel/CApplication.h"
#include "../Excel/CWorkbooks.h"
#include "../Excel/CWorkbook.h"
#include "../Excel/CWorksheets.h"
#include "../Excel/CWorksheet.h"
#include "../Excel/CRange.h"

//#include "SelectPathDlg.h"
#include <map>
#include "../Misc/HGTools.h"
#include "../Misc/Misc.h"
#include "../Misc/EncryptionInterface.h"
#include "../Work/WorkDef.h"

using namespace std;
// CDlgExcelInputSetting 对话框

IMPLEMENT_DYNAMIC(CDlgExcelInputSetting, CDialog)

CDlgExcelInputSetting::CDlgExcelInputSetting(LPCTSTR szFilePath, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExcelInputSetting::IDD, pParent)
	, m_strFilePath(szFilePath)
{

}

CDlgExcelInputSetting::~CDlgExcelInputSetting()
{
}

void CDlgExcelInputSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA_MAPPING, m_lcDataMapping);
}


BEGIN_MESSAGE_MAP(CDlgExcelInputSetting, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ROW1_AS_NAME, &CDlgExcelInputSetting::OnBnClickedRadio_Row1AsName)
	ON_BN_CLICKED(IDC_RADIO_ROW1_AS_DATA, &CDlgExcelInputSetting::OnBnClickedRadio_Row1AsData)
	ON_BN_CLICKED(IDOK, &CDlgExcelInputSetting::OnBnClickedOk)
	ON_NOTIFY(/*HDN_ENDTRACK*/HDN_ITEMCHANGING, 0, &CDlgExcelInputSetting::OnHdnItemChanging)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgExcelInputSetting 消息处理程序
BOOL CDlgExcelInputSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_RADIO_ROW1_AS_NAME))->SetCheck(1);


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

	if( m_strFilePath.IsEmpty())
	{
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();

		PostMessage(WM_CLOSE, 0, 0);
		return false;
	}

	try
	{
		lpDisp = books.Open(m_strFilePath, 
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

	long lRowCount = lFirstUBound - lFirstLBound + 1;
	long lColumnCount = lSecondUBound - lSecondLBound + 1;
	

	for(long i = lFirstLBound; i <= lFirstUBound; i++)
	{
		vector<CString> vOneColumn;
		for (long j = lSecondLBound; j <= lSecondUBound; j++)
		{
			index[0] = i;
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
			vOneColumn.push_back(strItem);
		}
		m_vAllRow.push_back(vOneColumn);
	}


	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();

	if(lRowCount <= 0 || lColumnCount <= 0)
	{
		AfxMessageBox(_T("此Excel表格没有数据！"));
		PostMessage(WM_CLOSE, 0, 0);
	}
	else
	{
		HWND hwnd = ::GetDlgItem(m_lcDataMapping.m_hWnd, 0); 
		if(hwnd) 
		{ 
			CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
			if(pHeader) 
			{ 
				if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl_ExcelSetting))) 
				{ 
					pHeader->Detach(); 
					m_hcDataMapping.SubclassWindow(hwnd); 
				} 
			} 
			else
			{
				m_hcDataMapping.SubclassWindow(hwnd);
			}
		}

		LONG dwStyle = GetWindowLong(m_lcDataMapping.m_hWnd, GWL_STYLE);
		SetWindowLong(m_lcDataMapping.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
		m_lcDataMapping.SetExtendedStyle((m_lcDataMapping.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);
		dwStyle = GetWindowLong(m_hcDataMapping.m_hWnd, GWL_STYLE);
		SetWindowLong(m_hcDataMapping.m_hWnd, GWL_STYLE, dwStyle | WS_CLIPCHILDREN);

		map<int, int> mapExcelToRequirement;
		CString strTmp;
		strTmp.Format(_T("kdata\\Excel%s"), g_szEncyptSubfix);
		CString hgePath = HGTools::getRelativePath(strTmp);
		CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
		decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
		TinyXml::TiXmlDocument doc(xmlPath);
		bool bLoadRes = doc.LoadFile();
		HGTools::deleteFile(xmlPath);

		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			vector<TiXmlElement*> vMap = xmlHandler.GetChildElms(doc.RootElement(), "Map");

			for(int i = 0; i < vMap.size(); i++)
			{
				TiXmlElement* elmMap = vMap[i];

				vector<TiXmlElement*> vItem = xmlHandler.GetChildElms(elmMap);
				for(int j = 0; j < vItem.size(); j++)
				{
					TiXmlElement* elmItem = vItem[j];
					CString strTmp;
					xmlHandler.GetXmlAttribute(elmItem, "Input", strTmp);
					std::string stringExcelColumnID = strTmp;
					xmlHandler.GetXmlAttribute(elmItem, "Output", strTmp);
					std::string stringRequirementListColumnID = strTmp;
					mapExcelToRequirement[atoi(stringExcelColumnID.c_str())] = atoi(stringRequirementListColumnID.c_str());
				}
			}
		}

		SwitchRow1Type(GetCurRow1Type());
		m_hcDataMapping.SetInitMapping(mapExcelToRequirement);
	}

	ArrangeUI();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDlgExcelInputSetting::OnBnClickedRadio_Row1AsName()
{
	// TODO: 在此添加控件通知处理程序代码
	map<int, int> mapCurMap = m_hcDataMapping.GetMapping();
	SwitchRow1Type(GetCurRow1Type());
	m_hcDataMapping.SetInitMapping(mapCurMap);
}

void CDlgExcelInputSetting::OnBnClickedRadio_Row1AsData()
{
	// TODO: 在此添加控件通知处理程序代码
	map<int, int> mapCurMap = m_hcDataMapping.GetMapping();
	SwitchRow1Type(GetCurRow1Type());
	m_hcDataMapping.SetInitMapping(mapCurMap);
}

CDlgExcelInputSetting::Row1Type CDlgExcelInputSetting::GetCurRow1Type()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_ROW1_AS_NAME))->GetCheck() == 1)
		return ROW1_AS_NAME;
	else
		return ROW1_AS_DATA;
}

void CDlgExcelInputSetting::SwitchRow1Type(Row1Type eRow1Type)
{
	if(m_vAllRow.size() > 0)
	{
		m_lcDataMapping.DeleteAllItems();
		while(m_lcDataMapping.GetHeaderCtrl()->GetItemCount() > 0)
		{
			m_lcDataMapping.DeleteColumn(m_lcDataMapping.GetHeaderCtrl()->GetItemCount() - 1);
			//m_hcDataMapping.DeleteItem(m_lcDataMapping.GetHeaderCtrl()->GetItemCount() - 1);
		}

		if(eRow1Type == ROW1_AS_DATA)
		{
			for(int i = 0; i < m_vAllRow[0].size(); i++)
			{
				CString strColumnName;
				strColumnName.Format(_T("F%d"), i+1);
				m_lcDataMapping.InsertColumn(i, strColumnName, 0, 120);


				//HDITEM item;
				//memset(&item, 0, sizeof(HDITEM));
				//item.cchTextMax = 100;
				//item.cxy = 120;
				//item.fmt = 0;
				//item.pszText = strColumnName.GetBuffer();
				//item.mask = HDI_FORMAT | HDI_TEXT | HDI_WIDTH;
				//m_hcDataMapping.InsertItem(i, &item);
				//strColumnName.ReleaseBuffer();

				//HDITEM item;
				//item.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
				//item.cxy = 100; // Make all columns 100 pixels wide.
				//item.fmt = HDF_STRING | HDF_CENTER;
				//item.pszText = strColumnName.GetBuffer(0);
				//m_hcDataMapping.InsertItem(i, &item);
			}

			int nItem = 0;
			for(int i = 0; i < m_vAllRow.size(); i++)
			{
				vector<CString> vOneRow = m_vAllRow[i];
				m_lcDataMapping.InsertItem(nItem, _T(""));
				for(int j = 0; j < vOneRow.size(); j++)
				{
					m_lcDataMapping.SetItemText(nItem, j, vOneRow[j]);
				}
				nItem++;
			}
		}
		else
		{
			vector<CString> vExcelColumn;
			vExcelColumn = m_vAllRow[0];
			for(int i = 0; i < vExcelColumn.size(); i++)
			{
				CString strColumnName;
				if(!vExcelColumn[i].IsEmpty())
					strColumnName = vExcelColumn[i];
				else
					strColumnName.Format(_T("F%d"), i+1);
				m_lcDataMapping.InsertColumn(i, strColumnName, 0, 120);

				//HDITEM item;
				//memset(&item, 0, sizeof(HDITEM));
				//item.cchTextMax = 100;
				//item.cxy = 120;
				//item.fmt = 0;
				//item.pszText = strColumnName.GetBuffer();
				//item.mask = HDI_FORMAT | HDI_TEXT | HDI_WIDTH;
				//m_hcDataMapping.InsertItem(i, &item);
				//strColumnName.ReleaseBuffer();

				//HDITEM item;
				//item.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
				//item.cxy = 100; // Make all columns 100 pixels wide.
				//item.fmt = HDF_STRING | HDF_CENTER;
				//item.pszText = strColumnName.GetBuffer(0);
				//m_hcDataMapping.InsertItem(i, &item);

			}

			int nItem = 0;
			for(int i = 1; i < m_vAllRow.size(); i++)
			{
				vector<CString> vOneRow = m_vAllRow[i];
				m_lcDataMapping.InsertItem(nItem, _T(""));
				for(int j = 0; j < vOneRow.size(); j++)
				{
					m_lcDataMapping.SetItemText(nItem, j, vOneRow[j]);
				}
				nItem++;
			}
		}

		m_hcDataMapping.ArrangeBtns();
	}
}

void CDlgExcelInputSetting::OnBnClickedOk()
{
	//CString hgePath = HGTools::getRelativePath("kdata\\Excel.hge");
	//CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	//decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
	//TinyXml::TiXmlDocument doc(xmlPath);
	//bool bLoadRes = doc.LoadFile();
	//HGTools::deleteFile(xmlPath);

	//if (bLoadRes)
	//{
	//	XmlHandler xmlHandler;
	//	vector<TiXmlElement*> vMap = xmlHandler.GetChildElms(doc.RootElement(), "Map");

	//	for(int i = 0; i < vMap.size(); i++)
	//	{	
	//		TiXmlElement* elmMap = vMap[i];
	//		vector<TiXmlElement*> vItem = xmlHandler.GetChildElms(elmMap);
	//		for(int j = 0; j < vItem.size(); j++)
	//		{
	//			TiXmlElement* elmItem = vItem[j];

	//			CString strTmp;
	//			xmlHandler.GetXmlAttribute(elmItem, "Output", strTmp);
	//			std::string outdata = strTmp;
	//			CString s_in;
	//			s_in.Format("%d",m_inoutmap[atoi(outdata.c_str())]);

	//			elmItem->SetAttribute("Input", s_in.GetBuffer());
	//			s_in.ReleaseBuffer();

	//		}
	//	}
	//}

	//doc.SaveFile(xmlPath);
	//encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
	//HGTools::deleteFile(xmlPath);
	//CDialogEx::OnOK();

	CString strTmp;
	strTmp.Format(_T("kdata\\Excel%s"), g_szEncyptSubfix);
	CString hgePath = HGTools::getRelativePath(strTmp);
	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);

	TiXmlDocument* xmlDoc = new TiXmlDocument();
	TiXmlElement* elmRoot = new TiXmlElement("Root");
	xmlDoc->LinkEndChild(elmRoot);

	TiXmlElement* elmImportingParam = new TiXmlElement("ImportingParam");
	elmRoot->LinkEndChild(elmImportingParam);
	elmImportingParam->SetAttribute("Row1AsData", GetCurRow1Type() == ROW1_AS_NAME ? "0" : "1");

	TiXmlElement* elmMap = new TiXmlElement("Map");
	elmRoot->LinkEndChild(elmMap);

	map<int, int> mapExcelToRequirement = m_hcDataMapping.GetMapping();
	for(map<int, int>::iterator itr = mapExcelToRequirement.begin(); itr != mapExcelToRequirement.end(); itr++)
	{
		if(itr->first != -1 && itr->second != -1)
		{
			TiXmlElement * elmParam = new TiXmlElement("Param");
			elmMap->LinkEndChild(elmParam);
			elmParam->SetAttribute("Input", itr->first);
			elmParam->SetAttribute("Output", itr->second);
		}
	}

	xmlDoc->SaveFile(xmlPath);
	delete xmlDoc;

	encrypt_base64(xmlPath.GetBuffer(), hgePath.GetBuffer());
	HGTools::deleteFile(xmlPath);

	CDialog::OnOK();
}

void CDlgExcelInputSetting::OnHdnItemChanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_hcDataMapping.ArrangeBtns();
	*pResult = 0;
}

void CDlgExcelInputSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ArrangeUI();
}

void CDlgExcelInputSetting::ArrangeUI()
{
	if(m_lcDataMapping.GetSafeHwnd())
	{
		CRect rcBtnOrg, rcBtn, rcClient;
		GetClientRect(rcClient);
		GetDlgItem(IDOK)->GetWindowRect(rcBtnOrg);
		ScreenToClient(rcBtn);
		rcBtn.SetRect(rcClient.right - 200, rcClient.bottom - 32, rcClient.right - 200 + rcBtnOrg.Width(), rcClient.bottom - 32 + rcBtnOrg.Height());
		GetDlgItem(IDOK)->MoveWindow(rcBtn);
		rcBtn.SetRect(rcClient.right - 100, rcClient.bottom - 32, rcClient.right - 100 + rcBtnOrg.Width(), rcClient.bottom - 32 + rcBtnOrg.Height());
		GetDlgItem(IDCANCEL)->MoveWindow(rcBtn);

		CRect rcListCtrlOrg, rcListCtrl;
		m_lcDataMapping.GetWindowRect(rcListCtrlOrg);
		ScreenToClient(rcListCtrlOrg);
		rcListCtrl.CopyRect(rcListCtrlOrg);
		rcListCtrl.right = rcClient.right - 10;
		rcListCtrl.bottom = rcClient.bottom - 40;
		m_lcDataMapping.MoveWindow(rcListCtrl);
	}
}
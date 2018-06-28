// DlgRemainder.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "DlgRemainder.h"
//#include "afxdialogex.h"
#include "../Misc/Misc.h"
#include <io.h>
#include "../Work/ProgramDef.h"


// CDlgRemainder 对话框

IMPLEMENT_DYNAMIC(CDlgRemainder, CDialogChildBase)

CDlgRemainder::CDlgRemainder(CWnd* pParent /*=NULL*/)
	: CDialogChildBase(CDlgRemainder::IDD, pParent)
{

}

CDlgRemainder::~CDlgRemainder()
{
}

void CDlgRemainder::DoDataExchange(CDataExchange* pDX)
{
	CDialogChildBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REMAINDER, m_lcRemainder);
}


BEGIN_MESSAGE_MAP(CDlgRemainder, CDialogChildBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_REMAINDER_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMAINDER_REMOVE, OnBtnRemove)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgRemainder 消息处理程序
void CDlgRemainder::ArrangeUI()
{
	CRect rcClient;
	GetClientRect(rcClient);

	if(m_lcRemainder.GetSafeHwnd())
	{
		CRect rcCtrl;
		rcCtrl.SetRect(rcClient.left, rcClient.top, rcClient.left + 500, rcClient.bottom);
		m_lcRemainder.MoveWindow(rcCtrl);
	}
}

BOOL CDlgRemainder::OnInitDialog()
{
	CDialog::OnInitDialog();

	HWND hwnd = ::GetDlgItem(m_lcRemainder.m_hWnd, 0); 
	if(hwnd) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcRemainder.SubclassWindow(hwnd); 
			} 
		} 
		else
		{
			m_hcRemainder.SubclassWindow(hwnd);
		}
	}

	LONG dwStyle = GetWindowLong(m_lcRemainder.m_hWnd, GWL_STYLE);
	SetWindowLong(m_lcRemainder.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_lcRemainder.SetExtendedStyle((m_lcRemainder.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	m_lcRemainder.InsertColumn(0,"X尺寸",0,100);
	m_lcRemainder.InsertColumn(1,"Y尺寸",0,100);
	m_lcRemainder.InsertColumn(2,"厚度",0,100);
	m_lcRemainder.InsertColumn(3,"材质",0,100);
	m_lcRemainder.InsertColumn(4,"数量",0,100);

	GetDlgItem(IDC_EDIT_REMAINDER_THICKNESS)->SetWindowText(_T("18"));
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(_T("1"));
	//ResetMaterialCombobox();

	return TRUE;
}

void CDlgRemainder::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	ArrangeUI();
}

void CDlgRemainder::OnBtnAdd()
{
	int nXLen = 0, nYLen = 0, nCount = 0;
	float fThickness = 0.0;
	CString strMaterial;

	CString strTmp;
	GetDlgItem(IDC_EDIT_SIZE_X)->GetWindowText(strTmp);
	nXLen = atoi(strTmp.GetBuffer());
	strTmp.ReleaseBuffer();

	GetDlgItem(IDC_EDIT_SIZE_Y)->GetWindowText(strTmp);
	nYLen = atoi(strTmp.GetBuffer());
	strTmp.ReleaseBuffer();

	GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(strTmp);
	nCount = 1/*atoi(strTmp.GetBuffer())*/;
	strTmp.ReleaseBuffer();

	GetDlgItem(IDC_COMBO_REMAINDER_MATERIAL)->GetWindowText(strMaterial);

	GetDlgItem(IDC_EDIT_REMAINDER_THICKNESS)->GetWindowText(strTmp);
	fThickness = atof(strTmp.GetBuffer());
	strTmp.ReleaseBuffer();

	bool bInputValidate = true;
	if(nXLen <= 0 || nYLen <= 0 || nCount <= 0 || fThickness <= 0.0 || strMaterial.IsEmpty())
		bInputValidate = false;
	if(!bInputValidate)
	{
		MessageBox(_T("数据不合法！"), NULL, MB_OK | MB_ICONWARNING);
		return;
	}

	int nItem = m_lcRemainder.InsertItem(m_lcRemainder.GetItemCount(), _T(""));
	strTmp.Format(_T("%d"), nXLen);
	m_lcRemainder.SetItemText(nItem, 0, strTmp);
	strTmp.Format(_T("%d"), nYLen);
	m_lcRemainder.SetItemText(nItem, 1, strTmp);
	strTmp.Format(_T("%.1f"), fThickness);
	m_lcRemainder.SetItemText(nItem, 2, strTmp);
	m_lcRemainder.SetItemText(nItem, 3, strMaterial);
	strTmp.Format(_T("%d"), nCount);
	m_lcRemainder.SetItemText(nItem, 4, strTmp);

}

void CDlgRemainder::OnBtnRemove()
{
	int nItem = m_lcRemainder.GetNextItem(-1,LVIS_SELECTED);
	if(nItem < 0)
	{
		MessageBox(_T("没有选中要移除的项！"), NULL, MB_OK);
		return;
	}

	while(nItem >= 0)
	{
		m_lcRemainder.DeleteItem(nItem);
		nItem = m_lcRemainder.GetNextItem(-1,LVIS_SELECTED);
	}
	m_lcRemainder.Invalidate();
}

vector<RemainderInputItem> CDlgRemainder::GetRemainderInputItem()
{
	vector<RemainderInputItem> vItem;
	for(int i = 0; i < m_lcRemainder.GetItemCount(); i++)
	{
		RemainderInputItem item;

		CString strTmp;
		strTmp = m_lcRemainder.GetItemText(i, 0);
		item.m_nXLen = atoi(strTmp.GetBuffer());
		strTmp.ReleaseBuffer();

		strTmp = m_lcRemainder.GetItemText(i, 1);
		item.m_nYLen = atoi(strTmp.GetBuffer());
		strTmp.ReleaseBuffer();

		strTmp = m_lcRemainder.GetItemText(i, 2);
		item.m_fThickness = atof(strTmp.GetBuffer());
		strTmp.ReleaseBuffer();

		strTmp = m_lcRemainder.GetItemText(i, 3);
		item.m_strMaterial = strTmp;
		strTmp.ReleaseBuffer();

		strTmp = m_lcRemainder.GetItemText(i, 4);
		item.m_nCount = atoi(strTmp.GetBuffer());
		strTmp.ReleaseBuffer();

		vItem.push_back(item);
	}
	return vItem;
}

void CDlgRemainder::SetRemainderInputItem(const vector<RemainderInputItem>& vRemainderInputItem)
{
	if(m_lcRemainder.GetSafeHwnd())
	{
		// 清空
		m_lcRemainder.DeleteAllItems();

		for(int i = 0; i < vRemainderInputItem.size(); i++)
		{
			const RemainderInputItem& theItem = vRemainderInputItem[i];
			CString strTmp;
			int nItem = m_lcRemainder.InsertItem(m_lcRemainder.GetItemCount(), _T(""));
			strTmp.Format(_T("%d"), theItem.m_nXLen);
			m_lcRemainder.SetItemText(nItem, 0, strTmp);
			strTmp.Format(_T("%d"), theItem.m_nYLen);
			m_lcRemainder.SetItemText(nItem, 1, strTmp);
			strTmp.Format(_T("%.1f"), theItem.m_fThickness);
			m_lcRemainder.SetItemText(nItem, 2, strTmp);
			m_lcRemainder.SetItemText(nItem, 3, theItem.m_strMaterial);
			strTmp.Format(_T("%d"), theItem.m_nCount);
			m_lcRemainder.SetItemText(nItem, 4, strTmp);
		}
	}
}

void CDlgRemainder::ResetMaterialCombobox()
{
	CString strTemporaryRecordFile = GetModulePath() + TEMPORARY_RECORD_FILE_DIR;
	TiXmlElement * elmHistoryMaterial = NULL;
	TiXmlDocument * xmlDoc = new TiXmlDocument();
	if(access(strTemporaryRecordFile, 0) == 0)
	{	
		bool bLoadRes = xmlDoc->LoadFile(strTemporaryRecordFile);
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			elmHistoryMaterial = xmlHandler.GetChildElm(xmlDoc->RootElement(), "HistoryMaterial");
			if(elmHistoryMaterial)
			{
				vector<TiXmlElement*> vPreExistItem = xmlHandler.GetChildElms(elmHistoryMaterial, "Item");
				if(vPreExistItem.size() > 0)
				{
					while(((CComboBox*)GetDlgItem(IDC_COMBO_REMAINDER_MATERIAL))->GetCount() > 0)
					{
						((CComboBox*)GetDlgItem(IDC_COMBO_REMAINDER_MATERIAL))->DeleteString(0);
					}
				}
				for(int i = 0; i < vPreExistItem.size(); i++)
				{
					CString strMaterial;
					xmlHandler.GetXmlAttribute(vPreExistItem[i], "Name", strMaterial);
					((CComboBox*)GetDlgItem(IDC_COMBO_REMAINDER_MATERIAL))->AddString(strMaterial);
				}
				((CComboBox*)GetDlgItem(IDC_COMBO_REMAINDER_MATERIAL))->SetCurSel(0);
			}
		}
	}

	delete xmlDoc;
	return;
}

void CDlgRemainder::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if(bShow == TRUE)
	{
		ResetMaterialCombobox();
	}
}
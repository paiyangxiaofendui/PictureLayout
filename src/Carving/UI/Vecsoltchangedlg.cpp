// Vecsoltchangedlg.cpp : implementation file
//

#include "stdafx.h"
#include "../Carving.h"
#include "Vecsoltchangedlg.h"
#include "afxdialogex.h"


// CVecsoltchangedlg dialog

IMPLEMENT_DYNAMIC(CVecsoltchangedlg, CDialogEx)

CVecsoltchangedlg::CVecsoltchangedlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVecsoltchangedlg::IDD, pParent)
{

	m_size = _T("");
	m_x = _T("");
	m_y = _T("");
}

CVecsoltchangedlg::~CVecsoltchangedlg()
{
}

void CVecsoltchangedlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITS, m_size);
	//  DDX_Control(pDX, IDC_EDITX, m_x);
	DDX_Text(pDX, IDC_EDITX, m_x);
	DDX_Text(pDX, IDC_EDITY, m_y);
}


BEGIN_MESSAGE_MAP(CVecsoltchangedlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVecsoltchangedlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVecsoltchangedlg message handlers


void CVecsoltchangedlg::OnBnClickedOk()
{
	
	UpdateData(TRUE);
	m_result = m_size;
	UpdateData(FALSE);
	CDialogEx::OnOK();
}

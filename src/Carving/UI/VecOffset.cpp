// VecOffset.cpp : implementation file
//

#include "stdafx.h"
#include "../Carving.h"
#include "VecOffset.h"
#include "afxdialogex.h"
#include "../Misc/Misc.h"


// CVecOffset dialog

IMPLEMENT_DYNAMIC(CVecOffset, CDialogEx)

CVecOffset::CVecOffset(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVecOffset::IDD, pParent)
{

	m_d = 0.0f;
	m_f = 0.0f;
}

CVecOffset::~CVecOffset()
{
}

void CVecOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ORIGINAL_HOLE_DIA, m_d);
	DDX_Text(pDX, IDC_EDIT_DIFF, m_f);
}


BEGIN_MESSAGE_MAP(CVecOffset, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVecOffset::OnBnClickedOk)
END_MESSAGE_MAP()


// CVecOffset message handlers


void CVecOffset::OnBnClickedOk()
{
	
	UpdateData(TRUE);
	//m_result = m_d + "," + m_f + "";
	m_result = GetFloatString(m_d, 0) + "," + GetFloatString(m_f, 0) + "";
	UpdateData(FALSE);
	CDialogEx::OnOK();
}

// PzCode.cpp : 实现文件
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "PzCode.h"
#include "afxdialogex.h"


// PzCode 对话框

IMPLEMENT_DYNAMIC(PzCode, CDialogEx)

PzCode::PzCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(PzCode::IDD, pParent)
{
	PzFirst1 = _T("");
	PzLast1 = _T("");
	PzFirst2 = _T("");
	PzLast2 = _T("");
}

PzCode::~PzCode()
{
}

void PzCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_PZEDIT1,PzFirst1);
	DDX_Text(pDX,IDC_PZEDITL1,PzLast1);
	DDX_Text(pDX,IDC_PZEDIT2,PzFirst2);
	DDX_Text(pDX,IDC_PZEDITL2,PzLast2);
}


BEGIN_MESSAGE_MAP(PzCode, CDialogEx)
END_MESSAGE_MAP()


// PzCode 消息处理程序

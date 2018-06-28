// BMainCode.cpp : 实现文件
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "BMainCode.h"
#include "afxdialogex.h"


// BMainCode 对话框

IMPLEMENT_DYNAMIC(BMainCode, CDialogEx)

BMainCode::BMainCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(BMainCode::IDD, pParent)
{
	BMainFirst1 = _T("");
	BMainLast1 = _T("");
	BMainFirst2 = _T("");
	BMainLast2 = _T("");
}

BMainCode::~BMainCode()
{
}

void BMainCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_BMAINEDIT1,BMainFirst1);
	DDX_Text(pDX,IDC_BMAINEDITL1,BMainLast1);
	DDX_Text(pDX,IDC_BMAINEDIT2,BMainFirst2);
	DDX_Text(pDX,IDC_BMAINEDITL2,BMainLast2);
}


BEGIN_MESSAGE_MAP(BMainCode, CDialogEx)
END_MESSAGE_MAP()


// BMainCode 消息处理程序

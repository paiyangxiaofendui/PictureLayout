// MainCode.cpp : 实现文件
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "MainCode.h"
#include "afxdialogex.h"


// MainCode 对话框

IMPLEMENT_DYNAMIC(MainCode, CDialogEx)

MainCode::MainCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(MainCode::IDD, pParent)
{
	MainFirst1 = _T("");
	MainLast1 = _T("");
	MainFirst2 = _T("");
	MainLast2 = _T("");
}

MainCode::~MainCode()
{
}

void MainCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_MAINEDIT1,MainFirst1);
	DDX_Text(pDX,IDC_MAINEDITL1,MainLast1);
	DDX_Text(pDX,IDC_MAINEDIT2,MainFirst2);
	DDX_Text(pDX,IDC_MAINEDITL2,MainLast2);
}


BEGIN_MESSAGE_MAP(MainCode, CDialogEx)
END_MESSAGE_MAP()


// MainCode 消息处理程序

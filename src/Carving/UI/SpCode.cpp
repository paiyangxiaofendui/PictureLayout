// SpCode.cpp : 实现文件
//

#include "stdafx.h"
#include "SpCode.h"
#include "afxdialogex.h"

// SpCode 对话框

IMPLEMENT_DYNAMIC(SpCode, CDialogEx)

SpCode::SpCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(SpCode::IDD, pParent)
{
	SpFirst1 = _T("");
	SpLast1 = _T("");
	SpFirst2 = _T("");
	SpLast2 = _T("");
}

SpCode::~SpCode()
{
}

void SpCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	
	DDX_Text(pDX,IDC_SPEDIT1,	SpFirst1);
	DDX_Text(pDX,IDC_SPEDITL1,	SpLast1);
	DDX_Text(pDX,IDC_SPEDIT2,	SpFirst2);
	DDX_Text(pDX,IDC_SPEDITL2,	SpLast2);
}


BEGIN_MESSAGE_MAP(SpCode, CDialogEx)
END_MESSAGE_MAP()


// SpCode 消息处理程序

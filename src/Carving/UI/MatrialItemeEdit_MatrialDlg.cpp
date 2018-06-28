// MatrialItemeEdit_MatrialDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrialItemeEdit_MatrialDlg.h"
#include "afxdialogex.h"


// MatrialItemeEdit_MatrialDlg 对话框

IMPLEMENT_DYNAMIC(MatrialItemeEdit_MatrialDlg, CDialogEx)

MatrialItemeEdit_MatrialDlg::MatrialItemeEdit_MatrialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MatrialItemeEdit_MatrialDlg::IDD, pParent)
	, m_MatrialNum(_T(""))
	, m_MatrialName(_T(""))
{

}

MatrialItemeEdit_MatrialDlg::~MatrialItemeEdit_MatrialDlg()
{
}

void MatrialItemeEdit_MatrialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SELECT_NUM, m_MatrialNum);
	DDX_CBString(pDX, IDC_EDIT_MATRIAL, m_MatrialName);
	DDX_Control(pDX, IDC_EDIT_MATRIAL, m_MatrialNameBox);
}


BEGIN_MESSAGE_MAP(MatrialItemeEdit_MatrialDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MatrialItemeEdit_MatrialDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// MatrialItemeEdit_MatrialDlg 消息处理程序

//显示当前材质列表
BOOL MatrialItemeEdit_MatrialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < m_MatrialNameList.size() ; i++)
	{
		m_MatrialNameBox.InsertString(i,m_MatrialNameList[i].c_str());
	}
	m_MatrialName = m_MatrialNameList[0].c_str();
	m_MatrialNameBox.SetCurSel(0);

	m_MatrialNum.Format("手工输入 或 在%d个材质中选择", m_MatrialNameList.size());

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//生效当前材质名称
void MatrialItemeEdit_MatrialDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

//追加到候选材质名称列表
void MatrialItemeEdit_MatrialDlg::AppendMatrialName( std::string name )
{
	for (int i = 0; i < m_MatrialNameList.size() ; i++)
	{
		if (m_MatrialNameList[i].compare(name) == 0)
		{
			return;
		}
	}
	m_MatrialNameList.push_back(name);
}

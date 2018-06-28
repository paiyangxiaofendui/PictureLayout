// DlgResetMachiningInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgResetMachiningInfo.h"


// CDlgResetMachiningInfo 对话框

IMPLEMENT_DYNAMIC(CDlgResetMachiningInfo, CDialog)

CDlgResetMachiningInfo::CDlgResetMachiningInfo(int nResetType, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResetMachiningInfo::IDD, pParent)
{
	m_nResetOtherShape = 0;
	m_nResetSlot_UpperFace = 0;
	m_nResetSlot_DownerFace = 0;
	m_nResetHole_UpperFace = 0;
	m_nResetHole_DownerFace = 0;

	if(nResetType == RESET_OTHERSHAPE)
		m_nResetOtherShape = 1;
	else if(nResetType == RESET_SLOT)
		m_nResetSlot_UpperFace = 1;
	else if(nResetType == RESET_HOLE)
		m_nResetHole_UpperFace = 1;
}

CDlgResetMachiningInfo::~CDlgResetMachiningInfo()
{
}

void CDlgResetMachiningInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_RESET_OTER_SHAPE, m_nResetOtherShape);
	DDX_Check(pDX, IDC_CHECK_RESET_SLOT_UPPERFACE, m_nResetSlot_UpperFace);
	DDX_Check(pDX, IDC_CHECK_RESET_SLOT_DOWNERFACE, m_nResetSlot_DownerFace);
	DDX_Check(pDX, IDC_CHECK_RESET_HOLE_UPPERFACE, m_nResetHole_UpperFace);
	DDX_Check(pDX, IDC_CHECK_RESET_HOLE_DOWNERFACE, m_nResetHole_DownerFace);
}


BEGIN_MESSAGE_MAP(CDlgResetMachiningInfo, CDialog)
END_MESSAGE_MAP()


// CDlgResetMachiningInfo 消息处理程序
void CDlgResetMachiningInfo::OnOK()
{
	UpdateData(TRUE);

	CDialog::OnOK();
}
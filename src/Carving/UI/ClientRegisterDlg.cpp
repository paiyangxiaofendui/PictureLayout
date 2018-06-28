#include "stdafx.h"

#include "ClientRegisterDlg.h"
#include "../../../include/FileReadWrite/ClientInfoReadWrite/ClientInfoReadWrite.h"
#include "../../../include/FileReadWrite/ClientInfoReadWrite/HardwareInfo.h"
#include "../../../include/InternetManager/InternetChecking/InternetChecking.h"
#include "../Misc/Misc.h"
#include "../../../include/TaoBaoSender/TaoBaoSenderInterface.h"
#include "../Work/WorkDef.h"

#define VERIFY_CODE_LEN 6
#define ID_TIMER_RESEND_COUNTING 0
#define ID_TIMER_REENABLE_SEND_BUTTON 1
#ifndef YiWei
	#define VERIFY_CODE_LIMIT_SENDING_COUNT 3
#else
	#define VERIFY_CODE_LIMIT_SENDING_COUNT 3
#endif

#define RE_ENABLE_SENDING_BUTTON_TIME 60

IMPLEMENT_DYNAMIC(ClientRegisterDlg, CDialogEx)

ClientRegisterDlg::ClientRegisterDlg(CWnd* pParent /* = NULL */)
	:CDialogEx(ClientRegisterDlg::IDD, pParent)
	, m_pImgBackground(NULL)
	, m_nVerifyCodeSendedCount(0)
{

	m_RegisterOrUpdate = Dlg_Register;

	// 读取客户信息
// 	ClientInfoReadWrite::ReadClientInfo();
// 
// 	m_valClientQQNumber = ClientInfoReadWrite::m_Info.m_QQNumber;
// 	m_valClientPhoneNumber = ClientInfoReadWrite::m_Info.m_PhoneNumber;
// 	m_valClientAddress = ClientInfoReadWrite::m_Info.m_Address;
// 	m_valClientName = ClientInfoReadWrite::m_Info.m_ClientName;
}

ClientRegisterDlg::~ClientRegisterDlg()
{
	if(m_pImgBackground)
		delete m_pImgBackground;

	if(m_fontEdit.GetSafeHandle())
		m_fontEdit.DeleteObject();
}


BEGIN_MESSAGE_MAP(ClientRegisterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SEND_CODE, &ClientRegisterDlg::OnBtnSendCodeToPhone)
	ON_WM_TIMER()
END_MESSAGE_MAP()



void ClientRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_QQ_NUMBER, m_valClientQQNumber);
//	DDV_MaxChars(pDX, m_valClientQQNumber, 20);

	DDX_Text(pDX, IDC_EDIT_PHONE_NUMBER, m_valClientPhoneNumber);
	DDV_MaxChars(pDX, m_valClientPhoneNumber, 11);

	DDX_Text(pDX, IDC_EDIT_CLIENT_ADDRESS, m_valClientAddress);
	//DDV_MaxChars(pDX, m_valClientAddress, 48);

	DDX_Text(pDX, IDC_EDIT_CLIENT_NAME, m_valClientName);
	//DDV_MaxChars(pDX, m_valClientName, 48);

	DDX_Control(pDX, IDC_STATIC_LINK, m_Link);
	DDX_Control(pDX, IDOK, m_btnRegister);
	DDX_Control(pDX, IDC_BUTTON_SEND_CODE, m_btnSendVerifyCodeToPhone);
}



BOOL ClientRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

#ifndef YiWei
	m_pImgBackground = LoadPngImgFromRes(IDB_PNG_BACKGROUND_REGISTER);
#else
	m_pImgBackground = LoadPngImgFromRes(IDB_PNG_BACKGROUND_REGISTER_YW);
#endif


	CRect rcWnd, rcClient;
	GetWindowRect(rcWnd);
	GetClientRect(rcClient);
	rcWnd.InflateRect(m_pImgBackground->GetWidth() - rcClient.Width(), m_pImgBackground->GetHeight() - rcClient.Height(), 0, 0);
	MoveWindow(rcWnd);
	CenterWindow();

	m_Link.SetWindowText(g_szCompanyWebSide);
	m_Link.SetLink(g_szCompanyWebSide);

	m_btnRegister.LoadImageFromRes(IDB_PNG_BTN_REGISTER_NORMAL, IDB_PNG_BTN_REGISTER_HOVER, IDB_PNG_BTN_REGISTER_HOVER, IDB_PNG_BTN_REGISTER_NORMAL, 0);
	m_btnSendVerifyCodeToPhone.LoadImageFromRes(IDB_PNG_BTN_SEND_TO_PHONE_NORMAL, IDB_PNG_BTN_SEND_TO_PHONE_HOVER, IDB_PNG_BTN_SEND_TO_PHONE_HOVER, IDB_PNG_BTN_SENT_TO_PHONE_DISABLE, 0);
	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);
	m_btnSendVerifyCodeToPhone.SetStringFormat(sf);
	m_btnSendVerifyCodeToPhone.SetTextColorEx(RGB(255,255,255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 20;
	strcpy(lf.lfFaceName, _T("Segoe UI"));
	m_fontEdit.CreateFontIndirect(&lf);
	GetDlgItem(IDC_EDIT_CLIENT_NAME)->SetFont(&m_fontEdit);
	GetDlgItem(IDC_EDIT_PHONE_NUMBER)->SetFont(&m_fontEdit);
	GetDlgItem(IDC_EDIT_QQ_NUMBER)->SetFont(&m_fontEdit);
	GetDlgItem(IDC_EDIT_CLIENT_ADDRESS)->SetFont(&m_fontEdit);
	GetDlgItem(IDC_EDIT_VERIFY_CODE)->SetFont(&m_fontEdit);

	CRect rcCtrl;
	rcCtrl.SetRect(113, 93, 263, 116);
	GetDlgItem(IDC_EDIT_CLIENT_NAME)->MoveWindow(rcCtrl);
	rcCtrl.SetRect(113, 125, 263, 148);
	GetDlgItem(IDC_EDIT_PHONE_NUMBER)->MoveWindow(rcCtrl);
	rcCtrl.SetRect(113, 158, 263, 181);
	GetDlgItem(IDC_EDIT_QQ_NUMBER)->MoveWindow(rcCtrl);
	rcCtrl.SetRect(113, 190, 263, 213);
	GetDlgItem(IDC_EDIT_CLIENT_ADDRESS)->MoveWindow(rcCtrl);

	rcCtrl.SetRect(113, 221, 185, 244);
	GetDlgItem(IDC_EDIT_VERIFY_CODE)->MoveWindow(rcCtrl);
	rcCtrl.SetRect(186, 221, 286, 241);
	GetDlgItem(IDC_BUTTON_SEND_CODE)->MoveWindow(rcCtrl);

	rcCtrl.SetRect(44, 262, 289, 281);
	GetDlgItem(IDOK)->MoveWindow(rcCtrl);
	rcCtrl.SetRect(71, 318, 287, 333);
	GetDlgItem(IDC_STATIC_LINK)->MoveWindow(rcCtrl);

	//CRect rcCtrl;
	//GetDlgItem(IDC_EDIT_VERIFY_CODE)->GetWindowRect(rcCtrl);
	//ScreenToClient(rcCtrl);
	//CString strTmp;
	//strTmp.Format(_T("Left%s top%s rig%s b%s\n"), GetFloatString(rcCtrl.left, 0), GetFloatString(rcCtrl.top, 0), GetFloatString(rcCtrl.right, 0), GetFloatString(rcCtrl.bottom, 0));
	//OutputDebugString(strTmp);

	GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText("发验证码到手机");			
		
		
		

	return TRUE;
}


void ClientRegisterDlg::OnCancel()
{

	CDialogEx::OnCancel();


	AfxGetMainWnd()->PostMessage(WM_QUIT); 
}



void ClientRegisterDlg::OnOK()
{

	// 更新数据
	UpdateData(TRUE);

	// 检查数据合法性

	// 判断姓名 不能为空，不能有数组
	if (m_valClientName.IsEmpty() == TRUE)
	{
		AfxMessageBox("姓名不能为空！");
		return ;
	}
	else if (m_valClientName.GetLength() < 4)
	{
		AfxMessageBox("姓名格式不正确！");
		return ;
	}
	else
	{
		// 判断是否有数字
		for(int n = 0;n < m_valClientName.GetLength(); n++)
		{
			if(m_valClientName[n] >= '0' && m_valClientName[n] < '9')
			{
				AfxMessageBox("姓名格式不正确！");
				return ;
			}
		}
	}

	// 判断手机号位数为11位，必须全为数字
	if (m_valClientPhoneNumber.IsEmpty() == TRUE)
	{
		AfxMessageBox("手机号码不能为空！");
		return ;
	}
	else
	{
		if (m_valClientPhoneNumber.GetLength() != 11)
		{
			AfxMessageBox("手机号码格式不正确！");
			return ;
		}

		char first_num = m_valClientPhoneNumber.GetAt(0);
		if ( first_num != '1')
		{
			AfxMessageBox("手机号码格式不正确！");
			return ;
		}

		// 判断是否全为数字
		for(int n = 0;n < m_valClientPhoneNumber.GetLength(); n++)
		{
			if(m_valClientPhoneNumber[n]<'0' || m_valClientPhoneNumber[n]>'9')
			{
				AfxMessageBox("手机号码格式不正确！");
				return ;
			}
		}
	}

	// 判断qq号码不能为空、必须全为数字
	if (m_valClientQQNumber.IsEmpty() == TRUE)
	{
		AfxMessageBox("QQ号不能为空！");
		return ;
	}
	else if (m_valClientQQNumber.GetLength() < 5)
	{
		AfxMessageBox("QQ号格式不正确！");
		return ;
	}
	else
	{
		char first_num = m_valClientQQNumber.GetAt(0);
		if ( first_num == '0')
		{
			AfxMessageBox("qq号码格式不正确！");
			return ;
		}


		// 判断是否全为数字
		for(int n = 0;n < m_valClientQQNumber.GetLength(); n++)
		{
			if(m_valClientQQNumber[n]<'0' || m_valClientQQNumber[n]>'9')
			{
				AfxMessageBox("QQ号码格式不正确！");
				return ;
			}
		}
	}


	if (m_valClientAddress.IsEmpty() == TRUE)
	{
		AfxMessageBox("地址不能为空！");
		return ;
	}

	CString strTheActualPhoneNum;
	if(!IsIgnoreVerifyPhoneNumMode())
	{
		CString strVerifyCodeInput;
		GetDlgItem(IDC_EDIT_VERIFY_CODE)->GetWindowText(strVerifyCodeInput);
		for(vector<pair<CString, CString>>::iterator itr = m_vVerifyCodePhoneNumPair.begin(); itr != m_vVerifyCodePhoneNumPair.end(); itr++)
		{
			if(strVerifyCodeInput.Compare(itr->first) == 0)
			{
				strTheActualPhoneNum = itr->second;
				break;
			}
		}
		if(strTheActualPhoneNum.IsEmpty())
		{
			AfxMessageBox("验证码不正确！");
			return;
		}
	}
	else
	{
		strTheActualPhoneNum = m_valClientPhoneNumber;
	}

	// 能到这里已经说明电脑可以联网，但可能由于未确定原因，无法连接CRM

#if 0

	if (m_RegisterOrUpdate == Dlg_Register)
	{
		// 网络连接，如果无法连接，则提示，并死循环
		if (InternetChecking::AccessCRM(m_valClientName, /*m_valClientPhoneNumber*/strTheActualPhoneNum, m_valClientQQNumber, m_valClientAddress) == true)
		{
			// 重置注册文件
			ClientInfoReadWrite::ResetClientInfo();

			// 写注册文件
			ClientInfoReadWrite::m_Info.m_ClientName	= m_valClientName;
			ClientInfoReadWrite::m_Info.m_PhoneNumber	= /*m_valClientPhoneNumber*/strTheActualPhoneNum;
			ClientInfoReadWrite::m_Info.m_QQNumber		= m_valClientQQNumber;
			ClientInfoReadWrite::m_Info.m_Address		= m_valClientAddress;

			ClientInfoReadWrite::WriteClientInfo();

			CDialogEx::OnOK();
		} 
		else
		{
			AfxMessageBox("网络连接失败，请检查网络！");
		}
	}
	else
	{

		// 网络连接，如果无法连接，则提示，并死循环
		if (InternetChecking::AccessCRM(m_valClientName, /*m_valClientPhoneNumber*/strTheActualPhoneNum, m_valClientQQNumber, m_valClientAddress) == true)
		{
			// 重置注册文件
			ClientInfoReadWrite::ResetClientInfo();


			// 写注册文件
			ClientInfoReadWrite::m_Info.m_ClientName	= m_valClientName;
			ClientInfoReadWrite::m_Info.m_PhoneNumber	= /*m_valClientPhoneNumber*/strTheActualPhoneNum;
			ClientInfoReadWrite::m_Info.m_QQNumber		= m_valClientQQNumber;
			ClientInfoReadWrite::m_Info.m_Address		= m_valClientAddress;


			ClientInfoReadWrite::WriteClientInfo();

			CDialogEx::OnOK();
		} 
		else
		{
			AfxMessageBox("网络连接失败，请检查网络！");
		}
	}

#else

	// 网络连接，如果无法连接，则提示，并死循环
	if (InternetChecking::AccessCRM(m_valClientName, /*m_valClientPhoneNumber*/strTheActualPhoneNum, m_valClientQQNumber, m_valClientAddress) == false)
	{
		AfxMessageBox("网络连接失败，请检查网络！");
	}

	// 重置注册文件
	ClientInfoReadWrite::ResetClientInfo();

	// 写注册文件
	ClientInfoReadWrite::m_Info.m_ClientName	= m_valClientName;
	ClientInfoReadWrite::m_Info.m_PhoneNumber	= /*m_valClientPhoneNumber*/strTheActualPhoneNum;
	ClientInfoReadWrite::m_Info.m_QQNumber		= m_valClientQQNumber;
	ClientInfoReadWrite::m_Info.m_Address		= m_valClientAddress;

	ClientInfoReadWrite::WriteClientInfo();
	CDialogEx::OnOK();
	

#endif


	
	
}

void ClientRegisterDlg::OnPaint()
{
	USES_CONVERSION;
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);


	Graphics g(dcMem.m_hDC);
	Gdiplus::Font font13(L"Segoe UI", 13, FontStyleRegular, UnitPixel);
	StringFormat sf;
	SolidBrush brushBlue(Color(34, 164, 227));


	g.DrawImage(m_pImgBackground, RectF(0, 0, rcClient.Width(), rcClient.Height()), 0, 0, m_pImgBackground->GetWidth(), m_pImgBackground->GetHeight(), UnitPixel);

	RectF rfText(40, 285, 500, 30);
	CStringW strText;
	strText.Format(L"无法注册？联系客服热线：%s", A2W(g_szCustomServiceTel));
	g.DrawString(strText, -1, &font13, rfText, &sf, &brushBlue);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}

BOOL ClientRegisterDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void ClientRegisterDlg::OnBtnSendCodeToPhone()
{
	UpdateData(TRUE);
	CString strPhoneNum = m_valClientPhoneNumber;
	CString strRandomCode = GetRandomCode();
	if(strPhoneNum.IsEmpty())
	{
		AfxMessageBox("请输入手机号码！");
		return;
	}

	GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText("正在发送");
	GetDlgItem(IDC_BUTTON_SEND_CODE)->EnableWindow(FALSE);

	CString strToSend;
	strToSend.Format(_T("%s"), strRandomCode);
	int nRes = SendStringToPhone(strPhoneNum, strToSend);
	if(nRes == 10006)
	{
		AfxMessageBox("验证码发送失败，请检查网络连接后重试！");
		GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText("发送验证码到手机");
		GetDlgItem(IDC_BUTTON_SEND_CODE)->EnableWindow(TRUE);
		return;
	}

	m_nVerifyCodeSendedCount++;
	if(IsIgnoreVerifyPhoneNumMode())
	{
		GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText("重新发送");
		GetDlgItem(IDC_EDIT_VERIFY_CODE)->SetWindowText(strRandomCode);
		GetDlgItem(IDC_EDIT_VERIFY_CODE)->EnableWindow(FALSE);
		PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)0);  
		return;
	}

	CString strBtnTxt;
	strBtnTxt.Format(_T("重新发送(%ds)"), RE_ENABLE_SENDING_BUTTON_TIME);
	GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText(strBtnTxt);
	SetTimer(ID_TIMER_RESEND_COUNTING, 1000, NULL);
	//SetTimer(ID_TIMER_REENABLE_SEND_BUTTON, 30000);
	m_nReEnableSendingButtonRemainTime = RE_ENABLE_SENDING_BUTTON_TIME;
	m_vVerifyCodePhoneNumPair.push_back(make_pair(strRandomCode, strPhoneNum));
	::SetFocus(GetDlgItem(IDC_EDIT_VERIFY_CODE)->GetSafeHwnd());
}

CString ClientRegisterDlg::GetRandomCode()
{
	CString strRet;

	srand((int)time(NULL));

	CString strRandNum;
	while(strRandNum.GetLength() < VERIFY_CODE_LEN)
	{
		CString strTmp;
		strTmp.Format(_T("%d"), rand());
		strRandNum += strTmp;
	}
	strRandNum = strRandNum.Left(VERIFY_CODE_LEN);

	strRet = strRandNum;
	return strRet;
}

void ClientRegisterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == ID_TIMER_RESEND_COUNTING)
	{
		m_nReEnableSendingButtonRemainTime--;
		if(m_nReEnableSendingButtonRemainTime <= 0)
		{
			GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText("重新发送");
			GetDlgItem(IDC_BUTTON_SEND_CODE)->EnableWindow(TRUE);
			KillTimer(ID_TIMER_RESEND_COUNTING);
		}
		else
		{
			CString strTmp;
			strTmp.Format(_T("重新发送(%ds)"), m_nReEnableSendingButtonRemainTime);
			GetDlgItem(IDC_BUTTON_SEND_CODE)->SetWindowText(strTmp);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

bool ClientRegisterDlg::IsIgnoreVerifyPhoneNumMode()
{
	return m_nVerifyCodeSendedCount >= VERIFY_CODE_LIMIT_SENDING_COUNT;
}
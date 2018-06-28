#include "stdafx.h"
#include "ClientInfoReadWrite.h"
#include "../Misc/EncryptionInterface.h"
#include "HardwareInfo.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"


CString ClientInfoReadWrite::m_ClientInfoFileName;
ClientInfo	ClientInfoReadWrite::m_Info;

// 清空客户信息文件
void ClientInfoReadWrite::ClearClientInfo()
{
	m_Info.m_ClientName.Empty();
	m_Info.m_PhoneNumber.Empty();
	m_Info.m_QQNumber.Empty();
	m_Info.m_Address.Empty();
	m_Info.m_MacAdress.Empty();
	m_Info.m_BaseBoardSerialNo.Empty();
	m_Info.m_CPUSerialNo.Empty();
	m_Info.m_HDSerialNo.Empty();

	m_Info.m_OutputNCTimes = 0;
	m_Info.m_RegisterTime = 0;	
}


/*---------------------------------------*/
//	函数说明：
//		设置客户信息文件路径
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void ClientInfoReadWrite::SetFileClientInfoPath()
{
	char ExeFullFilePath[2000];
	GetModuleFileName(NULL, ExeFullFilePath, 500);	// 获取应用程序完整路径名
	CString exe_path = ExeFullFilePath;
	exe_path = exe_path.Left(exe_path.ReverseFind('\\') + 1);	// 保留下分隔符"\"

	CString strTmp;
	strTmp.Format(_T("ClientInfo%s"), g_szSuffix);
	m_ClientInfoFileName = exe_path + strTmp;


}



/*---------------------------------------*/
//	函数说明：
//		客户信息文件是否存在
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
BOOL ClientInfoReadWrite::IsClientInfoFileExist()
{

	SetFileClientInfoPath();

	// 判断客户信息文件是否存在
	if (PathFileExists(m_ClientInfoFileName) == FALSE)
	{
		return FALSE;
	}
	else
		return TRUE;
}

#define		DebugRegister	0


/*---------------------------------------*/
//	函数说明：
//		客户是否注册
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
BOOL ClientInfoReadWrite::IsClientRegistered()
{
	CString CurMacAddress,	FileMacAddress;
	CString CurBaseboard,	FileBaseboard;
	CString CurCpu,			FileCpu;
	CString CurHD,			FileHD;
	
	SetFileClientInfoPath();

	// 获取当前机器的信息
	GetMacAdress(CurMacAddress);
	GetBaseboard(CurBaseboard);
	GetCpu(CurCpu);
	GetHDSerial(CurHD);

#if (DebugRegister == 1)
	CString rpt_msg;

	rpt_msg.Format("获取Mac地址：" + CurMacAddress + "\n" + "获取主板序列号：" + CurBaseboard  + "\n" + "获取硬盘序列号：" + CurHD  + "\n" + "获取CPU序列号：" + CurCpu);

	AfxMessageBox(rpt_msg);

#endif

	
	// 读取客户信息
	ReadClientInfo();

#if (DebugRegister == 1)
	

	rpt_msg.Format("文件Mac地址：" + m_Info.m_MacAdress + "\n" + "文件主板序列号：" + m_Info.m_BaseBoardSerialNo  + "\n" + "文件硬盘序列号：" + m_Info.m_HDSerialNo  + "\n" + "文件CPU序列号：" + m_Info.m_CPUSerialNo);

	AfxMessageBox(rpt_msg);

#endif


	// 对比MAC地址
	FileMacAddress = m_Info.m_MacAdress;

	// MAC地址不相同，重新注册
	int FileMacAddressLen = FileMacAddress.GetLength();
	int CurMacAddressLen = CurMacAddress.GetLength();

	// 2018-2-28 发现电脑共享手机网络注册时，获取本机mac地址失败，断网后能获取，导致mac地址不匹配
	if (CurMacAddressLen != 0 && FileMacAddressLen != 0)
	{
		if (CurMacAddress.Compare(FileMacAddress) != 0)
		{
			// 提示注册原因
		//	AfxMessageBox(CString("文件Mac地址：") +  FileMacAddress + CString("，当前Mac地址：") + CurMacAddress +  "，Mac地址不匹配，需要注册！");
		//	AfxMessageBox(CString("文件信息1：") +  FileMacAddress + CString("，当前信息1：") + CurMacAddress +  "，信息1不匹配，需要注册！");
			AfxMessageBox("信息1不匹配，需要注册！");


			return FALSE;
		}
	}

	// 对比主板序列号
	FileBaseboard = m_Info.m_BaseBoardSerialNo;
	int FileBaseboardLen = FileBaseboard.GetLength();
	int CurBaseboardLen = CurBaseboard.GetLength();

	if (CurBaseboardLen != 0)
	{
		if (CurBaseboard.Compare(FileBaseboard) != 0)
		{
			// 提示注册原因
			//AfxMessageBox(CString("文件主板序列号：") +  FileBaseboard + CString("，当前主板序列号：") + CurBaseboard +  "，主板序列号不匹配，需要注册！");
			//AfxMessageBox(CString("文件信息2：") +  FileBaseboard + CString("，当前信息2：") + CurBaseboard +  "，信息2不匹配，需要注册！");
			AfxMessageBox("信息2不匹配，需要注册！");

			return FALSE;
		}
	}


	// 硬盘序列号 
	// 发现此方法存在漏洞，部分机器会出现硬盘呢序列号会改变的情况
// 	FileHD = m_Info.m_HDSerialNo;
// 	int FileHDLen = FileHD.GetLength();
// 	int CurHDLen = CurHD.GetLength();
// 
// 	if (CurHDLen != 0)
// 	{
// 		if (CurHD.Compare(FileHD) != 0)
// 		{
// 			// 提示注册原因
// 			AfxMessageBox(CString("文件硬盘序列号：") +  FileHD + CString("，当前硬盘序列号：") + CurHD +  "，硬盘序列号不匹配，需要注册！");
// 
// 
// 			return FALSE;
// 		}
// 	}


	// CPU序列号
	FileCpu = m_Info.m_CPUSerialNo;
	int FileCpuLen = FileCpu.GetLength();
	int CurCpuLen = CurCpu.GetLength();

	if (CurCpuLen != 0)
	{
		if (CurCpu.Compare(FileCpu) != 0)
		{
			// 提示注册原因
			//AfxMessageBox(CString("文件CPU序列号：") +  FileCpu + CString("，当前CPU序列号：") + CurCpu +  "，CPU序列号不匹配，需要注册！");
			//AfxMessageBox(CString("文件信息3：") +  FileCpu + CString("，当前信息3：") + CurCpu +  "，信息3不匹配，需要注册！");
			AfxMessageBox("信息3不匹配，需要注册！");


			return FALSE;
		}
	}


	// 如果时间超过3个月，注册，记录当前时间 3*30*24*60*60 = 7776000秒
	CTime last_time(m_Info.m_RegisterTime);
	CTime cur_time = CTime::GetCurrentTime();
	CTimeSpan  timeSpan = cur_time - last_time;
	long days = timeSpan.GetDays();

	if (days > 90 || days < 0)
	{
		// 提示注册原因
		CString msg_info;

		//msg_info.Format("时间间隔：%d,时间超出90天，需要注册！" , days);
		msg_info = "信息4，需要注册！";
		AfxMessageBox(msg_info);

		return FALSE;
	}

	// 如果导出NC文件次数超过800次，注册，清零
	if (m_Info.m_OutputNCTimes > 800)
	{
		// 提示注册原因
		CString msg_info;

		//msg_info.Format("当前导出次数：%d,导出NC文件次数超过800次,需要注册！" , m_Info.m_OutputNCTimes);
		msg_info = "信息5,需要注册！" ;
		AfxMessageBox(msg_info);

		return FALSE;
	}

	return TRUE;
}

/*---------------------------------------*/
//	函数说明：
//		重置客户信息
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void ClientInfoReadWrite::ResetClientInfo(void)
{

	CString CurMacAddress;

	GetMacAdress(CurMacAddress);


	// 统一清空内存
	ClearClientInfo();


	// 保存m注册的ac地址

	//Mac地址
	GetMacAdress(m_Info.m_MacAdress);

	// 主板序列号
	char lpszBaseBoard[128] = {0};  
	GetBaseBoardByCmd(lpszBaseBoard);
	m_Info.m_BaseBoardSerialNo = lpszBaseBoard;

	// 硬盘序列号
	char lpszHD[128] = {0};  
	GetHDSerialByCmd(lpszHD); 
	m_Info.m_HDSerialNo = lpszHD;

	// CPU序列号
	char lpszCPU[128] = {0};  
	GetCpuByCmd(lpszCPU);
 	m_Info.m_CPUSerialNo = lpszCPU;


	// 保存注册时间
	CTime time = CTime::GetCurrentTime();
	m_Info.m_RegisterTime = (long)(time.GetTime());

	// 保存导出次数
	m_Info.m_OutputNCTimes = 1;
}


/*---------------------------------------*/
//	函数说明：
//		读取客户信息
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void ClientInfoReadWrite::ReadClientInfo()
{
	CFile file;

	SetFileClientInfoPath();

	// 统一清空内存
	ClearClientInfo();

	// 解密

	//加密文件读取标准
	CString HgePath = m_ClientInfoFileName;
	CString XmlPath = m_ClientInfoFileName.Left(m_ClientInfoFileName.GetLength()-4) + ".xml";

	// 新的加解密方式
	decrypt_base64(HgePath.GetBuffer(), XmlPath.GetBuffer());

	// 加载文件
	TiXmlDocument* m_xmlDoc = new TiXmlDocument();
	m_xmlDoc->LoadFile(XmlPath);

	TiXmlElement* pRoot = m_xmlDoc->RootElement();

	// 判断
	if (pRoot == NULL)
	{
		//AfxMessageBox("注册文件出错，请删除ClientInfo.hge文件并重新注册！");
		return ;
	}

	string strValue =  pRoot->Value();
	if (strValue != "注册信息")
	{
		//AfxMessageBox("注册文件出错，请删除ClientInfo.hge文件并重新注册！");
		return ;
	}

	// 读取文件信息
	for (TiXmlElement * pCurNode = pRoot->FirstChildElement(); pCurNode != NULL; pCurNode = (TiXmlElement*)(pCurNode->NextSibling()))
	{
		string strCurNodeName =  pCurNode->Value();

		if (strCurNodeName == "用户信息")
		{

			m_Info.m_ClientName		= pCurNode->Attribute("Name"			);		// 姓名
			m_Info.m_PhoneNumber	= pCurNode->Attribute("Phone"			);		// 手机
			m_Info.m_QQNumber		= pCurNode->Attribute("QQ"				);		// qq
			m_Info.m_Address		= pCurNode->Attribute("Address"			);		// 地址
			m_Info.m_RegisterTime	= _ttoi(pCurNode->Attribute("RegisterTime"	));		// 注册时间
			m_Info.m_OutputNCTimes	= _ttoi(pCurNode->Attribute("OutputNcTimes"));		// 导出NC文件次数

		}
		else if (strCurNodeName == "机器信息")
		{

			m_Info.m_MacAdress			= pCurNode->Attribute("MacAddress"			);	// MAC地址
			m_Info.m_BaseBoardSerialNo	= pCurNode->Attribute("BaseBoardSerialNo"	);	// 主板序列号
			m_Info.m_CPUSerialNo		= pCurNode->Attribute("HDSerialNo"			);	// 硬盘序列号
			m_Info.m_HDSerialNo			= pCurNode->Attribute("CPUSerialNo"			);	// CPU序列号
		}

	}

	delete m_xmlDoc;
	m_xmlDoc = NULL;

	// 读取完之后删除解密文件
	DeleteFile(XmlPath); 


}

/*---------------------------------------*/
//	函数说明：
//		写入客户信息
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
void ClientInfoReadWrite::WriteClientInfo()
{
	CFile file;

	SetFileClientInfoPath();


	CString HgePath = m_ClientInfoFileName;
	CString XmlPath = m_ClientInfoFileName.Left(m_ClientInfoFileName.GetLength()-4) + ".xml";

	//改为XML文件格式保存

	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 

	// 单例类
	//TiXmlElement* pSingletonElement = new TiXmlElement("Singleton");
	TiXmlElement* pRegInfoElem = new TiXmlElement("注册信息");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pRegInfoElem);

	// 用户信息
	TiXmlElement* pClientInfoElem = new TiXmlElement("用户信息");
	pRegInfoElem->LinkEndChild(pClientInfoElem);

	pClientInfoElem->SetAttribute("Name",			m_Info.m_ClientName);		// 姓名
	pClientInfoElem->SetAttribute("Phone",			m_Info.m_PhoneNumber);		// 手机
	pClientInfoElem->SetAttribute("QQ",				m_Info.m_QQNumber);			// qq
	pClientInfoElem->SetAttribute("Address",		m_Info.m_Address);			// 地址
	pClientInfoElem->SetAttribute("RegisterTime",	m_Info.m_RegisterTime);		// 注册时间
	pClientInfoElem->SetAttribute("OutputNcTimes",	m_Info.m_OutputNCTimes);	// 导出NC文件次数


	// 电脑机器信息
	TiXmlElement* pMachineInfoElem = new TiXmlElement("机器信息");
	pRegInfoElem->LinkEndChild(pMachineInfoElem);



	pMachineInfoElem->SetAttribute("MacAddress",			m_Info.m_MacAdress);			// MAC地址
	pMachineInfoElem->SetAttribute("BaseBoardSerialNo",	m_Info.m_BaseBoardSerialNo);	// 主板序列号
	pMachineInfoElem->SetAttribute("HDSerialNo",			m_Info.m_CPUSerialNo);			// 硬盘序列号
	pMachineInfoElem->SetAttribute("CPUSerialNo",		m_Info.m_HDSerialNo);			// CPU序列号



	// 保存文件
	m_xmlDoc->SaveFile(XmlPath);
	delete m_xmlDoc;


	// 加密
	encrypt_base64(XmlPath.GetBuffer(), HgePath.GetBuffer());
	DeleteFile(XmlPath);


}


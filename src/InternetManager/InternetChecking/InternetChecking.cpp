#include "stdafx.h"
#include "InternetChecking.h"
#include "../JsonCpp/json.h"

#include <afxinet.h>
#include <iphlpapi.h >



#include "../../../include/FileReadWrite/Misc/HGCode.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"


#pragma comment(lib,"Iphlpapi.lib")
//#pragma comment(lib, "../../lib/jsoncpp.lib")

// 判断是否在测试环境下
#ifdef DEBUG 

#pragma comment(lib, "../../lib/FileReadWrited.lib")

#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/FileReadWrite.lib")

#pragma  message(".......................................当前为release版本")

#endif



wstring AnsiToUnicode(const char* szStr)
{
	int nLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen+1];
	::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	wstring strReturn(pResult);
	delete[] pResult;
	return strReturn;
}



/*---------------------------------------*/
//	函数说明：
//		日常登录统计
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
bool InternetChecking::LoginStatisticsDaily(void)
{
	//网络验证
	CString str_mac;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	// 获取mac地址
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}

	if (ERROR_SUCCESS==nRel)
	{
		for (int i = 0;i < 6;i++)
		{
			CString tmp_mac; 
			tmp_mac.Format(_T("%x "),pIpAdapterInfo->Address[i]);
			str_mac = str_mac + tmp_mac;
		}
	}

	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	// 阿坚版本  http://119.23.20.26:8100/
#if (CUR_EDITION == YW_EDITION)

	CString hostName = "119.23.20.26";

#elif (CUR_EDITION == HG_EDITION)

	CString hostName = "api.techtimesun.net";

#endif



	CString insert_pathName = "api/hgsoftapi.aspx?method=insert&mac=";
	insert_pathName = insert_pathName + str_mac;

	// 产品名字部分转码
	string url_product_name		=   HGCode::UrlUTF8("雕刻机软件");   
	CString insert_ProductName	= CString("&product=")	+ url_product_name.c_str();

	insert_pathName += insert_ProductName ;


#if (CUR_EDITION == HG_EDITION)
	// 产品id 数据统计使用 
	CString insert_ProductID	= CString("&productid=159");	

	insert_pathName += insert_ProductID ;
#endif



	// 阿坚版本需要加上type=1，作为区别
#if (CUR_EDITION == YW_EDITION)

	string url_type			=   HGCode::UrlUTF8("1");
	CString insert_type		= CString("&type=")	+ url_type.c_str();


	insert_pathName += insert_type;

#endif






	// 发送http请求

	
#if (CUR_EDITION == YW_EDITION)
	
	// 阿坚的端口为8100
	int port = 8100;

#elif (CUR_EDITION == HG_EDITION)

	// 宏光的端口为8060
	int port = 80;

#endif





	CString pathName = insert_pathName; 
	string postData = "insert";

	// 网络回话
	CInternetSession session(_T("your app agent name")); 

	INTERNET_PORT nPort = (INTERNET_PORT)port; 
	DWORD dwRet = 0;
	CHttpConnection* pServer = session.GetHttpConnection((LPCTSTR)hostName, nPort); 

	CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)pathName);   
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头   

	//开始发送请求   
	pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size()); 
	pFile->QueryInfoStatusCode(dwRet);

	if (dwRet == HTTP_STATUS_OK) 
	{ 
		CString result, newline;   
		while(pFile->ReadString(newline)) 
		{
			//循环读取每行内容 
			result = result + newline+"\r\n"; 
		}   

		if (result.Find("true") < 0)
		{
			//AfxMessageBox("更新后台数据失败");
		}
	} 

	delete pFile; 
	delete pServer;   

	session.Close();   
	return true; 

}



/*---------------------------------------*/
//	函数说明：
//		网络验证
//
//
//	参数：
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
bool InternetChecking::AccessCRM(CString ClientName, CString ClientPhoneNumber, CString ClientQQNumber, CString ClientAddress)
{
	//网络验证
	CString str_mac;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	// 获取mac地址
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}

	if (ERROR_SUCCESS==nRel)
	{
		for (int i = 0;i < 6;i++)
		{
			CString tmp_mac; 
			tmp_mac.Format(_T("%x "),pIpAdapterInfo->Address[i]);
			str_mac = str_mac + tmp_mac;
		}
	}

	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}


	// 阿坚版本  http://119.23.20.26:8100/
#if (CUR_EDITION == YW_EDITION)

	CString hostName = "119.23.20.26";

#elif (CUR_EDITION == HG_EDITION)

	CString hostName = "api.techtimesun.net";

#endif

	// check命令参数
	CString check_pathName = "API/LockAPI.aspx?action=check&mac=";
	check_pathName = check_pathName + str_mac;

	// 阿坚版本需要加上type=1，作为区别
#if (CUR_EDITION == YW_EDITION)

	string url_type			=   HGCode::UrlUTF8("1");
	CString check_type		= CString("&type=")	+ url_type.c_str();


	check_pathName += check_type;

#endif








	CString insert_pathName = "API/LockAPI.aspx?action=insert&mac=";
	insert_pathName = insert_pathName+str_mac.GetBuffer();

	// 名字部分转码
	string url_name =   HGCode::UrlUTF8(ClientName.GetBuffer());
	//HGCode::CString_GB2312_To_Unicode(ClientName);
	CString insert_ClientName			= CString("&name=")	+ url_name.c_str();

	CString insert_ClientPhoneNumber	= CString("&mobilephone=")	+ ClientPhoneNumber;
	CString insert_ClientQQNumber		= CString("&qq=")	+ ClientQQNumber;

	// 地址部分转码
	//HGCode::CString_GB2312_To_Unicode(ClientAddress);

	string url_Address =   HGCode::UrlUTF8(ClientAddress.GetBuffer());
	CString insert_ClientAddress		= CString("&address=")	+ url_Address.c_str();





	insert_pathName += insert_ClientName ;
	insert_pathName += insert_ClientPhoneNumber;
	insert_pathName += insert_ClientQQNumber;
	insert_pathName += insert_ClientAddress;



	// 阿坚版本需要加上type=1，作为区别
#if (CUR_EDITION == YW_EDITION)

	CString insert_type		= CString("&type=")	+ url_type.c_str();


	insert_pathName += insert_type;

#endif



#if 0
	// 名字
	int name_len = insert_ClientName.GetLength();
	char* pNameBuf = new char[name_len+1];

	memset(pNameBuf, 0, name_len+1);
	memcpy(pNameBuf, insert_ClientName.GetBuffer(), name_len);

	// 手机
	int phone_len = insert_ClientPhoneNumber.GetLength();
	char* pPhoneBuf = new char[phone_len+1];

	memset(pPhoneBuf, 0, phone_len+1);
	memcpy(pPhoneBuf, insert_ClientPhoneNumber.GetBuffer(), phone_len);


	// qq
	int qq_len = insert_ClientQQNumber.GetLength();
	char* pQQBuf = new char[qq_len+1];

	memset(pQQBuf, 0, phone_len+1);
	memcpy(pQQBuf, insert_ClientQQNumber.GetBuffer(), qq_len);


	// 地址
	int address_len = insert_ClientAddress.GetLength();
	char* pAddressBuf = new char[address_len+1];

	memset(pAddressBuf, 0, address_len+1);
	memcpy(pAddressBuf, insert_ClientAddress.GetBuffer(), address_len);

	strcat(pNameBuf,pPhoneBuf);
	strcat(pNameBuf,pQQBuf);
	strcat(pNameBuf,pAddressBuf);



	insert_pathName += pNameBuf ;
	insert_pathName += pPhoneBuf;
	insert_pathName += pQQBuf;
	insert_pathName += pAddressBuf;

#endif

	// jason数据包
	std::string postData = "";

// 	Json::Value root;
// 	root["name"]			= std::string(ClientName.GetBuffer());
// 	root["mobilephone"]		= std::string(ClientPhoneNumber.GetBuffer());
// 	root["qq"]				= std::string(ClientQQNumber.GetBuffer());
// 	root["address"]			= std::string(ClientAddress.GetBuffer());
// 
// 	postData = root.toStyledString();


#if (CUR_EDITION == YW_EDITION)



	// 检查登陆次数
	if(PostHttpPage(8100, hostName, check_pathName,"check") == false)
		return false;
	// 增加登录次数
	if (PostHttpPage(8100, hostName, insert_pathName,"insert") == false)
		return false;

#elif (CUR_EDITION == HG_EDITION)


	// 检查登陆次数
	if(PostHttpPage(80, hostName, check_pathName,"check") == false)
		return false;
	// 增加登录次数
	if (PostHttpPage(80, hostName, insert_pathName,"insert") == false)
		return false;

#endif



	return true;
}


int GB2312ToUtf8(const char* gb2312, char* utf8)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return len;
}





/*---------------------------------------*/
//	函数说明：
//		连接crm，发送请求，接收消息
//
//
//	参数：
//		int port						--	端口号
//		const std::string & hostName	--	服务器名
//		const std::string & pathName	--	路径名
//		const std::string & postData	--	发送数据
//
//	返回值:
//
//
/*---------------------------------------*/

bool InternetChecking::PostHttpPage(int port, CString  hostName, CString pathName, const std::string & postData)
{
	// 网络回话
	CInternetSession session(_T("your app agent name")); 
	//session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 30000);

	try 
	{ 
		INTERNET_PORT nPort = (INTERNET_PORT)port; 
		DWORD dwRet = 0;
		CHttpConnection* pServer = session.GetHttpConnection((LPCTSTR)hostName, nPort); 
		
		CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)pathName);   
		
		CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头   

		//开始发送请求   
		pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size()); 
		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet == HTTP_STATUS_OK) 
		{ 
			CString result, newline;   
			while(pFile->ReadString(newline)) 
			{
				//循环读取每行内容 
				result = result + newline+"\r\n"; 
			}   

			if (postData == "check")
			{
				CString message_result = result;


				// 输出当前次数

				message_result.Replace("Message","");
				message_result.Replace("Result","");
				message_result.Replace("true","");
				message_result.Replace("false","");
				message_result.Replace("\"","");
				message_result.Replace(",","");
				message_result.Replace(":","");
				message_result.Replace("{","");
				message_result.Replace("}","");
				std::string result_num = message_result.GetBuffer();
				int i_num = atoi(result_num.c_str());

				//AfxMessageBox(CString("当前已使用次数为：") + message_result);


// 				if (i_num >= 1000)
// 				{
// 					AfxMessageBox("使用次数超过授权次数");
// 					return false;
// 				}
			}
			else
			{


				if (result.Find("true") < 0)
				{
					AfxMessageBox("更新后台数据失败");
					return false;
				}
			}

		} 
		else 
		{ 
			return false; 
		} 
		delete pFile; 
		delete pServer;   
	} 
	catch (CInternetException* pEx) 
	{ 
		TCHAR pszError[200]; 
		pEx->GetErrorMessage(pszError, 200);   
		AfxMessageBox(pszError);
		return false; 
	} 

	session.Close();   
	return true; 
}



#ifndef		_ClientInfoReadWrite_h_
#define		_ClientInfoReadWrite_h_

#pragma  once

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"




class AFX_EXT_CLASS  ClientInfoReadWrite
{

public:
	static BOOL IsClientInfoFileExist();			// 客户信息文件是否存在
	static BOOL IsClientRegistered();				// 客户是否注册
	static void ReadClientInfo();					// 读取客户信息
	static void WriteClientInfo();					// 写入客户信息
	static void SetFileClientInfoPath();			// 设置客户信息文件路径
	static void ResetClientInfo(void);				// 重置客户信息
	static void ClearClientInfo();					// 清空客户信息文件

public:
	static CString		m_ClientInfoFileName;
	static ClientInfo	m_Info;

};


#endif
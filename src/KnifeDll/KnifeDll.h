// KnifeDll.h : KnifeDll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号



// 判断是否在测试环境下
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma  message(".......................................当前为release版本")

#endif









// CKnifeDllApp
// 有关此类实现的信息，请参阅 KnifeDll.cpp
//

class CKnifeDllApp : public CWinApp
{
public:
	CKnifeDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

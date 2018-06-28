// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// Carving.h : Carving 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

// 判断是否在测试环境下
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/FileReadWrited.lib")
#pragma comment(lib, "../../lib/tinyxmld.lib")
//#pragma comment(lib, "../../lib/KnifeDlld.lib")
#pragma comment(lib, "../../lib/QRCoded.lib")
#pragma comment(lib, "../../lib/Encryptiond.lib")
#pragma comment(lib, "../../lib/InternetManagerd.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManagerd.lib")
#pragma comment(lib, "../../lib/TaoBaoSenderd.lib")

#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/FileReadWrite.lib")
#pragma comment(lib, "../../lib/tinyxml.lib")
//#pragma comment(lib, "../../lib/KnifeDll.lib")
#pragma comment(lib, "../../lib/QRCode.lib")
#pragma comment(lib, "../../lib/FileCompressandUnCompress.lib")
#pragma comment(lib, "../../lib/hg3d.lib")
#pragma comment(lib, "../../lib/Encryption.lib")
#pragma comment(lib, "../../lib/InternetManager.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManager.lib")
#pragma comment(lib, "../../lib/TaoBaoSender.lib")

#pragma  message(".......................................当前为release版本")

#endif


#define WM_REFRESH_PANEL_VIEW               WM_USER+1
#define WM_USER_EXPORT_MATERIAL_LIST        WM_USER+2
#define WM_USER_EXPORT_COMPONENT_NC         WM_USER+3

// CCarvingApp:
// 有关此类的实现，请参阅 Carving.cpp
//

class CCarvingApp : public CWinAppEx
{
public:
	CCarvingApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	ULONG_PTR m_pGdiToken;
};

extern CCarvingApp theApp;

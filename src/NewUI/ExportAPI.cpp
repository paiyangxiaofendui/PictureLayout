#include "stdafx.h"
#include "ExportAPI.h"

#include "UI/NewUIDlg.h"
#include "UI/DlgWeiXin.h"

 int  __stdcall Function1(int a, int b)
{
	return a+b;
}

int Test(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

 	CDlgNewUI dlg;

	dlg.DoModal();

// 	CDlgWeiXin dlg;
// 
// 	dlg.DoModal();


	return 0;
}


 int __stdcall JT_Layouts(char* src_file_path)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 CDlgNewUI dlg;

	 dlg.m_strSrcFilePath = src_file_path;
		

	 dlg.DoModal();

	 // 	CDlgWeiXin dlg;
	 // 
	 // 	dlg.DoModal();


	 return 0;
 }

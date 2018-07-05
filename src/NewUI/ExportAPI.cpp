#include "stdafx.h"
#include "ExportAPI.h"

#include "UI/NewUIDlg.h"
#include "UI/DlgWeiXin.h"

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

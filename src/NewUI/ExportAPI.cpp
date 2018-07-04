#include "stdafx.h"
#include "ExportAPI.h"

#include "UI/NewUIDlg.h"

int Test(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

 	CDlgNewUI dlg;

	dlg.DoModal();


	return 0;
}



#include "stdafx.h"
#include "AbTools.h"
#include "ABTDialog.h"


AbTools::AbTools()
{

}


AbTools::~AbTools()
{

}



CString  AbTools::GetPanelOutlineStr(void)
{
	return m_strOutline;
}

// ÏÔÊ¾´°¿Ú 
int AbTools::ShowDialog()
{
	int rt;
	ABTDialog dlg;

	rt = dlg.DoModal();
	if (rt == IDOK)
	{
		m_strOutline = dlg.m_strPanelOutlineNode;
	}
	else
	{
		m_strOutline.Empty();
	}

	return rt;
}
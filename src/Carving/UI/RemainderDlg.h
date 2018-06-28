#pragma once
#include "../Resource.h"
// #include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
// #include "./UICommon/SkinListCtrlBase.h"
// #include "SkinHeaderCtrl.h"
// #include "../UIData/UIData.h"


class Component;

class RemainderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RemainderDlg)

public:
	RemainderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RemainderDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_REMAINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

public:

	vector<Component*> AllRmdList;


public:
	CListCtrl m_lcRemainder;


};

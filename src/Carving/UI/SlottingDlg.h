#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "../Misc/YHInfoTools.h"
#include "../Resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
using namespace std;
// CSlottingDlg dialog


class CSlottingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSlottingDlg)

public:
	CSlottingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSlottingDlg();
	std::string replace_allstr(std::string str,const std::string old_value,const std::string new_value);
	void drawpanel();
// Dialog Data
	enum { IDD = IDD_SLOTTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	vector<AllSlot> GetSlotByListCtrl();
public:
	afx_msg void OnBnClickedOk();
	CString m_editd;
	CString m_edith;
	CString m_editSlotLength;
	CString m_edity;
	CString m_editx;
	CString m_result;
	CString m_editdir;

	CString m_strpoint;
	afx_msg void OnPaint();
	CStatic m_showpanel;

	vector<p_show> m_showps;
	float _scale;

	virtual BOOL OnInitDialog();
	CComboBox m_comdir;
	afx_msg void OnBnClickedButton1();
	CListCtrl m_soltlist;
	afx_msg void OnNMDblclkSoltlist(NMHDR *pNMHDR, LRESULT *pResult);
	void WriteSlotTemplate(CString Slotx,CString Sloty,CString SlotWidth,CString SlotDepth);
	afx_msg void OnNMRClickSoltlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRemoveSlot();

	CString m_strOutlinePointString;
	CString m_strPreExistSlots;
	float m_fComponentXLen;
	float m_fComponentYLen;

};

#pragma once
#include "SkinHeaderCtrl.h"
#include <list>
#include <map>

using namespace std;

//class CBtnExcelSetting;
class CPngButton_Stretchable;

struct HeaderItemData
{
	HeaderItemData()
		: m_pButton(NULL)
		, m_nCurSel(0)
		, m_uBtnID(0)
	{}
	//CComboBox* m_pComboBox;
	CPngButton_Stretchable* m_pButton;
	int m_nCurSel;
	UINT m_uBtnID;
};
// CSkinHeaderCtrl

class CSkinHeaderCtrl_ExcelSetting : public CSkinHeaderCtrl
{
	DECLARE_DYNAMIC(CSkinHeaderCtrl_ExcelSetting)

public:
	CSkinHeaderCtrl_ExcelSetting();
	virtual ~CSkinHeaderCtrl_ExcelSetting();

	//BOOL DeleteItem(int nPos);
	//int InsertItem(int nPos, HDITEM* phdi);
	void SetInitMapping(map<int, int> mapExcelToRequirement);
	map<int, int> GetMapping();
	void ArrangeBtns();

protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam);
	//afx_msg void OnPaint();
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnItemInsert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnItemDelete(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnWinPosChanging(WPARAM wParam, LPARAM lParam);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnBtn1();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void OnMenu(UINT uMenuID);

	virtual CRect GetTitleDrawingArea(int nItem);
	CRect GetComboBoxArea(int nItem);
	//void InitComboBoxMenu(CComboBox* pComboBox);
	void UpdateMapping(bool bSave = false);
	void SetOneItemMapping(HeaderItemData& theHeaderItemData, int nCurSelID);
	void OnBtnClicked(UINT uBtnID);
	CString GetMenuTextByID(int nID);



	//Image* m_pImgSortAscending;
	//Image* m_pImgSortDescending;
	//int m_eSortOrderType;
	//int m_nSortintSubItemID;

	list<HeaderItemData> m_listHeaderItemData;
	map<int, int> m_mapExcelToRequirement;
	UINT m_uClickedBtnID;
	Image* m_pImgCheck;
};



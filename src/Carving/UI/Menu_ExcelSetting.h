#pragma once
#include <vector>
#include <map>

using namespace std;

struct MenuItemData_ExcelSetting
{
	MenuItemData_ExcelSetting(){}
	~MenuItemData_ExcelSetting(){/*OutputDebugString("a\n")*/;}
	CString strText;
};
// CMenu_ExcelSetting

class CMenu_ExcelSetting : public CMenu
{
	DECLARE_DYNAMIC(CMenu_ExcelSetting)

public:
	CMenu_ExcelSetting();
	virtual ~CMenu_ExcelSetting();

	void SetCheckImage(Image* pImgCheck);
	void SetItemsText(map<UINT, CString> mapItemCommandIDToText);
	void SetSpecialColorItemsID(vector<int> vSpecialColorItemID);
protected:
	//DECLARE_MESSAGE_MAP()
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	Image* m_pImgCheck;
	map<UINT, CString> m_mapItemCommandIDToText;
	vector<int> m_vSpecialColorItemID;
};



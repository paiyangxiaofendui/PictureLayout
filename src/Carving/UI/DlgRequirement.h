#pragma once
#include "afxcmn.h"
#include "ListRequirement.h"
#include "SkinHeaderCtrl.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include <map>
#include "./UICommon/DialogChildBase.h"
#include "ListBoxPreCombine.h"
#include "../UIData/UIData.h"

using namespace std;

class CDlgRequirement_PreCombine;
// CDlgRequirement 对话框

class CDlgRequirement : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgRequirement)

public:
	CDlgRequirement(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRequirement();

// 对话框数据
	enum { IDD = IDD_DIALOG_PANEL_LIST };


	//从界面材料获得1个
	std::map<std::string,std::string> GetMatrialItemsOne( int index );
	vector<PreCombineItem> GetPreCombineItems();
	void SetRequirement(vector<ComponentInputItem>& vComponentInputItem, bool bResetPreCombine = true);
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickRequirmentList(NMHDR *pNMHDR, LRESULT *pResult);		//鼠标右击
	afx_msg void OnOtherShapeSetting();	//异形板设置
	afx_msg void OnDelOtherShape();		//删除异形板
	afx_msg void OnDeleteslot();		//删除槽
	afx_msg void OnSlot();				//槽设置
	afx_msg void OnSetdir();			//设置纹理
	afx_msg void OnSetMatrial();		//设置材质
	afx_msg void OnEditMatrialList();	//编辑需切成品
	afx_msg void OnDelMatrialList();
	afx_msg void OnSelfOthershap();
	afx_msg void OnSetReminder();
	afx_msg void OnResetHoleInfo();
	afx_msg void OnPreCombine();
	afx_msg void OnExportComponentNC();
	afx_msg void OnNMDblclkListRequirement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListRequirement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitemListRequirement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnShowOrHidePreCombine(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//设置界面材料1个
	void SetMatrialItemsOne( int i,std::map<std::string,std::string> noProcessSmallPanelOne );
	bool SetOneItemTextAndData(int nIndex, ComponentInputItem& theComponentInputItem);

	void ReadSlotTemplate(std::string &Slotx,std::string &Sloty,std::string &SlotWidth,std::string &SlotDepth);
	//获得选择序号
	std::vector<int> getSelectIndexFromMatrialList();
	ComponentInputItem* GetComponentInputItemByID(int nItem);

	void RefreshListCtrl();

	void OnResetMachiningInfo(int nType);

	void AddPreCombine(vector<CString> vPreCombineBarcode);
	void ShowOrHidePreCombineListBox();
	void ArrangeUI();

	void SetPanelInfo(vector<ComponentInputItem>& vComponentInputItem);

	void SelectAll();

	void RefreshAllItemState();
	void CheckDuplicatedBarcode(vector<vector<int>>& vDuplicatedBarcodeItem);
protected:
	CListRequirement m_lcRequirement;
	CSkinHeaderCtrl m_hcRequirement;
	vector<ComponentInputItem>* m_pvComponentInputItem;
	//CDlgRequirement_PreCombine* m_pDlgPreCombine;

	int m_eSortOrderType;
	int m_nSortintSubItemID;
	Image* m_pImgSortAscending;
	Image* m_pImgSortDescending;

	CListBoxPreCombine m_lbPreCombine;
};

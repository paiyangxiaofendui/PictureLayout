/** @brief 这里写你的摘要
 *  @file 你的文件名
 *  @author 谁tmd的搞的
 *  @version 版本了
 *  @date  你啥时候搞的
 *  @note       注解. 例如: 本文件有什么具体功能啊,使用时要注意什么啊…..
 *  @since    自从  例如: 自从2012年地球爆炸后这个文件就从地球上消失了…..
 */


#pragma once

#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/RawMaterialInfo/RawMaterialInfo.h"
#include "../../../include/DataManager/BaseDataType/BaseInfo/BaseInfo.h"
#include "listboxclipboard.h"
#include "./UICommon/DialogChildBase.h"
#include "./UICommon/PngButton.h"
#include "../UIData/UIData.h"
#include "../Misc/OtherShapeHelper.h"

// CDlgResult 对话框

class CDlgTotalResult;
class Component;
class PanelViewingParam;

#define WM_REFRESH_PANEL_VIEW               WM_USER+1


#define DEFAULT_WIDTH						(1000000) 


#define IDTIMER1  1



 /** 
    * @brief 简要说明文字 
    */
enum RemainderCutingSettingState
{
	CUTTING_SETTING_STATE_START,		/**< 原料宽度 */
	CUTTING_SETTING_STATE_END			/**< 原料宽度 */
};


  /**
    * @brief 类的简单概述 \n(换行)
    * 类的详细概述
    */
class CDlgResult : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESULT };

	void ResetResultDlg();
	void RefreshOptimizeResult();
	void StopDrawing();
	vector<Panel*> GetPanels(bool bChecked);
	bool IsPanelChecked(Panel* pPanel);
	PanelViewingParam* GetSelectedItemViewingParam();
	void ResetPastingState();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnRefreshPanelView(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMenuResetMachiningOrder();
	afx_msg void OnMenuCutComponent();
	afx_msg void OnMenuCopyComponent();
	afx_msg void OnMenuPasteComponent();
	afx_msg void OnMenuRemoveComponent();
	afx_msg void OnMenuRemainderCutting();
	afx_msg void OnMenuRotatePastingComponent();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer( UINT nIDEvent );

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	afx_msg void OnLbnSelchangeClipBoard();
	afx_msg LRESULT OnShowOrHideClipboard(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	CRect GetPanelViewRect();
	
	void DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, PanelViewingParam& param);
	void DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color = Color::Red);
	
	void DrawMachiningInfo(Graphics& g, CRect rcPanel, float fScale, Component* pComponent);
	void DrawDetail(Graphics& g, CRect rcComponent, float fScale, Component* pComponent);
	void DrawRemainderCuting(Graphics& g, CRect rcPanel, float fScale, Panel& thePanel);
	
	CRect GetComponentUIRect(float fPanelXLen, float fPanelYLen, float fComponentX, float fComponentY, float fComponentXLen, float fComponentYLen);
	float GetDrawingScale(float fPanelXLen, float fPanelYLen);

	void AddComponentToClipboard(Component* pComponent);
	

	void ShowOrHideClipboard();
	void DrawComponentToPos(Component* pComponent, CPoint ptComponent, bool bDrawOKIcon, Panel* pPanel);
	bool IsPosCanPasteComponent(Panel* pPanel, Component* pComponent, PointFloat ptComponentUILeftTop);
	PointFloat ConvertUIPosToPanelPos(Panel* pPanel, CPoint ptInUI);
	CPoint ConvertPanelPosToUIPos(Panel* pPanel, PointFloat ptInPanel);
	void CancelPasting();
	bool IsAttachMode();

	PointFloat GetAttachPanelPos_Component(CPoint ptMouseInUI, float fComponentXLen, float fComponentYLen);
	void RotatePastingComponent();
	void PastingComponent_OnMouseMove(CPoint point);

	void DrawRemainderCuttingToPos(CutPath& theRemainderCutting, Panel* pPanel);
	void StopRemainderCutting();
	PointFloat GetAttachPanelPos_RemainderCutting_Start(CPoint ptMouseInUI);
	PointFloat GetAttachPanelPos_RemainderCutting_End(CPoint ptMouseInUI, PointFloat ptStartInPanel);

	CDlgTotalResult* m_pDlgTotalResult;

	CPoint m_ptRClicked;

	CListBoxClipBoard m_lbClipBoard;
	Component* m_pPastingComponent;
	bool m_bNeedToEraseOldPastingComponent;
	CPoint m_ptOldPastingComponent;
	bool m_bOldPastingComponentOK;
	PointFloat m_ptfAttachPosInPanel;

	CutPath* m_pRemainderCutting;
	bool m_bNeedToEraseOldRemainderCutting;
	int m_nRemainderCuttingSettingState;



public:


	/** 
	* @brief 清除所有数据
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void ClearAllData();


	/** 
	* @brief 清除所有数据
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnOpenSolution();

	/** 
	* @brief 清除所有数据
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnLayout();

	/** 
	* @brief 打开图片信息
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnOpenSourcePicInfo();

	/** 
	* @brief 键盘输入字符串
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void  InputNormalString(string str);

	/** 
	* @brief 拷贝到剪贴板
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	bool CopyToClipboard(const char* pszData, const int nDataLen);


	/** 
	* @brief 排样结果对接蒙泰
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnConnectMaintop();

	void setEditCtrlString(int pos_x, int pos_y, string str, int sleep_time = 0);

	static void SetBottomAction(CString title, int id, UINT action);
	static void SetCtrlText(CString title, int id,  CString ctrl_type, CString text);

	static void EmptyCtrlContent(HWND hWnd);
	static CWnd* FindWndByCtrlID(CWnd* pWnd, UINT nCtrlID, LPCTSTR szClassName);
	static int FindWindowLike(CString& namelink);
	static BOOL CALLBACK EnumWindowsLikeProc(HWND hwnd,LPARAM lParam);

	/** 
	* @brief 优化
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void OnOptimize();

	/** 
	* @brief 检查
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);

	/** 
	* @brief 检查原料
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	bool CheckRawMaterialUsable(vector<ComponentInputItem>& vComponentInputItem, RawMaterialInfo rm_info, BaseInfo b_info);

	vector<PreCombineItem> m_vPreCombineItem;
	vector<ComponentInputItem> m_vComponentInputItem;						/**< 输入板件链表	*/ 
	vector<RawMaterialInfo> m_vRawMaterialList;								/**< 原料信息链表	*/ 
	BaseInfo m_BaseInfo;											/**< 原料信息链表	*/ 

	
	float m_len;				/**< 原料长度 */
	float m_width;				/**< 原料宽度 */

	float m_x_space;			/**<  X方向间隙		*/
	float m_y_space;			/**<  Y方向间隙		*/
	float m_left_offset;		/**<  左预留间隙	*/
	float m_right_offset;		/**<  右预留间隙	*/
	float m_top_offset;			/**<  上预留间隙	*/
	float m_bottom_offset;		/**<  下预留间隙	*/


	CComboBox control_arranging_origin;
	int m_arranging_origin;		/**<  排样原点	*/


	string m_strSrcFilePath;
	bool open_rt;



};

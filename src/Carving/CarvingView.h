// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CarvingView.h : CCarvingView 类的接口
//

#pragma once

#include "./UI/UICommon/TabBar.h"
#include "../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../include/KnifeDll/KnifeClass.h"
#include <map>

class CDlgRemainder;
class CDlgRequirement;
class CDlgResult;
class Panel;

struct PrepareExportGCodeData;
struct PrepareKnifeInfoData;
struct TransCoordinateData;
struct CombineFileData;

class CCarvingView : public CFormView
{
	enum ViewType{VIEW_REMAINDER, VIEW_PANEL_LIST, VIEW_RESULT};
	enum { IDD = IDD_INSTANCE };
protected: // 仅从序列化创建
	CCarvingView();
	DECLARE_DYNCREATE(CCarvingView)

// 特性
public:
	CCarvingDoc* GetDocument() const;

// 操作
public:
	vector<ComponentInputItem>& GetAllComponentInputItem(){return m_vComponentInputItem;}

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
public:
	virtual ~CCarvingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	//获得G码参数
	GcodeParam GetGcodeParam();

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnOpenSourceHG();
	afx_msg void OnOpenSourceHGAppend();
	afx_msg void OnOpenSourceExcelAppend();
	afx_msg void OnOptionsPen();
	virtual void OnInitialUpdate();
	afx_msg void OnViewRemainder();
	afx_msg void OnViewComponent();
	afx_msg void OnViewSolution();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOptimize();
	afx_msg void OnSave();
	afx_msg void OnEditPragmaSawKerf();
	afx_msg void OnEditPanelLength();
	afx_msg void OnEditPanelWidth();
	afx_msg void OnEditStep1Count();
	afx_msg void OnEditStep2Count();
	afx_msg void OnEditStep3Count();
	afx_msg void OnEditStep3AcceptableUti();
	afx_msg void OnComboStep1Alg();
	afx_msg void OnExportGcode();
	DECLARE_MESSAGE_MAP()

	void SwitchView(ViewType eViewType);
	CRect GetViewRect();
	CString GetProjectName();
	void ExportMeterialTable1(vector<Panel*>& vPanelToExport);
	void ExportMeterialTable2(vector<Panel*>& vPanelToExport);
	void ClearAllData();
	void CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);		// 检查并删除超出尺寸的板件
	//void CheckAndWarnOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);		// 检查并警告超出尺寸的板件
	void SetSmallCpnNoTexture(vector<ComponentInputItem>& vComponentInputItem);						// 设置小板无纹理
	bool CheckDeepHoleSlot(vector<ComponentInputItem>& vComponent);
	BOOL IsRepeatBarcode(vector<ComponentInputItem>& src_list, vector<ComponentInputItem>& add_list);
	BOOL IsRepeatBarcode(vector<ComponentInputItem>& src_list);
	void RecordMaterial();



	//导出CSV格式
	void OnWriteDataCSV(CString savePath, std::vector<std::string> title,std::vector<std::string> dataMap, std::vector<std::map<std::string,std::string>> dataSrc);
	//生成excel使用的数据源xml格式
	void SaveExcelFormat(CString savePath, std::vector<std::vector<std::string>> infos);
	//生成excel使用的数据源xml数据
	void SaveExcelData(CString savePath, std::vector<std::vector<std::string>> infos,std::vector<std::vector<std::map<std::string,std::string>>> dataSrcs);

	CTabBar m_Tab;
	CDlgRemainder* m_pDlgRemainder;
	CDlgRequirement* m_pDlgRequirement;
	CDlgResult* m_pDlgResult;
	ViewType m_eCurViewType;

	vector<ComponentInputItem> m_vComponentInputItem;
	//CString m_strOpenedFile;
	vector<CString> m_vOpenedFile;

	void DeleteSpecialCode(std::string &_code);
	void DeleteSpecialCode2(std::string &_code);

	bool eachLineAddChar(std::string patho,CString word);
	std::string ReplaceWithNextPath(std::string _code,std::string theNextPath);//替换*<>
	//刷新NC文件：命令字 G0X 变为 GX
	void Change_G0X_To_GX(std::string fileName);
	//刷新NC文件：坐标 X *** Y *** Z *** F *** -> X*** Y*** Z*** F***
	void Change_PosSpace_To_Empty(std::string fileName);
	//刷新NC文件：修改铣弧格式,将“G90 G03 X*** Y*** R***”格式，改为“G03 X*** Y*** U***”
	void Change_Arc_Milling_Format(std::string fileName);

	// 处理多余的回车\r和换行\n
	void dealRepeatCharReturnNewline(std::string fileName);

	bool PrepareExportGCode(PrepareExportGCodeData* pPrepareExportGCodeData);
	void PrepareKnifeInfoAndHoleSetting(PrepareExportGCodeData* pPrepareExportGCodeData, PrepareKnifeInfoData* pPrepareKnifeInfoData);
	void AfterExportGCode_TransCoordinate(PrepareExportGCodeData* pPrepareExportGCodeData, TransCoordinateData* pTransCoordinateData);
	void AfterExportGCode_CombineFile(PrepareExportGCodeData* pPrepareExportGCodeData, CombineFileData* pCombineFileData);
	int ExportComponentNC(vector<Panel*>& vComponentPanel);

	float _trim1;//反面修边值
	float _trim2;
	float _trim3;
	float _trim4;

	float _ftrim1;//正面修边值
	float _ftrim2;
	float _ftrim3;
	float _ftrim4;

	int _iscombine;//是否合并正反头尾

	std::string _header;//总文件头GFirst1
	std::string _ender;//总文件尾GLast1
	std::string _GFirst2;//总文件头GFirst2
	std::string _GLast2;//总文件尾GLast2

	std::string _GBFirst1;//总B文件头GFirst1
	std::string _GBFirst2;//总文件尾GLast1
	std::string _GBLast1;//总B文件WEI
	std::string _GBLast2;//总文件尾GLast2

	std::string _MFirst1;//主轴文件头GFirst1
	std::string _MLast1;//主轴文件尾GLast1
	std::string _MFirst2;//主轴文件头GFirst2
	std::string _MLast2;//主轴文件尾GLast2

	std::string _PZFirst1;//排钻文件头GFirst1
	std::string _PZLast1;//排钻文件尾GLast1
	std::string _PZFirst2;//排钻文件头GFirst2
	std::string _PZLast2;//排钻文件尾GLast2

	CString NCSavePath;


	//CString m_strProjectName;

	//void TMP_ConvertInputItemToOldInputItem(vector<map<string,string>>& vItem);

public:
	afx_msg void OnLayoutOrg();
	afx_msg void OnButtonPrintLabel();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnMenuMachineParamSetting();
	afx_msg void OnMenuBaseSetting();
	afx_msg void OnMenuOptimizeSetting();
	afx_msg void OnRemainderSetting();
	afx_msg void OnMenuDefaultLoadingDirSetting();
	afx_msg void OnMenuDefaultSavingDirSetting();
	afx_msg void OnMenuOtherSetting();
	afx_msg void OnMenuLabelSetting();
	afx_msg void OnMenuExcelTemplateSetting();
	afx_msg void OnMenuLingliaoTableSetting();
	afx_msg void OnExportMeterialTable1();
	afx_msg void OnExportMeterialTable2();
	afx_msg void OnOpenSourceExcel();
	afx_msg void OnOpenSolution();
	afx_msg void OnExportPanelMeterialTable();
	afx_msg LRESULT OnExportMaterialList(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnExportComponentNC(WPARAM wparam, LPARAM lparam);
	afx_msg void OnButtonAboutUs();
	afx_msg void OnButtonOurCustomers();
	afx_msg void OnButtonContackWithUsQQ();
	afx_msg void OnButtonContackWithUsWeiXin();
	afx_msg void OnButtonTutorial();
	afx_msg void OnButtonVideo();
	afx_msg void OnButtonOpenTemplate();
	afx_msg void OnMenuSaveAllSettings();
	afx_msg void OnMenuRevertAllSettings();


	static DWORD WINAPI LoginStatisticsDailyThread( PVOID lpThreadParameter );				// 日常登陆统计线程函数

};

#ifndef _DEBUG  // CarvingView.cpp 中的调试版本
inline CCarvingDoc* CCarvingView::GetDocument() const
   { return reinterpret_cast<CCarvingDoc*>(m_pDocument); }
#endif


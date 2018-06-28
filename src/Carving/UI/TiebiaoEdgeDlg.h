#pragma once

#include "../resource.h"
#include <vector>
#include <string>
#include <map>

// TiebiaoEdgeDlg 对话框

class TiebiaoEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TiebiaoEdgeDlg)

public:
	TiebiaoEdgeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TiebiaoEdgeDlg();

// 对话框数据
	enum { IDD = IDD_PARAM_TIEBIAO_EDGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	/************************************************************************/
	/* ui                                                                     */
	/************************************************************************/
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedFindImg();

public:
	/************************************************************************/
	/* 逻辑                                                                 */
	/************************************************************************/
	//内部绑定字符串解析到界面上
	void buildEdgeDataByOneEdgeString();
	//解析一行封边信息串
	std::map<std::string,std::string> buildEdgeDataByOneEdgeString(std::string oneEdgeString);
	
	//界面数据转换到内部数据字符串上
	void saveOneEdgeStringByEdgeData();
	//解析一组封边数据为一行字符串
	std::string saveOneEdgeStringByEdgeData(std::map<std::string,std::string> edgeData);

	//将一组封边信息的字符串解析为一组封边信息
	std::vector<std::map<std::string,std::string>> buildEdgeDataByAllEdgeString(std::string allEdgeString);
	//将一组封边信息转换为一行
	std::string saveAllEdgeStringByEdgeData(std::vector<std::map<std::string,std::string>> allEdgeData);

	//通过封边数值查找封边符号图片（需要设置AllEdgeString）
	std::string findImgPathByEdge(float edge);
	//通过封边数值查找封边符号字符（需要设置AllEdgeString）
	std::string findCharByEdge(float edge);

public:
	/************************************************************************/
	/* 数据                                                                 */
	/************************************************************************/
	// 封边厚度范围
	float m_fromThin;
	float FromThin() const { return m_fromThin; }
	void FromThin(float val) { m_fromThin = val; }

	// 封边厚度范围
	float m_toThin;
	float Thin() const { return m_toThin; }
	void Thin(float val) { m_toThin = val; }

	//显示的封边文字符号
	CString m_DisplayEdgeChar;
	CString DisplayEdgeChar() const { return m_DisplayEdgeChar; }
	void DisplayEdgeChar(CString val) { m_DisplayEdgeChar = val; }

	// 封边使用图片路径
	CString m_EdgeImgPath;
	CString EdgeImgPath() const { return m_EdgeImgPath; }
	void EdgeImgPath(CString val) { m_EdgeImgPath = val; }


	//一条封边数据
	CString m_oneEdgeString;
	CString OneEdgeString() const { return m_oneEdgeString; }
	void OneEdgeString(CString val) { m_oneEdgeString = val; }

	//全部封边数据
	CString m_allEdgeString;
	CString AllEdgeString() const { return m_allEdgeString; }
	void AllEdgeString(CString val) { m_allEdgeString = val; }
};

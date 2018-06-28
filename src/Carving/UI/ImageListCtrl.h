#pragma once
//#include "gdiplus.h"
//#include "hg3d/hgtypes.h"
//#include <hgIO/TextureLibCfg.h>
//#include "atlimage.h"
#include <vector>

using namespace std;

// CImageListCtrl

class CImageListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CImageListCtrl)

public:
	CImageListCtrl();
	virtual ~CImageListCtrl();

	//BOOL SetFolder(int icoWidth, int icoHeight, LPCTSTR pszFolder,const TextureLibCfg::MyNode* pNode);
	BOOL SetFolder(int icoWidth, int icoHeight, LPCTSTR pszFolder,CString stype);
	//BOOL SetFolder(int icoWidth, int icoHeight,  LPCTSTR pszFolder,int max_num);
	BOOL SetFolder(int icoWidth, int icoHeight, LPCTSTR pszFolder,int max_num,CString str_before);
 //   BOOL SetFolderItems(int icoWidth, int icoHeight, const hg3d::strvector& items);
 //   BOOL GetImageTitle(int nItem, CString& strTitle);
    BOOL GetImageFileName(int nItem, CString& strName);
 //   BOOL GetImageFilePath(int nItem, CString& strPath);

 //   BOOL SelectItem(LPCTSTR pszItemText, int &nSelectedItem, BOOL bNotify);
    BOOL SelectItem(int item, BOOL bNotify);
	bool SetCurselA(int iselect);

protected:
    BOOL SetItems(int icoWidth, int icoHeight,/* const hg3d::strvector& items*/vector<CString> vPanelImageFileDir);
	BOOL SetItems(int icoWidth, int icoHeight, /*const hg3d::strvector& items*/vector<CString> vPanelImageFileDir, bool nostr);
	//BOOL GetFiles(LPCTSTR pszFolder,const TextureLibCfg::MyNode* pNode);
	BOOL GetFiles(LPCTSTR pszFolder,CString stype);
	//BOOL GetFiles(LPCTSTR pszFolder,CString stype,CString ptype);
	//BOOL GetFiles(LPCTSTR pszFolder,int max_num);
	BOOL GetFiles(LPCTSTR pszFolder,CString str_before,int max_num);
	BOOL SetImageSize(int width, int height, int initCount);
	BOOL AddImage(LPCTSTR pszImageFile, int pos);
	Gdiplus::Bitmap* ScaleBitmap(Gdiplus::Bitmap* pBitmap, UINT nWidth, UINT nHeight, Gdiplus::Size & sValid);

protected:
	CImageList m_lstImages;
	////CStringArray m_strFiles;
	////CStringArray m_strFileTitles;
	////CString m_strFolder;
 //   hg3d::strvector m_itemValues;
	vector<CString> m_itemValues;
	vector<CString> m_vPanelImageFileDir;
	int m_width;
	int m_height;
	//float m_ratio;

protected:
	DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
};



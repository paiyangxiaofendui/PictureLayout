// ImageListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListCtrl.h"
#include <algorithm>
//#include <boost/filesystem.hpp>
//#include "hg3d/hgtypes.h"
//#include "UTF8ToMBCS.h"
#include "../Misc/Misc.h"


// CImageListCtrl
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CImageListCtrl, CListCtrl)

CImageListCtrl::CImageListCtrl()
: m_width(64)
, m_height(128)
//, m_ratio(1)
{
}

CImageListCtrl::~CImageListCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CImageListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

bool CImageListCtrl::SetCurselA(int iselect)
{
	return this->SelectItem(iselect,TRUE);

}
//
//BOOL CImageListCtrl::SetFolder(int icoWidth, int icoHeight, LPCTSTR pszFolder,const TextureLibCfg::MyNode* pNode)
//{
//    m_itemValues.clear();
//    if (!pszFolder) return FALSE;
//
//    if (!GetFiles(pszFolder,pNode))
//		return FALSE;
//
//    std::sort(m_itemValues.begin(), m_itemValues.end());
//
//    return SetItems(icoWidth, icoHeight, m_itemValues);
//}

BOOL CImageListCtrl::SetFolder(int icoWidth, int icoHeight, LPCTSTR pszFolder,CString stype)
{
	m_itemValues.clear();
	if (!pszFolder) return FALSE;

	if (!GetFiles(pszFolder,stype))
		return FALSE;

	std::sort(m_itemValues.begin(), m_itemValues.end());

	return SetItems(icoWidth, icoHeight, m_itemValues);
}

BOOL CImageListCtrl::SetFolder(int icoWidth, int icoHeight, LPCTSTR pszFolder,int max_num,CString str_before)
{
	//m_itemValues.clear();
	m_vPanelImageFileDir.clear();
	if (!pszFolder) return FALSE;

	if (!GetFiles(pszFolder,str_before,max_num))
		return FALSE;

	//std::sort(m_itemValues.begin(), m_itemValues.end());

	return SetItems(icoWidth, icoHeight, /*m_itemValues*/m_vPanelImageFileDir, true);
}
//
//BOOL CImageListCtrl::SetFolder(int icoWidth, int icoHeight,  LPCTSTR pszFolder,int max_num)
//{
//	m_itemValues.clear();
//	if (!pszFolder) return FALSE;
//
//	if (!GetFiles(pszFolder,max_num))
//		return FALSE;
//
//	//std::sort(m_itemValues.begin(), m_itemValues.end());
//
//	return SetItems(icoWidth, icoHeight, m_itemValues);
//}
//
//BOOL CImageListCtrl::SetFolderItems(int icoWidth, int icoHeight, const hg3d::strvector& items)
//{
//    m_itemValues.clear();
//    m_itemValues.assign(items.begin(), items.end());
//    std::sort(m_itemValues.begin(), m_itemValues.end());
//
//    return SetItems(icoWidth, icoHeight, m_itemValues);
//}
//
BOOL CImageListCtrl::SetItems(int icoWidth, int icoHeight, /*const hg3d::strvector& items*/vector<CString> vPanelImageFileDir, bool nostr)
{
	if (vPanelImageFileDir.size() == 0)
		return FALSE;

	if (!SetImageSize(icoWidth, icoHeight, vPanelImageFileDir.size()))
		return FALSE;

	if (!m_lstImages.SetImageCount(vPanelImageFileDir.size()))
		return FALSE;

	int pos = 0;
	for (vector<CString>::const_iterator it = vPanelImageFileDir.begin(); it != vPanelImageFileDir.end(); ++it)
	{
		if (!AddImage(/*it->c_str()*/*it, pos++)) return FALSE;
//#ifdef _DEBUG
//		TRACE1("Image count %d\n", m_lstImages.GetImageCount());
//#endif
	}

	// Insert items
	this->SetRedraw(FALSE);
	this->DeleteAllItems();
	int idx = 0;
	//boost::filesystem::path p;
	for (vector<CString>::const_iterator it = vPanelImageFileDir.begin(); it != vPanelImageFileDir.end(); ++it)
	{
		//p = *it;
		int nItem;
		if (nostr == true)
		{
			CString str_show;
			str_show.Format("%d",idx+1);
			std::string s_show;
			s_show = str_show.GetBuffer();

			nItem = this->InsertItem(idx,s_show.c_str(), idx);
		}
		else
			nItem = this->InsertItem(idx, /*p.stem().string().c_str()*/*it, idx);
		if (-1 == nItem)
			return FALSE;
		this->SetItemData(nItem, (DWORD_PTR) idx);
		++idx;
	}

	this->SetRedraw(TRUE);
	this->UpdateWindow();
	return TRUE;
}

BOOL CImageListCtrl::SetItems(int icoWidth, int icoHeight, /*const hg3d::strvector& items*/vector<CString> vPanelImageFileDir)
{
	if (vPanelImageFileDir.size() == 0)
		return FALSE;

	if (!SetImageSize(icoWidth, icoHeight, vPanelImageFileDir.size()))
		return FALSE;

	if (!m_lstImages.SetImageCount(vPanelImageFileDir.size()))
		return FALSE;

    int pos = 0;
	for (vector<CString>::const_iterator it = vPanelImageFileDir.begin(); it != vPanelImageFileDir.end(); ++it)
	{
        if (!AddImage(/*it->c_str()*/*it, pos++)) return FALSE;
//#ifdef _DEBUG
//    TRACE1("Image count %d\n", m_lstImages.GetImageCount());
//#endif
    }

    // Insert items
    this->SetRedraw(FALSE);
    this->DeleteAllItems();
    int idx = 0;
    //boost::filesystem::path p;
    for (vector<CString>::const_iterator it = vPanelImageFileDir.begin(); it != vPanelImageFileDir.end(); ++it)
	{
        //p = *it;
		CString strItemName;
		int nPos, nPos1;
		nPos = (*it).ReverseFind(_T('\\'));
		nPos1 = (*it).ReverseFind(_T('.'));
		strItemName = (*it).Mid(nPos+1, nPos1 - nPos-1);
        int nItem = this->InsertItem(idx, /*p.stem().string().c_str()*/strItemName, idx);
        if (-1 == nItem) return FALSE;
        this->SetItemData(nItem, (DWORD_PTR) idx);
        ++idx;
    }
	
    this->SetRedraw(TRUE);
    this->UpdateWindow();
    return TRUE;
}

BOOL CImageListCtrl::SetImageSize(int width, int height, int initCount)
{
	m_width = width;
	m_height = height;

	m_lstImages.DeleteImageList();
	if (!m_lstImages.Create(m_width, m_height, ILC_COLOR32, initCount, 4))
		return FALSE;

	SetImageList(&m_lstImages, LVSIL_NORMAL);

	return TRUE;
}
//
BOOL CImageListCtrl::AddImage(LPCTSTR pszImageFile, int pos)
{
    // Load image from file
#if !defined(_UNICODE)
    CStringW strWFile;
    //if (!MBToUnicode(pszImageFile, _tcslen(pszImageFile)+1, strWFile))
    //    return FALSE;
	strWFile = AnsiToUnicode(pszImageFile).c_str();
	std::auto_ptr<Bitmap> spBitmap(Bitmap::FromFile((LPCWSTR) strWFile));
#else
    std::auto_ptr<Bitmap> spBitmap(Bitmap::FromFile(pszImageFile));
#endif
	if (NULL == spBitmap.get())
		return FALSE;

    //float factor = min((float)(((float)m_width) / pBmp->GetWidth()), ((float) m_height)/pBmp->GetHeight());
    Size sValid;
    std::auto_ptr<Bitmap> spBmpScaled(ScaleBitmap(spBitmap.get(), m_width, m_height, sValid));
    if (NULL == spBmpScaled.get()) return false;

	HBITMAP hbmp;
	spBmpScaled->GetHBITMAP(RGB(255, 255, 255), &hbmp);
    CBitmap* pTempBmp = CBitmap::FromHandle(hbmp);
	//m_lstImages.Add(pTempBmp, RGB(255, 255, 255));
    m_lstImages.Replace(pos, pTempBmp, NULL);
    //delete pTempBmp;

	return TRUE;
}
//
//BOOL CImageListCtrl::GetFiles(LPCTSTR pszFolder,const TextureLibCfg::MyNode* pNode)
//{
//    hg3d::strstrmap files; ///< 对文件名排序
//
//	CString strName = pszFolder;
//	strName += _T("\\*.jpg");
//	CFileFind   finder;
//	BOOL bWorking = finder.FindFile(strName);
//	while (bWorking) {
//		bWorking   =   finder.FindNextFile();
//		if (finder.IsArchived()) 
//		{
//			hg3d::XmlNodeVec pNodes = pNode->GetChildNodes();
//			std::string path_node =std:: string((char*)pszFolder); 
//			bool m_insert = false;
//			for(auto iter_node = pNodes.begin();iter_node != pNodes.end();iter_node++)
//			{
//				hg3d::CXmlNode * node = * iter_node;
//				std::string m_path = path_node + "\\" + node->GetAttributeValue("name") + ".jpg";
//				std::string get_path(finder.GetFilePath().GetBuffer());
//				//修改大写JPG也显示.LZB 2016.6.8
//				CString Cget_path = get_path.c_str();
//				Cget_path.Replace("JPG","jpg");
//				if (m_path == Cget_path.GetBuffer())
//				{
//					m_insert = true;
//					break;
//				}
//
//			}
//			if (m_insert == true)
//			{
//				m_itemValues.push_back((LPCTSTR) finder.GetFilePath());
//			}
//		}
//	}
//
//    //for (hg3d::strstrmap::const_iterator it = files.begin(); it != files.end(); ++it)
//    //{
//    //    m_strFiles.Add(it->second.c_str());
//    //    m_strFileTitles.Add(it->first.c_str());
//    //}
//
//	return TRUE;
//}
//
BOOL CImageListCtrl::GetFiles(LPCTSTR pszFolder,CString stype)
{
	//hg3d::strstrmap files; ///< 对文件名排序

	CString strName = pszFolder;
	strName += _T("\\*.jpg");
	CFileFind   finder;
	BOOL bWorking = finder.FindFile(strName);
	while (bWorking) {
		bWorking   =   finder.FindNextFile();
		vector<CString> alltype;
		CString t_type = stype;
		while (t_type != "")
		{
			alltype.push_back(t_type.Left(t_type.Find(";")));
			t_type = t_type.Right(t_type.GetLength()-t_type.Find(";")-1);
		}
		if (finder.IsArchived()) 
		{
			std::string path_node =std::string((char*)pszFolder); 
			bool m_insert = false;
			for(auto iter_type = alltype.begin();iter_type != alltype.end();iter_type++)
			{
				CString ctype = * iter_type;
				std::string m_path = path_node + "\\" + ctype.GetBuffer() + ".jpg";
				std::string get_path(finder.GetFilePath().GetBuffer());
				//修改大写JPG也显示.LZB 2016.6.8
				CString Cget_path = get_path.c_str();
				Cget_path.Replace("JPG","jpg");
				if (m_path == Cget_path.GetBuffer())
				{
					m_insert = true;
					break;
				}

			}
			if (m_insert == true || stype == "#")
			{
				m_itemValues.push_back((LPCTSTR) finder.GetFilePath());
			}
		}
	}

	//for (hg3d::strstrmap::const_iterator it = files.begin(); it != files.end(); ++it)
	//{
	//    m_strFiles.Add(it->second.c_str());
	//    m_strFileTitles.Add(it->first.c_str());
	//}

	return TRUE;
}

//
//BOOL CImageListCtrl::GetFiles(LPCTSTR pszFolder,CString stype,CString ptype)
//{
//	hg3d::strstrmap files; ///< 对文件名排序
//
//	CString strName = pszFolder;
//	strName += _T("\\*");
//	strName += ptype;
//	CFileFind   finder;
//	BOOL bWorking = finder.FindFile(strName);
//	while (bWorking) {
//		bWorking   =   finder.FindNextFile();
//		vector<CString> alltype;
//		CString t_type = stype;
//		while (t_type != "")
//		{
//			alltype.push_back(t_type.Left(t_type.Find(";")));
//			t_type = t_type.Right(t_type.GetLength()-t_type.Find(";")-1);
//		}
//		if (finder.IsArchived()) 
//		{
//			std::string path_node =std::string((char*)pszFolder); 
//			bool m_insert = false;
//			for(auto iter_type = alltype.begin();iter_type != alltype.end();iter_type++)
//			{
//				CString ctype = * iter_type;
//				std::string m_path = path_node + "\\" + ctype.GetBuffer();
//				std::string get_path(finder.GetFilePath().GetBuffer());
//				if (m_path == get_path)
//				{
//					m_insert = true;
//					break;
//				}
//
//			}
//			if (m_insert == true || stype == "#")
//			{
//				m_itemValues.push_back((LPCTSTR) finder.GetFilePath());
//			}
//		}
//	}
//
//	//for (hg3d::strstrmap::const_iterator it = files.begin(); it != files.end(); ++it)
//	//{
//	//    m_strFiles.Add(it->second.c_str());
//	//    m_strFileTitles.Add(it->first.c_str());
//	//}
//
//	return TRUE;
//}
//
//BOOL CImageListCtrl::GetFiles(LPCTSTR pszFolder,int max_num)
//{
//
//	CString strName = pszFolder;
//	strName += _T("\\");
//	
//	for (int i = 0;i < max_num;i++)
//	{
//		CString s_name;
//		s_name.Format("%d.jpg",i);
//		CFileFind   finder;
//		BOOL bWorking = finder.FindFile(strName + s_name);
//		if (bWorking == true)
//		{
//			m_itemValues.push_back((LPCTSTR)(strName + s_name));
//		}
//	}
//	return TRUE;
//}
//
BOOL CImageListCtrl::GetFiles(LPCTSTR pszFolder,CString str_before,int max_num)
{

	CString strName = pszFolder;
	strName += _T("\\");

	for (int i = 0;i < max_num;i++)
	{
		CString s_name;
		s_name.Format("%d.bmp",i);
		s_name = str_before + "-" + s_name;

		CFileFind   finder;
		BOOL bWorking = finder.FindFile(strName + s_name);
		if (bWorking == true)
		{
			//m_itemValues.push_back((LPCTSTR)(strName + s_name));
			m_vPanelImageFileDir.push_back((LPCTSTR)(strName + s_name));
		}
	}

	return TRUE;
}
//
Bitmap* CImageListCtrl::ScaleBitmap(Bitmap* pBitmap, UINT nWidth, UINT nHeight, Size& sValid)
{
	Bitmap* pTemp = new Bitmap(nWidth, nHeight, pBitmap->GetPixelFormat());
    if (NULL == pTemp) return NULL;

    Graphics * g = Graphics::FromImage(pTemp);
    if (g)
    {
        // use the best interpolation mode
        g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
        g->Clear(Color(255, 255, 255, 255));

        float factor = min((float) (((float) m_width) / pBitmap->GetWidth()), (((float) m_height) / pBitmap->GetHeight()));
        int sizex = pBitmap->GetWidth() * factor;
        int sizey = pBitmap->GetHeight() * factor;
        int offsetx = (m_width - sizex) / 2;
        int offsety = (m_height - sizey) / 2;
        g->DrawImage(pBitmap, offsetx, offsety, sizex, sizey);
        sValid.Width = sizex;
        sValid.Height = sizey;
        delete g;
    }

	return pTemp;
}

void CImageListCtrl::PreSubclassWindow()
{
    this->SetBkColor(RGB(255, 255, 255));
    this->ModifyStyle(LVS_TYPEMASK|LVS_ALIGNMASK, LVS_ICON|LVS_ALIGNTOP|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
}

BOOL CImageListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &= ~LVS_TYPEMASK;
    cs.style |= LVS_ICON|LVS_SINGLESEL|LVS_SHOWSELALWAYS;

    return CListCtrl::PreCreateWindow(cs);
}
//
//BOOL CImageListCtrl::GetImageTitle( int nItem, CString& strTitle )
//{
//    if (nItem < 0) return FALSE;
//    int index = (int) (this->GetItemData(nItem));
//
//    if (index >= 0 && index < m_itemValues.size()) {
//        boost::filesystem::path p(m_itemValues[index]);
//        strTitle = p.stem().string().c_str();
//        return TRUE;
//    }
//
//    return FALSE;
//}

BOOL CImageListCtrl::GetImageFileName( int nItem, CString& strName )
{
	if(nItem == -1 || nItem >= GetItemCount())
		return FALSE;

	int index = (int) (this->GetItemData(nItem));

	if (index >= 0 && index < m_itemValues.size())
	{
		//boost::filesystem::path p(m_itemValues[index]);
		//strName = p.filename().string().c_str();
		CString strFileFullName = m_itemValues[index];
		strName = strFileFullName.Right(strFileFullName.GetLength() - (strFileFullName.ReverseFind(_T('\\')) + 1));
		return TRUE;
	}

	return FALSE;
}

//BOOL CImageListCtrl::GetImageFilePath( int nItem, CString& strPath )
//{
//    int index = (int) (this->GetItemData(nItem));
//
//    if (index >= 0 && index < m_itemValues.size()) 
//	{
//        strPath = m_itemValues[index].c_str();
//
//        return TRUE;
//    }
//
//    return FALSE;
//}
//
//
//BOOL CImageListCtrl::SelectItem(LPCTSTR pszItemText, int &nSelectedItem, BOOL bNotify)
//{
//    for (int i = 0; i < GetItemCount(); ++i)
//    {
//        CString strText = GetItemText(i, 0);
//        if (strText != pszItemText) continue;
//
//        nSelectedItem = i;
//        if (!SetItemState(i, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED))
//            return FALSE;
//
//        this->EnsureVisible(i, FALSE);
//
//        if (bNotify)
//        {
//            NMLISTVIEW nmlv;
//            ZeroMemory(&nmlv, sizeof(NMLISTVIEW));
//            nmlv.iItem = i;
//            nmlv.hdr.idFrom = ::GetWindowLong(GetSafeHwnd(), GWL_ID);
//            nmlv.uChanged = LVIF_STATE;
//            nmlv.uNewState = LVIS_SELECTED;
//            ::SendMessage(GetParent()->GetSafeHwnd(), LVN_ITEMCHANGED, (WPARAM) nmlv.hdr.idFrom, (LPARAM) &nmlv);
//        }
//
//        return TRUE;
//    }
//
//    return FALSE;
//}

BOOL CImageListCtrl::SelectItem(int item, BOOL bNotify)
{
    if (GetItemCount()+1 < item || !SetItemState(item, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED))
        return FALSE;

    this->EnsureVisible(item, FALSE);

    if (bNotify)
    {
        NMLISTVIEW nmlv;
        ZeroMemory(&nmlv, sizeof(NMLISTVIEW));
        nmlv.iItem = item;
        nmlv.hdr.idFrom = ::GetWindowLong(GetSafeHwnd(), GWL_ID);
        nmlv.uChanged = LVIF_STATE;
        nmlv.uNewState = LVIS_SELECTED;
        ::SendMessage(GetParent()->GetSafeHwnd(), LVN_ITEMCHANGED, (WPARAM) nmlv.hdr.idFrom, (LPARAM) &nmlv);
    }

    return TRUE;
}


void CImageListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = CDRF_DODEFAULT; 

    switch (pNMCD->dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        return;
    case CDDS_ITEMPREPAINT:
        {
            int nItem = static_cast<int>(pNMCD->dwItemSpec);

            CDC* pDC = CDC::FromHandle(pNMCD->hdc);
            COLORREF crBkgnd;
            BOOL bListHasFocus;
            CRect  rcBounds;
            CRect  rcIcon;
            CRect  rcText;
            CString  sText;
            UINT uFormat;

            bListHasFocus = (this->GetSafeHwnd() == ::GetFocus());

            // Get the image index and selected/focused state of the
            // item being drawn.
            LVITEM rItem;
            ZeroMemory(&rItem, sizeof(LVITEM));
            rItem.mask = LVIF_IMAGE | LVIF_STATE;
            rItem.iItem = nItem;
            rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
            this->GetItem(&rItem);

            // Get the rect that holds the item 's icon.
            this->GetItemRect(nItem, &rcBounds, LVIR_BOUNDS);
            this->GetItemRect(nItem, &rcIcon, LVIR_ICON);

            // Draw the icon.
            uFormat = ILD_TRANSPARENT;

            COLORREF clrSHlight = GetSysColor(COLOR_HIGHLIGHT);
            //if (rItem.state & LVIS_SELECTED)
            //{
            //    pDC->FillSolidRect(&rcBounds, clrSHlight);

            //    if (bListHasFocus)
            //    {
            //        CBrush br(RGB(255, 255, 0));
            //        pDC->FrameRect(CRect(rcBounds.left, rcBounds.top, rcBounds.right, rcBounds.bottom), &br);
            //    }
            //}

            //if ((rItem.state & LVIS_SELECTED) && bListHasFocus )
            //    uFormat |= ILD_FOCUS;

            //CImageList *pImageList = GetImageList(LVSIL_NORMAL);
            IMAGEINFO ii;
            m_lstImages.GetImageInfo(rItem.iImage, &ii);
            //CPoint pt(rcBounds.left+(rcBounds.Width() - (ii.rcImage.right - ii.rcImage.left))/2, rcBounds.top+3);
            CPoint pt(rcIcon.left+(rcIcon.Width() - (ii.rcImage.right - ii.rcImage.left))/2, rcIcon.top+3);
            m_lstImages.Draw(pDC, rItem.iImage, pt, uFormat);

            // Get the rect that bounds the text label.
            this->GetItemRect(nItem, rcText, LVIR_LABEL);

            // Draw the background of the list item. Colors are selected
            // according to the item 's state.
            if ( rItem.state & LVIS_SELECTED )
            {
                crBkgnd = clrSHlight;//GetSysColor(COLOR_HIGHLIGHT);
                if (bListHasFocus)
                {
                    //crBkgnd = GetSysColor ( COLOR_HIGHLIGHT );
                    pDC-> SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
                }
                else
                {
                    //crBkgnd = GetSysColor ( COLOR_BTNFACE );
                    pDC-> SetTextColor(GetSysColor(COLOR_BTNTEXT));
                }
            }
            else
            {
                crBkgnd = GetSysColor(COLOR_WINDOW);
                pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
            }

            // Draw the background & prep the DC for the text drawing. Note
            // that the entire item RECT is filled in, so this emulates the full-
            // row selection style of normal lists.
            pDC-> FillSolidRect(rcText, crBkgnd);
            //pDC-> SetBkMode(TRANSPARENT);

            // Tweak the rect a bit for nicer-looking text alignment.
            // Draw the text.
            uFormat = DT_CENTER;
            if (((GetStyle() & LVS_TYPEMASK) & LVS_NOLABELWRAP) != LVS_NOLABELWRAP)
                uFormat |= DT_WORDBREAK;
            sText = this->GetItemText(nItem, 0);
            pDC->DrawText(sText, rcText, uFormat);

            // Draw a focus rect around the item if necessary.
            if (bListHasFocus && (rItem.state & LVIS_FOCUSED))
            {
                pDC->DrawFocusRect(rcText);
            }

            *pResult |= CDRF_SKIPDEFAULT; // We 've painted everything.
        }
        return;
   }

   *pResult = 0;
}

BOOL CImageListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_MOUSEMOVE)//鼠标移动消息
	{
		CWnd *pWnd = GetParent(); //获取父窗口指针
		if(pWnd && pWnd->GetSafeHwnd()) //父窗口指针有效
		{
			//计算此控件与父窗口的位置偏差
			CRect this_rect;
			GetWindowRect(&this_rect);
			CRect parent_rect;
			pWnd->GetWindowRect(&parent_rect);
			CPoint offset(this_rect.left - parent_rect.left,this_rect.top - parent_rect.top);

			//当前窗口中的鼠标位置
			CPoint point(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));

			//换算到父窗口位置
			CPoint parentPoint = point + offset;

			//传递消息给父窗口
			::PostMessage(pWnd->m_hWnd,WM_MOUSEMOVE,pMsg->wParam,MAKELONG(parentPoint.x,parentPoint.y));
		}    
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
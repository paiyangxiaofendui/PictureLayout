// RibbonButtonSelf.cpp : 实现文件
//

#include "stdafx.h"
//#include "Carving.h"
#include "RibbonButtonSelf.h"
#include "../../Misc/Misc.h"

// CRibbonButtonSelf

CRibbonButtonSelf::CRibbonButtonSelf()
{
}

CRibbonButtonSelf::CRibbonButtonSelf(UINT nID, LPCTSTR lpszText, int nSmallImageIndex, int nLargeImageIndex, UINT uImgResID, BOOL bAlwaysShowDescription, bool bStretch, bool bDrawText)
	:CMFCRibbonButton(nID, lpszText, nSmallImageIndex, nLargeImageIndex, bAlwaysShowDescription)
	,m_bStretch(bStretch)
	,m_bDrawText(bDrawText)
{
	m_pImgIcon = LoadPngImgFromRes(uImgResID);
}

CRibbonButtonSelf::~CRibbonButtonSelf()
{
	if(m_pImgIcon)
		delete m_pImgIcon;
}


// CRibbonButtonSelf 成员函数
void CRibbonButtonSelf::OnDraw(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	//if (IsDefaultMenuLook() && !IsQATMode() && !m_bIsLargeImage)
	//{
	//	CMFCToolBarMenuButton dummy;

	//	dummy.m_strText = m_strText;
	//	dummy.m_nID = m_nID;
	//	dummy.m_bMenuMode = TRUE;
	//	dummy.m_pWndParent = GetParentWnd();
	//	dummy.m_bIsRadio = m_bIsRadio;

	//	if (IsChecked())
	//	{
	//		dummy.m_nStyle |= TBBS_CHECKED;
	//	}

	//	if (HasMenu())
	//	{
	//		dummy.m_bDrawDownArrow = TRUE;
	//	}

	//	BOOL bIsHighlighted = m_bIsHighlighted;

	//	if (IsDisabled())
	//	{
	//		dummy.m_nStyle |= TBBS_DISABLED;

	//		bIsHighlighted = IsFocused();
	//	}

	//	dummy.OnDraw(pDC, m_rect, NULL, TRUE, FALSE, bIsHighlighted || m_bIsFocused);
	//	return;
	//}

	BOOL bIsDisabled = m_bIsDisabled;
	BOOL bIsDroppedDown = m_bIsDroppedDown;
	BOOL bIsHighlighted = m_bIsHighlighted;
	BOOL bMenuHighlighted = m_bIsMenuHighlighted;
	BOOL bCommandHighlighted = m_bIsCommandHighlighted;

	const int cxDropDown = GetDropDownImageWidth();

	if (m_bIsDisabled && HasMenu())
	{
		if (m_bIsDefaultCommand || (!m_bIsDefaultCommand && !(m_nID == 0 || m_nID == (UINT)-1)))
		{
			m_bIsHighlighted = FALSE;
		}
		else
		{
			m_bIsDisabled = FALSE;
		}
	}

	if (m_bToBeClosed)
	{
		m_bIsDroppedDown = FALSE;
	}

	if (m_bIsFocused)
	{
		m_bIsHighlighted = TRUE;
		m_bIsMenuHighlighted = TRUE;
		m_bIsCommandHighlighted = TRUE;
	}

	CRect rectMenuArrow;
	rectMenuArrow.SetRectEmpty();

	if (HasMenu())
	{
		rectMenuArrow = m_rect;

		rectMenuArrow.left = rectMenuArrow.right - cxDropDown - m_nMenuArrowMargin;
		if (m_sizeTextRight.cx == 0 && !m_bQuickAccessMode)
		{
			rectMenuArrow.left -= 2;
		}

		rectMenuArrow.bottom -= m_nMenuArrowMargin;

		if (m_bIsDefaultCommand)
		{
			m_rectMenu = m_rect;

			m_rectMenu.left = m_rectMenu.right - cxDropDown - m_nMenuArrowMargin - 1;

			m_rectCommand = m_rect;
			m_rectCommand.right = m_rectMenu.left;

			m_bMenuOnBottom = FALSE;
		}
	}

	CSize sizeImageLarge = GetImageSize(RibbonImageLarge);
	CSize sizeImageSmall = GetImageSize(RibbonImageSmall);

	CRect rectText = m_rect;
	BOOL bDrawText = !IsApplicationButton() && !m_bQuickAccessMode && !m_bFloatyMode;

	if (m_bQuickAccessMode || m_bFloatyMode)
	{
		bDrawText = FALSE;
	}
	else if (m_bCompactMode)
	{
		bDrawText = FALSE;
	}
	else if (sizeImageLarge != CSize(0, 0) && !m_bMenuOnBottom && m_bIsLargeImage)
	{
		if (!m_rectMenu.IsRectEmpty())
		{
			m_rectMenu.left -= cxDropDown;
			m_rectCommand.right = m_rectMenu.left;
		}

		rectMenuArrow.OffsetRect(-cxDropDown / 2, 0);
	}

	const RibbonImageType imageType = m_bIsLargeImage ? RibbonImageLarge : RibbonImageSmall;

	CSize sizeImage = GetImageSize(imageType);
	BOOL bDrawDefaultImage = FALSE;

	if ((m_bQuickAccessMode || m_bFloatyMode) && sizeImage == CSize(0, 0))
	{
		// Use default image:
		sizeImage = CSize(16, 16);

		if (afxGlobalData.GetRibbonImageScale() != 1.)
		{
			sizeImage.cx = (int)(.5 + afxGlobalData.GetRibbonImageScale() * sizeImage.cx);
			sizeImage.cy = (int)(.5 + afxGlobalData.GetRibbonImageScale() * sizeImage.cy);
		}

		bDrawDefaultImage = TRUE;
	}

	CRect rectImage = m_rect;
	rectImage.DeflateRect(m_szMargin);

	if (IsApplicationButton())
	{
		if (afxGlobalData.GetRibbonImageScale() != 1.)
		{
			sizeImage.cx = (int) (.8 * afxGlobalData.GetRibbonImageScale() * sizeImage.cx);
			sizeImage.cy = (int) (.8 * afxGlobalData.GetRibbonImageScale() * sizeImage.cy);
		}

		rectImage.left += (rectImage.Width () - sizeImage.cx) / 2;
		rectImage.top  += (rectImage.Height () - sizeImage.cy) / 2;

		rectImage.OffsetRect(CMFCVisualManager::GetInstance ()->GetRibbonMainImageOffset());
	}
	else if (m_bIsLargeImage && !m_bTextAlwaysOnRight)
	{
		rectImage.left = rectImage.CenterPoint().x - sizeImage.cx / 2;
		rectImage.top += m_szMargin.cy + 1;

		if (!bDrawText)
		{
			rectImage.top = rectImage.CenterPoint().y - sizeImage.cy / 2;
		}
	}
	else
	{
		rectImage.top = rectImage.CenterPoint().y - sizeImage.cy / 2;
	}

	rectImage.right = rectImage.left + sizeImage.cx;
	rectImage.bottom = rectImage.top + sizeImage.cy;

	if (m_bIsLargeImage && !m_bTextAlwaysOnRight && HasMenu() && m_bIsDefaultCommand)
	{
		m_rectMenu = m_rect;
		m_rectMenu.top = rectImage.bottom + 3;

		m_rectCommand = m_rect;
		m_rectCommand.bottom = m_rectMenu.top;

		m_bMenuOnBottom = TRUE;
	}

	COLORREF clrText = (COLORREF)-1;

	if (!IsApplicationButton())
	{
		clrText = OnFillBackground(pDC);
	}

	if (IsMenuMode() && IsChecked() && sizeImage != CSize(0, 0))
	{
		CMFCVisualManager::GetInstance()->OnDrawRibbonMenuCheckFrame(pDC, this, rectImage);
	}

	//------------
	// Draw image:
	//------------
	if (bDrawDefaultImage)
	{
		CMFCVisualManager::GetInstance()->OnDrawDefaultRibbonImage(pDC, rectImage, m_bIsDisabled, m_bIsPressed, m_bIsHighlighted);
	}
	else
	{
		BOOL bIsRibbonImageScale = afxGlobalData.IsRibbonImageScaleEnabled();

		if (IsMenuMode() && !m_bIsLargeImage)
		{
			if (m_pParentMenu == NULL || !m_pParentMenu->IsMainPanel())
			{
				afxGlobalData.EnableRibbonImageScale(FALSE);
			}
		}

		//DrawImage(pDC, imageType, rectImage);
		CRect rcIcon;
		if(!m_bStretch)
		{
			rcIcon.SetRect(rectImage.left - (m_pImgIcon->GetWidth()-rectImage.Width())/2, \
				rectImage.top, \
				rectImage.right + (m_pImgIcon->GetWidth()- rectImage.Width())/2, \
				rectImage.top+m_pImgIcon->GetHeight());
		}
		else
		{
			rcIcon = rectImage;
		}

		Graphics g(pDC->m_hDC);
		g.DrawImage(m_pImgIcon, RectF(rcIcon.left, rcIcon.top, rcIcon.Width(), rcIcon.Height()), 0, 0, m_pImgIcon->GetWidth(), m_pImgIcon->GetHeight(), UnitPixel);
		afxGlobalData.EnableRibbonImageScale(bIsRibbonImageScale);
	}

	//-----------
	// Draw text:
	//-----------
	if (/*bDrawText*/m_bDrawText)
	{
		CFont* pOldFont = NULL;

		rectText = m_rect;
		UINT uiDTFlags = 0;

		COLORREF clrTextOld = (COLORREF)-1;

		if (bIsDisabled && (m_bIsDefaultCommand || (!m_bIsDefaultCommand && !(m_nID == 0 || m_nID == (UINT)-1))))
		{
			if (m_bQuickAccessMode)
			{
				clrText = CMFCVisualManager::GetInstance()->GetRibbonQuickAccessToolBarTextColor(TRUE);
			}
			else
			{
				clrTextOld = pDC->SetTextColor( clrText == (COLORREF)-1 ? CMFCVisualManager::GetInstance()->GetToolbarDisabledTextColor() : clrText);
			}
		}
		else if (clrText != (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor(clrText);
		}

		if (m_bIsLargeImage && !m_bTextAlwaysOnRight)
		{
			DrawBottomText(pDC, FALSE);
			rectMenuArrow.SetRectEmpty();
		}
		else
		{
			rectText.left = rectImage.right;

			if (m_nImageOffset > 0)
			{
				rectText.left = m_rect.left + m_nImageOffset + 3 * m_szMargin.cx;
			}
			else if (sizeImage.cx != 0)
			{
				rectText.left += GetTextOffset();
			}

			uiDTFlags = DT_SINGLELINE | DT_END_ELLIPSIS;

			if (!m_bAlwaysShowDescription || m_strDescription.IsEmpty())
			{
				uiDTFlags |= DT_VCENTER;
			}
			else
			{
				pOldFont = pDC->SelectObject(&afxGlobalData.fontBold);
				ENSURE(pOldFont != NULL);

				rectText.top += max(0, (m_rect.Height() - m_sizeTextRight.cy) / 2);
			}

			int nTextHeight = DrawRibbonText(pDC, m_strText, rectText, uiDTFlags);

			if (pOldFont != NULL)
			{
				pDC->SelectObject(pOldFont);
			}

			if (m_bAlwaysShowDescription && !m_strDescription.IsEmpty())
			{
				rectText.top += nTextHeight + m_szMargin.cy;
				rectText.right = m_rect.right - m_szMargin.cx;

				pDC->DrawText(m_strDescription, rectText, DT_WORDBREAK | DT_END_ELLIPSIS);
			}

			if (nTextHeight == m_sizeTextRight.cy && m_bIsLargeImage && HasMenu())
			{
				rectMenuArrow = m_rect;
				rectMenuArrow.DeflateRect(m_nMenuArrowMargin, m_nMenuArrowMargin * 2);
				rectMenuArrow.right -= 2;

				int cyMenu = CMenuImages::Size().cy;

				rectMenuArrow.top = rectMenuArrow.bottom - cyMenu;
				rectMenuArrow.bottom = rectMenuArrow.top + CMenuImages::Size().cy;
			}
		}

		if (clrTextOld != (COLORREF)-1)
		{
			pDC->SetTextColor(clrTextOld);
		}
	}

	if (!IsApplicationButton())
	{
		if (!rectMenuArrow.IsRectEmpty())
		{
			CMenuImages::IMAGES_IDS id = CMenuImages::IdArrowDown;

			if (IsMenuMode())
			{
				BOOL bIsRTL = FALSE;

				CMFCRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar();
				if (pTopLevelRibbon->GetSafeHwnd() != NULL)
				{
					bIsRTL = (pTopLevelRibbon->GetExStyle() & WS_EX_LAYOUTRTL);
				}

				id = bIsRTL ? CMenuImages::IdArrowLeftLarge : CMenuImages::IdArrowRightLarge;
			}

			CRect rectWhite = rectMenuArrow;
			rectWhite.OffsetRect(0, 1);

			CMenuImages::Draw(pDC, id, rectWhite, CMenuImages::ImageWhite);
			CMenuImages::Draw(pDC, id, rectMenuArrow, m_bIsDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
		}

		OnDrawBorder(pDC);
	}

	//pDC->TextOut(0, 0, "hello");

	m_bIsDisabled = bIsDisabled;
	m_bIsDroppedDown = bIsDroppedDown;
	m_bIsHighlighted = bIsHighlighted;
	m_bIsMenuHighlighted = bMenuHighlighted;
	m_bIsCommandHighlighted = bCommandHighlighted;
}
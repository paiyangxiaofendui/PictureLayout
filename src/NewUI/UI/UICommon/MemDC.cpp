#include "stdafx.h"
#include "MEMDC.H"

CMemoryDC::CMemoryDC(CDC* pDC, const CRect& rcBounds) : CDC()
{
	CreateCompatibleDC(pDC);
	m_bitmap = new CBitmap;
	m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
	m_oldBitmap = SelectObject(m_bitmap);
	m_pDC = pDC;
	m_rcBounds = rcBounds;
}
CMemoryDC::~CMemoryDC() 
{
	m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
		this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
	SelectObject(m_oldBitmap);
	if(m_bitmap->GetSafeHandle())
		m_bitmap->DeleteObject();
	if (m_bitmap != NULL)
	{
		delete m_bitmap;
		m_bitmap = NULL;
	}
}
CMemoryDC* CMemoryDC::operator->() 
{
	return this;
}
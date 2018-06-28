#ifndef _MEMDC_H_
#define _MEMDC_H_

//#pragma once

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements a memory Device Context

class CMemoryDC: public CDC 
{
public:
	CMemoryDC(CDC* pDC, const CRect& rcBounds);

	CMemoryDC* operator->();
	~CMemoryDC();

private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
};

#endif
#include "stdafx.h"
#include "UIData.h"

void PreCombineComponent::SetButtonState( BUTTON_STATE nState)
{	
	BOOL bErase = FALSE;
	if (m_nState != nState)
		bErase = TRUE;
	m_nState = nState ;
}

void OutlineLine::SetValue(float fStartX, float fStartY, float fEndX, float fEndY, int nLineType, float fR, int nDir)
{
	m_fStartX = fStartX;
	m_fStartY = fStartY;
	m_fEndX = fEndX;
	m_fEndY = fEndY;
	m_nLineType = nLineType;
	m_fR = fR;
	m_nDir = nDir;
}
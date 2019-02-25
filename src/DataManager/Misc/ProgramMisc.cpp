#include "stdafx.h"
#include "ProgramMisc.h"



void FindAllComponentInPanel(Component& thePanel, vector<Component*>& vAllComponent)
{
	for(int i = 0; i < thePanel.m_ChildrenList.size(); i++)
	{		
		try
		{

			if(((Component*)(thePanel.m_ChildrenList[i]))->m_type == NodeType_NeededComponent)
			{
				vAllComponent.push_back((Component*)(thePanel.m_ChildrenList[i]));
			}
			else if(((Component*)(thePanel.m_ChildrenList[i]))->m_type == NodeType_CombinePanel)
			{
				FindAllComponentInPanel(*(static_cast<Component*>(thePanel.m_ChildrenList[i])), vAllComponent);
			}

		}
		catch(CException* e)
		{
			int j = 0;
		}

	}
}


void OutlinePointsClockwiseRotate90(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vOutlinePoint.size(); i++)
	{
		PointInfo& theOutlinePoint = vOutlinePoint[i];
		float fOrgX = theOutlinePoint.x;
		float fOrgY = theOutlinePoint.y;
		theOutlinePoint.x = fOrgY;
		theOutlinePoint.y = fOrgXLen - fOrgX;
	}

}

void HoleClockwiseRotate90(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vHole.size(); i++)
	{
		AllVec& theHole = vHole[i];
		float fOrgX = theHole._x;
		float fOrgY = theHole._y;
		theHole._x = fOrgY;
		theHole._y = fOrgXLen - fOrgX;
	}
}

void SlotClockwiseRotate90(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vSlot.size(); i++)
	{
		AllSlot& theSlot = vSlot[i];
		float fOrgX = theSlot._x;
		float fOrgY = theSlot._y;
		theSlot._x = fOrgY;
		theSlot._y = fOrgXLen - fOrgX;
		theSlot._dir = (theSlot._dir == 0) ? 1 : 0;
	}
}



void BandingClockwiseRotate90(float (&afBanding)[4])
{
	float afBandingRotated[4];
	afBandingRotated[0] = afBanding[1];
	afBandingRotated[1] = afBanding[2];
	afBandingRotated[2] = afBanding[3];
	afBandingRotated[3] = afBanding[0];
	memcpy(afBanding, afBandingRotated, sizeof(float)*4);
}

void LabelPosClockwiseRotate90(float& fXLabelCenter, float& fYLabelCenter, float fOrgXLen, float fOrgYLen)
{
	float fOrgX = fXLabelCenter;
	float fOrgY = fYLabelCenter;
	fXLabelCenter = fOrgY;
	fYLabelCenter = fOrgXLen - fOrgX;
}

/*static */bool Sort_ComponentByMachiningOrder(const PTR_COMPONENT & m1, const PTR_COMPONENT & m2)
{
	return (m1->m_NumberInPanel < m2->m_NumberInPanel);
}

int GetRegularAngle(int nAngle)
{
	nAngle = nAngle % 360;
	return nAngle;
}

void ComponentRotateClockwise90(Component* pComponent)
{
	Component& theComponent = *pComponent;
	theComponent.m_nRotatedAngle = GetRegularAngle(theComponent.m_nRotatedAngle - 90);
	//if(bRotateTarget)
	{
		OutlinePointsClockwiseRotate90(theComponent.m_vOutlinePoint, theComponent.m_RealLength, theComponent.m_RealWidth);
		HoleClockwiseRotate90(theComponent.m_vUpperFaceHole, theComponent.m_RealLength, theComponent.m_RealWidth);
		SlotClockwiseRotate90(theComponent.m_vUpperFaceSlot, theComponent.m_RealLength, theComponent.m_RealWidth);
		HoleClockwiseRotate90(theComponent.m_vDownerFaceHole, theComponent.m_RealLength, theComponent.m_RealWidth);
		SlotClockwiseRotate90(theComponent.m_vDownerFaceSlot, theComponent.m_RealLength, theComponent.m_RealWidth);
		BandingClockwiseRotate90(theComponent.m_afBanding);
		LabelPosClockwiseRotate90(theComponent.m_fXLabelCenter, theComponent.m_fYLabelCenter, theComponent.m_RealLength, theComponent.m_RealWidth);

		float fOrgProductXLen = theComponent.m_fProductLength;
		float fOrgProductYLen = theComponent.m_fProductWidth;
		theComponent.m_fProductLength = fOrgProductYLen;
		theComponent.m_fProductWidth = fOrgProductXLen;
		if (theComponent.m_Texture == 1)
			theComponent.m_Texture = 2;
		else if(theComponent.m_Texture == 2)
			theComponent.m_Texture = 1;
	}

	float fTmp = theComponent.m_RealLength;
	theComponent.m_RealLength = theComponent.m_RealWidth;
	theComponent.m_RealWidth = fTmp;
}
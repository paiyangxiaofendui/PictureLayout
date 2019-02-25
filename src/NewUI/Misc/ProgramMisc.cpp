#include "stdafx.h"
#include "ProgramMisc.h"
#include "Misc.h"
#include "JT2_Tools.h"

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"



#include <algorithm>


#include "../UIData/UIData.h"
#include "YHInfoTools.h"

#include "../Work/ProgramDef.h"

#define PI 3.14159

void FindAllComponentInPanel(Component& thePanel, vector<Component*>& vAllComponent, NodeType eNodeType)
{
	for(int i = 0; i < thePanel.m_ChildrenList.size(); i++)
	{		
		try
		{

			if(((Component*)(thePanel.m_ChildrenList[i]))->m_type == /*NodeType_NeededComponent*/eNodeType)
			{
				vAllComponent.push_back((Component*)(thePanel.m_ChildrenList[i]));
			}
			else if(((Component*)(thePanel.m_ChildrenList[i]))->m_type == NodeType_CombinePanel)
			{
				FindAllComponentInPanel(*(static_cast<Component*>(thePanel.m_ChildrenList[i])), vAllComponent, eNodeType);
			}

		}
		catch(CException* e)
		{
			int j = 0;
		}
	}
}

int GetComponentCountInPanel(Component& thePanel)
{
	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);
	return vAllComponent.size();
}

void GetSolutionPanel(vector<PanelAndTheSolution>& vPanelAndTheSolution)
{
	vPanelAndTheSolution.clear();
	for(int i = 0; i < CSingleton::GetSingleton()->m_BackupSolutionList.size(); i++)
	{
		CSolution* pSolution = CSingleton::GetSingleton()->m_BackupSolutionList[i];
		for(int j = 0; j < pSolution->m_PanelList.size(); j++)
		{
			PanelAndTheSolution panleAndSol;
			panleAndSol.m_pPanel = pSolution->m_PanelList[j];
			panleAndSol.m_pSolution = pSolution;
			vPanelAndTheSolution.push_back(panleAndSol);
		}
	}
}

float GetSolutionComponentToatlArea(CSolution& theSolution)
{
	float fRet = 0.0;
	for(int i = 0; i < theSolution.m_PanelList.size(); i++)
	{
		Panel* thePanel = theSolution.m_PanelList[i];
		vector<Component*> vAllComponent;
		FindAllComponentInPanel(*((Component*)thePanel), vAllComponent);
		for(int j = 0; j < vAllComponent.size(); j++)
		{
			Component& theComponent = *(vAllComponent[j]);
			float fComponentArea = (theComponent.m_RealLength * theComponent.m_RealWidth) / (1000.0 * 1000.0);
			fRet += fComponentArea;
		}
	}
	return fRet;
}

float GetSolutionRemainderToatlArea(CSolution& theSolution)
{
	float fRet = 0.0;
	for(int i = 0; i < theSolution.m_PanelList.size(); i++)
	{
		Panel* thePanel = theSolution.m_PanelList[i];
		vector<Component*> vAllRemainder;
		FindAllComponentInPanel(*((Component*)thePanel), vAllRemainder, NodeType_Remainder);
		for(int j = 0; j < vAllRemainder.size(); j++)
		{
			Component& theRemainder = *(vAllRemainder[j]);
			float fRemainderArea = (theRemainder.m_RealLength * theRemainder.m_RealWidth) / (1000.0 * 1000.0);
			fRet += fRemainderArea;
		}
	}
	return fRet;
}

float GetSolutionPanelTotalArea(CSolution& theSolution)
{
	float fRet = 0.0;
	for(int i = 0; i < theSolution.m_PanelList.size(); i++)
	{
		Panel* thePanel = theSolution.m_PanelList[i];

		float fPanelArea = (thePanel->m_OrgLen * thePanel->m_OrgWidth) / (1000.0 * 1000.0);
		fRet += fPanelArea;

	}
	return fRet;
}

//得到贴标的宽高
void GetTiebiaoSize( float& tiebiao_width, float& tiebiao_height )
{

}

LineType GetLineType(int nLineSign)
{
	LineType eLineType = UNKNOWN;
	if(nLineSign == 0 || nLineSign == 2)
		eLineType = STRAIGHT_LINE;
	else if(nLineSign == 1 || nLineSign == 3)
		eLineType = CIRCULAR_ARC;
	return eLineType;
}

void FindAllComponentInSingleton(vector<Component*>& vAllComponent)
{
	vAllComponent.clear();

	int nSolutionCount = CSingleton::GetSingleton()->GetBackupSolutionNum();

	for(int i = 0; i < nSolutionCount; i++)
	{
		CSolution* pSln =  CSingleton::GetSingleton()->m_BackupSolutionList.at(i);

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			vector<Component*> vComponentInPanel;
			FindAllComponentInPanel(*((Component*)pPanel), vComponentInPanel);

			vAllComponent.insert(vAllComponent.begin(), vComponentInPanel.begin(), vComponentInPanel.end());
		}
	}
}

void FindAllPanelInSingleton(vector<Panel*>& vAllPanel)
{
	vAllPanel.clear();

	int nSolutionCount = CSingleton::GetSingleton()->GetBackupSolutionNum();

	for(int i = 0; i < nSolutionCount; i++)
	{
		CSolution* pSln =  CSingleton::GetSingleton()->m_BackupSolutionList.at(i);

		int nPanelNum = pSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pPanel = pSln->GetPanel(i_panel);
			vAllPanel.push_back(pPanel);
		}
	}
}

void CollectAllMachiningInfoFromComponents(vector<Component*>& vAllComponent, vector<AllVec>& vUpperFaceHole, vector<AllVec>& vDownerFaceHole, vector<AllSlot>& vUpperFaceSlot, vector<AllSlot>& vDownerFaceSlot)
{
	vUpperFaceHole.clear();
	vDownerFaceHole.clear();
	vUpperFaceSlot.clear();
	vUpperFaceSlot.clear();

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		vUpperFaceHole.insert(vUpperFaceHole.begin(), theComponent.m_vUpperFaceHole.begin(), theComponent.m_vUpperFaceHole.end());
		vDownerFaceHole.insert(vDownerFaceHole.begin(), theComponent.m_vDownerFaceHole.begin(), theComponent.m_vDownerFaceHole.end());
		vUpperFaceSlot.insert(vUpperFaceSlot.begin(), theComponent.m_vUpperFaceSlot.begin(), theComponent.m_vUpperFaceSlot.end());
		vDownerFaceSlot.insert(vDownerFaceSlot.begin(), theComponent.m_vDownerFaceSlot.begin(), theComponent.m_vDownerFaceSlot.end());
	}
}

void CollectAllMachiningInfoInSingleton(vector<AllVec>& vUpperFaceHole, vector<AllVec>& vDownerFaceHole, vector<AllSlot>& vUpperFaceSlot, vector<AllSlot>& vDownerFaceSlot)
{
	vector<Component*> vAllComponent;
	FindAllComponentInSingleton(vAllComponent);
	CollectAllMachiningInfoFromComponents(vAllComponent, vUpperFaceHole, vDownerFaceHole, vUpperFaceSlot, vDownerFaceSlot);
}

TextureDirectionType GetComponentRotetedTextureDirType(TextureDirectionType eOrgInputTextureDir, int nRotatedAngle)
{
	//TextureDirectionType eRet = TEXTRUE_H_DIR;
	//bool bRotated90 = ((nRotatedAngle / 90) % 2) == 0 ? false : true;
	//switch(eOrgInputTextureDir)
	//{
	//case TEXTURE_NO_DIR:
	//	eRet = TEXTURE_NO_DIR;
	//	break;
	//case TEXTRUE_H_DIR:
	//	if(bRotated90)
	//		eRet = TEXTURE_V_DIR;
	//	else
	//		eRet = TEXTRUE_H_DIR;
	//	break;
	//case TEXTURE_V_DIR:
	//	if(bRotated90)
	//		eRet = TEXTRUE_H_DIR;
	//	else
	//		eRet = TEXTURE_V_DIR;
	//	break;
	//default:
	//	break;
	//}

	//return eRet;
	return eOrgInputTextureDir;
}

int GetOutterOutlineVertex(vector<PointInfo>& vOutlinePoint, vector<PointInfo>& vOutterOutlinePoint)
{
	//vOutterOutlinePoint.clear();
	//int nRet = 0;
	//for(int i = 0; i < vOutlinePoint.size(); i++)
	//{
	//	if(vOutlinePoint[i].side == 1)
	//	{
	//		nRet++;
	//		vOutterOutlinePoint.push_back(vOutlinePoint[i]);
	//	}
	//}
	//nRet--;
	//return nRet;

	vOutterOutlinePoint.clear();
	int nRet = 0;
	int nStart = 0;
	for(int i = vOutlinePoint.size()-1; i > -1; i--)
	{
		if(vOutlinePoint[i].group == 1)
		{
			nStart = i;
			break;
		}
	}
	vector<PointInfo>::iterator itrStart = vOutlinePoint.begin();
	advance(itrStart, nStart);
	vOutterOutlinePoint.insert(vOutterOutlinePoint.end(), itrStart, vOutlinePoint.end());
	nRet = vOutlinePoint.size() - nStart;
	return nRet;

}

void GetOutlineGroups(vector<PointInfo>& vOutlinePoint, vector<vector<PointInfo>>& vOutlinePointGroup)
{
	vOutlinePointGroup.clear();
	vector<PointInfo> vOneOutlinePoint;
	for(int i = 0; i < vOutlinePoint.size(); i++)
	{
		vOneOutlinePoint.push_back(vOutlinePoint[i]);
		if(vOutlinePoint[i].group == 2)
		{
			vOutlinePointGroup.push_back(vOneOutlinePoint);
			vOneOutlinePoint.clear();
		}
	}
	if(vOneOutlinePoint.size() != 0)
		vOutlinePointGroup.push_back(vOneOutlinePoint);
}

int GetKnifeDownPosByClickedPos(vector<PointInfo>& vOutlinePoint, float fXClickedInComponent, float fYClickedInComponent)
{
	float fCurMinDistance = FLT_MAX;
	int nCurMinDistanceLineID = -1;
	int nOutlineLineCount =  vOutlinePoint.size() - 1;
	for(int i = 0; i < nOutlineLineCount; i++)
	{
		float fDistance;



		float fXLineStart = vOutlinePoint[i].x;
		float fYLineStart = vOutlinePoint[i].y;
		float fXLineEnd = vOutlinePoint[i+1].x;
		float fYLineEnd = vOutlinePoint[i+1].y;

		fDistance = GetPointAndLineSegmentDistance(fXClickedInComponent, fYClickedInComponent, fXLineStart, fYLineStart, fXLineEnd, fYLineEnd);

		if(fDistance < fCurMinDistance)
		{
			nCurMinDistanceLineID = i;
			fCurMinDistance = fDistance;
		}

	}
	return nCurMinDistanceLineID;
}

void ResetMachiningOrderByMap(Panel*pThePanel, map<int, int> mapResettedOrder)
{
	vector<Component*> vAllComponent;
	FindAllComponentInPanel(*pThePanel, vAllComponent);
	for(int i = 0; i < vAllComponent.size(); i++)
	{
		vAllComponent[i]->m_NumberInPanel = mapResettedOrder[vAllComponent[i]->m_NumberInPanel];
	}
}

Component* GetClickedComponent(CPoint point, CRect rcDrawArea, int nGap, Panel* pPanel)
{
	Component* pComponent = NULL;

	Panel& thePanel = *(pPanel);
	int nPanelLen = thePanel.m_OrgLen;
	int nPanelWidth =thePanel.m_OrgWidth;

	float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-nGap, rcDrawArea.Height()-nGap);
	CRect rcPanelArea(nGap/2, nGap/2, rcDrawArea.Width()-nGap/2, rcDrawArea.Height()-nGap/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);


	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		Region regionComponent;
		GetComponentRegion(theComponent.m_vOutlinePoint, rcComponent, fScale, regionComponent);
		Point pt;
		pt.X = point.x;
		pt.Y = point.y;
		//if(rcComponent.PtInRect(point))
		if(regionComponent.IsVisible(pt))
		{
			pComponent = vAllComponent[i];
			break;
		}
	}

	return pComponent;
}

void RemoveComponentFromPanel(Component* pComponent, Panel* pPanel)
{
	SetComponentToRemainder(pComponent);
	ReArrangeComponentMachiningOrder(pPanel);
}

void SetComponentToRemainder(Component* pComponent)
{
	pComponent->m_type = NodeType_Remainder;
	pComponent->m_CpnID = -1;					
	pComponent->m_NumberInPanel = -1;		
	pComponent->m_BarCode = _T("");			
	pComponent->m_strCabinetName = _T("");	
	pComponent->m_strComponentName = _T("");
	pComponent->m_Material = _T("");
	pComponent->m_Texture = 0;	

	//pComponent->m_bRotated = false;
	pComponent->m_nRotatedAngle = 0;
	pComponent->m_nOtherShapeID = 0;
	pComponent->m_vOutlinePoint.clear();
	pComponent->m_nKnifeDownPos = 0;  
	pComponent->m_vUpperFaceHole.clear();
	pComponent->m_vUpperFaceSlot.clear();
	pComponent->m_vDownerFaceHole.clear(); 
	pComponent->m_vDownerFaceSlot.clear();
}

bool IsComponentOverlap(Panel* pThePanel, Component* pComponent, PointFloat ptInPanel, float fKerf)
{
	bool bRet = false;

	RectF rfInflatedComponent;
	vector<RectF> vExistComponentInflatedRect;

	float fPanelYLen = pThePanel->m_OrgWidth;
	rfInflatedComponent.X = ptInPanel.x - fKerf/2;
	rfInflatedComponent.Y = fPanelYLen - (ptInPanel.y + fKerf/2);
	rfInflatedComponent.Width = pComponent->m_RealLength + fKerf;
	rfInflatedComponent.Height = pComponent->m_RealWidth + fKerf;

	vector<Component*> vAllExistComponent;
	FindAllComponentInPanel(*pThePanel, vAllExistComponent);
	for(int i = 0; i < vAllExistComponent.size(); i++)
	{
		Component* pTheExistComponent = vAllExistComponent[i];

		RectF rfExistComponentInflatedRect;
		rfExistComponentInflatedRect.X = pTheExistComponent->m_x-fKerf/2;
		rfExistComponentInflatedRect.Y = fPanelYLen - (pTheExistComponent->m_y+pTheExistComponent->m_RealWidth+fKerf/2);
		rfExistComponentInflatedRect.Width = pTheExistComponent->m_RealLength + fKerf;
		rfExistComponentInflatedRect.Height = pTheExistComponent->m_RealWidth + fKerf;
		vExistComponentInflatedRect.push_back(rfExistComponentInflatedRect);
	}

	for(int i = 0; i < vExistComponentInflatedRect.size(); i++)
	{
		RectF rfInterset;
		if(RectF::Intersect(rfInterset, rfInflatedComponent, vExistComponentInflatedRect[i]))
		{
			//CString strTmp;
			//strTmp.Format(_T("Component RectF: Left=%s Top=%s Right=%s Bottom=%s\n"), \
			//	GetFloatString(rfInflatedComponent.X, 1), \
			//	GetFloatString(rfInflatedComponent.Y, 1), \
			//	GetFloatString(rfInflatedComponent.GetRight(), 1), \
			//	GetFloatString(rfInflatedComponent.GetBottom(), 1));
			//OutputDebugString(strTmp);

			//strTmp.Format(_T("Overlap:Other Component RectF: Left=%s Top=%s Right=%s Bottom=%s\n"), \
			//	GetFloatString(vExistComponentInflatedRect[i].X, 1), \
			//	GetFloatString(vExistComponentInflatedRect[i].Y, 1), \
			//	GetFloatString(vExistComponentInflatedRect[i].GetRight(), 1), \
			//	GetFloatString(vExistComponentInflatedRect[i].GetBottom(), 1));
			//OutputDebugString(strTmp);

			bRet = true;
			break;
		}
	}

	return bRet;
}

bool IsComponentInPanelClipped(Panel* pThePanel, Component* pComponent, PointFloat ptInPanel, float fDeburringWidth)
{
	bool bRet = false;

	RectF rfComponent;
	RectF rfClippedPanel;

	float fPanelYLen = pThePanel->m_OrgWidth;

	rfComponent.X = ptInPanel.x;
	rfComponent.Y =	fPanelYLen - (ptInPanel.y);
	rfComponent.Width =	pComponent->m_RealLength;
	rfComponent.Height = pComponent->m_RealWidth;

	rfClippedPanel.X = fDeburringWidth;
	rfClippedPanel.Y = fDeburringWidth;
	rfClippedPanel.Width = pThePanel->m_OrgLen - fDeburringWidth*2;
	rfClippedPanel.Height =	pThePanel->m_OrgWidth - fDeburringWidth*2;

	RectF rfInterset;
	if(RectF::Intersect(rfInterset, rfComponent, rfClippedPanel))
	{
		if(IsFloatEqual(rfInterset.X, rfComponent.X) \
			&& IsFloatEqual(rfInterset.Y, rfComponent.Y) \
			&& IsFloatEqual(rfInterset.Width, rfComponent.Width) \
			&& IsFloatEqual(rfInterset.Height, rfComponent.Height))
			bRet = true;
	}

	return bRet;
}

bool InsertComponentToPanelRoot(Panel* pPanel, Component* pComponent, PointFloat ptComponentLeftBottom)
{
	pComponent->m_x = ptComponentLeftBottom.x;
	pComponent->m_y = ptComponentLeftBottom.y;
	pComponent->m_NumberInPanel = INT_MAX;

	pPanel->AddChild(pComponent);
	pPanel->m_type = NodeType_CombinePanel;
	ReArrangeComponentMachiningOrder(pPanel);

	return true;
}

typedef Component* PTR_COMPONENT;

static bool Sort_ComponentByMachiningOrder(const PTR_COMPONENT & m1, const PTR_COMPONENT & m2)
{
	return (m1->m_NumberInPanel < m2->m_NumberInPanel);
}

void ReArrangeComponentMachiningOrder(Panel* pPanel)
{
	vector<Component*> vComponent;
	FindAllComponentInPanel(*pPanel, vComponent);
	std::sort(vComponent.begin(),vComponent.end(), Sort_ComponentByMachiningOrder);
	for(int i = 0; i < vComponent.size(); i++)
	{
		vComponent[i]->m_NumberInPanel = i + 1;
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

void OutlinePointsAntiClockwiseRotate90(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vOutlinePoint.size(); i++)
	{
		PointInfo& theOutlinePoint = vOutlinePoint[i];
		float fOrgX = theOutlinePoint.x;
		float fOrgY = theOutlinePoint.y;
		theOutlinePoint.x = fOrgYLen - fOrgY;
		theOutlinePoint.y = fOrgX;
	}
}

void HoleAntiClockwiseRotate90(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vHole.size(); i++)
	{
		AllVec& theHole = vHole[i];
		float fOrgX = theHole._x;
		float fOrgY = theHole._y;
		theHole._x = fOrgYLen - fOrgY;
		theHole._y = fOrgX;
	}
}

void SlotAntiClockwiseRotate90(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vSlot.size(); i++)
	{
		AllSlot& theSlot = vSlot[i];
		float fOrgX = theSlot._x;
		float fOrgY = theSlot._y;
		theSlot._x = fOrgYLen - fOrgY;
		theSlot._y = fOrgX;
		theSlot._dir = (theSlot._dir == 0) ? 1 : 0;
	}
}

void BandingAntiClockwiseRotate90(float (&afBanding)[4])
{
	float afBandingRotated[4];
	afBandingRotated[0] = afBanding[3];
	afBandingRotated[1] = afBanding[0];
	afBandingRotated[2] = afBanding[1];
	afBandingRotated[3] = afBanding[2];
	memcpy(afBanding, afBandingRotated, sizeof(float)*4);
}

void LabelPosAntiClockwiseRotate90(float& fXLabelCenter, float& fYLabelCenter, float fOrgXLen, float fOrgYLen)
{
	float fOrgX = fXLabelCenter;
	float fOrgY = fYLabelCenter;
	fXLabelCenter = fOrgYLen - fOrgY;
	fYLabelCenter = fOrgX;
}

void OutlinePointsFlipH(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen)
{
	for(int i = 0; i < vOutlinePoint.size(); i++)
	{
		PointInfo& theOutlinePoint = vOutlinePoint[i];
		float fOrgX = theOutlinePoint.x;
		float fOrgY = theOutlinePoint.y;
		theOutlinePoint.x = fOrgXLen - fOrgX;
		theOutlinePoint.y = fOrgY;
		switch(theOutlinePoint.dir)
		{
		case ANTI_CLOCKWISE:
			theOutlinePoint.dir = CLOCKWISE;
			break;
		case CLOCKWISE:
			theOutlinePoint.dir = ANTI_CLOCKWISE;
			break;
		}
	}
}

//void HoleFlipH(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen)
//{
//	for(int i = 0; i < vHole.size(); i++)
//	{
//		AllVec& theHole = vHole[i];
//		float fOrgX = theHole._x;
//		float fOrgY = theHole._y;
//		theHole._x = fOrgXLen - fOrgX;
//		theHole._y = fOrgY;
//	}
//}
//
//void SlotFlipH(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen)
//{
//	for(int i = 0; i < vSlot.size(); i++)
//	{
//		AllSlot& theSlot = vSlot[i];
//		float fOrgX = theSlot._x;
//		float fOrgY = theSlot._y;
//		theSlot._x = fOrgXLen - fOrgX;
//		theSlot._y = fOrgY;
//	}
//}

void ComponentRotateClockwise90(Component* pComponent)
{
	Component& theComponent = *pComponent;
	//bool bRotateTarget = !theComponent.m_bRotated;
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
	//else
	//{
	//	OutlinePointsAntiClockwiseRotate90(theComponent.m_vOutlinePoint, theComponent.m_RealLength, theComponent.m_RealWidth);
	//	HoleAntiClockwiseRotate90(theComponent.m_vUpperFaceHole, theComponent.m_RealLength, theComponent.m_RealWidth);
	//	SlotAntiClockwiseRotate90(theComponent.m_vUpperFaceSlot, theComponent.m_RealLength, theComponent.m_RealWidth);
	//	HoleAntiClockwiseRotate90(theComponent.m_vDownerFaceHole, theComponent.m_RealLength, theComponent.m_RealWidth);
	//	SlotAntiClockwiseRotate90(theComponent.m_vDownerFaceSlot, theComponent.m_RealLength, theComponent.m_RealWidth);
	//	BandingAntiClockwiseRotate90(theComponent.m_afBanding);
	//	LabelPosAntiClockwiseRotate90(theComponent.m_fXLabelCenter, theComponent.m_fYLabelCenter, theComponent.m_RealLength, theComponent.m_RealWidth);
	//}

	float fTmp = theComponent.m_RealLength;
	theComponent.m_RealLength = theComponent.m_RealWidth;
	theComponent.m_RealWidth = fTmp;
	
	//pComponent->m_bRotated = bRotateTarget;
}

void FlipComponentH(Component* pComponent)
{
	Component& theComponent = *pComponent;
	
	vector<AllVec> tmpHole = theComponent.m_vDownerFaceHole;
	theComponent.m_vDownerFaceHole = theComponent.m_vUpperFaceHole;
	theComponent.m_vUpperFaceHole = tmpHole;
	for(int i = 0; i < theComponent.m_vUpperFaceHole.size(); i++)
		theComponent.m_vUpperFaceHole[i]._x = theComponent.m_RealLength - theComponent.m_vUpperFaceHole[i]._x;
	for(int i = 0; i < theComponent.m_vDownerFaceHole.size(); i++)
		theComponent.m_vDownerFaceHole[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceHole[i]._x;

	vector<AllSlot> tmpSlot = theComponent.m_vDownerFaceSlot;
	theComponent.m_vDownerFaceSlot = theComponent.m_vUpperFaceSlot;
	theComponent.m_vUpperFaceSlot = tmpSlot;
	for(int i = 0; i < theComponent.m_vUpperFaceSlot.size(); i++)
		theComponent.m_vUpperFaceSlot[i]._x = theComponent.m_RealLength - theComponent.m_vUpperFaceSlot[i]._x;
	for(int i = 0; i < theComponent.m_vDownerFaceSlot.size(); i++)
		theComponent.m_vDownerFaceSlot[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceSlot[i]._x;

	OutlinePointsFlipH(theComponent.m_vOutlinePoint, theComponent.m_RealLength, theComponent.m_RealWidth);
}

CString GetTextureString(TextureDirectionType eTextureType)
{
	CString strRet;
	switch(eTextureType)
	{
	case TEXTURE_NO_DIR:
		strRet = _T("无纹理");
		break;
	case TEXTRUE_H_DIR:
		strRet = _T("横纹");
		break;
	case TEXTURE_V_DIR:
		strRet = _T("竖纹");
		break;
	}

	return strRet;
}

//获取PrintInfo合并成字符
std::string getPrintInfoAsString(ComponentInputItem& theItem)
{
// 	ParamSettingDlg settings;
// 	settings.loadSettings();
// 	TiebiaoEdgeDlg tiebiao;
// 	tiebiao.AllEdgeString(settings.Tiebiao_panel_edge());
// 
// 	CString strUpperFaceBarcode = theItem.m_strBarcode;
// 	if(strUpperFaceBarcode.CompareNoCase(theItem.m_strReverseSideBarcode) == 0)
// 		strUpperFaceBarcode = "";
// 
// 
// 	CString result;
// 
// 	result = result + theItem.m_strOrderID + "," ;														//订单号
// 	result = result + theItem.m_strCabinetID + "," ;													//柜号
// 	result = result + /*theItem.m_strBarcode*/strUpperFaceBarcode + "," ;														//条码
// 	result = result + theItem.m_strCabinetName + "," ;													//产品名称
// 	result = result + theItem.m_strPanelName + "," ;													//成品名称
// 	result = result + theItem.m_strMaterial + "," ;														//材质
// 	result = result + GetFloatStringTrimming(theItem.m_fLength, 1) + "," ;								//长
// 	result = result + GetFloatStringTrimming(theItem.m_fWidth, 1) + "," ;								//宽
// 	result = result + GetFloatStringTrimming(theItem.m_fThickness, 1) + "," ;							//厚度
// 	result = result + GetFloatString(theItem.m_nCount, 0) + "," ;										//切割数量
// 	result = result + theItem.m_strTexture + "," ;														//纹理方向
// 	result = result + tiebiao.findCharByEdge(theItem.m_afBanding[BANDING_ID_FRONT]).c_str() + "," ;		//封长1
// 	result = result + tiebiao.findCharByEdge(theItem.m_afBanding[BANDING_ID_RIGHT]).c_str() + "," ;		//封宽1
// 	result = result + tiebiao.findCharByEdge(theItem.m_afBanding[BANDING_ID_BACK]).c_str() + "," ;		//封长2
// 	result = result + tiebiao.findCharByEdge(theItem.m_afBanding[BANDING_ID_LEFT]).c_str() + "," ;		//封宽2
// 	//result = result + noProcessSmallPanelOne["pointInfo"] + "," ;										//异形信息
// 	result = result + GetFloatString(theItem.m_eRotateType, 0) + "," ;									//是否旋转
// 	//result = result + noProcessSmallPanelOne["slotInfo"] + "," ;										//槽
// 	result = result + (theItem.m_bSlotFlipped ? "1" : "0") + "," ;										//槽翻转
// 	//result = result + noProcessSmallPanelOne["dslotInfo"] + "," ;										//反面槽
// 	//result = result + noProcessSmallPanelOne["vecInfo"] + "," ;										//孔
// 	result = result + (theItem.m_bVHoleFlipped ? "1" : "0") + "," ;										//孔翻转
// 	//result = result + noProcessSmallPanelOne["dvecInfo"] + "," ;										//反面孔
// 	result = result + theItem.m_strCustomerInfo + "," ;													//客户信息
// 	result = result + theItem.m_strJoinedStore + "," ;													//加盟店
// 	result = result + theItem.m_strSlottingFlag + "," ;													//拉槽标识
// 	result = result + theItem.m_strReminder + "," ;														//助记号
// 	result = result + theItem.m_strDrilling + "," ;														//钻孔
// 	result = result + theItem.m_strOrderType + "," ;													//订单类型
// 	result = result + theItem.m_strReverseSideBarcode + "," ;											//反面条码
// 	result = result + GetFloatStringTrimming(theItem.m_fProductLength, 1) + "," ;						//成品长度
// 	result = result + GetFloatStringTrimming(theItem.m_fProductWidth, 1) + "," ;						//成品宽度
// 	result = result + GetFloatStringTrimming(theItem.m_fProductThickness, 1) + "," ;					//成品厚度
// 	result = result + theItem.m_strOtherShapeID + "," ;													//异型ID
// 	result = result + theItem.m_strCustomerAddress + "," ;												//客户地址
// 	result = result + theItem.m_strHoleSlotFlag + "," ;													//钻槽标识
// 	//result = result + noProcessSmallPanelOne["OtherPoints"] + "," ;									//异形点信息
// 
// 	std::string stringRet = result;
// 	return stringRet;

	return "";
}

float GetKerf()
{
	CSingleton* pSingleton = CSingleton::GetSingleton();
	return pSingleton->m_BaseInfo.m_SawKerfWidth;
}

void DrawLogoTextInLabel(CDC* pDC, CRect rcLabelArea)
{

}

int GetLabelSide(LPCSTR szMachineName)
{
	int bRet = 0;


	return bRet;
}

CString GetLabelImageName(int nPanelID, int nComponentID)
{
	CString strRet;
	strRet.Format("%06d-%d", nPanelID+1, nComponentID);
	strRet = strRet.Right(8);
	strRet += _T(".bmp");
	return strRet;
}

CString GetPanelImageName(int nPanelID)
{
	CString strRet;
	strRet.Format("A%03d.bmp", nPanelID+1);
	return strRet;
}
//获取预组合项的整合矩形
void GetPreCombineItemRect(PreCombineItem* pItem, float& fLeft, float& fTop, float& fRight, float& fBottom)
{
	float fLeftMost = FLT_MAX, fRightMost = -FLT_MAX, fTopMost = -FLT_MAX, fBottomMost = FLT_MAX;
	for(int i = 0; i < pItem->size(); i++)
	{
		PreCombineComponent& thePreCombineComponent = (*pItem)[i];

		float fXLen = (thePreCombineComponent.m_nRotatedAngleWhenCombine / 90) % 2 == 0 ? thePreCombineComponent.m_Item.m_fLength : thePreCombineComponent.m_Item.m_fWidth;
		float fYLen = (thePreCombineComponent.m_nRotatedAngleWhenCombine / 90) % 2 == 0 ? thePreCombineComponent.m_Item.m_fWidth : thePreCombineComponent.m_Item.m_fLength;

		if(thePreCombineComponent.m_fX_Left < fLeftMost)
			fLeftMost = thePreCombineComponent.m_fX_Left;
		if(thePreCombineComponent.m_fY_Bottom < fBottomMost)
			fBottomMost = thePreCombineComponent.m_fY_Bottom;
		if((thePreCombineComponent.m_fY_Bottom + fYLen) > fTopMost)
			fTopMost = thePreCombineComponent.m_fY_Bottom + fYLen;
		if((thePreCombineComponent.m_fX_Left + fXLen) > fRightMost)
			fRightMost = thePreCombineComponent.m_fX_Left + fXLen;
	}
	fLeft = fLeftMost;
	fTop = fTopMost;
	fRight = fRightMost;
	fBottom = fBottomMost;
}

bool IsVHoleDeeperThanThickness(ComponentInputItem& theItem)
{
	bool bRet = false;

	vector<AllVec> vHole;
	YHInfoTools::getAllVec(vHole, theItem.m_strUpperFaceHoleInfo, _T(""));

	for(int i = 0; i < vHole.size(); i++)
	{
		if(vHole[i]._depth - theItem.m_fThickness > 0.3)
		{
			bRet = true;
			break;
		}
	}

	if(bRet == false)
	{
		vHole.clear();
		YHInfoTools::getAllVec(vHole, theItem.m_strDownerFaceHoleInfo, _T(""));

		for(int i = 0; i < vHole.size(); i++)
		{
			if(vHole[i]._depth - theItem.m_fThickness > 0.3)
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

bool IsVHoleOutOfPanel(ComponentInputItem& theItem)
{
	bool bRet = false;

	vector<AllVec> vHole;
	YHInfoTools::getAllVec(vHole, theItem.m_strUpperFaceHoleInfo, _T(""));

	for(int i = 0; i < vHole.size(); i++)
	{
		if(vHole[i]._x < 0 || vHole[i]._x > theItem.m_fLength || vHole[i]._y < 0 || vHole[i]._y > theItem.m_fWidth)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool IsSlotDeeperThanThickness(ComponentInputItem& theItem)
{
	bool bRet = false;

	vector<AllSlot> vSlot;
	YHInfoTools::getAllSlot(vSlot, theItem.m_strUpperFaceSlotInfo, _T(""), _T(""));

	for(int i = 0; i < vSlot.size(); i++)
	{
		if(vSlot[i]._depth - theItem.m_fThickness > 0.3)
		{
			bRet = true;
			break;
		}
	}

	if(bRet == false)
	{
		vSlot.clear();
		YHInfoTools::getAllSlot(vSlot, theItem.m_strDownerFaceSlotInfo, _T(""), _T(""));

		for(int i = 0; i < vSlot.size(); i++)
		{
			if(vSlot[i]._depth - theItem.m_fThickness > 0.3)
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

bool IsComponentOverSize(ComponentInputItem& theItem)
{
	bool bRet = false;

	CSingleton* pSingleton = CSingleton::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;
	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	float fDeburredPanelXLen = base_info.m_PanelLength - 2*base_info.m_DeburringWidth;
	float fDeburredPanelYLen = base_info.m_PanelWidth - 2*base_info.m_DeburringWidth;
	if(theItem.m_strTexture.CompareNoCase(_T("无纹理")) != 0)
	{
		float fComponentXLen, fComponentYLen;
		if(theItem.m_strTexture.CompareNoCase(_T("横纹")) == 0)
		{
			fComponentXLen = theItem.m_fLength;
			fComponentYLen = theItem.m_fWidth;
		}
		else
		{
			fComponentXLen = theItem.m_fWidth;
			fComponentYLen = theItem.m_fLength;
		}
		if (fComponentXLen > fDeburredPanelXLen || fComponentYLen > fDeburredPanelYLen)
			bRet = true;
	}
	else
	{
		float fComponentXLen, fComponentYLen;
		fComponentXLen = theItem.m_fLength;
		fComponentYLen = theItem.m_fWidth;
		if (fComponentXLen > fDeburredPanelXLen || fComponentYLen > fDeburredPanelYLen)
		{
			if (fComponentYLen > fDeburredPanelXLen || fComponentXLen > fDeburredPanelYLen)
				bRet = true;
		}
			
	}

	return bRet;
}

bool SortComponentByIDInPanel(const PCOMPONENT& first, const PCOMPONENT& second)
{
	return first->m_NumberInPanel < second->m_NumberInPanel;
}

void GetOutlinePath(vector<PointInfo>& vOutlinePoint, CRect rcComponent, float fScale, GraphicsPath& pathOutline)
{
	for(int j = 1; j < vOutlinePoint.size(); j++)
	{
		PointInfo& vertexStart = vOutlinePoint[j - 1];
		PointInfo& vertexEnd = vOutlinePoint[j];

		//DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd);

		if(GetLineType(vertexStart.sign) == STRAIGHT_LINE)
		{

			INT p1_x = rcComponent.left + vertexStart.x * fScale;
			INT p1_y = rcComponent.top + (rcComponent.Height() - vertexStart.y * fScale);
			INT p2_x = rcComponent.left + vertexEnd.x * fScale;
			INT p2_y = rcComponent.top + (rcComponent.Height() - vertexEnd.y * fScale);
			//g.DrawLine(&pen, p1_x, p1_y, p2_x, p2_y);
			pathOutline.AddLine(p1_x, p1_y, p2_x, p2_y);
		}
		else if(GetLineType(vertexStart.sign) == CIRCULAR_ARC)
		{
			//DrawArc(g, rcPanel, fScale, vertexStart, vertexEnd, pen);
			CString strTmp;
			//vertexStart.m_nDir = OutlineVertex::ANTI_CLOCKWISE;
			int eArcDir = vertexEnd.dir;
			float fXa = vertexStart.x, fYa = vertexStart.y, fXb = vertexEnd.x, fYb = vertexEnd.y, fR = /*vertexStart.r*/vertexEnd.r;
			//strTmp.Format("fXa = %f, fYa = %f, fXb = %f, fYb = %f, fR = %f\n", fXa, fYa, fXb, fYb, fR);
			//OutputDebugString(strTmp);
			float fDx = fXa - fXb;
			//strTmp.Format("fDx = %f\n", fDx);
			//OutputDebugString(strTmp);
			float fDy = fYa - fYb;
			//strTmp.Format("fDy = %f\n", fDy);
			//OutputDebugString(strTmp);

			float fCenterX1;
			float fCenterX2;
			float fCenterY1;
			float fCenterY2;
			if(IsFloatEqual(fDy, 0, 0.1))
			{
				fCenterX1 = (fXa + fXb) / 2;
				fCenterX2 = (fXa + fXb) / 2;

				fCenterY1 = fYa - sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
				fCenterY2 = fYa + sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
			}
			else
			{
				float fD = fDx / fDy;
				//strTmp.Format("fD = %f\n", fD);
				//OutputDebugString(strTmp);
				float fa = 1 + fD*fD;
				float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
				float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
				//strTmp.Format("fa = %f, fb = %f, fc = %f\n", fa, fb, fc);
				//OutputDebugString(strTmp);
				//strTmp.Format("fb*fb - 4*fa*fc = %f\n", fb*fb - 4*fa*fc);
				//OutputDebugString(strTmp);
				fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
				fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
				fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
				fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
			}
			//strTmp.Format("fCenterX1 = %f, fCenterY1 = %f\n", fCenterX1, fCenterY1);
			//OutputDebugString(strTmp);
			//strTmp.Format("fCenterX2 = %f, fCenterY2 = %f\n", fCenterX2, fCenterY2);
			//OutputDebugString(strTmp);

			//SolidBrush brush(Color::Red);
			//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX1*fScale), (REAL)(rcPanel.bottom -fCenterY1*fScale), (REAL)3, (REAL)3);
			//g.FillRectangle(&brush, (REAL)(rcPanel.left +fCenterX2*fScale), (REAL)(rcPanel.bottom -fCenterY2*fScale), (REAL)3, (REAL)3);

			//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXa*fScale), (REAL)(rcPanel.top +fYa*fScale), (REAL)3, (REAL)3);
			//g.FillRectangle(&brush, (REAL)(rcPanel.left +fXb*fScale), (REAL)(rcPanel.top +fYb*fScale), (REAL)3, (REAL)3);


			PointF vectorStart(fXa-fCenterX1, fYa-fCenterY1);
			PointF vectorEnd(fXb-fCenterX1, fYb-fCenterY1);
			float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
			float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
			float fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
			float fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);

			//strTmp.Format(L"fCos = %f, fSin = %f\n", fCos, fSin);
			//OutputDebugString(strTmp);
			//bool bAngleLessThan180 = true;
			//if(fSin < 0 || (IsFloatEqual(fSin, 0) && fCos < 0))
			//	bAngleLessThan180 = false;


			PointF ptTheCenter;
			if(((-fSin) >= 0 && /*vertexStart.dir*/eArcDir == ANTI_CLOCKWISE) || ((-fSin) < 0 && /*vertexStart.dir*/eArcDir == CLOCKWISE))
			{
				ptTheCenter.X = fCenterX2;
				ptTheCenter.Y = fCenterY2;
			}
			else
			{
				ptTheCenter.X = fCenterX1;
				ptTheCenter.Y = fCenterY1;
			}

			//SolidBrush brush(Color::Red);
			//g.FillRectangle(&brush, (REAL)(rcPanel.left +ptTheCenter.X*fScale), (REAL)(rcPanel.bottom -ptTheCenter.Y*fScale), (REAL)3, (REAL)3);

			RectF rc(rcComponent.left, rcComponent.top, 100, 100);
			rc.X = rcComponent.left + (ptTheCenter.X - /*vertexStart.r*/fR)*fScale;
			rc.Y = rcComponent.top + (rcComponent.Height() - (ptTheCenter.Y + /*vertexStart.r*/fR)*fScale);
			rc.Width = (/*vertexStart.r*/fR * 2)*fScale;
			rc.Height = (/*vertexStart.r*/fR * 2)*fScale;
			float fArc = acos(fCos);
			float fAng = fArc*360/(PI*2);

			PointF vectorDrawingStart;
			if(/*vertexStart.dir*/eArcDir == CLOCKWISE)
			{
				vectorDrawingStart.X = vertexStart.x - ptTheCenter.X;
				vectorDrawingStart.Y = vertexStart.y - ptTheCenter.Y;
			}
			else
			{
				vectorDrawingStart.X = vertexEnd.x - ptTheCenter.X;
				vectorDrawingStart.Y = vertexEnd.y - ptTheCenter.Y;
			}

			float fCosDrawingStart = vectorDrawingStart.X/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
			float fSinDrawingStart = vectorDrawingStart.Y/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
			float fAngDrawingStart = (acos(fCosDrawingStart))*360/(PI*2);
			if(fSinDrawingStart >= 0)
				fAngDrawingStart = 360 - fAngDrawingStart;

			//g.DrawArc(&pen, rc, fAngDrawingStart, fAng);
			pathOutline.AddArc(rc, fAngDrawingStart, fAng);

			//RectF rfVertex;
			//rfVertex.X = rcPanel.left + vertexStart.x*fScale-3;
			//rfVertex.Y = rcPanel.bottom - vertexStart.y*fScale-3;
			//rfVertex.Width = 6;
			//rfVertex.Height = 6;
			//strTmp.Format("rfVertex.X = %f, rfVertex.Y = %f\n", rfVertex.X, rfVertex.Y);
			//OutputDebugString(strTmp);
			//g.FillRectangle(&SolidBrush(Color::Blue), rfVertex);

			//rfVertex.X = rcPanel.left + vertexEnd.x*fScale-3;
			//rfVertex.Y = rcPanel.bottom - vertexEnd.y*fScale-3;
			//rfVertex.Width = 6;
			//rfVertex.Height = 6;
			//g.FillRectangle(&SolidBrush(Color::Blue), rfVertex);
		}
	}
	pathOutline.CloseFigure();
}

void GetComponentRegion(vector<PointInfo>& vAllOutlinePoint, CRect rcComponent, float fScale, Region& regionComponent)
{
	//Region regionTmp;

	vector<vector<PointInfo>> vOutlinePointGroup;
	GetOutlineGroups(vAllOutlinePoint, vOutlinePointGroup);

	GraphicsPath pathComponentOutter;
	//vector<PointInfo> vOutterOutlinePoint;
	//GetOutterOutlineVertex(vAllOutlinePoint, vOutterOutlinePoint);
	if (vOutlinePointGroup.size() > 0)
	{
		GetOutlinePath(vOutlinePointGroup[vOutlinePointGroup.size()-1], rcComponent, fScale, pathComponentOutter);
		Region regionComponentOutter(&pathComponentOutter);

		for(int i = 0; i < vOutlinePointGroup.size()-1; i++)
		{
			GraphicsPath pathComponentOneInner;
			GetOutlinePath(vOutlinePointGroup[i], rcComponent, fScale, pathComponentOneInner);
			Region regionComponentOneInner(&pathComponentOneInner);
			regionComponentOutter.Exclude(&regionComponentOneInner);
		}

		regionComponent.MakeEmpty();
		regionComponent.Union(&regionComponentOutter);
	}
}

bool SortMaterialListData(std::map<std::string,std::string>& first, std::map<std::string,std::string>& second)
{
	CString strMaterial_First, strMaterial_Second, strThickness_First, strThickness_Second;
	if(first.find("PanelMatrial") != first.end())
		strMaterial_First = first["PanelMatrial"].c_str();
	if(first.find("Thickness") != first.end())
		strThickness_First = first["Thickness"].c_str();
	if(second.find("PanelMatrial") != second.end())
		strMaterial_Second = second["PanelMatrial"].c_str();
	if(second.find("Thickness") != second.end())
		strThickness_Second = second["Thickness"].c_str();

	bool bRet = true;
	if(atof(strThickness_First.GetBuffer()) < atof(strThickness_Second.GetBuffer()))
		bRet = true;
	else if(IsFloatEqual(atof(strThickness_First.GetBuffer()), atof(strThickness_Second.GetBuffer())))
	{
		if(strMaterial_First.Compare(strMaterial_Second) <= 0)
			bRet = true;
		else
			bRet = false;
	}
	else
		bRet = false;

	return bRet;
}

std::map<std::string,std::string> GetMapByComponentInputItem(ComponentInputItem& theComponentInputItem)
{
	std::map<std::string,std::string> noProcessSmallPanelOne;

	noProcessSmallPanelOne["order"]			 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strOrderID);	//订单号
	noProcessSmallPanelOne["cabinet"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strCabinetID);	//柜号
	noProcessSmallPanelOne["info"]			 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strBarcode);	//条码
	noProcessSmallPanelOne["PorductName"]	 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strCabinetName);	//产品名称
	noProcessSmallPanelOne["porduct"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strPanelName);	//成品名称
	noProcessSmallPanelOne["matrial"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strMaterial);	//材质
	noProcessSmallPanelOne["width"]			 = JT2_Tools::Convert2StdString(GetFloatStringTrimming(theComponentInputItem.m_fLength, 1));	//长
	noProcessSmallPanelOne["height"]		 = JT2_Tools::Convert2StdString(GetFloatStringTrimming(theComponentInputItem.m_fWidth, 1));	//宽
	noProcessSmallPanelOne["thin"]			 = JT2_Tools::Convert2StdString(GetFloatStringTrimming(theComponentInputItem.m_fThickness, 1));	//厚度
	noProcessSmallPanelOne["dir"]			 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strTexture);	//纹理方向
	noProcessSmallPanelOne["edgeLen1"]		 = JT2_Tools::Convert2StdString(GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_FRONT], 1));	//封长1
	noProcessSmallPanelOne["edgeWidth1"]	 = JT2_Tools::Convert2StdString(GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_RIGHT], 1));	//封宽1
	noProcessSmallPanelOne["edgeLen2"]		 = JT2_Tools::Convert2StdString(GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_BACK], 1));	//封长2
	noProcessSmallPanelOne["edgeWidth2"]	 = JT2_Tools::Convert2StdString(GetFloatString(theComponentInputItem.m_afBanding[BANDING_ID_LEFT], 1));	//封宽2
	noProcessSmallPanelOne["pointInfo"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strOtherShapeIDAndParam);	//异形信息
	CString strTmp;
	strTmp.Format(_T("%d"), theComponentInputItem.m_eRotateType);
	noProcessSmallPanelOne["rotate"]		 = JT2_Tools::Convert2StdString(strTmp);	//是否旋转
	noProcessSmallPanelOne["slotInfo"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strUpperFaceSlotInfo);	//槽
	noProcessSmallPanelOne["slotRotate"]	 = JT2_Tools::Convert2StdString(theComponentInputItem.m_bSlotFlipped ? _T("是") :  _T(""));	//槽翻转
	noProcessSmallPanelOne["dslotInfo"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strDownerFaceSlotInfo);	//反面槽
	noProcessSmallPanelOne["vecInfo"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strUpperFaceHoleInfo);	//孔
	noProcessSmallPanelOne["vecRotate"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_bVHoleFlipped ? _T("是") :  _T(""));	//孔翻转
	noProcessSmallPanelOne["dvecInfo"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strDownerFaceHoleInfo);	//反面孔
	noProcessSmallPanelOne["customeInfo"]	 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strCustomerInfo);	//客户信息
	noProcessSmallPanelOne["store"]			 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strJoinedStore);	//加盟店
	noProcessSmallPanelOne["SawMark"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strSlottingFlag);	//拉槽标识
	noProcessSmallPanelOne["HelpSign"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strReminder);	//助记号
	noProcessSmallPanelOne["Vertical"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strDrilling);	//钻孔
	noProcessSmallPanelOne["OrderType"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strOrderType);	//订单类型
	noProcessSmallPanelOne["BackCode"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strReverseSideBarcode);	//反面条码
	noProcessSmallPanelOne["ProductWidth"]	 = JT2_Tools::Convert2StdString(GetFloatStringTrimming(theComponentInputItem.m_fProductLength, 1));	//成品长度
	noProcessSmallPanelOne["ProductHeight"]	 = JT2_Tools::Convert2StdString(GetFloatStringTrimming(theComponentInputItem.m_fProductWidth, 1));	//成品宽度
	noProcessSmallPanelOne["ProductThin"]	 = JT2_Tools::Convert2StdString(GetFloatStringTrimming(theComponentInputItem.m_fProductThickness, 1));	//成品厚度
	noProcessSmallPanelOne["otherID"]		 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strOtherShapeID);	//异型ID
	noProcessSmallPanelOne["customeAddr"]	 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strCustomerAddress);	//客户地址
	noProcessSmallPanelOne["DrillingLogo"]	 = JT2_Tools::Convert2StdString(theComponentInputItem.m_strHoleSlotFlag);	//钻槽标识
	noProcessSmallPanelOne["OtherPoints"]	 = JT2_Tools::Convert2StdString(_T(""));	//异形点信息

	return (noProcessSmallPanelOne);
}

void ModifyReminderByDateAndOrderFormat(vector<ComponentInputItem>& vComponentInputItem)
{
	int nDate_InFile = 0;
	map<CString, int> mapMainOrderToSerID_InFile;

	CString strTemporaryRecordFile = GetModulePath() + TEMPORARY_RECORD_FILE_DIR;
	TiXmlDocument * xmlDoc = new TiXmlDocument();
	if(PathFileExists(strTemporaryRecordFile))
	{	
		bool bLoadRes = xmlDoc->LoadFile(strTemporaryRecordFile);
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			TiXmlElement* elmDateAndOrder = xmlHandler.GetChildElm(xmlDoc->RootElement(), "DateAndOrder");

			xmlHandler.GetXmlAttribute(elmDateAndOrder, "Date", nDate_InFile);
			vector<TiXmlElement*> vMainOrderElm = xmlHandler.GetChildElms(elmDateAndOrder, "MainOrder");
			for(int i = 0; i < vMainOrderElm.size(); i++)
			{
				CString strTmp;
				int nTmp;
				xmlHandler.GetXmlAttribute(vMainOrderElm[i], "Name", strTmp);
				xmlHandler.GetXmlAttribute(vMainOrderElm[i], "SerialID", nTmp);
				mapMainOrderToSerID_InFile.insert(make_pair(strTmp, nTmp));
			}
		}
	}
	delete xmlDoc;

	int nDate_Today = 0;
	map<CString, int> mapMainOrderToSerID_Today;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	nDate_Today = sysTime.wDay;
	if(nDate_InFile == nDate_Today)
	{
		mapMainOrderToSerID_Today = mapMainOrderToSerID_InFile;
	}

	for(int i = 0; i < vComponentInputItem.size(); i++)
	{
		CString strOrderName = vComponentInputItem[i].m_strOrderID;
		if(strOrderName.Find(_T("-")) != -1)
		{
			int nSerIDForTheComponent;

			CString strMainOrder = strOrderName.Left(strOrderName.Find(_T("-")));
			map<CString, int>::iterator itr = mapMainOrderToSerID_Today.find(strMainOrder);
			if(itr != mapMainOrderToSerID_Today.end())
				nSerIDForTheComponent = itr->second;
			else
			{
				nSerIDForTheComponent = mapMainOrderToSerID_Today.size() + 1;
				mapMainOrderToSerID_Today.insert(make_pair(strMainOrder, nSerIDForTheComponent));
			}

			CString strReminder;
			strReminder.Format(_T("%d-%d"), nDate_Today, nSerIDForTheComponent);
			vComponentInputItem[i].m_strReminder = strReminder;
		}
	}



	xmlDoc = new TiXmlDocument();
	TiXmlElement* elmDateAndOrder = NULL;
	if(!PathFileExists(strTemporaryRecordFile))
	{
		TiXmlElement* elmRoot = new TiXmlElement("Root");
		xmlDoc->LinkEndChild(elmRoot);
		elmDateAndOrder = new TiXmlElement("DateAndOrder");
		elmRoot->LinkEndChild(elmDateAndOrder);
	}
	else
	{
		bool bLoadRes = xmlDoc->LoadFile(strTemporaryRecordFile);
		if(bLoadRes)
		{
			XmlHandler xmlHandler;
			elmDateAndOrder = xmlHandler.GetChildElm(xmlDoc->RootElement(), "DateAndOrder");
			if(!elmDateAndOrder)
			{
				elmDateAndOrder = new TiXmlElement("DateAndOrder");
				xmlDoc->RootElement()->LinkEndChild(elmDateAndOrder);
			}
		}
		else
		{
			delete xmlDoc;
			return;
		}
	}

	elmDateAndOrder->Clear();
	elmDateAndOrder->SetAttribute("Date", nDate_Today);
	for(map<CString, int>::iterator itr = mapMainOrderToSerID_Today.begin(); itr != mapMainOrderToSerID_Today.end(); itr++)
	{	
		TiXmlElement* elmItem = new TiXmlElement("MainOrder");
		elmDateAndOrder->LinkEndChild(elmItem);
		elmItem->SetAttribute("Name", itr->first);

		elmItem->SetAttribute("SerialID", itr->second);
	}

	xmlDoc->SaveFile(strTemporaryRecordFile);
	delete xmlDoc;
}

CString GetLogDir()
{
	CString strRet;

	TCHAR szMyDoc[_MAX_PATH];  
	SHGetSpecialFolderPath(NULL, szMyDoc, CSIDL_MYDOCUMENTS, 0);
	strRet.Format(_T("%s\\HG3D\\log\\Carving\\"), szMyDoc);

	return strRet;
}

void RemoveRedundandCoppiedDataFile()
{
	vector<CString> vDataFile;
	vector<CTime> vDataFileTime;

	CFileFind fileFind; 

	CString strFileFind;
	CString strDir = GetLogDir();
	strFileFind.Format(_T("%s*.*"), strDir);
	BOOL IsFinded = fileFind.FindFile(strFileFind); 
	while (IsFinded) 
	{ 
		IsFinded = fileFind.FindNextFile(); 
		if (!fileFind.IsDots())
		{ 
			if(fileFind.IsDirectory()) 
			{ 			
				CString strFile = strDir + fileFind.GetFileName();
				vDataFile.push_back(strFile);
				CTime timeFile;
				fileFind.GetCreationTime(timeFile);
				vDataFileTime.push_back(timeFile);
			} 
		} 
	} 
	fileFind.Close(); 


	while(vDataFile.size() > 30)
	{
		int nToBeRemove = 0;
		for(int i = 1; i < vDataFile.size(); i++)
		{
			if(vDataFileTime[i] < vDataFileTime[nToBeRemove])
				nToBeRemove = i;
		}

		//DeleteFile(vDataFile[nToBeRemove]);
		DeleteFilesInDirectory(vDataFile[nToBeRemove], TRUE);
		vector<CString>::iterator itrDataFileRemove = vDataFile.begin();
		std::advance(itrDataFileRemove, nToBeRemove);
		vDataFile.erase(itrDataFileRemove);
		vector<CTime>::iterator itrDataFileTimeRemove = vDataFileTime.begin();
		std::advance(itrDataFileTimeRemove, nToBeRemove);
		vDataFileTime.erase(itrDataFileTimeRemove);
	}
}
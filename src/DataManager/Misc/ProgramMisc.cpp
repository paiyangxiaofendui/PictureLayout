#include "stdafx.h"
#include "ProgramMisc.h"

//#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
//#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
//#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
//#include "../../../include/HG3D/XmlFile.h"
//#include "../../../include/HG3D/XmlNode.h"
//#include "../../../include/HG3D/Utils.h"

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

//void GetSolutionPanel(vector<PanelAndTheSolution>& vPanelAndTheSolution)
//{
//	vPanelAndTheSolution.clear();
//	for(int i = 0; i < CSingleon::GetSingleton()->m_BackupSolutionList.size(); i++)
//	{
//		CSolution* pSolution = CSingleon::GetSingleton()->m_BackupSolutionList[i];
//		for(int j = 0; j < pSolution->m_PanelList.size(); j++)
//		{
//			PanelAndTheSolution panleAndSol;
//			panleAndSol.m_pPanel = pSolution->m_PanelList[j];
//			panleAndSol.m_pSolution = pSolution;
//			vPanelAndTheSolution.push_back(panleAndSol);
//		}
//	}
//}
//
////得到贴标的宽高
//void GetTiebiaoSize( float& tiebiao_width, float& tiebiao_height )
//{
//	hg3d::TString strConfigFile = _T("picture");
//	strConfigFile = hg3d::getFullFileName(strConfigFile);
//	strConfigFile += _T("\\printcode.xml");
//
//	hg3d::CXmlFile* doc = new hg3d::CXmlFile();
//	if (!doc->Load(strConfigFile.c_str()))
//	{
//		AfxMessageBox("读取printcode.xml失败",MB_OK);
//		return;
//	}
//
//	hg3d::CXmlNode * print_node = NULL;
//
//	for (int i = 0;i < doc->GetChildNodes().size();i++)
//	{
//		hg3d::CXmlNode* child_node = doc->GetChildNodes().at(i);
//		std::string _used = child_node->GetAttributeValue("Used");
//		if (_used == "1")
//		{
//			print_node = child_node;
//			break;
//		}
//	}
//
//	//如果没有找到任何标签配置信息，那么默认标签大小认为是100*100
//	if (print_node == NULL)
//	{
//		tiebiao_width = 100/4.0;
//		tiebiao_height = 100/4.0;
//		return;
//	}
//
//	tiebiao_width = atof(print_node->GetAttributeValue("paperWidth").c_str())/4.0;
//	tiebiao_height = atof(print_node->GetAttributeValue("paperHeight").c_str())/4.0;
//}

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
//void OutlinePointsAntiClockwiseRotate90(vector<PointInfo>& vOutlinePoint)
//{
//
//}

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
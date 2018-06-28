#pragma once
#include<vector>
#include "../BaseDataType/Component/Component.h"

using namespace std;

class Component;
//class Panel;
//class CSolution;

//struct PanelAndTheSolution
//{
//	Panel* m_pPanel;
//	CSolution* m_pSolution;
//};

void FindAllComponentInPanel(Component& thePanel, vector<Component*>& vAllComponent);

//void GetSolutionPanel(vector<PanelAndTheSolution>& vPanelAndTheSolution);
//
//void GetTiebiaoSize( float& tiebiao_width, float& tiebiao_height );

void OutlinePointsClockwiseRotate90(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen);

void HoleClockwiseRotate90(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen);

void SlotClockwiseRotate90(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen);

//void OutlinePointsAntiClockwiseRotate90(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen);

void BandingClockwiseRotate90(float (&afBanding)[4]);

void LabelPosClockwiseRotate90(float& fXLabelCenter, float& fYLabelCenter, float fOrgXLen, float fOrgYLen);

typedef Component* PTR_COMPONENT;
/*extern static */bool Sort_ComponentByMachiningOrder(const PTR_COMPONENT & m1, const PTR_COMPONENT & m2);

int GetRegularAngle(int nAngle);

void ComponentRotateClockwise90(Component* pComponent);
	
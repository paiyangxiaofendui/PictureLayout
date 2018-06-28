#pragma once
#include<vector>
#include "../Work/WorkDef.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include <map>
#include "../UIData/UIData.h"

using namespace std;

class Component;
class Panel;
class CSolution;

struct PanelAndTheSolution
{
	Panel* m_pPanel;
	CSolution* m_pSolution;
};

void FindAllComponentInPanel(Component& thePanel, vector<Component*>& vAllComponent, NodeType eNodeType = NodeType_NeededComponent);

int GetComponentCountInPanel(Component& thePanel);

void GetSolutionPanel(vector<PanelAndTheSolution>& vPanelAndTheSolution);

float GetSolutionComponentToatlArea(CSolution& theSolution);

float GetSolutionRemainderToatlArea(CSolution& theSolution);

float GetSolutionPanelTotalArea(CSolution& theSolution);

void GetTiebiaoSize( float& tiebiao_width, float& tiebiao_height );

LineType GetLineType(int nLineSign);

void FindAllComponentInSingleton(vector<Component*>& vAllComponent);

void FindAllPanelInSingleton(vector<Panel*>& vAllPanel);

void CollectAllMachiningInfoFromComponents(vector<Component*>& vAllComponent, vector<AllVec>& vUpperFaceHole, vector<AllVec>& vDownerFaceHole, vector<AllSlot>& vUpperFaceSlot, vector<AllSlot>& vDownerFaceSlot);

void CollectAllMachiningInfoInSingleton(vector<AllVec>& vUpperFaceHole, vector<AllVec>& vDownerFaceHole, vector<AllSlot>& vUpperFaceSlot, vector<AllSlot>& vDownerFaceSlot);

TextureDirectionType GetComponentRotetedTextureDirType(TextureDirectionType eOrgInputTextureDir, int nRotatedAngle);

int GetOutterOutlineVertex(vector<PointInfo>& vOutlinePoint, vector<PointInfo>& vOutterOutlinePoint);

void GetOutlineGroups(vector<PointInfo>& vOutlinePoint, vector<vector<PointInfo>>& vOutlinePointGroup);

int GetKnifeDownPosByClickedPos(vector<PointInfo>& vOutlinePoint, float fXClickedInComponent, float fYClickedInComponent);

void ResetMachiningOrderByMap(Panel*pThePanel, map<int, int> mapResettedOrder);

Component* GetClickedComponent(CPoint point, CRect rcDrawArea, int nGap, Panel* pPanel);

void RemoveComponentFromPanel(Component* pComponent, Panel* pPanel);

void SetComponentToRemainder(Component* pComponent);

bool IsComponentOverlap(Panel* pThePanel, Component* pComponent, PointFloat ptInPanel, float fKerf);

bool IsComponentInPanelClipped(Panel* pThePanel, Component* pComponent, PointFloat ptInPanel, float fDeburringWidth);

bool InsertComponentToPanelRoot(Panel* pPanel, Component* pComponent, PointFloat ptComponentLeftBottom);

void ReArrangeComponentMachiningOrder(Panel* pPanel);

void OutlinePointsClockwiseRotate90(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen);

void HoleClockwiseRotate90(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen);

void SlotClockwiseRotate90(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen);

void BandingClockwiseRotate90(float (&afBanding)[4]);

void LabelPosClockwiseRotate90(float& fXLabelCenter, float& fYLabelCenter, float fOrgXLen, float fOrgYLen);

void OutlinePointsAntiClockwiseRotate90(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen);

void HoleAntiClockwiseRotate90(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen);

void SlotAntiClockwiseRotate90(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen);

void BandingAntiClockwiseRotate90(float (&afBanding)[4]);

void LabelPosAntiClockwiseRotate90(float& fXLabelCenter, float& fYLabelCenter, float fOrgXLen, float fOrgYLen);

void ComponentRotateClockwise90(Component* pComponent);

void FlipComponentH(Component* pComponent);

void OutlinePointsFlipH(vector<PointInfo>& vOutlinePoint, float fOrgXLen, float fOrgYLen);

//void HoleFlipH(vector<AllVec>& vHole, float fOrgXLen, float fOrgYLen);
//
//void SlotFlipH(vector<AllSlot>& vSlot, float fOrgXLen, float fOrgYLen);

CString GetTextureString(TextureDirectionType eTextureType);

//获取PrintInfo合并成字符
std::string getPrintInfoAsString(ComponentInputItem& theItem);

float GetKerf();

void DrawLogoTextInLabel(CDC* pDC, CRect rcLabelArea);

int GetLabelSide(LPCSTR szMachineName);

CString GetLabelImageName(int nPanelID, int nComponentID);

CString GetPanelImageName(int nPanelID);

void GetPreCombineItemRect(PreCombineItem* pItem, float& fLeft, float& fTop, float& fRight, float& fBottom);

bool IsVHoleDeeperThanThickness(ComponentInputItem& theItem);

bool IsVHoleOutOfPanel(ComponentInputItem& theItem);

bool IsSlotDeeperThanThickness(ComponentInputItem& theItem);

bool IsComponentOverSize(ComponentInputItem& theItem);

typedef Component* PCOMPONENT;
bool SortComponentByIDInPanel(const PCOMPONENT& first, const PCOMPONENT& second);

void GetOutlinePath(vector<PointInfo>& vOutlinePoint, CRect rcComponent, float fScale, GraphicsPath& pathOutline);

void GetComponentRegion(vector<PointInfo>& vAllOutlinePoint, CRect rcComponent, float fScale, Region& regionComponent);

bool SortMaterialListData(std::map<std::string,std::string>& first, std::map<std::string,std::string>& second);

std::map<std::string,std::string> GetMapByComponentInputItem(ComponentInputItem& theComponentInputItem);

void ModifyReminderByDateAndOrderFormat(vector<ComponentInputItem>& vComponentInputItem);

CString GetLogDir();

void RemoveRedundandCoppiedDataFile();
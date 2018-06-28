#include "StdAfx.h"
#include "CNCDataConverter.h"
#include "../Misc/Misc.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"

struct FloatPoint;
struct OutlineElement;

void PanelsToCadDataFile(const vector<HomagDataItem>& vHomagDataItem, LPCTSTR szDir);
void CallConverter(LPCTSTR szMachineID, LPCTSTR szPanelDataFilePath, LPCTSTR szOutputDir);

TiXmlElement* GenPanelElm(const HomagDataItem& theItem);
TiXmlElement* GenOneHoleElm(const AllVec& theHole);
TiXmlElement* GenOneSlotElm(const AllSlot& theSlot);
TiXmlElement* GenOneTrackLineElm(const OutlineElement& outlineElm);
vector<OutlineElement> GetComponentOutlineElm(const Component& theComponent, int& nOutlineGroupID);

struct FloatPoint
{
	FloatPoint():m_x(0.0),m_y(0.0),m_z(0.0){}
	FloatPoint(float x, float y, float z = 0.0):m_x(x),m_y(y),m_z(z){}
	FloatPoint(const FloatPoint& fp)
	{
		m_x = fp.m_x;
		m_y = fp.m_y;
		m_z = fp.m_z;
	}
	float Distance(const FloatPoint& fp)
	{
		return sqrt((fp.m_x - m_x) * (fp.m_x - m_x) + (fp.m_y - m_y) * (fp.m_y - m_y));
	}

	BOOL IsEqual(const FloatPoint& fp)
	{
		if((fabs(fp.m_x-m_x) < 0.001) && (fabs(fp.m_y-m_y) < 0.001))
			return TRUE;
		return FALSE;
	}

	float m_x;
	float m_y;
	float m_z;
};

struct OutlineElement
{
	int m_nGroupID;
	bool m_bIsArc;
	int m_nType;
	vector<FloatPoint> m_vPoint;
};

//void ExportGCode_Homag(const vector<PanelAndTheSolution>& vPanelAndTheSolution, DirVHType eFlipType)
//{
//	CString strDir;
//	strDir.Format(_T("%s%s"), GetModulePath(), _T("temp\\CadData.xml"));
//	PanelsToCadDataFile(vPanelAndTheSolution, eFlipType, strDir);
//	CallConverter("Homag", "PTP160", strDir);
//}

void ExportGCode_Homag(const vector<HomagDataItem>& vHomagDataItem, LPCTSTR szOutputDir)
{
	CString strOutputDir = szOutputDir;
	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	CString strTmpFilePath;
	strTmpFilePath.Format(_T("%s%s"), GetModulePath(), _T("temp\\CadData.xml"));

	PanelsToCadDataFile(vHomagDataItem, strTmpFilePath);
	CallConverter("80000", strTmpFilePath, strOutputDir);
}

void PanelsToCadDataFile(const vector<HomagDataItem>& vHomagDataItem, LPCTSTR szDir)
{
	TiXmlDocument * xmlDoc = new TiXmlDocument();
	TiXmlDeclaration * xmlDecl = new TiXmlDeclaration(WStringToUTF8(L"1.0").c_str(), WStringToUTF8(L"utf-8").c_str(), WStringToUTF8(L"").c_str());
	xmlDoc->LinkEndChild(xmlDecl);
	TiXmlElement * elmRoot = new TiXmlElement(WStringToUTF8(L"ProducData").c_str());
	xmlDoc->LinkEndChild(elmRoot);

	for(int i = 0; i < vHomagDataItem.size(); i++)
	{
		//const Panel* pThePanel =vHomagDataItem[i].m_PanelAndTheSolution.m_pPanel;
		//const CSolution* pTheSolution = vPanelAndTheSolution[i].m_pSolution;

		elmRoot->LinkEndChild(GenPanelElm(vHomagDataItem[i]));

	}

	xmlDoc->SaveFile(szDir);
	delete xmlDoc;
	xmlDoc = NULL;
}

void CallConverter(LPCTSTR szMachineID, LPCTSTR szPanelDataFilePath, LPCTSTR szOutputDir)
{
	USES_CONVERSION;
	typedef void (__cdecl * CNCDataConverterFun)(const char* szMachineID, const char* szDataFileDir, const char* szOutputDir);
	CString strDllDir/*, strDataDir*/;

	strDllDir = GetModulePath() + _T("CNCDataConverter.dll");
	//strDataDir.Format(_T("%s%s"), GetModulePath(), _T("temp\\CadData.xml"));

	HMODULE hDll = NULL;
	hDll = ::LoadLibrary(strDllDir);
	CNCDataConverterFun func;
	func = (CNCDataConverterFun)GetProcAddress(hDll, "CNCDataConverterFuncM_Dir");


	(*func)(szMachineID, szPanelDataFilePath, szOutputDir);
	FreeLibrary(hDll);
}

TiXmlElement* GenPanelElm(const HomagDataItem& theItem)
{
	TiXmlElement * elmPlane = new TiXmlElement(WStringToUTF8(L"PanelProduc").c_str());

	const Panel* pThePanel = theItem.m_PanelAndTheSolution.m_pPanel;

	float fPanelXLen = pThePanel->m_OrgLen;
	float fPanelYLen = pThePanel->m_OrgWidth;
	float fPanelThickness = pThePanel->m_Thickness;

	CString strBarcode, strABarcode, strBBarcode;
	//strBarcode.Format(_T("HG_%d"), nSerID+1);
	strBarcode = GetFileNameInPath(theItem.m_strPanelUpperFaceFileName);
	strABarcode = strBarcode;
	if(!theItem.m_strPanelDownerFaceFileName_Flip.IsEmpty())
	{
		//strBBarcode.Format(_T("HG_%d%s"), nSerID+1, eFlipType == DirVHType::DIR_H ? _T("@W") : _T("@H"));
		strBBarcode = GetFileNameInPath(theItem.m_strPanelDownerFaceFileName_Flip);
	}
	elmPlane->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"Name1").c_str());
	elmPlane->SetAttribute(WStringToUTF8(L"PanelStyle").c_str(), WStringToUTF8(L"100000").c_str());
	elmPlane->SetAttribute(WStringToUTF8(L"Barcode").c_str(), WStringToUTF8(AnsiToUnicode(strBarcode.GetBuffer()).c_str()).c_str());
	elmPlane->SetAttribute(WStringToUTF8(L"ABarcode").c_str(), WStringToUTF8(AnsiToUnicode(strABarcode.GetBuffer()).c_str()).c_str());
	elmPlane->SetAttribute(WStringToUTF8(L"BBarcode").c_str(), WStringToUTF8(AnsiToUnicode(strBBarcode.GetBuffer()).c_str()).c_str());

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(*((Component*)pThePanel), vAllComponent, NodeType::NodeType_NeededComponent);

	TiXmlElement * elmUpperFace = new TiXmlElement(WStringToUTF8(L"PanelFace").c_str());
	elmPlane->LinkEndChild(elmUpperFace);
	elmUpperFace->SetAttribute(WStringToUTF8(L"Index").c_str(), WStringToUTF8(L"0").c_str());
	elmUpperFace->SetAttribute(WStringToUTF8(L"Width").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(fPanelXLen, 1).GetBuffer()).c_str()).c_str());
	elmUpperFace->SetAttribute(WStringToUTF8(L"Heigth").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(fPanelYLen, 1).GetBuffer()).c_str()).c_str());
	elmUpperFace->SetAttribute(WStringToUTF8(L"Thickness").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(fPanelThickness, 1).GetBuffer()).c_str()).c_str());
	elmUpperFace->SetAttribute(WStringToUTF8(L"IsUseTrackLine").c_str(), WStringToUTF8(L"1").c_str());
	elmUpperFace->SetAttribute(WStringToUTF8(L"IsShowTrackLine").c_str(), WStringToUTF8(L"1").c_str());

	vector<AllVec> vUpperFaceHole;
	for(int j = 0; j < vAllComponent.size(); j++)
	{
		const Component& theComponent = *(vAllComponent[j]);
		for(int k = 0; k < theComponent.m_vUpperFaceHole.size(); k++)
		{
			AllVec theHole = theComponent.m_vUpperFaceHole[k];
			theHole._x += theComponent.m_x;
			theHole._y += theComponent.m_y;
			vUpperFaceHole.push_back(theHole);
		}
	}

	//int nFlipType = 1;
	vector<AllVec> vDownerFaceHole;
	for(int j = 0; j < vAllComponent.size(); j++)
	{
		const Component& theComponent = *(vAllComponent[j]);
		for(int k = 0; k < theComponent.m_vDownerFaceHole.size(); k++)
		{
			AllVec theHole = theComponent.m_vDownerFaceHole[k];
			if(theItem.m_eFlipType == DirVHType::DIR_V)
			{
				theHole._x += theComponent.m_x;
				theHole._y = fPanelYLen - (theComponent.m_y + theHole._y);
			}
			else
			{
				theHole._x = fPanelXLen - (theComponent.m_x + theHole._x);
				theHole._y = theComponent.m_y + theHole._y;
			}

			vDownerFaceHole.push_back(theHole);
		}
	}

	vector<AllSlot> vUpperFaceSlot;
	for(int j = 0; j < vAllComponent.size(); j++)
	{
		const Component& theComponent = *(vAllComponent[j]);
		for(int k = 0; k < theComponent.m_vUpperFaceSlot.size(); k++)
		{
			AllSlot theSlot = theComponent.m_vUpperFaceSlot[k];
			theSlot._x += theComponent.m_x;
			theSlot._y += theComponent.m_y;
			vUpperFaceSlot.push_back(theSlot);
		}
	}

	vector<AllSlot> vDownerFaceSlot;
	for(int j = 0; j < vAllComponent.size(); j++)
	{
		const Component& theComponent = *(vAllComponent[j]);
		for(int k = 0; k < theComponent.m_vDownerFaceSlot.size(); k++)
		{
			AllSlot theSlot = theComponent.m_vDownerFaceSlot[k];
			if(theItem.m_eFlipType == DirVHType::DIR_V)
			{
				theSlot._x += theComponent.m_x;
				theSlot._y = fPanelYLen - (theComponent.m_y + theSlot._y);
			}
			else
			{
				theSlot._x = fPanelXLen - (theComponent.m_x + theSlot._x);
				theSlot._y += theComponent.m_y;
			}

			vDownerFaceSlot.push_back(theSlot);
		}
	}

	vector<OutlineElement> vOutlineElm;
	int nOutlineGroupID = 1;
	for(int j = 0; j < vAllComponent.size(); j++)
	{
		const Component& theComponent = *(vAllComponent[j]);
		vector<OutlineElement> vOneComponentOutlineElm = GetComponentOutlineElm(theComponent, nOutlineGroupID);
		vOutlineElm.insert(vOutlineElm.end(), vOneComponentOutlineElm.begin(), vOneComponentOutlineElm.end());
	}

	for(int j = 0; j < vUpperFaceHole.size(); j++)
	{
		const AllVec& theHole = vUpperFaceHole[j];
		elmUpperFace->LinkEndChild(GenOneHoleElm(theHole));
	}

	for(int j = 0; j < vUpperFaceSlot.size(); j++)
	{
		const AllSlot& theSlot = vUpperFaceSlot[j];
		elmUpperFace->LinkEndChild(GenOneSlotElm(theSlot));
	}

	for(int j = 0; j < vOutlineElm.size(); j++)
	{
		const OutlineElement& outlineElm = vOutlineElm[j];
		elmUpperFace->LinkEndChild(GenOneTrackLineElm(outlineElm));
	}

	if(!theItem.m_strPanelDownerFaceFileName_Flip.IsEmpty())
	{
		TiXmlElement * elmDownerFace = new TiXmlElement(WStringToUTF8(L"PanelFace").c_str());
		elmPlane->LinkEndChild(elmDownerFace);
		elmDownerFace->SetAttribute(WStringToUTF8(L"Index").c_str(), WStringToUTF8(L"5").c_str());
		elmDownerFace->SetAttribute(WStringToUTF8(L"Width").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(fPanelXLen, 1).GetBuffer()).c_str()).c_str());
		elmDownerFace->SetAttribute(WStringToUTF8(L"Heigth").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(fPanelYLen, 1).GetBuffer()).c_str()).c_str());
		elmDownerFace->SetAttribute(WStringToUTF8(L"Thickness").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(fPanelThickness, 1).GetBuffer()).c_str()).c_str());
		elmDownerFace->SetAttribute(WStringToUTF8(L"IsUseTrackLine").c_str(), WStringToUTF8(L"0").c_str());
		elmDownerFace->SetAttribute(WStringToUTF8(L"IsShowTrackLine").c_str(), WStringToUTF8(L"1").c_str());

		for(int j = 0; j < vDownerFaceHole.size(); j++)
		{
			const AllVec& theHole = vDownerFaceHole[j];
			elmDownerFace->LinkEndChild(GenOneHoleElm(theHole));
		}

		for(int j = 0; j < vDownerFaceSlot.size(); j++)
		{
			const AllSlot& theSlot = vDownerFaceSlot[j];
			elmDownerFace->LinkEndChild(GenOneSlotElm(theSlot));
		}
	}

	return elmPlane;
}

TiXmlElement* GenOneHoleElm(const AllVec& theHole)
{
	TiXmlElement * elmHole = new TiXmlElement(WStringToUTF8(L"Mark").c_str());
	elmHole->SetAttribute(WStringToUTF8(L"Layer").c_str(), WStringToUTF8(L"VERTICAL").c_str());
	elmHole->SetAttribute(WStringToUTF8(L"pos_x").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theHole._x, 1).GetBuffer()).c_str()).c_str());
	elmHole->SetAttribute(WStringToUTF8(L"pos_y").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theHole._y, 1).GetBuffer()).c_str()).c_str());
	elmHole->SetAttribute(WStringToUTF8(L"size_x").c_str(),  WStringToUTF8(AnsiToUnicode(GetFloatString(theHole._r, 1).GetBuffer()).c_str()).c_str());
	elmHole->SetAttribute(WStringToUTF8(L"depth").c_str(),  WStringToUTF8(AnsiToUnicode(GetFloatString(theHole._depth, 1).GetBuffer()).c_str()).c_str());
	elmHole->SetAttribute(WStringToUTF8(L"isUse").c_str(), WStringToUTF8(L"1").c_str());//////////////////////////////////////////////////////////

	return elmHole;
}

TiXmlElement* GenOneSlotElm(const AllSlot& theSlot)
{
	TiXmlElement * elmSlot = new TiXmlElement(WStringToUTF8(L"Mark").c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"Layer").c_str(), WStringToUTF8(L"ROUTER").c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"pos_x").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theSlot._x, 1).GetBuffer()).c_str()).c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"pos_y").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theSlot._y, 1).GetBuffer()).c_str()).c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"angle").c_str(), WStringToUTF8(theSlot._dir == 0 ? L"90" : L"0").c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"size_x").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theSlot._width, 1).GetBuffer()).c_str()).c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"size_y").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theSlot._height, 1).GetBuffer()).c_str()).c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"depth").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(theSlot._depth, 1).GetBuffer()).c_str()).c_str());
	elmSlot->SetAttribute(WStringToUTF8(L"isUse").c_str(), WStringToUTF8(L"1").c_str());

	return elmSlot;
}

TiXmlElement* GenOneTrackLineElm(const OutlineElement& outlineElm)
{
	TiXmlElement * elmTrackLine = new TiXmlElement(WStringToUTF8(L"Trackline").c_str());
	elmTrackLine->SetAttribute(WStringToUTF8(L"Layer").c_str(), WStringToUTF8(L"ROUTER").c_str());
	elmTrackLine->SetAttribute(WStringToUTF8(L"GroupID").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(outlineElm.m_nGroupID, 0).GetBuffer()).c_str()).c_str());
	elmTrackLine->SetAttribute(WStringToUTF8(L"IsArc").c_str(), WStringToUTF8(outlineElm.m_bIsArc ? L"True" : L"False").c_str());
	elmTrackLine->SetAttribute(WStringToUTF8(L"type").c_str(), WStringToUTF8(AnsiToUnicode(GetFloatString(outlineElm.m_nType, 0).GetBuffer()).c_str()).c_str());
	for(int k = 0; k < outlineElm.m_vPoint.size(); k++)
	{
		TiXmlElement * elmPoint = new TiXmlElement(WStringToUTF8(L"Param").c_str());
		elmTrackLine->LinkEndChild(elmPoint);
		elmPoint->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"vertex").c_str());
		CStringW strPoint;
		strPoint.Format(L"%s %s 0.0", \
			AnsiToUnicode(GetFloatString(outlineElm.m_vPoint[k].m_x, 1).GetBuffer()).c_str(), \
			AnsiToUnicode(GetFloatString(outlineElm.m_vPoint[k].m_y, 1).GetBuffer()).c_str());
		elmPoint->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(strPoint.GetBuffer()).c_str());
		strPoint.ReleaseBuffer();
	}

	return elmTrackLine;
}

vector<OutlineElement> GetComponentOutlineElm(const Component& theComponent, int& nOutlineGroupID)
{
	vector<OutlineElement> vOneComponentOutlineElm;

	for(int k = 1; k < theComponent.m_vOutlinePoint.size(); k++)
	{
		const PointInfo& vertexStart = theComponent.m_vOutlinePoint[k - 1];
		const PointInfo& vertexEnd = theComponent.m_vOutlinePoint[k];
		if(vertexStart.group == OUTLINE_GROUP_TYPE_END)
		{
			nOutlineGroupID++;
			continue;
		}

		OutlineElement outlineElm;
		outlineElm.m_nType = 1;
		if(GetLineType(vertexStart.sign) == STRAIGHT_LINE)
		{
			outlineElm.m_bIsArc = false;
			outlineElm.m_nGroupID = nOutlineGroupID;
			outlineElm.m_vPoint.push_back(FloatPoint(theComponent.m_x + vertexStart.x, theComponent.m_y + vertexStart.y));
			outlineElm.m_vPoint.push_back(FloatPoint(theComponent.m_x + vertexEnd.x, theComponent.m_y + vertexEnd.y));
		}
		else if(GetLineType(vertexStart.sign) == CIRCULAR_ARC)
		{
			float fArcMidPointX, fArcMidPointY;
			GetArcMidPoint(vertexStart.x, vertexStart.y, vertexEnd.x, vertexEnd.y, vertexEnd.r, vertexEnd.dir, fArcMidPointX, fArcMidPointY);

			outlineElm.m_bIsArc = true;
			outlineElm.m_nGroupID = nOutlineGroupID;
			outlineElm.m_vPoint.push_back(FloatPoint(theComponent.m_x + vertexStart.x, theComponent.m_y + vertexStart.y));
			outlineElm.m_vPoint.push_back(FloatPoint(theComponent.m_x + fArcMidPointX, theComponent.m_y + fArcMidPointY));
			outlineElm.m_vPoint.push_back(FloatPoint(theComponent.m_x + vertexEnd.x, theComponent.m_y + vertexEnd.y));
		}
		vOneComponentOutlineElm.push_back(outlineElm);
	}
	nOutlineGroupID++;

	return vOneComponentOutlineElm;
}
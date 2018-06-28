#include "StdAfx.h"
#include "SyntecProcess.h"

#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"
#include "../Work/MachineDef.h"
#include <algorithm>

typedef Component* PCOMPONENT;
CString SyntecGetSummaryFileName();
CString SyntecGetLabelFileName(int nPanelID);
void SyntecLabelOutput(vector<SyntecDataItem>& vSyntecData, UINT nCoordinateType, CString strOutputDir, LabelPastingSide eSide = LabelPastingSide::LABEL_UP_SIDE);
static bool sortFuncByComponentID(const PCOMPONENT& m1, const PCOMPONENT& m2) ;
void ConvertAllBmpToOneBitBmp(CString strOutputDir);
CString GetLargePanelImageName(CString strPanelImageName);
void CopyPanelImage(vector<SyntecDataItem>& vSyntecData, CString strOutputDir);
void RescalePanelImage(vector<SyntecDataItem>& vSyntecData, CString strOutputDir);

void SyntecSummaryOutput(vector<SyntecDataItem>& vSyntecData, UINT nCoordinateType, CString strOutputDir, LabelPastingSide eSide)
{
	USES_CONVERSION;

	SyntecLabelOutput(vSyntecData, nCoordinateType, strOutputDir, eSide);

	CString strOutputFileName;
	strOutputFileName = SyntecGetSummaryFileName();
	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");
	strOutputFileName = strOutputDir + strOutputFileName;

	TiXmlDocument * xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDecl = new TiXmlDeclaration(WStringToUTF8(L"1.0").c_str(), WStringToUTF8(L"utf-8").c_str(), WStringToUTF8(L"").c_str());
	xmlDoc->LinkEndChild(xmlDecl);

	TiXmlElement * elmRoot = new TiXmlElement(WStringToUTF8(L"CycleFile").c_str());
	xmlDoc->LinkEndChild(elmRoot);

	for(int i = 0; i < vSyntecData.size(); i++)
	{
		SyntecDataItem& item = vSyntecData[i];

		TiXmlElement * elmOnePanel = new TiXmlElement(WStringToUTF8(L"Cycle").c_str());
		elmRoot->LinkEndChild(elmOnePanel);
		elmOnePanel->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"Cycle_List").c_str());

		TiXmlElement * elmPlateID = new TiXmlElement(WStringToUTF8(L"Field").c_str());
		elmOnePanel->LinkEndChild(elmPlateID);
		elmPlateID->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"PlateID").c_str());
		elmPlateID->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(GetFileNameInPath_WithSuffix(item.m_strNCFilePath))).c_str());

		TiXmlElement * elmLabelName = new TiXmlElement(WStringToUTF8(L"Field").c_str());
		elmOnePanel->LinkEndChild(elmLabelName);
		elmLabelName->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"LabelName").c_str());
		elmLabelName->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(item.m_strLabelFileName)).c_str());

		TiXmlElement * elmLargeImage = new TiXmlElement(WStringToUTF8(L"Field").c_str());
		elmOnePanel->LinkEndChild(elmLargeImage);
		elmLargeImage->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"LargeImage").c_str());
		elmLargeImage->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(GetLargePanelImageName(item.m_strPanelImageName))).c_str());

		TiXmlElement * elmSmallImage = new TiXmlElement(WStringToUTF8(L"Field").c_str());
		elmOnePanel->LinkEndChild(elmSmallImage);
		elmSmallImage->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"SmallImage").c_str());
		elmSmallImage->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(item.m_strPanelImageName)).c_str());

		TiXmlElement * elmColor = new TiXmlElement(WStringToUTF8(L"Field").c_str());
		elmOnePanel->LinkEndChild(elmColor);
		elmColor->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"Color").c_str());
		elmColor->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(item.m_pPanel->m_Material)).c_str());

		TiXmlElement * elmThickness = new TiXmlElement(WStringToUTF8(L"Field").c_str());
		elmOnePanel->LinkEndChild(elmThickness);
		elmThickness->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"Thickness").c_str());
		elmThickness->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(GetFloatString(item.m_pPanel->m_Thickness, 1))).c_str());
	}

	xmlDoc->SaveFile(strOutputFileName);
	delete xmlDoc;
	xmlDoc = NULL;

	CopyPanelImage(vSyntecData, strOutputDir);
	RescalePanelImage(vSyntecData, strOutputDir);
	ConvertAllBmpToOneBitBmp(strOutputDir);
}

CString SyntecGetSummaryFileName()
{
	static int nSerialID = 0;
	nSerialID++;

	CString strRet;
	CTime timeCur(::time(NULL));
	strRet = timeCur.Format(_T("%Y%m%d%H%M%S"));
	CString strTmp;
	strTmp.Format(_T("_%03d"), nSerialID);
	strRet = _T("List_") + strRet + strTmp +  + _T(".xml");
	return strRet;
}

CString SyntecGetLabelFileName(int nPanelID)
{
	//static int nSerialID = 0;
	//nSerialID++;

	CString strRet;
	CTime timeCur(::time(NULL));
	strRet = timeCur.Format(_T("%Y%m%d%H%M%S"));
	CString strTmp;
	strTmp.Format(_T("_%03d"), /*nSerialID*/nPanelID+1);
	strRet = _T("Label_") + strRet + strTmp +  + _T(".cyc");
	return strRet;
}

void SyntecLabelOutput(vector<SyntecDataItem>& vSyntecData, UINT nCoordinateType, CString strOutputDir, LabelPastingSide eSide)
{
	USES_CONVERSION;

	//CoordinateHandler coordinateHandler(CoordinateHandler::PositionLRType::RIGHT, \
	//	CoordinateHandler::PositionFBType::FRONT, \
	//	CoordinateHandler::PositionTBType::TOP, \
	//	'Y', 'X', 'Z', \
	//	CoordinateHandler::PlusMinusType::PLUS, \
	//	CoordinateHandler::PlusMinusType::PLUS, \
	//	CoordinateHandler::PlusMinusType::PLUS);

	CoordinateHandler* pCoordinateHandler = g_IDToCoordinateMap.m_mapIDToCoordinate[nCoordinateType];

	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	for(int i = 0; i < vSyntecData.size(); i++)
	{
		SyntecDataItem& item = vSyntecData[i];

		CString strLabelFileName = SyntecGetLabelFileName(i);
		CString strLabelFilePath = strOutputDir + strLabelFileName;

		item.m_strLabelFileName = strLabelFileName;
		item.m_strPanelImageName = GetPanelImageName(i);

		TiXmlDocument * xmlDoc = new TiXmlDocument();

		TiXmlDeclaration * xmlDecl = new TiXmlDeclaration(WStringToUTF8(L"1.0").c_str(), WStringToUTF8(L"utf-8").c_str(), WStringToUTF8(L"").c_str());
		xmlDoc->LinkEndChild(xmlDecl);

		TiXmlElement * elmRoot = new TiXmlElement(WStringToUTF8(L"CycleFile").c_str());
		xmlDoc->LinkEndChild(elmRoot);

		vector<Component*> vAllComponent;
		FindAllComponentInPanel(*(item.m_pPanel), vAllComponent);
		std::sort(vAllComponent.begin(),vAllComponent.end(),sortFuncByComponentID);
		for(int j = 0; j < vAllComponent.size(); j++)
		{
			TiXmlElement * elmCycle = new TiXmlElement(WStringToUTF8(L"Cycle").c_str());
			elmRoot->LinkEndChild(elmCycle);
			elmCycle->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"Cycle_Label").c_str());

			TiXmlElement * elmLabelName = new TiXmlElement(WStringToUTF8(L"Field").c_str());
			elmCycle->LinkEndChild(elmLabelName);
			elmLabelName->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"LabelName").c_str());
			elmLabelName->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(GetLabelImageName(/*item.m_pPanel->m_PanelID*/i, vAllComponent[j]->m_NumberInPanel))).c_str());

			float fX_Converted, fY_Converted;
			float fX_Org = vAllComponent[j]->m_x+vAllComponent[j]->m_fXLabelCenter;
			float fY_Org = vAllComponent[j]->m_y+vAllComponent[j]->m_fYLabelCenter;

			// 2018-6-20 yuanzb 反面有信息是才使用反面坐标
			if(eSide == LabelPastingSide::LABEL_BACK_SIDE_H_FLIP && item.m_pPanel->HaveDownerFaceInfo() == true)
				fX_Org = item.m_pPanel->m_OrgLen - fX_Org;
			else if(eSide == LabelPastingSide::LABEL_BACK_SIDE_V_FLIP  && item.m_pPanel->HaveDownerFaceInfo() == true)
				fY_Org = item.m_pPanel->m_OrgWidth - fY_Org;

			pCoordinateHandler->GetConvertedCoordinate(item.m_pPanel->m_OrgLen, item.m_pPanel->m_OrgWidth, item.m_pPanel->m_Thickness, \
				fX_Org, fY_Org, 0, &fX_Converted, &fY_Converted, NULL);

			TiXmlElement * elmX = new TiXmlElement(WStringToUTF8(L"Field").c_str());
			elmCycle->LinkEndChild(elmX);
			elmX->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"X").c_str());
			elmX->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(GetFloatStringTrimming(fX_Converted, 1))).c_str());

			TiXmlElement * elmY = new TiXmlElement(WStringToUTF8(L"Field").c_str());
			elmCycle->LinkEndChild(elmY);
			elmY->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"Y").c_str());
			elmY->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(A2W(GetFloatStringTrimming(fY_Converted, 1))).c_str());

			TiXmlElement * elmR = new TiXmlElement(WStringToUTF8(L"Field").c_str());
			elmCycle->LinkEndChild(elmR);
			elmR->SetAttribute(WStringToUTF8(L"Name").c_str(), WStringToUTF8(L"R").c_str());
			elmR->SetAttribute(WStringToUTF8(L"Value").c_str(), WStringToUTF8(L"0").c_str());
		}

		xmlDoc->SaveFile(strLabelFilePath);
		delete xmlDoc;
		xmlDoc = NULL;
	}
}

bool sortFuncByComponentID(const PCOMPONENT& m1, const PCOMPONENT& m2)
{
	return m1->m_NumberInPanel < m2->m_NumberInPanel;
}

void ConvertAllBmpToOneBitBmp(CString strOutputDir)
{
	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	vector<CString> vAllBmpFilePath;

	CFileFind find;
	CString strFind = strOutputDir + _T("*.*");//遍历这一级全部的目录
	int nResult = find.FindFile(strFind);
	while(nResult)
	{
		nResult = find.FindNextFile();
		if(!find.IsDirectory() && !find.IsDots())
		{
			CString strFilePath = find.GetFilePath();
			if(strFilePath.Right(3).CompareNoCase(_T("bmp")) == 0)
				vAllBmpFilePath.push_back(strFilePath);
		}
	}

	if(vAllBmpFilePath.size() == 0)
	{
		AfxMessageBox("输出目录下没有板件标签！\n须要先导出标签然后导出NC文件！");
		return;
	}

	for(int i = 0; i < vAllBmpFilePath.size(); i++)
	{
		ConvertBmpToOneBitBmp(vAllBmpFilePath[i]);
	}
}

CString GetLargePanelImageName(CString strPanelImageName)
{
	CString strRet;
	int nPos = strPanelImageName.ReverseFind(_T('\\'));
	int nPos1 = strPanelImageName.ReverseFind(_T('.'));
	strRet = strPanelImageName.Left(nPos+1) + strPanelImageName.Mid(nPos+1, nPos1-nPos-1) + _T("_Large") + strPanelImageName.Right(strPanelImageName.GetLength()-nPos1);
	return strRet;
}

void CopyPanelImage(vector<SyntecDataItem>& vSyntecData, CString strOutputDir)
{
	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	for(int i = 0; i < vSyntecData.size(); i++)
	{
		CopyFile(strOutputDir + vSyntecData[i].m_strPanelImageName, strOutputDir + GetLargePanelImageName(vSyntecData[i].m_strPanelImageName), TRUE);
	}
}

void RescalePanelImage(vector<SyntecDataItem>& vSyntecData, CString strOutputDir)
{
	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	for(int i = 0; i < vSyntecData.size(); i++)
	{
		CString strPanelImageName = strOutputDir + vSyntecData[i].m_strPanelImageName;
		CString strPanelImageName_Large = strOutputDir + GetLargePanelImageName(vSyntecData[i].m_strPanelImageName); 
		RescaleBmp(strPanelImageName, 158, 107);
		RescaleBmp(strPanelImageName_Large, 246, 167);
	}
}
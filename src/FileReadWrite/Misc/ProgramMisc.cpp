#include "stdafx.h"
#include "ProgramMisc.h"
#include "HGTools.h"
#include "Misc.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "OtherShapeHelper.h"


//孔翻转
std::string ExchangeVerticalXY(std::string str_vertical/*, float fXLen*/, float fYLen)
{
	CString result;
	if (str_vertical.compare("") != 0)
	{
		CString vertical = HGTools::Convert2CString(str_vertical);

		CStringArray verticalArr;
		HGTools::SplitCString(vertical,verticalArr,';');
		for (int line_i = 0;line_i<verticalArr.GetSize();line_i++)
		{
			CString oneVertical = verticalArr.GetAt(line_i);
			if (oneVertical != "")
			{
				CStringArray vercitalData;
				HGTools::SplitCString(oneVertical,vercitalData,',');

				if (vercitalData.GetCount() >= 4)
				{
					//转换分解孔字符串，并转换X,Y坐标/////////////////////////////////////////////////////////////////////////////////
					float fOrgX = atof(vercitalData[0].GetBuffer());
					vercitalData[0].ReleaseBuffer();
					float fOrgY = atof(vercitalData[1].GetBuffer());
					vercitalData[1].ReleaseBuffer();
					fOrgY = fYLen - fOrgY;
					float fTmp = fOrgY;
					fOrgY = fOrgX;
					fOrgX = fTmp;
					fOrgY = fYLen - fOrgY;

					CString new_oneVertical;
					new_oneVertical.Format("%s,%s,%s,%s;",/*vercitalData[1]*/GetFloatString(fOrgX, 1),GetFloatString(fOrgY, 1),vercitalData[2],vercitalData[3]);
					result = result + new_oneVertical;
				}
			}
		}
	}
	return result.GetBuffer();
}

//槽旋转
std::string ExchangeSlotXY(std::string str_slot/*, float fXLen*/, float fYLen)
{
	CString result;
	if (str_slot.compare("") != 0)
	{
		CString slot = HGTools::Convert2CString(str_slot);

		CStringArray slotArr;
		HGTools::SplitCString(slot,slotArr,';');
		for (int line_i = 0;line_i<slotArr.GetSize();line_i++)
		{
			CString oneSlot = slotArr.GetAt(line_i);
			if (oneSlot != "")
			{
				CStringArray slotData;
				HGTools::SplitCString(oneSlot,slotData,',');

				if (slotData.GetCount() >= 6)
				{
					//转换分解槽字符串，并转换X,Y坐标，方向
					//CString newx = slotData[1];//原y
					//CString newy = slotData[0];//原x
					float fOrgX = atof(slotData[0].GetBuffer());
					slotData[0].ReleaseBuffer();
					float fOrgY = atof(slotData[1].GetBuffer());
					slotData[1].ReleaseBuffer();
					fOrgY = fYLen - fOrgY;
					float fTmp = fOrgY;
					fOrgY = fOrgX;
					fOrgX = fTmp;
					fOrgY = fYLen - fOrgY;

					CString dir = slotData[5];//原dir
					dir = (dir == "0") ? "1" : "0";	//旋转槽方向

					CString new_oneSolt;
					new_oneSolt.Format("%s,%s,%s,%s,%s,%s;",/*newx*/GetFloatString(fOrgX, 1),/*newy*/GetFloatString(fOrgY, 1),slotData[2],slotData[3],slotData[4],dir);
					result = result + new_oneSolt;
				}
			}
		}
	}
	return result.GetBuffer();
}

//板件翻转
//包括，槽翻转，孔翻转，异形翻转，封边翻转，异形字符串翻转
void  TurnOverVecSlot(std::map<std::string,std::string>& map_one_matrial)
{
	{//槽孔翻转
		std::string s_dvec = map_one_matrial["反面孔1"];
		std::string s_dsolt = map_one_matrial["反面槽1"];

		map_one_matrial["反面孔1"] = map_one_matrial["正面孔1"];
		map_one_matrial["反面槽1"] = map_one_matrial["正面槽1"];

		map_one_matrial["正面孔1"] = s_dvec;
		map_one_matrial["正面槽1"] = s_dsolt;

		if (s_dvec != "")
		{
			map_one_matrial["孔翻转"] = "1";
		}
		else
			map_one_matrial["孔翻转"] = "0";

		if (s_dsolt != "")
		{
			map_one_matrial["槽翻转"] = "1";
		}
		else
			map_one_matrial["槽翻转"] = "0";
	}

	//异形点翻转
	try
	{
		std::vector<PointInfo> pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(map_one_matrial["异形点信息"]);
		pointInfos = OtherShapeHelper::getPointInfoBack(pointInfos,map_one_matrial["开料长度"],map_one_matrial["开料宽度"]);
		map_one_matrial["异形点信息"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(pointInfos);
	}
	catch (std::logic_error* e)
	{
		CString mm;
		mm.Format("%s:%s",map_one_matrial["info"].c_str(),e->what());
		AfxMessageBox(mm);
	}
	catch (...)
	{
		AfxMessageBox("异形点翻转轮廓出错");
	}

	//封边信息翻转（上下）
	{
		std::string alldef = map_one_matrial["封边量"];
		std::string def1,def2,def3,def4;
		int def_num = 0;
		for (int d = 0;d < alldef.size();d++)
		{
			if (alldef.at(d) == ' ')
			{
				def_num++;
				continue;
			}
			else
			{
				if (def_num == 0)
				{
					def1.push_back(alldef.at(d));
				}
				else if (def_num == 1)
				{
					def2.push_back(alldef.at(d));
				}
				else if (def_num == 2)
				{
					def3.push_back(alldef.at(d));
				}
				else if (def_num == 3)
				{
					def4.push_back(alldef.at(d));
				}
			}
		}


		CString temp;
		temp.Format("%s %s %s %s",def1.c_str(),def4.c_str(),def3.c_str(),def2.c_str());
		map_one_matrial["封边量"] = temp.GetBuffer();
	}
}

//生成新的条码
CString newBarCode(int seed)
{
	CString newCode;
	CTime timersetting = CTime::GetCurrentTime();
	newCode.Format("%04d%02d%02d%02d%02d%02d%03d"
		,timersetting.GetYear()
		,timersetting.GetMonth()
		,timersetting.GetDay()
		,timersetting.GetHour()
		,timersetting.GetMinute()
		,timersetting.GetSecond()
		,seed);
	return newCode;
}
//获得唯一条码
CString uniqueBarCode(CString originBarCode,std::vector<CString>& barCodeCheck)
{
	int size = barCodeCheck.size();
	if (StrCmpC(originBarCode,"") == 0)
	{
		CString newCode = newBarCode(size);
		barCodeCheck.push_back(newCode);
		return newCode;
	}
	for (int i = 0 ; i < size ; i++)
	{
		CString cmp = barCodeCheck.at(i);
		if (StrCmpC(originBarCode,cmp) == 0)
		{
			CString newCode = newBarCode(size);
			barCodeCheck.push_back(newCode);
			return newCode;
		}
	}
	barCodeCheck.push_back(originBarCode);
	return originBarCode;
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

int GetLineLinkingTypeByLineType(LineType eFrontLineType, LineType eBackLineType)
{
	int nRet = 0;
	if(eBackLineType == LineType::CIRCULAR_ARC)
	{
		if(eFrontLineType == LineType::STRAIGHT_LINE)
			nRet = 1;
		else
			nRet = 3;
	}
	else
	{
		if(eFrontLineType == LineType::STRAIGHT_LINE)
			nRet = 0;
		else
			nRet = 2;
	}

	return nRet;
}
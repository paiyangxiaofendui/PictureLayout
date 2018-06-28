#pragma once
#include <string>
#include <map>
#include <vector>
#include "../Work/WorkDef.h"

using namespace std;

//槽旋转
string ExchangeSlotXY(string str_slot/*, float fXLen*/, float fYLen);

//孔翻转
string ExchangeVerticalXY(string str_vertical/*, float fXLen*/, float fYLen);

//板件翻转
//包括，槽翻转，孔翻转，异形翻转，封边翻转，异形字符串翻转
void TurnOverVecSlot(std::map<std::string,std::string>& map_one_matrial);

//生成新的条码
CString newBarCode(int seed);
//获得唯一条码
CString uniqueBarCode(CString originBarCode,std::vector<CString>& barCodeCheck);

LineType GetLineType(int nLineSign);

int GetLineLinkingTypeByLineType(LineType eFrontLineType, LineType eBackLineType);
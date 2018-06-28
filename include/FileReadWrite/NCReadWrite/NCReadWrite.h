#ifndef		_NCReadWrite_h_
#define		_NCReadWrite_h_
#pragma  once

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#include <vector>
#include <string>

class Panel;

using namespace std;

class AFX_EXT_CLASS  NCReadWrite
{
public:
	static bool OutputGcode(Panel* panel,				string CurCutFileName,			string CurPrintFileName,
							int cutprint,				/*bool duplex,					bool sys_offset,			int coortype,*/		int PanelNum, 
							string NextPrintFileName,	string NextCutFileName/*,			vector<P_Info> _pinfo,		string grouptype,*/ 
							/*string file_change,			string _mainktgcodeb,			string _mainktgcodee,*/
							/*string _vecktgcodeb,		string _vecktgcodee,			int _vecnochange,			bool KnifeOffset*/);

};

#endif
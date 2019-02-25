#include "StdAfx.h"
#include "YHInfoTools.h"


YHInfoTools::YHInfoTools(void)
{
}


//(用数字替换字母)例如：PW+PD-PK
string YHInfoTools::replace_allstr(string str,const string old_value,const string new_value)
{
	while(true)   
	{                                                                                                                                                              
		string::size_type pos(0);     
		if((pos=str.find(old_value))!=string::npos)
			str.replace(pos,old_value.length(),new_value);
		else
			break;
	}
	return str;  
}

//得到正面槽信息
void YHInfoTools::getAllSlot(std::vector<AllSlot>& rallslot,CString slotinfo,CString rotateinfo,CString panelinfo)
{
	std::string str_slotinfo = slotinfo.GetBuffer();

	int str_num = 0;
	std::string str_value = "";
	AllSlot oneslot;
	oneslot.m_bDisabled = false;
	for (int i = 0;i < str_slotinfo.size();i++)
	{
		char ch = str_slotinfo.at(i);
		if (ch == ',')
		{
			switch(str_num)
			{
			case 0:
				oneslot._x = atof(str_value.c_str());
				break;
			case 1:
				oneslot._y = atof(str_value.c_str());
				break;
			case 2:
				oneslot._width = atof(str_value.c_str());
				break;
			case 3:
				oneslot._height = atof(str_value.c_str());
				break;
			case 4:
				oneslot._depth = atof(str_value.c_str());
				break;
			case 5:
				oneslot._dir = atoi(str_value.c_str());
				break;
			default:
				break;
			}
			str_value = "";
			str_num++;
		}
		else if (ch == ';')
		{
			oneslot._dir = atoi(str_value.c_str());
			oneslot.panel_info = panelinfo.GetBuffer();
			rallslot.push_back(oneslot);
			str_num = 0;
			str_value="";
		}
		else
		{
			//str_value = str_value + ch;
			str_value.push_back(ch);
		}
	}
}

//得到正面孔信息
void YHInfoTools::getAllVec(std::vector<AllVec>& rallvec,CString vecinfo,CString panelinfo)
{
	std::string str_vec = vecinfo.GetBuffer();

	int str_num = 0;
	std::string str_value = "";
	AllVec onevec;
	onevec.m_bDisabled = false;
	for (int i = 0;i < str_vec.size();i++)
	{
		char ch = str_vec.at(i);
		if (ch == ',')
		{
			switch(str_num)
			{
			case 0:
				onevec._x = atof(str_value.c_str());
				break;
			case 1:
				onevec._y = atof(str_value.c_str());
				break;
			case 2:
				onevec._r = atof(str_value.c_str());
				break;
			case 3:
				onevec._depth = atof(str_value.c_str());
				break;
			default:
				break;
			}
			str_value = "";
			str_num++;
		}
		else if (ch == ';')
		{
			onevec._depth = atof(str_value.c_str());
			onevec.panel_info = panelinfo.GetBuffer();
			rallvec.push_back(onevec);
			str_num = 0;
			str_value="";
		}
		else
		{
			//str_value = str_value + ch;
			str_value.push_back(ch);
		}
	}
}

#include "StdAfx.h"
#include "YHInfoTools.h"
//#include "KnifeClass.h"
//#include "HGTools.h"
//#include "XmlHandlePlus.h"
//#include "CIniFile.h"
//
//#include "hgCst/Construction.h"
//#include "hg3d/XmlFile.h"
//#include "hg3d/Utils.h"
//#include "hg3d/hgtypes.h"
//#include "hg3d/XmlNode.h"

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
//
////翻转后的点位信息
//void YHInfoTools::getOtherAllPoint(std::vector<AllPoint>& otherallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width)
//{
//	std::vector<param_point> pa_p;
//	std::string str_info = pointinfo.GetBuffer(0);
//	std::string str = "";
//	std::string str_name;
//	std::string str_value;
//	for (int i = 0;i < str_info.size();i++)
//	{
//		char ch = str_info.at(i);
//		if(ch == ':')
//		{
//			str_name = str;
//			str = "";
//		}
//		else if (ch == ';')
//		{
//			str_value = str;
//			str = "";
//
//			if(rotateinfo == "0" || rotateinfo == "3")
//			{
//				if (str_name == "PW")
//					str_value = p_width;
//				if (str_name == "PD")
//					str_value = p_height;
//			}
//			else
//			{
//				if (str_name == "PD")
//					str_value = p_width;
//				if (str_name == "PW")
//					str_value = p_height;
//			}
//
//			param_point p;
//			p._name = str_name;
//			p._value = str_value;
//			pa_p.push_back(p);
//		}
//		else
//		{
//			str = str + ch;
//		}
//	}
//
//	bool havePW = false;
//	bool havePD = false;
//	for (int i = 0;i < pa_p.size();i++)
//	{
//		if (pa_p.at(i)._name == "PW")
//		{
//			havePW = true;
//		}
//		if (pa_p.at(i)._name == "PD")
//		{
//			havePD = true;
//		}
//	}
//	param_point pw_p,pd_p;
//	if(rotateinfo == "0" || rotateinfo == "3")
//	{
//		pw_p._name = "PW";
//		pw_p._value = p_width;
//		pd_p._name = "PD";
//		pd_p._value = p_height;
//	}
//	else
//	{
//		pw_p._name = "PW";
//		pw_p._value = p_height;
//		pd_p._name = "PD";
//		pd_p._value = p_width;
//	}
//	if (havePW == false)
//	{
//		pa_p.push_back(pw_p);
//	}
//	if (havePD == false)
//	{
//		pa_p.push_back(pd_p);
//	}
//
//	sortParam_pointByStrLen(pa_p);
//
//	std::string m_stypeid;
//	for (int i = 0;i < pa_p.size();i++)
//	{
//		std::string p_id = "ID";
//		if (strcmp(pa_p.at(i)._name.c_str(),p_id.c_str()) == 0)
//		{
//			m_stypeid = pa_p.at(i)._value;
//		}
//
//	}
//
//
//	//异形转换
//	{
//		CString hgmpath = HGTools::getRelativePath("kdata\\mapOutList.hgm");
//		CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);
//		HGTools::decryptFile(hgmpath,xmlpath);
//
//		CString strConfigFile1Hgm = hgmpath;
//		CString strConfigFile1Xml = xmlpath;
//		TinyXml::TiXmlDocument config1;
//		config1.LoadFile(strConfigFile1Xml);
//		TinyXml::XmlHandlePlus config1Handle(config1);
//		config1Handle.changeDirectoryTo("/PanelOutlineList/PanelOutline/",TinyXml::XmlAttrValueCompare("StyleId",m_stypeid));
//		m_stypeid = config1Handle.getAttr("OtherStyleId");
//		HGTools::deleteFile(strConfigFile1Xml);
//
//		for (int i = 1;i < pa_p.size();i++)
//		{
//			std::string pa_name1 = pa_p.at(i)._name;
//			for (int j = 0;j < i;j++)
//			{
//				std::string pa_name = pa_p.at(j)._name;
//				if (pa_name.length() < pa_name1.length())
//				{
//					std::swap(pa_p[i],pa_p[j]);
//				}
//			}
//		}
//	}
//
//	//异形点阵
//	{
//
//		CString hgmpath = HGTools::getRelativePath("kdata\\PanelOutlineList.hgm");
//		CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);
//		HGTools::decryptFile(hgmpath,xmlpath);
//
//		CString strConfigFile2Xml = xmlpath;
//		TinyXml::TiXmlDocument config2;
//		config2.LoadFile(strConfigFile2Xml);
//		TinyXml::XmlHandlePlus config2Handle(config2);
//		config2Handle.changeDirectoryTo("/PanelOutlineList/PanelOutline/",TinyXml::XmlAttrValueCompare("StyleId",m_stypeid));
//		std::vector<TinyXml::TiXmlNode*> vectorGroup = config2Handle.findAll("Vector/");
//
//		AllPoint allpoint;
//		allpoint.panel_info = panelinfo.GetBuffer(0);
//		for (int j = 0;j<vectorGroup.size();j++)
//		{
//			TinyXml::XmlHandlePlus child_node(vectorGroup.at(j));
//
//			PointInfo pos_i;
//
//			std::string str_x = child_node.getAttr("X");
//			std::string str_y = child_node.getAttr("Y");
//			std::string str_r = child_node.getAttr("R");
//			std::string str_sign = child_node.getAttr("Sign");
//			std::string str_dir = child_node.getAttr("Dir");
//			std::string str_cut = child_node.getAttr("Cut");
//			std::string str_group = child_node.getAttr("Group");
//			std::string str_side = child_node.getAttr("Side");
//
//			for (int k = 0;k < pa_p.size();k++)
//			{
//				std::string p_name = pa_p.at(k)._name;
//				std::string p_value = pa_p.at(k)._value;
//
//				str_x = replace_allstr(str_x,p_name,p_value);
//				str_y = replace_allstr(str_y,p_name,p_value);
//				str_r = replace_allstr(str_r,p_name,p_value);
//			}
//
//			hg3d::Construction cst;
//
//			if (rotateinfo == "1" || rotateinfo == "2")
//			{
//				float mid_x = cst.calcFormula(str_x);
//				float mid_y = cst.calcFormula(str_y);
//
//				float rotate_x = mid_y + atof(p_width.c_str());
//				float rotate_y = -1*mid_x;
//
//				pos_i.x = rotate_x;
//				pos_i.y = -1*rotate_y;
//				pos_i.r = cst.calcFormula(str_r);
//				pos_i.sign = atoi(str_sign.c_str());
//				pos_i.dir = atoi(str_dir.c_str());
//				pos_i.cut = atoi(str_cut.c_str());
//				pos_i.group = atoi(str_group.c_str());
//				pos_i.side = atoi(str_side.c_str());
//			}
//			else
//			{
//				pos_i.x = cst.calcFormula(str_x);
//				pos_i.y = -1*cst.calcFormula(str_y);
//				pos_i.r = cst.calcFormula(str_r);
//				pos_i.sign = atoi(str_sign.c_str());
//				pos_i.dir = atoi(str_dir.c_str());
//				pos_i.cut = atoi(str_cut.c_str());
//				pos_i.group = atoi(str_group.c_str());
//				pos_i.side = atoi(str_side.c_str());
//			}
//			//绕中心旋转
//			if (rotateinfo == "2" || rotateinfo == "3")
//			{
//				pos_i.x = atof(p_width.c_str()) - pos_i.x;
//				pos_i.y = atof(p_height.c_str()) - pos_i.y;
//			}
//
//			pos_i.x = atof(p_width.c_str()) - pos_i.x;
//			pos_i.y = atof(p_height.c_str()) - pos_i.y;
//
//			if (atoi(str_sign.c_str()) != -1)
//			{
//				allpoint.point_info.push_back(pos_i);
//			}
//
//		}
//		otherallpoint.push_back(allpoint);
//
//		HGTools::deleteFile(strConfigFile2Xml);
//
//	}
//}
//
////得到反面点位信息
//void YHInfoTools::getAllDPoint(std::vector<DAllPoint>& rdallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width,float _OtherKerfWidth)
//{
//	//
//	std::vector<param_point> pa_p;
//	std::string str_info = pointinfo.GetBuffer(0);
//	std::string str = "";
//	std::string str_name;
//	std::string str_value;
//	param_point theJF;//锯缝
//	theJF._name = "dis";
//	char tempValue[12];
//	sprintf(tempValue,"%f",_OtherKerfWidth);
//	theJF._value = tempValue;
//	pa_p.push_back(theJF);
//	for (int i = 0;i < str_info.size();i++)
//	{
//		char ch = str_info.at(i);
//		if(ch == ':')
//		{
//			str_name = str;
//			str = "";
//		}
//		else if (ch == ';')
//		{
//			str_value = str;
//			str = "";
//
//			if(rotateinfo == "0" || rotateinfo == "3")
//			{
//				if (str_name == "PW")
//					str_value = p_width;
//				if (str_name == "PD")
//					str_value = p_height;
//			}
//			else
//			{
//				if (str_name == "PD")
//					str_value = p_width;
//				if (str_name == "PW")
//					str_value = p_height;
//			}
//
//			param_point p;
//			p._name = str_name;
//			p._value = str_value;
//			pa_p.push_back(p);
//		}
//		else
//		{
//			str = str + ch;
//		}
//	}
//
//	bool havePW = false;
//	bool havePD = false;
//	for (int i = 0;i < pa_p.size();i++)
//	{
//		if (pa_p.at(i)._name == "PW")
//		{
//			havePW = true;
//		}
//		if (pa_p.at(i)._name == "PD")
//		{
//			havePD = true;
//		}
//	}
//	param_point pw_p,pd_p;
//	if(rotateinfo == "0" || rotateinfo == "3")
//	{
//		pw_p._name = "PW";
//		pw_p._value = p_width;
//		pd_p._name = "PD";
//		pd_p._value = p_height;
//	}
//	else
//	{
//		pw_p._name = "PW";
//		pw_p._value = p_height;
//		pd_p._name = "PD";
//		pd_p._value = p_width;
//	}
//	if (havePW == false)
//	{
//		pa_p.push_back(pw_p);
//	}
//	if (havePD == false)
//	{
//		pa_p.push_back(pd_p);
//	}
//
//	std::string m_stypeid;
//	for (int i = 0;i < pa_p.size();i++)
//	{
//		std::string p_id = "ID";
//		if (strcmp(pa_p.at(i)._name.c_str(),p_id.c_str()) == 0)
//		{
//			m_stypeid = pa_p.at(i)._value;
//		}
//
//	}
//
//	for (int i = 1;i < pa_p.size();i++)
//	{
//		std::string pa_name1 = pa_p.at(i)._name;
//		for (int j = 0;j < i;j++)
//		{
//			std::string pa_name = pa_p.at(j)._name;
//			if (pa_name.length() < pa_name1.length())
//			{
//				std::swap(pa_p[i],pa_p[j]);
//			}
//		}
//	}
//
//
//	YHInfoTools::sortParam_pointByStrLen(pa_p);
//
//	hg3d::TString strConfigFile = _T("kdata");
//	strConfigFile = hg3d::getFullFileName(strConfigFile);
//	strConfigFile += _T("\\DPanelOutlineList.xml");
//
//	hg3d::CXmlFile* doc = new hg3d::CXmlFile();
//	if (!doc->Load(strConfigFile.c_str()))
//	{
//		AfxMessageBox("读取DPanelOutlineList.xml失败",MB_OK);
//		return;
//	}
//
//	for (int i = 0;i < doc->GetChildNodes().size();i++)
//	{
//		hg3d::CXmlNode* child_node = doc->GetChildNodes().at(i);
//		std::string str_styleid = child_node->GetAttributeValue("StyleId");
//		if (strcmp(str_styleid.c_str(),m_stypeid.c_str()) == 0)
//		{
//			DAllPoint allpoint;
//			allpoint.panel_info = panelinfo.GetBuffer(0);
//			for (int j = 0;j < child_node->GetChildNodes().size();j++)
//			{
//				DPointInfo pos_i;
//
//				std::string str_x = child_node->GetChildNodes().at(j)->GetAttributeValue("X");
//				std::string str_y = child_node->GetChildNodes().at(j)->GetAttributeValue("Y");
//				std::string str_r = child_node->GetChildNodes().at(j)->GetAttributeValue("R");
//				std::string str_sign = child_node->GetChildNodes().at(j)->GetAttributeValue("Sign");
//				std::string str_dir = child_node->GetChildNodes().at(j)->GetAttributeValue("Dir");
//				std::string str_cut = child_node->GetChildNodes().at(j)->GetAttributeValue("Cut");
//				std::string str_group = child_node->GetChildNodes().at(j)->GetAttributeValue("Group");
//				std::string str_side = child_node->GetChildNodes().at(j)->GetAttributeValue("Side");
//				std::string str_oldpos = child_node->GetChildNodes().at(j)->GetAttributeValue("OldPos");
//				std::string str_newpos = child_node->GetChildNodes().at(j)->GetAttributeValue("NewPos");
//
//				for (int k = 0;k < pa_p.size();k++)
//				{
//					std::string p_name = pa_p.at(k)._name;
//					std::string p_value = pa_p.at(k)._value;
//
//					str_x = replace_allstr(str_x,p_name,p_value);
//					str_y = replace_allstr(str_y,p_name,p_value);
//					str_r = replace_allstr(str_r,p_name,p_value);
//				}
//
//				hg3d::Construction cst;
//
//				if (rotateinfo == "1" || rotateinfo == "2")
//				{
//					float mid_x = cst.calcFormula(str_x);
//					float mid_y = cst.calcFormula(str_y);
//
//					float rotate_x = mid_y + atof(p_width.c_str());
//					float rotate_y = -1*mid_x;
//
//					pos_i.x = rotate_x;
//					pos_i.y = -1*rotate_y;
//					pos_i.r = cst.calcFormula(str_r);
//					pos_i.sign = atoi(str_sign.c_str());
//					pos_i.dir = atoi(str_dir.c_str());
//					pos_i.cut = atoi(str_cut.c_str());
//					pos_i.group = atoi(str_group.c_str());
//					pos_i.side = atoi(str_side.c_str());
//					pos_i.posold = atoi(str_oldpos.c_str());
//					pos_i.posnew = atoi(str_newpos.c_str());
//				}
//				else
//				{
//					pos_i.x = cst.calcFormula(str_x);
//					pos_i.y = -1*cst.calcFormula(str_y);
//					pos_i.r = cst.calcFormula(str_r);
//					pos_i.sign = atoi(str_sign.c_str());
//					pos_i.dir = atoi(str_dir.c_str());
//					pos_i.cut = atoi(str_cut.c_str());
//					pos_i.group = atoi(str_group.c_str());
//					pos_i.side = atoi(str_side.c_str());
//					pos_i.posold = atoi(str_oldpos.c_str());
//					pos_i.posnew = atoi(str_newpos.c_str());
//				}
//				//绕中心旋转
//				if (rotateinfo == "2" || rotateinfo == "3")
//				{
//					pos_i.x = atof(p_width.c_str()) - pos_i.x;
//					pos_i.y = atof(p_height.c_str()) - pos_i.y;
//				}
//
//				if (atoi(str_sign.c_str()) != -1)
//				{
//					allpoint.point_info.push_back(pos_i);
//				}
//			}
//			rdallpoint.push_back(allpoint);
//		}
//	}
//}
//
////得到点位信息
//void YHInfoTools::getAllPoint(std::vector<AllPoint>& rallpoint,CString pointinfo,CString rotateinfo,CString panelinfo,std::string p_height,std::string p_width)
//{
//	//外轮廓，使用异形string 分割然后加入旋转、板件长宽，算出异形外轮廓
//	//结果集合rallpoint保存的是后面用于加工的异形内容，打开后是板件ID和对应的异形
//	std::vector<param_point> pa_p;
//	std::string str_info = pointinfo.GetBuffer(0);
//	std::string str = "";
//	std::string str_name;
//	std::string str_value;
//	for (int i = 0;i < str_info.size();i++)
//	{
//		char ch = str_info.at(i);
//		if(ch == ':')
//		{
//			str_name = str;
//			str = "";
//		}
//		else if (ch == ';')
//		{
//			str_value = str;
//			str = "";
//
//			if(rotateinfo == "0" || rotateinfo == "3")
//			{
//				if (str_name == "PW")
//					str_value = p_width;
//				if (str_name == "PD")
//					str_value = p_height;
//			}
//			else
//			{
//				if (str_name == "PD")
//					str_value = p_width;
//				if (str_name == "PW")
//					str_value = p_height;
//			}
//
//			param_point p;
//			p._name = str_name;
//			p._value = str_value;
//			pa_p.push_back(p);
//		}
//		else
//		{
//			str = str + ch;
//		}
//	}
//
//	bool havePW = false;
//	bool havePD = false;
//	for (int i = 0;i < pa_p.size();i++)
//	{
//		if (pa_p.at(i)._name == "PW")
//		{
//			havePW = true;
//		}
//		if (pa_p.at(i)._name == "PD")
//		{
//			havePD = true;
//		}
//	}
//	param_point pw_p,pd_p;
//	if(rotateinfo == "0" || rotateinfo == "3")
//	{
//		pw_p._name = "PW";
//		pw_p._value = p_width;
//		pd_p._name = "PD";
//		pd_p._value = p_height;
//	}
//	else
//	{
//		pw_p._name = "PW";
//		pw_p._value = p_height;
//		pd_p._name = "PD";
//		pd_p._value = p_width;
//	}
//	if (havePW == false)
//	{
//		pa_p.push_back(pw_p);
//	}
//	if (havePD == false)
//	{
//		pa_p.push_back(pd_p);
//	}
//
//	std::string m_stypeid;
//	for (int i = 0;i < pa_p.size();i++)
//	{
//		std::string p_id = "ID";
//		if (strcmp(pa_p.at(i)._name.c_str(),p_id.c_str()) == 0)
//		{
//			m_stypeid = pa_p.at(i)._value;
//		}
//
//	}
//
//	for (int i = 1;i < pa_p.size();i++)
//	{
//		std::string pa_name1 = pa_p.at(i)._name;
//		for (int j = 0;j < i;j++)
//		{
//			std::string pa_name = pa_p.at(j)._name;
//			if (pa_name.length() < pa_name1.length())
//			{
//				std::swap(pa_p[i],pa_p[j]);
//			}
//		}
//	}
//
//
//	YHInfoTools::sortParam_pointByStrLen(pa_p);
//
//	hg3d::TString strConfigFile = _T("kdata");
//	strConfigFile = hg3d::getFullFileName(strConfigFile);
//	strConfigFile += _T("\\PanelOutlineList.xml");
//
//	hg3d::CXmlFile* doc = new hg3d::CXmlFile();
//	if (!doc->Load(strConfigFile.c_str()))
//	{
//		AfxMessageBox("读取TextureLib.xml失败",MB_OK);
//		return;
//	}
//
//	for (int i = 0;i < doc->GetChildNodes().size();i++)
//	{
//		hg3d::CXmlNode* child_node = doc->GetChildNodes().at(i);
//		std::string str_styleid = child_node->GetAttributeValue("StyleId");
//		if (strcmp(str_styleid.c_str(),m_stypeid.c_str()) == 0)
//		{
//			AllPoint allpoint;
//			allpoint.panel_info = panelinfo.GetBuffer(0);
//			for (int j = 0;j < child_node->GetChildNodes().size();j++)
//			{
//				PointInfo pos_i;
//
//				std::string str_x = child_node->GetChildNodes().at(j)->GetAttributeValue("X");
//				std::string str_y = child_node->GetChildNodes().at(j)->GetAttributeValue("Y");
//				std::string str_r = child_node->GetChildNodes().at(j)->GetAttributeValue("R");
//				std::string str_sign = child_node->GetChildNodes().at(j)->GetAttributeValue("Sign");
//				std::string str_dir = child_node->GetChildNodes().at(j)->GetAttributeValue("Dir");
//				std::string str_cut = child_node->GetChildNodes().at(j)->GetAttributeValue("Cut");
//				std::string str_group = child_node->GetChildNodes().at(j)->GetAttributeValue("Group");
//				std::string str_side = child_node->GetChildNodes().at(j)->GetAttributeValue("Side");
//
//				for (int k = 0;k < pa_p.size();k++)
//				{
//					std::string p_name = pa_p.at(k)._name;
//					std::string p_value = pa_p.at(k)._value;
//
//					str_x = replace_allstr(str_x,p_name,p_value);
//					str_y = replace_allstr(str_y,p_name,p_value);
//					str_r = replace_allstr(str_r,p_name,p_value);
//				}
//
//				hg3d::Construction cst;
//
//				if (rotateinfo == "1" || rotateinfo == "2")
//				{
//					float mid_x = cst.calcFormula(str_x);
//					float mid_y = cst.calcFormula(str_y);
//
//					float rotate_x = mid_y + atof(p_width.c_str());
//					float rotate_y = -1*mid_x;
//
//					pos_i.x = rotate_x;
//					pos_i.y = -1*rotate_y;
//					pos_i.r = cst.calcFormula(str_r);
//					pos_i.sign = atoi(str_sign.c_str());
//					pos_i.dir = atoi(str_dir.c_str());
//					pos_i.cut = atoi(str_cut.c_str());
//					pos_i.group = atoi(str_group.c_str());
//					pos_i.side = atoi(str_side.c_str());
//				}
//				else
//				{
//					pos_i.x = cst.calcFormula(str_x);
//					pos_i.y = -1*cst.calcFormula(str_y);
//					pos_i.r = cst.calcFormula(str_r);
//					pos_i.sign = atoi(str_sign.c_str());
//					pos_i.dir = atoi(str_dir.c_str());
//					pos_i.cut = atoi(str_cut.c_str());
//					pos_i.group = atoi(str_group.c_str());
//					pos_i.side = atoi(str_side.c_str());
//				}
//				//绕中心旋转
//				if (rotateinfo == "2" || rotateinfo == "3")
//				{
//					pos_i.x = atof(p_width.c_str()) - pos_i.x;
//					pos_i.y = atof(p_height.c_str()) - pos_i.y;
//				}
//
//				if (atoi(str_sign.c_str()) != -1)
//				{
//					allpoint.point_info.push_back(pos_i);
//				}
//			}
//			rallpoint.push_back(allpoint);
//		}
//	}
//
//}
//
////得到反面槽信息函数
//void YHInfoTools::getdAllSlot(std::vector<AllSlot>& rdallslot,CString slotinfo,CString rotateinfo,CString panelinfo)
//{
//	std::string str_slotinfo = slotinfo.GetBuffer();
//
//	int str_num = 0;
//	std::string str_value = "";
//	AllSlot oneslot;
//	for (int i = 0;i < str_slotinfo.size();i++)
//	{
//		char ch = str_slotinfo.at(i);
//		if (ch == ',')
//		{
//			switch(str_num)
//			{
//			case 0:
//				oneslot._x = atof(str_value.c_str());
//				break;
//			case 1:
//				oneslot._y = atof(str_value.c_str());
//				break;
//			case 2:
//				oneslot._width = atof(str_value.c_str());
//				break;
//			case 3:
//				oneslot._height = atof(str_value.c_str());
//				break;
//			case 4:
//				oneslot._depth = atof(str_value.c_str());
//				break;
//			case 5:
//				oneslot._dir = atoi(str_value.c_str());
//				break;
//			default:
//				break;
//			}
//			str_value = "";
//			str_num++;
//		}
//		else if (ch == ';')
//		{
//			oneslot._dir = atoi(str_value.c_str());
//			oneslot.panel_info = panelinfo.GetBuffer();
//			rdallslot.push_back(oneslot);
//			str_num = 0;
//			str_value="";
//		}
//		else
//		{
//			str_value = str_value + ch;
//		}
//	}
//}
//
////得到反面孔信息
//void YHInfoTools::getdAllVec(std::vector<AllVec>& rdallvec,CString vecinfo,CString panelinfo)
//{
//	std::string str_vec = vecinfo.GetBuffer();
//
//	int str_num = 0;
//	std::string str_value = "";
//	AllVec onevec;
//	for (int i = 0;i < str_vec.size();i++)
//	{
//		char ch = str_vec.at(i);
//		if (ch == ',')
//		{
//			switch(str_num)
//			{
//			case 0:
//				onevec._x = atof(str_value.c_str());
//				break;
//			case 1:
//				onevec._y = atof(str_value.c_str());
//				break;
//			case 2:
//				onevec._r = atof(str_value.c_str());
//				break;
//			case 3:
//				onevec._depth = atof(str_value.c_str());
//				break;
//			default:
//				break;
//			}
//			str_value = "";
//			str_num++;
//		}
//		else if (ch == ';')
//		{
//			onevec._depth = atof(str_value.c_str());
//			onevec.panel_info = panelinfo.GetBuffer();
//			rdallvec.push_back(onevec);
//			str_num = 0;
//			str_value="";
//		}
//		else
//		{
//			str_value = str_value + ch;
//		}
//	}
//}

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

////获得材料（厚度-材质）中的厚度
//float YHInfoTools::GetThinFromThinMatrial(std::string thin_matrial)
//{
//	std::string str_thin = "";
//	std::string str_matrial = "";
//	int key = 0;
//	for (int s = 0;s < thin_matrial.size();s++)
//	{
//		char c_thin = thin_matrial.at(s);
//		if (c_thin == '-')
//		{
//			key++;
//			continue;
//		}
//		else
//		{
//			if (key == 0)
//			{
//				str_thin = str_thin + c_thin;	//去除重复后获得各个板件的厚度
//			}
//			else
//			{
//				//str_matrial = str_matrial + c_thin;	//去除重复后获得各个板件的材质
//			}
//		}
//	}
//
//	return atof(str_thin.c_str());;
//}
//
////获得材料（厚度-材质）中的材质
//std::string YHInfoTools::GetMatrialFromThinMatrial(std::string thin_matrial)
//{
//	std::string str_thin = "";
//	std::string str_matrial = "";
//	int key = 0;
//	for (int s = 0;s < thin_matrial.size();s++)
//	{
//		char c_thin = thin_matrial.at(s);
//		if (c_thin == '-')
//		{
//			key++;
//			continue;
//		}
//		else
//		{
//			if (key == 0)
//			{
//				//str_thin = str_thin + c_thin;	//去除重复后获得各个板件的厚度
//			}
//			else
//			{
//				str_matrial = str_matrial + c_thin;	//去除重复后获得各个板件的材质
//			}
//		}
//	}
//	return str_matrial;
//}
//
////获得优化参数
//OptimizeParam YHInfoTools::GetOptimizParamFromFile()
//{
//	OptimizeParam oparam;
//	CString hgmpath = HGTools::getRelativePath("kdata\\Op.hgm");
//	CString xmlpath = HGTools::getXmlPathFromHgxPath(hgmpath);
//	HGTools::decryptFile(hgmpath,xmlpath);
//
//	TinyXml::TiXmlDocument opDoc(xmlpath.GetBuffer());
//	opDoc.LoadFile();
//
//	TinyXml::XmlHandlePlus docHandle(&opDoc);
//	docHandle.changeDirectoryTo("Root");
//	docHandle.setParamElementName("Param");
//
//	oparam.minrate = atof(docHandle.getParam("MinRate").c_str());
//	oparam._knife_type = atof(docHandle.getParam("KnifeType").c_str());
//	oparam.minwidth = atof(docHandle.getParam("MinW").c_str());
//	oparam.minheight = atof(docHandle.getParam("MinD").c_str());
//	oparam.minsize = atof(docHandle.getParam("MinSize").c_str());
//
//
//	HGTools::deleteFile(xmlpath);
//
//	return oparam;
//}
//
//
//
////根据长度整理参数的排序函数
//bool YHInfoTools::sort_baseStrLen(const param_point & m1, const param_point & m2) 
//{
//	return m1._name.length() > m2._name.length();
//}
//
////根据长度整理参数的排序函数, 参数按照长度，长到短
//void YHInfoTools::sortParam_pointByStrLen(std::vector<param_point> & pa_p) 
//{
//	std::sort(pa_p.begin(),pa_p.end(),YHInfoTools::sort_baseStrLen);
//}
//
////根据传入数组，清理所有条码中存在的# %加数字
//void YHInfoTools::BarCodeFixer(std::vector<KnifeClass *> &_knifeclasses)
//{
//
//}

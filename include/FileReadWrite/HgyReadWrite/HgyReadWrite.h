#ifndef		_HgyReadWrite_h_
#define		_HgyReadWrite_h_
#pragma  once

#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#include <vector>
#include <string>

class Component;

using namespace std;
using namespace TinyXml;

class AFX_EXT_CLASS  HgyReadWrite
{
public:
	//	标准读写函数
	static bool ReadHgy(CString szFileDir);										// 加载hgy
	static bool WriteHgy(CString szFileDir);									// 保存为hgy
	static bool SaveOneTree2Xml(TiXmlElement* pCurNode, Component* pCurCpn);	// 保存大板树形结构到XML文件
	static bool LoadOneTreeFromXml(TiXmlElement* pCurNode, Component* pCurCpn);	// 从XML文件读取大板树型结构

	// 保存板件异形、孔槽数据

	static void SaveOutline(TiXmlElement* pCpnElem, Component* pCpn);			// 保存板件异形数据
	

	// 读取板件异形、孔槽数据
	static void ReadOutlineHoleSlotInfo(TiXmlElement* pCurCpnNode, Component* pNewCpn);// 读取板件异形、孔槽数据
	static void ReadOutline(TiXmlElement* pCpnElem, Component* pCpn);			// 读取板件异形数据
	





};

#endif
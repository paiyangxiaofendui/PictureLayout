#include <stdafx.h>
#include "RemainderLibraryReadWrite.h"
#include <string>
#include <sstream>

using namespace std;

#include "../Misc/HGTools.h"
#include "../Misc/Misc.h"
#include "../Misc/EncryptionInterface.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/RemainderGroup/RemainderGroup.h"
#include "../../../include/DataManager/BaseDataType/RemainderItem/RemainderItem.h"
#include "../../../include/DataManager/BaseDataType/RemainderManager/RemainderManager.h"


bool RemainderLibraryReadWrite::Read(char* file_name)
{

	CString strSrcPath(file_name);
	//加密文件读取标准
	CString XmlPath = HGTools::getXmlPathFromHgxPath(strSrcPath);

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	decrypt_base64(strSrcPath.GetBuffer(), XmlPath.GetBuffer());

#else

	//HGTools::decryptFile(strSrcPath, XmlPath);

#endif


	// 加载文件
	TiXmlDocument* m_xmlDoc = new TiXmlDocument();
	if (m_xmlDoc->LoadFile(XmlPath) == false)
	{
		return false;
	}
	
	// 判断
	TiXmlElement* pRoot = m_xmlDoc->RootElement();
	if (pRoot == NULL)
	{
		return false;
	}

	CSingleon* pSingleton = CSingleon::GetSingleton();
	RemainderManager& RmdMgr = pSingleton->m_RemainderManager;

	// 清空原有的余料
	pSingleton->ClearRemainderManager();

	// 循环读取group
	for (TiXmlElement * pCurGroupNode = pRoot->FirstChildElement(); pCurGroupNode != NULL; pCurGroupNode = (TiXmlElement*)(pCurGroupNode->NextSibling()))
	{
		for (TiXmlElement * pCurItemNode = pCurGroupNode->FirstChildElement(); pCurItemNode != NULL; pCurItemNode = (TiXmlElement*)(pCurItemNode->NextSibling()))
		{
			RemainderItem* pRmd = new RemainderItem();

			pRmd->m_GroupID		=	stoi(pCurItemNode->Attribute("GroupID"));
			pRmd->m_ItemID		=	stoi(pCurItemNode->Attribute("ItemID"));
			pRmd->m_Material	=		 pCurItemNode->Attribute("Material");
			pRmd->m_Thickness	=	stof(pCurItemNode->Attribute("Thickness"));
			pRmd->m_Length		=	stof(pCurItemNode->Attribute("Length"));
			pRmd->m_Width		=	stof(pCurItemNode->Attribute("Width"));
			pRmd->m_nCount		=	stoi(pCurItemNode->Attribute("Count"));

			pSingleton->m_RemainderManager.AddRemainderItem(pRmd);
		}
	}

	return true;
}


bool RemainderLibraryReadWrite::Write(char* file_name)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();
	RemainderManager& RmdMgr = pSingleton->m_RemainderManager;

	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 


	// 余料库类
	TiXmlElement* pRMElement = new TiXmlElement("Root");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pRMElement);


	// 余料库
	vector<RemainderGroup*>& RmdGroupList = RmdMgr.m_RemainderGroupList;
	int nGroupNum = RmdGroupList.size();

	for (int i_group = 0; i_group < nGroupNum; i_group++)
	{
		RemainderGroup* pCurGroup = RmdGroupList.at(i_group);
		TiXmlElement* pGroupElement = new TiXmlElement("Group");

		// 转换
		string strID, strThickness;
		std::stringstream ss ;

		ss << pCurGroup->m_GroupID<< " "  << pCurGroup->m_Thickness;
		ss >> strID >> strThickness;

		pGroupElement->SetAttribute("GroupID",		strID.c_str());
		pGroupElement->SetAttribute("Material",		pCurGroup->m_Material.GetBuffer());
		pGroupElement->SetAttribute("Thickness",	strThickness.c_str());
		pRMElement->LinkEndChild(pGroupElement);

		// 每个group中有多个item
		vector<RemainderItem*>& RmdItemList = pCurGroup->m_RemainderItemList;
		int nItemNum = RmdItemList.size();

		for (int i_item = 0; i_item < nItemNum; i_item++)
		{
			RemainderItem* pCurItem = RmdItemList.at(i_item);
			TiXmlElement* pItemElement = new TiXmlElement("Item");

			string strGroupID, strItemID, strThickness, strLen, strWidth, strCount;
			std::stringstream ss ;

			ss << pCurItem->m_GroupID << " "  << pCurItem->m_ItemID << " " << pCurItem->m_Thickness<< " " << pCurItem->m_Length<< " " << pCurItem->m_Width<< " " << pCurItem->m_nCount ;
			ss >> strGroupID >>  strItemID >> strThickness >>  strLen >>  strWidth >>  strCount;

			pItemElement->SetAttribute("GroupID",		strGroupID.c_str());
			pItemElement->SetAttribute("ItemID",		strItemID.c_str());
			pItemElement->SetAttribute("Material",		pCurItem->m_Material.GetBuffer());
			pItemElement->SetAttribute("Thickness",		strThickness.c_str());
			pItemElement->SetAttribute("Length",		strLen.c_str());
			pItemElement->SetAttribute("Width",			strWidth.c_str());
			pItemElement->SetAttribute("Count",			strCount.c_str());

			pGroupElement->LinkEndChild(pItemElement);
		}
	}


	CString szFileDir(file_name);
	CString xml_path = szFileDir.Left(szFileDir.GetLength() - 4) + ".xml";

	m_xmlDoc->SaveFile(xml_path);
	delete m_xmlDoc;

#if (NEW_ENCRYPT_BASE64 == 1)


	//从hgx文件目录生成xml文件目录
	CString hge_path = szFileDir;

	encrypt_base64(xml_path.GetBuffer(), hge_path.GetBuffer());
	DeleteFile(xml_path);


#endif





	return true;
}

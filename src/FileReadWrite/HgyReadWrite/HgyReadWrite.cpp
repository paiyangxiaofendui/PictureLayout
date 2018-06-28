#include "stdafx.h"

#include "HgyReadWrite.h"
#include "../Misc/HGTools.h"
#include "../Misc/Misc.h"

//#include "../../../include/FileCompressandUnCompress/FileCompressandUnCompress.h"


#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../Misc/EncryptionInterface.h"


/*---------------------------------------*/
//	函数说明：
//		保存为hgy,特定给CSharp调用的临时函数
//	
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
bool HgyReadWrite::WriteHgy_CSharp(CString szFileDir)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();


	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 


	// 单例类
	//TiXmlElement* pSingletonElement = new TiXmlElement("Singleton");
	//TiXmlElement* pSingletonElement = new TiXmlElement("单例类");
	TiXmlElement* pSingletonElement = new TiXmlElement("Root");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pSingletonElement);


	// 解决方案
	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
	int nSlnNum = SlnList.size();

	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
	{
		CSolution* pCurSln = SlnList.at(i_sln);

		//TiXmlElement* pSolutionElement = new TiXmlElement("Solution");
		//TiXmlElement* pSolutionElement = new TiXmlElement("解决方案");
		TiXmlElement* pSolutionElement = new TiXmlElement("Panel");

		pSolutionElement->SetAttribute("Material",		pCurSln->m_strMaterial.GetBuffer());
		pSolutionElement->SetAttribute("Thickness",		pCurSln->m_fThickness);
		pSolutionElement->SetAttribute("Count",			pCurSln->GetPanelNum());

		pSingletonElement->LinkEndChild(pSolutionElement);
	}

	m_xmlDoc->SaveFile(szFileDir);
	delete m_xmlDoc;

	return true;
}


/*---------------------------------------*/
//	函数说明：
//
//
//
//	参数：
//
//
//
//
//	返回值:
//
//
/*---------------------------------------*/
bool  HgyReadWrite::WriteHgy(CString szFileDir)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();


	TiXmlDocument* m_xmlDoc = new TiXmlDocument();

	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "GB2312", "yes"); 


	// 单例类
	//TiXmlElement* pSingletonElement = new TiXmlElement("Singleton");
	TiXmlElement* pSingletonElement = new TiXmlElement("单例类");

	m_xmlDoc->LinkEndChild(xmlDec);
	m_xmlDoc->LinkEndChild(pSingletonElement);



	// 输入小板
	vector<ComponentInputItem>& InputItem = pSingleton->m_vBackupComponentInputItem;

	//TiXmlElement* pInputItemElement = new TiXmlElement("InputItem");
	TiXmlElement* pInputItemElement = new TiXmlElement("输入小板");

	pInputItemElement->SetAttribute("Line",	InputItem.size());
	pSingletonElement->LinkEndChild(pInputItemElement);


	for (vector<ComponentInputItem>::iterator i_item = InputItem.begin(); i_item != InputItem.end(); i_item++)
	{
		ComponentInputItem& item = *i_item;

		TiXmlElement* pItemElement = new TiXmlElement("原始小板");

		pItemElement->SetAttribute("OrderID",				item.m_strOrderID);							// 订单号
		pItemElement->SetAttribute("CabinetID",				item.m_strCabinetID);						// 柜号
		pItemElement->SetAttribute("Barcode",				item.m_strBarcode);							// 条码
		pItemElement->SetAttribute("CabinetName",			item.m_strCabinetName);						// 柜体名
		pItemElement->SetAttribute("PanelName",				item.m_strPanelName);						// 板件名
		pItemElement->SetAttribute("Material",				item.m_strMaterial);						// 材质
		pItemElement->SetAttribute("Length",				GetFloatString(item.m_fLength,1));			// 开料长
		pItemElement->SetAttribute("Width",					GetFloatString(item.m_fWidth,1));				// 开料宽
		pItemElement->SetAttribute("Thickness",				GetFloatString(item.m_fThickness,1));			// 厚度
		pItemElement->SetAttribute("Count",					item.m_nCount);								// 需切数量
		pItemElement->SetAttribute("Texture",				item.m_strTexture);							// 纹理

		//封边		
		pItemElement->SetAttribute("Banding0",				GetFloatString(item.m_afBanding[0],1));	
		pItemElement->SetAttribute("Banding1",				GetFloatString(item.m_afBanding[1],1));	
		pItemElement->SetAttribute("Banding2",				GetFloatString(item.m_afBanding[2],1));	
		pItemElement->SetAttribute("Banding3",				GetFloatString(item.m_afBanding[3],1));	

		pItemElement->SetAttribute("OtherShapeIDAndParam",	item.m_strOtherShapeIDAndParam);			// 异形参数
		pItemElement->SetAttribute("RotateType",			item.m_eRotateType);						// 旋转
		pItemElement->SetAttribute("FrontSlotInfo",			item.m_strUpperFaceSlotInfo);				// 正面槽信息
		pItemElement->SetAttribute("SlotFlipped",			item.m_bSlotFlipped);						// 槽翻转
		pItemElement->SetAttribute("BackSlotInfo",			item.m_strDownerFaceSlotInfo);				// 反面槽信息
		pItemElement->SetAttribute("FrontHoleInfo",			item.m_strUpperFaceHoleInfo);				// 正面孔信息
		pItemElement->SetAttribute("VHoleFlipped",			item.m_bVHoleFlipped);						// 孔翻转
		pItemElement->SetAttribute("BackHoleInfo",			item.m_strDownerFaceHoleInfo);				// 反面孔信息
		pItemElement->SetAttribute("CustomerInfo",			item.m_strCustomerInfo);					// 客户信息
		pItemElement->SetAttribute("JoinedStore",			item.m_strJoinedStore);						// 加盟店
		pItemElement->SetAttribute("SlottingFlag",			item.m_strSlottingFlag);					// 拉槽标识
		pItemElement->SetAttribute("Reminder",				item.m_strReminder);						// 助记号
		pItemElement->SetAttribute("Drilling",				item.m_strDrilling);						// 钻孔
		pItemElement->SetAttribute("OrderType",				item.m_strOrderType);						// 订单类型
		pItemElement->SetAttribute("ReverseSideBarcode",	item.m_strReverseSideBarcode);				// 反面条码
		pItemElement->SetAttribute("ProductLength",			GetFloatString(item.m_fProductLength,1));						// 成品长度
		pItemElement->SetAttribute("ProductWidth",			GetFloatString(item.m_fProductWidth,1));						// 成品宽度
		pItemElement->SetAttribute("ProductThickness",		GetFloatString(item.m_fProductThickness,1));					// 成品厚度
		pItemElement->SetAttribute("OtherShapeID",			item.m_strOtherShapeID);					// 异形ID
		pItemElement->SetAttribute("CustomerAddress",		item.m_strCustomerAddress);					// 客户地址
		pItemElement->SetAttribute("HoleSlotFlag",			item.m_strHoleSlotFlag);					// 钻槽标识
		pItemElement->SetAttribute("AbnormalInfo",			item.m_strAbnormalInfo);					// 异形点信息



		pInputItemElement->LinkEndChild(pItemElement);


	}





	


	// 解决方案
	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
	int nSlnNum = SlnList.size();

	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
	{
		CSolution* pCurSln = SlnList.at(i_sln);

		//TiXmlElement* pSolutionElement = new TiXmlElement("Solution");
		TiXmlElement* pSolutionElement = new TiXmlElement("解决方案");

		pSolutionElement->SetAttribute("SlnId",			pCurSln->m_SlnID);
		pSolutionElement->SetAttribute("PanelLength",	GetFloatStringTrimming(pCurSln->m_BaseInfo.m_PanelLength, 1));
		pSolutionElement->SetAttribute("PanelWidth",	GetFloatStringTrimming(pCurSln->m_BaseInfo.m_PanelWidth, 1));
		pSolutionElement->SetAttribute("SawKerfWidth",	GetFloatStringTrimming(pCurSln->m_BaseInfo.m_SawKerfWidth, 1));
		pSolutionElement->SetAttribute("DeburringWidth",GetFloatStringTrimming(pCurSln->m_BaseInfo.m_DeburringWidth, 1));
		pSolutionElement->SetAttribute("Material",		pCurSln->m_strMaterial.GetBuffer());
		pSolutionElement->SetAttribute("Thickness",		GetFloatStringTrimming(pCurSln->m_fThickness, 1));
		pSingletonElement->LinkEndChild(pSolutionElement);

		int nPanelNum = pCurSln->GetPanelNum();
		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
		{
			Panel* pCurPanel = pCurSln->GetPanel(i_panel);

			//TiXmlElement* pPanelElement = new TiXmlElement("Panel");
			TiXmlElement* pPanelElement = new TiXmlElement("大板");

			// panel的信息
			pPanelElement->SetAttribute("PanelId",		pCurPanel->m_PanelID);
			pPanelElement->SetAttribute("OrgLen",		GetFloatString(pCurPanel->m_OrgLen, 1));
			pPanelElement->SetAttribute("OrgWidth",		GetFloatString(pCurPanel->m_OrgWidth, 1));
			pPanelElement->SetAttribute("OriginX",		GetFloatString(pCurPanel->m_OriginX, 1));
			pPanelElement->SetAttribute("OriginY",		GetFloatString(pCurPanel->m_OriginY, 1));

			// component的信息
			pPanelElement->SetAttribute("x",			GetFloatString(pCurPanel->m_x, 1));
			pPanelElement->SetAttribute("y",			GetFloatString(pCurPanel->m_y, 1));
			pPanelElement->SetAttribute("RealLength",	GetFloatString(pCurPanel->m_RealLength, 1));
			pPanelElement->SetAttribute("RealWidth",	GetFloatString(pCurPanel->m_RealWidth, 1));
			pPanelElement->SetAttribute("Thickness",	GetFloatString(pCurPanel->m_Thickness, 1));
			pPanelElement->SetAttribute("Material",		pCurPanel->m_Material);
			pPanelElement->SetAttribute("type",			pCurPanel->m_type);


			pSolutionElement->LinkEndChild(pPanelElement);

			// 插入属性结构
			SaveOneTree2Xml(pPanelElement, pCurPanel);

		}
	}


	CString xml_path = szFileDir.Left(szFileDir.GetLength() - 4) + ".xml";

	m_xmlDoc->SaveFile(xml_path);
	delete m_xmlDoc;

#if (NEW_ENCRYPT_BASE64 == 1)


	//从hgx文件目录生成xml文件目录
	CString hgo_path = szFileDir;

	encrypt_base64(xml_path.GetBuffer(), hgo_path.GetBuffer());
	DeleteFile(xml_path);


#endif



	return true;
}







// 保存大板树形结构到XML文件
bool HgyReadWrite::SaveOneTree2Xml(TiXmlElement* pCurNode, Component* pCurCpn)
{
	if (pCurCpn == NULL)
	{
		return false;
	}

	int i, nCount;
	
	nCount = pCurCpn->m_ChildrenList.size();
	for (i = 0; i < nCount; i++)
	{
		TiXmlElement* pCpnElem = NULL;
		Component* pChildCpn = static_cast<Component*>(pCurCpn->m_ChildrenList.at(i));
		int cur_cpn_type = pChildCpn->m_type;


		// 判断板件类型
		switch(cur_cpn_type)
		{
		case NodeType_Remainder:
			//pElem = new TiXmlElement("Remainder");
			pCpnElem = new TiXmlElement("余料");
			break;
		case NodeType_NeededComponent:
			//pElem = new TiXmlElement("Component");
			pCpnElem = new TiXmlElement("小板");
			break;
		case NodeType_CombinePanel:
			//pElem = new TiXmlElement("CombinePanel");
			pCpnElem = new TiXmlElement("组合板");
			break;
		default:
			pCpnElem = new TiXmlElement("Error");
			break;
		}

		pCpnElem->SetAttribute("type",				pChildCpn->m_type);						// 0：余料（子节点）  1：要开的小板（子节点）   2：组合板（父节点）
		pCpnElem->SetAttribute("NumberInPanel",		pChildCpn->m_NumberInPanel);			// 板内序号 第几块排的小板
		pCpnElem->SetAttribute("Barcode",			pChildCpn->m_BarCode);					// 条码

		pCpnElem->SetAttribute("CabinetName",		pChildCpn->m_strCabinetName);			// 柜体名称
		pCpnElem->SetAttribute("ComponentName",		pChildCpn->m_strComponentName);			// 板件名称
		pCpnElem->SetAttribute("Material",			pChildCpn->m_Material);					// 材料
		pCpnElem->SetAttribute("Texture",			pChildCpn->m_Texture);					// 纹理	0：无纹理  1：横纹 2：竖纹

		pCpnElem->SetAttribute("Thickness",			GetFloatString(pChildCpn->m_Thickness, 1));				// 厚度
		pCpnElem->SetAttribute("RealLength",		GetFloatString(pChildCpn->m_RealLength, 1));				// 真实长
		pCpnElem->SetAttribute("RealWidth",			GetFloatString(pChildCpn->m_RealWidth, 1));				// 真实宽
		pCpnElem->SetAttribute("x",					GetFloatString(pChildCpn->m_x, 1));						// x坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
		pCpnElem->SetAttribute("y",					GetFloatString(pChildCpn->m_y, 1));						// y坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
		pCpnElem->SetAttribute("RotatedAngle",		pChildCpn->m_nRotatedAngle);			// 旋转标识	
		pCpnElem->SetAttribute("OtherShapeID",		pChildCpn->m_nOtherShapeID);			// 异形ID
		pCpnElem->SetAttribute("KnifeDownPos",		pChildCpn->m_nKnifeDownPos);			// 下刀点

		pCpnElem->SetAttribute("OrderID",			pChildCpn->m_strOrderID);				// 订单号
		pCpnElem->SetAttribute("CabinetID",			pChildCpn->m_strCabinetID);				// 柜号

		pCpnElem->SetAttribute("Banding0",			GetFloatString(pChildCpn->m_afBanding[0], 1));				// 封边  
		pCpnElem->SetAttribute("Banding1",			GetFloatString(pChildCpn->m_afBanding[1], 1));
		pCpnElem->SetAttribute("Banding2",			GetFloatString(pChildCpn->m_afBanding[2], 1));
		pCpnElem->SetAttribute("Banding3",			GetFloatString(pChildCpn->m_afBanding[3], 1));

		pCpnElem->SetAttribute("SlotFlipped",		pChildCpn->m_bSlotFlipped);				// 槽翻转
		pCpnElem->SetAttribute("VHoleFlipped",		pChildCpn->m_bVHoleFlipped);			// 孔翻转
		pCpnElem->SetAttribute("CustomerInfo",		pChildCpn->m_strCustomerInfo);			// 客户信息
		pCpnElem->SetAttribute("JoinedStore",		pChildCpn->m_strJoinedStore);			// 加盟店
		pCpnElem->SetAttribute("SlottingFlag",		pChildCpn->m_strSlottingFlag);			// 拉槽标识
		pCpnElem->SetAttribute("Reminder",			pChildCpn->m_strReminder);				// 助记号
		pCpnElem->SetAttribute("Drilling",			pChildCpn->m_strDrilling);				// 钻孔
		pCpnElem->SetAttribute("OrderType",			pChildCpn->m_strOrderType);				// 订单类型
		pCpnElem->SetAttribute("ReverseSideBarcode",pChildCpn->m_strReverseSideBarcode);	// 反面条码
		pCpnElem->SetAttribute("ProductLength",		GetFloatString(pChildCpn->m_fProductLength, 1));			// 成品长度
		pCpnElem->SetAttribute("ProductWidth",		GetFloatString(pChildCpn->m_fProductWidth, 1));			// 成品宽度
		pCpnElem->SetAttribute("ProductThickness",	GetFloatString(pChildCpn->m_fProductThickness, 1));		// 成品厚度
		pCpnElem->SetAttribute("CustomerAddress",	pChildCpn->m_strCustomerAddress);		// 客户地址
		pCpnElem->SetAttribute("HoleSlotFlag",		pChildCpn->m_strHoleSlotFlag);			// 钻槽标识


		if (cur_cpn_type == NodeType_NeededComponent)
		{
			// 保存轮廓点
			SaveOutline(pCpnElem, pChildCpn);

			// 保存正面孔
			SaveUpperFaceHole(pCpnElem, pChildCpn);

			// 保存正面槽
			SaveUpperFaceSlot(pCpnElem, pChildCpn);

			// 保存反面孔
			SaveDownerFaceHole(pCpnElem, pChildCpn);

			// 保存反面槽
			SaveDownerFaceSlot(pCpnElem, pChildCpn);
		}


		// 板件节点插入父板件节点
		pCurNode->LinkEndChild(pCpnElem);

		// 递归插入
		if (pChildCpn->m_ChildrenList.size() > 0)
		{
			SaveOneTree2Xml(pCpnElem, pChildCpn);
		}
	}





	return true;
}

/*---------------------------------------*/
//	函数说明：
//		读取板件异形、孔槽数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadOutlineHoleSlotInfo(TiXmlElement* pCurCpnNode, Component* pNewCpn)
{

	for (TiXmlElement * pCurInfoNode = pCurCpnNode->FirstChildElement(); pCurInfoNode != NULL; pCurInfoNode = (TiXmlElement*)(pCurInfoNode->NextSibling()))
	{
		string node_name = pCurInfoNode->Value();

		if(node_name == "轮廓点")
		{
			// 读取异形点信息
			ReadOutline(pCurInfoNode, pNewCpn);
		}
		else if (node_name == "正面孔")
		{
			// 读取正面孔 
			ReadUpperFaceHole(pCurInfoNode, pNewCpn);

		}
		else if (node_name == "正面槽")
		{
			// 读取正面槽
			ReadUpperFaceSlot(pCurInfoNode, pNewCpn);

		}
		else if (node_name == "反面孔")
		{
			// 读取反面孔
			ReadDownerFaceHole(pCurInfoNode, pNewCpn);
		}
		else if (node_name == "反面槽")
		{
			// 读取反面槽
			ReadDownerFaceSlot(pCurInfoNode, pNewCpn);
		}
		else
		{
			AfxMessageBox("数据错误！");
		}
	}

}


/*---------------------------------------*/
//	函数说明：
//		读取板件异形数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadOutline(TiXmlElement* pCpnElem, Component* pCpn)
{
	// 先清空原有的
	pCpn->m_vOutlinePoint.clear();

	// 循环读取轮廓点信息
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		PointInfo pnt;

		pnt.x		= _ttof(pInfoNode->Attribute("X"	));	
		pnt.y		= _ttof(pInfoNode->Attribute("Y"	));	
		pnt.r		= _ttof(pInfoNode->Attribute("R"	));	
		pnt.sign	= _ttoi(pInfoNode->Attribute("Sign"	));
		pnt.dir		= _ttoi(pInfoNode->Attribute("Dir"	));	
		pnt.cut		= _ttoi(pInfoNode->Attribute("Cut"	));	
		pnt.group	= _ttoi(pInfoNode->Attribute("Group"));
		pnt.side	= _ttoi(pInfoNode->Attribute("Side"	));
		pnt.type	= _ttoi(pInfoNode->Attribute("type"	));

		pCpn->m_vOutlinePoint.push_back(pnt);
	}
}

/*---------------------------------------*/
//	函数说明：
//		读取板件正面孔数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadUpperFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	// 先清空原有的
	pCpn->m_vUpperFaceHole.clear();

	// 循环读取轮廓点信息
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllVec hole;

		hole._x			=	_ttof(pInfoNode->Attribute("X"		));	
		hole._y			=	_ttof(pInfoNode->Attribute("Y"		));	
		hole._r			=	_ttof(pInfoNode->Attribute("R"		));	
		hole._depth		=	_ttof(pInfoNode->Attribute("Depth"	));
		hole.panel_info	=	pInfoNode->Attribute("Info"	);

		pCpn->m_vUpperFaceHole.push_back(hole);
	}
}

/*---------------------------------------*/
//	函数说明：
//		读取板件正面槽数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadUpperFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	// 先清空原有的
	pCpn->m_vUpperFaceSlot.clear();

	// 循环读取轮廓点信息
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllSlot slot;

		slot._x			= _ttof(pInfoNode->Attribute("X"		));	
		slot._y			= _ttof(pInfoNode->Attribute("Y"		));	
		slot._width		= _ttof(pInfoNode->Attribute("Width"	));	
		slot._height	= _ttof(pInfoNode->Attribute("Height"	));	
		slot._depth		= _ttof(pInfoNode->Attribute("Depth"	));
		slot._dir		= _ttoi(pInfoNode->Attribute("Dir"		));
		slot.panel_info	= pInfoNode->Attribute("Info"		);	

		pCpn->m_vUpperFaceSlot.push_back(slot);
	}
}


/*---------------------------------------*/
//	函数说明：
//		读取板件反面孔数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadDownerFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	// 先清空原有的
	pCpn->m_vDownerFaceHole.clear();

	// 循环读取轮廓点信息
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllVec hole;

		hole._x			=	_ttof(pInfoNode->Attribute("X"		));	
		hole._y			=	_ttof(pInfoNode->Attribute("Y"		));	
		hole._r			=	_ttof(pInfoNode->Attribute("R"		));	
		hole._depth		=	_ttof(pInfoNode->Attribute("Depth"	));
		hole.panel_info	=	pInfoNode->Attribute("Info"	);

		pCpn->m_vDownerFaceHole.push_back(hole);
	}
}


/*---------------------------------------*/
//	函数说明：
//		读取板件正面槽数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::ReadDownerFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	// 先清空原有的
	pCpn->m_vDownerFaceSlot.clear();

	// 循环读取轮廓点信息
	for (TiXmlElement * pInfoNode = pCpnElem->FirstChildElement(); pInfoNode != NULL; pInfoNode = (TiXmlElement*)(pInfoNode->NextSibling()))
	{
		AllSlot slot;

		slot._x			= _ttof(pInfoNode->Attribute("X"		));	
		slot._y			= _ttof(pInfoNode->Attribute("Y"		));	
		slot._width		= _ttof(pInfoNode->Attribute("Width"	));	
		slot._height	= _ttof(pInfoNode->Attribute("Height"	));	
		slot._depth		= _ttof(pInfoNode->Attribute("Depth"	));
		slot._dir		= _ttoi(pInfoNode->Attribute("Dir"		));
		slot.panel_info	= pInfoNode->Attribute("Info"		);	

		pCpn->m_vDownerFaceSlot.push_back(slot);
	}
}

/*---------------------------------------*/
//	函数说明：
//		保存板件异形数据
//
//
//	参数：
//		TiXmlElement* pCpnElem	--	板件XML节点
//		Component* pCpn			--	板件指针
//
//
//	返回值:
//
//
/*---------------------------------------*/
void HgyReadWrite::SaveOutline(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nOutlineCount = pCpn->m_vOutlinePoint.size();
	if (nOutlineCount > 0)
	{
		// 新建一个Outline节点
		//TiXmlElement* pOutlineElem = new TiXmlElement("Outline");
		TiXmlElement* pOutlineElem = new TiXmlElement("轮廓点");
		pCpnElem->LinkEndChild(pOutlineElem);

		// 新建一个个的轮廓点节点插入pOutlineElem中
		for (int i_outline = 0; i_outline < nOutlineCount; i_outline++)
		{
			PointInfo pnt = pCpn->m_vOutlinePoint.at(i_outline);
			//TiXmlElement* pVectorNode = new TiXmlElement("PointInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("点信息");

			pVectorNode->SetAttribute("X",		GetFloatString(pnt.x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(pnt.y,1));	
			pVectorNode->SetAttribute("R",		GetFloatString(pnt.r,1));	
			pVectorNode->SetAttribute("Sign",	pnt.sign);
			pVectorNode->SetAttribute("Dir",	pnt.dir);	
			pVectorNode->SetAttribute("Cut",	pnt.cut);	
			pVectorNode->SetAttribute("Group",	pnt.group);
			pVectorNode->SetAttribute("Side",	pnt.side);
			pVectorNode->SetAttribute("type",	pnt.type);

			pOutlineElem->LinkEndChild(pVectorNode);
		}
	}
}




// 保存板件正面孔数据
void HgyReadWrite::SaveUpperFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nUpperFaceHoleCount = pCpn->m_vUpperFaceHole.size();
	if (nUpperFaceHoleCount > 0)
	{
		// 新建一个Outline节点
		//TiXmlElement* pUpperFaceHoleElem = new TiXmlElement("UpperFaceHole");
		TiXmlElement* pUpperFaceHoleElem = new TiXmlElement("正面孔");
		pCpnElem->LinkEndChild(pUpperFaceHoleElem);

		// 新建一个个的轮廓点节点插入pOutlineElem中
		for (int i_hole = 0; i_hole < nUpperFaceHoleCount; i_hole++)
		{
			AllVec hole = pCpn->m_vUpperFaceHole.at(i_hole);
			//TiXmlElement* pVectorNode = new TiXmlElement("UpperFaceHoleInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("正面孔信息");

			pVectorNode->SetAttribute("X",		GetFloatString(hole._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(hole._y,1));	
			pVectorNode->SetAttribute("R",		GetFloatString(hole._r,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(hole._depth,1));
			pVectorNode->SetAttribute("Info",	hole.panel_info);	

			pUpperFaceHoleElem->LinkEndChild(pVectorNode);
		}
	}
}	

// 保存板件正面槽数据
void HgyReadWrite::SaveUpperFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nUpperFaceSlotCount = pCpn->m_vUpperFaceSlot.size();
	if (nUpperFaceSlotCount > 0)
	{
		// 新建一个Outline节点
		//TiXmlElement* pUpperFaceSlotElem = new TiXmlElement("UpperFaceSlot");
		TiXmlElement* pUpperFaceSlotElem = new TiXmlElement("正面槽");
		pCpnElem->LinkEndChild(pUpperFaceSlotElem);

		// 新建一个个的轮廓点节点插入pOutlineElem中
		for (int i_slot = 0; i_slot < nUpperFaceSlotCount; i_slot++)
		{
			AllSlot slot = pCpn->m_vUpperFaceSlot.at(i_slot);
			//TiXmlElement* pVectorNode = new TiXmlElement("UpperFaceSlotInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("正面槽信息");

			pVectorNode->SetAttribute("X",		GetFloatString(slot._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(slot._y,1));	
			pVectorNode->SetAttribute("Width",	GetFloatString(slot._width,1));	
			pVectorNode->SetAttribute("Height",	GetFloatString(slot._height,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(slot._depth,1));
			pVectorNode->SetAttribute("Dir",	slot._dir);
			pVectorNode->SetAttribute("Info",	slot.panel_info);	

			pUpperFaceSlotElem->LinkEndChild(pVectorNode);
		}
	}
}		

// 保存板件反面孔数据
void HgyReadWrite::SaveDownerFaceHole(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nDownerFaceHoleCount = pCpn->m_vDownerFaceHole.size();
	if (nDownerFaceHoleCount > 0)
	{
		// 新建一个Outline节点
		//TiXmlElement* pDownerFaceHoleElem = new TiXmlElement("DownerFaceHole");
		TiXmlElement* pDownerFaceHoleElem = new TiXmlElement("反面孔");
		pCpnElem->LinkEndChild(pDownerFaceHoleElem);

		// 新建一个个的轮廓点节点插入pOutlineElem中
		for (int i_hole = 0; i_hole < nDownerFaceHoleCount; i_hole++)
		{
			AllVec hole = pCpn->m_vDownerFaceHole.at(i_hole);
			//TiXmlElement* pVectorNode = new TiXmlElement("UpperFaceHoleInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("反面孔信息");

			pVectorNode->SetAttribute("X",		GetFloatString(hole._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(hole._y,1));	
			pVectorNode->SetAttribute("R",		GetFloatString(hole._r,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(hole._depth,1));
			pVectorNode->SetAttribute("Info",	hole.panel_info);	

			pDownerFaceHoleElem->LinkEndChild(pVectorNode);
		}
	}
}	

// 保存板件反面槽数据
void HgyReadWrite::SaveDownerFaceSlot(TiXmlElement* pCpnElem, Component* pCpn)
{
	int nDownerFaceSlotCount = pCpn->m_vDownerFaceSlot.size();
	if (nDownerFaceSlotCount > 0)
	{
		// 新建一个Outline节点
		//TiXmlElement* pUpperFaceSlotElem = new TiXmlElement("DownerFaceSlot");
		TiXmlElement* pDownerFaceSlotElem = new TiXmlElement("反面槽");
		pCpnElem->LinkEndChild(pDownerFaceSlotElem);

		// 新建一个个的轮廓点节点插入pOutlineElem中
		for (int i_slot = 0; i_slot < nDownerFaceSlotCount; i_slot++)
		{
			AllSlot slot = pCpn->m_vDownerFaceSlot.at(i_slot);
			//TiXmlElement* pVectorNode = new TiXmlElement("DownerFaceSlotInfo");
			TiXmlElement* pVectorNode = new TiXmlElement("反面槽信息");

			pVectorNode->SetAttribute("X",		GetFloatString(slot._x,1));	
			pVectorNode->SetAttribute("Y",		GetFloatString(slot._y,1));	
			pVectorNode->SetAttribute("Width",	GetFloatString(slot._width,1));	
			pVectorNode->SetAttribute("Height",	GetFloatString(slot._height,1));	
			pVectorNode->SetAttribute("Depth",	GetFloatString(slot._depth,1));
			pVectorNode->SetAttribute("Dir",	slot._dir);
			pVectorNode->SetAttribute("Info",	slot.panel_info);	

			pDownerFaceSlotElem->LinkEndChild(pVectorNode);
		}
	}
}	







// 从XML文件读取大板树型结构
bool HgyReadWrite::LoadOneTreeFromXml(TiXmlElement* pCurNode, Component* pParentCpn)
{
	if (pParentCpn == NULL)
	{
		return false;
	}

	for (TiXmlElement * pCurCpnNode = pCurNode->FirstChildElement(); pCurCpnNode != NULL; pCurCpnNode = (TiXmlElement*)(pCurCpnNode->NextSibling()))
	{
		string bar_code;
		string node_name = pCurCpnNode->Value();
		Component* pNewCpn = new Component;

		pNewCpn->m_type								= _ttoi(pCurCpnNode->Attribute("type"));				// 0：余料（子节点）  1：要开的小板（子节点）   2：组合板（父节点）
		pNewCpn->m_NumberInPanel					= _ttoi(pCurCpnNode->Attribute("NumberInPanel"));		// 板内序号 第几块排的小板
		pNewCpn->m_BarCode							= pCurCpnNode->Attribute("Barcode");

		pNewCpn->m_strCabinetName					= pCurCpnNode->Attribute("CabinetName");				// 柜体名称
		pNewCpn->m_strComponentName					= pCurCpnNode->Attribute("ComponentName");				// 板件名称
		pNewCpn->m_Material							= pCurCpnNode->Attribute("Material");					// 材料
		pNewCpn->m_Texture							= _ttoi(pCurCpnNode->Attribute("Texture"));				// 纹理	0：无纹理  1：横纹 2：竖纹

		pNewCpn->m_Thickness						= _ttof(pCurCpnNode->Attribute("Thickness"));			// 厚度
		pNewCpn->m_RealLength						= _ttof(pCurCpnNode->Attribute("RealLength"));			// 真实长
		pNewCpn->m_RealWidth						= _ttof(pCurCpnNode->Attribute("RealWidth"));			// 真实宽
		pNewCpn->m_x								= _ttof(pCurCpnNode->Attribute("x"));					// x坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
		pNewCpn->m_y								= _ttof(pCurCpnNode->Attribute("y"));					// y坐标		相对大板的原点，直角坐标系，第一象限，左下角为原点
		pNewCpn->m_nRotatedAngle					= _ttoi(pCurCpnNode->Attribute("RotatedAngle"));		// 旋转标识
		pNewCpn->m_nOtherShapeID					= _ttoi(pCurCpnNode->Attribute("OtherShapeID"));		// 异形ID
		pNewCpn->m_nKnifeDownPos					= _ttoi(pCurCpnNode->Attribute("KnifeDownPos"));		// 下刀点

		pNewCpn->m_strOrderID						= pCurCpnNode->Attribute("OrderID");					// 订单号
		pNewCpn->m_strCabinetID						= pCurCpnNode->Attribute("CabinetID");					// 柜号

		pNewCpn->m_afBanding[0]						= _ttof(pCurCpnNode->Attribute("Banding0"));			// 封边 
		pNewCpn->m_afBanding[1]						= _ttof(pCurCpnNode->Attribute("Banding1"));			// 封边 
		pNewCpn->m_afBanding[2]						= _ttof(pCurCpnNode->Attribute("Banding2"));			// 封边 
		pNewCpn->m_afBanding[3]						= _ttof(pCurCpnNode->Attribute("Banding3"));			// 封边 	 

		pNewCpn->m_bSlotFlipped						= _ttoi(pCurCpnNode->Attribute("SlotFlipped"));			// 槽翻转
		pNewCpn->m_bVHoleFlipped					= _ttoi(pCurCpnNode->Attribute("VHoleFlipped"));		// 孔翻转
		pNewCpn->m_strCustomerInfo					= pCurCpnNode->Attribute("CustomerInfo");				// 客户信息
		pNewCpn->m_strJoinedStore					= pCurCpnNode->Attribute("JoinedStore");				// 加盟店
		pNewCpn->m_strSlottingFlag					= pCurCpnNode->Attribute("SlottingFlag");				// 拉槽标识
		pNewCpn->m_strReminder						= pCurCpnNode->Attribute("Reminder");					// 助记号
		pNewCpn->m_strDrilling						= pCurCpnNode->Attribute("Drilling");					// 钻孔
		pNewCpn->m_strOrderType						= pCurCpnNode->Attribute("OrderType");					// 订单类型
		pNewCpn->m_strReverseSideBarcode			= pCurCpnNode->Attribute("ReverseSideBarcode");			// 反面条码
		pNewCpn->m_fProductLength					= _ttof(pCurCpnNode->Attribute("ProductLength"));		// 成品长度
		pNewCpn->m_fProductWidth					= _ttof(pCurCpnNode->Attribute("ProductWidth"));		// 成品宽度
		pNewCpn->m_fProductThickness				= _ttof(pCurCpnNode->Attribute("ProductThickness"));	// 成品厚度
		pNewCpn->m_strCustomerAddress				= pCurCpnNode->Attribute("CustomerAddress");			// 客户地址
		pNewCpn->m_strHoleSlotFlag					= pCurCpnNode->Attribute("HoleSlotFlag");				// 钻槽标识







		// 插入父节点
		pParentCpn->AddChild(pNewCpn);

		// 小板不递归读取
		if (node_name == "小板")
		{
			// 读取轮廓、孔槽信息
			ReadOutlineHoleSlotInfo(pCurCpnNode, pNewCpn);
		}
		else
		{
			// 其他类型的节点递归读取
			LoadOneTreeFromXml(pCurCpnNode, pNewCpn);
		}
	}

	return true;
}



bool  HgyReadWrite::ReadHgy(CString strSrcPath)
{
	CSingleon* pSingleton = CSingleon::GetSingleton();

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
	m_xmlDoc->LoadFile(XmlPath);

	TiXmlElement* pRoot = m_xmlDoc->RootElement();

	// 判断
	if (pRoot == NULL)
	{
		return false;
	}

	string strValue =  pRoot->Value();
	if (strValue != "单例类")
	{
		return false;
	}

	for (TiXmlElement * pCurNode = pRoot->FirstChildElement(); pCurNode != NULL; pCurNode = (TiXmlElement*)(pCurNode->NextSibling()))
	{
		string strCurNodeName =  pCurNode->Value();
		
		if (strCurNodeName == "输入小板")
		{
			vector<ComponentInputItem>  vInputCpn;

			for (TiXmlElement * pInputItem = pCurNode->FirstChildElement(); pInputItem != NULL; pInputItem = (TiXmlElement*)(pInputItem->NextSibling()))
			{
				ComponentInputItem item;

				item.m_strOrderID				=		pInputItem->Attribute("OrderID"				)		;	// 订单号
				item.m_strCabinetID				=		pInputItem->Attribute("CabinetID"			)		;	// 柜号
				item.m_strBarcode				=		pInputItem->Attribute("Barcode"				)		;	// 条码
				item.m_strCabinetName			=		pInputItem->Attribute("CabinetName"			)		;	// 柜体名
				item.m_strPanelName				=		pInputItem->Attribute("PanelName"			)		;	// 板件名
				item.m_strMaterial				=		pInputItem->Attribute("Material"			)		;	// 材质
				item.m_fLength					= _ttof(pInputItem->Attribute("Length")				)		;	// 开料长
				item.m_fWidth					= _ttof(pInputItem->Attribute("Width")				)		;	// 开料宽
				item.m_fThickness				= _ttof(pInputItem->Attribute("Thickness")			)		;	// 厚度
				item.m_nCount					= _ttoi(pInputItem->Attribute("Count")				)		;	// 需切数量
				item.m_strTexture				=		pInputItem->Attribute("Texture"				)		;	// 纹理

				//封边
				item.m_afBanding[0]				= _ttof(pInputItem->Attribute("Banding0")			)		;	
				item.m_afBanding[1]				= _ttof(pInputItem->Attribute("Banding1")			)		;	
				item.m_afBanding[2]				= _ttof(pInputItem->Attribute("Banding2")			)		;	
				item.m_afBanding[3]				= _ttof(pInputItem->Attribute("Banding3")			)		;	

				item.m_strOtherShapeIDAndParam	=		pInputItem->Attribute("OtherShapeIDAndParam")		;	// 异形参数
				item.m_eRotateType				= _ttoi(pInputItem->Attribute("RotateType")			)		;	// 旋转
				item.m_strUpperFaceSlotInfo		=		pInputItem->Attribute("FrontSlotInfo"		)		;	// 正面槽信息
				item.m_bSlotFlipped				= _ttoi(pInputItem->Attribute("SlotFlipped")		)		;	// 槽翻转
				item.m_strDownerFaceSlotInfo 	=		pInputItem->Attribute("BackSlotInfo"		)		;	// 反面槽信息
				item.m_strUpperFaceHoleInfo		=		pInputItem->Attribute("FrontHoleInfo"		)		;	// 正面孔信息
				item.m_bVHoleFlipped			= _ttoi(pInputItem->Attribute("VHoleFlipped")		)		;	// 孔翻转
				item.m_strDownerFaceHoleInfo 	=		pInputItem->Attribute("BackHoleInfo"		)		;	// 反面孔信息
				item.m_strCustomerInfo		 	=		pInputItem->Attribute("CustomerInfo"		)		;	// 客户信息
				item.m_strJoinedStore		 	=		pInputItem->Attribute("JoinedStore"			)		;	// 加盟店
				item.m_strSlottingFlag		 	=		pInputItem->Attribute("SlottingFlag"		)		;	// 拉槽标识
				item.m_strReminder			 	=		pInputItem->Attribute("Reminder"			)		;	// 助记号
				item.m_strDrilling			 	=		pInputItem->Attribute("Drilling"			)		;	// 钻孔
				item.m_strOrderType			 	=		pInputItem->Attribute("OrderType"			)		;	// 订单类型
				item.m_strReverseSideBarcode 	=		pInputItem->Attribute("ReverseSideBarcode"	)		;	// 反面条码
				item.m_fProductLength			= _ttof(pInputItem->Attribute("ProductLength"	)	)		;	// 成品长度
				item.m_fProductWidth			= _ttof(pInputItem->Attribute("ProductWidth"	)	)		;	// 成品宽度
				item.m_fProductThickness		= _ttof(pInputItem->Attribute("ProductThickness")	)		;	// 成品厚度
				item.m_strOtherShapeID			=		pInputItem->Attribute("OtherShapeID"		)		;	// 异形ID
				item.m_strCustomerAddress		=		pInputItem->Attribute("CustomerAddress"		)		;	// 客户地址
				item.m_strHoleSlotFlag			=		pInputItem->Attribute("HoleSlotFlag"		)		;	// 钻槽标识
				item.m_strAbnormalInfo			=		pInputItem->Attribute("AbnormalInfo"		)		;	// 异形点信息


				vInputCpn.push_back(item);
			}

			pSingleton->SetBackupComponentInputItem(vInputCpn);
		}
		else if (strCurNodeName == "解决方案")
		{
			CSolution* pNewSln = new CSolution;

 			pNewSln->m_SlnID						= _ttoi(pCurNode->Attribute("SlnId"));	
			pNewSln->m_BaseInfo.m_PanelLength		= _ttof(pCurNode->Attribute("PanelLength"));		
			pNewSln->m_BaseInfo.m_PanelWidth		= _ttof(pCurNode->Attribute("PanelWidth"));	
			pNewSln->m_BaseInfo.m_SawKerfWidth		= _ttof(pCurNode->Attribute("SawKerfWidth"));
			pNewSln->m_BaseInfo.m_DeburringWidth	= _ttof(pCurNode->Attribute("DeburringWidth"));	
			pNewSln->m_strMaterial					=		pCurNode->Attribute("Material");			
			pNewSln->m_fThickness					= _ttof(pCurNode->Attribute("Thickness"));		

			pSingleton->m_BackupSolutionList.push_back(pNewSln);

			// 读取大板数据
			for (TiXmlElement * pCurPanelNode = pCurNode->FirstChildElement(); pCurPanelNode != NULL; pCurPanelNode = (TiXmlElement*)(pCurPanelNode->NextSibling()))
			{
				Panel* pNewPanel = new Panel;

				// panel的信息
				pNewPanel->m_PanelID	= _ttoi(pCurPanelNode->Attribute("PanelId"		));
				pNewPanel->m_OrgLen		= _ttof(pCurPanelNode->Attribute("OrgLen"		));
				pNewPanel->m_OrgWidth	= _ttof(pCurPanelNode->Attribute("OrgWidth"		));
				pNewPanel->m_OriginX	= _ttof(pCurPanelNode->Attribute("OriginX"		));
				pNewPanel->m_OriginY	= _ttof(pCurPanelNode->Attribute("OriginY"		));

				// component的信息
				pNewPanel->m_x			= _ttof(pCurPanelNode->Attribute("x"			));
				pNewPanel->m_y			= _ttof(pCurPanelNode->Attribute("y"			));
				pNewPanel->m_RealLength	= _ttof(pCurPanelNode->Attribute("RealLength"	));
				pNewPanel->m_RealWidth	= _ttof(pCurPanelNode->Attribute("RealWidth"	));
				pNewPanel->m_Thickness	= _ttof(pCurPanelNode->Attribute("Thickness"	));
				pNewPanel->m_type		= _ttoi(pCurPanelNode->Attribute("type"	));
				pNewPanel->m_Material	= pCurPanelNode->Attribute("Material");

				// 插入属性结构
				LoadOneTreeFromXml(pCurPanelNode, pNewPanel);

				pNewSln->m_PanelList.push_back(pNewPanel);
			}
		}
		else
		{
			AfxMessageBox("hgy中读取到未知节点，跳过！");
		}
	}


	// 读取完之后删除解密文件
	DeleteFile(XmlPath); 


	return true;
}
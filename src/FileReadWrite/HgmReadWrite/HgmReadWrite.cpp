#include "stdafx.h"

#include "HgmReadWrite.h"
#include "Markup.h"


#include "../Misc/HGTools.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Misc/ProgramMisc.h"

#include "../../../include/FileCompressandUnCompress/FileCompressandUnCompress.h"
#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/KnifeDll/KnifeClass.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../Misc/Misc.h"

using namespace TinyXml;

#define NO_OTHER_SHAPE_AUTO_CREATE 1





/*---------------------------------------*/
//	函数说明：
//		读取xml字符串中的板件信息
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
bool  HgmReadWrite::ReadPanelInfoByStream(char* pszXml, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{


	vector<map<string, string>> input_matrial = translatexmlByStream(pszXml, param);

	for (int i = 0; i < input_matrial.size(); i++)
	{
		map<string, string> one_cpn_map = input_matrial.at(i);
		ComponentInputItem componentInputItem;

		componentInputItem.m_strOrderID					= one_cpn_map["订单号"].c_str();
		componentInputItem.m_strCabinetID				= one_cpn_map["柜号"].c_str();
		componentInputItem.m_strBarcode					= one_cpn_map["条码编号"].c_str();
		componentInputItem.m_strCabinetName				= one_cpn_map["产品名称"].c_str();
		componentInputItem.m_strPanelName				= one_cpn_map["成品名称"].c_str();
		componentInputItem.m_strMaterial				= one_cpn_map["材质名称"].c_str();
		componentInputItem.m_fLength					= _ttof(one_cpn_map["开料长度"].c_str());
		componentInputItem.m_fWidth						= _ttof(one_cpn_map["开料宽度"].c_str());
		componentInputItem.m_fThickness					= _ttof(one_cpn_map["开料厚度"].c_str());
		componentInputItem.m_nCount						= _ttoi(one_cpn_map["需切数量"].c_str());
		componentInputItem.m_strTexture					= one_cpn_map["纹路方向"].c_str();
		CString strBandingString						= one_cpn_map["封边量"].c_str();
		SpyBandingString(strBandingString, componentInputItem.m_afBanding);
		componentInputItem.m_strOtherShapeIDAndParam	= one_cpn_map["异型"].c_str();
		componentInputItem.m_eRotateType				= atoi(one_cpn_map["旋转"].c_str());
		componentInputItem.m_strUpperFaceSlotInfo		= one_cpn_map["正面槽1"].c_str();
		componentInputItem.m_bSlotFlipped				= strcmp(one_cpn_map["槽翻转"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceSlotInfo		= one_cpn_map["反面槽1"].c_str();
		componentInputItem.m_strUpperFaceHoleInfo		= one_cpn_map["正面孔1"].c_str();
		componentInputItem.m_bVHoleFlipped				= strcmp(one_cpn_map["孔翻转"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceHoleInfo		= one_cpn_map["反面孔1"].c_str();
		componentInputItem.m_strCustomerInfo			= one_cpn_map["客户信息"].c_str();
		componentInputItem.m_strJoinedStore				= one_cpn_map["加盟店"].c_str();
		componentInputItem.m_strSlottingFlag			= one_cpn_map["拉槽标识"].c_str();
		componentInputItem.m_strReminder				= one_cpn_map["助记号"].c_str();
		componentInputItem.m_strDrilling				= one_cpn_map["钻孔"].c_str();
		componentInputItem.m_strOrderType				= one_cpn_map["订单类型"].c_str();
		componentInputItem.m_strReverseSideBarcode		= one_cpn_map["反面条码"].c_str();
		componentInputItem.m_fProductLength				= _ttof(one_cpn_map["成品长度"].c_str());
		componentInputItem.m_fProductWidth				= _ttof(one_cpn_map["成品宽度"].c_str());
		componentInputItem.m_fProductThickness			= _ttof(one_cpn_map["成品厚度"].c_str());
		componentInputItem.m_strOtherShapeID			= one_cpn_map["异型ID"].c_str();
		componentInputItem.m_strCustomerAddress			= one_cpn_map["客户地址"].c_str();
		componentInputItem.m_strHoleSlotFlag			= one_cpn_map["钻槽标识"].c_str();
		componentInputItem.m_strAbnormalInfo			= one_cpn_map["异形点信息"].c_str();
		componentInputItem.m_strSendingDate             = one_cpn_map["发货日期"].c_str();

		vComponentInputItem.push_back(componentInputItem);



	}

	return true;
}





/*---------------------------------------*/
//	函数说明：
//		读取hgm文件中的板件信息
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
bool  HgmReadWrite::ReadPanelInfo(CString strSrcPath, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{

	//加密文件读取标准
	CString XmlPath = HGTools::getXmlPathFromHgxPath(strSrcPath);



	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	decrypt_base64(strSrcPath.GetBuffer(), XmlPath.GetBuffer());

#else

	//HGTools::decryptFile(strSrcPath, XmlPath);

#endif




	vector<map<string, string>> input_matrial = translatexml(XmlPath.GetBuffer(), param);

	DeleteFile(XmlPath);


// 	string PointOverPanelSize = "";
// 	string AbnormalPointError = "";
	for (int i = 0; i < input_matrial.size(); i++)
	{
		map<string, string> one_cpn_map = input_matrial.at(i);
		ComponentInputItem componentInputItem;

		componentInputItem.m_strOrderID					= one_cpn_map["订单号"].c_str();
		componentInputItem.m_strCabinetID				= one_cpn_map["柜号"].c_str();
		componentInputItem.m_strBarcode					= one_cpn_map["条码编号"].c_str();
		componentInputItem.m_strCabinetName				= one_cpn_map["产品名称"].c_str();
		componentInputItem.m_strPanelName				= one_cpn_map["成品名称"].c_str();
		componentInputItem.m_strMaterial				= one_cpn_map["材质名称"].c_str();
		componentInputItem.m_fLength					= _ttof(one_cpn_map["开料长度"].c_str());
		componentInputItem.m_fWidth						= _ttof(one_cpn_map["开料宽度"].c_str());
		componentInputItem.m_fThickness					= _ttof(one_cpn_map["开料厚度"].c_str());
 		componentInputItem.m_nCount						= _ttoi(one_cpn_map["需切数量"].c_str());
 		componentInputItem.m_strTexture					= one_cpn_map["纹路方向"].c_str();
		CString strBandingString						= one_cpn_map["封边量"].c_str();
		SpyBandingString(strBandingString, componentInputItem.m_afBanding);
 		componentInputItem.m_strOtherShapeIDAndParam	= one_cpn_map["异型"].c_str();
		componentInputItem.m_eRotateType				= atoi(one_cpn_map["旋转"].c_str());
 		componentInputItem.m_strUpperFaceSlotInfo		= one_cpn_map["正面槽1"].c_str();
 		componentInputItem.m_bSlotFlipped				= strcmp(one_cpn_map["槽翻转"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceSlotInfo		= one_cpn_map["反面槽1"].c_str();
		componentInputItem.m_strUpperFaceHoleInfo		= one_cpn_map["正面孔1"].c_str();
		componentInputItem.m_bVHoleFlipped				= strcmp(one_cpn_map["孔翻转"].c_str(), _T("1")) == 0 ? true : false;
		componentInputItem.m_strDownerFaceHoleInfo		= one_cpn_map["反面孔1"].c_str();
		componentInputItem.m_strCustomerInfo			= one_cpn_map["客户信息"].c_str();
		componentInputItem.m_strJoinedStore				= one_cpn_map["加盟店"].c_str();
		componentInputItem.m_strSlottingFlag			= one_cpn_map["拉槽标识"].c_str();
		componentInputItem.m_strReminder				= one_cpn_map["助记号"].c_str();
		componentInputItem.m_strDrilling				= one_cpn_map["钻孔"].c_str();
		componentInputItem.m_strOrderType				= one_cpn_map["订单类型"].c_str();
		componentInputItem.m_strReverseSideBarcode		= one_cpn_map["反面条码"].c_str();
		componentInputItem.m_fProductLength				= _ttof(one_cpn_map["成品长度"].c_str());
		componentInputItem.m_fProductWidth				= _ttof(one_cpn_map["成品宽度"].c_str());
		componentInputItem.m_fProductThickness			= _ttof(one_cpn_map["成品厚度"].c_str());
		componentInputItem.m_strOtherShapeID			= one_cpn_map["异型ID"].c_str();
		componentInputItem.m_strCustomerAddress			= one_cpn_map["客户地址"].c_str();
		componentInputItem.m_strHoleSlotFlag			= one_cpn_map["钻槽标识"].c_str();
		componentInputItem.m_strAbnormalInfo			= one_cpn_map["异形点信息"].c_str();
		componentInputItem.m_strPanelClass              = one_cpn_map["部件类别"].c_str();
		componentInputItem.m_strSendingDate              = one_cpn_map["发货日期"].c_str();

		componentInputItem.m_aHasSideHole[SideType::SIDE_LEFT] = one_cpn_map["有左侧孔"] == "1";
		componentInputItem.m_aHasSideHole[SideType::SIDE_FRONT] = one_cpn_map["有前侧孔"] == "1";
		componentInputItem.m_aHasSideHole[SideType::SIDE_RIGHT] = one_cpn_map["有右侧孔"] == "1";
		componentInputItem.m_aHasSideHole[SideType::SIDE_BACK] = one_cpn_map["有后侧孔"] == "1";
		
		vComponentInputItem.push_back(componentInputItem);


		// 判断异形是否超出板件
// 		float f_pwidth			= atof(one_cpn_map["开料长度"].c_str());
// 		float f_pheight			= atof(one_cpn_map["开料宽度"].c_str());
// 		std::string str_pinfo	= one_cpn_map["条码编号"];
// 
// 
// 		// 判断异形数据是否超出了板件尺寸
// 		std::vector<PointInfo> input_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(one_cpn_map["异形点信息"]);
// 
// 		// 计算异形点矩形
// 		int nCount = input_pointInfos.size();
// 		float OutlineMax_x, OutlineMax_y, OutlineMin_x, OutlineMin_y, max_r;
// 
// 		// 轮廓点不对
// 		if (nCount < 3)
// 		{
// 			AbnormalPointError = AbnormalPointError + "板件：" + str_pinfo + " " + one_cpn_map["开料长度"] + "*" + one_cpn_map["开料宽度"] + "异形点数据错误\r\n";
// 		}
// 		else
// 		{
// 			PointInfo pnt = input_pointInfos[0];
// 			OutlineMax_x = OutlineMin_x = pnt.x;
// 			OutlineMax_y = OutlineMin_y = pnt.y;
// 			max_r = pnt.r;
// 
// 			// 求得矩形
// 			for (int j = 1; j < nCount; j++)
// 			{
// 				pnt = input_pointInfos[j];
// 				float x = pnt.x;
// 				float y = pnt.y;
// 				float r = pnt.r;
// 
// 				if (x > OutlineMax_x)
// 					OutlineMax_x = x;
// 
// 				if (x < OutlineMin_x)
// 					OutlineMin_x = x;
// 
// 				if (y > OutlineMax_y)
// 					OutlineMax_y = y;
// 
// 				if (y < OutlineMin_y)
// 					OutlineMin_y = y;
// 
// 				if (r > max_r)
// 					max_r = r;
// 			}
// 
// 			// 判断是否超出板件大小
// 			float width = abs(OutlineMax_x - OutlineMin_x);
// 			float height = abs(OutlineMax_y - OutlineMin_y);
// 
// 			if (width > f_pwidth || height > f_pheight || max_r > f_pwidth || max_r > f_pheight)
// 			{
// 				PointOverPanelSize = PointOverPanelSize + "板件：" + str_pinfo + " " + one_cpn_map["开料长度"] + "*" + one_cpn_map["开料宽度"] + "异形点可能超出板件大小，请检查\r\n";
// 			}
// 		}

	}

// 	if (PointOverPanelSize != "")
// 	{
// 		CString str_Outedgeinfo;
// 		str_Outedgeinfo.Format("%s",PointOverPanelSize.c_str());
// 		AfxMessageBox(str_Outedgeinfo);
// 	}


	return true;
}


/*---------------------------------------*/
//	函数说明：
//		读取XML文件中的板件信息
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
bool  HgmReadWrite::ReadPanelInfo_Debug(CString szFileDir, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{

	vector<map<string, string>> input_matrial = translatexml(szFileDir.GetBuffer(), param);


	for (int i = 0; i < input_matrial.size(); i++)
	{
		map<string, string> one_cpn_map = input_matrial.at(i);
		ComponentInputItem componentInputItem;

		componentInputItem.m_strBarcode			= one_cpn_map["条码编号"].c_str();
		componentInputItem.m_strCabinetName		= one_cpn_map["产品名称"].c_str();
		componentInputItem.m_strPanelName		= one_cpn_map["成品名称"].c_str();
		componentInputItem.m_fLength			= _ttof(one_cpn_map["开料长度"].c_str());
		componentInputItem.m_fWidth				= _ttof(one_cpn_map["开料宽度"].c_str());
		componentInputItem.m_fThickness			= _ttof(one_cpn_map["开料厚度"].c_str());
		componentInputItem.m_strMaterial		= one_cpn_map["材质名称"].c_str();
		componentInputItem.m_strTexture			= one_cpn_map["纹路方向"].c_str();
		componentInputItem.m_nCount				= _ttoi(one_cpn_map["需切数量"].c_str());
		componentInputItem.m_strOtherShapeIDAndParam = one_cpn_map["异型"].c_str();
		componentInputItem.m_strOtherShapeID    = one_cpn_map["异型ID"].c_str();
		componentInputItem.m_strAbnormalInfo	= one_cpn_map["异形点信息"].c_str();
		componentInputItem.m_strUpperFaceHoleInfo	= one_cpn_map["正面孔1"].c_str();
		componentInputItem.m_strDownerFaceHoleInfo	= one_cpn_map["反面孔1"].c_str();
		componentInputItem.m_strUpperFaceSlotInfo	= one_cpn_map["正面槽1"].c_str();
		componentInputItem.m_strDownerFaceSlotInfo	= one_cpn_map["反面槽1"].c_str();

		vComponentInputItem.push_back(componentInputItem);
	}

	return true;
}


/*---------------------------------------*/
//	函数说明：
//		将xml字符串转换成map数组
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
vector<map<string, string>> HgmReadWrite::translatexmlByStream(char* pszXml, SourceFilePreProccesParam& param)
{

	std::vector<std::map<std::string,std::string>> matrial_info;

	// 文档
	CMarkup xml;
	xml.SetDoc((const char*)pszXml);
	BOOL bFind = true;
	xml.ResetMainPos();//将当前主位置复位为第一个兄弟位置之前

	// 根节点
	if (xml.FindElem((MCD_CSTR)"KnifeData") == false)
	{
		AfxMessageBox("空文件");
		return matrial_info;
	}


	bool b_upvecImportEnable		= param.b_upvecImportEnable;//正面孔
	bool b_downvecImportEnable		= param.b_downvecImportEnable;//反面孔
	bool b_upsoltImportEnable		= param.b_upsoltImportEnable;//正面槽
	bool b_downsoltImportEnable		= param.b_downsoltImportEnable;//反面槽
	bool b_othershapeImportEnable	= param.b_othershapeImportEnable;//异形过滤


	int i_comnum	= param.i_comnum;//切割数量
	int i_comchange =param.i_comchange;//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
	float f_changex = param.f_changex;//孔槽偏移
	float f_changey = param.f_changey;//孔槽偏移


	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	std::map<float,float> vecAdder = param.vecAdder;

	//孔位过滤（找到指定孔直径的孔，并移除）
	vector<float> _cxy = param._cxy;



	while (xml.FindChildElem((MCD_CSTR)"ComPanel"))   //定位到下一个子元素，匹配元素名或路径。
	{
		xml.IntoElem();  //进入当前主位置的下一级，当前的位置变为父位置 ComPanel

		std::map<std::string,std::string> map_one_matrial;

// 		CString strTagName = _T("");  
// 		CString strData = _T("");  
// 		CString strData3 = _T("");  
// 
// 		strTagName = xml.GetTagName();		//得到主位置元素（或正在进行的指令的）标签名称
// 		strData = xml.GetAttrib("Barcode"); // 得到当前主位置元素或节点的字符串值
// 		strData3 = xml.GetAttrib("Name"); // 得到当前主位置元素或节点的字符串值

//		AfxMessageBox(strData + ":" + strData3);

		while (xml.FindChildElem((MCD_CSTR)"ComInfomation"))   //定位到下一个子元素，匹配元素名或路径。
		{
			xml.IntoElem();//进入当前主位置的下一级，当前的位置变为父位置

			while (xml.FindChildElem((MCD_CSTR)"Info"))   //定位到下一个子元素，匹配元素名或路径。
			{
				xml.IntoElem();//进入当前主位置的下一级，当前的位置变为父位置

				std::string info_name	= xml.GetAttrib("Name");
				std::string info_value	= xml.GetAttrib("Value");

				map_one_matrial[info_name] = info_value;


				xml.OutOfElem();//使当前父位置变成当前位置 
			}

			xml.OutOfElem();//使当前父位置变成当前位置  
		}


		int knifenum = atoi(map_one_matrial["需切数量"].c_str()) * i_comnum;
		CString str_knifenum;
		str_knifenum.Format("%d",knifenum);
		map_one_matrial["需切数量"] = str_knifenum;

		// 有数据
		if (map_one_matrial.size() > 0)
		{
			matrial_info.push_back(map_one_matrial);
		}

		xml.OutOfElem();//使当前父位置变成当前位置  
	}


	return matrial_info;
}





/*---------------------------------------*/
//	函数说明：
//		将hgm文件转换成map数组
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
vector<map<string, string>> HgmReadWrite::translatexml(std::string filepath, SourceFilePreProccesParam& param)
{
	std::vector<std::map<std::string,std::string>> matrial_info;

	// 文档
	TiXmlDocument* doc = new TiXmlDocument;
	doc->LoadFile(filepath.c_str());

	// 根节点
	TiXmlElement *pRootElement = doc->RootElement();
	if (pRootElement == NULL)
	{
		AfxMessageBox("空文件");
		return matrial_info;
	}

	bool b_upvecImportEnable		= param.b_upvecImportEnable;//正面孔
	bool b_downvecImportEnable		= param.b_downvecImportEnable;//反面孔
	bool b_upsoltImportEnable		= param.b_upsoltImportEnable;//正面槽
	bool b_downsoltImportEnable		= param.b_downsoltImportEnable;//反面槽
	bool b_othershapeImportEnable	= param.b_othershapeImportEnable;//异形过滤
	bool bReserveDeepHole           = param.bReserveDeepHole;
	bool bReserveDeepSlot           = param.bReserveDeepSlot;


	int i_comnum	= param.i_comnum;//切割数量
	int i_comchange =param.i_comchange;//翻转设置（不翻转=0，正面无信息翻转=1，打孔优先翻转=2，开槽优先翻转=3）
	float f_changex = param.f_changex;//孔槽偏移
	float f_changey = param.f_changey;//孔槽偏移


	//孔位偏差（找到指定孔直径的孔，其直径加一个值）
	std::map<float,float> vecAdder = param.vecAdder;

	//孔位过滤（找到指定孔直径的孔，并移除）
	vector<float> _cxy = param._cxy;


	for (TiXmlElement* pCurCompanel = pRootElement->FirstChildElement(); pCurCompanel != NULL; pCurCompanel = (TiXmlElement*)(pCurCompanel->NextSibling()))
	{
		std::map<std::string,std::string> map_one_matrial;
		float fComponentXLen = 0.0;
		float fComponentYLen = 0.0;
		float fComponentThickness = 0.0;

		string strBarCode = pCurCompanel->Attribute("Barcode");

		int index0_saw = 0;
		int index0_vertical = 0;

		int index5_saw = 0;
		int index5_vertical = 0;

		for (TiXmlElement * pCurComInfomation = pCurCompanel->FirstChildElement(); pCurComInfomation != NULL; pCurComInfomation = (TiXmlElement*)(pCurComInfomation->NextSibling()))
		{
			string strComInfo =  pCurComInfomation->Value();
			string strFaceIndex;
			const char* pIndex = pCurComInfomation->Attribute("Index");

			if (pIndex != NULL)
				strFaceIndex = pIndex;

			if (strComInfo == "ComInfomation")
			{
				// 板件基本信息

#if 1

				for (TiXmlElement * pCurInfoNode = pCurComInfomation->FirstChildElement(); pCurInfoNode != NULL; pCurInfoNode = (TiXmlElement*)(pCurInfoNode->NextSibling()))
				{

					std::string info_name = pCurInfoNode->Attribute("Name");
					std::string info_value = pCurInfoNode->Attribute("Value");

					map_one_matrial[info_name] = info_value;
					int i = 0;
				}

				fComponentXLen = atof(map_one_matrial["开料长度"].c_str());
				fComponentYLen = atof(map_one_matrial["开料宽度"].c_str());
				fComponentThickness = atof(map_one_matrial["开料厚度"].c_str());
#endif
			}
			else if(strComInfo == "PanelFace" &&  strFaceIndex == "0")	
			{
				// 正面信息 
#if 1

				//获取正面槽信息
				std::string saw_name = "正面槽1";
				std::string saw_value = "";
				//获取正面孔信息
				std::string vertical_name = "正面孔1";
				std::string vertical_value = "";

				for (TiXmlElement * pCurSawOrVtc = pCurComInfomation->FirstChildElement(); pCurSawOrVtc != NULL; pCurSawOrVtc = (TiXmlElement*)(pCurSawOrVtc->NextSibling()))
				{
					string NodeValue = pCurSawOrVtc->Value();


					// 拉槽信息 Saw
					if (NodeValue == "SAW")		
					{
						index0_saw++;

						string str_pos_x; 
						string str_pos_y; 
						string str_angle; 
						string str_size_x;
						string str_size_y;
						string str_depth; 

						// 读取槽信息
						for (TiXmlElement * saw_node = pCurSawOrVtc->FirstChildElement(); saw_node != NULL; saw_node = (TiXmlElement*)(saw_node->NextSibling()))
						{
							string param_name = saw_node->Attribute("Name");
							
							if (param_name == "pos_x")
							{
								str_pos_x = saw_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = saw_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = saw_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = saw_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = saw_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = saw_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						int i_angle = atoi(str_angle.c_str());
						if (i_angle%180 != 0)
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_y.c_str());
							float size_y = atof(str_size_x.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_y;
							float _height = size_x;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,0);

							if (b_upsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
						else
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_x.c_str());
							float size_y = atof(str_size_y.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_x;
							float _height = size_y;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,1);

							if (b_upsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
					}

					// 孔信息

					//打孔信息 VERTICAL
					if (NodeValue == "VERTICAL")
					{
						index0_vertical++;


						std::string str_pos_x	;
						std::string str_pos_y	;
						std::string str_angle	;
						std::string str_size_x	;
						std::string str_size_y	;
						std::string str_depth	;



						// 读取槽信息
						for (TiXmlElement * vertical_node = pCurSawOrVtc->FirstChildElement(); vertical_node != NULL; vertical_node = (TiXmlElement*)(vertical_node->NextSibling()))
						{
							string param_name = vertical_node->Attribute("Name");

							if (param_name == "pos_x")
							{
								str_pos_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = vertical_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						float pos_x = atof(str_pos_x.c_str());
						float pos_y = atof(str_pos_y.c_str());
						float size_x = atof(str_size_x.c_str());
						float size_y = atof(str_size_y.c_str());
						float depth = atof(str_depth.c_str());

						float _x = pos_x + f_changex;
						float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
						float _r = size_x;
						float _depth = depth;

						bool vecdelete = false;
						for (int v = 0;v < _cxy.size();v++)
						{
							if (_r - _cxy.at(v) > -0.1 && _r - _cxy.at(v) < 0.1)
							{
								vecdelete = true;
								break;
							}
						}
						if (vecdelete == false)
						{
							for (auto iter_mapf = vecAdder.begin();iter_mapf != vecAdder.end();iter_mapf++)
							{
								float f_r = iter_mapf->first;
								float f_f = iter_mapf->second;
								if (f_r - _r > -0.01 && f_r - _r < 0.01)
								{
									_r = _r + f_f;
									break;
								}
							}

							char one_vertical_value[100];
							sprintf_s(one_vertical_value,"%0.1f,%0.1f,%0.1f,%0.1f;",_x,_y,_r,_depth);
							if (b_upvecImportEnable || (bReserveDeepHole && (_depth - fComponentThickness) > -0.1))
								vertical_value = vertical_value + one_vertical_value;
						}
					}
				}

				// 增加槽数据
				map_one_matrial[saw_name] = saw_value;

				// 增加孔数据
				map_one_matrial[vertical_name] = vertical_value;

#endif
			}
			else if (strComInfo == "PanelFace" &&  strFaceIndex == "5")
			{
				//获取反面槽信息

#if 1
				std::string saw_name = "反面槽1";
				std::string saw_value = "";

				//获取反面孔信息
				std::string vertical_name = "反面孔1";
				std::string vertical_value = "";


				for (TiXmlElement * pCurSawOrVtc = pCurComInfomation->FirstChildElement(); pCurSawOrVtc != NULL; pCurSawOrVtc = (TiXmlElement*)(pCurSawOrVtc->NextSibling()))
				{
					string NodeValue = pCurSawOrVtc->Value();


					// 拉槽信息 Saw
					if (NodeValue == "SAW")		
					{
						index0_saw++;

						string str_pos_x; 
						string str_pos_y; 
						string str_angle; 
						string str_size_x;
						string str_size_y;
						string str_depth; 

						// 读取槽信息
						for (TiXmlElement * saw_node = pCurSawOrVtc->FirstChildElement(); saw_node != NULL; saw_node = (TiXmlElement*)(saw_node->NextSibling()))
						{
							string param_name = saw_node->Attribute("Name");

							if (param_name == "pos_x")
							{
								str_pos_x = saw_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = saw_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = saw_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = saw_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = saw_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = saw_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						int i_angle = atoi(str_angle.c_str());
						if (i_angle%180 != 0)
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_y.c_str());
							float size_y = atof(str_size_x.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_y;
							float _height = size_x;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,0);

							if (b_downsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
						else
						{
							float pos_x = atof(str_pos_x.c_str());
							float pos_y = atof(str_pos_y.c_str());
							float size_x = atof(str_size_x.c_str());
							float size_y = atof(str_size_y.c_str());
							float depth = atof(str_depth.c_str());

							float _x = pos_x + f_changex;
							float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
							float _width = size_x;
							float _height = size_y;
							float _depth = depth;

							char one_saw_value[100];
							sprintf_s(one_saw_value,"%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;",_x,_y,_width,_height,_depth,1);
							
							if (b_downsoltImportEnable || (bReserveDeepSlot && (_depth - fComponentThickness) > -0.1))
								saw_value = saw_value + one_saw_value;
						}
					}

					// 孔信息

					//打孔信息 VERTICAL
					if (NodeValue == "VERTICAL")
					{
						index5_vertical++;


						std::string str_pos_x	;
						std::string str_pos_y	;
						std::string str_angle	;
						std::string str_size_x	;
						std::string str_size_y	;
						std::string str_depth	;



						// 读取槽信息
						for (TiXmlElement * vertical_node = pCurSawOrVtc->FirstChildElement(); vertical_node != NULL; vertical_node = (TiXmlElement*)(vertical_node->NextSibling()))
						{
							string param_name = vertical_node->Attribute("Name");

							if (param_name == "pos_x")
							{
								str_pos_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "pos_y")
							{
								str_pos_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "angle")
							{
								str_angle = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_x")
							{
								str_size_x = vertical_node->Attribute("Value");
							}
							else if(param_name == "size_y")
							{
								str_size_y = vertical_node->Attribute("Value");
							}
							else if(param_name == "depth")
							{
								str_depth = vertical_node->Attribute("Value");
							}
							else
							{
								;
							}
						}


						float pos_x = atof(str_pos_x.c_str());
						float pos_y = atof(str_pos_y.c_str());
						float size_x = atof(str_size_x.c_str());
						float size_y = atof(str_size_y.c_str());
						float depth = atof(str_depth.c_str());

						float _x = pos_x + f_changex;
						float _y = /*pos_y + f_changey*/(fComponentYLen - pos_y) - f_changey;
						float _r = size_x;
						float _depth = depth;

						bool vecdelete = false;
						for (int v = 0;v < _cxy.size();v++)
						{
							if (_r - _cxy.at(v) > -0.1 && _r - _cxy.at(v) < 0.1)
							{
								vecdelete = true;
								break;
							}
						}
						if (vecdelete == false)
						{
							for (auto iter_mapf = vecAdder.begin();iter_mapf != vecAdder.end();iter_mapf++)
							{
								float f_r = iter_mapf->first;
								float f_f = iter_mapf->second;
								if (f_r - _r > -0.01 && f_r - _r < 0.01)
								{
									_r = _r + f_f;
									break;
								}
							}

							char one_vertical_value[100];
							sprintf_s(one_vertical_value,"%0.1f,%0.1f,%0.1f,%0.1f;",_x,_y,_r,_depth);

							if (b_downvecImportEnable || (bReserveDeepHole && (_depth - fComponentThickness) > -0.1))
								vertical_value = vertical_value + one_vertical_value;
						}
					}
				}

				map_one_matrial[saw_name] = saw_value;

				map_one_matrial[vertical_name] = vertical_value;

#endif

			}
			else if (strComInfo == "OutLine")
			{
				//录入异形

#if 1

				vector<PointInfo> infos;

				//将outline点加载出来
				for (TiXmlElement * pVectorNode = pCurComInfomation->FirstChildElement(); pVectorNode != NULL; pVectorNode = (TiXmlElement*)(pVectorNode->NextSibling()))
				{

					PointInfo info;

					info.x		= atof(	pVectorNode->Attribute("X"));	
					info.y		= atof(	pVectorNode->Attribute("Y"));	
					info.r		= atof(	pVectorNode->Attribute("R"));	
					info.sign	= atoi(	pVectorNode->Attribute("Sign"));	
					info.dir	= atoi(	pVectorNode->Attribute("Dir"));	
					info.cut	= atoi(	pVectorNode->Attribute("Cut"));	
					info.group	= atoi(	pVectorNode->Attribute("Group"));	
					info.side	= atoi(	pVectorNode->Attribute("Side"));	
					info.type	= atoi(	pVectorNode->Attribute("type"));	

					if (info.group == 1 && infos.size()>0)	//一定要让点闭合
					{
						infos[infos.size() - 1].group = 2;
					}

					infos.push_back(info);
				}

				if (infos.size()>0)	//一定要让点闭合
				{
					infos[infos.size() - 1].group = 2;
				}

				map_one_matrial["异形点信息"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(infos);

#endif
			}
		}

		//处理新老异形问题
		{
			//有outline的，不管异形（上面已有）36
			if (map_one_matrial["异形点信息"].compare("") != 0 && b_othershapeImportEnable)
			{
				//根据界面选择的旋转对图形进行旋转
				std::vector<PointInfo> m_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(map_one_matrial["异形点信息"]);
				//m_pointInfos = OtherShapeHelper::SetPointInfoYNegative(m_pointInfos);
				OtherShapeHelper::GetPointsToYPlus(m_pointInfos, map_one_matrial["旋转"], map_one_matrial["开料长度"], map_one_matrial["开料宽度"]);
				m_pointInfos = OtherShapeHelper::getPointInfoRotate(m_pointInfos,map_one_matrial["旋转"],map_one_matrial["开料长度"],map_one_matrial["开料宽度"]);
				map_one_matrial["异形点信息"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(m_pointInfos);
			}
			//没有outline的，用异形字符串生成outline
			else if (map_one_matrial["异形点信息"].compare("") == 0 && map_one_matrial["异型"].compare("") != 0 && b_othershapeImportEnable)
			{
#if NO_OTHER_SHAPE_AUTO_CREATE 
				//获得界面的所有参数，并校验
				std::vector<param_point> pa_p = OtherShapeHelper::GetParamPointsFromOtherShapeString(map_one_matrial["异型"],map_one_matrial["开料长度"],map_one_matrial["开料宽度"]);

				OutputDebugString("START:\n");
				for(int k = 0; k < pa_p.size(); k++)
				{
					CString strTmp;
					strTmp.Format("name=%s value =%s\n", pa_p[k]._name.c_str(), pa_p[k]._value.c_str());
					OutputDebugString(strTmp);
				}

				OtherShapeHelper::RotateOtherShapeParamPoint(pa_p,map_one_matrial["旋转"]);

				//绘制界面参数所生成的图形
				std::vector<std::map<std::string,std::string>>m_outline = OtherShapeHelper::GetOtherShapePanelOutLineListFromOtherShapeID(OtherShapeHelper::GetOtherShapeId(pa_p));
				std::vector<PointInfo> m_pointInfos = OtherShapeHelper::GetPointInfoFromPanelOutLineAndParamPoint(m_outline,pa_p);

				OutputDebugString("START:\n");
				for(int k = 0; k < m_pointInfos.size(); k++)
				{
					CString strTmp;
					strTmp.Format("x=%f y =%f\n", m_pointInfos[k].x, m_pointInfos[k].y);
					OutputDebugString(strTmp);
				}

				OtherShapeHelper::GetPointsToYPlus(m_pointInfos, map_one_matrial["旋转"], map_one_matrial["开料长度"], map_one_matrial["开料宽度"]);

				//根据界面选择的旋转对图形进行旋转
				m_pointInfos = OtherShapeHelper::RotateOtherShapePointInfos(m_pointInfos,map_one_matrial["旋转"],map_one_matrial["开料长度"],map_one_matrial["开料宽度"]);

				map_one_matrial["异形点信息"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(m_pointInfos);
#endif
			}
			else	//啥都没有生成outline为长方形
			{
#if NO_OTHER_SHAPE_AUTO_CREATE 
				map_one_matrial["异型ID"] = "异0";
				map_one_matrial["异形点信息"] = OtherShapeHelper::GetPointInfoStringFromPointInfo(
					OtherShapeHelper::getPointInfoRectangle(
					map_one_matrial["旋转"],
					map_one_matrial["开料长度"],
					map_one_matrial["开料宽度"])
					);
#endif
			}
		}

		////处理板件孔槽翻转
		//{
		//	//需要进行孔槽旋转
		//	if (map_one_matrial["孔槽旋转"].compare("1") == 0 )
		//	{
		//		//map_one_matrial["正面孔1"] = ExchangeVerticalXY(map_one_matrial["正面孔1"], fComponentYLen);
		//		//map_one_matrial["反面孔1"] = ExchangeVerticalXY(map_one_matrial["反面孔1"], fComponentYLen);
		//		//map_one_matrial["正面槽1"] = ExchangeSlotXY(map_one_matrial["正面槽1"], fComponentYLen);
		//		//map_one_matrial["反面槽1"] = ExchangeSlotXY(map_one_matrial["反面槽1"], fComponentYLen);
		//		map_one_matrial["纹路方向"] = "无纹理";
		//	}
		//}

		//翻转设置 （正面无信息翻转=1）
		if (i_comchange == 1)
		{
			//反面有信息
			if (map_one_matrial["反面孔1"] != "" || map_one_matrial["反面槽1"] != "")
			{
				//且正面无信息
				if (map_one_matrial["正面孔1"] == "" && map_one_matrial["正面槽1"] == "")
				{
					//翻转
					TurnOverVecSlot(map_one_matrial);
				}
			}
		}
		//翻转设置（打孔优先翻转=2）
		else if (i_comchange == 2)
		{
			//如果正面孔为空
			if (map_one_matrial["正面孔1"] == "")
			{
				//且反面孔不为空
				if (map_one_matrial["反面孔1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
				//且正面槽为空，反面槽不空
				else if (map_one_matrial["正面槽1"] == "" && map_one_matrial["反面槽1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
			}
		}
		//翻转设置（开槽优先翻转=3）
		else if (i_comchange == 3)
		{
			//如果正面槽为空
			if (map_one_matrial["正面槽1"] == "")
			{
				//且反面槽不为空
				if (map_one_matrial["反面槽1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
				//且正面孔为空，反面孔不空
				else if (map_one_matrial["正面孔1"] == "" && map_one_matrial["反面孔1"] != "")
				{
					TurnOverVecSlot(map_one_matrial);
				}
			}
		}
		//反面孔数多于正面孔数时翻转
		else if (i_comchange == 4)
		{
			//正面孔数 < 反面孔数，翻转
			if (index0_vertical < index5_vertical)
			{
				TurnOverVecSlot(map_one_matrial);
			}
		}

		int knifenum = atoi(map_one_matrial["需切数量"].c_str()) * i_comnum;
		CString str_knifenum;
		str_knifenum.Format("%d",knifenum);
		map_one_matrial["需切数量"] = str_knifenum;

		XmlHandler xmlHandler;
		TiXmlElement* elmFace1 = xmlHandler.GetIndicatedElement(pCurCompanel, "PanelFace", "Index", "1");
		if(xmlHandler.GetIndicatedElement(elmFace1, "VERTICAL", "Layer", "VERTICAL"))
			map_one_matrial["有左侧孔"] = "1";
		else
			map_one_matrial["有左侧孔"] = "0";

		TiXmlElement* elmFace2 = xmlHandler.GetIndicatedElement(pCurCompanel, "PanelFace", "Index", "2");
		if(xmlHandler.GetIndicatedElement(elmFace2, "VERTICAL", "Layer", "VERTICAL"))
			map_one_matrial["有前侧孔"] = "1";
		else
			map_one_matrial["有前侧孔"] = "0";

		TiXmlElement* elmFace3 = xmlHandler.GetIndicatedElement(pCurCompanel, "PanelFace", "Index", "3");
		if(xmlHandler.GetIndicatedElement(elmFace3, "VERTICAL", "Layer", "VERTICAL"))
			map_one_matrial["有右侧孔"] = "1";
		else
			map_one_matrial["有右侧孔"] = "0";

		TiXmlElement* elmFace4 = xmlHandler.GetIndicatedElement(pCurCompanel, "PanelFace", "Index", "4");
		if(xmlHandler.GetIndicatedElement(elmFace4, "VERTICAL", "Layer", "VERTICAL"))
			map_one_matrial["有后侧孔"] = "1";
		else
			map_one_matrial["有后侧孔"] = "0";
		

		// 有数据
		if (map_one_matrial.size() > 0)
		{
			matrial_info.push_back(map_one_matrial);
		}
	}

	return matrial_info;
}

/*---------------------------------------*/
//	函数说明：
//		加载刀库配置信息
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
void HgmReadWrite::LoadSettings_KnifeType()
{

	// 新的加解密方式
#if (NEW_ENCRYPT_BASE64 == 1)

	// hgm文件路径和xml文件路径
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szSuffix);
	CString knifeType_hge = HGTools::getRelativePath(strTmp);
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hge);

	// 解密文件
	decrypt_base64(knifeType_hge.GetBuffer(), knifeType_xml.GetBuffer());


#else

	// hgm文件路径和xml文件路径
	CString knifeType_hgm = HGTools::getRelativePath("kdata\\knifetype.hgm");
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hgm);

	// 解密文件
	//HGTools::decryptFile(knifeType_hgm, knifeType_xml);

#endif

	

	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->m_KnifetypeDoc.LoadFile(knifeType_xml);



	HGTools::deleteFile(knifeType_xml);
}

/*---------------------------------------*/
//	函数说明：
//		保存刀库配置信息
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
void HgmReadWrite::SaveSettings_KnifeType()
{

#if (NEW_ENCRYPT_BASE64 == 1)

	//从hgx文件目录生成xml文件目录
	CString strTmp;
	strTmp.Format(_T("kdata\\knifetype%s"), g_szSuffix);
	CString knifeType_hge = HGTools::getRelativePath(strTmp);
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hge);

	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->m_KnifetypeDoc.SaveFile(knifeType_xml);

	encrypt_base64(knifeType_xml.GetBuffer(), knifeType_hge.GetBuffer());
	DeleteFile(knifeType_xml);
	//HGTools::encryptFile(knifeType_xml,knifeType_hge);
	//HGTools::deleteFile(knifeType_xml);


#else

	//从hgx文件目录生成xml文件目录
	CString knifeType_hgm = HGTools::getRelativePath("kdata\\knifetype.hgm");
	CString knifeType_xml = HGTools::getXmlPathFromHgxPath(knifeType_hgm);

	CSingleon* pSingleton = CSingleon::GetSingleton();
	pSingleton->m_KnifetypeDoc.SaveFile(knifeType_xml);


	//HGTools::encryptFile(knifeType_xml,knifeType_hgm);
	HGTools::deleteFile(knifeType_xml);

#endif



}
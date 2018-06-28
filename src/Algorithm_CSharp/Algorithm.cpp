#include "stdafx.h"
#include "Algorithm.h"
#include "Markup.h"


#include "../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../include/FileReadWrite/HgmReadWrite/HgmReadWrite.h"
#include "../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"

// 判断是否在测试环境下
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/FileReadWrited.lib")
#pragma comment(lib, "../../lib/tinyxmld.lib")

#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/FileReadWrite.lib")
#pragma comment(lib, "../../lib/tinyxml.lib")

#pragma  message(".......................................当前为release版本")

#endif





void SetBaseInfo(BaseInfo* pInfo)
{
	CString strTmp;

	// 长、宽、锯缝
	pInfo->m_SawKerfWidth = 6;
	pInfo->m_PanelLength = 2440;
	pInfo->m_PanelWidth = 1220;

	// 第一阶段参数
	pInfo->m_FirstSectionOPTimes = 1;
	pInfo->m_FirstSectionOPMethod = 1;	// 贪心算法

	// 第二阶段参数
	pInfo->m_SecondSectionOPTimes = 0;

	// 第三阶段参数0
	pInfo->m_ThirdSectionOPTimes = 0;
	pInfo->m_ThirdSectionOAccptableUtilization = 0.9;

	// 排样原点
	pInfo->m_LayoutOrg = 1;

}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		将读入的EXCEL小板结构体（ComponentInputItem）转换为小板类（Component）链表
//	
//	param：
//		vector<ComponentInputItem>& vComponentInputItem -- 读入的EXCEL小板数组
//		ComponentList& componentList -- 小板链表
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void ConvertInputInfoToComponentList(vector<ComponentInputItem>& vComponentInputItem, ComponentList& componentList)
{
	componentList.clear();

	int nID = 0;
	for(int i = 0; i < vComponentInputItem.size(); i++)
	{
		ComponentInputItem& theInputItem = vComponentInputItem[i];
		for(int j = 0; j < theInputItem.m_nCount; j++)
		{
			Component *pCpn = new Component;
			pCpn->m_CpnID = nID;
			pCpn->m_BarCode = theInputItem.m_strBarcode;
			pCpn->m_strCabinetName = theInputItem.m_strCabinetName;
			pCpn->m_strComponentName = theInputItem.m_strPanelName;
			pCpn->m_RealLength = theInputItem.m_fLength;
			pCpn->m_RealWidth = theInputItem.m_fWidth;
			pCpn->m_Thickness = theInputItem.m_fThickness;
			pCpn->m_Material = theInputItem.m_strMaterial;

			if(theInputItem.m_strTexture.CompareNoCase("横纹") == 0)
				pCpn->m_Texture = TextureType_H_TEXTURE;
			else if(theInputItem.m_strTexture.CompareNoCase("竖纹") == 0)
				pCpn->m_Texture = TextureType_V_TEXTURE;
			else if(theInputItem.m_strTexture.CompareNoCase("无纹理") == 0)
				pCpn->m_Texture = TextureType_NO_TEXTURE;
			else
				pCpn->m_Texture = TextureType_H_TEXTURE;

			pCpn->m_type = NodeType_NeededComponent;
			pCpn->m_x = 0;
			pCpn->m_y = 0;

			componentList.push_back(pCpn);

			nID++;
		}
	}
}

bool IsFloatEqual(float f1, float f2)
{
	if(fabs(f1 - f2) <= 0.01)
		return TRUE;
	return FALSE;
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		将小板根据材质和厚度分组，同材质、通厚度的为一组
//	
//	param：
//		ComponentList& SrcComponentList -- 所有的小板链表
//		vector<ComponentList>& SplitComponentGroup -- 分好组的小板链表
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void SplitComponentList(ComponentList& SrcComponentList, vector<ComponentList>& SplitComponentGroup)
{
	int nSrcCompSize = SrcComponentList.size();

	// 遍历
	for(int i = 0; i < nSrcCompSize; i++)
	{
		int nListToInsertItemID = -1;
		int nSplitCompGroupSize = SplitComponentGroup.size();
		Component* pSrcComponent = SrcComponentList[i];

		// 查看板件分组中是否已有同材质、厚度的分组
		for(int j = 0; j < nSplitCompGroupSize; j++)
		{
			if(SplitComponentGroup[j].size() > 0 )		// 存在分组
			{
				Component* pSplitComponent = SplitComponentGroup[j][0];

				// 判断材质、厚度是否相同
				if (pSplitComponent->m_Material.Compare(pSrcComponent->m_Material) == 0 
					&& IsFloatEqual(pSplitComponent->m_Thickness, pSrcComponent->m_Thickness) == TRUE)
				{
					nListToInsertItemID = j;
					break;
				}
			}
		}

		if(nListToInsertItemID != -1)	// 找到已有分组
		{
			SplitComponentGroup[nListToInsertItemID].push_back(pSrcComponent);
		}
		else							// 未找到分组，新建分组并插入
		{
			ComponentList componentList;
			componentList.push_back(pSrcComponent);
			SplitComponentGroup.push_back(componentList);
		}
	}
}






// 测试打印
int Test(char* pszXml)
{
// 	CMarkup xml;
// 
// 	xml.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");
// 	xml.AddElem((MCD_CSTR)"UserInfo"); //在当前主位置元素或最后兄弟位置之后增加一个元素
// 	xml.IntoElem(); //进入当前主位置的下一级，当前的位置变为父位置。
// 	xml.AddElem((MCD_CSTR)"UserID",(MCD_CSTR)"Jason");  
// 	xml.AddElem((MCD_CSTR)"UserID",(MCD_CSTR)"evil");
// 	xml.OutOfElem(); //使当前父位置变成当前位置。
// 	xml.Save((MCD_CSTR)"E:\\新雕刻机\\trunk\\bin\\UserInfo.xml");     //可指定目录   将XML数据写入文件中
	
	CMarkup xml;
	xml.SetDoc((const char*)pszXml);
	BOOL bFind = true;
	xml.ResetMainPos();//将当前主位置复位为第一个兄弟位置之前

	while (xml.FindChildElem((MCD_CSTR)"ComPanel"))   //定位到下一个子元素，匹配元素名或路径。
	{
		xml.IntoElem();//进入当前主位置的下一级，当前的位置变为父位置

		CString strTagName = _T("");  
		CString strData = _T("");  
		CString strData3 = _T("");  

		strTagName = xml.GetTagName();		//得到主位置元素（或正在进行的指令的）标签名称
		strData = xml.GetAttrib("Barcode"); // 得到当前主位置元素或节点的字符串值
		strData3 = xml.GetAttrib("Name"); // 得到当前主位置元素或节点的字符串值

		AfxMessageBox(strData + ":" + strData3);

		while (xml.FindChildElem((MCD_CSTR)"ComInfomation"))   //定位到下一个子元素，匹配元素名或路径。
		{
			xml.IntoElem();//进入当前主位置的下一级，当前的位置变为父位置

			while (xml.FindChildElem((MCD_CSTR)"Info"))   //定位到下一个子元素，匹配元素名或路径。
			{
				xml.IntoElem();//进入当前主位置的下一级，当前的位置变为父位置

				CString strTagName = _T("");  
				CString strData2 = _T("");  
				CString strData3 = _T("");  

				strTagName = xml.GetTagName();		//得到主位置元素（或正在进行的指令的）标签名称
				strData2 = xml.GetAttrib("Name");	// 得到当前主位置元素或节点的字符串值
				strData3 = xml.GetAttrib("Value");	// 得到当前主位置元素或节点的字符串值

				AfxMessageBox(strData2 + ":" + strData3);

				xml.OutOfElem();//使当前父位置变成当前位置 
			}

			xml.OutOfElem();//使当前父位置变成当前位置  
		}


		xml.OutOfElem();//使当前父位置变成当前位置  
	}



	return 0;
}


int TestRead(void)
{
// 	CMarkup xml;
// 
// 	xml.Load((MCD_CSTR)"E:\\新雕刻机\\trunk\\bin\\UserInfo.xml");
// 	BOOL bFind = true;
// 	xml.ResetMainPos();//将当前主位置复位为第一个兄弟位置之前
// 	while (xml.FindChildElem((MCD_CSTR)"UserID"))   //定位到下一个子元素，匹配元素名或路径。
// 	{
// 		xml.IntoElem();//进入当前主位置的下一级，当前的位置变为父位置  
// 		CString strTagName = _T("");  
// 		CString strData = _T("");  
// 		strTagName = xml.GetTagName();  //得到主位置元素（或正在进行的指令的）标签名称
// 		strData = xml.GetData(); // 得到当前主位置元素或节点的字符串值

		//以下为了看输出结果把CString 转 char*类型
// 		int len1 =WideCharToMultiByte(CP_ACP,0,strTagName,-1,NULL,0,NULL,NULL);  
// 		char ptagName[128]; 
// 		WideCharToMultiByte(CP_ACP,0,strTagName,-1,ptagName,len1,NULL,NULL );  
// 
// 		int len2 =WideCharToMultiByte(CP_ACP,0,strData,-1,NULL,0,NULL,NULL);  
// 		char pData[128];  
// 		WideCharToMultiByte(CP_ACP,0,strData,-1,pData,len2,NULL,NULL ); 
// 
// 		printf("\n-----tagName:%s,Data:%s\n",ptagName,pData);
// 		xml.OutOfElem();//使当前父位置变成当前位置  
// 	}
// 
// 	system("pause");
 	return 0;





}



/*---------------------------------------*/
//	函数说明：
//		解析xml字符串流，然后排样
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
char* LayoutOptimizeByStream(char* pszXml)
{
	vector<ComponentInputItem> m_vComponentInputItem;

	// 清空单例类的数据
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();

	// 打开文件，读取信息
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString strCurPath = ExeFile;
	strCurPath = strCurPath.Left(strCurPath.ReverseFind('\\'));


	//CString SrcFilePath = strCurPath + "\\unseodata.hgm";
	CString DstFilePath = strCurPath + "\\TEMP\\seodata.xml";
	CString TempFilePath = strCurPath + "\\TEMP";

	// 一级
	if (!PathIsDirectory(TempFilePath))  
	{  
		::CreateDirectory(TempFilePath, NULL);//创建目录,已有的话不影响  

		if (!PathIsDirectory(TempFilePath))
		{
			//AfxMessageBox(_T(TempFilePath + "目录创建失败"));
			return NULL;
		}
	} 

	SourceFilePreProccesParam param;

	if(HgmReadWrite::ReadPanelInfoByStream(pszXml, m_vComponentInputItem, param ) == true)
	{
		// 计算小板
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;                                                            
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}
	}
	else
	{
		//AfxMessageBox("hgm文件读取失败！");
		return FALSE;
	}


	// 备份输入板件组
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

	// 准备参数，开始优化
	SetBaseInfo(&(pSingleton->m_BaseInfo));

	// 排样原点
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;

	// 优化完成

	ComponentList componentList;

	// 第一段优化
	for(int i_first_op_times = 0; i_first_op_times < pSingleton->m_BaseInfo.m_FirstSectionOPTimes; i_first_op_times++)
	{
		// 清空所有数据
		pSingleton->ClearAllData();

		// 重新赋值
		ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
		SplitComponentList(componentList, pSingleton->m_vComponentGroup);

		// 优化
		if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// 最低轮廓线
		{
			pSingleton->Layout(0, CutDir_Random, Org);
		}
		else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// 贪心
		{
			pSingleton->Layout(1, CutDir_Random, Org);
		}
		else
		{
			// 组合 贪心+最低轮廓线
			int flag = /*m_valFirstSectionOPTimes*/pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

			if (i_first_op_times > flag) // 随机
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else
			{
				pSingleton->Layout(1, CutDir_Random, Org);
			}
		}

		pSingleton->BackupBestSolution();
	}

	// 更新解决方案ID和板件ID
	pSingleton->UpdateSlnNum();
	//更新板件轮廓点、孔槽等加工信息（排板过程中可能有的板件被旋转了，所以排板之后被旋转过的板件的轮廓点、孔槽也要随之旋转一下）
	pSingleton->UpdateComponentMachiningInfo();


	CMarkup xml;

	//xml.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");/

	xml.AddElem((MCD_CSTR)"Root"); //在当前主位置元素或最后兄弟位置之后增加一个元素

	//xml.SetAttrib((MCD_CSTR)"Material", (MCD_CSTR)"2");
	
	// 解决方案
 	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
 	int nSlnNum = SlnList.size();
 
 	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
 	{
 		CSolution* pCurSln = SlnList.at(i_sln);
 
 		xml.AddChildElem((MCD_CSTR)"Panel");  
 		xml.IntoElem();
 
 		CString strMaterial;
 		CString strThickness;
 		CString strCount;
 
 		strMaterial = pCurSln->m_strMaterial.GetBuffer();
 		strThickness.Format("%0.1f", pCurSln->m_fThickness);
 		strCount.Format("%d", pCurSln->GetPanelNum());
 
		xml.SetAttrib((MCD_CSTR)"Material", (MCD_CSTR)(strMaterial.GetBuffer()));
		xml.SetAttrib((MCD_CSTR)"Thickness", (MCD_CSTR)(strThickness.GetBuffer()));
		xml.SetAttrib((MCD_CSTR)"Count", (MCD_CSTR)(strCount.GetBuffer()));
 		xml.OutOfElem();
 
 	}

//	xml.OutOfElem(); //使当前父位置变成当前位置。

// 	xml.AddElem( "ORDER" );
// 
// 
// 	xml.AddChildElem( "ITEM" );
// 
// 	xml.IntoElem();
// 
// 	xml.AddChildElem( "SN");
// 
// 	xml.SetChildAttrib((MCD_CSTR)"111", (MCD_CSTR)"222");
// 
// 	xml.OutOfElem();

	static CString strResult = xml.GetDoc();

	xml.Save((MCD_CSTR)"E:\\新雕刻机\\trunk\\bin\\UserInfo.xml");     //可指定目录   将XML数据写入文件中
	


	// 导出文件
// 	if (HgyReadWrite::WriteHgy_CSharp(DstFilePath) == true)
// 	{
// 		//AfxMessageBox(DstFilePath + "文件导出成功!");
// 	}
// 	else
// 	{
// 		//AfxMessageBox("文件导出失败！");
// 		return FALSE;
// 	}




	return strResult.GetBuffer();


}



int LayoutOptimize()
{

	vector<ComponentInputItem> m_vComponentInputItem;

	// 清空单例类的数据
	CSingleon* pSingleton = CSingleon::GetSingleton();

	pSingleton->ClearAllData();

	// 打开文件，读取信息
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString strCurPath = ExeFile;
	strCurPath = strCurPath.Left(strCurPath.ReverseFind('\\'));
	

	CString SrcFilePath = strCurPath + "\\unseodata.hgm";
	CString DstFilePath = strCurPath + "\\TEMP\\seodata.xml";
	CString TempFilePath = strCurPath + "\\TEMP";

	// 一级
	if (!PathIsDirectory(TempFilePath))  
	{  
		::CreateDirectory(TempFilePath, NULL);//创建目录,已有的话不影响  

		if (!PathIsDirectory(TempFilePath))
		{
			//AfxMessageBox(_T(TempFilePath + "目录创建失败"));
			return FALSE;
		}
	} 

	SourceFilePreProccesParam param;

	if(HgmReadWrite::ReadPanelInfo(SrcFilePath, m_vComponentInputItem, param ) == true)
	{
		// 计算小板
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;                                                            
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}
	}
	else
	{
		//AfxMessageBox("hgm文件读取失败！");
		return FALSE;
	}

	// 备份输入板件组
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);

	// 准备参数，开始优化
	SetBaseInfo(&(pSingleton->m_BaseInfo));

	// 排样原点
	int Org = pSingleton->m_BaseInfo.m_LayoutOrg;

	// 优化完成

	ComponentList componentList;

	// 第一段优化
	for(int i_first_op_times = 0; i_first_op_times < pSingleton->m_BaseInfo.m_FirstSectionOPTimes; i_first_op_times++)
	{
		// 清空所有数据
		pSingleton->ClearAllData();

		// 重新赋值
		ConvertInputInfoToComponentList(m_vComponentInputItem, componentList);
		SplitComponentList(componentList, pSingleton->m_vComponentGroup);

		// 优化
		if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// 最低轮廓线
		{
			pSingleton->Layout(0, CutDir_Random, Org);
		}
		else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// 贪心
		{
			pSingleton->Layout(1, CutDir_Random, Org);
		}
		else
		{
			// 组合 贪心+最低轮廓线
			int flag = /*m_valFirstSectionOPTimes*/pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

			if (i_first_op_times > flag) // 随机
			{
				pSingleton->Layout(0, CutDir_Random, Org);
			}
			else
			{
				pSingleton->Layout(1, CutDir_Random, Org);
			}
		}

		pSingleton->BackupBestSolution();
	}

	// 更新解决方案ID和板件ID
	pSingleton->UpdateSlnNum();
	//更新板件轮廓点、孔槽等加工信息（排板过程中可能有的板件被旋转了，所以排板之后被旋转过的板件的轮廓点、孔槽也要随之旋转一下）
	pSingleton->UpdateComponentMachiningInfo();

	// 导出文件
	if (HgyReadWrite::WriteHgy_CSharp(DstFilePath) == true)
	{
		//AfxMessageBox(DstFilePath + "文件导出成功!");
	}
	else
	{
		//AfxMessageBox("文件导出失败！");
		return FALSE;
	}


	return TRUE;
}



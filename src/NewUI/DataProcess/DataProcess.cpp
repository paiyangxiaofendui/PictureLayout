#include "stdafx.h"
#include "DataProcess.h"
#include "../Misc/Misc.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Misc/YHInfoTools.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/Algorithm/Algorithm.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
#include "../../../include/KnifeDll/KnifeClass.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../Misc/ProgramMisc.h"
#include <algorithm>
//#include "../UI/ParamSettingDlg.h"
#include <string>
#include <algorithm>
//#include "../MainFrm.h"
//#include "../UI/ParamDlg.h"

#define UNAVALIBLE_KNIFE_DOWN_DISTANCE 20

void GetInflatedComponentRectOutline(PointFloat ptLeftTopInPanel, float fComponentXLen, float fComponentYLen, float fKerf, pair<PointFloat, PointFloat> (&aComponentOutline)[4]);
void GetInflatedComponentRectVertex(PointFloat ptLeftTopInPanel, float fComponentXLen, float fComponentYLen, float fKerf, PointFloat (&aComponentVertex)[4]);

bool IsRemainedEnough(vector<ComponentInputItem>& vComponentInputItem, map<string, int>& mapBarcodeToNeedCount);//剩下的板件是否足以进行预组合

int CalcKnifeDownPosOfComponent(Component& theComponent, vector<OutlineLine>& vAllCuttedLine, float fKerf, float fKnifeDownDistance, float fPanelXLen, float fPanelYLen);
float GetDistanceBetweenPointAndCuttedLine(OutlineLine line, float fXKnifeDown, float fYKnifeDown, vector<OutlineLine>& vAllCuttedLine);
float GetNotCuttedOutlineLineLen(OutlineLine line, vector<OutlineLine>& vAllCuttedLine, float fKerf);

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
			Component *pCpn = ConvertOneInputInfoToOneComponent(theInputItem);
			pCpn->m_CpnID = nID;

			componentList.push_back(pCpn);

			nID++;
		}
	}
}

void ConvertInputInfoToComponentList(vector<ComponentInputItem>& vComponentInputItem, vector<PreCombineItem>& vPreCombineItem, ComponentList& componentList)
{
	componentList.clear();
	vector<ComponentInputItem> vComponentInputItem_Tmp = vComponentInputItem;
	for(int i = 0; i < vPreCombineItem.size(); i++) //对各个预组合项，检查是否有剩余的板件来组合，如有，就组合为一个Component，让排版算法去排这个组合出来的Component
	{
		PreCombineItem& thePreCombineItem = vPreCombineItem[i];
		bool bIsRemainedEnough = true;
		map<string, int> mapBarcodeToNeedCount;
		for(int j = 0; j < thePreCombineItem.size(); j++)//统计这种组合需要的板件的数量
		{
			PreCombineComponent& thePreCombineComponent = thePreCombineItem[j];
			map<string, int>::iterator itrTmp = mapBarcodeToNeedCount.find(thePreCombineComponent.m_Item.m_strBarcode.GetBuffer());
			if(itrTmp == mapBarcodeToNeedCount.end())
				mapBarcodeToNeedCount[thePreCombineComponent.m_Item.m_strBarcode.GetBuffer()] = 1;
			else
				itrTmp->second++;
		}

		while(IsRemainedEnough(vComponentInputItem_Tmp, mapBarcodeToNeedCount))//只要还有剩余的板件组合，就组合
		{
			Component *pCpn = new Component;
			float fLeft, fTop, fRight, fBottom;
			GetPreCombineItemRect(&thePreCombineItem, fLeft, fTop, fRight, fBottom);//获取组合出来的Component的尺寸
			pCpn->m_RealLength = fRight - fLeft;
			pCpn->m_RealWidth = fTop - fBottom;
			pCpn->m_Thickness = thePreCombineItem[0].m_Item.m_fThickness;//以第一块板的厚度材质纹理来设置组合板的厚度材质纹理
			pCpn->m_Material = thePreCombineItem[0].m_Item.m_strMaterial;

			CString strTexture = thePreCombineItem[0].m_Item.m_strTexture;
			if(strTexture.CompareNoCase("横纹") == 0)
				pCpn->m_Texture = TextureType_H_TEXTURE;
			else if(strTexture.CompareNoCase("竖纹") == 0)
				pCpn->m_Texture = TextureType_V_TEXTURE;
			else if(strTexture.CompareNoCase("无纹理") == 0)
				pCpn->m_Texture = TextureType_NO_TEXTURE;
			else
				pCpn->m_Texture = TextureType_H_TEXTURE;

			pCpn->m_type = NodeType_NeededComponent;
			pCpn->m_x = 0;
			pCpn->m_y = 0;

			for(int l = 0; l < thePreCombineItem.size(); l++)//把被组合的板件存进组合板里
			{
				PreCombineComponent& thePreCombineComponent = thePreCombineItem[l];
				Component* pCpnCombined = ConvertOneInputInfoToOneComponent(thePreCombineComponent.m_Item);
				pCpnCombined->m_pParent = pCpn;
				pCpnCombined->m_x = thePreCombineComponent.m_fX_Left - fLeft;
				pCpnCombined->m_y = thePreCombineComponent.m_fY_Bottom - fBottom;
				pCpnCombined->m_nRotatedAngle = thePreCombineComponent.m_nRotatedAngleWhenCombine;
				pCpn->m_ChildrenList.push_back(pCpnCombined);
			}
			

			componentList.push_back(pCpn);

			for(map<string, int>::iterator itr = mapBarcodeToNeedCount.begin(); itr != mapBarcodeToNeedCount.end(); itr++)
			{
				for(int k = 0; k < vComponentInputItem_Tmp.size(); k++)
				{
					if(vComponentInputItem_Tmp[k].m_strBarcode.Compare(itr->first.c_str()) == 0)
					{
						vComponentInputItem_Tmp[k].m_nCount -= itr->second;
						break;
					}
				}
			}
		}
	}

	for(int i = 0; i < vComponentInputItem_Tmp.size(); i++)//预组合后剩下的板件
	{
		ComponentInputItem& theInputItem = vComponentInputItem_Tmp[i];
		for(int j = 0; j < theInputItem.m_nCount; j++)
		{
			Component *pCpn = ConvertOneInputInfoToOneComponent(theInputItem);
			componentList.push_back(pCpn);
		}
	}
}

bool IsRemainedEnough(vector<ComponentInputItem>& vComponentInputItem, map<string, int>& mapBarcodeToNeedCount)
{
	bool bIsRemainedEnough = true;
	for(map<string, int>::iterator itr = mapBarcodeToNeedCount.begin(); itr != mapBarcodeToNeedCount.end(); itr++)
	{
		bool bOneBarcodeEnough = false;
		for(int k = 0; k < vComponentInputItem.size(); k++)
		{
			if(vComponentInputItem[k].m_strBarcode.Compare(itr->first.c_str()) == 0)
			{
				if(vComponentInputItem[k].m_nCount >= itr->second)
					bOneBarcodeEnough = true;
				break;
			}
		}
		if(!bOneBarcodeEnough)
		{
			bIsRemainedEnough = false;
			break;
		}
	}
	return bIsRemainedEnough;
}

Component* ConvertOneInputInfoToOneComponent(ComponentInputItem& theInputItem)
{
	Component *pCpn = new Component;
	//pCpn->m_CpnID = nID;
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

	CString strRemoveCh = "异";
	CString strOtherShapeID = theInputItem.m_strOtherShapeID.Right(theInputItem.m_strOtherShapeID.GetLength() - strRemoveCh.GetLength());
	pCpn->m_nOtherShapeID = atoi(strOtherShapeID.GetBuffer());
	strOtherShapeID.ReleaseBuffer();

	pCpn->m_vOutlinePoint = OtherShapeHelper::GetPointInfoFromPointInfoString(theInputItem.m_strAbnormalInfo.GetBuffer());		// 设置异形数据信息
	YHInfoTools::getAllVec(pCpn->m_vUpperFaceHole, theInputItem.m_strUpperFaceHoleInfo, theInputItem.m_strBarcode);				// 设置正面孔信息
	YHInfoTools::getAllSlot(pCpn->m_vUpperFaceSlot, theInputItem.m_strUpperFaceSlotInfo, "", theInputItem.m_strBarcode);		// 设置正面槽信息
	YHInfoTools::getAllVec(pCpn->m_vDownerFaceHole, theInputItem.m_strDownerFaceHoleInfo, theInputItem.m_strBarcode);			// 设置正面孔信息
	YHInfoTools::getAllSlot(pCpn->m_vDownerFaceSlot, theInputItem.m_strDownerFaceSlotInfo, "", theInputItem.m_strBarcode);		// 设置正面槽信息
	for(int i = 0; i < pCpn->m_vDownerFaceHole.size(); i++)
	{
		pCpn->m_vDownerFaceHole[i]._x = pCpn->m_RealLength - pCpn->m_vDownerFaceHole[i]._x;
	}
	for(int i = 0; i < pCpn->m_vDownerFaceSlot.size(); i++)
	{
		pCpn->m_vDownerFaceSlot[i]._x = pCpn->m_RealLength - pCpn->m_vDownerFaceSlot[i]._x;
	}



	pCpn->m_strOrderID = theInputItem.m_strOrderID;               //订单号
	pCpn->m_strCabinetID = theInputItem.m_strCabinetID;            //柜号
	memcpy(pCpn->m_afBanding, theInputItem.m_afBanding, sizeof(float)*4);               //封边
	pCpn->m_bSlotFlipped = theInputItem.m_bSlotFlipped;               //槽翻转
	pCpn->m_bVHoleFlipped = theInputItem.m_bVHoleFlipped;              //孔翻转
	pCpn->m_strCustomerInfo = theInputItem.m_strCustomerInfo;         //客户信息
	pCpn->m_strJoinedStore = theInputItem.m_strJoinedStore;         //加盟店
	pCpn->m_strSlottingFlag = theInputItem.m_strSlottingFlag;         //拉槽标识
	pCpn->m_strReminder = theInputItem.m_strReminder;            //助记号
	pCpn->m_strDrilling = theInputItem.m_strDrilling;             //钻孔
	pCpn->m_strOrderType = theInputItem.m_strOrderType;             //订单类型
	pCpn->m_strReverseSideBarcode = theInputItem.m_strReverseSideBarcode;    //反面条码
	pCpn->m_fProductLength = theInputItem.m_fProductLength;            //成品长度
	pCpn->m_fProductWidth = theInputItem.m_fProductWidth;              //成品宽度
	pCpn->m_fProductThickness = theInputItem.m_fProductThickness;          //成品厚度
	pCpn->m_strCustomerAddress = theInputItem.m_strCustomerAddress;      //客户地址
	pCpn->m_strHoleSlotFlag = theInputItem.m_strHoleSlotFlag;        //钻槽标识
	pCpn->m_strPanelClass = theInputItem.m_strPanelClass;            //板件类型,"Door"为门板，"Panel"为普通板
	pCpn->m_strSendingDate = theInputItem.m_strSendingDate;          //发货日期
	memcpy(pCpn->m_aHasSideHole, theInputItem.m_aHasSideHole, sizeof(pCpn->m_aHasSideHole)); //是否有侧孔（左、前、右、后）

	return pCpn;
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
			
			// 查找已有分组中的同材料分组，找到比当前薄的分组，插在指定组前面，否则，插在后面
			
			bool insert_flag = false;
			vector<ComponentList>::iterator itr_group;


			for (itr_group = SplitComponentGroup.begin(); itr_group != SplitComponentGroup.end(); itr_group++)
			{
				ComponentList& cur_list = *itr_group;
				Component* pSplitComponent = cur_list[0];

				// 材质相同 新厚度大于已有厚度，插入
				if (pSplitComponent->m_Material == pSrcComponent->m_Material && 
					pSrcComponent->m_Thickness > pSplitComponent->m_Thickness)
				{
					insert_flag = true;
					break;
				}
			}

			if (insert_flag == true)
			{
				SplitComponentGroup.insert(itr_group, componentList);
			}
			else
			{
				SplitComponentGroup.push_back(componentList);
			}



		}
	}


}

void ProcessOptimize(/*CSingleton& data*/)
{
	CSingleton* pSingleton = CSingleton::GetSingleton();

	//pSingleton->Layout();
	//pSingleton->BackupBestSolution();

	CString str;
	
	str.Format("%d", pSingleton->GetBackupSolutionPanelNum());
	str += "块大板,";
	str = "目前最优:" + str;

	MessageBox(NULL, str, "", MB_OK);

}

void PreProcessOfExportGCode()
{
	vector<Component*> vAllComponent;
	FindAllComponentInSingleton(vAllComponent);

	for(int j = 0; j < vAllComponent.size(); j++)
	{
		Component& theComponent = *(vAllComponent[j]);
		for(int i = 0; i < theComponent.m_vDownerFaceHole.size(); i++)
		{
			theComponent.m_vDownerFaceHole[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceHole[i]._x;
		}
		for(int i = 0; i < theComponent.m_vDownerFaceSlot.size(); i++)
		{
			theComponent.m_vDownerFaceSlot[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceSlot[i]._x;
		}
	}

}

void PostProcessOfExportGCode()
{
	vector<Component*> vAllComponent;
	FindAllComponentInSingleton(vAllComponent);

	for(int j = 0; j < vAllComponent.size(); j++)
	{
		Component& theComponent = *(vAllComponent[j]);
		for(int i = 0; i < theComponent.m_vDownerFaceHole.size(); i++)
		{
			theComponent.m_vDownerFaceHole[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceHole[i]._x;
		}
		for(int i = 0; i < theComponent.m_vDownerFaceSlot.size(); i++)
		{
			theComponent.m_vDownerFaceSlot[i]._x = theComponent.m_RealLength - theComponent.m_vDownerFaceSlot[i]._x;
		}
	}
}

//某几个异形无法用正常方法计算贴标座标，判断该异形是否是那种
bool IsSpecialOtherShape(int nOtherShapeID)
{
	bool bRet = false;

	char* a_szSpecialOtherSharID[] = {"500", "501", "502", "503", "520", "521", "522", "523", "540", "541", "542", "543", "560", "561", "562", "563"};

	if(nOtherShapeID > 0)
	{
		CString strID;
		strID.Format(_T("%d"), nOtherShapeID);

		int nSize = sizeof(a_szSpecialOtherSharID)/sizeof(char*);
		for(int i = 0; i < nSize; i++)
		{
			if(strID.CompareNoCase(a_szSpecialOtherSharID[i]) == 0)
			{
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}

//为板件创建贴标信息，传入板件整板参数，传入贴标标签宽高，得到贴标应该以哪个点为顶点坐标
PointFloat createTiebiaoPosition(Component& theComponent,float tiebiao_width, float tiebiao_height, int nLabelPosType, float fXGap, float fYGap, bool bLabelInUpperSide)
{
	float panelWidth = theComponent.m_RealLength;
	float panelHeight = theComponent.m_RealWidth;

	if(!IsSpecialOtherShape(theComponent.m_nOtherShapeID))
	{
		std::vector<PointFloat> tryPointList = createPointMatrixExtend(panelWidth,panelHeight,tiebiao_width,tiebiao_height, nLabelPosType, fXGap, fYGap);

		//把设想的点传递到数据里面尝试是否合适，合适则返回
		for (int i = 0; i < tryPointList.size() ; i++)
		{
			if (isTieBiaoPositionUsable(theComponent,tryPointList[i],tiebiao_width,tiebiao_height, bLabelInUpperSide))
			{
				return tryPointList[i];
			}
		}
	}

	//不行则采取中点为贴标坐标
	PointFloat ptRet = GetDefaultLabelPos(theComponent, tiebiao_width, tiebiao_height, nLabelPosType, fXGap, fYGap);

	return ptRet;
}

//创建贴标系列点阵，用于尝试(每个点坐标是贴标位置的中心点)
vector<PointFloat> createPointMatrixExtend(float panelWidth,float panelHeight, float tiebiao_width, float tiebiao_height, int nLabelPosType, float fXGap, float fYGap)
{
	std::vector<PointFloat> result;

	PointFloat center;
	switch(nLabelPosType)
	{
	case LabelPosType::LABEL_POS_LEFTTOP:
		center.x = tiebiao_width/2 + fXGap;
		center.y = tiebiao_height/2 + fYGap;
		break;
	case LabelPosType::LABEL_POS_RIGHTTOP:
		center.x = panelWidth - (tiebiao_width/2 + fXGap);
		center.y = tiebiao_height/2 + fYGap;
		break;
	case LabelPosType::LABEL_POS_LEFTBOTTOM:	
		center.x = tiebiao_width/2 + fXGap;
		center.y = panelHeight - (tiebiao_height/2 + fYGap);
		break;
	case LabelPosType::LABEL_POS_RIGHTBOTTOM:
		center.x = panelWidth - (tiebiao_width/2 + fXGap);
		center.y = panelHeight - (tiebiao_height/2 + fYGap);		
		break;
	default:
		center.x = panelWidth/2;
		center.y = panelHeight/2;
		break;
	}
	result.push_back(center);

	float setpw = (panelWidth - tiebiao_width - tiebiao_width) /15;
	float setph = (panelHeight - tiebiao_height - tiebiao_height) /15;

	//板件不足以放标签的时候直接返回中心点
	if (setpw < 0.1 || setph < 0.1 )
	{
		if (setpw < 0.1 && setph < 0.1 )
		{
			return result;
		} 
		setpw = setpw < 0.1 ? 0.1 : setpw;
		setph = setph < 0.1 ? 0.1 : setph;
	}

	for (float i = (tiebiao_width/2); i < (panelWidth - (tiebiao_width/2)) ; i = i + setpw )
	{
		for (float j = (tiebiao_height/2); j < (panelHeight - (tiebiao_height/2)) ; j = j + setph )
		{
			PointFloat pt;
			pt.x = i;
			pt.y = j;
			result.push_back(pt);
		}
	}

	std::sort(result.begin(),result.end(),
		[&](const PointFloat& p1,const PointFloat& p2)->bool{
			float p1_x = (p1.x - center.x) > 0 ? (p1.x - center.x) : (center.x - p1.x);
			float p1_y = (p1.y - center.y) > 0 ? (p1.y - center.y) : (center.y - p1.y);
			float p2_x = (p2.x - center.x) > 0 ? (p2.x - center.x) : (center.x - p2.x);
			float p2_y = (p2.y - center.y) > 0 ? (p2.y - center.y) : (center.y - p2.y);
			return (p1_x + p1_y) < (p2_x + p2_y);
	}
	);

	return result;
}

#define TieBiaoDebug 0

//判断这个贴标是否合法
bool isTieBiaoPositionUsable(Component& theComponent, PointFloat point, float tiebiao_width , float tiebiao_height, bool bLabelInUpperSide)
{
// 	try
// 	{
// #if (TieBiaoDebug==1)
// 		Graphics g(this->GetDC()->m_hDC);
// 		g.Clear(Color::White);
// #endif
// 
// 		Bitmap img(2440,1220); 
// 		Graphics graphics((Image*)&img);
// 		Region regionCompare;
// 
// 		//当前标签，当前区域
// 
// 		GraphicsPath path;
// 		path.StartFigure();
// 		RectF rectTiebiao(
// 			point.x - tiebiao_width/2,
// 			point.y - tiebiao_height/2,
// 			tiebiao_width,
// 			tiebiao_height);
// 
// 
// 		path.AddRectangle(rectTiebiao);
// 		path.CloseFigure();
// 		Region regionTiebiao(&path);
// 		//graphics.FillRegion(&SolidBrush(Color::Blue),&regionTiebiao);
// 
// 		ParamSettingDlg settings;
// 		settings.loadSettings();
// 
// 		//当前区域，与所有槽区域，有交集返回false
// #if 1
// 		if (settings.Tiebiao_auto_slot())
// 		{
// 			//std::string sSlot = map_one_matrial.at("slotInfo");//槽
// 			//std::vector<SOLT> v_solt = CVecSoltDlg::TraniferSolt(sSlot);		//解构正面槽
// 
// 			GraphicsPath pathSlot;
// 			//画正面槽
// 			vector<AllSlot>& vTheSlot = bLabelInUpperSide ? theComponent.m_vUpperFaceSlot : theComponent.m_vDownerFaceSlot;
// 			for (int i = 0;i < vTheSlot.size();i++)
// 			{
// 				float s_x = /*v_solt.at(i)._x*/vTheSlot[i]._x;
// 				float s_y = /*v_solt.at(i)._y*/theComponent.m_RealWidth - vTheSlot[i]._y;
// 				float s_width = /*v_solt.at(i)._width*/vTheSlot[i]._width;
// 				float s_height = /*v_solt.at(i)._height*/vTheSlot[i]._height;
// 
// 				int s_direction = /*v_solt.at(i)._direction*/vTheSlot[i]._dir;
// 
// 				RectF rect;
// 				if (s_direction == 0)	//0是竖向
// 				{
// 					rect.X = s_x - (s_height/2.0);
// 					rect.Y = s_y - (s_width/2.0);
// 					rect.Width = s_height;
// 					rect.Height = s_width;
// 				}
// 				else	// 1 是横向
// 				{
// 					rect.X = s_x - (s_width/2.0);
// 					rect.Y = s_y - (s_height/2.0);
// 					rect.Width = s_width;
// 					rect.Height = s_height;
// 				}
// 
// 				pathSlot.StartFigure();
// 				pathSlot.AddRectangle(rect);
// 				pathSlot.CloseFigure();
// 			}
// 			Region regionSolt(&pathSlot);
// 
// 			regionCompare.MakeInfinite();
// 			regionCompare.Intersect(&regionTiebiao);
// 			regionCompare.Intersect(&regionSolt);
// 
// 
// 
// 			if ( !(regionCompare.IsEmpty(&graphics)) ) 
// 			{
// 				return false;
// 			}
// #if (TieBiaoDebug==1)
// 			g.FillRegion(&SolidBrush(Color::Blue),&regionSolt);
// #endif
// 
// 		}
// #endif
// 
// 		//当前区域，与所有孔区域，有交集返回false
// #if 1
// 		if (settings.Tiebiao_auto_vec())
// 		{
// 			//std::string sVec = map_one_matrial.at("vecInfo");//孔
// 			//std::vector<VEC> v_vec = CVecSoltDlg::TraniferVec(sVec);			//解构正面孔
// 
// 			//画正面孔
// 			GraphicsPath pathVec;
// 			vector<AllVec>& vTheHole = bLabelInUpperSide ? theComponent.m_vUpperFaceHole : theComponent.m_vDownerFaceHole;
// 			//for (int i = 0;i < v_vec.size();i++)
// 			for (int i = 0;i < vTheHole.size();i++)
// 			{
// 				float v_x = /*v_vec.at(i)._x*/vTheHole[i]._x;
// 				float v_y = /*v_vec.at(i)._y*/theComponent.m_RealWidth - vTheHole[i]._y;
// 				float v_r = /*v_vec.at(i)._r*/vTheHole[i]._r;
// 
// 				RectF rect;
// 				rect.X = v_x - v_r;
// 				rect.Y = v_y - v_r;
// 				rect.Width = v_r + v_r ;
// 				rect.Height = v_r + v_r;
// 
// 				pathVec.StartFigure();
// 				pathVec.AddRectangle(rect);
// 				pathVec.CloseFigure();
// 			}
// 			Region regionVec(&pathVec);
// 
// 			regionCompare.MakeInfinite();
// 			regionCompare.Intersect(&regionTiebiao);
// 			regionCompare.Intersect(&regionVec);
// 
// 			if ( !(regionCompare.IsEmpty(&graphics)) ) 
// 			{
// 				return false;
// 			}
// #if (TieBiaoDebug==1)
// 			g.FillRegion(&SolidBrush(Color::Blue),&regionVec);
// #endif
// 
// 		}
// #endif
// 
// 
// 		//当前区域，与所有其他区域，有交集返回false
// 		//构建异形数据
// #if 1
// 		//std::string sOthershape = map_one_matrial.at("OtherPoints");//异形点信息
// 		//std::vector<PointInfo> pointOtherShape = OtherShapeHelper::GetPointInfoFromPointInfoString(sOthershape);
// 		std::vector<PointInfo> pointOtherShape = theComponent.m_vOutlinePoint;
// 		for(int j = 0; j < pointOtherShape.size(); j++)
// 		{
// 			pointOtherShape[j].y = theComponent.m_RealWidth - pointOtherShape[j].y;
// 		}
// 		std::vector<InfoLine> lines = OtherShapeHelper::GetInfoLineFromPointInfo(pointOtherShape);
// 		std::vector<std::vector<InfoLine>> tempLineGroup = OtherShapeHelper::GetInfoLineGroup(lines);
// 
// 		if (tempLineGroup.size() == 0)
// 		{
// 			OutputDebugString("贴标区域处理轮廓点错误");
// 			return false;
// 		}
// 		std::vector<InfoLine> outlines = tempLineGroup[tempLineGroup.size() - 1];
// 		tempLineGroup.pop_back();
// 		std::vector<std::vector<InfoLine>> inlinesGroup = tempLineGroup;
// 		if(lines.size() == 0)
// 		{
// 			OutputDebugString("贴标区域处理轮廓点错误");
// 			return false;
// 		}
// #endif
// 
// 
// 
// 		//画内部所有区域
// #if 1
// 		if (settings.Tiebiao_auto_inner())
// 		{
// 			inlinesGroup;
// 			GraphicsPath pathInlines;
// 			for (int indexInlinesGroup = 0; indexInlinesGroup < inlinesGroup.size() ; indexInlinesGroup++)
// 			{
// 				GraphicsPath pathLines;
// 				pathInlines.StartFigure();
// 				for (int indexInlines = 0; indexInlines < inlinesGroup[indexInlinesGroup].size() ; indexInlines++)
// 				{
// 					InfoLine oneline = inlinesGroup[indexInlinesGroup][indexInlines];
// 					if ( oneline.r == 0.0 )
// 					{
// 						pathInlines.AddLine(oneline.begin.x,oneline.begin.y,oneline.end.x,oneline.end.y);
// 					}
// 					else if ( oneline.r > 0.0 )
// 					{
// 						RectF rect;
// 						double beginAngle;
// 						double endAngle;
// 						OtherShapeHelper::getArc(
// 							OtherPointFloat(oneline.begin.x,oneline.begin.y),
// 							OtherPointFloat(oneline.end.x,oneline.end.y),
// 							oneline.r,oneline.dir,
// 							rect,beginAngle,endAngle);
// 						pathInlines.AddArc(rect,beginAngle,endAngle);
// 					}
// 					else if ( oneline.r < 0.0 )
// 					{
// 						int dir = oneline.dir == 1 ? 2 : 1;//影响圆心计算方式，不改变原来dir
// 
// 						RectF rect;
// 						double beginAngle;
// 						double endAngle;
// 						OtherShapeHelper::getArc(
// 							OtherPointFloat(oneline.begin.x,oneline.begin.y),
// 							OtherPointFloat(oneline.end.x,oneline.end.y),
// 							-1* oneline.r,dir,
// 							rect,beginAngle,endAngle);
// 						pathInlines.AddArc(rect,beginAngle,endAngle);
// 					}
// 				}
// 				pathInlines.CloseFigure();
// 
// 				pathInlines.AddPath(&pathLines,false);
// 			}
// 			Region regionInlines(&pathInlines);
// 
// 
// 			regionCompare.MakeInfinite();
// 			regionCompare.Intersect(&regionTiebiao);
// 			regionCompare.Intersect(&regionInlines);
// 			if ( !(regionCompare.IsEmpty(&graphics)) )
// 			{
// 				return false;
// 			}
// 		}
// #endif
// 
// 		//当前区域，与主区域，有交集返回true
// #if 1
// 		if (settings.Tiebiao_auto_outline())
// 		{
// 			GraphicsPath pathOutlines;
// 			pathOutlines.StartFigure();
// 			for (int indexOutlines = 0; indexOutlines < outlines.size() ; indexOutlines++)
// 			{
// 				InfoLine oneline = outlines[indexOutlines];
// 				if (OtherShapeHelper::isPointFloatEquals(oneline.begin.x,oneline.begin.y,oneline.end.x,oneline.end.y))
// 				{
// 					//长度为0的线段不操作
// 				} 
// 				else if ( oneline.r == 0.0 )
// 				{
// 					pathOutlines.AddLine(oneline.begin.x,oneline.begin.y,oneline.end.x,oneline.end.y);
// 				}
// 				else if ( oneline.r > 0.0 )
// 				{
// 					RectF rect;
// 					double beginAngle;
// 					double endAngle;
// 					OtherShapeHelper::getArc(
// 						OtherPointFloat(oneline.begin.x,oneline.begin.y),
// 						OtherPointFloat(oneline.end.x,oneline.end.y),
// 						oneline.r,oneline.dir,
// 						rect,beginAngle,endAngle);
// 					pathOutlines.AddArc(rect,beginAngle,endAngle);
// 				}
// 				else if ( oneline.r < 0.0 )
// 				{
// 					int dir = oneline.dir == 1 ? 2 : 1;//影响圆心计算方式，不改变原来dir
// 
// 					RectF rect;
// 					double beginAngle;
// 					double endAngle;
// 					OtherShapeHelper::getArc(
// 						OtherPointFloat(oneline.begin.x,oneline.begin.y),
// 						OtherPointFloat(oneline.end.x,oneline.end.y),
// 						-1* oneline.r,dir,
// 						rect,beginAngle,endAngle);
// 					pathOutlines.AddArc(rect,beginAngle,endAngle);
// 				}
// 			}
// 			pathOutlines.CloseFigure();
// 			Region regionOutlines(&pathOutlines);
// 
// 			regionCompare.MakeInfinite();
// 			regionCompare.Exclude(&regionOutlines);	//板件区域的补集
// 			regionCompare.Intersect(&regionTiebiao);	//和贴标区域相交
// 
// 			if ( !(regionCompare.IsEmpty(&graphics)) )	//如果是非空，代表贴标部分超出交集，不可行
// 			{
// 				return false;
// 			}
// 
// #if (TieBiaoDebug==1)
// 			g.DrawPath(&Pen(Color::Blue),&pathOutlines);
// 			//g.FillRegion(&SolidBrush(Color::Blue),&regionOutlines);
// 			g.FillRegion(&SolidBrush(Color::Yellow),&regionTiebiao);
// #endif
// 		}
// #endif
// 
// 	}
// 	catch (...)
// 	{
// 		return false;
// 	}
	return true;
}

PointFloat GetDefaultLabelPos(Component& theComponent, float tiebiao_width, float tiebiao_height, int nLabelPosType, float fXGap, float fYGap, bool bForce)
{
	PointFloat ptRet;

	float panelWidth = theComponent.m_RealLength;
	float panelHeight = theComponent.m_RealWidth;

	switch(nLabelPosType)
	{
	case LabelPosType::LABEL_POS_LEFTTOP:
		ptRet.x = tiebiao_width/2 + fXGap;
		ptRet.y = tiebiao_height/2 + fYGap;
		break;
	case LabelPosType::LABEL_POS_RIGHTTOP:
		ptRet.x = panelWidth - (tiebiao_width/2 + fXGap);
		ptRet.y = tiebiao_height/2 + fYGap;
		break;
	case LabelPosType::LABEL_POS_LEFTBOTTOM:
		ptRet.x = tiebiao_width/2 + fXGap;
		ptRet.y = panelHeight - (tiebiao_height/2 + fYGap);
		break;
	case LabelPosType::LABEL_POS_RIGHTBOTTOM:
		ptRet.x = panelWidth - (tiebiao_width/2 + fXGap);
		ptRet.y = panelHeight - (tiebiao_height/2 + fYGap);
		break;
	default:
		ptRet.x = panelWidth/2;
		ptRet.y = panelHeight/2;
		break;
	}

	if(IsSpecialOtherShape(theComponent.m_nOtherShapeID) && !bForce)
	{
		ptRet.x = panelWidth/2;
		ptRet.y = panelHeight/2;
	}

	CRect rcLabel;
	rcLabel.SetRect(ptRet.x - tiebiao_width/2, ptRet.y - tiebiao_height/2, ptRet.x + tiebiao_width/2, ptRet.y + tiebiao_height/2);
	CRect rcComponent;
	rcComponent.SetRect(0, 0, panelWidth, panelHeight);
	CRect rcInterset;
	rcInterset.IntersectRect(rcComponent, rcLabel);
	if(!rcInterset.IntersectRect(rcComponent, rcLabel) || !rcInterset.EqualRect(rcLabel))
	//if(ptRet.x + tiebiao_width/2 > panelWidth || ptRet.y + tiebiao_height/2 > panelHeight)
	{
		ptRet.x = panelWidth/2;
		ptRet.y = panelHeight/2;
	}
	
	return ptRet;
}

PointFloat GetAttachPos_Component(Panel* pPanel, PointFloat ptMouseInPanel, float fComponentXLen, float fComponentYLen, float fKerf, int nAttachDistance)
{
	PointFloat ptRet = ptMouseInPanel;

	//vector<pair<PointFloat, PointFloat>> vAllAttachableLineInPanel;
	//pair<PointFloat, PointFloat> aComponentOutline[4];
	//GetInflatedComponentRectOutline(ptMouseInPanel, fComponentXLen, fComponentYLen, fKerf, aComponentOutline);
	vector<PointFloat> vAllAttachableVertexInPanel;
	PointFloat aPastingComponentVertex[4];
	GetInflatedComponentRectVertex(ptMouseInPanel, fComponentXLen, fComponentYLen, fKerf, aPastingComponentVertex);

	PointFloat aPanelVertex[4];
	PointFloat ptPanelLeftTop;
	ptPanelLeftTop.x = pPanel->m_OriginX;
	ptPanelLeftTop.y = pPanel->m_OriginY + pPanel->m_RealWidth;
	GetInflatedComponentRectVertex(ptPanelLeftTop, pPanel->m_RealLength, pPanel->m_RealWidth, fKerf, aPanelVertex);
	for(int i = 0; i < 4; i++)
		vAllAttachableVertexInPanel.push_back(aPanelVertex[i]);

	vector<Component*> vAllComponent;
	//FindAllComponentInSingleton(vAllComponent);
	FindAllComponentInPanel(*pPanel, vAllComponent);
	for(int i = 0; i < vAllComponent.size(); i++)
	{
		PointFloat aComponentVertex[4];
		PointFloat ptComponentLeftTop;
		ptComponentLeftTop.x = vAllComponent[i]->m_x;
		ptComponentLeftTop.y = vAllComponent[i]->m_y + vAllComponent[i]->m_RealWidth;
		GetInflatedComponentRectVertex(ptComponentLeftTop, vAllComponent[i]->m_RealLength, vAllComponent[i]->m_RealWidth, fKerf, aComponentVertex);
		for(int j = 0; j < 4; j++)
			vAllAttachableVertexInPanel.push_back(aComponentVertex[j]);
	}

	float fMinDistanceForVertex[] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
	PointFloat ptMinDistanceForVertex[4];
	for(int i = 0; i < 4; i++)
	{
		PointFloat theVertex = aPastingComponentVertex[i];
		for(int j = 0; j < vAllAttachableVertexInPanel.size(); j++)
		{
			PointFloat theCompareVertex = vAllAttachableVertexInPanel[j];
			float fDistance = DistanceBetweenPoints(theVertex.x, theVertex.y, theCompareVertex.x, theCompareVertex.y);
			if(fDistance < fMinDistanceForVertex[i])
			{
				fMinDistanceForVertex[i] = fDistance;
				ptMinDistanceForVertex[i] = theCompareVertex;
			}
		}
	}

	int nMinDistanceVertexID = -1;
	float fMinDistance = FLT_MAX;
	for(int i = 0; i < 4; i++)
	{
		if(fMinDistanceForVertex[i] < fMinDistance && fMinDistanceForVertex[i] <= nAttachDistance)
		{
			nMinDistanceVertexID = i;
			fMinDistance = fMinDistanceForVertex[i];
		}
	}

	if(nMinDistanceVertexID != -1)
	{
		switch(nMinDistanceVertexID)
		{
		case LEFT_TOP:
			ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x + fKerf/2;
			ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y - fKerf/2;
			break;
		case LEFT_BOTTOM:
			ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x + fKerf/2;
			ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y + fKerf/2 + fComponentYLen;
			break;
		case RIGHT_BOTTOM:
			ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x - fKerf/2 - fComponentXLen;
			ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y + fKerf/2 + fComponentYLen;
			break;
		case RIGHT_TOP:
			ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x - fKerf/2 - fComponentXLen;
			ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y - fKerf/2;
			break;
		default:
			break;
		}
	}
	
	return ptRet;
}

PointFloat GetAttachPos_RemainderCutting_Start(Panel* pPanel, PointFloat ptMouseInPanel, float fKerf, int nAttachDistance)
{
	PointFloat ptRet = ptMouseInPanel;

	vector<PointFloat> vAllAttachableVertexInPanel;
	//PointFloat aPastingComponentVertex[4];

	PointFloat aPanelVertex[4];
	PointFloat ptPanelLeftTop;
	ptPanelLeftTop.x = pPanel->m_OriginX;
	ptPanelLeftTop.y = pPanel->m_OriginY + pPanel->m_RealWidth;
	GetInflatedComponentRectVertex(ptPanelLeftTop, pPanel->m_RealLength, pPanel->m_RealWidth, 0, aPanelVertex);
	for(int i = 0; i < 4; i++)
		vAllAttachableVertexInPanel.push_back(aPanelVertex[i]);

	vector<Component*> vAllComponent;
	FindAllComponentInSingleton(vAllComponent);
	for(int i = 0; i < vAllComponent.size(); i++)
	{
		PointFloat aComponentVertex[4];
		PointFloat ptComponentLeftTop;
		ptComponentLeftTop.x = vAllComponent[i]->m_x;
		ptComponentLeftTop.y = vAllComponent[i]->m_y + vAllComponent[i]->m_RealWidth;
		GetInflatedComponentRectVertex(ptComponentLeftTop, vAllComponent[i]->m_RealLength, vAllComponent[i]->m_RealWidth, fKerf, aComponentVertex);
		for(int j = 0; j < 4; j++)
			vAllAttachableVertexInPanel.push_back(aComponentVertex[j]);
	}

	//float fMinDistanceForVertex[] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
	//PointFloat ptMinDistanceForVertex[4];
	float fMinDistance = FLT_MAX;
	PointFloat ptMinDistance;
	//for(int i = 0; i < 4; i++)
	//{
		PointFloat theVertex = /*aPastingComponentVertex[i]*/ptMouseInPanel;
		for(int j = 0; j < vAllAttachableVertexInPanel.size(); j++)
		{
			PointFloat theCompareVertex = vAllAttachableVertexInPanel[j];
			float fDistance = DistanceBetweenPoints(theVertex.x, theVertex.y, theCompareVertex.x, theCompareVertex.y);
			if(fDistance < /*fMinDistanceForVertex[i]*/fMinDistance)
			{
				//fMinDistanceForVertex[i] = fDistance;
				//ptMinDistanceForVertex[i] = theCompareVertex;
				fMinDistance = fDistance;
				ptMinDistance = theCompareVertex;
			}
		}
	//}

	//int nMinDistanceVertexID = -1;
	//float fMinDistance = FLT_MAX;
	//for(int i = 0; i < 4; i++)
	//{
	//	if(fMinDistanceForVertex[i] < fMinDistance && fMinDistanceForVertex[i] <= nAttachDistance)
	//	{
	//		nMinDistanceVertexID = i;
	//		fMinDistance = fMinDistanceForVertex[i];
	//	}
	//}

	//if(nMinDistanceVertexID != -1)
	//{
	//	switch(nMinDistanceVertexID)
	//	{
	//	case LEFT_TOP:
	//		ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x + fKerf/2;
	//		ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y - fKerf/2;
	//		break;
	//	case LEFT_BOTTOM:
	//		ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x + fKerf/2;
	//		ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y + fKerf/2 + fComponentYLen;
	//		break;
	//	case RIGHT_BOTTOM:
	//		ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x - fKerf/2 - fComponentXLen;
	//		ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y + fKerf/2 + fComponentYLen;
	//		break;
	//	case RIGHT_TOP:
	//		ptRet.x = ptMinDistanceForVertex[nMinDistanceVertexID].x - fKerf/2 - fComponentXLen;
	//		ptRet.y = ptMinDistanceForVertex[nMinDistanceVertexID].y - fKerf/2;
	//		break;
	//	default:
	//		break;
	//	}
	//}

	if(fMinDistance <= nAttachDistance)
	{
		ptRet = ptMinDistance;
	}

	return ptRet;
}

PointFloat GetAttachPos_RemainderCutting_End(Panel* pPanel, PointFloat ptMouseInPanel, float fKerf, int nAttachDistance, PointFloat ptStartInPanel)
{
	PointFloat ptRet = ptMouseInPanel;

	PointFloat ptHVCoverted;
	if(abs(ptMouseInPanel.x - ptStartInPanel.x) > abs(ptMouseInPanel.y - ptStartInPanel.y))
	{
		ptHVCoverted.x = ptMouseInPanel.x;
		ptHVCoverted.y = ptStartInPanel.y;
	}
	else
	{
		ptHVCoverted.x = ptStartInPanel.x;
		ptHVCoverted.y = ptMouseInPanel.y;
	}
	ptRet = ptHVCoverted;

	return ptRet;
}

void GetInflatedComponentRectOutline(PointFloat ptLeftTopInPanel, float fComponentXLen, float fComponentYLen, float fKerf, pair<PointFloat, PointFloat> (&aComponentOutline)[4])
{
	PointFloat ptLeftTopInPanel_Inflated = ptLeftTopInPanel;
	ptLeftTopInPanel_Inflated.x -= fKerf/2;
	ptLeftTopInPanel_Inflated.y += fKerf/2;
	float fComponentXLen_Inflated = fComponentXLen + fKerf;
	float fComponentYLen_Inflated = fComponentYLen + fKerf;
	aComponentOutline[SIDE_LEFT].first.x = ptLeftTopInPanel_Inflated.x;
	aComponentOutline[SIDE_LEFT].first.y = ptLeftTopInPanel_Inflated.y - fComponentYLen_Inflated;
	aComponentOutline[SIDE_LEFT].second.x = ptLeftTopInPanel_Inflated.x;
	aComponentOutline[SIDE_LEFT].second.y = ptLeftTopInPanel_Inflated.y;
	aComponentOutline[SIDG_BOTTOM].first.x = ptLeftTopInPanel_Inflated.x;
	aComponentOutline[SIDG_BOTTOM].first.y = ptLeftTopInPanel_Inflated.y - fComponentYLen_Inflated;
	aComponentOutline[SIDG_BOTTOM].second.x = ptLeftTopInPanel_Inflated.x + fComponentXLen_Inflated;
	aComponentOutline[SIDG_BOTTOM].second.y = ptLeftTopInPanel_Inflated.y - fComponentYLen_Inflated;
	aComponentOutline[SIDE_RIGHT].first.x = ptLeftTopInPanel_Inflated.x + fComponentXLen_Inflated;
	aComponentOutline[SIDE_RIGHT].first.y = ptLeftTopInPanel_Inflated.y - fComponentYLen_Inflated;
	aComponentOutline[SIDE_RIGHT].second.x = ptLeftTopInPanel_Inflated.x + fComponentXLen_Inflated;
	aComponentOutline[SIDE_RIGHT].second.y = ptLeftTopInPanel_Inflated.y;
	aComponentOutline[SIDE_TOP].first.x = ptLeftTopInPanel_Inflated.x;
	aComponentOutline[SIDE_TOP].first.y = ptLeftTopInPanel_Inflated.y;
	aComponentOutline[SIDE_TOP].second.x = ptLeftTopInPanel_Inflated.x + fComponentXLen_Inflated;
	aComponentOutline[SIDE_TOP].second.y = ptLeftTopInPanel_Inflated.y;
}

void GetInflatedComponentRectVertex(PointFloat ptLeftTopInPanel, float fComponentXLen, float fComponentYLen, float fKerf, PointFloat (&aComponentVertex)[4])
{
	PointFloat ptLeftTopInPanel_Inflated = ptLeftTopInPanel;
	ptLeftTopInPanel_Inflated.x -= fKerf/2;
	ptLeftTopInPanel_Inflated.y += fKerf/2;
	float fComponentXLen_Inflated = fComponentXLen + fKerf;
	float fComponentYLen_Inflated = fComponentYLen + fKerf;
	aComponentVertex[LEFT_TOP].x = ptLeftTopInPanel_Inflated.x;
	aComponentVertex[LEFT_TOP].y = ptLeftTopInPanel_Inflated.y;
	aComponentVertex[LEFT_BOTTOM].x = ptLeftTopInPanel_Inflated.x;
	aComponentVertex[LEFT_BOTTOM].y = ptLeftTopInPanel_Inflated.y - fComponentYLen_Inflated;
	aComponentVertex[RIGHT_BOTTOM].x = ptLeftTopInPanel_Inflated.x + fComponentXLen_Inflated;
	aComponentVertex[RIGHT_BOTTOM].y = ptLeftTopInPanel_Inflated.y - fComponentYLen_Inflated;
	aComponentVertex[RIGHT_TOP].x = ptLeftTopInPanel_Inflated.x + fComponentXLen_Inflated;
	aComponentVertex[RIGHT_TOP].y = ptLeftTopInPanel_Inflated.y;
}

PointFloat GetAttachPos_PreCombineComponent(Panel* pPanel,  vector<PreCombineComponent> vPreCombineComponent, int nSelectedItem, PointFloat ptfMovingPosInPanel, float fKerf, int nAttachDistance)
{
	PointFloat ptRet = ptfMovingPosInPanel;

	vector<PointFloat> vAllAttachableVertexInPanel;
	PointFloat aMovingComponentVertex[4];

	PreCombineComponent& theMovingItem = vPreCombineComponent[nSelectedItem];

	vector<vector<OutlineLine>> vMillingOutlineGroup = theMovingItem.m_vMillingOutlineGroup;
	int nRotateTimes = theMovingItem.m_nRotatedAngleWhenCombine / 90;
	float fComponentXLen = theMovingItem.m_Item.m_fLength;
	float fComponentYLen = theMovingItem.m_Item.m_fWidth;
	for(int i = 0; i < abs(nRotateTimes); i++)
	{
		if(nRotateTimes < 0)
		{
			OutlineGroupRotateClockwise90(vMillingOutlineGroup, fComponentXLen, fComponentYLen);
			float fTmp = fComponentXLen;
			fComponentXLen = fComponentYLen;
			fComponentYLen = fTmp;
		}
	}

	float fLeftMost = FLT_MAX, fRightMost = -FLT_MAX, fTopMost = -FLT_MAX, fBottomMost = FLT_MAX;
	vector<OutlineLine>& theOutterOutline = vMillingOutlineGroup[vMillingOutlineGroup.size()-1];
	for(int i = 0; i < theOutterOutline.size(); i++)
	{
		OutlineLine& theLine = theOutterOutline[i];
		if(theLine.m_fStartX < fLeftMost)
			fLeftMost = theLine.m_fStartX;
		if(theLine.m_fStartX > fRightMost)
			fRightMost = theLine.m_fStartX;
		if(theLine.m_fStartY < fBottomMost)
			fBottomMost = theLine.m_fStartY;
		if(theLine.m_fStartY > fTopMost)
			fTopMost = theLine.m_fStartY;

		if(theLine.m_fEndX < fLeftMost)
			fLeftMost = theLine.m_fEndX;
		if(theLine.m_fEndX > fRightMost)
			fRightMost = theLine.m_fEndX;
		if(theLine.m_fEndY < fBottomMost)
			fBottomMost = theLine.m_fEndY;
		if(theLine.m_fEndY > fTopMost)
			fTopMost = theLine.m_fEndY;

	}
	aMovingComponentVertex[LEFT_BOTTOM].x = /*theMovingItem.m_fX_Left*/ptfMovingPosInPanel.x + fLeftMost;
	aMovingComponentVertex[LEFT_BOTTOM].y = /*theMovingItem.m_fY_Bottom*/ptfMovingPosInPanel.y + fBottomMost;
	aMovingComponentVertex[LEFT_TOP].x = /*theMovingItem.m_fX_Left*/ptfMovingPosInPanel.x + fLeftMost;
	aMovingComponentVertex[LEFT_TOP].y = /*theMovingItem.m_fY_Bottom*/ptfMovingPosInPanel.y + fTopMost;
	aMovingComponentVertex[RIGHT_TOP].x = /*theMovingItem.m_fX_Left*/ptfMovingPosInPanel.x + fRightMost;
	aMovingComponentVertex[RIGHT_TOP].y = /*theMovingItem.m_fY_Bottom*/ptfMovingPosInPanel.y + fTopMost;
	aMovingComponentVertex[RIGHT_BOTTOM].x = /*theMovingItem.m_fX_Left*/ptfMovingPosInPanel.x + fRightMost;
	aMovingComponentVertex[RIGHT_BOTTOM].y = /*theMovingItem.m_fY_Bottom*/ptfMovingPosInPanel.y + fBottomMost;

	PointFloat ptPanelCorner;
	ptPanelCorner.x = pPanel->m_OriginX - fKerf/2;
	ptPanelCorner.y = pPanel->m_OriginY - fKerf/2;
	vAllAttachableVertexInPanel.push_back(ptPanelCorner);

	ptPanelCorner.x = pPanel->m_OriginX - fKerf/2;
	ptPanelCorner.y = pPanel->m_OriginY + pPanel->m_RealWidth + fKerf/2;
	vAllAttachableVertexInPanel.push_back(ptPanelCorner);

	ptPanelCorner.x = pPanel->m_OriginX + pPanel->m_RealLength + fKerf/2;
	ptPanelCorner.y = pPanel->m_OriginY - fKerf/2;
	vAllAttachableVertexInPanel.push_back(ptPanelCorner);

	ptPanelCorner.x = pPanel->m_OriginX + pPanel->m_RealLength + fKerf/2;
	ptPanelCorner.y = pPanel->m_OriginY + pPanel->m_RealWidth + fKerf/2;
	vAllAttachableVertexInPanel.push_back(ptPanelCorner);

	for(int i = 0; i < vPreCombineComponent.size(); i++)
	{
		if(i != nSelectedItem)
		{
			PreCombineComponent& thePreCombineComponent = vPreCombineComponent[i];
			//Component* pTheComponent = ConvertOneInputInfoToOneComponent(thePreCombineComponent.m_Item);
			vector<vector<OutlineLine>> vMillingOutlineGroup = thePreCombineComponent.m_vMillingOutlineGroup;
			int nRotateTimes = thePreCombineComponent.m_nRotatedAngleWhenCombine / 90;
			float fXLen = thePreCombineComponent.m_Item.m_fLength;
			float fYLen = thePreCombineComponent.m_Item.m_fWidth;
			for(int j = 0; j < abs(nRotateTimes); j++)
			{
				if(nRotateTimes < 0)
				{
					OutlineGroupRotateClockwise90(vMillingOutlineGroup, fXLen, fYLen);
					//ComponentRotateClockwise90(pTheComponent);
					float fTmp = fXLen;
					fXLen = fYLen;
					fYLen = fTmp;
				}
			}

			PointFloat ptComponetnCorner;
			for(int j = 0; j < vMillingOutlineGroup.size(); j++)
			{
				for(int k = 0; k < vMillingOutlineGroup[j].size(); k++)
				{
					OutlineLine& theLine = vMillingOutlineGroup[j][k];

					ptComponetnCorner.x = thePreCombineComponent.m_fX_Left + theLine.m_fStartX;
					ptComponetnCorner.y = thePreCombineComponent.m_fY_Bottom + theLine.m_fStartY;
					vAllAttachableVertexInPanel.push_back(ptComponetnCorner);
					ptComponetnCorner.x = thePreCombineComponent.m_fX_Left + theLine.m_fEndX;
					ptComponetnCorner.y = thePreCombineComponent.m_fY_Bottom + theLine.m_fEndY;
					vAllAttachableVertexInPanel.push_back(ptComponetnCorner);
				}
			}
		}

	}

	float fMinDistanceForVertex_X[] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
	float fMinDistanceForVertex_Y[] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
	//PointFloat ptMinDistanceForVertex[4];
	for(int i = 0; i < 4; i++)
	{
		PointFloat theVertex = aMovingComponentVertex[i];
		for(int j = 0; j < vAllAttachableVertexInPanel.size(); j++)
		{
			PointFloat theCompareVertex = vAllAttachableVertexInPanel[j];
			if(fabs(theVertex.x - theCompareVertex.x) < fabs(fMinDistanceForVertex_X[i]))
			{
				fMinDistanceForVertex_X[i] = theVertex.x - theCompareVertex.x;
			}
			if(fabs(theVertex.y - theCompareVertex.y) < fabs(fMinDistanceForVertex_Y[i]))
			{
				fMinDistanceForVertex_Y[i] = theVertex.y - theCompareVertex.y;
			}

		}
	}

	int nAttachSideID_X = -1;
	float fMinDistance = FLT_MAX;
	int nAttachSideID_Y = -1;
	for(int i = 0; i < 4; i++)
	{
		if(fabs(fMinDistanceForVertex_X[i]) < fabs(fMinDistance))
		{
			nAttachSideID_X = i;
			fMinDistance = fMinDistanceForVertex_X[i];
		}
	}

	fMinDistance = FLT_MAX;
	for(int i = 0; i < 4; i++)
	{
		if(fabs(fMinDistanceForVertex_Y[i]) < fabs(fMinDistance))
		{
			nAttachSideID_Y = i;
			fMinDistance = fMinDistanceForVertex_Y[i];
		}
	}

	if(nAttachSideID_X != -1 && fabs(fMinDistanceForVertex_X[nAttachSideID_X]) <= nAttachDistance)
	{
		ptRet.x = ptRet.x - fMinDistanceForVertex_X[nAttachSideID_X]/* - (aMovingComponentVertex[nAttachSideID_X].x - theMovingItem.m_fX_Left)*/;

	}

	if(nAttachSideID_Y != -1 && fabs(fMinDistanceForVertex_Y[nAttachSideID_Y]) <= nAttachDistance)
	{
		ptRet.y = ptRet.y - fMinDistanceForVertex_Y[nAttachSideID_Y]/* - (aMovingComponentVertex[nAttachSideID_Y].y - theMovingItem.m_fY_Bottom)*/;
	}

	return ptRet;
}

void CalcKnifeDowPos()
{
	vector<Panel*> vAllPanel;
	FindAllPanelInSingleton(vAllPanel);
	for(int i = 0; i < vAllPanel.size(); i++)
	{
		Panel* pThePanel = vAllPanel[i];

		CalcKnifeDownPosInPanel(pThePanel);

	}
}

void CalcKnifeDownPosInPanel(Panel* pPanel)
{
	CSingleton* pSingleton = CSingleton::GetSingleton();
	float fKerf = pSingleton->m_BaseInfo.m_SawKerfWidth;
	float fKnifeDownDistance = pSingleton->m_BaseInfo.m_distance2;

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(*pPanel, vAllComponent);
	std::sort(vAllComponent.begin(), vAllComponent.end(), SortComponentByIDInPanel);

	vector<OutlineLine> vAllCuttedLine;
	PointFloat aPanelVertex[4];
	PointFloat ptPanelLeftTop;
	ptPanelLeftTop.x = pPanel->m_OriginX;
	ptPanelLeftTop.y = pPanel->m_OriginY + pPanel->m_RealWidth;
	GetInflatedComponentRectVertex(ptPanelLeftTop, pPanel->m_RealLength, pPanel->m_RealWidth, fKerf, aPanelVertex);
	for(int i = 0; i < 4; i++)
	{
		OutlineLine line;
		line.SetValue(aPanelVertex[i].x, aPanelVertex[i].y, aPanelVertex[(i+1)%4].x, aPanelVertex[(i+1)%4].y, STRAIGHT_LINE);
		vAllCuttedLine.push_back(line);
	}

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		theComponent.m_nKnifeDownPos = CalcKnifeDownPosOfComponent(theComponent, vAllCuttedLine, fKerf, fKnifeDownDistance, pPanel->m_OrgLen, pPanel->m_OrgWidth);
		
		PointFloat aComponentVertex[4];
		PointFloat ptComponentLeftTop;
		ptComponentLeftTop.x = theComponent.m_x;
		ptComponentLeftTop.y = theComponent.m_y + theComponent.m_RealWidth;
		GetInflatedComponentRectVertex(ptComponentLeftTop, theComponent.m_RealLength, theComponent.m_RealWidth, fKerf, aComponentVertex);
		for(int i = 0; i < 4; i++)
		{
			OutlineLine line;
			line.SetValue(aComponentVertex[i].x, aComponentVertex[i].y, aComponentVertex[(i+1)%4].x, aComponentVertex[(i+1)%4].y, STRAIGHT_LINE);
			vAllCuttedLine.push_back(line);
		}
	}
}

int CalcKnifeDownPosOfComponent(Component& theComponent, vector<OutlineLine>& vAllCuttedLine, float fKerf, float fKnifeDownDistance, float fPanelXLen, float fPanelYLen)
{
	int nRet = 0;

	vector<PointInfo> vOutterOutlinePoint;
	GetOutterOutlineVertex(theComponent.m_vOutlinePoint, vOutterOutlinePoint);
	if(vOutterOutlinePoint.size() == 0)
		return nRet;

	float aValuePower[] = {0.5, 0.5};
	Normalizer normalizer(2, aValuePower);
	vector<int> vNormalizeOutlineID;
	for(int i = 0; i < vOutterOutlinePoint.size() - 1; i++)
	{
		int nKnifeDownVertexIDStart = i;
		int nKnifeDownVertexIDEnd = i + 1;
		PointInfo& vertexStart = vOutterOutlinePoint[nKnifeDownVertexIDStart];
		//vertexStart.x += theComponent.m_x;
		//vertexStart.y += theComponent.m_y;
		PointInfo& vertexEnd = vOutterOutlinePoint[nKnifeDownVertexIDEnd];
		//vertexEnd.x += theComponent.m_x;
		//vertexEnd.y += theComponent.m_y;
		OutlineLine line = GenLineByVertex(vertexStart, vertexEnd);
		line.m_fStartX += theComponent.m_x;
		line.m_fStartY += theComponent.m_y;
		line.m_fEndX += theComponent.m_x;
		line.m_fEndY += theComponent.m_y;
		if(line.m_nLineType != LineType::STRAIGHT_LINE)
			continue;

		float fXKnifeDown, fYKnifeDown;
		float fEdgeLen = GetPointDistance(line.m_fStartX, line.m_fStartY, line.m_fEndX, line.m_fEndY);
		fXKnifeDown = line.m_fEndX + fKnifeDownDistance * (line.m_fStartX - line.m_fEndX)/ fEdgeLen;
		fYKnifeDown = line.m_fEndY + fKnifeDownDistance * (line.m_fStartY - line.m_fEndY)/ fEdgeLen;
		//fXKnifeDown += theComponent.m_x;
		//fYKnifeDown += theComponent.m_y;

		bool bKnifeDownPointNearCuttedLines = false;
		for(int j = 0; j < vAllCuttedLine.size(); j++)
		{
			OutlineLine theLine = vAllCuttedLine[j];
			float fPointAndLineSegmentDistance = GetPointAndLineSegmentDistance(fXKnifeDown, fYKnifeDown, theLine.m_fStartX, theLine.m_fStartY, theLine.m_fEndX, theLine.m_fEndY);
			if(fabs(fPointAndLineSegmentDistance - fKerf/2.0) < 0.1)
			{
				bKnifeDownPointNearCuttedLines = true;
				break;
			}
		}
		if(bKnifeDownPointNearCuttedLines)
			continue;

		bool bKnifeDownPointInUnavalibleArea = false;
		if(fXKnifeDown < UNAVALIBLE_KNIFE_DOWN_DISTANCE || fXKnifeDown > (fPanelXLen-UNAVALIBLE_KNIFE_DOWN_DISTANCE) \
			|| fYKnifeDown < UNAVALIBLE_KNIFE_DOWN_DISTANCE || fYKnifeDown > (fPanelYLen-UNAVALIBLE_KNIFE_DOWN_DISTANCE))
			bKnifeDownPointInUnavalibleArea = true;
		if(bKnifeDownPointInUnavalibleArea)
			continue;

		float aItemValue[2];
		aItemValue[0] = /*DistanceBetweenPoints(line.m_fStartX, line.m_fStartY, line.m_fEndX, line.m_fEndY)*/GetNotCuttedOutlineLineLen(line, vAllCuttedLine, fKerf);
		aItemValue[1] = GetDistanceBetweenPointAndCuttedLine(line, fXKnifeDown, fYKnifeDown, vAllCuttedLine);


		vNormalizeOutlineID.push_back(i);
		normalizer.InputItem(aItemValue);

		//if(!bKnifeDownPointNearCuttedLines && !bKnifeDownPointInUnavalibleArea)
		//{
		//	nRet = i;
		//	break;
		//}
	}

	vector<float> vNormalizeResult = normalizer.GetNormalizeResult();
	if(vNormalizeResult.size() > 0)
	{
		float fMaxValue = -FLT_MAX;
		int nMaxValueItemID = -1;
		for(int i = 0; i < vNormalizeResult.size(); i++)
		{
			if(vNormalizeResult[i] > fMaxValue)
			{
				fMaxValue = vNormalizeResult[i];
				nMaxValueItemID = i;
			}
		}
		if(nMaxValueItemID != -1 && nMaxValueItemID < vNormalizeOutlineID.size())
		{
			nRet = vNormalizeOutlineID[nMaxValueItemID];
		}
	}

	return nRet;
}

float GetDistanceBetweenPointAndCuttedLine(OutlineLine line, float fXKnifeDown, float fYKnifeDown, vector<OutlineLine>& vAllCuttedLine)
{
	float fRet = FLT_MAX;

	for(int i = 0; i < vAllCuttedLine.size(); i++)
	{
		OutlineLine& theCuttedLine = vAllCuttedLine[i];

		bool bCross = true;
		if(IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX) && IsFloatEqual(line.m_fEndY, line.m_fStartY))
			bCross = false;
		else if(IsFloatEqual(line.m_fEndX, line.m_fStartX) && IsFloatEqual(theCuttedLine.m_fEndY, theCuttedLine.m_fStartY))
			bCross = false;
		else if(!(IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX) || IsFloatEqual(line.m_fEndX, line.m_fStartX)))
		{
			float fKOutlineLine = (line.m_fEndY - line.m_fStartY) / (line.m_fEndX - line.m_fStartX);
			float fKCuttedLine = (theCuttedLine.m_fEndY - theCuttedLine.m_fStartY) / (theCuttedLine.m_fEndX - theCuttedLine.m_fStartX);
			if(IsFloatEqual(-1.0/fKOutlineLine, fKCuttedLine))
				bCross = false;
		}

		if(bCross)
		{
			float fCrossPointX, fCrossPointY;
			if(!IsFloatEqual(line.m_fEndX, line.m_fStartX))
			{
				if(!IsFloatEqual(line.m_fEndY, line.m_fStartY))
				{
					float fKOutlineLine = (line.m_fEndY - line.m_fStartY) / (line.m_fEndX - line.m_fStartX);
					float fKKnifeDownExtendLine = -1.0 / fKOutlineLine;
					float fBKnifeDownExtendLine = fYKnifeDown - fKKnifeDownExtendLine * fXKnifeDown;

					if(!IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX))
					{
						float fKCuttedLine = (theCuttedLine.m_fEndY - theCuttedLine.m_fStartY) / (theCuttedLine.m_fEndX - theCuttedLine.m_fStartX);
						float fBCuttedLine = theCuttedLine.m_fStartY - fKCuttedLine * theCuttedLine.m_fStartX;

						fCrossPointX = (fBCuttedLine - fBKnifeDownExtendLine) / (fKKnifeDownExtendLine - fKCuttedLine);
						fCrossPointY = fKKnifeDownExtendLine * fCrossPointX + fBKnifeDownExtendLine;
					}
					else
					{
						fCrossPointX = theCuttedLine.m_fStartX;
						fCrossPointY = fKKnifeDownExtendLine * fCrossPointX + fBKnifeDownExtendLine;
					}
				}
				else
				{
					fCrossPointX = fXKnifeDown;

					if(!IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX))
					{
						float fKCuttedLine = (theCuttedLine.m_fEndY - theCuttedLine.m_fStartY) / (theCuttedLine.m_fEndX - theCuttedLine.m_fStartX);
						float fBCuttedLine = theCuttedLine.m_fStartY - fKCuttedLine * theCuttedLine.m_fStartX;

						fCrossPointY = fKCuttedLine * fCrossPointX + fBCuttedLine;
					}			
				}
			}
			else
			{
				fCrossPointY = fYKnifeDown;

				if(!IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX))
				{
					float fKCuttedLine = (theCuttedLine.m_fEndY - theCuttedLine.m_fStartY) / (theCuttedLine.m_fEndX - theCuttedLine.m_fStartX);
					float fBCuttedLine = theCuttedLine.m_fStartY - fKCuttedLine * theCuttedLine.m_fStartX;
					fCrossPointX = (fCrossPointY - fBCuttedLine) / fKCuttedLine;
				}
				else
				{
					fCrossPointX = theCuttedLine.m_fStartX;
				}
			}

			float fCuttedLineSegmentLen = DistanceBetweenPoints(theCuttedLine.m_fStartX, theCuttedLine.m_fStartY, theCuttedLine.m_fEndX, theCuttedLine.m_fEndY);
			float fCrossPointToStartEndLen = DistanceBetweenPoints(fCrossPointX, fCrossPointY, theCuttedLine.m_fStartX, theCuttedLine.m_fStartY)+DistanceBetweenPoints(fCrossPointX, fCrossPointY, theCuttedLine.m_fEndX, theCuttedLine.m_fEndY);
			if(IsFloatEqual(fCrossPointToStartEndLen, fCuttedLineSegmentLen))
			{
				float fVecOutlineLineX = line.m_fEndX - line.m_fStartX;
				float fVecOutlineLineY = line.m_fEndY - line.m_fStartY;
				float fVecCrossPointX = fCrossPointX - line.m_fStartX;
				float fVecCrossPointY = fCrossPointY - line.m_fStartY;

				float fSinValue = fVecOutlineLineX * fVecCrossPointY - fVecCrossPointX * fVecOutlineLineY;
				if(fSinValue < 0)
				{
					float fKnifeDownPtCrossPtDistance = DistanceBetweenPoints(fXKnifeDown, fYKnifeDown, fCrossPointX, fCrossPointY);
					if(fKnifeDownPtCrossPtDistance < fRet)
						fRet = fKnifeDownPtCrossPtDistance;
				}
			}
		}
	}

	return fRet;
}

float GetNotCuttedOutlineLineLen(OutlineLine line, vector<OutlineLine>& vAllCuttedLine, float fKerf)
{
	float fRet = DistanceBetweenPoints(line.m_fStartX, line.m_fStartY, line.m_fEndX, line.m_fEndY);

	float fTotalOverlapLen = 0;
	for(int i = 0; i < vAllCuttedLine.size(); i++)
	{
		OutlineLine& theCuttedLine = vAllCuttedLine[i];

		float fOverlapLen = 0;

		if(IsFloatEqual(line.m_fEndX, line.m_fStartX))
		{
			if(IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX))
			{
				if(fabs(line.m_fStartX - theCuttedLine.m_fStartX) < fKerf/2.0+1)
				{
					float fLineMinY, fLineMaxY;
					float fCuttedLineMinY, fCuttedLineMaxY;
					if(line.m_fStartY > line.m_fEndY)
					{
						fLineMinY = line.m_fEndY;
						fLineMaxY = line.m_fStartY;
					}
					else
					{
						fLineMinY = line.m_fStartY;
						fLineMaxY = line.m_fEndY;
					}
					if(theCuttedLine.m_fStartY > theCuttedLine.m_fEndY)
					{
						fCuttedLineMinY = theCuttedLine.m_fEndY;
						fCuttedLineMaxY = theCuttedLine.m_fStartY;
					}
					else
					{
						fCuttedLineMinY = theCuttedLine.m_fStartY;
						fCuttedLineMaxY = theCuttedLine.m_fEndY;
					}

					if(fCuttedLineMinY >= fLineMinY && fCuttedLineMinY <= fLineMaxY)
					{
						if(fCuttedLineMaxY > fLineMaxY)
							fOverlapLen =fLineMaxY - fCuttedLineMinY;
						else
							fOverlapLen =fCuttedLineMaxY - fCuttedLineMinY;
					}
					else if(fCuttedLineMaxY >= fLineMinY && fCuttedLineMaxY <= fLineMaxY)
					{
						if(fCuttedLineMinY < fLineMinY)
							fOverlapLen =fCuttedLineMaxY - fLineMinY;
						else
							fOverlapLen =fCuttedLineMaxY - fCuttedLineMinY;
					}
				}
			}
		}
		else if(!IsFloatEqual(theCuttedLine.m_fEndX, theCuttedLine.m_fStartX))
		{
			float fKLine = (line.m_fEndY - line.m_fStartY) / (line.m_fEndX - line.m_fStartX);
			float fBLine = line.m_fStartY - fKLine * line.m_fStartX;
			float fKCuttedLine = (theCuttedLine.m_fEndY - theCuttedLine.m_fStartY) / (theCuttedLine.m_fEndX - theCuttedLine.m_fStartX);
			float fBCuttedLine = theCuttedLine.m_fStartY - fKCuttedLine * theCuttedLine.m_fStartX;

			float fLineDistance = fabs(fBLine - fBCuttedLine)*(1.0/(sqrt(1+pow(fKLine,2))));
			if(IsFloatEqual(fKLine, fKCuttedLine) && fLineDistance < fKerf/2.0+1)
			{
				OutlineLine lineTranslated = line;
				lineTranslated.m_fStartY += (fBCuttedLine - fBLine);
				lineTranslated.m_fEndY += (fBCuttedLine - fBLine);

				float fLineMinX, fLineMaxX;
				float fCuttedLineMinX, fCuttedLineMaxX;
				if(lineTranslated.m_fStartX > lineTranslated.m_fEndX)
				{
					fLineMinX = line.m_fEndX;
					fLineMaxX = line.m_fStartX;
				}
				else
				{
					fLineMinX = line.m_fStartX;
					fLineMaxX = line.m_fEndX;
				}
				if(theCuttedLine.m_fStartX > theCuttedLine.m_fEndX)
				{
					fCuttedLineMinX = theCuttedLine.m_fEndX;
					fCuttedLineMaxX = theCuttedLine.m_fStartX;
				}
				else
				{
					fCuttedLineMinX = theCuttedLine.m_fStartX;
					fCuttedLineMaxX = theCuttedLine.m_fEndX;
				}

				float fOverlapLenX = 0;
				if(fCuttedLineMinX >= fLineMinX && fCuttedLineMinX <= fLineMaxX)
				{
					if(fCuttedLineMaxX > fLineMaxX)
						fOverlapLenX =fLineMaxX - fCuttedLineMinX;
					else
						fOverlapLenX =fCuttedLineMaxX - fCuttedLineMinX;
				}
				else if(fCuttedLineMaxX >= fLineMinX && fCuttedLineMaxX <= fLineMaxX)
				{
					if(fCuttedLineMinX < fLineMinX)
						fOverlapLenX =fCuttedLineMaxX - fLineMinX;
					else
						fOverlapLenX =fCuttedLineMaxX - fCuttedLineMinX;
				}
				fOverlapLen = fOverlapLenX * sqrt(1 + pow(fKLine, 2));
			}
		}

		fTotalOverlapLen += fOverlapLen;
	}

	fRet -= fTotalOverlapLen;

	if(fRet < 0)
		fRet = 0;

	return fRet;
}

bool ModifyLabelImgNameLikeNCName(map<int, CString>& mapPanelSerIDToNCFileFullPath, CString strOutputDir)
{
	if(strOutputDir.GetAt(strOutputDir.GetLength()-1) != _T('\\'))
		strOutputDir += _T("\\");

	vector<CString> vAllBmpFilePath;

	CFileFind find;
	CString strFind = strOutputDir + _T("*.*");//遍历这一级全部的目录
	int nResult = find.FindFile(strFind);
	while(nResult)
	{
		nResult = find.FindNextFile();
		if(!find.IsDirectory() && !find.IsDots())
		{
			CString strFilePath = find.GetFilePath();
			if(strFilePath.Right(3).CompareNoCase(_T("bmp")) == 0)
				vAllBmpFilePath.push_back(strFilePath);
		}
	}

	if(vAllBmpFilePath.size() == 0)
	{
		AfxMessageBox("输出目录下没有板件标签！\n须要先导出标签然后导出NC文件！");
		return false;
	}

	for(map<int, CString>::iterator itr = mapPanelSerIDToNCFileFullPath.begin(); itr != mapPanelSerIDToNCFileFullPath.end(); itr++)
	{
		int nSerID = itr->first;
		CString strNCFileName = GetFileNameInPath(itr->second);

		for(int i = 0; i < vAllBmpFilePath.size(); i++)
		{
			CString strBmpFilePath = vAllBmpFilePath[i];
			CString strBmpFileOrgName = GetFileNameInPath_WithSuffix(strBmpFilePath);
			if(strBmpFileOrgName.Find(_T('-')) != -1)
			{
				int nSerIDOfBmp = atoi(strBmpFileOrgName.Left(strBmpFileOrgName.Find(_T('-'))));
				if(nSerIDOfBmp == nSerID)
				{
					CString strModifiedBmpFileFullPath;
					strModifiedBmpFileFullPath.Format(_T("%s%s%s"), strOutputDir, strNCFileName, strBmpFileOrgName.Right(strBmpFileOrgName.GetLength()-strBmpFileOrgName.Find(_T('-'))));
					rename(strBmpFilePath, strModifiedBmpFileFullPath);
				}
			}
		}
	}

	return true;
}
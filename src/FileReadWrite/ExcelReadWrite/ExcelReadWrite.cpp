#include "stdafx.h"
#include "ExcelReadWrite.h"
#include "Excel\CApplication.h"
#include "Excel\CRange.h"
#include "Excel\CWorkbooks.h"
#include "Excel\CWorkbook.h"
#include "Excel\CWorksheets.h"
#include "Excel\CWorksheet.h"
#include "comdef.h"
#include "../Misc/OtherShapeHelper.h"
#include "../Misc/Misc.h"
#include "../Misc/HGTools.h"
//#include "../../../include/Encryption/base64/base64.h"
#include "../Misc/EncryptionInterface.h"
#include "../Misc/ProgramMisc.h"
#include "../Misc/YHInfoTools.h"

#define NO_OTHER_SHAPE_AUTO_CREATE 1
enum BandingID{BANDING_ID_FRONT = 0, BANDING_ID_RIGHT, BANDING_ID_BACK, BANDING_ID_LEFT};

int  add(LPCTSTR szFileDir ,  vector<ComponentInputItem>& vComponentInputItem)
{
	return 1;
}

bool ExcelReadWrite::ReadPanelInfo(LPCTSTR szFileDir,  vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param)
{
	std::map<int,int> InOutdata;
	bool bRow1AsData = false;

	CString strTmp;
	strTmp.Format(_T("kdata\\Excel%s"), g_szSuffix);
	CString hgePath = HGTools::getRelativePath(strTmp);

	CString xmlPath = HGTools::getXmlPathFromHgxPath(hgePath);
	decrypt_base64(hgePath.GetBuffer(), xmlPath.GetBuffer());
	TinyXml::TiXmlDocument doc(xmlPath);
	bool bLoadRes = doc.LoadFile();
	HGTools::deleteFile(xmlPath);

	if(bLoadRes)
	{
		XmlHandler xmlHandler;
		vector<TiXmlElement*> vMap = xmlHandler.GetChildElms(doc.RootElement(), "Map");
		for(int i = 0; i < vMap.size(); i++)
		{
			TiXmlElement* elmMap = vMap[i];
			vector<TiXmlElement*> vItem = xmlHandler.GetChildElms(elmMap);
			for(int j = 0; j < vItem.size(); j++)
			{
				TiXmlElement* elmItem = vItem[j];
				CString strTmp;
				xmlHandler.GetXmlAttribute(elmItem, "Input", strTmp);
				std::string indata = strTmp;
				xmlHandler.GetXmlAttribute(elmItem, "Output", strTmp);
				std::string outdata = strTmp;
				InOutdata[atoi(outdata.c_str())] = atoi(indata.c_str());
			}
		}
		TiXmlElement* elmImportingParam = xmlHandler.GetChildElm(doc.RootElement(), "ImportingParam");
		xmlHandler.GetXmlAttribute(elmImportingParam, "Row1AsData", bRow1AsData);
	}
	else
		return false;

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
	float fMaxXLen = param.f_width;
	float fMaxYLen = param.f_height;
	float fDeburringwidth = param.f_Deburringwidth;
	CString str_OutedgePanel = "";

	CApplication ExcelApp;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	LPDISPATCH lpDisp = NULL;



	if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return false;
	}

	books.AttachDispatch(ExcelApp.get_Workbooks());

	try
	{
		lpDisp = books.Open(szFileDir, 
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing, vtMissing, vtMissing);
		book.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		AfxMessageBox(_T("打开文件失败"));
		return false;
	}

	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());

	CString strSheetName = _T("NewSheet");

	try
	{
		//lpDisp = sheets.get_Item(_variant_t(strSheetName));
		lpDisp = sheets.get__Default(_variant_t(1));
		sheet.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		AfxMessageBox(_T("打开文件失败"));
		return false;
	}

	vComponentInputItem.clear();

	range.AttachDispatch(sheet.get_UsedRange(), true);

	VARIANT varRead = range.get_Value2();
	COleSafeArray olesaRead(varRead);

	VARIANT varItem;
	CString strItem;
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaRead.GetLBound(1, &lFirstLBound);
	olesaRead.GetUBound(1, &lFirstUBound);
	olesaRead.GetLBound(2, &lSecondLBound);
	olesaRead.GetUBound(2, &lSecondUBound);
	//long index[2] = {0, 0};

	long i;
	if(!bRow1AsData)
		i = lFirstLBound+1;
	else
		i = lFirstLBound;
	for (; i <= lFirstUBound; i++)
	{
		//index[0] = i;
		CString strValue[100];

		for (long j = lSecondLBound; j <= lSecondUBound; j++)
		{
			long index[2];
			index[0] = i;
			index[1] = j;
			olesaRead.GetElement(index, &varItem);

			//if(j != COLUMN_BARCODE && j != COLUMN_CABINET_NAME && j != COLUMN_PANEL_NAME && j != COLUMN_LEN \
			//   && j != COLUMN_WIDTH && j != COLUMN_THICKNESS && j != COLUMN_MATERIAL && j != COLUMN_TEXTURE && j != COLUMN_COUNT)
			//	continue;
		
			CString strItem;

			switch (varItem.vt)
			{
			case VT_R8:
				{
					int i_item = (int)varItem.dblVal;
					float f_item = (float)varItem.dblVal;

					if ((float)i_item - f_item > -0.01 && (float)i_item - f_item < 0.01)
						strItem.Format(_T("%d"),i_item);
					else
						strItem.Format(_T("%0.1f"),f_item);
					break;
				}

			case VT_BSTR:
				{
					strItem = varItem.bstrVal;
					break;
				}

			case VT_I4:
				{
					int i_item = (int)varItem.lVal;
					float f_item = (float)varItem.lVal;

					if ((float)i_item - f_item > -0.01 && (float)i_item - f_item < 0.01)
						strItem.Format(_T("%d"),i_item);
					else
						strItem.Format(_T("%0.1f"),f_item);

					//strItem.Format(_T("%0.1f"), (float)varItem.lVal);
					break;
				}

			default:
				{

				}
			}

			strValue[j-1] = strItem;
			
		}

		if (/*InOutdata.find(0) == InOutdata.end()*/InOutdata.size() == 0)
			return false;

		CString strBarCode = strValue[InOutdata[COLUMN_BARCODE]];
		if (strBarCode.IsEmpty())
			continue;
		
		ComponentInputItem componentInputItem;

		for(int key = 0;key < 37;key++)
		{
			if(InOutdata.find(key) != InOutdata.end())
			{
				int nInputIndex = InOutdata[key];
				if(nInputIndex > -1)
				{
					CString strInputItem = (LPCTSTR)strValue[nInputIndex];
					switch(key)
					{
					case COLUMN_ORDER_ID:
						componentInputItem.m_strOrderID = strInputItem;
						break;
					case COLUMN_CABINET_ID:
						componentInputItem.m_strCabinetID = strInputItem;
						break;
					case COLUMN_BARCODE:
						componentInputItem.m_strBarcode = strInputItem;
						break;
					case COLUMN_CABINET_NAME:
						componentInputItem.m_strCabinetName = strInputItem;
						break;
					case COLUMN_PANEL_NAME:
						componentInputItem.m_strPanelName = strInputItem;
						break;
					case COLUMN_MATERIAL:
						componentInputItem.m_strMaterial = strInputItem;
						break;
					case COLUMN_CARVING_LEN:
						componentInputItem.m_fLength = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_CARVING_WIDTH:
						componentInputItem.m_fWidth = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_CARVING_THICKNESS:
						componentInputItem.m_fThickness = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_COUNT:
						componentInputItem.m_nCount = _ttoi(strInputItem.GetBuffer());
						break;
					case COLUMN_TEXTURE:
						componentInputItem.m_strTexture = strInputItem;
						break;
					case COLUMN_BANDING_FRONT:
						componentInputItem.m_afBanding[0] = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_BANDING_RIGHT:
						componentInputItem.m_afBanding[1] = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_BANDING_BACK:
						componentInputItem.m_afBanding[2] = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_BANDING_LEFT:
						componentInputItem.m_afBanding[3] = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_OTHER_SHAPE_PARAM:
						componentInputItem.m_strOtherShapeIDAndParam = strInputItem;
						break;
					case COLUMN_ROTATE_FLAG:
						componentInputItem.m_eRotateType = _ttoi(strInputItem.GetBuffer());
						break;
					case COLUMN_UP_FACE_SLOT:
						componentInputItem.m_strUpperFaceSlotInfo = strInputItem;
						break;
					case COLUMN_SLOT_FLIP_FLAG:
						componentInputItem.m_bSlotFlipped = strInputItem.CompareNoCase(_T("1")) == 0 ? true : false;
						break;
					case COLUMN_DOWNER_FACE_SLOT:
						componentInputItem.m_strDownerFaceSlotInfo = strInputItem;
						break;	
					case COLUMN_UP_FACE_VHOLE:
						componentInputItem.m_strUpperFaceHoleInfo = strInputItem;
						break;
					case COLUMN_VHOLE_FLIP_FLAG:
						componentInputItem.m_bVHoleFlipped = strInputItem.CompareNoCase(_T("1")) == 0 ? true : false;
						break;	
					case COLUMN_DOWN_FACE_VHOLE:
						componentInputItem.m_strDownerFaceHoleInfo = strInputItem;
						break;
					case COLUMN_CUSTOMER_INFO:
						componentInputItem.m_strCustomerInfo = strInputItem;
						break;
					case COLUMN_JOINED_STORE:
						componentInputItem.m_strJoinedStore = strInputItem;
						break;
					case COLUMN_SLOTTING_FLAG:
						componentInputItem.m_strSlottingFlag = strInputItem;
						break;
					case COLUMN_REMINDER:
						componentInputItem.m_strReminder = strInputItem;
						break;
					case COLUMN_DRILLING:
						componentInputItem.m_strDrilling = strInputItem;
						break;
					case COLUMN_ORDER_TYPE:
						componentInputItem.m_strOrderType = strInputItem;
						break;
					case COLUMN_REVERSE_SIDE_BARCODE:
						componentInputItem.m_strReverseSideBarcode = strInputItem;
						break;	
					case COLUMN_PRODUCT_LEN:
						componentInputItem.m_fProductLength = _ttof(strInputItem.GetBuffer());
						break;		
					case COLUMN_PRODUCT_WIDTH:
						componentInputItem.m_fProductWidth = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_PRODUCT_THICKNESS:
						componentInputItem.m_fProductThickness = _ttof(strInputItem.GetBuffer());
						break;
					case COLUMN_OTHER_SHAPE_ID:
						componentInputItem.m_strOtherShapeID = strInputItem;
						break;
					case COLUMN_CUSTOMER_ADDRESS:
						componentInputItem.m_strCustomerAddress = strInputItem;
						break;
					case COLUMN_HOLE_SLOT_FLAG:
						componentInputItem.m_strHoleSlotFlag = strInputItem;
						break;		
					case COLUMN_OTHER_SHAPE_POINT:
						componentInputItem.m_strAbnormalInfo = strInputItem;
						break;		
					case COLUMN_SENDING_DATE:
						componentInputItem.m_strSendingDate = strInputItem;
						break;	
					default:
						break;
					}
				}
			}
		}

		//输入倍数
		componentInputItem.m_nCount = componentInputItem.m_nCount * i_comnum;

		vComponentInputItem.push_back(componentInputItem);

	}

	if (!str_OutedgePanel.IsEmpty())
	{
		AfxMessageBox(str_OutedgePanel);
	}

	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();


	std::vector<CString> barCodeRepeatCheck;

	for (int i = 0; i < vComponentInputItem.size() ; i++)
	{
		//如果重复需要插入新的条码
		//items[i]["info"] = uniqueBarCode(items[i]["info"].c_str(),barCodeRepeatCheck).GetBuffer();
		vComponentInputItem[i].m_strBarcode = uniqueBarCode(vComponentInputItem[i].m_strBarcode,barCodeRepeatCheck);

		//板件数量加倍

		////计算板件数量
		//All_com_num = All_com_num + atoi(items[i]["num"].c_str());

		//纹理删除多余空格
		 vComponentInputItem[i].m_strTexture.Replace(" ","");
		 vComponentInputItem[i].m_strTexture.Replace("　","");

		//正面孔
		if (!b_upvecImportEnable)
		{
			CString strHoleInfo = vComponentInputItem[i].m_strUpperFaceHoleInfo;
			vComponentInputItem[i].m_strUpperFaceHoleInfo = _T("");
		
			if(bReserveDeepHole)
			{
				CString strDeepHole;

				vector<AllVec> vHole;
				YHInfoTools::getAllVec(vHole, strHoleInfo, vComponentInputItem[i].m_strBarcode);
				for(int j = 0; j < vHole.size(); j++)
				{
					if((vHole[j]._depth - vComponentInputItem[i].m_fThickness) > -0.1)
					{
						CString strTmp;
						strTmp.Format(_T("%0.1f,%0.1f,%0.1f,%0.1f;"), vHole[j]._x, vHole[j]._y, vHole[j]._r, vHole[j]._depth);
						strDeepHole += strTmp;
					}
				}

				vComponentInputItem[i].m_strUpperFaceHoleInfo = strDeepHole;
			}
		}

		//反面孔
		if (!b_downvecImportEnable)
		{
			CString strHoleInfo = vComponentInputItem[i].m_strDownerFaceHoleInfo;
			vComponentInputItem[i].m_strDownerFaceHoleInfo = _T("");

			if(bReserveDeepHole)
			{
				CString strDeepHole;

				vector<AllVec> vHole;
				YHInfoTools::getAllVec(vHole, strHoleInfo, vComponentInputItem[i].m_strBarcode);
				for(int j = 0; j < vHole.size(); j++)
				{
					if((vHole[j]._depth - vComponentInputItem[i].m_fThickness) > -0.1)
					{
						CString strTmp;
						strTmp.Format(_T("%0.1f,%0.1f,%0.1f,%0.1f;"), vHole[j]._x, vHole[j]._y, vHole[j]._r, vHole[j]._depth);
						strDeepHole += strTmp;
					}
				}

				vComponentInputItem[i].m_strDownerFaceHoleInfo = strDeepHole;
			}
		}

		//正面槽
		if (!b_upsoltImportEnable)
		{
			CString strSlotInfo = vComponentInputItem[i].m_strUpperFaceSlotInfo;
			vComponentInputItem[i].m_strUpperFaceSlotInfo = _T("");

			if(bReserveDeepSlot)
			{
				CString strDeepSlot;

				vector<AllSlot> vSlot;
				YHInfoTools::getAllSlot(vSlot, strSlotInfo, _T(""), vComponentInputItem[i].m_strBarcode);
				for(int j = 0; j < vSlot.size(); j++)
				{
					if((vSlot[j]._depth - vComponentInputItem[i].m_fThickness) > -0.1)
					{
						CString strTmp;
						strTmp.Format(_T("%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;"), vSlot[j]._x, vSlot[j]._y, vSlot[j]._width, vSlot[j]._height, vSlot[j]._depth, vSlot[j]._dir);
						strDeepSlot += strTmp;
					}
				}

				vComponentInputItem[i].m_strUpperFaceSlotInfo = strDeepSlot;
			}
		}

		//反面槽
		if (!b_downsoltImportEnable)
		{
			CString strSlotInfo = vComponentInputItem[i].m_strDownerFaceSlotInfo;
			vComponentInputItem[i].m_strDownerFaceSlotInfo = _T("");

			if(bReserveDeepSlot)
			{
				CString strDeepSlot;

				vector<AllSlot> vSlot;
				YHInfoTools::getAllSlot(vSlot, strSlotInfo, _T(""), vComponentInputItem[i].m_strBarcode);
				for(int j = 0; j < vSlot.size(); j++)
				{
					if((vSlot[j]._depth - vComponentInputItem[i].m_fThickness) > -0.1)
					{
						CString strTmp;
						strTmp.Format(_T("%0.1f,%0.1f,%0.1f,%0.1f,%0.1f,%d;"), vSlot[j]._x, vSlot[j]._y, vSlot[j]._width, vSlot[j]._height, vSlot[j]._depth, vSlot[j]._dir);
						strDeepSlot += strTmp;
					}
				}

				vComponentInputItem[i].m_strDownerFaceSlotInfo = strDeepSlot;
			}
		}


		if(IsFloatEqual(vComponentInputItem[i].m_fProductLength, 0) || IsFloatEqual(vComponentInputItem[i].m_fProductWidth, 0) || IsFloatEqual(vComponentInputItem[i].m_fThickness, 0))
		{
			vComponentInputItem[i].m_fProductLength = vComponentInputItem[i].m_fLength + vComponentInputItem[i].m_afBanding[BANDING_ID_RIGHT] + vComponentInputItem[i].m_afBanding[BANDING_ID_LEFT];
			vComponentInputItem[i].m_fProductWidth = vComponentInputItem[i].m_fWidth + vComponentInputItem[i].m_afBanding[BANDING_ID_FRONT] + vComponentInputItem[i].m_afBanding[BANDING_ID_BACK];
			vComponentInputItem[i].m_fProductThickness = vComponentInputItem[i].m_fThickness;
		}

		//处理新老异形问题
		//try
		{
			//有outline的，不管异形（上面已有）36
			if (!vComponentInputItem[i].m_strAbnormalInfo.IsEmpty() && b_othershapeImportEnable)
			{
				//根据界面选择的旋转对图形进行旋转
				std::vector<PointInfo> m_pointInfos = OtherShapeHelper::GetPointInfoFromPointInfoString(vComponentInputItem[i].m_strAbnormalInfo.GetBuffer());
				vComponentInputItem[i].m_strAbnormalInfo.ReleaseBuffer();
				OtherShapeHelper::GetPointsToYPlus(m_pointInfos, \
					GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0).GetBuffer(), \
					GetFloatString(vComponentInputItem[i].m_fLength, 1).GetBuffer(), \
					GetFloatString(vComponentInputItem[i].m_fWidth, 1).GetBuffer());
				m_pointInfos = OtherShapeHelper::getPointInfoRotate(m_pointInfos, \
					string(GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0)), \
					string(GetFloatString(vComponentInputItem[i].m_fLength, 1)), \
					string(GetFloatString(vComponentInputItem[i].m_fWidth, 1)));
				vComponentInputItem[i].m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(m_pointInfos).c_str();
			}
			//没有outline的，用异形字符串生成outline
			else if (vComponentInputItem[i].m_strAbnormalInfo.IsEmpty() && !vComponentInputItem[i].m_strOtherShapeIDAndParam.IsEmpty() && b_othershapeImportEnable)
			{
#if NO_OTHER_SHAPE_AUTO_CREATE 
				//获得界面的所有参数，并校验
				std::vector<param_point> pa_p = OtherShapeHelper::GetParamPointsFromOtherShapeString(vComponentInputItem[i].m_strOtherShapeIDAndParam.GetBuffer(), \
					GetFloatString((int)vComponentInputItem[i].m_fLength, 0).GetBuffer(), \
					GetFloatString((int)vComponentInputItem[i].m_fWidth, 0).GetBuffer());
				vComponentInputItem[i].m_strOtherShapeIDAndParam.ReleaseBuffer();

				//OutputDebugString("START:\n");
				//for(int k = 0; k < pa_p.size(); k++)
				//{
				//	CString strTmp;
				//	strTmp.Format("name=%s value =%s\n", pa_p[k]._name.c_str(), pa_p[k]._value.c_str());
				//	OutputDebugString(strTmp);
				//}

				OtherShapeHelper::RotateOtherShapeParamPoint(pa_p, GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0).GetBuffer());

				//绘制界面参数所生成的图形
				std::vector<std::map<std::string,std::string>>m_outline = OtherShapeHelper::GetOtherShapePanelOutLineListFromOtherShapeID(OtherShapeHelper::GetOtherShapeId(pa_p));
				std::vector<PointInfo> m_pointInfos = OtherShapeHelper::GetPointInfoFromPanelOutLineAndParamPoint(m_outline,pa_p);

				//OutputDebugString("START:\n");
				//for(int k = 0; k < m_pointInfos.size(); k++)
				//{
				//	CString strTmp;
				//	strTmp.Format("x=%f y =%f\n", m_pointInfos[k].x, m_pointInfos[k].y);
				//	OutputDebugString(strTmp);
				//}

				OtherShapeHelper::GetPointsToYPlus(m_pointInfos, \
					GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0).GetBuffer(), \
					GetFloatString(vComponentInputItem[i].m_fLength, 1).GetBuffer(), \
					GetFloatString(vComponentInputItem[i].m_fWidth, 1).GetBuffer());

				//根据界面选择的旋转对图形进行旋转
				m_pointInfos = OtherShapeHelper::RotateOtherShapePointInfos(m_pointInfos, \
					GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0).GetBuffer(), \
					GetFloatString(vComponentInputItem[i].m_fLength, 1).GetBuffer(), \
					GetFloatString(vComponentInputItem[i].m_fWidth, 1).GetBuffer());

				vComponentInputItem[i].m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(m_pointInfos).c_str();
#endif
			}
			else	//啥都没有生成outline为长方形
			{
#if NO_OTHER_SHAPE_AUTO_CREATE 
				vComponentInputItem[i].m_strOtherShapeID = "异0";
				vComponentInputItem[i].m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(
					OtherShapeHelper::getPointInfoRectangle(
					string(GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0)), \
					string(GetFloatString(vComponentInputItem[i].m_fLength, 1)), \
					string(GetFloatString(vComponentInputItem[i].m_fWidth, 1))) \
					).c_str();
#endif
			}
		}
		//catch (...)
		//{
			//vComponentInputItem[i].m_strOtherShapeID = "异0";
			//vComponentInputItem[i].m_strAbnormalInfo = OtherShapeHelper::GetPointInfoStringFromPointInfo(
			//OtherShapeHelper::getPointInfoRectangle(
			//string(GetFloatString((int)vComponentInputItem[i].m_eRotateType, 0)), \
			//string(GetFloatString(vComponentInputItem[i].m_fLength, 1)), \
			//string(GetFloatString(vComponentInputItem[i].m_fWidth, 1))) \
			//).c_str();
			////continue;
		//}
	}


	return true;
}
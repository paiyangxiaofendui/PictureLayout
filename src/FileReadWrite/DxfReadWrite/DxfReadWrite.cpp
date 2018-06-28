/*--------------------------------------------------------------------------------------------------------------------*/
//	DxfReadWrite.cpp      -- dxf文件读写类实现文件
//	
//	作者：	yuanzb
//	时间：	2017.7.21
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DxfReadWrite.h"
#include "dl_dxf.h"



// 外部DataManager.dll的头文件
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleon.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/Algorithm/Algorithm.h"



// AbnormalShapeManager
#include "../../../include/AbnormalShapeManager/DataManager/CommonData.h"
#include "../../../include/AbnormalShapeManager/DataManager/CSingleton.h"
#include "../../../include/AbnormalShapeManager/DataManager/Layer/Layer.h"
#include "../../../include/AbnormalShapeManager/DataManager/Figure/Complex.h"
#include "../../../include/AbnormalShapeManager/DataManager/Figure/Arc.h"
#include "../../../include/AbnormalShapeManager/DataManager/Point/ShapePoint.h"
#include "../../../include/AbnormalShapeManager/GeneralInterface/GeneralInterface.h"


/*-------------------------------------------------------*/
//	函数说明：
//		导出正面的DXF文件
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
/*-------------------------------------------------------*/
bool DxfReadWrite::OutputUpperFaceDxf(Panel* pPanel, CString strDxfFilePath) 
{


	//2017-7-20 yuanzb  以下是范例代码，一般情况下不做更改，只更改ENTITIES段的数据


    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out(strDxfFilePath.GetBuffer(), exportVersion);

    if (dw==NULL) 
	{
		return false;
    }

    dxf->writeHeader(*dw);
    /*
    // int variable:
    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, 4);
    // real (double, float) variable:
    dw->dxfString(9, "$DIMEXE");
    dw->dxfReal(40, 1.25);
    // string variable:
    dw->dxfString(9, "$TEXTSTYLE");
    dw->dxfString(7, "Standard");
    // vector variable:
    dw->dxfString(9, "$LIMMIN");
    dw->dxfReal(10, 0.0);
    dw->dxfReal(20, 0.0);
    */
    dw->sectionEnd();
    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLinetypes(3);
    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dw->tableEnd();

    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);

    dxf->writeLayer(*dw,
                   DL_LayerData("0", 0),
                   DL_Attributes(
                       std::string(""),      // leave empty
                       DL_Codes::black,        // default color
                       100,                  // default width
                       "CONTINUOUS", 1.0));       // default line style
 
    dxf->writeLayer(*dw,
                   DL_LayerData("mainlayer", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::red,
                       100,
                       "CONTINUOUS", 1.0));

    dxf->writeLayer(*dw,
                   DL_LayerData("anotherlayer", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::black,
                       100,
                       "CONTINUOUS", 1.0));

    dw->tableEnd();

    dw->tableStyle(1);
    dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "txt", ""));
    dw->tableEnd();

    dxf->writeView(*dw);
    dxf->writeUcs(*dw);

    dw->tableAppid(1);
    dxf->writeAppid(*dw, "ACAD");
    dw->tableEnd();

    dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);

    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();

    dw->sectionEnd();

    dw->sectionBlocks();
    dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw, DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");

    dxf->writeBlock(*dw, DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");

    dw->sectionEnd();
    dw->sectionEntities();


	// 下面开始将孔、槽、板件数据写入ENTITIES段

#if 1

	// 获取内存数据
// 	CSingleon* pSingleton = CSingleon::GetSingleton();
// 	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
// 	int nSlnNum = SlnList.size();
// 
// 	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
// 	{
// 		CSolution* pCurSln = SlnList.at(i_sln);
// 
// 		int nPanelNum = pCurSln->GetPanelNum();
// 		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
// 		{
// 			Panel* pPanel = pCurSln->GetPanel(i_panel);


			// 大板轮廓
#if 1

			// 画矩形
			double panel_pnt1_x = 0;
			double panel_pnt1_y = 0;

			double panel_pnt2_x = pPanel->m_OrgLen;
			double panel_pnt2_y = 0;

			double panel_pnt3_x = pPanel->m_OrgLen;
			double panel_pnt3_y = pPanel->m_OrgWidth;

			double panel_pnt4_x = 0;
			double panel_pnt4_y = pPanel->m_OrgWidth;

			// 第一条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt1_x,   // start point
				panel_pnt1_y,
				0.0,
				panel_pnt2_x,   // end point
				panel_pnt2_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

			// 第二条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt2_x,   // start point
				panel_pnt2_y,
				0.0,
				panel_pnt3_x,   // end point
				panel_pnt3_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

			// 第三条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt3_x,   // start point
				panel_pnt3_y,
				0.0,
				panel_pnt4_x,   // end point
				panel_pnt4_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

			// 第四条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt4_x,   // start point
				panel_pnt4_y,
				0.0,
				panel_pnt1_x,   // end point
				panel_pnt1_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));



#endif


			// 板件轮廓点处理模块
			

			vector<Component*> CpnList;
			pPanel->GetAllNeededComponent(CpnList);

			int nCpnCount = CpnList.size();
			for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
			{
				Component* pCpn = CpnList.at(i_cpn);

// 板件信息
#if 1
				float info_offset = 20;

				float info_x = pCpn->m_x + info_offset;
				float info_y =  pCpn->m_y + info_offset;

				CString str_area;
				str_area.Format("%0.1f x %0.1f", pCpn->m_RealLength, pCpn->m_RealWidth);

				CString str_info = pCpn->m_BarCode;
				str_info = str_info +"  " + pCpn->m_strComponentName +"  " + str_area ;



				dxf->writeMText(*dw,
					DL_MTextData(info_x, info_y, 0,
								 0, 0, 0,
								 5, pCpn->m_RealLength - 2*info_offset,
								 1,1,1,2.5,
								 str_info.GetBuffer(),"",0),
								 DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0)); 


#endif



// 小板轮廓
#if 1

				// 将异形的点分段
				vector<PointInfo> points = pCpn->m_vOutlinePoint;
				vector<vector<PointInfo>> _pNewpoint;
				vector<PointInfo> _tmppoint;
				for (int n_p = 0;n_p < points.size();n_p++)
				{
					int _cut = points.at(n_p).cut;
					if (_cut == 1)
					{
						_tmppoint.push_back(points.at(n_p));
						_pNewpoint.push_back(_tmppoint);
						_tmppoint.clear();
					}
					else if (_cut == 2)
					{
						_tmppoint.clear();
						_tmppoint.push_back(points.at(n_p));
					}
					else
					{
						_tmppoint.push_back(points.at(n_p));
					}
				}

				// 有这么多的点
				if (_tmppoint.size() > 0)
					_pNewpoint.push_back(_tmppoint);

				for (int _pn = 0;_pn < _pNewpoint.size();_pn++)
				{
					int nCount;
					vector<PointInfo> newpoint;
					vector<PointInfo> tmpnewpoint;

					// 这一次的轮廓点
					tmpnewpoint = _pNewpoint.at(_pn);

					// 过滤重合点
					tmpnewpoint = ALGORITHM_API::FiltrateCoincidentPoints2(tmpnewpoint);

					// 根据点组织成所需要的数据结构
					Cad_Complex* pComplex = new Cad_Complex;

					// 重新获取形状点个数
					nCount = tmpnewpoint.size();
					for (int index = 0; index < nCount; index++)
					{
						PointInfo cur_point = tmpnewpoint[index];

						if (index == 0)
						{
							if (cur_point.sign == 0)	// 直线直接添加
							{
								ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

								pPnt->m_FigureType = FigureType_PolyLine;
								pComplex->AddShapePoint(pPnt);
							}
							else if (cur_point.sign == 1 || cur_point.sign == 3)	// 圆弧起点或圆弧连接点(属于数据错误但暂时先兼容) 跟圆弧终点一起算，添加一个圆弧
							{
								float center_x, center_y, r,sign ;
								float mid_x, mid_y, vec_x, vec_y;
								PointInfo end_pnt = tmpnewpoint[index+1];

								// 求圆心
								r = end_pnt.r;
								sign = end_pnt.dir;
								ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x,cur_point.y, end_pnt.y, r, sign, center_x, center_y);

								// 求弦中点绝对坐标
								mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
								mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

								vec_x = mid_x - center_x;
								vec_y = mid_y - center_y;

								ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
								vec_x *= r;
								vec_y *= r;

								// 求圆弧中点绝对坐标
								mid_x = center_x + vec_x;
								mid_y = center_y + vec_y;

								// 添加圆弧三个点
								ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
								ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
								ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

								pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;


								pComplex->AddShapePoint(pStart);
								pComplex->AddShapePoint(pMid);
								pComplex->AddShapePoint(pEnd);

								//index++;
							}
						}
						else
						{
							PointInfo prev_point = tmpnewpoint[index-1];

							if (cur_point.sign == 0)	// 直线直接添加
							{
								ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

								pPnt->m_FigureType = FigureType_PolyLine;
								pComplex->AddShapePoint(pPnt);
							}
							else if (cur_point.sign == 1 || cur_point.sign == 3)	// 圆弧起点 跟圆弧终点一起算，添加一个圆弧
							{
								float center_x, center_y, r,sign ;
								float mid_x, mid_y, vec_x, vec_y;
								PointInfo end_pnt = tmpnewpoint[index+1];

								// 求圆心
								r = end_pnt.r;
								sign = end_pnt.dir;	// 圆弧方向
								ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x, cur_point.y, end_pnt.y, r, sign, center_x, center_y);

								// 求弦中点绝对坐标
								mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
								mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

								vec_x = mid_x - center_x;
								vec_y = mid_y - center_y;

								ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
								vec_x *= r;
								vec_y *= r;

								// 求圆弧中点绝对坐标
								mid_x = center_x + vec_x;
								mid_y = center_y + vec_y;

								// 添加圆弧三个点
								ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
								ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
								ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

								if (prev_point.sign == 0 || prev_point.sign == 2)	// 前一点为直线或者圆弧终点，说明前一段是直线，当前点的图形图形设为直线
								{
									pStart->m_FigureType = FigureType_PolyLine;
									pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
								}
								else
								{
									pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
								}

								if (pStart->IsCloseTo(pComplex->GetTailShapePoint()) == FALSE)	// 不与最后一点重合，才添加第一点
								{
									pComplex->AddShapePoint(pStart);
								}

								pComplex->AddShapePoint(pMid);
								pComplex->AddShapePoint(pEnd);
							}
						}
					}

					// 复合图形已构造完毕，写入dxf中
					pComplex->CalComplexFigureList();


					int nFigCount = pComplex->m_ComplexFigureList.size();
					for (int i_fig = 0; i_fig < nFigCount; i_fig++)
					{
						Figure* pFig = pComplex->m_ComplexFigureList.at(i_fig);

						if (pFig->m_FigureType == FigureType_PolyLine)
						{
							// 直线
							// 暂时使用画线段的方式，以后采用画折线的方式

							int nPntCount = pFig->GetShapePointNum();
							for (int i_pnt = 0; i_pnt < nPntCount-1; i_pnt++)
							{
								ShapePoint* pFirst = pFig->GetShapePoint(i_pnt);
								ShapePoint* pNext = pFig->GetShapePoint(i_pnt+1);

								double real_start_x = pCpn->m_x + pFirst->m_x;
								double real_start_y = pCpn->m_y + pFirst->m_y;
								double real_end_x = pCpn->m_x + pNext->m_x;
								double real_end_y = pCpn->m_y + pNext->m_y;


								dxf->writeLine(
									*dw,
									DL_LineData(
									real_start_x,   // start point
									real_start_y,
									0.0,
									real_end_x,   // end point
									real_end_y,
									0.0),
									DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));
							}
						}
						else if (pFig->m_FigureType == FigureType_Arc)
						{
							// 圆弧
							Cad_Arc* pArc = static_cast<Cad_Arc*>(pFig);

							double real_x = pCpn->m_x + pArc->m_CenterX;
							double real_y = pCpn->m_y + pArc->m_CenterY;
							double r = pArc->m_radius;
							double start_angle, end_angle;
							if (pArc->GetDirection() == Dir_Clockwise)
							{
								// CAD画圆时逆时针，顺时针的圆弧要反过来画
								start_angle = Radian2Angle(pArc->m_EndAngle);
								end_angle = Radian2Angle(pArc->m_StartAngle);
							}
							else
							{
								// CAD画圆时逆时针
								start_angle = Radian2Angle(pArc->m_StartAngle);
								end_angle = Radian2Angle(pArc->m_EndAngle);
							}


							dxf->writeArc(*dw,
								DL_ArcData(real_x, real_y, 0, r, start_angle, end_angle),
								DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));


						}
						else
						{
							// 其他不作处理
						}
					}
				}



#endif


// 孔
#if 1

				vector<AllVec> UpperFaceHole = pCpn->m_vUpperFaceHole;

				int nUpperFaceHoleCount = UpperFaceHole.size();
				for (int i_hole = 0; i_hole < nUpperFaceHoleCount; i_hole++)
				{
					AllVec cur_hole = UpperFaceHole.at(i_hole);

					// 画圆
					double real_x = pCpn->m_x + cur_hole._x;
					double real_y = pCpn->m_y +cur_hole._y;
					double r = cur_hole._r;

					dxf->writeCircle(*dw,
						 			DL_CircleData(real_x, real_y, 0, r/2),
						 			DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));

				}

#endif


// 槽
#if 1

				// 存在正面槽
				int nUpperFaceSlotCount = pCpn->m_vUpperFaceSlot.size();
				for(int i_upper_face_slot = 0; i_upper_face_slot < nUpperFaceSlotCount; i_upper_face_slot++)
				{
					AllSlot	OneSlot;

					OneSlot = pCpn->m_vUpperFaceSlot.at(i_upper_face_slot);

					// 画矩形
					double pnt1_x = pCpn->m_x + OneSlot._x - OneSlot._width/2;
					double pnt1_y = pCpn->m_y + OneSlot._y + OneSlot._height/2;

					double pnt2_x = pCpn->m_x + OneSlot._x - OneSlot._width/2;
					double pnt2_y = pCpn->m_y + OneSlot._y - OneSlot._height/2;

					double pnt3_x = pCpn->m_x + OneSlot._x + OneSlot._width/2;
					double pnt3_y = pCpn->m_y + OneSlot._y - OneSlot._height/2;

					double pnt4_x = pCpn->m_x + OneSlot._x + OneSlot._width/2;
					double pnt4_y = pCpn->m_y + OneSlot._y + OneSlot._height/2;

					// 第一条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt1_x,   // start point
						pnt1_y,
						0.0,
						pnt2_x,   // end point
						pnt2_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

					// 第二条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt2_x,   // start point
						pnt2_y,
						0.0,
						pnt3_x,   // end point
						pnt3_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));
					
					// 第三条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt3_x,   // start point
						pnt3_y,
						0.0,
						pnt4_x,   // end point
						pnt4_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

					// 第四条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt4_x,   // start point
						pnt4_y,
						0.0,
						pnt1_x,   // end point
						pnt1_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));


				}




#endif




 			}
// 		}
// 	}


#endif


    dw->sectionEnd();

    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);

    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;

	return true;
}


/*-------------------------------------------------------*/
//	函数说明：
//		导出反面的DXF文件
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
/*-------------------------------------------------------*/
bool DxfReadWrite::OutputDownerFaceDxf(Panel* pPanel, CString strDxfFilePath) 
{


	//2017-7-20 yuanzb  以下是范例代码，一般情况下不做更改，只更改ENTITIES段的数据


    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out(strDxfFilePath.GetBuffer(), exportVersion);

    if (dw==NULL) 
	{
		return false;
    }

    dxf->writeHeader(*dw);
    /*
    // int variable:
    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, 4);
    // real (double, float) variable:
    dw->dxfString(9, "$DIMEXE");
    dw->dxfReal(40, 1.25);
    // string variable:
    dw->dxfString(9, "$TEXTSTYLE");
    dw->dxfString(7, "Standard");
    // vector variable:
    dw->dxfString(9, "$LIMMIN");
    dw->dxfReal(10, 0.0);
    dw->dxfReal(20, 0.0);
    */
    dw->sectionEnd();
    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLinetypes(3);
    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dw->tableEnd();

    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);

    dxf->writeLayer(*dw,
                   DL_LayerData("0", 0),
                   DL_Attributes(
                       std::string(""),      // leave empty
                       DL_Codes::black,        // default color
                       100,                  // default width
                       "CONTINUOUS", 1.0));       // default line style
 
    dxf->writeLayer(*dw,
                   DL_LayerData("mainlayer", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::red,
                       100,
                       "CONTINUOUS", 1.0));

    dxf->writeLayer(*dw,
                   DL_LayerData("anotherlayer", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::black,
                       100,
                       "CONTINUOUS", 1.0));

    dw->tableEnd();

    dw->tableStyle(1);
    dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "txt", ""));
    dw->tableEnd();

    dxf->writeView(*dw);
    dxf->writeUcs(*dw);

    dw->tableAppid(1);
    dxf->writeAppid(*dw, "ACAD");
    dw->tableEnd();

    dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);

    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();

    dw->sectionEnd();

    dw->sectionBlocks();
    dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw, DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");

    dxf->writeBlock(*dw, DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");

    dw->sectionEnd();
    dw->sectionEntities();


	// 下面开始将孔、槽、板件数据写入ENTITIES段

#if 1

	// 获取内存数据
// 	CSingleon* pSingleton = CSingleon::GetSingleton();
// 	vector<CSolution*>& SlnList = pSingleton->m_BackupSolutionList;
// 	int nSlnNum = SlnList.size();
// 
// 	for (int i_sln = 0; i_sln < nSlnNum; i_sln++)
// 	{
// 		CSolution* pCurSln = SlnList.at(i_sln);
// 
// 		int nPanelNum = pCurSln->GetPanelNum();
// 		for (int i_panel = 0; i_panel < nPanelNum; i_panel++)
// 		{
// 			Panel* pPanel = pCurSln->GetPanel(i_panel);

			// 大板轮廓
#if 1

			// 画矩形
			double panel_pnt1_x = 0;
			double panel_pnt1_y = 0;

			double panel_pnt2_x = pPanel->m_OrgLen;
			double panel_pnt2_y = 0;

			double panel_pnt3_x = pPanel->m_OrgLen;
			double panel_pnt3_y = pPanel->m_OrgWidth;

			double panel_pnt4_x = 0;
			double panel_pnt4_y = pPanel->m_OrgWidth;

			// 第一条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt1_x,   // start point
				panel_pnt1_y,
				0.0,
				panel_pnt2_x,   // end point
				panel_pnt2_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));

			// 第二条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt2_x,   // start point
				panel_pnt2_y,
				0.0,
				panel_pnt3_x,   // end point
				panel_pnt3_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));

			// 第三条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt3_x,   // start point
				panel_pnt3_y,
				0.0,
				panel_pnt4_x,   // end point
				panel_pnt4_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));

			// 第四条线
			dxf->writeLine(
				*dw,
				DL_LineData(
				panel_pnt4_x,   // start point
				panel_pnt4_y,
				0.0,
				panel_pnt1_x,   // end point
				panel_pnt1_y,
				0.0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));



#endif



			// 板件轮廓点处理模块
			

			vector<Component*> CpnList;
			pPanel->GetAllNeededComponent(CpnList);

			int nCpnCount = CpnList.size();
			for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
			{
				Component* pCpn = CpnList.at(i_cpn);



				// 板件信息
#if 1
				float info_offset = 20;

				float info_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + info_offset;
				float info_y =  pCpn->m_y + info_offset;

				CString str_area;
				str_area.Format("%0.1f x %0.1f", pCpn->m_RealLength, pCpn->m_RealWidth);

				CString str_info = pCpn->m_BarCode;
				str_info = str_info +"  " + pCpn->m_strComponentName +"  " + str_area ;



				dxf->writeMText(*dw,
					DL_MTextData(info_x, info_y, 0,
					0, 0, 0,
					5, pCpn->m_RealLength - 2*info_offset,
					1,1,1,2.5,
					str_info.GetBuffer(),"",0),
					DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0)); 


#endif






// 小板轮廓
#if 1

				// 将异形的点分段
				vector<PointInfo> points = pCpn->m_vOutlinePoint;
				vector<vector<PointInfo>> _pNewpoint;
				vector<PointInfo> _tmppoint;
				for (int n_p = 0;n_p < points.size();n_p++)
				{
					int _cut = points.at(n_p).cut;
					if (_cut == 1)
					{
						_tmppoint.push_back(points.at(n_p));
						_pNewpoint.push_back(_tmppoint);
						_tmppoint.clear();
					}
					else if (_cut == 2)
					{
						_tmppoint.clear();
						_tmppoint.push_back(points.at(n_p));
					}
					else
					{
						_tmppoint.push_back(points.at(n_p));
					}
				}

				// 有这么多的点
				if (_tmppoint.size() > 0)
					_pNewpoint.push_back(_tmppoint);

				for (int _pn = 0;_pn < _pNewpoint.size();_pn++)
				{
					int nCount;
					vector<PointInfo> newpoint;
					vector<PointInfo> tmpnewpoint;

					// 这一次的轮廓点
					tmpnewpoint = _pNewpoint.at(_pn);

					// 过滤重合点
					tmpnewpoint = ALGORITHM_API::FiltrateCoincidentPoints2(tmpnewpoint);

					// 根据点组织成所需要的数据结构
					Cad_Complex* pComplex = new Cad_Complex;

					// 重新获取形状点个数
					nCount = tmpnewpoint.size();
					for (int index = 0; index < nCount; index++)
					{
						PointInfo cur_point = tmpnewpoint[index];

						if (index == 0)
						{
							if (cur_point.sign == 0)	// 直线直接添加
							{
								ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

								pPnt->m_FigureType = FigureType_PolyLine;
								pComplex->AddShapePoint(pPnt);
							}
							else if (cur_point.sign == 1 || cur_point.sign == 3)	// 圆弧起点或圆弧连接点(属于数据错误但暂时先兼容) 跟圆弧终点一起算，添加一个圆弧
							{
								float center_x, center_y, r,sign ;
								float mid_x, mid_y, vec_x, vec_y;
								PointInfo end_pnt = tmpnewpoint[index+1];

								// 求圆心
								r = end_pnt.r;
								sign = end_pnt.dir;
								ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x,cur_point.y, end_pnt.y, r, sign, center_x, center_y);

								// 求弦中点绝对坐标
								mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
								mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

								vec_x = mid_x - center_x;
								vec_y = mid_y - center_y;

								ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
								vec_x *= r;
								vec_y *= r;

								// 求圆弧中点绝对坐标
								mid_x = center_x + vec_x;
								mid_y = center_y + vec_y;

								// 添加圆弧三个点
								ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
								ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
								ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

								pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;


								pComplex->AddShapePoint(pStart);
								pComplex->AddShapePoint(pMid);
								pComplex->AddShapePoint(pEnd);

								//index++;
							}
						}
						else
						{
							PointInfo prev_point = tmpnewpoint[index-1];

							if (cur_point.sign == 0)	// 直线直接添加
							{
								ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

								pPnt->m_FigureType = FigureType_PolyLine;
								pComplex->AddShapePoint(pPnt);
							}
							else if (cur_point.sign == 1 || cur_point.sign == 3)	// 圆弧起点 跟圆弧终点一起算，添加一个圆弧
							{
								float center_x, center_y, r,sign ;
								float mid_x, mid_y, vec_x, vec_y;
								PointInfo end_pnt = tmpnewpoint[index+1];

								// 求圆心
								r = end_pnt.r;
								sign = end_pnt.dir;	// 圆弧方向
								ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x, cur_point.y, end_pnt.y, r, sign, center_x, center_y);

								// 求弦中点绝对坐标
								mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
								mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

								vec_x = mid_x - center_x;
								vec_y = mid_y - center_y;

								ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
								vec_x *= r;
								vec_y *= r;

								// 求圆弧中点绝对坐标
								mid_x = center_x + vec_x;
								mid_y = center_y + vec_y;

								// 添加圆弧三个点
								ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
								ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
								ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

								if (prev_point.sign == 0 || prev_point.sign == 2)	// 前一点为直线或者圆弧终点，说明前一段是直线，当前点的图形图形设为直线
								{
									pStart->m_FigureType = FigureType_PolyLine;
									pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
								}
								else
								{
									pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
								}

								if (pStart->IsCloseTo(pComplex->GetTailShapePoint()) == FALSE)	// 不与最后一点重合，才添加第一点
								{
									pComplex->AddShapePoint(pStart);
								}

								pComplex->AddShapePoint(pMid);
								pComplex->AddShapePoint(pEnd);
							}
						}
					}

					// 复合图形已构造完毕，写入dxf中
					pComplex->CalComplexFigureList();


					int nFigCount = pComplex->m_ComplexFigureList.size();
					for (int i_fig = 0; i_fig < nFigCount; i_fig++)
					{
						Figure* pFig = pComplex->m_ComplexFigureList.at(i_fig);

						if (pFig->m_FigureType == FigureType_PolyLine)
						{
							// 直线
							// 暂时使用画线段的方式，以后采用画折线的方式

							int nPntCount = pFig->GetShapePointNum();
							for (int i_pnt = 0; i_pnt < nPntCount-1; i_pnt++)
							{
								ShapePoint* pFirst = pFig->GetShapePoint(i_pnt);
								ShapePoint* pNext = pFig->GetShapePoint(i_pnt+1);

								double real_start_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - pFirst->m_x);
								double real_start_y = pCpn->m_y + pFirst->m_y;
								double real_end_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) +  (pCpn->m_RealLength - pNext->m_x);
								double real_end_y = pCpn->m_y + pNext->m_y;


								dxf->writeLine(
									*dw,
									DL_LineData(
									real_start_x,   // start point
									real_start_y,
									0.0,
									real_end_x,   // end point
									real_end_y,
									0.0),
									DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));
							}
						}
						else if (pFig->m_FigureType == FigureType_Arc)
						{
							// 圆弧
							Cad_Arc* pCurArc = static_cast<Cad_Arc*>(pFig);

							ShapePoint* pCurStart	= pCurArc->GetShapePoint(0);
							ShapePoint*	pCurMid		= pCurArc->GetShapePoint(1);
							ShapePoint*	pCurEnd		= pCurArc->GetShapePoint(2);


							// 先翻转点，再计算圆弧的圆心、起始结束角度等参数，才是正确的翻转后的圆弧
							Cad_Arc* TmpArc = new Cad_Arc;

							// 添加圆弧三个点 真实的相对大板左下角的坐标
							ShapePoint* TmpStart = new ShapePoint(	(pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - pCurStart->m_x),  pCpn->m_y + pCurStart->m_y);
							ShapePoint* TmpMid = new ShapePoint(	(pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - pCurMid->m_x),	pCpn->m_y + pCurMid->m_y);
							ShapePoint* TmpEnd = new ShapePoint(	(pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - pCurEnd->m_x),	pCpn->m_y + pCurEnd->m_y);

							TmpArc->AddShapePoint(TmpStart);
							TmpArc->AddShapePoint(TmpMid);
							TmpArc->AddShapePoint(TmpEnd);

							TmpArc->CalCenterRadiusInfo();


							double real_x = TmpArc->m_CenterX;
							double real_y = TmpArc->m_CenterY;
							double r = TmpArc->m_radius;
							double start_angle ;
							double end_angle;

							if (TmpArc->GetDirection() == Dir_Clockwise)
							{
								// CAD画圆时逆时针，顺时针的圆弧要反过来画
								start_angle = Radian2Angle(TmpArc->m_EndAngle);
								end_angle = Radian2Angle(TmpArc->m_StartAngle);
							}
							else
							{
								// CAD画圆时逆时针
								start_angle = Radian2Angle(TmpArc->m_StartAngle);
								end_angle = Radian2Angle(TmpArc->m_EndAngle);
							}
					


// 							double real_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - pCurArc->m_CenterX);
// 							double real_y = pCpn->m_y + pCurArc->m_CenterY;
// 							double r = pCurArc->m_radius;
// 
// 							double start_angle, end_angle;
// 							if (pCurArc->GetDirection() == Dir_Clockwise)
// 								{
// 								// CAD画圆时逆时针，顺时针的圆弧要反过来画
// 								start_angle = Radian2Angle(pCurArc->m_EndAngle) + 90;
// 									end_angle = Radian2Angle(pCurArc->m_StartAngle) + 90;
// 							}
// 							else
// 							{
// 								// CAD画圆时逆时针
// 								start_angle = Radian2Angle(pCurArc->m_StartAngle) + 90;
// 								end_angle = Radian2Angle(pCurArc->m_EndAngle) + 90;
// 							}


							

							dxf->writeArc(*dw,
								DL_ArcData(real_x, real_y, 0, r, start_angle, end_angle),
								DL_Attributes("mainlayer", 256, -1, "BYLAYER", 4.0));

							delete TmpArc;
							TmpArc = NULL;

						}
						else
						{
							// 其他不作处理

						}
					}
				}



#endif


// 孔
#if 1

				vector<AllVec> DownerFaceHole = pCpn->m_vDownerFaceHole;

				int nDownerFaceHoleCount = DownerFaceHole.size();
				for (int i_hole = 0; i_hole < nDownerFaceHoleCount; i_hole++)
				{
					AllVec cur_hole = DownerFaceHole.at(i_hole);

					// 画圆
					double real_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) +  (pCpn->m_RealLength - cur_hole._x);
					double real_y = pCpn->m_y +cur_hole._y;
					double r = cur_hole._r;

					dxf->writeCircle(*dw,
						 			DL_CircleData(real_x, real_y, 0, r/2),
						 			DL_Attributes("mainlayer", 256, -1, "BYLAYER", 4.0));

				}

#endif


// 槽
#if 1

				// 存在正面槽
				int nDownerFaceSlotCount = pCpn->m_vDownerFaceSlot.size();
				for(int i_downer_face_slot = 0; i_downer_face_slot < nDownerFaceSlotCount; i_downer_face_slot++)
				{
					AllSlot	OneSlot;

					OneSlot = pCpn->m_vDownerFaceSlot.at(i_downer_face_slot);

					// 画矩形
					double pnt1_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - OneSlot._x) - OneSlot._width/2;
					double pnt1_y = pCpn->m_y + OneSlot._y + OneSlot._height/2;

					double pnt2_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - OneSlot._x) - OneSlot._width/2;
					double pnt2_y = pCpn->m_y + OneSlot._y - OneSlot._height/2;

					double pnt3_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - OneSlot._x) + OneSlot._width/2;
					double pnt3_y = pCpn->m_y + OneSlot._y - OneSlot._height/2;

					double pnt4_x = (pPanel->m_OrgLen - pCpn->m_x - pCpn->m_RealLength) + (pCpn->m_RealLength - OneSlot._x) + OneSlot._width/2;
					double pnt4_y = pCpn->m_y + OneSlot._y + OneSlot._height/2;

					// 第一条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt1_x,   // start point
						pnt1_y,
						0.0,
						pnt2_x,   // end point
						pnt2_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));

					// 第二条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt2_x,   // start point
						pnt2_y,
						0.0,
						pnt3_x,   // end point
						pnt3_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));
					
					// 第三条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt3_x,   // start point
						pnt3_y,
						0.0,
						pnt4_x,   // end point
						pnt4_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));

					// 第四条线
					dxf->writeLine(
						*dw,
						DL_LineData(
						pnt4_x,   // start point
						pnt4_y,
						0.0,
						pnt1_x,   // end point
						pnt1_y,
						0.0),
						DL_Attributes("mainlayer", 256, -1, "BYLAYER", 2.0));


				}




#endif




			}
// 		}
// 	}


#endif



    dw->sectionEnd();

    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);

    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;

	return true;
}



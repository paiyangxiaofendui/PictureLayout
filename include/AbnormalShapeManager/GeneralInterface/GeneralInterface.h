/*--------------------------------------------------------------------------------------------------------------------*/
//	GeneralInterface.h    -- 公共借口类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.12
//	备注：	数据操作公共接口
//
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef		GeneralInterface_H
#define		GeneralInterface_H

#include <vector>
using namespace std;

class Layer;
class Figure;
class ShapePoint;
class FPoint;

class AFX_EXT_CLASS GeneralInterface
{
public:
	//单例类操作
	static void ClearLayerList(vector<Layer*>* pList);						// 清空图层链表
public:
	//图层链表操作
	//static void ClearLayerList(vector<Figure*>* pList);					// 清空图形链表

public:
	// 平行图形链表
	static BOOL ParallelFigureList(vector<Figure*>* pSrcList, vector<Figure*>* pDstList, int dir, float dist);

	//图形链表操作
	static void ClearFigureList(vector<Figure*>* pList);					// 清空图形链表

	// 图形链表排序 -- 按矩形大小
	static void SortFigureListByRect(vector<Figure*>* pList);					// 清空图形链表

	static BOOL CalFigureIntersect(Figure* pSrcFig, Figure* pDstFig, FPoint& inter_pnt);			// 计算两图形是否相交

	static vector<ShapePoint*> ArcInterpolate(ShapePoint* center, ShapePoint* start, ShapePoint* end, float r);	// 对圆弧插补若干个点

public:
	// 形状点链表
	static void MoveShapePointList(vector<ShapePoint*>* pList, float rel_x, float rel_y);	// 移动形状点链表
	static void ReverseShapePointList(vector<ShapePoint*>* pList);			// 反向形状点链表
	static void ClearShapePointList(vector<ShapePoint*>* pList);			// 清空形状点链表

	// 连接
	static int LinkFigure(Figure* pSrcFig, Figure* pDstFig);				// 连接图形
	static int LinkShapePoint(ShapePoint* pSrcPnt, ShapePoint* pDstPnt);	// 连接形状点
	static int LinkTail2Head(Figure* pSrcFig, Figure* pDstFig);				// 图形尾连接图形头
	static BOOL LinkShapePoint_Judge(ShapePoint* pSrcPnt, ShapePoint* pDstPnt, int* pStyle);	// 连接形状点判断


public:
	// 轮廓线链表
	static void MoveOutlineList(vector<FPoint*>* pList, float rel_x, float rel_y);	// 移动轮廓点链表
	static void ClearOutlineList(vector<FPoint*>* pList);					// 清空轮廓线链表

public:
	// 数学计算

	// 获取极坐标系中的角度，弧度表示
	static float GetRadianFromXY(float dx, float dy);	

	// 指定点到直线的距离
	static float GetDistanceFromPoint2Line(float start_x, float start_y, float end_x, float end_y, float x, float y);	

	// 求两向量夹角
	static float CalVectorAngle(float dx1, float dy1, float dx2, float dy2);


	// 平移一条线段
	static BOOL ParallelSegment(float start_x, float start_y, float end_x, float end_y, int dir, float dist,
								float & new_start_x, float & new_start_y, float & new_end_x, float & new_end_y);

	// 逆时针旋转
	static void CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc);

	// 顺时针旋转
	static void ClockwiseRotate(float x0,float y0,float & x,float & y,float arc);

	// 向量规范化
	static void NormalVector(float x0,float y0,float & x,float & y);

	// 计算圆弧上的中点
	static BOOL CalArcMidPoint(float start_x, float start_y, float end_x, float end_y, float center_x, float center_y, float r, float &mid_x, float &mid_y);

	// 两点取圆心
	static void GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y);

	// 求线段相交
	static BOOL CalSegmenIntersect(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y);

	// 求线段相交
	static BOOL CalSegmenIntersect2(float start_x1, float start_y1, float end_x1, float end_y1,float start_x2, float start_y2, float end_x2, float end_y2, float &inter_x, float &inter_y);

	// 求直线相交
	 static BOOL CalLineIntersect(float start_x1, float start_y1, float end_x1, float end_y1, float start_x2, float start_y2, float end_x2, float end_y2, float& inter_x, float& inter_y);
};


#endif
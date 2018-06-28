
#ifndef HG3D_GLOBALFUNCTION_H
#define HG3D_GLOBALFUNCTION_H

#include "Export.h"

#include <tchar.h>
//#include <iostream>
//#include <algorithm>
//#include <osg/io_utils>
//#include <osg/Notify>
//#include <osg/Array>
//#include <osg/Matrix>
//#include <osg/Uniform>
//#include <osg/Plane>
//#include <osg/BoundingBox>
//#include <osgViewer/View>

namespace hg3d
{
	//extern HG3D_EXPORT std::wstring multibyteToWide(const std::string& multibyte);

	//extern HG3D_EXPORT std::string getModuleFileName();

	//extern HG3D_EXPORT std::string getModuleFilePath();

	/** 获取绝对路径。*/
	extern HG3D_EXPORT std::string getFullFileName(const std::string& fileName);

	///** 获取相对路径。*/
	//extern HG3D_EXPORT std::string getRelativelyFileName(const std::string& fileName);

	//extern HG3D_EXPORT std::string getDataPath();
	//extern HG3D_EXPORT void setDataPath(const std::string& dataPath);

 //   extern HG3D_EXPORT std::string getFilePathFromXml(const std::string& pathName);
 //   extern HG3D_EXPORT void setFilePathFromXml(const std::string& pathName, const std::string& filePath);

 //   extern HG3D_EXPORT std::string getFileName(const std::string& filePath);
 //   extern HG3D_EXPORT std::string getFileNameWithoutExt(const std::string& filePath);
 //   extern HG3D_EXPORT std::string getFileExtension(const std::string& filePath);
 //   extern HG3D_EXPORT std::string getFileParentPath(const std::string& filePath);

 //   extern HG3D_EXPORT std::string getPersonalFolderPath(void);
 //   extern HG3D_EXPORT std::string getAppDataFolderPath(const TCHAR* appName=nullptr);
 //   extern HG3D_EXPORT std::string getTempFolderPath(void);
 //   extern HG3D_EXPORT std::string getUniqueTempFile(const TCHAR* postfix=nullptr);

 //   extern HG3D_EXPORT std::string combinePath(const std::string& first, const std::string& second);

	///** Calculate the normal of a plane made by vector l1 & l2.
	//* \param l1 The first vector.
	//* \param l2 The second vector in counter-clockwise, or the normal may need to flip.
	//* \param ok FALSE if meets 0-vector or all vectors are parallel.
	//* \return The normal of plane.
	//*/
	//extern HG3D_EXPORT osg::Vec3 calcNormal( const osg::Vec3 l1, const osg::Vec3 l2, bool* ok=0 );

	///** Calculate the normal of a plane made by points p1, p2 & p3. Provided for convenience. */
	//extern HG3D_EXPORT osg::Vec3 calcNormal( const osg::Vec3 p1, const osg::Vec3 p2, const osg::Vec3 p3, bool* ok=0 );

	///** 计算两个向量之间的夹角，值在0到pi之间。*/
	//extern HG3D_EXPORT float calcIncludedAngle(const osg::Vec3 p1, const osg::Vec3 p2);

	///** 计算v1到v2的夹角，值在0到2pi之间。*/
	//extern HG3D_EXPORT float calcIncludedAngle1( const osg::Vec3 v1, const osg::Vec3 v2 );

	///** 顶点偏移。*/
	//extern HG3D_EXPORT void offsetVertices(osg::Vec3Array& originVertices, osg::Vec3Array& newVertices, bool isToLeft, float offset);

	//extern HG3D_EXPORT osg::Camera* createPerspectiveCapCamera(osgViewer::View *view);

	//extern HG3D_EXPORT osg::Camera* createOrthoCapCamera(osgViewer::View *view);

	//extern HG3D_EXPORT bool onLine(const osg::Vec3& lineStart, const osg::Vec3& lineEnd, const osg::Vec3& pt);
	//extern HG3D_EXPORT bool onLines(osg::Vec3Array* linesVertices, const osg::Vec3& pt);

	//extern HG3D_EXPORT osg::Vec3 centerPoint(const osg::Vec3& p1, const osg::Vec3& p2);

	////创建二维HUD相机
	//extern HG3D_EXPORT osg::Camera* createHUD();

	///** 获取整型值，四舍五进。*/
	//extern HG3D_EXPORT int getIntValue(float val);
	//extern HG3D_EXPORT int getIntValue(double val);
	//extern HG3D_EXPORT void setIntValue(osg::Vec3& val);
	///** 设置精度。*/
	//extern HG3D_EXPORT std::string get2DotPrecision(const std::string& str);

	///** Print debug information on the console. */
	//#define PRINT_VEC2(log, v) \
	//	std::cout << log << ": " << v.x() << ", " << v.y() << std::endl;
	//#define PRINT_VEC3(log, v) \
	//	std::cout << log << ": " << v.x() << ", " << v.y() << ", " << v.z() << std::endl;

	///** return TRUE if equivalent, meaning that the difference between 2 points is less than an epsilon value.*/
	//inline bool equivalent( osg::Vec3 lhs, osg::Vec3 rhs=osg::Vec3(0.0f,0.0f,0.0f), double epsilon=1e-6 )
	//{
	//	osg::Vec3 delta = rhs-lhs;
	//	return osg::equivalent((double)delta.x(), (double)0.0f, epsilon)
	//		&& osg::equivalent((double)delta.y(), (double)0.0f, epsilon)
	//		&& osg::equivalent((double)delta.z(), (double)0.0f, epsilon);
	//}

 //   /** return TRUE if equivalent, meaning that the difference between 2 quaternion is less than an epsilon value.*/
	//inline bool equivalent( osg::Quat lhs, osg::Quat rhs=osg::Quat(0.0f,0.0f,0.0f, 0.0f), double epsilon=1e-6 )
	//{
	//	osg::Quat delta = rhs-lhs;
	//	return osg::equivalent((double)delta[0], (double)0.0f, epsilon)
	//		&& osg::equivalent((double)delta[1], (double)0.0f, epsilon)
	//		&& osg::equivalent((double)delta[2], (double)0.0f, epsilon)
 //           && osg::equivalent((double)delta[3], (double)0.0f, epsilon);
	//}

	///** Calculate the projection of a vector on another vector.
	// * This function may also be useful to calculate the length of a point projected on a line-segment.
	// * The vertical vector is v - v'.
	// * \param v The vector to be projected.
	// * \param target The vector to be projected on.
	// * \return The projection vector.
	// */
	//extern HG3D_EXPORT osg::Vec3 calcProjection( const osg::Vec3 v, const osg::Vec3 target );

	///** Calculate the projection of a vector on a plane.
	// * This function may also be useful to calculate the distance of v and target.
	// * The distance is |v - v'|.
	// * \param v The vector to be projected.
	// * \param target The plane to be projected on.
	// * \return The projection vector.
	// */
	//extern HG3D_EXPORT osg::Vec3 calcProjection( const osg::Vec3 v, const osg::Plane target );

	///** Calculate angle between 2 vectors.
	// * \param v1 The first vector.
	// * \param v2 The second vector.
	// * \return The angle in radian (0 - PI).
	// */
	//extern HG3D_EXPORT double calcAngle( const osg::Vec3 v1, const osg::Vec3 v2 );

	///** Calculate angle between a vector and a plane.
	// * \param v The vector.
	// * \param p The plane.
	// * \return The angle in radian (-PI_2 - PI_2).
	// */
	//extern HG3D_EXPORT double calcAngle( const osg::Vec3 v, const osg::Plane p );

	///** Calculate angle between 2 planes.
	// * \param p1 The first plane.
	// * \param p2 The second plane.
	// * \return The angle in radian (0 - PI).
	// */
	//extern HG3D_EXPORT double calcAngle( const osg::Plane p1, const osg::Plane p2 );

	///** Create a plane from 3 different points.
	//* \param v1 The first point on the plane.
	//* \param v2 The second point on the plane.
	//* \param v3 The third point on the plane.
	//* \param ok FALSE if given points cannot form a plane.
	//* \return The plane.
	//*/
	//extern HG3D_EXPORT osg::Plane calcPlane( const osg::Vec3 p1, const osg::Vec3 p2, const osg::Vec3 p3, bool* ok=0 );

	///** Build a new coordinate system using given 2 or 3 basis axises and the origin point.
	// * While transform vectors from the standard coordinate system to a customized one, a transition matrix C is needed.
	// * Use v' = vC to get what vector v is in the new coordinate system (The affine coordinates).
	// * Every 3D system has 3 linear-independence vectors as its basis. User may specify all of them or leave one as 0-vector.
	// * The function will normalize and calculate all the vectors and return the transition matrix.
	// * \param orig The origin point of the new system.
	// * \param newX New basis vector x.
	// * \param newY New basis vector y.
	// * \param newZ New basis vector z.
	// * \return The transition matrix.
	// */
	//extern HG3D_EXPORT osg::Matrix coordSystemMatrix( const osg::Vec3 orig, osg::Vec3 newX, osg::Vec3 newY, osg::Vec3 newZ );

	///** Build the rotate matrix of ANY axis and a specified angle.
	// * Use v' = vC to get the rotated vector.
	// * \param axis The rotate axis.
	// * \param angle The rotate angle in radian format.
	// * \return The rotate matrix.
	// */
	//extern HG3D_EXPORT osg::Matrix rotateMatrix( osg::Vec3 axis, const double angle );

	///** Check if the orientation from v1 to v2 is clockwise(CW) or counter-clockwise(CCW).
	// * User should specify a ref-vector to project vectors to a plane, because orientations depend on eyesights.
	// * This is useful to determine vertices order of polygons.
	// * The return value is also 2 times of area of the projected triangle.
	// * \param v1 From this vector,
	// * \param v2 To this vector.
	// * \param ref Ref-vector, a vector point to user's eyes. A Z+ reference projects vectors to XY plane.
	// * \return positive if CW, negative if CCW, and 0 if not determinable.
	// */
	//extern HG3D_EXPORT double checkOrientation( const osg::Vec3 v1, const osg::Vec3 v2, const osg::Vec3 ref=osg::Vec3(0.0f,0.0f,1.0f) );

	///** Calculate the determinant of a 2x2 matrix.
	//* \param m The matrix.
	//* \return the DET value.
	//*/
	//extern HG3D_EXPORT double determinant( osg::Matrix2 m );

	///** Calculate the determinant of a 3x3 matrix.
	// * \param m The matrix.
	// * \return the DET value.
	// */
	//extern HG3D_EXPORT double determinant( osg::Matrix3 m );

	///** Calculate the factorial of an integer number.
	// * \param n The integer number.
	// * \param warnLargeValue if set to TRUE, the function will stop running and report when n>32.
	// * \return The result (n!).
	// */
	//extern HG3D_EXPORT double factorial( const int n, bool warnLargeValue=true );

	///** Calculate the linear interpolation of two points.
	// * \param a The first point.
	// * \param b The second point.
	// * \param u The interpolation value, always in [0, 1].
	// * \return The interpolation point.
	// */
	//template<typename T>
	//inline T lerp( const T& a, const T& b, double u ) { return a*(1.0f-u)+b*u; }

	///** Use to compare two vectors in a std::find_if function. */
	//struct LessPtr
	//{
	//	inline bool operator() ( const osg::Vec3* lhs, const osg::Vec3* rhs ) const
	//	{
	//		return *lhs < *rhs;
	//	}
	//};

	///** 添加点到顶点数组，如果改点在数组中有相同的点则不添加。*/
	//extern HG3D_EXPORT bool addPointToVertices(const osg::Vec3& pt, osg::Vec3Array* vertices);

	//extern HG3D_EXPORT void cleanUpVertices(osg::Vec3Array* vertices);

	//extern HG3D_EXPORT osg::Geode* createLines(osg::Vec3Array* vertices, bool isClose=true);
	//extern HG3D_EXPORT osg::Geode* createLine( const osg::Vec3& p1, const osg::Vec3& p2 );
	//extern HG3D_EXPORT osg::Geode* createPoints(osg::Vec3Array* vertices, float size);
	//extern HG3D_EXPORT osg::Geode* createPointText(osg::Vec3Array* vertices, float size);

	//extern HG3D_EXPORT void getModelSize( osg::Node* node, double& width, double& depth, double& height );



	//extern HG3D_EXPORT osg::Node* readNodeFile(const std::string& fileName);
	//extern HG3D_EXPORT osg::Node* readNodeFile(const std::string& fileName, double size_x, double size_y, double size_z);
 //   extern HG3D_EXPORT osg::Node* readDoorNode(const std::string& fileName);
	//extern HG3D_EXPORT osg::Image* readImageFile(const std::string& fileName);
 //   extern HG3D_EXPORT osg::Image* readImageFile(const std::string& fileName, float physicalWidth, float physicalHeight);
	//extern HG3D_EXPORT osg::Texture2D* createTexture2D(const std::string& fileName, bool bClamp = false);
 //   extern HG3D_EXPORT osg::Texture2D* createTexture2D(const std::string& fileName, float physicalWidth, float physicalHeight, bool bClamp = false);
 //   //extern HG3D_EXPORT bool getImagePhysicalSize(const std::string& fileName, int& width, int& height);


	//extern HG3D_EXPORT std::vector<std::string> getDirectoryContents(const std::string& filePath);


	//extern HG3D_EXPORT void mirror_h(osg::Node* node);
	//extern HG3D_EXPORT void mirror_v(osg::Node* node);

	//extern HG3D_EXPORT void deleteNode(osg::Node* node);


	///************************************************************************/
	///*  纹理处理函数
	///************************************************************************/
	///** Map a point in an box to another, scaling to fit the new one. */
	//extern HG3D_EXPORT osg::Vec3 mapTo( const osg::Vec3 p, osg::BoundingBox originRect, osg::BoundingBox newRect );

	///** Map a point in an plane to another plane, scaling and switching to fit the new one. */
	//extern HG3D_EXPORT osg::Vec2 mapTo2D( const osg::Vec3 p, osg::BoundingBox originRect, osg::BoundingBox newRect );

	///** Calculate the bounding rectangle and center point of a points group.
	//* \param ptr Beginning of a set of 3D points.
	//* \param size Number of points. Maybe dangerous if user inputs incorrect number here.
	//* \param center Returns center of the points.
	//* \param bound Returns bounding box of the points.
	//* \return FALSE if the points group is invalid.
	//*/
	//extern HG3D_EXPORT bool calcBoundAndCenter( osg::Vec3* ptr, unsigned int size, osg::Vec3* center=0, osg::BoundingBox* bound=0 );

	///** Calculate the bounding rectangle and center point of a points group. Provided for convenience. */
	//extern HG3D_EXPORT bool calcBoundAndCenter( osg::Vec3Array* pts, osg::Vec3* center=0, osg::BoundingBox* bound=0 );

	///** 设置多边形纹理坐标。*/
	//extern HG3D_EXPORT void setPolygonTexCoordArray(osg::Geometry* geom);

	///** 设置多边形平铺贴图。*/
	//extern HG3D_EXPORT void setPolygonTexCoordArray(osg::Geometry* geom, float tcX, float tcY);
	//extern HG3D_EXPORT void setWallTexCoordArray(osg::Geometry* geom, float tcX, float tcY);
	//extern HG3D_EXPORT void setWallTexCoordArray1(osg::Geometry* geom, float tcX, float tcY,float tex_angle);//20141210djp

	//extern HG3D_EXPORT bool checkFileExtension(const std::string& file, const std::string& ext);

	//extern HG3D_EXPORT void calcVerticesSize( osg::Vec3Array* vertices, double& size_x, double& size_y );

	//extern HG3D_EXPORT osg::Vec3Array* getDxfPolylineVertices(const std::string& dxfFile);

	//extern HG3D_EXPORT std::string toString(osg::Matrix m);
	//extern HG3D_EXPORT std::string toString(osg::Vec3 v);
	//extern HG3D_EXPORT std::string toString(double d);
	//extern HG3D_EXPORT std::string toString(float f);
	//extern HG3D_EXPORT std::string toString(int i);

	//extern HG3D_EXPORT bool setGeometryMaterial(const std::string& fileName, osg::Geometry* geom);
	//extern HG3D_EXPORT osg::Vec4 string2Vec4(const char* buf);

}


//#include <tchar.h>
//
//extern "C" {
//    // Export file from doo file
//    HG3D_EXPORT bool DumpFile(const TCHAR* pszSrcDb, const TCHAR* pszDestFile);
//}



#endif
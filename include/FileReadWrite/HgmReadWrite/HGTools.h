#pragma  once

#include <string>

// 判断是否在测试环境下
#ifdef DEBUG 

#pragma comment(lib, "../../lib/FileCompressandUnCompressd.lib")
#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/FileCompressandUnCompress.lib")
#pragma  message(".......................................当前为release版本")

#endif


class HGTools
{
public:
	//创建指定目录
	static void createDir(CString path);
	//删除这个目录下的文件
	static void deleteFile(CString path);
	//删除这个目录以及下属所有文件
	static bool deletePath(CString path);
	//删除这个目录下属所有文件(不包括子目录)
	static void deletePathFiles(CString path);
	//判断这个路径是否是一个文件
	static bool isFileExists(CString path);
	//判断这个路径是否是一个目录
	static bool isDir(CString path);

	//获取所指路径基于exe路径的相对路径
	static CString getRelativePath(CString path);
	//获取当前运行EXE所在路径
	static CString getCurExeFullPath();
	//获取当前运行EXE所在路径的目录
	static CString getCurExeContainPath();
	//获取文件所在路径的目录
	static CString getContainPath(CString path);
	//获取文件名称带后缀名
	static CString getFileNameWithSuffix(CString path);
	//获取文件名称不带后缀名
	static CString getFileName(CString path);
	//获得程序当前版本号
	static CString getVersion();
	//从hgx文件目录生成xml文件目录
	static CString getXmlPathFromHgxPath(CString path);
	//处理绝对路径中".",".."等路径转换，变成直接是存地址。
	static CString getRealPathFromReleativePath(CString path);
	//处理绝对路径中".",".."等路径转换，变成直接是存地址。
	//static CString getReleativePathFromRealPath(CString path);



	//加密指定文件到指定位置
	static void encryptFile(CString src,CString dst);
	//解密指定文件到指定位置
	static void decryptFile(CString src,CString dst);

	//转换为Char*
	static char* Convert2FillChar(CString src,char* dst);
	//转换为Char*
	static const char* Convert2Char(CString src);
	//转换为std::string
	static std::string Convert2StdString(int src);
	//转换为std::string
	static std::string Convert2StdString(float src);
	//转换为std::string
	static std::string Convert2StdString(CString src);
	//转换为std::string
	static std::string Convert2StdString(char* src);
	//转换为CString
	static CString Convert2CString(char* src);
	//转换为CString
	static CString Convert2CString(std::string src);



	//获得最小适配缩放比例
	static float getMinScale(CRect& putterRect, CRect& containerRect);

	//获得最小适配缩放比例
	static float getMinScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h);

	//获得最大适配缩放比例
	static float HGTools::getMaxScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h);

	//将字符串按给定的符号进行拆分
	static int SplitCString(CString strIn, CStringArray& strAryRe, char division);

	//将字符串按给定的符号进行拆分
	static int SplitCString(CString &strIn, CStringArray& strAryRe, LPCTSTR str);

	//获得相反数
	static std::string GetNagativeValue( std::string value );

	//获得文件的最后保存时间
	static long long GetFileLastTime(const CString filePath);
};

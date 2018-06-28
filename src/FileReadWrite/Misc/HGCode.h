#pragma  once
#include <string>
class AFX_EXT_CLASS HGCode
{
public:
	// 把UTF-8转换成Unicode
	static void UTF8_To_Unicode(wchar_t* pOut,char *pText);  
	//UTF-8 转为 GB2312
	static void UTF_8To_GB2312(std::string &pOut, char *pText, int pLen);

	//Unicode 转换成UTF-8
	static void Unicode_To_UTF8(char* pOut,wchar_t* pText);  
	//把Unicode 转换成 GB2312
	static void Unicode_To_GB2312(char* pOut,wchar_t uData);  

	//GB2312 转换成　Unicode
	static void Gb2312_To_Unicode(wchar_t* pOut,char *gbBuffer);
	//GB2312 转为 UTF-8
	static void GB2312_To_UTF8(std::string& pOut,char *pText, int pLen);

	static std::wstring UTF8_To_ws( const std::string& src );
	static std::string HGCode::UTF8_To_s( const std::string& src );


private:
	// 把UTF-8转换成Unicode
	static void one_UTF8_To_Unicode(wchar_t* pOut,char *pText);  
	//UTF-8 转为 GB2312
	static void one_UTF_8To_GB2312(std::string &pOut, char *pText, int pLen);

	//Unicode 转换成UTF-8
	static void one_Unicode_To_UTF8(char* pOut,wchar_t* pText);  
	//把Unicode 转换成 GB2312
	static void one_Unicode_To_GB2312(char* pOut,wchar_t uData);  

	//GB2312 转换成　Unicode
	static void one_Gb2312_To_Unicode(wchar_t* pOut,char *gbBuffer);
	//GB2312 转为 UTF-8
	static void one_GB2312_To_UTF8(std::string& pOut,char *pText, int pLen);

private:
	static char* szResult;
	static wchar_t* wszResult;

public:
	// wchar 转 char
	static const char* wchar_t_Unicode_To_GB2312(wchar_t* str);
	// char 转 wchar
	static const wchar_t* char_Gb2312_To_Unicode(const char* szTestNew);
	// 方便使用
	static const char* convert(CString& str)
	{
		USES_CONVERSION;
		return wchar_t_Unicode_To_GB2312(A2W(str.GetBuffer()));
	}
	// 方便使用
	static const char* convert(wchar_t* wszTestNew)
	{
		return wchar_t_Unicode_To_GB2312(wszTestNew);
	}
	// 方便使用
	static const wchar_t* convert(const char* szTestNew)
	{
		return char_Gb2312_To_Unicode(szTestNew);
	}
	// 方便使用
	static const wchar_t* convert(std::string& str)
	{
		return char_Gb2312_To_Unicode(str.c_str());
	}


public:
	static void CString_Unicode_To_GB2312(CString& cstr)
	{
		std::string str;
		one_GB2312_To_UTF8(str,cstr.GetBuffer(),cstr.GetLength());
		cstr = str.c_str();
	}
	static void CString_GB2312_To_Unicode(CString& cstr)
	{
		std::string str;
		one_GB2312_To_UTF8(str,cstr.GetBuffer(),cstr.GetLength());
		cstr = str.c_str();
	}



public:
	static std::string UrlGB2312(char * str);                           //urlgb2312编码
	static std::string UrlUTF8(char * str);                             //urlutf8 编码
	static std::string UrlUTF8Decode(std::string str);                  //urlutf8解码
	static std::string UrlGB2312Decode(std::string str);                //urlgb2312解码
private:
	static char StrToBin(char *str);
	static char HGCode:: CharToInt(char ch);


};
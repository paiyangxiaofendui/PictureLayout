#include "stdafx.h"
#include "HGTools.h"


#include "../../../include/FileCompressandUnCompress/FileCompressandUnCompress.h"

//创建指定目录
void HGTools::createDir(CString path)
{
	if (!HGTools::isDir(path))
	{
		path = "\""+ path +"\"";
		char temp[512] = {0};
		temp[0] = 'm';
		temp[1] = 'd';
		temp[2] = ' ';
		HGTools::Convert2FillChar(path,temp+3);
		system(temp);
	}
}

//判断这个路径是否是一个目录
bool HGTools::isDir(CString path)
{
	return PathIsDirectory(path);
}




//删除这个目录下的文件
void HGTools::deleteFile(CString path)
{
	DeleteFile(path);
}


//删除这个目录以及下属所有文件
bool HGTools::deletePath(CString path)
{
	char DirName[MAX_PATH];
	Convert2FillChar(path,DirName);

	char szCurPath[MAX_PATH];       //用于定义搜索格式  
    _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName); //匹配格式为*.*,即该目录下的所有文件  
    WIN32_FIND_DATAA FindFileData;        
    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  
    HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);  
    BOOL IsFinded = TRUE;  
    while(IsFinded)  
    {  
        IsFinded = FindNextFileA(hFile, &FindFileData); //递归搜索其他的文件  
        if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //如果不是"." ".."目录  
        {  
            std::string strFileName = "";  
            strFileName = strFileName + DirName + "//" + FindFileData.cFileName;  
            std::string strTemp;  
            strTemp = strFileName;  
            if( isDir(strFileName.c_str()) ) //如果是目录，则递归地调用  
            {     
                printf("目录为:%s/n", strFileName.c_str());  
                deletePath(strTemp.c_str());  
            }  
            else  
            {  
                DeleteFileA(strTemp.c_str());  
            }  
        }  
    }  
    FindClose(hFile);   
	return TRUE;  
}

//删除这个目录下属所有文件(不包括子目录)
void HGTools::deletePathFiles(CString path)
{
	if (HGTools::isDir(path))
	{
		path = "\""+ path +"\\*.*\" /Q";
		char temp[512] = {0};
		temp[0] = 'd';
		temp[1] = 'e';
		temp[2] = 'l';
		temp[3] = ' ';
		HGTools::Convert2FillChar(path,temp+4);
		system(temp);

	}
}


//判断这个路径是否是一个文件
bool HGTools::isFileExists(CString path)
{
	return PathFileExists(path);
}



//获取当前运行EXE所在路径附加相对路径
CString HGTools::getRelativePath(CString path)
{
	return getCurExeContainPath() + "\\" + path ;
}
//获取当前运行EXE所在路径
CString HGTools::getCurExeFullPath()
{
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;

	return file_path;
}

//获取当前运行EXE所在路径的目录
CString HGTools::getCurExeContainPath()
{
	CString file_path = getCurExeFullPath();
	file_path = getContainPath(file_path);

	return file_path;
}

//获取文件所在路径的目录
CString HGTools::getContainPath(CString path)
{
	CString file_path = path;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	return file_path;
}



//获取文件名称带后缀名
CString HGTools::getFileNameWithSuffix(CString path)
{
	CString file_path = path;
	file_path = file_path.Mid(file_path.ReverseFind('\\')+1);
	return file_path;
}
//获取文件名称不带后缀名
CString HGTools::getFileName(CString path)
{
	CString file_path = getFileNameWithSuffix(path);
	file_path = file_path.Left(file_path.ReverseFind('.'));
	return file_path;
}
//获得程序当前版本号
CString HGTools::getVersion()
{
	return " v2.0";
}

//加密指定文件到指定位置
// void HGTools::encryptFile(CString src,CString dst)
// {
// 	char encryptPath[1000];
// 	char originPath[1000];
// 	Convert2FillChar(dst,encryptPath);
// 	Convert2FillChar(src,originPath);
// 	File_Encrypt(originPath,encryptPath);
// }
// //解密指定文件到指定位置
// void HGTools::decryptFile(CString src,CString dst)
// {
// 	char decryptPath[1000];
// 	char originPath[1000];
// 	Convert2FillChar(src,decryptPath);
// 	Convert2FillChar(dst,originPath);
// 	File_Decrypt(decryptPath,originPath);	
// 
// 
// }


//将CString 转换成 char[]数据
char* HGTools::Convert2FillChar(CString src,char* dst)
{
	std::string src_string = src.GetBuffer();
	int i = 0;
	for(;i < src_string.size();i++)
	{
		dst[i] = src_string.at(i);
	}
	dst[i] = '\0';
	return dst;
}

//转换为std::string
std::string HGTools::Convert2StdString(int src)
{
	CString temp;
	temp.Format("%d",src);
	return temp.GetBuffer();
}

//转换为std::string
std::string HGTools::Convert2StdString(float src)
{
	CString temp;
	temp.Format("%f",src);
	return temp.GetBuffer();
}



//转换为Char*
const char* HGTools::Convert2Char(CString src)
{
	std::string temp = src.GetBuffer();
	return temp.c_str();
}
//转换为std::string
std::string HGTools::Convert2StdString(CString src)
{
	return src.GetBuffer();
}

//转换为std::string
std::string HGTools::Convert2StdString(char* src)
{
	return std::string(src);
}


//转换为CString
CString HGTools::Convert2CString(char* src)
{
	CString temp;
	temp.Format("%s",src);
	return temp;
}
//转换为CString
CString HGTools::Convert2CString(std::string src)
{
	CString temp;
	temp.Format("%s",src.c_str());
	return temp;
}

//从hgx文件目录生成xml文件目录
CString HGTools::getXmlPathFromHgxPath(CString pathName)
{
	if (pathName.Right(4).Left(3) = ".hg")//匹配 ".hg*"
	{
		pathName = pathName.Left(pathName.GetLength()-4) + ".xml";
		return pathName;
	}
	return pathName;
}
//处理绝对路径中".",".."等路径转换，变成直接是存地址。
CString HGTools::getRealPathFromReleativePath(CString path)
{
	char tempPath[512] = {0};
	return PathCombine(tempPath,path,NULL);
}


//获得最小适配缩放比例
float HGTools::getMinScale(CRect& putterRect, CRect& containerRect)
{
	float scale_w = putterRect.Width()/((float)(containerRect.Width()));
	float scale_h = putterRect.Height()/((float)(containerRect.Height()));

	float scale = scale_w > scale_h ? scale_w : scale_h;

	return scale;
}

//获得最小适配缩放比例
float HGTools::getMinScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h)
{
	float scale_w = putterRect_w/containerRect_w;
	float scale_h = putterRect_h/containerRect_h;

	float scale = scale_w > scale_h ? scale_h : scale_w;

	return scale;
}

//获得最大适配缩放比例
float HGTools::getMaxScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h)
{
	float scale_w = containerRect_w/putterRect_w;
	float scale_h = containerRect_h/putterRect_h;

	float scale = scale_w < scale_h ? scale_w : scale_h;

	return scale;
}


//将字符串按给定的符号进行拆分
int HGTools::SplitCString(CString strIn, CStringArray& strAryRe, char division)
{
	strAryRe.RemoveAll();
	if (!strIn.IsEmpty())
	{
		int nCount = 0;
		int nPos = -1;
		nPos = strIn.Find(division);
		CString strTemp = strIn;
		while (nPos != -1)
		{
			CString strSubString = strTemp.Left(nPos);   
			strTemp = strTemp.Right(strTemp.GetLength() - nPos-1);   
			nPos = strTemp.Find(division); 
			nCount++;
			strAryRe.Add(strSubString);
		}

		if (nCount == strAryRe.GetSize())
		{
			CString str;
			int nSize = strIn.ReverseFind(division);
			str = strIn.Right(strIn.GetLength()-nSize-1);
			strAryRe.Add(str);
		}	
	}

	return strAryRe.GetSize();
}

//将字符串按给定的符号进行拆分
int HGTools::SplitCString(CString &strIn, CStringArray& strAryRe, LPCTSTR str)
{
	if (str == _T(""))
	{
		strAryRe.RemoveAll();
		return 0;
	}

	int nStart = 0;
	int nEnd = 0;
	CString strTmp;
	while ( (nEnd = strIn.Find(str, nStart)) != -1 )
	{
		strTmp = strIn.Mid(nStart, nEnd-nStart);
		strAryRe.Add(strTmp);
		nStart = nEnd+2;
	}
	strTmp = strIn.Mid(nStart, strIn.GetLength());
	strAryRe.Add(strTmp);

	return strAryRe.GetSize();

}

//获得相反数
std::string HGTools::GetNagativeValue( std::string value )
{
	float val = atof(value.c_str());
	val = val*-1;
	CString output;
	output.Format("%f",val);
	return output.GetBuffer();
}

//获得文件的最后保存时间
long long HGTools::GetFileLastTime(const CString filePath)
{
	WIN32_FIND_DATAA FindFileData;
	::memset(&FindFileData,0,sizeof(FindFileData));
	::FindClose(::FindFirstFileA(filePath,&FindFileData));
	FILETIME& lastWriteTime = FindFileData.ftLastWriteTime;
	return (((long long)lastWriteTime.dwHighDateTime)<<32)+lastWriteTime.dwLowDateTime;
}
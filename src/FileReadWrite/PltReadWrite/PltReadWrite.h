/*--------------------------------------------------------------------------------------------------------------------*/
//	PltReadWrite.h      -- plt文件读写类声明文件
//	
//	作者：	yuanzb
//	时间：	2017.7.21
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_PltReadWrite_H_
#define		_PltReadWrite_H_


class Panel;

class AFX_EXT_CLASS PltReadWrite
{
public:
	static bool OutputPlt(Panel* pPanel, CString strPltFilePath);	

};



#endif
/*--------------------------------------------------------------------------------------------------------------------*/
//	DxfReadWrite.h      -- dxf文件读写类声明文件
//	
//	作者：	yuanzb
//	时间：	2017.7.21
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_DxfReadWrite_H_
#define		_DxfReadWrite_H_


class Panel;

class AFX_EXT_CLASS DxfReadWrite
{
public:
	static bool OutputUpperFaceDxf(Panel* pPanel, CString strDxfFilePath);	
	static bool OutputDownerFaceDxf(Panel* pPanel, CString strDxfFilePath);

};



#endif
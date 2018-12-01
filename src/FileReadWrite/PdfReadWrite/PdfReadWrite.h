/*--------------------------------------------------------------------------------------------------------------------*/
//	PdfReadWrite.h      -- pdf文件读写类声明文件
//	
//	作者：	yuanzb
//	时间：	2017.7.21
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_PdfReadWrite_H_
#define		_PdfReadWrite_H_


using namespace std ;
class Panel;

class AFX_EXT_CLASS PdfReadWrite
{
public:
	static bool OutputPdf(Panel* pPanel, string strPdfFilePath);	

};



#endif
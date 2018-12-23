/*--------------------------------------------------------------------------------------------------------------------*/
//	PdfReadWrite.cpp       -- PdfReadWrite文件读写类实现文件
//	
//	作者：	yuanzb
//	时间：	2017.7.21
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <xstring>
#include "PdfReadWrite.h"
#include "pdflib.h"
#include "pdflib.hpp"


// 外部DataManager.dll的头文件
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
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

#pragma comment(lib, "../../lib/pdflib.lib")
using namespace pdflib;



std::wstring StringToWString(const std::string& str) 
{
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	return w_str;
}


std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)

{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;

	WCHAR *pwszDst = new WCHAR[nSize+1];
	if( NULL == pwszDst) return NULL;

	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;

	if( pwszDst[0] == 0xFEFF) // skip Oxfeff
		for(int i = 0; i < nSize; i ++) 
			pwszDst[i] = pwszDst[i+1];

	wstring wcharString(pwszDst);
	delete pwszDst;

	return wcharString;
}


std::string WStringToString(const std::wstring &wstr)
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
	{
		return "";
	}
	return str;
}


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
bool PdfReadWrite::OutputPdf(Panel* pPanel, string strPdfFilePath) 
{
	try 
	{
		int font;
		PDFlib p;
		//  This means we must check return values of load_font() etc.
		p.set_option(L"errorpolicy=return");

		wstring unicode_filepath = Ansi2WChar(strPdfFilePath.c_str(), strPdfFilePath.length());


		if (p.begin_document(unicode_filepath.c_str(),  L"") == -1) {
			wcerr << "Error: " << p.get_errmsg() << endl;

			string msg = WStringToString(p.get_errmsg());

			AfxMessageBox(msg.c_str());

			return false;
		}

		p.set_info(L"Creator", L"hello.cpp");
		p.set_info(L"Author", L"Thomas Merz");
		p.set_info(L"Title", L"Hello, world (C++)!");

		// 大板长宽
		p.begin_page_ext(pPanel->m_OrgLen*72/25.4, pPanel->m_OrgWidth*72/25.4, L"");

		// Change "host" encoding to "winansi" or whatever you need!
		font = p.load_font(L"Helvetica-Bold", L"host", L"");
		//font = p.load_font(L"arial", L"host", L"");
		//font = p.load_font(L"arial", L"winansi", L"");
		//font = p.load_font(L"Arial", L"host", L"");
		//font = p.load_font(L"Arial", L"winansi", L"");
		//font = p.load_font(L"Times New Roman", L"winansi", L"");
		//font = p.load_font(L"Times New Roman", L"host", L"");
		//font = p.load_font(L"宋体 常规", L"winansi", L"");
		//font = p.load_font(L"宋体 常规", L"host", L"");


		if (font == -1) {
			wcerr << L"Error: " << p.get_errmsg() << endl;

			string msg = WStringToString(p.get_errmsg());

			AfxMessageBox(msg.c_str());
			return false;
		}
		p.setfont(font, 24);
		p.set_text_pos(50, 50);
		//p.show(L"Hello, world!");
		//p.continue_text(L"pdf");





		//const wstring searchpath = L"D:/搜狗高速下载/PDFlib-9.1.2p1-MSWin32-C-C%2B%2B+Cracked/PDFlib-9.1.2p1-MSWin32-C-C++ Cracked/bind/data";
		//	const wstring imagefile = L"nesrin.jpg";
		const wstring searchpath = L"";
		
		wostringstream optlist;




		int image_index = 0;
		map<const wstring, int> ImageFileMap;

		// 依次画图
		vector<Component*> CpnList;
		pPanel->GetAllNeededComponent(CpnList);

		int nCpnCount = CpnList.size();
		for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
		{
			Component* pCpn = CpnList.at(i_cpn);

			string file_path = pCpn->m_strCabinetName;
			wstring unicode_file_path = Ansi2WChar(file_path.c_str(), file_path.length());
			float x = pCpn->m_x*72/25.4;
			float y = pCpn->m_y*72/25.4;
			wostringstream op_para;

			if (pCpn->m_nRotatedAngle == 0)
			{
				op_para.str(L"");

			}
			else
			{
				op_para.str(L"orientate=west");


			}

			// Set the search path for fonts and PDF files
			optlist.str(L"");
			optlist << L"searchpath={{" << searchpath << L"}}";
			p.set_option(optlist.str());



			const wstring imagefile = unicode_file_path;
			int image;
			
			map<const wstring, int>::iterator it;

			it = ImageFileMap.find(imagefile);

			if (it == ImageFileMap.end())
			{
				// 不存在，插入
				ImageFileMap[imagefile] = image_index;
				image_index++;

				image = p.load_image(L"auto", imagefile, L"");

			}
			else
			{
				// 存在 直接取 索引
				image = it->second;

 			}
			



			if (image == -1) {
				wcerr << L"Error: " << p.get_errmsg() << endl;

				string msg = WStringToString(p.get_errmsg());

				AfxMessageBox(msg.c_str());

				return false;
			}

			optlist.str(L"");
			optlist.str(L"fontname={LinLibertine_R} encoding=unicode embedding fontsize=24");



			p.fit_image(image, x, y, op_para.str());

// 			p.fit_image(image, 2000, 2000,
// 				L"orientate=west");
// 
// 			p.fit_image(image, 2000, 4000,
// 				L"orientate=east");
// 
// 			p.fit_image(image, 2000, 6000,
// 				L"orientate=south");



		}







		p.end_page_ext(L"");

		p.end_document(L"");
	}
	catch (PDFlib::Exception &ex) {
		wcerr << L"PDFlib exception occurred in hello sample: " << endl
			<< L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
			<< L": " << ex.get_errmsg() << endl;

		string msg = WStringToString(ex.get_errmsg());

		AfxMessageBox(msg.c_str());

		return false;
	}





	return true;
}


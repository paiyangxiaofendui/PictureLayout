// PDFTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "../../include/FileReadWrite/PdfReadWrite/pdflib.h"
#include "../../include/FileReadWrite/PdfReadWrite/pdflib.hpp"
#pragma comment(lib, "pdflib.lib")


using namespace std;
using namespace pdflib;

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		int font;
		PDFlib p;
		//  This means we must check return values of load_font() etc.
		p.set_option(L"errorpolicy=return");


		if (p.begin_document(L"hello.pdf", L"") == -1) {
			wcerr << "Error: " << p.get_errmsg() << endl;
			return 2;
		}

		p.set_info(L"Creator", L"hello.cpp");
		p.set_info(L"Author", L"Thomas Merz");
		p.set_info(L"Title", L"Hello, world (C++)!");

		p.begin_page_ext(10000, 10000, L"");

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
			return(2);
		}
		p.setfont(font, 24);
		p.set_text_pos(50, 50);
		//p.show(L"Hello, world!");
		//p.continue_text(L"pdf");





		//const wstring searchpath = L"D:/搜狗高速下载/PDFlib-9.1.2p1-MSWin32-C-C%2B%2B+Cracked/PDFlib-9.1.2p1-MSWin32-C-C++ Cracked/bind/data";
		//	const wstring imagefile = L"nesrin.jpg";
		const wstring searchpath = L"";
		const wstring imagefile = L"F:/PictureLayout/测试目录/新建文件夹/3.tif";
		int image;

		wostringstream optlist;


		// Set the search path for fonts and PDF files
		optlist.str(L"");
		optlist << L"searchpath={{" << searchpath << L"}}";
		p.set_option(optlist.str());



		image = p.load_image(L"auto", imagefile, L"");

		if (image == -1) {
			wcerr << L"Error: " << p.get_errmsg() << endl;
			return 2;
		}


		optlist.str(L"");
		optlist.str(L"fontname={LinLibertine_R} encoding=unicode embedding fontsize=24");

		//p.fit_textline(L"en: Hello!", 50, 700, optlist.str());

		p.fit_image(image, 0, 0,L" ");

		p.fit_image(image, 2000, 2000,
			L"orientate=west");

		p.fit_image(image, 2000, 4000,
			L"orientate=east");

		p.fit_image(image, 2000, 6000,
			L"orientate=south");





		p.end_page_ext(L"");

		p.end_document(L"");
	}
	catch (PDFlib::Exception &ex) {
		wcerr << L"PDFlib exception occurred in hello sample: " << endl
			<< L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
			<< L": " << ex.get_errmsg() << endl;
		return 2;
	}










	return 0;
}


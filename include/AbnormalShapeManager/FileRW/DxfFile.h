/*--------------------------------------------------------------------------------------------------------------------*/
//	DxfFile.h    -- dxf文件类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.7.8
//	备注：	读取dxf文件的图元信息
//
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef		DxfFile_H
#define		DxfFile_H

#include <vector>
using namespace std;

class AFX_EXT_CLASS Figure;

class DxfFile
{
public:
	DxfFile();
	~DxfFile();

	int Load(CString file_name);
public:

	CStdioFile m_File;					// 文件句柄
	vector<Figure*> m_FigureList;		// 读取的图元容器

public:
	static CString SectionStar;			// 段开始
	static CString SectionEnd;			// 段结束
	static CString EntitySectionStar;	// 实体段开始

	void ReadFigureData();
	void ReadEntitySection();
	void ReadPolyline();
	void ReadCircle();
	void ReadArc();
};

#endif
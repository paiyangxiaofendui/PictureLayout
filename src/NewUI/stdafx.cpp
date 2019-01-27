// stdafx.cpp : 只包括标准包含文件的源文件
// NewUI.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


GdiplusStartupInput m_gdiplusStartupInput; 
ULONG_PTR m_gdiplusToken;  

GdiplusStartupInput* getGdiplusStartupInput()
{
	return &m_gdiplusStartupInput;
}

ULONG_PTR* getgdiplusToken()
{
	return &m_gdiplusToken;
}


// stdafx.cpp : 只包括标准包含文件的源文件
// FileReadWrite.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


#if (CUR_EDITION == HG_EDITION)
TCHAR* g_szSuffix = ".hge";
#else
TCHAR* g_szSuffix = ".ywe";
#endif

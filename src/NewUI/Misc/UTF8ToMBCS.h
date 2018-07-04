
// UTF8编码转为多字节编码  
extern bool UTF8ToMB(const char* pszUTF8, int utf8Len, CString &strMB);

// 多字节编码转为Unicode编码  
extern bool MBToUnicode(const char* pmb, int len, CStringW& strDest);  
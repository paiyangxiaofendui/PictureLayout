#ifndef _Algorithm_H
#define _Algorithm_H

extern "C"  __declspec(dllexport) int LayoutOptimize(); 
extern "C"  __declspec(dllexport) char* LayoutOptimizeByStream(char* pszXml);
extern "C"  __declspec(dllexport) int Test(char* pWords); 
extern "C"  __declspec(dllexport) int TestRead(void); 


#endif
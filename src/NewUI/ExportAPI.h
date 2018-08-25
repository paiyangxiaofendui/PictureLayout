#ifndef			ExportAPI_h 
#define			ExportAPI_h




extern "C"  __declspec(dllexport) int Test(void); 
extern "C"  __declspec(dllexport) int JT_Layouts(char* src_file_path); 

#endif
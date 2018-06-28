#pragma once

#define PLAIN_FILE_OPEN_ERROR -1  
#define KEY_FILE_OPEN_ERROR -2  
#define CIPHER_FILE_OPEN_ERROR -3  
#define OK 0   
typedef char ElemType; 

////初始置换表IP  
//extern int IP_Table[64];   
////逆初始置换表IP^-1  
//extern int IP_1_Table[64];  
//
////扩充置换表E  
//extern int E_Table[48];  
//
////置换函数P  
//extern int P_Table[32]};  
//
////S盒  
//extern int S[8][4][16];  
////置换选择1  
//extern int PC_1[56];  
//
////置换选择2  
//extern int PC_2[48];  
//
////对左移次数的规定  
////模块化
//extern int MOVE_TIMES[16]; 

extern "C" __declspec(dllexport) int func_drawing0(char* src_path, char* dst_path); 
extern "C" __declspec(dllexport) int func_drawing1(char* src_path, char* dst_path); 

 

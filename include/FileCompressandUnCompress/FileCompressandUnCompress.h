#ifndef OUTPUT_HS
#define OUTPUT_HS __declspec(dllexport)    
#endif
#include<time.h>
typedef unsigned char byte;
int ByteToBit(byte ch, byte bit[8]);
int BitToByte(byte bit[8], byte *ch);
int Char8ToBit64(byte ch[8], byte bit[64]);
int Bit64ToChar8(byte bit[64], byte ch[8]);
int DES_MakeSubKeys(byte key[64], byte subKeys[16][48]);
int DES_PC1_Transform(byte key[64], byte tempbts[56]);
int DES_PC2_Transform(byte key[56], byte tempbts[48]);
int DES_ROL(byte data[56], int time);
int DES_IP_Transform(byte data[64]);
int DES_IP_1_Transform(byte data[64]);
int DES_E_Transform(byte data[48]);
int DES_P_Transform(byte data[32]);
int DES_SBOX(byte data[48]);
int DES_XOR(byte R[48], byte L[48], int count);
int DES_Swap(byte left[32], byte right[32]);
int DES_EncryptBlock(byte plainBlock[8], byte subKeys[16][48], byte cipherBlock[8]);
int DES_DecryptBlock(byte cipherBlock[8], byte subKeys[16][48], byte plainBlock[8]);
int DES_Encrypt(char *plainFile, char *keyStr, char *cipherFile);/*对文件进行加密的具体函数*/
int DES_Decrypt(char *cipherFile, char *keyStr, char *plainFile); /*对文件解密的具体函数*/
extern "C" OUTPUT_HS void File_Encrypt(char *plainFile, char *cipherFile);/*对文件进行加密的具体函数*/
extern "C" OUTPUT_HS void File_Decrypt(char *cipherFile, char *plainFile); /*对文件解密的具体函数*/
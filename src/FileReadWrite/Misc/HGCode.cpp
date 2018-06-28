#include "stdafx.h"
#include "HGCode.h"

#include <string>
#include <locale>
#include <codecvt>

void HGCode::UTF8_To_Unicode(wchar_t* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

void HGCode::Unicode_To_UTF8(char* pOut,wchar_t* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}

void HGCode::Unicode_To_GB2312(char* pOut,wchar_t uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
	return;
}    

void HGCode::Gb2312_To_Unicode(wchar_t* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return ;
}

void HGCode::GB2312_To_UTF8(std::string& pOut,char *pText, int pLen)
{
	char buf[4];
	int nLength = pLen* 3;
	char* rst = new char[nLength];

	memset(buf,0,4);
	memset(rst,0,nLength);

	int i = 0;
	int j = 0;     
	while(i < pLen)
	{
		//如果是英文直接复制就能
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;
			Gb2312_To_Unicode(&pbuffer,pText+i);

			Unicode_To_UTF8(buf,&pbuffer);

			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];    

			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';

	//返回结果
	pOut = rst;             
	delete []rst;  

	return;
}

void HGCode::UTF_8To_GB2312(std::string &pOut, char *pText, int pLen)
{
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);

	int i =0;
	int j = 0;

	while(i < pLen)
	{
		if(pText > 0)
		{
			newBuf[j++] = pText[i++];                      
		}
		else                
		{
			WCHAR Wtemp;
			UTF8_To_Unicode(&Wtemp,pText + i);

			Unicode_To_GB2312(Ctemp,Wtemp);

			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];

			i += 3;    
			j += 2;  
		}
	}
	newBuf[j] = '\0';

	pOut = newBuf;
	delete []newBuf;

	return;
} 

std::wstring HGCode::UTF8_To_ws( const std::string& src )
{
	if(src.empty()) return L"";
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring temp;
	try
	{
		temp = conv.from_bytes( src );
	}		
	catch (const std::range_error& e)
	{
		temp =  conv.from_bytes(src.substr(0, conv.converted()));
	}
	return move(temp);
}

std::string HGCode::UTF8_To_s( const std::string& src )
{
	return CString(UTF8_To_ws(src).c_str()).GetBuffer();
}








wchar_t* HGCode::wszResult = nullptr;

const char* HGCode::wchar_t_Unicode_To_GB2312(wchar_t* str)
{
	if (szResult)
	{
		delete szResult;
		szResult = NULL;
	}
	int nChar = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);    // 返回结果已包含'\0'所要占用的内存
	nChar = nChar * sizeof(char);
	//char* szResult = new char[nChar];
	szResult = new char[nChar];
	ZeroMemory(szResult, nChar);
	int i = WideCharToMultiByte(CP_ACP, 0, str, -1, szResult, nChar, NULL, NULL);
	int nszResultLen = lstrlenA(szResult);            
	int nszResultSize = sizeof(szResult);            

#if 0	// DEMO
	wchar_t wszTest[] = L"ziwuge";
	wchar_t wszTestNew[] = L"ziwuge博客园";
	int nwszTestLen = lstrlenW(wszTest);            // 6
	int nwszTestNewLen = lstrlenW(wszTestNew);        // 9
	int nwszTestSize = sizeof(wszTest);                // 14
	int nwszTestNewSize = sizeof(wszTestNew);        //    20
	int nChar = WideCharToMultiByte(CP_ACP, 0, wszTestNew, -1, NULL, 0, NULL, NULL);    // 13, 返回结果已包含'\0'所要占用的内存
	nChar = nChar * sizeof(char);                    // 13, 其实这一步可不需要,请见本文前面解释
	char* szResult = new char[nChar];
	ZeroMemory(szResult, nChar);
	int i = WideCharToMultiByte(CP_ACP, 0, wszTestNew, -1, szResult, nChar, NULL, NULL);    // 13
	int nszResultLen = lstrlenA(szResult);            // 12
	int nszResultSize = sizeof(szResult);            // 4
#endif // DEMO

	return szResult;
}

const wchar_t* HGCode::char_Gb2312_To_Unicode(const char* szTestNew)
{
	if (wszResult)
	{
		delete wszResult;
		wszResult = nullptr;
	}

	int nszTestNewLen = lstrlenA(szTestNew);        
	int nszTestNewSize = sizeof(szTestNew);           
	int nWChar = MultiByteToWideChar(CP_ACP, 0, szTestNew, -1, NULL, 0);        // 返回结果已包含'\0'所要占用的内存
	nWChar = nWChar * sizeof(wchar_t);               
	wszResult = new wchar_t[nWChar];
	ZeroMemory(wszResult, nWChar);
	int j = MultiByteToWideChar(CP_ACP, 0, szTestNew, -1, wszResult, nWChar);    
	int nwszResultLen = lstrlenW(wszResult);        
	int nwszResultSize = sizeof(wszResult);           
	return wszResult;

#if 0	//DEMO
	char szTest[] = "ziwuge";
	char szTestNew[] = "ziwuge博客园";
	int nszTestLen = lstrlenA(szTest);                // 6
	int nszTestNewLen = lstrlenA(szTestNew);        // 12
	int nszTestSize = sizeof(szTest);                // 7
	int nszTestNewSize = sizeof(szTestNew);            // 13
	int nWChar = MultiByteToWideChar(CP_ACP, 0, szTestNew, -1, NULL, 0);        // 10, 返回结果已包含'\0'所要占用的内存
	nWChar = nWChar * sizeof(wchar_t);                // 20
	wchar_t* wszResult = new wchar_t[nWChar];
	ZeroMemory(wszResult, nWChar);
	int j = MultiByteToWideChar(CP_ACP, 0, szTestNew, -1, wszResult, nWChar);    // 10
	int nwszResultLen = lstrlenW(wszResult);        // 9
	int nwszResultSize = sizeof(wszResult);            // 4
	return 0;
#endif	//DEMO
}

char* HGCode::szResult = nullptr;


void HGCode::one_UTF8_To_Unicode(wchar_t* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

void HGCode::one_Unicode_To_UTF8(char* pOut,wchar_t* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}

void HGCode::one_Unicode_To_GB2312(char* pOut,wchar_t uData)
{
	int a = WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
	return;
}    

void HGCode::one_Gb2312_To_Unicode(wchar_t* pOut,char *gbBuffer)
{
	int a = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return ;
}

void HGCode::one_GB2312_To_UTF8(std::string& pOut,char *pText, int pLen)
{
	char buf[4];
	int nLength = pLen* 3;
	char* rst = new char[nLength];

	memset(buf,0,4);
	memset(rst,0,nLength);

	int i = 0;
	int j = 0;     
	while(i < pLen)
	{
		//如果是英文直接复制就能
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;
			one_Gb2312_To_Unicode(&pbuffer,pText+i);

			one_Unicode_To_UTF8(buf,&pbuffer);

			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];    

			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';

	//返回结果
	pOut = rst;             
	delete []rst;  

	return;
}


void HGCode::one_UTF_8To_GB2312(std::string &pOut, char *pText, int pLen)
{
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);

	int i =0;
	int j = 0;

	while(i < pLen)
	{
		if(pText > 0)
		{
			newBuf[j++] = pText[i++];                      
		}
		else                
		{
			WCHAR Wtemp;
			one_UTF8_To_Unicode(&Wtemp,pText + i);

			one_Unicode_To_GB2312(Ctemp,Wtemp);

			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];

			i += 3;    
			j += 2;  
		}
	}
	newBuf[j] = '\0';

	pOut = newBuf;
	delete []newBuf;

	return;
} 



//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节 如%3D%AE%88
std::string HGCode::UrlGB2312(char * str)
{
	std::string dd;
	size_t len = strlen(str);
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)str[i]))
		{
			char tempbuff[2];
			sprintf(tempbuff,"%c",str[i]);
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)str[i]))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节 如%3D%AE%88

std::string HGCode::UrlUTF8(char * str)
{
	std::string tt;
	std::string dd;
	one_GB2312_To_UTF8(tt,str,(int)strlen(str));
	//GB2312ToUTF_8(tt,str,(int)strlen(str));

	size_t len=tt.length();
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)tt.at(i)))
		{
			char tempbuff[2]={0};
			sprintf(tempbuff,"%c",(BYTE)tt.at(i));
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)tt.at(i)))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}
//把url GB2312解码
std::string HGCode::UrlGB2312Decode(std::string str)
{
	std::string output="";
	char tmp[2];
	int i=0,idx=0,ndx,len=str.length();

	while(i<len){
		if(str[i]=='%'){
			tmp[0]=str[i+1];
			tmp[1]=str[i+2];
			output += StrToBin(tmp);
			i=i+3;
		}
		else if(str[i]=='+'){
			output+=' ';
			i++;
		}
		else{
			output+=str[i];
			i++;
		}
	}

	return output;
}
//把url utf8解码
std::string HGCode::UrlUTF8Decode(std::string str)
{
	std::string output="";

	std::string temp =UrlGB2312Decode(str);//

	one_UTF_8To_GB2312(output,(char *)temp.data(),strlen(temp.data()));
	//UTF_8ToGB2312(output,(char *)temp.data(),strlen(temp.data()));

	return output;

}
//做为解Url使用
char HGCode:: CharToInt(char ch){
	if(ch>='0' && ch<='9')return (char)(ch-'0');
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
	return -1;
}
char HGCode::StrToBin(char *str){
	char tempWord[2];
	char chn;

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0 -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}

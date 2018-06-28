#ifndef		_HgmReadWrite_h_
#define		_HgmReadWrite_h_
#pragma  once

#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#include <vector>
#include <string>
#include <map>


using namespace std;

class AFX_EXT_CLASS  HgmReadWrite
{
public:

	// 读取拆单软件导出的雕刻机对接hgm格式文件
	static bool  ReadPanelInfo(CString szFileDir, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param);
	static vector<map<string, string>> translatexml(std::string filepath, SourceFilePreProccesParam& param);

	// 读取刀库文件
	static void LoadSettings_KnifeType();
	static void SaveSettings_KnifeType();

public:
	// 给erp使用的读取拆单数据接口
	static bool  ReadPanelInfoByStream(char* pszXml, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param);
		static vector<map<string, string>> translatexmlByStream(char* pszXml, SourceFilePreProccesParam& param);





public:

	// 为了debug调试，不解密，直接读取xml
	static bool  ReadPanelInfo_Debug(CString szFileDir, vector<ComponentInputItem>& vComponentInputItem, SourceFilePreProccesParam& param);


};

#endif
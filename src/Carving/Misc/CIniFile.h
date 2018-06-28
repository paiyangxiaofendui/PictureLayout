#if !defined(AFX_INIFILE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_INIFILE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_


//----------------------------------------------CIniFile.h---------------------
class CIniFile 
{
public://default destructor
	CIniFile();
	virtual ~CIniFile();
private:
	//ini file path
	CString m_strFilePath;
	//ini file name
	CString m_strFileName;
	//ini file
	CStdioFile m_IniFile;
	//file content container
	CStringArray m_FileContainer;
public:
	//sets name of ini file to read and write from
	void SetFileName(LPCTSTR lpFileName);
	//sets path of ini file to read and write from
	void SetFilePath(LPCTSTR lpFilePath);
	//open ini file
	BOOL OpenIniFile(void);
	//open ini file for read
	BOOL OpenIniFileForRead(void);
	//open ini file for write
	BOOL OpenIniFileForWrite(void);
	//find Section in ini file
	WORD OnFindSection(LPCTSTR lpAppName);
	//find Item in ini file
	BOOL OnFindItem(LPCTSTR lpKeyName,WORD& wLine);
	//get all key vealue of section
	BOOL GetSectionAllItem(LPCTSTR lpAppName,CStringArray&strArray);
	//read value of CString from ini file
	BOOL GetItemString(LPCTSTR lpAppName,LPCTSTR lpKeyName,CString& strValue);
	//write Section to ini file
	void WriteSection(LPCTSTR lpAppName);
	//write Item and value of CString to ini file
	void WriteItemString(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpValue);
};


#endif
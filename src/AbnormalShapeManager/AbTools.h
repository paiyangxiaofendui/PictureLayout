#ifndef			_AbTools_h_ 
#define			_AbTools_h_

#include <string>

using namespace std;



class AFX_EXT_CLASS AbTools
{
public:
	AbTools();
	~AbTools();

public:
	int ShowDialog();		// ÏÔÊ¾´°¿Ú 
	CString m_strOutline;

public:
	CString GetPanelOutlineStr(void);

};

#endif
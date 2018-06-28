#ifndef			_ParallelDialog_h_ 
#define			_ParallelDialog_h_


#include "Resource.h "


// 外部类声明

class  ParallelDialog : public CDialog
{

	DECLARE_DYNAMIC(ParallelDialog)

public:
	ParallelDialog(CWnd* pParentWnd = NULL);
	~ParallelDialog();

private:
	enum {IDD = IDD_DLG_PARALLEL};

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedRadioRight();
	afx_msg void OnClickedRadioLeft();
	virtual void DoDataExchange(CDataExchange* pDX);
	float m_parallel_dist;
	int m_parallel_dir;
};

#endif


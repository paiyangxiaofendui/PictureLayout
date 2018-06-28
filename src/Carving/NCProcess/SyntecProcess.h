#pragma once
#include <vector>
#include "../Work/WorkDef.h"

using namespace std;

class Panel;

struct SyntecDataItem
{
	CString m_strNCFilePath;
	Panel* m_pPanel;
	CString m_strLabelFileName;
	CString m_strPanelImageName;
};

void SyntecSummaryOutput(vector<SyntecDataItem>& vSyntecData, UINT nCoordinateType, CString strOutputDir, LabelPastingSide eSide = LabelPastingSide::LABEL_UP_SIDE);
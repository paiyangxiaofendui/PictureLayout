#pragma once
#include "../Misc/ProgramMisc.h"

struct HomagDataItem
{
	PanelAndTheSolution m_PanelAndTheSolution;
	CString m_strPanelUpperFaceFileName;
	CString m_strPanelDownerFaceFileName_Flip;
	DirVHType m_eFlipType;
};

//void ExportGCode_Homag(const vector<PanelAndTheSolution>& vPanelAndTheSolution, DirVHType eFlipType = DirVHType::DIR_V);

void ExportGCode_Homag(const vector<HomagDataItem>& vHomagDataItem, LPCTSTR szOutputDir);


#pragma once
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include <vector>
using namespace std;

struct OutlineLine
{
	void SetValue(float fStartX = 0, float fStartY = 0, float fEndX = 0, float fEndY = 0, int nLineType = 0, float fR = 0, int nDir = 0);
	float m_fStartX;
	float m_fStartY;
	float m_fEndX;
	float m_fEndY;

	int m_nLineType;
	float m_fR;
	int m_nDir;
};

struct PreCombineComponent
{
public:
	enum BUTTON_STATE{bs_Normal=0, bs_Down, bs_Disabled, bs_Hovered};

	PreCombineComponent()
		:m_nRotatedAngleWhenCombine(0)
		,m_fX_Left(0.0)
		,m_fY_Bottom(0.0)
	{}

	BUTTON_STATE GetButtonState() { return m_nState; };
	void SetButtonState(BUTTON_STATE nState) ;

	ComponentInputItem m_Item;
	int m_nRotatedAngleWhenCombine;
	float m_fX_Left;
	float m_fY_Bottom;
	vector<vector<OutlineLine>> m_vMillingOutlineGroup;

	BUTTON_STATE m_nState;
};

typedef vector<PreCombineComponent> PreCombineItem;

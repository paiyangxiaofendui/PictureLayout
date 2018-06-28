/*--------------------------------------------------------------------------------------------------------------------*
///	Outline.cpp   --  轮廓线类实现文件
//	
//	作者：	yuanzb
//	时间：	2016.11.8
//	备注：
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Outline.h"
#include "../CommonData/CommonData.h"
#include "../Component/Component.h"
#include "../Panel/Panel.h"

// 构造
Outline::Outline()
{
	m_start_x = 0;
	m_end_x = 0;
	m_start_y = 0;
	m_end_y = 0;
	m_pParent = NULL;
}

// 析构
Outline::~Outline()
{

}




// >操作符重载,水平线y优先,长度次之
bool Outline::operator> (const Outline& ln) const
{
	if (this->m_start_y < ln.m_start_y)			// y优先
	{
		return true;
	}
	else if (this->m_start_y == ln.m_start_y)		// x次之
	{
		float src_len = this->GetLength();
		float dst_len = ln.GetLength();

		if (src_len > dst_len)
		{
			return true;
		}
	}

	return false;
}



// <操作符重载
bool Outline::operator< (const Outline& ln) const
{
	if (this->m_start_y < ln.m_start_y)			// y优先
	{
		return true;
	}
	else if (this->m_start_y == ln.m_start_y)		// x次之
	{
		float src_len = this->GetLength();
		float dst_len = ln.GetLength();

		if (src_len < dst_len)
		{
			return true;
		}
	}

	return false;
}

bool Outline::operator== (const Outline& ln) const		// ==操作符重载
{
	if (this->m_start_x == ln.m_start_x 
		&& this->m_end_x == ln.m_end_x
		&& this->m_start_y == ln.m_start_y 
		&& this->m_end_y == ln.m_end_y)			// xy都相等
	{
		return true;
	}

	return false;
}

// 获取长度
float Outline::GetLength(void) const
{
	return abs(m_end_x - m_start_x);
}

// 获取宽度
float Outline::GetWidth(void) const
{
	return abs(m_end_y - m_start_y);
}

// 能否容纳
bool Outline::Containable(Component* pCpn)
{
	float cur_len = GetLength();
	float cur_width = GetWidth();

	if (pCpn != NULL)
	{
		if (cur_len >= pCpn->m_RealLength && cur_width >= pCpn->m_RealWidth)
		{
			return true;
		}
	}

	return false;
}

// 轮廓线是否合法
bool Outline::IsLegal(void)
{
	if(m_start_x >= m_end_x)
		return false;

	if (m_start_y >= m_end_y)
		return false;

	return true;
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		设置轮廓线
//	
//	param：
//		float start_x	--	轮廓线起点x
//		float end_x		--	轮廓线终点x
//		float start_y	--	轮廓线起点y
//		float end_y		--	轮廓线终点y
//		Panel* pPanel	--	大板指针，属于哪块大板
//
//	return:
//		void
/*--------------------------------------------------------------------------------------*/
void Outline::SetOutline(float start_x, float end_x, float start_y, float end_y,  Component* pParent)
{
	m_start_x = start_x;
	m_end_x = end_x;
	m_start_y = start_y;
	m_end_y = end_y;
	m_pParent = pParent;
}

	
	
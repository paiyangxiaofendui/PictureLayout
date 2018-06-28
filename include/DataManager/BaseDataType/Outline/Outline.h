/*--------------------------------------------------------------------------------------------------------------------*
///	Outline.h   --  轮廓线类声明文件
//	
//	作者：	yuanzb
//	时间：	2016.11.8
//	备注：	用于改进后的最低轮廓线算法	L(x1, x2, y),以后可能根据需求改为宽度优先排样
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Outline_H_ 
#define		_Outline_H_

class	Panel;
class	Component;

class AFX_EXT_CLASS Outline
{
public:
	Outline();
	virtual ~Outline();

public:
	// 操作符重载
	bool operator> (const Outline& ln) const ;		// >操作符重载
	bool operator< (const Outline& ln) const;		// <操作符重载
	bool operator== (const Outline& ln) const;		// ==操作符重载





public:
	// 操作接口
	float GetLength(void) const;					// 获取长度
	float GetWidth(void)const;					// 获取宽度
	void SetOutline(float start_x, float end_x, float start_y, float end_y, Component* pParent);	// 设置轮廓线
	bool Containable(Component* pCpn);		// 能否容纳
	bool IsLegal(void);						// 轮廓线是否合法

public:
	// 成员变量
	float m_start_x;	// 轮廓线起点x坐标
	float m_end_x;		// 轮廓线终点x坐标
	float m_start_y;	// 轮廓线起点y坐标
	float m_end_y;		// 轮廓线终点终点x

	Component* m_pParent;	// 大板指针，属于哪块大板

};

/*typedef vector<Outline>	OutlineList;*/



#endif
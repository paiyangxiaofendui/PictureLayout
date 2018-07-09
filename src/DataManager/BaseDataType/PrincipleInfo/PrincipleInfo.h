/** @brief		规则信息头文件
 *  @file		PrincipleInfo.h 
 *  @author		yaunzb
 *  @version	1.0
 *  @date		2018.7.8
 *  @note       
 *  @since    
 */

#ifndef		_PrincipleInfo_H_
#define		_PrincipleInfo_H_
#include <string>

using namespace std;

  /**
    * @brief	原料信息 \n
    *			原料材质、长宽、价格
    */
//基础信息
class AFX_EXT_CLASS PrincipleInfo
{
public:
	PrincipleInfo(void);
	~PrincipleInfo(void);

public:

	int		m_LayoutOrg;							/**<  排样原点		*/
	int		m_LayoutMethod;							/**<  排样方法		*/

	float	m_PanelLength;							/**<  大板长度		*/
	float	m_PanelWidth;							/**<  大板宽度		*/
	float	m_x_space;								/**<  X方向间隙		*/
	float	m_y_space;								/**<  Y方向间隙		*/
	float	m_left_offset;							/**<  左预留间隙	*/
	float	m_right_offset;							/**<  右预留间隙	*/
	float	m_top_offset;							/**<  上预留间隙	*/
	float	m_bottom_offset;						/**<  下预留间隙	*/
};


#endif
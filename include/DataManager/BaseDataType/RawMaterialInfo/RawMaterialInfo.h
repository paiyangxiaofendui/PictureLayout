/** @brief		原料信息头文件
 *  @file		RawMaterialInfo.h 
 *  @author		yaunzb
 *  @version	1.0
 *  @date		2018.7.8
 *  @note       
 *  @since    
 */

#ifndef		_RawMaterialInfo_H_
#define		_RawMaterialInfo_H_
#include <string>

using namespace std;

  /**
    * @brief	原料信息 \n
    *			原料材质、长宽、价格
    */
//基础信息
class AFX_EXT_CLASS RawMaterialInfo
{
public:
	RawMaterialInfo(void);
	~RawMaterialInfo(void);

public:
	// 原料相关的信息
	
	float	m_PanelLength;					/**<  大板长度		*/
	float	m_PanelWidth;					/**<  大板宽度		*/
	string	m_Material;						/**<  大板材质		*/
};


#endif
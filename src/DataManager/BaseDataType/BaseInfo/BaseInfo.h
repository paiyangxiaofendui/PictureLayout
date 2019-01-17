

#ifndef		_BaseInfo_H_
#define		_BaseInfo_H_
#include <string>

using namespace std;


//基础信息
class AFX_EXT_CLASS BaseInfo
{
public:
	BaseInfo(void);
	~BaseInfo(void);

public:
	// 开料相关的信息
	bool    m_LengthUnlimited;					/**<  长度无限制	*/
	bool    m_WidthUnlimited;					/**<  宽度无限制	*/
	int		m_LayoutOrg;						/**<  排样原点		0：左下角  1：左上角  2：右下角 3：右上角*/
	int		m_LayoutMethod;						/**<  排样方法		 0：省料优先 1：后道优先 2：客户优先 3：智能模式*/	
	float	m_PanelLength;						/**<  大板长度		*/
	float	m_PanelWidth;						/**<  大板宽度		*/

	float	m_x_space;							/**<  X方向间隙		*/
	float	m_y_space;							/**<  Y方向间隙		*/
	float	m_left_offset;						/**<  左预留间隙	*/
	float	m_right_offset;						/**<  右预留间隙	*/
	float	m_top_offset;						/**<  上预留间隙	*/
	float	m_bottom_offset;					/**<  下预留间隙	*/

	float	m_SawKerfWidth;						/**< 锯缝 用于排样				*/
	float	m_DeburringWidth;					/**<  修边距离 4边都修			*/
	float	m_UpHeight;							/**< 安全高度					*/	
	float	m_DownHeight;							/**< 下刀深度					*/
	float	m_org_offset;							/**< 							*/
	float	m_distance1;							/**< 下刀偏移1					*/
	float	m_distance2;							/**< 下刀偏移2					*/
	float	m_MinLength;							/**< 最小大板参数 - 最小长度	*/
	float	m_MinWidth;							/**< 最小大板参数 - 最小宽度	*/
	float	m_MinArea;							/**< 最小大板参数 - 最小面积	*/
																				
public:																			
	// 优化相关参数															 
																				
	int m_FirstSectionOPTimes;					/**<  第一阶段优化次数			*/
	int m_FirstSectionOPMethod;					/**<  第一阶段优化方法			*/
	int m_SecondSectionOPTimes;					/**<  第二阶段优化次数			*/
	int m_ThirdSectionOPTimes;					/**<  第三阶段优化次数			*/
	float m_ThirdSectionOAccptableUtilization;	/**<  第三阶可接受优化率		   */
												
	bool m_bDownerFaceFirst;					/**<  有反面信息优先排列		   */
	bool m_bCustomerFirst;			

public:
	// 字体相关 
	int m_FileTextPosition;						/**<  文件名位置		0：上左  1：上中  2：上右 3：下左  4：下中  5：下右*/
	int m_AutoSpace;							/**<  智能间距			0：不启用  1：启用 */
	int m_OneLabel;								/**<  标签只打一份		0：不启用  1：启用  */
	int m_OneLabelSpace;						/**<  标签只打一份时板的间距  */
	int m_PositionSignType;						/**<  定位符类型		0：圆  1: 十字*/
	float m_PositionSignSize;					/**<  定位符大小		mm  */
	float m_PositionSignDist;					/**<  定位符			定位符间隔为1000mm时，dxf文件每隔1000mm显示一个圆  */
	float m_PositionSignOffset;					/**<  定位符偏移		定位符距离纸张的间隔  */
	float m_FontSize;							/**<  字体大小pt		长=pt/7   高= pt/3  20pt,每个字符长3mm 高7mm */

public:
	string m_strMainTopPath;



};


#endif
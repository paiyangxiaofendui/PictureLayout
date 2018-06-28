#include "stdafx.h"
#include "WorkDef.h"

char* g_aRequirementListTitle[] = {"订单号", "柜号", "条码编号", "产品名称", "成品名称", "材质名称", "开料长度", "开料宽度", "开料厚度", "需切数量", "纹路方向", "封长1", \
	"封宽1", "封长2", "封宽2", "异型", "旋转", "正面槽", "槽翻转", "反面槽", "正面孔", "孔翻转", "反面孔", "客户信息", "加盟店", "拉槽标识", "助记号", \
	"钻孔", "订单类型", "反面条码", "成品长度", "成品宽度", "成品厚度", "异型ID", "客户地址", "钻槽标识", "异形点信息"};

char* g_aRequirementListTitle_ENU[] = {"order", "cabinet", "info", "PorductName", "porduct", "matrial", "width", "height", "thin", "num", "dir", "edgeLen1", \
	"edgeWidth1", "edgeLen2", "edgeWidth2", "pointInfo", "rotate", "slotInfo", "slotRotate", "dslotInfo", "vecInfo", "vecRotate", "dvecInfo", "customeInfo", "store", "SawMark", "HelpSign", \
	"Vertical", "OrderType", "BackCode", "ProductWidth", "ProductHeight", "ProductThin", "otherID", "customeAddr", "DrillingLogo", "OtherPoints"};

char* g_aLabelMachineName[] = {"无", "星辉", "锐诺-维宏系统", "南兴-新代系统", "鼎力", "红马-新代系统", "红马-维宏系统", "锐诺-新代系统", "拓雕-维宏系统", "恩德", "拓雕-新代系统", \
	"中机数控", "永茂-新代系统", "锐诺K1", "WorkList-新代系统", "锐诺K2", "晓风", "先达SKL-2613E", "瑞诺K1-不贴标", "极东-3408-D"};

int g_aMaterialListItem[] = {LIST_COLUMN_ORDER_ID, LIST_COLUMN_CABINET_ID, LIST_COLUMN_BARCODE, LIST_COLUMN_REVERSE_SIDE_BARCODE, LIST_COLUMN_CABINET_NAME, LIST_COLUMN_PANEL_NAME, LIST_COLUMN_MATERIAL, \
	LIST_COLUMN_PRODUCT_LEN, LIST_COLUMN_PRODUCT_WIDTH, LIST_COLUMN_PRODUCT_THICKNESS, LIST_COLUMN_COUNT, LIST_COLUMN_BANDING_FRONT, LIST_COLUMN_BANDING_RIGHT, LIST_COLUMN_BANDING_BACK, LIST_COLUMN_BANDING_LEFT, \
	LIST_COLUMN_ORDER_TYPE, LIST_COLUMN_CUSTOMER_INFO, LIST_COLUMN_CARVING_LEN, LIST_COLUMN_CARVING_WIDTH, LIST_COLUMN_CARVING_THICKNESS, LIST_COLUMN_TEXTURE, LIST_COLUMN_OTHER_SHAPE_ID, LIST_COLUMN_CUSTOMER_ADDRESS/*, LIST_COLUMN_HOLE_SLOT_FLAG*/};
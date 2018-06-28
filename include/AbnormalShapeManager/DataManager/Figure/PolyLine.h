#ifndef	__PolyLine_H__
#define __PolyLine_H__

#include "Figure.h"

class Cad_PolyLine : public Figure
{
public:
	Cad_PolyLine();
	~Cad_PolyLine();

public:
	virtual Figure* CalParallelFigure(int dir = 1, float dist = 2.0);	// 计算平行图形

};

#endif
#include "stdafx.h"
#include "MachineDef.h"
#include "../Misc/Misc.h"

IDToCoordinateMap g_IDToCoordinateMap;

IDToCoordinateMap::IDToCoordinateMap()
{
	CoordinateHandler* pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::LEFT, \
		CoordinateHandler::PositionFBType::BACK, \
		CoordinateHandler::PositionTBType::TOP, \
		'Y', 'X', 'Z', \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[0] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::LEFT, \
		CoordinateHandler::PositionFBType::FRONT, \
		CoordinateHandler::PositionTBType::TOP, \
		'X', 'Y', 'Z', \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[1] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::LEFT, \
		CoordinateHandler::PositionFBType::BACK, \
		CoordinateHandler::PositionTBType::TOP, \
		'X', 'Y', 'Z', \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[2] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::LEFT, \
		CoordinateHandler::PositionFBType::FRONT, \
		CoordinateHandler::PositionTBType::TOP, \
		'Y', 'X', 'Z', \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[3] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::RIGHT, \
		CoordinateHandler::PositionFBType::BACK, \
		CoordinateHandler::PositionTBType::TOP, \
		'Y', 'X', 'Z', \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[4] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::RIGHT, \
		CoordinateHandler::PositionFBType::FRONT, \
		CoordinateHandler::PositionTBType::TOP, \
		'X', 'Y', 'Z', \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::PLUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[5] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::RIGHT, \
		CoordinateHandler::PositionFBType::BACK, \
		CoordinateHandler::PositionTBType::TOP, \
		'X', 'Y', 'Z', \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[6] = pHandler;

	pHandler = new CoordinateHandler(CoordinateHandler::PositionLRType::RIGHT, \
		CoordinateHandler::PositionFBType::FRONT, \
		CoordinateHandler::PositionTBType::TOP, \
		'Y', 'X', 'Z', \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::MINUS, \
		CoordinateHandler::PlusMinusType::PLUS);

	m_mapIDToCoordinate[7] = pHandler;
}

IDToCoordinateMap::~IDToCoordinateMap()
{
	while(m_mapIDToCoordinate.begin() != m_mapIDToCoordinate.end())
	{
		map<int, CoordinateHandler*>::iterator itr = m_mapIDToCoordinate.begin();
		CoordinateHandler* pHandler = itr->second;
		delete pHandler;
		m_mapIDToCoordinate.erase(itr);
	}
}
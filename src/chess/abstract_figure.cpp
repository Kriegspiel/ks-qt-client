#include "abstract_figure.h"

int AbstractFigure::Ids = 0;

AbstractFigure::AbstractFigure(PlayerSide side, FigureType type)
	: m_id(++Ids),
	  m_side(side), m_type(type)
{

}


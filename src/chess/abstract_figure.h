#ifndef ABSTRACT_FIGURE_H
#define ABSTRACT_FIGURE_H

#include "rules.h"

class AbstractFigure
{
private:
	static int Ids;
	int m_id;
	PlayerSide m_side;
	FigureType m_type;

public:
	int Id() const { return m_id; }
	PlayerSide Side() const { return m_side; }
	FigureType Type() const { return m_type; }

	void setId(int id) { m_id = id; }
	void setSide(PlayerSide side) { m_side = side; }
	void setType(FigureType type) { m_type = type; }

	AbstractFigure(PlayerSide side, FigureType type);
};

#endif // ABSTRACT_FIGURE_H

#ifndef ABSTRACT_FIGURE_H
#define ABSTRACT_FIGURE_H

#include <QDebug>
#include <QString>
#include "rules.h"

class AbstractFigure
{
private:
	static int Ids;
	int m_id;
	PlayerSide m_side;
	FigureType m_type;
	int m_row;
	int m_col;

protected:
	QString playerSide2str (PlayerSide side) const;
	QString figureType2str (FigureType type) const;

public:
	int Id() const { return m_id; }
	PlayerSide Side() const { return m_side; }
	FigureType Type() const { return m_type; }
	int Row() const { return m_row; }
	int Col() const { return m_col; }


	void setId(int id) { m_id = id; }
	void setSide(PlayerSide side) { m_side = side; }
	void setType(FigureType type) { m_type = type; }
	void setRow(int row) { m_row = row; }
	void setCol(int col) { m_col = col; }

	static QString pos2Str(int r, int c) { return QString("%1%2").arg(QChar('a'+c)).arg(QChar('8'-r));}
	QString posStr() const { return pos2Str(Row(), Col());}
	friend QDebug operator<<(QDebug debug, const AbstractFigure &f);

	AbstractFigure(PlayerSide side, FigureType type);
	AbstractFigure(PlayerSide side, FigureType type, int row, int col);
};

#endif // ABSTRACT_FIGURE_H

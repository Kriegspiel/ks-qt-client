#include "abstract_figure.h"

int AbstractFigure::Ids = 0;

#define CASE(x) case x: return #x;

QString AbstractFigure::playerSide2str(PlayerSide type) const
{
	switch(type)
	{
		CASE(BLACK)
		CASE(WHITE)

		default:
			throw QObject::tr("Unknown player Side (%1)").arg(type);
	}
}

QString AbstractFigure::figureType2str(FigureType type) const
{
	switch(type)
	{
		CASE(PAWN)
		CASE(ROOK)
		CASE(KNIGHT)
		CASE(BISHOP)
		CASE(QUEEN)
		CASE(KING)

		default:
			throw QObject::tr("Unknown pieceType2s(%1)").arg(type);
	}
}

QDebug operator<<(QDebug debug, const AbstractFigure &f)
{
	QDebugStateSaver saver(debug);
	debug << "id:" << f.Id()
		  << f.figureType2str(f.Type())
		  << f.posStr();

	return debug;
}

AbstractFigure::AbstractFigure(PlayerSide side, FigureType type)
	: m_id(++Ids),
	  m_side(side), m_type(type)
{

}

AbstractFigure::AbstractFigure(PlayerSide side, FigureType type, int row, int col)
	: AbstractFigure(side, type)
{
	m_row = row;
	m_col = col;
}


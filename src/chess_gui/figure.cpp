#include <QDebug>
#include "figure.h"

#define CASE(x) case x: return #x;

QString Figure::playerSide2str(PlayerSide type) {
	switch(type)
	{
		CASE(BLACK)
		CASE(WHITE)

		default:
			throw tr("Unknown player Side (%1)").arg(type);
	}
}

QString Figure::figureType2str(FigureType type)
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
			throw QString("Unknown pieceType2s(%1)").arg(type);
	}
}

QString Figure::resPath(const QString &ext)
{
	QString s = playerSide2str(Side()).toLower();
	QString t = figureType2str(Type()).toLower();
	return QString(":/res/%1-%2%3").arg(t).arg(s).arg(ext);
}

void Figure::initPixmap()
{
	setPixmap(QPixmap(resPath()));
}

void Figure::initPixmap(const QSize &size)
{
	setPixmap(QIcon(resPath(".svg")).pixmap(size));
}

void Figure::resizeEvent(QResizeEvent *event)
{
	QSize new_size = event->size();
	initPixmap(new_size);
}

Figure::Figure(QWidget* parent, PlayerSide side, FigureType type, int row, int col)
	: QLabel((QWidget *)parent), AbstractFigure(side, type),
	  m_row(row), m_col(col)
{
	initPixmap(size());
	setAttribute(Qt::WA_DeleteOnClose);
	setCursor(Qt::OpenHandCursor);
}

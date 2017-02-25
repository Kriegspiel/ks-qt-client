#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QFrame>
#include <QPainter>
#include <QBrush>
#include <QList>

#include "../chess/abstract_chess.h"
#include "figure.h"

class ChessBoard : public QWidget, public AbstractChess
{
	Q_OBJECT

signals:
	void historyUpdated(const QString &move_info);

private:
	static const QBrush disabledColors[];
	static const QBrush enabledColors[];
	static const QBrush underAttackColors[];
	static const QBrush attackColors[];

	const int LABEL_HEIGHT = 20;
	const int LABEL_WIDTH = 15;
	QRectF boardRect;
	int cellSize;

protected:
	virtual void initFigure(PlayerSide side, FigureType type, int row, int col);
	virtual void figureSelectedAt(int row, int col);
	virtual void figureMovedFromTo(int src_row, int src_col, int dst_row, int dst_col, bool will_eat);
	virtual void hideFigureAt(int row, int col);
	virtual void figureDeselected();

	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
	void mousePressEvent(QMouseEvent* event);

	void startDragEvent(const QPointF &pos, Figure *figure);

	void paintEvent(QPaintEvent *);

private:
	QRectF cellGeometry (int row, int col);
	QPointF getCellCenter(int row, int col);
	void moveFigure (Figure *f, int row, int col);
	void redrawBoardLabels (QPainter &p);
	void boardLabel (QPainter &p, char c, int x, int y, int width);
	void updateGeometry ();

public:
	explicit ChessBoard(QWidget* parent = 0);
};

#endif // CHESSBOARD_H

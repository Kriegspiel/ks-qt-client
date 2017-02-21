#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QFrame>
#include <QPainter>
#include <QBrush>
#include <QList>

#include "../chess/abstract_board.h"
#include "figure.h"

class ChessBoard : public QWidget, public AbstractBoard
{
	Q_OBJECT

signals:
	void moveFinished(const QString &move_info);

private:

	enum Status
	{
		ENABLE = 0,
		DISABLE,
		ATTACK
	};

	struct Field
	{
		int row;
		int col;
		Status status;

		Field(int row, int col, Status status)
			: row(row), col(col), status(status)
		{}

		Field(){}
	};

	const int LABEL_HEIGHT = 20;
	const int LABEL_WIDTH = 15;
	QRectF board_rect;
	int cell_size;

public:
	// Load from file
	void loadFromeFile(QString fileName);
	void loadFromResources(QString fileName);
	FigureType char2figureType(QChar c);

	// Доступные ходы
	QList<Field> avalibleMoviesList;
	Status avalibleMoves[ROWS][COLS];
	bool fieldsUnderAttack[ROWS][COLS];

	Figure* selectedFigure;

 protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
	void mousePressEvent(QMouseEvent* event);

	void startDragEvent(const QPointF &pos, Figure *figure);

	void paintEvent(QPaintEvent *);

private:
	QRectF cellGeometry (int row, int col);
	void moveFigure (Figure *f, int row, int col);
	void redrawBoardLabels(QPainter &p);
	void boardLabel(QPainter &p, char c, int x, int y, int width);
	void updateGeometry();


	// Get abalible turnes for figure
	QList<Field> getAvalibleTurnes(AbstractFigure *piece, int row, int col);

	void fillFieldsUnderAttack(PlayerSide side);
	void clearAvailableMoves();
	Status getFieldStatus(int row, int col, PlayerSide side);

	QList<Field> getDiagonalStatus(int row, int col, PlayerSide side);
	QList<Field> getHorVerStatus(int row, int col, PlayerSide side);
	QList<Field> getStatusNear(int row, int col, PlayerSide side, int d_r, int d_c);


public:
	explicit ChessBoard(QWidget* parent = 0);
};

#endif // CHESSBOARD_H

#ifndef ABSTRACT_BOARD_H
#define ABSTRACT_BOARD_H

#include <QDebug>
#include <QList>
#include <QFile>
#include <QResource>

#include "rules.h"
#include "abstract_figure.h"

class AbstractChess
{

protected:
	enum MoveStatus
	{
		CORRECT_MOVE = 0,
		INCORRECT_MOVE,
		ATTACK_MOVE
	};

	struct Field
	{
		int row;
		int col;
		MoveStatus status;

		Field(int row, int col, MoveStatus status)
			: row(row), col(col), status(status)
		{}

		Field(){}
	};

	void selectCell(int row, int col);
	void selectFigureAt(int row, int col);
	bool hasSelectedFigure() { return selectedFigure != NULL; }
	void addFigureAt(AbstractFigure *figure, int row, int col);
	FigureType getSelectedFigureType() { return (hasSelectedFigure()? selectedFigure->Type(): FigureType::EMPTY); }
	AbstractFigure *figureAt(int row, int col) { return boardMap[row][col]; }

	virtual void initFigure(PlayerSide side, FigureType type, int row, int col);
	virtual void figureSelectedAt(int row, int col) = 0;
	virtual void figureDeselected() = 0;
	virtual void figureMovedFromTo(int src_row, int src_col, int dst_row, int dst_col, AbstractFigure *attacked_figure) = 0;
	virtual void hideFigureAt(int row, int col) = 0;

	MoveStatus avalibleMoveStatus(int row, int col) { return avalibleMoves[row][col]; }

	bool isFieldUnderAttack(int row, int col) { return fieldsUnderAttack[row][col]; }

protected:
	// Properties
	AbstractFigure* boardMap[ROWS][COLS];
	bool fieldsUnderAttack[ROWS][COLS];
	MoveStatus avalibleMoves[ROWS][COLS];
	AbstractFigure *selectedFigure;

	//Moving and selecting
	bool canSelectFigure(int row, int col);
	void moveSelectedFigure(int row, int col);
	void deselectFigure();

	// Help functions
	virtual void updateAvalibleMoviesMap();

	MoveStatus getFieldStatus(int row, int col, PlayerSide side);

public:
	// Load from file
	void loadFromFile(QString fileName);
	void loadFromResources(QString fileName);
	FigureType char2figureType(QChar c);

	AbstractChess();
};

#endif // ABSTRACT_BOARD_H

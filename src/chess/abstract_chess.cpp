#include "abstract_chess.h"

void AbstractChess::selectCell(int row, int col)
{
	if (selectedFigure != NULL)
	{
		// deselect
		if (selectedFigure->Row() == row && selectedFigure->Col() == col)
			deselectFigure();
		else
			moveSelectedFigure(row, col);
	}
	else
	{
		if (canSelectFigure(row, col))
			selectFigureAt(row, col);
	}
}

bool AbstractChess::canSelectFigure(int row, int col)
{
	// ToDo:: check player side
	return (boardMap[row][col] != NULL);
}

void AbstractChess::moveSelectedFigure(int row, int col)
{
	if (avalibleMoves[row][col] != INCORRECT_MOVE)
	{
		int old_row = selectedFigure->Row();
		int old_col = selectedFigure->Col();
		AbstractFigure *attacked_figure = attackedFigure(selectedFigure, row, col);
		bool will_eat = (attacked_figure != NULL);
		if (will_eat)
		{
			qDebug() << "Eat figure:" << *attacked_figure;
			hideFigureAt(attacked_figure->Row(), attacked_figure->Col());
			boardMap[attacked_figure->Row()][attacked_figure->Col()] = NULL;
		}

		qDebug() << "Move figure" << *selectedFigure << "to" << AbstractFigure::pos2Str(row, col);
		figureMovedFromTo(selectedFigure->Row(), selectedFigure->Col(),
						  row, col, attacked_figure);
		boardMap[old_row][old_col] = NULL;

		selectedFigure->setRow(row);
		selectedFigure->setCol(col);
		boardMap[row][col] = selectedFigure;

		updateGameStatus(selectedFigure, old_row, old_col);
		deselectFigure();

		return;
	}
}

void AbstractChess::selectFigureAt(int row, int col)
{
	selectedFigure = boardMap[row][col];

	qDebug() << "Selected figure:" << *selectedFigure;
	updateAvalibleMoviesMap();
	figureSelectedAt(row, col);
}

void AbstractChess::addFigureAt(AbstractFigure *figure, int row, int col)
{
	if (boardMap[row][col] != NULL)
	{
		AbstractFigure *f = boardMap[row][col];
		qDebug() << "Warning in add figure on cell" << f->posStr() << "already has figure" << *f;
	}
	boardMap[row][col] = figure;
}

void AbstractChess::initFigure(PlayerSide side, FigureType type, int row, int col)
{
	AbstractFigure* figure = new AbstractFigure(side, type, row, col);
	addFigureAt(figure, row, col);
}

void AbstractChess::deselectFigure()
{
	qDebug() << "Deselect figure:" << *selectedFigure;
	selectedFigure = NULL;
	updateAvalibleMoviesMap();
	figureDeselected();
}

void AbstractChess::updateAvalibleMoviesMap()
{
	for (int row = 0; row < ROWS; row++)
		for (int col = 0; col < COLS; col++)
			avalibleMoves[row][col] = hasSelectedFigure() ?
					getFieldStatus(row, col, selectedFigure->Side()) :
					INCORRECT_MOVE;
}

AbstractChess::MoveStatus AbstractChess::getFieldStatus(int row, int col, PlayerSide side)
{
	if((row < 0) || (row >= ROWS) || (col < 0) || (col >= COLS))
		return INCORRECT_MOVE;

	if(boardMap[row][col] == NULL)
		return CORRECT_MOVE;
	else
	{
		if(side == boardMap[row][col]->Side()) // friend figure
			return INCORRECT_MOVE;
		else
			return ATTACK_MOVE;
	}
}

void AbstractChess::loadFromFile(QString fileName)
{
	QFile f(fileName);

	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
//		QMessageBox::critical(this, tr("File read error"), tr("Cannot read from file %1 Error: %2")
//				.arg(f.fileName()).arg(f.errorString()));

		qDebug() << "Cannot open: " << f.fileName() <<
			 " Error: " << f.errorString() << endl;
		return;
	}

	QTextStream in(&f);

	for (int row = 0; row < ROWS; ++row)
	{
		QString line = in.readLine();
		qDebug() << "Line readen:" << line;

		for (int col = 0; col < COLS; ++col)
		{
			boardMap[row][col] = NULL;
			QChar c = line.at(col);

			if(c == '_') continue;

			PlayerSide side = (c.isLower()) ? BLACK : WHITE;
			FigureType type = char2figureType(c);

			initFigure(side, type, row, col);
		}
	}
}

void AbstractChess::loadFromResources(QString fileName)
{
	qDebug() << "Load from file" << fileName;

	QResource res(fileName);
	loadFromFile(res.absoluteFilePath());
}

FigureType AbstractChess::char2figureType(QChar c)
{
	switch(c.toUpper().toLatin1())
	{
		case 'P':
			return PAWN;

		case 'R':
			return ROOK;

		case 'H':
			return KNIGHT;

		case 'B':
			return BISHOP;

		case 'Q':
			return QUEEN;

		case 'K':
			return KING;

		default:
			throw QObject::tr("Cannot parse symbol: %1").arg(c);
	}
}

AbstractChess::AbstractChess()
	: selectedFigure(NULL)
{
	updateAvalibleMoviesMap();
}


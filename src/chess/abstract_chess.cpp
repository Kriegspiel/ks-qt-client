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
	foreach(Field f, avalibleMoviesList)
	{
		if (f.col == col && f.row == row)
		{
			bool will_eat = (boardMap[row][col] != NULL);
			if (will_eat)
				hideFigureAt(row, col);

			qDebug() << "Move figure" << *selectedFigure << "to" << AbstractFigure::pos2Str(row, col);
			boardMap[selectedFigure->Row()][selectedFigure->Col()] = NULL;
			figureMovedFromTo(selectedFigure->Row(), selectedFigure->Col(),
							  row, col, will_eat);

			selectedFigure->setRow(row);
			selectedFigure->setCol(col);
			boardMap[row][col] = selectedFigure;
			deselectFigure();
			return;
		}
	}
}

void AbstractChess::selectFigureAt(int row, int col)
{
	selectedFigure = boardMap[row][col];

	qDebug() << "Selected figure:" << *selectedFigure;
	avalibleMoviesList = getAvalibleMoves(selectedFigure, row, col);
	foreach (Field a, avalibleMoviesList)
	{
		avalibleMoves[a.row][a.col] = a.status;
		qDebug() << "Can go to:" << AbstractFigure::pos2Str(a.row, a.col) << a.status;
	}

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
	clearAvailableMoves();
	figureDeselected();
}

QList<AbstractChess::Field> AbstractChess::getAvalibleMoves(AbstractFigure* figure, int row, int col)
{
	QList<Field> avalibale_moves;
	MoveStatus st;
	PlayerSide side = figure->Side();

	switch (figure->Type())
	{
		case PAWN: {
			int max = 2;
			if((side == BLACK && row == 1) ||
			   (side == WHITE && row == ROWS-2))
				max = 3;

			for (int var = 1; var < max; ++var)
			{
				int next_row = row - var; // white
				if(side == BLACK)
					next_row = row + var;

				if((st = getFieldStatus(next_row, col, BLACK)) == CORRECT_MOVE)
					avalibale_moves.append(Field (next_row, col, st));
			}

			// Check if we can attack
			int near_row = row - 1; // for white
			if(side == BLACK)
				near_row = row + 1;

			if ((st = getFieldStatus(near_row, col + 1, side)) == ATTACK_MOVE)
				avalibale_moves.append(Field (near_row, col + 1, st));

			if ((st = getFieldStatus(near_row, col - 1, side)) == ATTACK_MOVE)
				avalibale_moves.append(Field (near_row, col - 1, st));

			break;
		}

		case BISHOP: {
			avalibale_moves = getDiagonalStatus(row, col, side);
			break;
		}

		case KNIGHT: {
			int k_turns[8][2] = {2, 1,
								2, -1,
								1, 2,
								1, -2,
								-1, -2,
								-1, 2,
								-2, 1,
								-2, -1
							   };

			for (int i = 0; i < 8; ++i)
			{
				int r = row + k_turns[i][0];
				int c = col + k_turns[i][1];
				if ((st = getFieldStatus(r, c, side)) != INCORRECT_MOVE)
					avalibale_moves.append(Field(r, c, st));
			}

			break;
		}

		case ROOK: {
			avalibale_moves = getHorVerStatus(row, col, side);
			break;
		}

		case QUEEN: {
			avalibale_moves = getHorVerStatus(row, col, side) + getDiagonalStatus(row, col, side);
			break;
		}

		case KING: {
			int k_turns[8][2] = {1, 1,
								 1, 0,
								 1, -1,
								 0, 1,
								 0, -1,
								 -1, 1,
								 -1, 0,
								 -1, -1};
			fillFieldsUnderAttack(side);

			for (int i = 0; i < 8; ++i)
			{
				int r = row + k_turns[i][0];
				int c = col + k_turns[i][1];
				if (((st = getFieldStatus(r, c, side)) != INCORRECT_MOVE) &&
					!fieldsUnderAttack[r][c])
					avalibale_moves.append(Field(r, c, st));
			}

			break;
		}

		case EMPTY:
		default:
			break;
	}

	return avalibale_moves;
}

void AbstractChess::clearAvailableMoves()
{
	avalibleMoviesList.clear();

	for (int row = 0; row < ROWS; row++)
		for (int col = 0; col < COLS; col++)
			avalibleMoves[row][col] = INCORRECT_MOVE;
}


QList<AbstractChess::Field> AbstractChess::getDiagonalStatus(int row, int col, PlayerSide side)
{
	QList<Field> res;
	res.append(getStatusNear(row, col, side, 1, 1));
	res.append(getStatusNear(row, col, side, -1, 1));
	res.append(getStatusNear(row, col, side, -1, -1));
	res.append(getStatusNear(row, col, side, 1, -1));

	return res;
}

QList<AbstractChess::Field> AbstractChess::getHorVerStatus(int row, int col, PlayerSide side)
{
	QList<Field> res;
	res.append(getStatusNear(row, col, side, 1, 0));
	res.append(getStatusNear(row, col, side, -1, 0));
	res.append(getStatusNear(row, col, side, 0, 1));
	res.append(getStatusNear(row, col, side, 0, -1));

	return res;
}

QList<AbstractChess::Field> AbstractChess::getStatusNear(int row, int col, PlayerSide side, int d_r, int d_c)
{
	QList<Field> res;
	MoveStatus st;
	row += d_r;
	col += d_c;

	while ((st = getFieldStatus(row, col, side)) != INCORRECT_MOVE)
	{
		res.append(Field(row, col, st));
		if (st == ATTACK_MOVE)
			break;
		row += d_r;
		col += d_c;
	}
	return res;
}

void AbstractChess::fillFieldsUnderAttack(PlayerSide side)
{
	QList<Field> fields_under_attack_list;

	for (int col = 0; col < COLS; ++col)
	{
		for (int row = 0; row < ROWS; ++row)
		{
			fieldsUnderAttack[row][col] = false;
			AbstractFigure* figure = boardMap[row][col];

			if (figure != NULL && side != figure->Side())
			{
				if (figure->Type() == KING)
				{
					for (int r = -1; r <= 1; ++r)
						for (int c = -1; c <= 1; ++c)
						{
							if (c == 0 && r == 0) continue;

							if (getFieldStatus(row + r, col + c, side) == CORRECT_MOVE)
								fields_under_attack_list.append(Field(row + r, col + c, ATTACK_MOVE));
						}
				}
				else if (figure->Type() == PAWN)
				{
					int near_row = row - 1; // for black
					if(side == WHITE)
						near_row = row + 1;

					if (getFieldStatus(near_row, col + 1, side) == CORRECT_MOVE)
						fields_under_attack_list.append(Field (near_row, col + 1, ATTACK_MOVE));

					if (getFieldStatus(near_row, col - 1, side) == CORRECT_MOVE)
						fields_under_attack_list.append(Field (near_row, col - 1, ATTACK_MOVE));
				}
				else
					fields_under_attack_list.append(getAvalibleMoves(figure, row, col));
			}
		}
	}

	foreach (Field f, fields_under_attack_list)
	{
		fieldsUnderAttack[f.row][f.col] = true;
	}
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
	clearAvailableMoves();
}


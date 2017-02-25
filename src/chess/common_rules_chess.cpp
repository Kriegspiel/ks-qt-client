#include "common_rules_chess.h"

void CommonRulesChess::updateAvalibleMoviesMap()
{
	if (selectedFigure == NULL)
	{
		clearAvalibleMoviesMap();
		return;
	}

	QList<Field> avalible_movies_list = getAvalibleMoves(selectedFigure);
	foreach (Field a, avalible_movies_list)
	{
		avalibleMoves[a.row][a.col] = a.status;
		//qDebug() << "Can go to:" << AbstractFigure::pos2Str(a.row, a.col) << a.status;
	}
}

QList<AbstractChess::Field> CommonRulesChess::getAvalibleMoves(AbstractFigure* figure)
{
	QList<Field> avalibale_movies;
	MoveStatus st;
	PlayerSide side = figure->Side();
	int row = figure->Row();
	int col = figure->Col();

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
					avalibale_movies.append(Field (next_row, col, st));
			}

			// Check if we can attack
			int near_row = row - 1; // for white
			if(side == BLACK)
				near_row = row + 1;

			if ((st = getFieldStatus(near_row, col + 1, side)) == ATTACK_MOVE)
				avalibale_movies.append(Field (near_row, col + 1, st));

			if ((st = getFieldStatus(near_row, col - 1, side)) == ATTACK_MOVE)
				avalibale_movies.append(Field (near_row, col - 1, st));

			if (canEatEnPassant &&
				qAbs(col - cellToEatEnPassant.y()) == 1 && near_row == cellToEatEnPassant.x())
				avalibale_movies.append(Field (near_row, cellToEatEnPassant.y(), ATTACK_MOVE));
			break;
		}

		case BISHOP: {
			avalibale_movies = getDiagonalStatus(row, col, side);
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
					avalibale_movies.append(Field(r, c, st));
			}

			break;
		}

		case ROOK: {
			avalibale_movies = getHorVerStatus(row, col, side);
			break;
		}

		case QUEEN: {
			avalibale_movies = getHorVerStatus(row, col, side) + getDiagonalStatus(row, col, side);
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
					avalibale_movies.append(Field(r, c, st));
			}

			break;
		}

		case EMPTY:
		default:
			break;
	}

	return avalibale_movies;
}

void CommonRulesChess::clearAvalibleMoviesMap()
{
	for (int row = 0; row < ROWS; row++)
		for (int col = 0; col < COLS; col++)
			avalibleMoves[row][col] = INCORRECT_MOVE;
}

void CommonRulesChess::updateGameStatus(AbstractFigure *figure, int old_row, int old_col)
{
	canEatEnPassant = false;
	if (figure->Type() == KING)
		canDoRoque[figure->Type()][ROQUE_LEFT] = canDoRoque[figure->Type()][ROQUE_RIGHT] = false;
	else if (figure->Type() == ROOK)
		canDoRoque[figure->Type()][((old_col==0)?ROQUE_LEFT:ROQUE_RIGHT)] = false;
	else if (figure->Type() == PAWN && qAbs(figure->Row() - old_row) == 2)
	{
		canEatEnPassant = true;
		cellToEatEnPassant = QPoint((figure->Row() + old_row)/2, old_col);
	}
}

AbstractFigure *CommonRulesChess::attackedFigure(AbstractFigure *figure, int row, int col)
{
	int attacked_row = row;
	if (figure->Type() == PAWN)
	{
		if (canEatEnPassant && cellToEatEnPassant == QPoint(row, col))
		{
			attacked_row = figure->Row();
		}
	}

	return boardMap[attacked_row][col];
}

QList<AbstractChess::Field> CommonRulesChess::getDiagonalStatus(int row, int col, PlayerSide side)
{
	QList<Field> res;
	res.append(getStatusNear(row, col, side, 1, 1));
	res.append(getStatusNear(row, col, side, -1, 1));
	res.append(getStatusNear(row, col, side, -1, -1));
	res.append(getStatusNear(row, col, side, 1, -1));

	return res;
}

QList<AbstractChess::Field> CommonRulesChess::getHorVerStatus(int row, int col, PlayerSide side)
{
	QList<Field> res;
	res.append(getStatusNear(row, col, side, 1, 0));
	res.append(getStatusNear(row, col, side, -1, 0));
	res.append(getStatusNear(row, col, side, 0, 1));
	res.append(getStatusNear(row, col, side, 0, -1));

	return res;
}

QList<AbstractChess::Field> CommonRulesChess::getStatusNear(int row, int col, PlayerSide side, int d_r, int d_c)
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

void CommonRulesChess::fillFieldsUnderAttack(PlayerSide side)
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
					fields_under_attack_list.append(getAvalibleMoves(figure));
			}
		}
	}

	foreach (Field f, fields_under_attack_list)
	{
		fieldsUnderAttack[f.row][f.col] = true;
	}
}

CommonRulesChess::CommonRulesChess()
{
	canEatEnPassant = false;
	canDoRoque[BLACK][ROQUE_LEFT] = canDoRoque[BLACK][ROQUE_RIGHT] = true;
	canDoRoque[WHITE][ROQUE_LEFT] = canDoRoque[WHITE][ROQUE_RIGHT] = true;
}


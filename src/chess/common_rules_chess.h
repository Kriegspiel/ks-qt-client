#ifndef COMMONRULESCHESS_H
#define COMMONRULESCHESS_H

#include "abstract_chess.h"

class CommonRulesChess : public AbstractChess
{
	enum RoqueType { ROQUE_LEFT = 0, ROQUE_RIGHT };
protected:
	virtual void updateAvalibleMoviesMap();

private:
	bool canEatEnPassant;
	QPoint cellToEatEnPassant;
	bool canDoRoque[2][2];

	QList<Field> getAvalibleMoves(AbstractFigure *piece);
	void clearAvalibleMoviesMap();
	virtual void updateGameStatus(AbstractFigure *figure, int old_row, int old_col);
	virtual AbstractFigure *attackedFigure(AbstractFigure *figure, int row, int col);

	QList<Field> getDiagonalStatus(int row, int col, PlayerSide side);
	QList<Field> getHorVerStatus(int row, int col, PlayerSide side);
	QList<Field> getStatusNear(int row, int col, PlayerSide side, int d_r, int d_c);
	void fillFieldsUnderAttack(PlayerSide side);

public:
	CommonRulesChess();
};

#endif // COMMONRULESCHESS_H

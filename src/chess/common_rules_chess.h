#ifndef COMMONRULESCHESS_H
#define COMMONRULESCHESS_H

#include "abstract_chess.h"

class CommonRulesChess : public AbstractChess
{
protected:
	virtual void updateAvalibleMoviesMap();

private:
	QList<Field> getAvalibleMoves(AbstractFigure *piece);
	void clearAvalibleMoviesMap();

	QList<Field> getDiagonalStatus(int row, int col, PlayerSide side);
	QList<Field> getHorVerStatus(int row, int col, PlayerSide side);
	QList<Field> getStatusNear(int row, int col, PlayerSide side, int d_r, int d_c);
	void fillFieldsUnderAttack(PlayerSide side);

public:
	CommonRulesChess();
};

#endif // COMMONRULESCHESS_H

#ifndef COMMONRULESCHESS_H
#define COMMONRULESCHESS_H

#include "abstract_chess.h"

class CommonRulesChess : public AbstractChess
{
	enum RoqueType { ROQUE_LEFT = 0, ROQUE_RIGHT };
protected:
	virtual void updateAvalibleMoviesMap();
	virtual void figureMovedFromTo(int src_row, int src_col, int dst_row, int dst_col, AbstractFigure *attacked_figure);

private:
	bool canEatEnPassant;
	QPoint cellToEatEnPassant;

	bool canDoRoque[2][2];
	QList<QPoint> cellsToDoRoque;
	bool canMoveRoque(AbstractFigure *rook_figure, int *king_target_col, bool skip_attack_fields_update = true);

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

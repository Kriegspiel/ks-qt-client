#ifndef ABSTRACT_BOARD_H
#define ABSTRACT_BOARD_H

#include "rules.h"
#include "abstract_figure.h"

class AbstractBoard
{
protected:
	AbstractFigure* boardMap[8][8];

public:
	AbstractBoard();
};

#endif // ABSTRACT_BOARD_H

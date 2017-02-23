#include <QtGui>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

#include "chessboard.h"

const char* mimeFormat = "application/x-ks_chessfigure";

QList<ChessBoard::Field> ChessBoard::getAvalibleTurnes(AbstractFigure* figure, int row, int col)
{
	QList<Field> avalibale_moves;
	Status st;
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

				if((st = getFieldStatus(next_row, col, BLACK)) == ENABLE)
					avalibale_moves.append(Field (next_row, col, st));
			}

			// Check if we can attack
			int near_row = row - 1; // for white
			if(side == BLACK)
				near_row = row + 1;

			if ((st = getFieldStatus(near_row, col + 1, side)) == ATTACK)
				avalibale_moves.append(Field (near_row, col + 1, st));

			if ((st = getFieldStatus(near_row, col - 1, side)) == ATTACK)
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
				if ((st = getFieldStatus(r, c, side)) != DISABLE)
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
				if (((st = getFieldStatus(r, c, side)) != DISABLE) &&
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

void ChessBoard::clearAvailableMoves()
{
	avalibleMoviesList.clear();

	for (int row = 0; row < ROWS; row++)
		for (int col = 0; col < COLS; col++)
			avalibleMoves[row][col] = DISABLE;
}

void ChessBoard::moveFigure(Figure *f, int row, int col)
{
	f->setRow(row);
	f->setCol(col);

	QRectF cell = cellGeometry(row, col);
	f->setGeometry(cell.adjusted(2, 2, -2, -2).toRect());
	f->show();

}

QRectF ChessBoard::cellGeometry(int row, int col)
{
	return QRectF(board_rect.x() + col * cell_size,
			board_rect.y() + row * cell_size,
			cell_size, cell_size);
}

void ChessBoard::dragEnterEvent(QDragEnterEvent* event)
{
	if (!event->mimeData()->hasFormat(mimeFormat)) {
		event->ignore();
		return;
	}

	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->acceptProposedAction();
}

void ChessBoard::dragMoveEvent(QDragMoveEvent* event)
{
	if (!event->mimeData()->hasFormat(mimeFormat))
	{
		event->ignore();
		return;
	}

	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->acceptProposedAction();
}

void ChessBoard::dropEvent(QDropEvent* event)
{
	if (!event->mimeData()->hasFormat(mimeFormat) || selectedFigure == NULL)
	{
		event->ignore();
		return;
	}

	QPoint offset;	

	// Move figure
	QPoint p = event->pos() - offset;
	int tmpRow, tmpCol;
	tmpRow = selectedFigure->Row();
	tmpCol = selectedFigure->Col();
	selectedFigure->setRow((p.y() + cell_size / 2) / cell_size);
	selectedFigure->setCol((p.x() + cell_size / 2) / cell_size);
	selectedFigure->move(tmpCol * cell_size, tmpRow * cell_size);
	selectedFigure->show();


	if (event->source() == this)
	{
		boardMap[tmpRow][tmpCol] = NULL;

		boardMap[selectedFigure->Row()][selectedFigure->Col()] = selectedFigure;

		event->setDropAction(Qt::MoveAction);
		event->accept();
	} else
		event->acceptProposedAction();
}

void ChessBoard::mousePressEvent(QMouseEvent* event)
{
	if (selectedFigure != NULL)
	{
		QPointF pos = event->pos();
		if (board_rect.contains(pos))
		{
			pos -= board_rect.topLeft();
			int row = pos.y() / cell_size;
			int col = pos.x() / cell_size;

			if (selectedFigure->Row() == row && selectedFigure->Col() == col)
			{
				selectedFigure = NULL;
				clearAvailableMoves();
				update();
				return;
			}

			foreach(Field a, avalibleMoviesList)
			{
				if (a.col == col && a.row == row)
				{
					boardMap[selectedFigure->Row()][selectedFigure->Col()] = NULL;
					bool will_eat = (boardMap[row][col] != NULL);
					if (will_eat)
						((Figure *)boardMap[row][col])->hide();

					QString from = QString("%1%2").arg(QString('a'+selectedFigure->Col())).
								   arg(QString('0'+selectedFigure->Row()));
					QString to = QString("%1%2").arg(QString('a'+col)).arg(QString('0'+row));
					emit moveFinished(QString("%1: %2%4%3")
									  .arg((selectedFigure->Side() == WHITE)?tr("White"):tr("Black"))
									  .arg(from).arg(to).arg(will_eat? tr("x"):"—"));

					boardMap[row][col] = selectedFigure;
					moveFigure(selectedFigure, row, col);
					selectedFigure = NULL;
					clearAvailableMoves();
					update();
					return;
				}
			}
		}
		return;
	}
	Figure* figure = static_cast<Figure*>(childAt(event->pos()));

	if (!figure)
		return;

	selectedFigure = figure;
	qDebug() << "Selected figure:" << figure->Id() << "row:" << figure->Row() << "col:" << figure->Col();

	avalibleMoviesList = getAvalibleTurnes(selectedFigure, figure->Row(), figure->Col());
	foreach (Field a, avalibleMoviesList)
	{
		avalibleMoves[a.row][a.col] = a.status;
		qDebug() << "Can go to:" << a.row << a.col << a.status;
		update();
	}

	//startDragEvent(event->localPos(), figure);
}

void ChessBoard::startDragEvent(const QPointF &pos, Figure* figure)
{
	QPixmap pixmap = figure->pixmap();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << pixmap << QPointF(pos - figure->pos());

	QMimeData* mimeData = new QMimeData;
	mimeData->setData(mimeFormat, itemData);

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(pos.toPoint() - figure->pos());

	QPixmap tempPixmap = pixmap;
	QPainter painter;
	painter.begin(&tempPixmap);
	painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
	painter.end();

	figure->setPixmap(tempPixmap);

	if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
	{
		;//piece->close();
	} else {
		figure->show();
		figure->setPixmap(pixmap);
	}
}

void ChessBoard::boardLabel(QPainter &p, char c, int x, int y, int width)
{
	QRect rect = QRect(x, y, width, LABEL_HEIGHT);
	p.drawText(rect, Qt::AlignCenter, QString(c));
}

void ChessBoard::updateGeometry()
{
	QRect g = this->geometry();

	board_rect = QRectF (0, 0, g.width(), g.height());
	board_rect = board_rect.marginsRemoved(QMargins(LABEL_WIDTH, LABEL_HEIGHT, LABEL_WIDTH, LABEL_HEIGHT));

	qreal delta = (board_rect.width() - board_rect.height()) / 2.0;
	if (delta > 0.0)
		board_rect.adjust(delta, 0.0, -delta, 0.0);
	else
		board_rect.adjust(0.0, -delta, 0.0, delta);

	cell_size = board_rect.width() / ROWS;
}

void ChessBoard::paintEvent(QPaintEvent *)
{
	updateGeometry();

	QPainter p(this);

	redrawBoardLabels(p);
	QBrush disabled_color[] = { QBrush(Qt::white), QBrush(Qt::gray) } ;
	QBrush enabled_color[] = { QBrush(Qt::yellow), QBrush(Qt::darkYellow) } ;
	QBrush under_attack_color[] = { QBrush(Qt::blue), QBrush(Qt::darkBlue) } ;
	QBrush attack_color[] = { QBrush(Qt::red), QBrush(Qt::darkRed) } ;

	for(int row = 0; row < ROWS; ++row)
	{
		for(int col = 0; col < COLS; ++col)
		{
			switch(avalibleMoves[row][col])
			{
			case Status::ENABLE:
				p.setBrush(enabled_color[(row + col) % 2]);
				break;

			case Status::DISABLE:
				p.setBrush(disabled_color[(row + col) % 2]);
				break;
			case Status::ATTACK:
				p.setBrush(attack_color[(row + col) % 2]);
				break;
			}

			if (selectedFigure && selectedFigure->Type() == KING && fieldsUnderAttack[row][col])
				p.setBrush(under_attack_color[(row + col) % 2]);

			QRectF cell = cellGeometry(row, col);
			p.drawRect(cell);

			Figure * figure = (Figure *)boardMap[row][col];

			if (figure)
			{
				figure->setGeometry(cell.adjusted(2, 2, -2, -2).toRect());
				figure->show();
			}

		}
	}
}

void ChessBoard::redrawBoardLabels(QPainter &p)
{
	QFont font = p.font();
	font.setPixelSize(16);
	p.setFont(font);

	for(int col = 0; col < COLS; ++col)
	{
		char L = 'a' + col;
		int x = col * cell_size + board_rect.left();
		boardLabel(p, L, x, board_rect.top() - LABEL_HEIGHT, cell_size);
		boardLabel(p, L, x, board_rect.bottom(), cell_size);
	}
	for (int row = 0; row < ROWS; ++row)
	{
		char D = '1' + row;
		int y = board_rect.bottom() - row * cell_size - (cell_size + LABEL_HEIGHT) / 2;
		boardLabel(p, D, board_rect.left() - LABEL_WIDTH, y, LABEL_WIDTH);
		boardLabel(p, D, board_rect.right(), y, LABEL_WIDTH);
	}
}

ChessBoard::Status ChessBoard::getFieldStatus(int row, int col, PlayerSide side)
{
	if((row < 0) || (row >= ROWS) || (col < 0) || (col >= COLS))
		return DISABLE;

	if(boardMap[row][col] == NULL)
		return ENABLE;
	else
	{
		if(side == boardMap[row][col]->Side()) // friend figure
			return DISABLE;
		else
			return ATTACK;
	}
}

QList<ChessBoard::Field> ChessBoard::getDiagonalStatus(int row, int col, PlayerSide side)
{
	QList<Field> res;
	res.append(getStatusNear(row, col, side, 1, 1));
	res.append(getStatusNear(row, col, side, -1, 1));
	res.append(getStatusNear(row, col, side, -1, -1));
	res.append(getStatusNear(row, col, side, 1, -1));

	return res;
}

QList<ChessBoard::Field> ChessBoard::getHorVerStatus(int row, int col, PlayerSide side)
{
	QList<Field> res;
	res.append(getStatusNear(row, col, side, 1, 0));
	res.append(getStatusNear(row, col, side, -1, 0));
	res.append(getStatusNear(row, col, side, 0, 1));
	res.append(getStatusNear(row, col, side, 0, -1));

	return res;
}

QList<ChessBoard::Field> ChessBoard::getStatusNear(int row, int col, PlayerSide side, int d_r, int d_c)
{
	QList<Field> res;
	Status st;
	row += d_r;
	col += d_c;

	while ((st = getFieldStatus(row, col, side)) != DISABLE)
	{
		res.append(Field(row, col, st));
		if (st == ATTACK)
			break;
		row += d_r;
		col += d_c;
	}
	return res;
}

void ChessBoard::fillFieldsUnderAttack(PlayerSide side)
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

							if (getFieldStatus(row + r, col + c, side) == ENABLE)
								fields_under_attack_list.append(Field(row + r, col + c, ATTACK));
						}
				}
				else if (figure->Type() == PAWN)
				{
					int near_row = row - 1; // for black
					if(side == WHITE)
						near_row = row + 1;

					if (getFieldStatus(near_row, col + 1, side) == ENABLE)
						fields_under_attack_list.append(Field (near_row, col + 1, ATTACK));

					if (getFieldStatus(near_row, col - 1, side) == ENABLE)
						fields_under_attack_list.append(Field (near_row, col - 1, ATTACK));
				}
				else
					fields_under_attack_list.append(getAvalibleTurnes(figure, row, col));
			}
		}
	}

	qDebug() << "here";
	foreach (Field f, fields_under_attack_list)
	{
		fieldsUnderAttack[f.row][f.col] = true;
	}
}

void ChessBoard::loadFromeFile(QString fileName)
{
	QFile f(fileName);

	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("File read error"), tr("Cannot read from file %1 Error: %2")
				.arg(f.fileName()).arg(f.errorString()));

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

			Figure* figure = new Figure(this, side, type, row, col);
			boardMap[row][col] = figure;

			moveFigure(figure, row, col);
		}
	}
}

void ChessBoard::loadFromResources(QString fileName)
{
	qDebug() << "Load from file" << fileName;

	QResource res(fileName);
	loadFromeFile(res.absoluteFilePath());
}

FigureType ChessBoard::char2figureType(QChar c)
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

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), AbstractBoard(),
	  selectedFigure(NULL)
{
	setAcceptDrops(true);
	clearAvailableMoves();
}


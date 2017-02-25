#include <QtGui>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

#include "chessboard.h"

const char* mimeFormat = "application/x-ks_chessfigure";
const QBrush ChessBoard::disabledColors[] = { QBrush(QColor("#f0d9b5")), QBrush(QColor("#b58863")) } ;
const QBrush ChessBoard::enabledColors[] = { QBrush(Qt::yellow), QBrush(Qt::darkYellow) } ;
const QBrush ChessBoard::underAttackColors[] = { QBrush(Qt::blue), QBrush(Qt::darkBlue) } ;
const QBrush ChessBoard::attackColors[] = { QBrush(Qt::red), QBrush(Qt::darkRed) } ;

void ChessBoard::boardLabel(QPainter &p, char c, int x, int y, int width)
{
	QRect rect = QRect(x, y, width, LABEL_HEIGHT);
	p.drawText(rect, Qt::AlignCenter, QString(c));
}

void ChessBoard::updateGeometry()
{
	QRect g = this->geometry();

	boardRect = QRectF (0, 0, g.width(), g.height());
	boardRect = boardRect.marginsRemoved(QMargins(LABEL_WIDTH, LABEL_HEIGHT, LABEL_WIDTH, LABEL_HEIGHT));

	qreal delta = (boardRect.width() - boardRect.height()) / 2.0;
	if (delta > 0.0)
		boardRect.adjust(delta, 0.0, -delta, 0.0);
	else
		boardRect.adjust(0.0, -delta, 0.0, delta);

	cellSize = boardRect.width() / ROWS;
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
	return QRectF(boardRect.x() + col * cellSize,
			boardRect.y() + row * cellSize,
				  cellSize, cellSize);
}

QPointF ChessBoard::getCellCenter(int row, int col)
{
	return cellGeometry(row, col).center();
}

void ChessBoard::dragEnterEvent(QDragEnterEvent* event)
{
//	if (!event->mimeData()->hasFormat(mimeFormat)) {
//		event->ignore();
//		return;
//	}

//	if (event->source() == this)
//	{
//		event->setDropAction(Qt::MoveAction);
//		event->accept();
//	}
//	else
//		event->acceptProposedAction();
}

void ChessBoard::dragMoveEvent(QDragMoveEvent* event)
{
//	if (!event->mimeData()->hasFormat(mimeFormat))
//	{
//		event->ignore();
//		return;
//	}

//	if (event->source() == this)
//	{
//		event->setDropAction(Qt::MoveAction);
//		event->accept();
//	}
//	else
//		event->acceptProposedAction();
}

void ChessBoard::dropEvent(QDropEvent* event)
{
//	if (!event->mimeData()->hasFormat(mimeFormat) || selectedFigure == NULL)
//	{
//		event->ignore();
//		return;
//	}

//	QPoint offset;

//	// Move figure
//	QPoint p = event->pos() - offset;
//	int tmpRow, tmpCol;
//	tmpRow = selectedFigure->Row();
//	tmpCol = selectedFigure->Col();
//	selectedFigure->setRow((p.y() + cell_size / 2) / cell_size);
//	selectedFigure->setCol((p.x() + cell_size / 2) / cell_size);
//	selectedFigure->move(tmpCol * cell_size, tmpRow * cell_size);
//	selectedFigure->show();


//	if (event->source() == this)
//	{
//		boardMap[tmpRow][tmpCol] = NULL;

//		boardMap[selectedFigure->Row()][selectedFigure->Col()] = selectedFigure;

//		event->setDropAction(Qt::MoveAction);
//		event->accept();
//	} else
//		event->acceptProposedAction();
}

void ChessBoard::startDragEvent(const QPointF &pos, Figure* figure)
{
//	QPixmap pixmap = figure->pixmap();

//	QByteArray itemData;
//	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
//	dataStream << pixmap << QPointF(pos - figure->pos());

//	QMimeData* mimeData = new QMimeData;
//	mimeData->setData(mimeFormat, itemData);

//	QDrag* drag = new QDrag(this);
//	drag->setMimeData(mimeData);
//	drag->setPixmap(pixmap);
//	drag->setHotSpot(pos.toPoint() - figure->pos());

//	QPixmap tempPixmap = pixmap;
//	QPainter painter;
//	painter.begin(&tempPixmap);
//	painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
//	painter.end();

//	figure->setPixmap(tempPixmap);

//	if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
//	{
//		;//piece->close();
//	} else {
//		figure->show();
//		figure->setPixmap(pixmap);
//	}
}

void ChessBoard::mousePressEvent(QMouseEvent* event)
{
	QPointF pos = event->pos();
	if (!boardRect.contains(pos))
		return;

	pos -= boardRect.topLeft();
	int row = pos.y() / cellSize;
	int col = pos.x() / cellSize;

	selectCell(row, col);

	update();

	//startDragEvent(event->localPos(), figure);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
	updateGeometry();

	QPainter p(this);

	redrawBoardLabels(p);

	for(int row = 0; row < ROWS; ++row)
	{
		for(int col = 0; col < COLS; ++col)
		{
			switch( avalibleMoveStatus(row, col))
			{
			case MoveStatus::CORRECT_MOVE:
				p.setBrush(enabledColors[(row + col) % 2]);
				break;
			case MoveStatus::INCORRECT_MOVE:
				p.setBrush(disabledColors[(row + col) % 2]);
				break;
			case MoveStatus::ATTACK_MOVE:
				p.setBrush(attackColors[(row + col) % 2]);
				break;
			}

			if (getSelectedFigureType() == KING && isFieldUnderAttack(row,col))
				p.setBrush(underAttackColors[(row + col) % 2]);

			QRectF cell = cellGeometry(row, col);
			p.drawRect(cell);

			Figure * figure = (Figure *)figureAt(row, col);
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
		int x = col * cellSize + boardRect.left();
		boardLabel(p, L, x, boardRect.top() - LABEL_HEIGHT, cellSize);
		boardLabel(p, L, x, boardRect.bottom(), cellSize);
	}
	for (int row = 0; row < ROWS; ++row)
	{
		char D = '1' + row;
		int y = boardRect.bottom() - row * cellSize - (cellSize + LABEL_HEIGHT) / 2;
		boardLabel(p, D, boardRect.left() - LABEL_WIDTH, y, LABEL_WIDTH);
		boardLabel(p, D, boardRect.right(), y, LABEL_WIDTH);
	}
}

void ChessBoard::initFigure(PlayerSide side, FigureType type, int row, int col)
{
	Figure* figure = new Figure(this, side, type, row, col);
	addFigureAt(figure, row, col);
	moveFigure(figure, row, col);
}

void ChessBoard::figureSelectedAt(int row, int col)
{
	Figure *figure = (Figure *)figureAt(row, col);
	if (figure)
	{
		update();
	}
}

void ChessBoard::figureMovedFromTo(int src_row, int src_col, int dst_row, int dst_col, AbstractFigure *attacked_figure)
{
	Figure *figure = (Figure *)figureAt(src_row, src_col);
	if (figure)
	{
		QString from = figure->posStr();
		QString to = Figure::pos2Str(dst_row, dst_col);
		bool will_eat = (attacked_figure != NULL);
		emit historyUpdated(QString("%1: %2%4%3")
						  .arg((figure->Side() == WHITE)?tr("White"):tr("Black"))
						  .arg(from).arg(to).arg(will_eat? tr("x"):"—"));

		moveFigure(figure, dst_row, dst_col);
		update();
	}
}

void ChessBoard::hideFigureAt(int row, int col)
{
	Figure *figure = (Figure *)figureAt(row, col);
	if (figure)
	{
		figure->hide();
	}
}

void ChessBoard::figureDeselected()
{
	update();
}

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent),
	  boardRect(QRectF(0.0, 0.0, 80.0, 80.0)), cellSize(10.0),
	  CommonRulesChess()
{
	setAcceptDrops(true);
}


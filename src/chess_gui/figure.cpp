#include <QDebug>
#include "figure.h"

QString Figure::resPath(const QString &ext)
{
	QString s = playerSide2str(Side()).toLower();
	QString t = figureType2str(Type()).toLower();
	return QString(":/res/%1-%2%3").arg(t).arg(s).arg(ext);
}

void Figure::initPixmap() // deprecated
{
	setPixmap(QPixmap(resPath()));
}

void Figure::initPixmap(const QSize &size)
{
    QSize checked_size = size; // caused by problems with null size on android init
    if (size.isNull())
        checked_size = QSize(1, 1);
	setPixmap(QIcon(resPath(".svg")).pixmap(checked_size));
}

void Figure::resizeEvent(QResizeEvent *event)
{
	QSize new_size = event->size();
    initPixmap(new_size);
}

void Figure::paintEvent(QPaintEvent *)
{
    QPainter p (this);
    if (!pixmap_.isNull())
        p.drawPixmap(QPoint(0,0), pixmap_);
}

Figure::Figure(QWidget* parent, PlayerSide side, FigureType type, int row, int col)
	: QWidget((QWidget *)parent), AbstractFigure(side, type, row, col)
{
	initPixmap(size());
	setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("background:transparent;");
	setCursor(Qt::PointingHandCursor);
}

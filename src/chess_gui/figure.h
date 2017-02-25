#ifndef FIGURE_H
#define FIGURE_H

#include <QDebug>
#include <QWidget>
#include <QMetaEnum>
#include <QStaticAssertFailure>
#include <QResizeEvent>
#include <QIcon>
#include <QVector>
#include <QPainter>

#include "../chess/abstract_figure.h"

class Figure : public QWidget, public AbstractFigure
{
	Q_OBJECT

private:
    QPixmap pixmap_;

	// Used for convert figure params to image resources
	QString resPath(const QString &ext = ".png");
	void initPixmap();
	void initPixmap(const QSize &size);

protected:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *);

public:
	QPixmap pixmap() const { return pixmap_; }
	void setPixmap(const QPixmap p) { pixmap_ = p; }

	Figure(QWidget *parent, PlayerSide side, FigureType type, int row, int col );
};

#endif // FIGURE_H

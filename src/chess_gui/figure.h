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
	Q_PROPERTY(int row READ Row WRITE setRow)
	Q_PROPERTY(int col READ Col WRITE setCol)

public slots:
	void setRow(int row) { m_row = row; }
	void setCol(int col) { m_col = col; }

private:
	int m_row;
	int m_col;
    QPixmap pixmap_;

	// Used for convert figure params to image resources
	QString playerSide2str (PlayerSide side) const;
	QString figureType2str (FigureType type) const;
	QString resPath(const QString &ext = ".png");
	void initPixmap();
	void initPixmap(const QSize &size);
    

protected:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *);
	friend QDebug operator<<(QDebug debug, const Figure &f);

public:
	int Row() const { return m_row; }
	int Col() const { return m_col; }
	QString posStr() const { return pos2Str(Row(), Col());}
	static QString pos2Str(int r, int c) { return QString("%1%2").arg(QChar('a'+c)).arg(r);}
    QPixmap pixmap() const { return pixmap_; }
    void setPixmap(const QPixmap p) { pixmap_ = p; }

	Figure(QWidget *parent, PlayerSide side, FigureType type, int row, int col );
};

#endif // FIGURE_H

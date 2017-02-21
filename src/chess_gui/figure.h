#ifndef FIGURE_H
#define FIGURE_H

#include <QLabel>
#include <QMetaEnum>
#include <QStaticAssertFailure>
#include <QResizeEvent>
#include <QIcon>
#include <QVector>

#include "../chess/abstract_figure.h"

class Figure : public QLabel, public AbstractFigure
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

	// Used for convert figure params to image resources
	QString playerSide2str(PlayerSide side);
	QString figureType2str(FigureType type);
	QString resPath(const QString &ext = ".png");
	void initPixmap();
	void initPixmap(const QSize &size);

protected:
	void resizeEvent(QResizeEvent *event);

public:
	int Row() const { return m_row; }
	int Col() const { return m_col; }

	Figure(QWidget *parent, PlayerSide side, FigureType type, int row, int col );
};

#endif // FIGURE_H

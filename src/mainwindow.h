#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public slots:
	void addStringToHistory(const QString &history);

private slots:
	void on_exitAction_triggered();

private:
	void setupUI();
	void loadConfig();

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

protected:
	Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H

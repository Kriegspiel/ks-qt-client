#include <QPainter>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common/config.h"
#include "chess_gui/chessboard.h"

void MainWindow::addStringToHistory(const QString &history)
{
	ui->lastMoves->addItem(QString("%1. %2").arg(ui->lastMoves->count()).arg(history));
}

void MainWindow::on_exitAction_triggered()
{
	close();
}

void MainWindow::setupUI()
{
	ui->setupUi(this);
	setWindowTitle(Config::ProgramName);
	ui->versionLabel->setText("ver. " + Config::Version);
	ui->centralWidget->setStyleSheet("background-image: url(:/res/wooden-texture-bg.jpeg)");

	ui->historyLabel->setStyleSheet("background: transparent");
	ui->versionLabel->setStyleSheet("background: transparent");
	ui->lastMoves->setStyleSheet("background: white");

	connect(ui->chessBoard, SIGNAL(historyUpdated(QString)), this, SLOT(addStringToHistory(QString)));
}

void MainWindow::loadConfig()
{
	ui->chessBoard->loadFromResources(":/res/board.txt");
}

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	setupUI();
	loadConfig();
}

MainWindow::~MainWindow()
{
	delete ui;
}

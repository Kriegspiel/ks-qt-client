QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

TARGET = ks_client
TEMPLATE = app

MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui

TRANSLATIONS = i18n/ks_ru.ts


SOURCES += main.cpp\
	src/mainwindow.cpp \
	src/chess/abstract_board.cpp \
	src/chess/abstract_figure.cpp \
	src/chess_gui/figure.cpp \
	src/chess_gui/chessboard.cpp

HEADERS	+= src/mainwindow.h \
	src/common/config.h \
	src/chess/rules.h \
	src/chess/abstract_board.h \
	src/chess/abstract_figure.h \
	src/chess_gui/chessboard.h \
	src/chess_gui/figure.h \
	src/chess_gui/chessboard.h

FORMS += src/mainwindow.ui

RESOURCES += \
	resources.qrc

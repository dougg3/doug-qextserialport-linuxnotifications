TEMPLATE = app
DEPENDPATH += .
QT += core gui widgets extserialport

HEADERS += MainWindow.h \
		MessageWindow.h \
        QespTest.h

SOURCES += main.cpp \
		MainWindow.cpp \
		MessageWindow.cpp \
		QespTest.cpp

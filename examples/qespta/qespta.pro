TEMPLATE = app
DEPENDPATH += .

HEADERS += MainWindow.h \
		MessageWindow.h \
        QespTest.h

SOURCES += main.cpp \
		MainWindow.cpp \
		MessageWindow.cpp \
		QespTest.cpp

include(../../src/qextserialport.pri)

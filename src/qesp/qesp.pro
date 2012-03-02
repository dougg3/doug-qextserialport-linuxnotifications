load(qt_module)
TARGET     = QextSerialPort
QPRO_PWD   = $$PWD
CONFIG    += module
MODULE_PRI = ../../modules/qt_qesp.pri

QT = core

DEFINES += QEXTSERIALPORT_BUILD_SHARED QT_MAKEDLL
load(qt_module_config)

HEADERS                += qespversion.h

HEADERS                += qextserialport.h \
                          qextserialport_p.h \
                          qextserialenumerator.h \
                          qextserialenumerator_p.h \
                          qextserialport_global.h
SOURCES                += qextserialport.cpp \
                          qextserialenumerator.cpp
unix:SOURCES           += qextserialport_unix.cpp
unix:!macx:SOURCES     += qextserialenumerator_unix.cpp
macx:SOURCES           += qextserialenumerator_osx.cpp
win32:SOURCES          += qextserialport_win.cpp \
                          qextserialenumerator_win.cpp

macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32

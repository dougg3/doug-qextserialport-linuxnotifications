load(qt_module)

#Must be the same as that specified in modules/*.pri file.
TARGET     = $$QT.extserialport.name

QPRO_PWD   = $$PWD
CONFIG    += module
MODULE_PRI = ../../modules/qt_extserialport.pri

DEFINES += QEXTSERIALPORT_BUILD_SHARED QT_MAKEDLL
load(qt_module_config)

#Version must be add or Qt's Version will be used. This is a bug in qt_module_config.prf ?
VERSION = $$QT.extserialport.VERSION

HEADERS                += qextserialport.h \
                          qextserialport_p.h \
                          qextserialenumerator.h \
                          qextserialenumerator_p.h \
                          qextserialport_global.h \
                          qtextserialportversion.h \
                          qextserialhelper.h

SOURCES                += qextserialport.cpp \
                          qextserialenumerator.cpp \
                          qextserialhelper.cpp

unix:SOURCES           += qextserialport_unix.cpp
unix:!macx:SOURCES     += qextserialenumerator_unix.cpp
macx:SOURCES           += qextserialenumerator_osx.cpp
win32:SOURCES          += qextserialport_win.cpp \
                          qextserialenumerator_win.cpp

macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32

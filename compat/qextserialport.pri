infile($$PWD/config.pri, QEXTSERIALPORT_LIBRARY, yes): CONFIG += qextserialport-library
qextserialport-library{
    infile(config.pri, QEXTSERIALPORT_STATIC, yes): CONFIG += qextserialport-static
}

SAVE_TEMPLATE = $$TEMPLATE
TEMPLATE = fakelib
contains(QT_VERSION, ^5\\..*\\..*) {
    #different name for Qt4 and Qt5
    QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(QtExtSerialPort-2.0)
} else {
    QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(qextserialport-2.0)
}
TEMPLATE = $$SAVE_TEMPLATE

QEXTSERIALPORT_LIBDIR = $$PWD/lib
unix:qextserialport-library:!qextserialport-buildlib:QMAKE_RPATHDIR += $$QEXTSERIALPORT_LIBDIR

QESP_SRCDIR = $$PWD/../src/extserialport

INCLUDEPATH += $$QESP_SRCDIR
DEPENDPATH += $$QESP_SRCDIR

qextserialport-library:!qextserialport-buildlib {
    # Using QextSerialPort as shared or static library.
    LIBS += -L$$QEXTSERIALPORT_LIBDIR -l$$QEXTSERIALPORT_LIBNAME
   !qextserialport-static: DEFINES += QEXTSERIALPORT_USING_SHARED
} else {
    # Building library(shared or static)
    # or including source files
    HEADERS                += $$QESP_SRCDIR/qextserialport.h \
                              $$QESP_SRCDIR/qextserialport_p.h \
                              $$QESP_SRCDIR/qextserialenumerator.h \
                              $$QESP_SRCDIR/qextserialenumerator_p.h \
                              $$QESP_SRCDIR/qextserialhelper.h \
                              $$QESP_SRCDIR/qextserialhelper_p.h \
                              $$QESP_SRCDIR/qextserialport_global.h
    SOURCES                += $$QESP_SRCDIR/qextserialport.cpp \
                              $$QESP_SRCDIR/qextserialenumerator.cpp \
                              $$QESP_SRCDIR/qextserialhelper.cpp
    unix:SOURCES           += $$QESP_SRCDIR/qextserialport_unix.cpp
    unix:!macx:SOURCES     += $$QESP_SRCDIR/qextserialenumerator_unix.cpp
    macx:SOURCES           += $$QESP_SRCDIR/qextserialenumerator_osx.cpp
    win32:SOURCES          += $$QESP_SRCDIR/qextserialport_win.cpp \
                              $$QESP_SRCDIR/qextserialenumerator_win.cpp

    # For building shared library only
    qextserialport-buildlib:contains(TEMPLATE, .*lib):contains(CONFIG, shared){
        DEFINES += QEXTSERIALPORT_BUILD_SHARED
    }
}

macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32

DEFINES += QEXTSERIALPORT_COMPAT

TARGET  = qml_extserialport
TARGETPATH = QtExtSerialPort

include(qextserialportimport.pri)
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
DESTDIR = $$QT.extserialport.imports/$$TARGETPATH
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += qmldir

QT += qml extserialport

# Input
HEADERS += qextqmlserialport_p.h \

SOURCES += plugin.cpp \
           qextqmlserialport.cpp \

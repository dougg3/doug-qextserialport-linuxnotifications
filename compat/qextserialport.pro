TEMPLATE=subdirs
CONFIG += ordered
infile($$PWD/config.pri, QEXTSERIALPORT_LIBRARY, yes)::SUBDIRS=buildlib
SUBDIRS+=examples

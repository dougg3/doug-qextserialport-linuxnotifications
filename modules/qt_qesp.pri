QT.qesp.VERSION = 1.9.0
QT.qesp.MAJOR_VERSION = 1
QT.qesp.MINOR_VERSION = 9
QT.qesp.PATCH_VERSION = 0

QT.qesp.name = QextSerialPort
QT.qesp.bins = $$QT_MODULE_BIN_BASE
QT.qesp.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/qesp
QT.qesp.private_includes = $$QT_MODULE_INCLUDE_BASE/QeSP/$$QT.qesp.VERSION
QT.qesp.sources = $$QT_MODULE_BASE/src/qesp
QT.qesp.libs = $$QT_MODULE_LIB_BASE
QT.qesp.plugins = $$QT_MODULE_PLUGIN_BASE
QT.qesp.imports = $$QT_MODULE_IMPORT_BASE
QT.qesp.depends = core
QT.qesp.DEFINES = QT_QESP

QT_CONFIG += qesp

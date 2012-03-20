QT.extserialport.VERSION = 2.0.0
QT.extserialport.MAJOR_VERSION = 2
QT.extserialport.MINOR_VERSION = 0
QT.extserialport.PATCH_VERSION = 0

QT.extserialport.name = QextSerialPort
QT.extserialport.bins = $$QT_MODULE_BIN_BASE
QT.extserialport.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/ExtSerialPort
QT.extserialport.private_includes = $$QT_MODULE_INCLUDE_BASE/ExtSerialPort/$$QT.extserialport.VERSION
QT.extserialport.sources = $$QT_MODULE_BASE/src/extserialport
QT.extserialport.libs = $$QT_MODULE_LIB_BASE
QT.extserialport.plugins = $$QT_MODULE_PLUGIN_BASE
QT.extserialport.imports = $$QT_MODULE_IMPORT_BASE
QT.extserialport.depends = core
QT.extserialport.DEFINES = QT_EXTSERIALPORT_LIB

QT_CONFIG += extserialport

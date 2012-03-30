%modules = ( # path to module name map
    "QtExtSerialPort" => "$basedir/src/extserialport",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
    "qtextserialportversion.h" => "QtExtSerialPortVersion"
);
%mastercontent = (
    "core" => "#include <QtCore/QtCore>\n",
);
%modulepris = (
    "QtExtSerialPort" => "$basedir/modules/qt_extserialport.pri",
);

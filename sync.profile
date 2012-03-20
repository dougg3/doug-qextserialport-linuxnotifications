%modules = ( # path to module name map
    "ExtSerialPort" => "$basedir/src/extserialport",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
);
%mastercontent = (
    "core" => "#include <QtCore/QtCore>\n",
);
%modulepris = (
    "ExtSerialPort" => "$basedir/modules/qt_extserialport.pri",
);

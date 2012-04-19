/****************************************************************************
** Copyright (c) 2012 Debao Zhang
** All right reserved.
** Web: http://code.google.com/p/qextserialport/
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#ifndef QEXTSERIALHELPER_H
#define QEXTSERIALHELPER_H

#include "qextserialport_global.h"
#include "qextserialport.h"
#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class QEXTSERIALPORT_EXPORT QextSerialHelper : public QObject
{
    Q_OBJECT
public:
    
    static QStringList getPortNameList();
    static QStringList getBaudRateList();
    static QStringList getDataBitsList();
    static QStringList getParityList();
    static QStringList getStopBitsList();
    static QStringList getFlowList();

    static QString fromBaudRateType(QextSerialPort::BaudRateType, bool *ok=0);
    static QString fromDataBitsType(QextSerialPort::DataBitsType, bool *ok=0);
    static QString fromParityType(QextSerialPort::ParityType, bool *ok=0);
    static QString fromStopBitsType(QextSerialPort::StopBitsType, bool *ok=0);
    static QString fromFlowType(QextSerialPort::FlowType, bool *ok=0);

    static QextSerialPort::BaudRateType toBaudRateType(const QString &, bool *ok=0);
    static QextSerialPort::DataBitsType toDataBitsType(const QString &, bool *ok=0);
    static QextSerialPort::ParityType toParityType(const QString &, bool *ok=0);
    static QextSerialPort::StopBitsType toStopBitsType(const QString &, bool *ok=0);
    static QextSerialPort::FlowType toFlowType(const QString &, bool *ok=0);

Q_SIGNALS:
    
public Q_SLOTS:

private:
    explicit QextSerialHelper(QObject *parent = 0);

};

#endif // QEXTSERIALHELPER_H

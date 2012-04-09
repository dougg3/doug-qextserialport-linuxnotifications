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

#include "qextserialhelper.h"
#include "qextserialenumerator.h"
#include <QtCore/QStringList>
#include <QtCore/QMetaEnum>
#include <QtCore/QDebug>


static QStringList getEnumKeyList(const char *type)
{
    int index = QextSerialPort::staticMetaObject.indexOfEnumerator(type);
    QMetaEnum metaEnum = QextSerialPort::staticMetaObject.enumerator(index);
    QStringList values;
    for (int i=0; i<metaEnum.keyCount(); ++i)
    {
        values << QString::fromLatin1(metaEnum.key(i));
    }
    return values;
}

static const char * valueToKey(const char *type, int value)
{
    int index = QextSerialPort::staticMetaObject.indexOfEnumerator(type);
    QMetaEnum metaEnum = QextSerialPort::staticMetaObject.enumerator(index);
    return metaEnum.valueToKey(value);
}

static int keyToValue(const char *type, const QString &key)
{
    int index = QextSerialPort::staticMetaObject.indexOfEnumerator(type);
    QMetaEnum metaEnum = QextSerialPort::staticMetaObject.enumerator(index);
    return metaEnum.keyToValue(key.toLatin1().constData());

}

/*! \class QextSerialHelper

    \brief The QextSerialHelper class provides some convenient functions to ease the use of QextSerialPort.
*/

QextSerialHelper::QextSerialHelper(QObject *parent) :
    QObject(parent)
{
}

/*!
    Returns available port names.
*/
QStringList QextSerialHelper::getPortNameList()
{
    QStringList names;
    Q_FOREACH (QextPortInfo info, QextSerialEnumerator::getPorts())
        names << info.portName;

    return names;
}

/*!
    Returns available baudrate names.
*/
QStringList QextSerialHelper::getBaudRateList()
{
    return getEnumKeyList("BaudRateType");
}

/*!
    Returns available databits names.
*/
QStringList QextSerialHelper::getDataBitsList()
{
    return getEnumKeyList("DataBitsType");
}

/*!
    Returns available parity names.
*/
QStringList QextSerialHelper::getParityList()
{
    return getEnumKeyList("ParityType");
}


/*!
    Returns available stopbits names.
*/
QStringList QextSerialHelper::getStopBitsList()
{
    return getEnumKeyList("StopBitsType");
}

/*!
    Returns available flowcontrol names.
*/
QStringList QextSerialHelper::getFlowList()
{
    return getEnumKeyList("FlowType");
}

/*!
    Returns the string representation of QextSerialPort::BaudRateType \a value.
    Returns a default string "BAUD9600" if the conversion fails, for example,
    (QextSerialPort::BaudRateType)9999 is passed as parameter.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa toBaudRateType()
*/
QString QextSerialHelper::fromBaudRateType(QextSerialPort::BaudRateType value, bool *ok)
{
    const char * key = valueToKey("BaudRateType", value);
    if (ok)
        *ok = !!key;
    if (!key) {
        QESP_WARNING()<<"Invalid BaudRateType: "<<value;
        key = valueToKey("BaudRateType", QextSerialPort::BAUD9600);
    }
    return QString::fromLatin1(key);
}

/*!
    Returns the string representation of QextSerialPort::DataBitsType \a value.
    Returns a default string "DATA_8" if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa toDataBitsType()
*/
QString QextSerialHelper::fromDataBitsType(QextSerialPort::DataBitsType value, bool *ok)
{
    const char * key = valueToKey("DataBitsType", value);
    if (ok)
        *ok = !!key;
    if (!key) {
        QESP_WARNING()<<"Invalid DataBitsType: "<<value;
        key = valueToKey("DataBitsType", QextSerialPort::DATA_8);
    }
    return QString::fromLatin1(key);
}

/*!
    Returns the string representation of QextSerialPort::ParityType \a value.
    Returns a default string "PAR_NONE" if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa toParityType()
*/
QString QextSerialHelper::fromParityType(QextSerialPort::ParityType value, bool *ok)
{
    const char * key = valueToKey("ParityType", value);
    if (ok)
        *ok = !!key;
    if (!key) {
        QESP_WARNING()<<"Invalid ParityType: "<<value;
        key = valueToKey("ParityType", QextSerialPort::PAR_NONE);
    }
    return QString::fromLatin1(key);
}

/*!
    Returns the string representation of QextSerialPort::StopBitsType \a value.
    Returns a default string "STOP_1" if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa toStopBitsType()
*/
QString QextSerialHelper::fromStopBitsType(QextSerialPort::StopBitsType value, bool *ok)
{
    const char * key = valueToKey("StopBitsType", value);
    if (ok)
        *ok = !!key;
    if (!key) {
        QESP_WARNING()<<"Invalid StopBitsType: "<<value;
        key = valueToKey("StopBitsType", QextSerialPort::STOP_1);
    }
    return QString::fromLatin1(key);
}

/*!
    Returns the string representation of QextSerialPort::FlowType \a value.
    Returns a default string "FLOW_OFF" if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa toFlowType()
*/
QString QextSerialHelper::fromFlowType(QextSerialPort::FlowType value, bool *ok)
{
    const char * key = valueToKey("FlowType", value);
    if (ok)
        *ok = !!key;
    if (!key) {
        QESP_WARNING()<<"Invalid FlowType: "<<value;
        key = valueToKey("FlowType", QextSerialPort::FLOW_OFF);
    }
    return QString::fromLatin1(key);
}

/*!
    Converts the string \a key to QextSerialPort::BaudRateType value.
    Returns a default value QextSerialPort::BAUD9600 if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa fromBaudRateType()
*/
QextSerialPort::BaudRateType QextSerialHelper::toBaudRateType(const QString &key, bool *ok)
{
    int value = keyToValue("BaudRateType", key);
    if (ok)
        *ok = value != -1;
    if (value == -1) {
        QESP_WARNING()<<"Invalid BaudRateType Key: "<<key;
        return QextSerialPort::BAUD9600;
    }
    return (QextSerialPort::BaudRateType)(value);
}

/*!
    Converts the string \a key to QextSerialPort::DataBitsType value.
    Returns a default value QextSerialPort::DATA_8 if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa fromDataBitsType()
*/
QextSerialPort::DataBitsType QextSerialHelper::toDataBitsType(const QString &key, bool *ok)
{
    int value = keyToValue("DataBitsType", key);
    if (ok)
        *ok = value != -1;
    if (value == -1) {
        QESP_WARNING()<<"Invalid DataBitsType Key: "<<key;
        return QextSerialPort::DATA_8;
    }
    return (QextSerialPort::DataBitsType)(value);
}

/*!
    Converts the string \a key to QextSerialPort::ParityType value.
    Returns a default value QextSerialPort::PAR_NONE if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa fromParityType()
*/
QextSerialPort::ParityType QextSerialHelper::toParityType(const QString &key, bool *ok)
{
    int value = keyToValue("ParityType", key);
    if (ok)
        *ok = value != -1;
    if (value == -1) {
        QESP_WARNING()<<"Invalid ParityType Key: "<<key;
        return QextSerialPort::PAR_NONE;
    }
    return (QextSerialPort::ParityType)(value);
}

/*!
    Converts the string \a key to QextSerialPort::StopBitsType value.
    Returns a default value QextSerialPort::STOP_1 if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa fromStopBitsType()
*/
QextSerialPort::StopBitsType QextSerialHelper::toStopBitsType(const QString &key, bool *ok)
{
    int value = keyToValue("StopBitsType", key);
    if (ok)
        *ok = value != -1;
    if (value == -1) {
        QESP_WARNING()<<"Invalid StopBitsType Key: "<<key;
        return QextSerialPort::STOP_1;
    }
    return (QextSerialPort::StopBitsType)(value);
}

/*!
    Converts the string \a key to QextSerialPort::FlowType value.
    Returns a default value QextSerialPort::FLOW_OFF if the conversion fails.

    If a conversion error occurs, * \a ok is set to false; otherwise * \a ok is set to true.

    \sa fromFlowType()
*/
QextSerialPort::FlowType QextSerialHelper::toFlowType(const QString &key, bool *ok)
{
    int value = keyToValue("FlowType", key);
    if (ok)
        *ok = value != -1;
    if (value == -1) {
        QESP_WARNING()<<"Invalid FlowType Key: "<<key;
        return QextSerialPort::FLOW_OFF;
    }
    return (QextSerialPort::FlowType)(value);
}

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

#include "qextqmlserialport_p.h"
#include "qextserialport.h"
#include "qextserialhelper.h"

class QextQmlSerialPortPrivate
{
    Q_DECLARE_PUBLIC(QextQmlSerialPort)
public:
    QextQmlSerialPortPrivate(QextQmlSerialPort *q)
        :componentComplete(false), needConnect(false), q_ptr(q)
    {
    }

    void doConnect(bool connect)
    {
        if (connect && !port->isOpen()) {
            port->open(QIODevice::ReadWrite);
            if (port->isOpen())
                Q_EMIT q_ptr->connectedChanged();
        } else if (!connect && port->isOpen()) {
            port->close();
            Q_EMIT q_ptr->connectedChanged();
        }
    }

    bool componentComplete;
    bool needConnect; //before componentComplete
    QextSerialPort * port;
    QextQmlSerialPort * q_ptr;
};

/*!
   \qmlclass ExtSerialPort QextQmlSerialPort
   \brief Enables you to connect and communicate with a serial port.

   \sa QextSerialPort
*/
QextQmlSerialPort::QextQmlSerialPort(QObject *parent) :
    QObject(parent), d_ptr(new QextQmlSerialPortPrivate(this))
{
    d_ptr->port = new QextSerialPort();

    connect(d_ptr->port, SIGNAL(readyRead()), SIGNAL(dataAvailable()));
}

QextQmlSerialPort::~QextQmlSerialPort()
{
    delete d_ptr->port;
}

void QextQmlSerialPort::componentComplete()
{
    d_ptr->componentComplete = true;
    d_ptr->doConnect(d_ptr->needConnect);
}

/*!
  \qmlproperty bool ExtSerialPort::connected

  This property holds the device state of the port. If the port is opened,
  it returns true. It can be set true or false to control the port.
*/
bool QextQmlSerialPort::connected()
{
    return d_ptr->port->isOpen();
}

/*!
  \qmlproperty string ExtSerialPort::stringData

  This property receives or sends data to serial port device. Arrival of
  data is signaled by the dataAvailable signal and can be read by
  stringData. Calling sendStringData will transmit the string.
*/
QString QextQmlSerialPort::stringData()
{
    if (d_ptr->port->isOpen() && d_ptr->port->bytesAvailable() > 0) {
        return QString::fromLatin1(d_ptr->port->readAll());
    } else {
        return QString();
    }
}

/*!
  \qmlproperty string ExtSerialPort::stringCodec

  This property controls which QTextCodec is used to convert QByteArray to and from
  QString.
*/
QString QextQmlSerialPort::stringCodec()
{
    return QString::fromLatin1("latin1");
}

/*!
  \qmlproperty string ExtSerialPort::portName

   Gets and sets the baudRate associated with the serial port
*/
QString QextQmlSerialPort::portName()
{
    return d_ptr->port->portName();
}


/*!
  \qmlproperty string ExtSerialPort::baudRate

   Gets and sets the baudRate associated with the serial port
*/
QString QextQmlSerialPort::baudRate()
{
    return QextSerialHelper::fromBaudRateType(d_ptr->port->baudRate());
}

/*!
  \qmlproperty string ExtSerialPort::dataBits

   Gets and sets the dataBits associated with the serial port
*/
QString QextQmlSerialPort::dataBits()
{
    return QextSerialHelper::fromDataBitsType(d_ptr->port->dataBits());
}

/*!
  \qmlproperty string ExtSerialPort::parity

   Gets and sets the parity associated with the serial port
*/
QString QextQmlSerialPort::parity()
{
    return QextSerialHelper::fromParityType(d_ptr->port->parity());
}

/*!
  \qmlproperty string ExtSerialPort::stopBits

   Gets and sets the stopBits associated with the serial port
*/
QString QextQmlSerialPort::stopBits()
{
    return QextSerialHelper::fromStopBitsType(d_ptr->port->stopBits());
}

/*!
  \qmlproperty string ExtSerialPort::flowControl

   Gets and sets the flowControl associated with the serial port
*/
QString QextQmlSerialPort::flowControl()
{
    return QextSerialHelper::fromFlowType(d_ptr->port->flowControl());
}

void QextQmlSerialPort::setConnected(bool connect)
{
    if (d_ptr->componentComplete) {
        d_ptr->doConnect(connect);
    } else {
        //delay the operation
        d_ptr->needConnect = connect;
    }
}

void QextQmlSerialPort::sendStringData(QString data)
{
    d_ptr->port->write(data.toLatin1());
}

void QextQmlSerialPort::setStringCodec(QString /*codec*/)
{

}

void QextQmlSerialPort::setPortName(QString portName)
{
    d_ptr->port->setPortName(portName);
}

void QextQmlSerialPort::setBaudRate(QString baudrate)
{
    d_ptr->port->setBaudRate(QextSerialHelper::toBaudRateType(baudrate));
}

void QextQmlSerialPort::setDataBits(QString databits)
{
    d_ptr->port->setDataBits(QextSerialHelper::toDataBitsType(databits));
}

void QextQmlSerialPort::setParity(QString parity)
{
    d_ptr->port->setParity(QextSerialHelper::toParityType(parity));
}

void QextQmlSerialPort::setStopBits(QString stopbits)
{
    d_ptr->port->setStopBits(QextSerialHelper::toStopBitsType(stopbits));
}

void QextQmlSerialPort::setFlowControl(QString flow)
{
    d_ptr->port->setFlowControl(QextSerialHelper::toFlowType(flow));
}

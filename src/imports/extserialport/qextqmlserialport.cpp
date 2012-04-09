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

/*!
  \internal
*/
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

void QextQmlSerialPort::setConnected(bool connect)
{
    if (d_ptr->componentComplete) {
        d_ptr->doConnect(connect);
    } else {
        //delay the operation
        d_ptr->needConnect = connect;
    }
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

void QextQmlSerialPort::sendStringData(QString data)
{
    d_ptr->port->write(data.toLatin1());
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

void QextQmlSerialPort::setStringCodec(QString /*codec*/)
{

}

/*!
  \qmlproperty string ExtSerialPort::portName

   Gets and sets the port name associated with the serial port
*/
QString QextQmlSerialPort::portName()
{
    return d_ptr->port->portName();
}

void QextQmlSerialPort::setPortName(QString portName)
{
    d_ptr->port->setPortName(portName);
}


/*!
  \qmlproperty int ExtSerialPort::baudRate

   Gets and sets the baudRate associated with the serial port
*/
int QextQmlSerialPort::baudRate()
{
    return static_cast<int>(d_ptr->port->baudRate());
}

void QextQmlSerialPort::setBaudRate(int baudrate)
{
    d_ptr->port->setBaudRate(static_cast<QextSerialPort::BaudRateType>(baudrate));
}

/*!
  \qmlproperty ExtSerialPort::DataBitsType ExtSerialPort::dataBits

   Gets and sets the dataBits associated with the serial port

   \list
   \li ExtSerialPort.DATA_8
   \li ExtSerialPort.DATA_7
   \li ExtSerialPort.DATA_6
   \li ExtSerialPort.DATA_5
   \endlist
*/
QextQmlSerialPort::DataBitsType QextQmlSerialPort::dataBits()
{
    return static_cast<QextQmlSerialPort::DataBitsType>(d_ptr->port->dataBits());
}

void QextQmlSerialPort::setDataBits(QextQmlSerialPort::DataBitsType databits)
{
    d_ptr->port->setDataBits(static_cast<QextSerialPort::DataBitsType>(databits));
}

/*!
  \qmlproperty ExtSerialPort::ParityType ExtSerialPort::parity

   Gets and sets the parity associated with the serial port

   \list
   \li ExtSerialPort.PAR_NONE - (default)
   \li ExtSerialPort.PAR_ODD
   \li ExtSerialPort.PAR_EVEN
   \li ExtSerialPort.PAR_MARK - Windows only
   \li ExtSerialPort.PAR_SPACE
   \endlist
*/
QextQmlSerialPort::ParityType QextQmlSerialPort::parity()
{
    return static_cast<QextQmlSerialPort::ParityType>(d_ptr->port->parity());
}

void QextQmlSerialPort::setParity(QextQmlSerialPort::ParityType parity)
{
    d_ptr->port->setParity(static_cast<QextSerialPort::ParityType>(parity));
}

/*!
  \qmlproperty ExtSerialPort::StopBitsType ExtSerialPort::stopBits

   Gets and sets the stopBits associated with the serial port
   \list
   \li ExtSerialPort.STOP_1   - (default)
   \li ExtSerialPort.STOP_1_5 - Windows only
   \li ExtSerialPort.STOP_2
   \endlist
*/
QextQmlSerialPort::StopBitsType QextQmlSerialPort::stopBits()
{
    return static_cast<QextQmlSerialPort::StopBitsType>(d_ptr->port->stopBits());
}

void QextQmlSerialPort::setStopBits(QextQmlSerialPort::StopBitsType stopbits)
{
    d_ptr->port->setStopBits(static_cast<QextSerialPort::StopBitsType>(stopbits));
}

/*!
  \qmlproperty ExtSerialPort::FlowType ExtSerialPort::flowControl

   Gets and sets the flowControl associated with the serial port

   \list
   \li ExtSerialPort.FLOW_OFF        -   No flow control (default)
   \li ExtSerialPort.FLOW_HARDWARE   -   Hardware (RTS/CTS) flow control
   \li ExtSerialPort.FLOW_XONXOFF    -   Software (XON/XOFF) flow control
   \endlist
*/
QextQmlSerialPort::FlowType QextQmlSerialPort::flowControl()
{
    return static_cast<QextQmlSerialPort::FlowType>(d_ptr->port->flowControl());
}

void QextQmlSerialPort::setFlowControl(QextQmlSerialPort::FlowType flow)
{
    d_ptr->port->setFlowControl(static_cast<QextSerialPort::FlowType>(flow));
}

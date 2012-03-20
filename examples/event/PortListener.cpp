
#include "PortListener.h"
#include <QtDebug>

PortListener::PortListener(const QString & portName)
{
    qDebug() << "hi there";
    this->port = new QextSerialPort(portName);
    port->setBaudRate(QextSerialPort::BAUD9600);
    port->setFlowControl(QextSerialPort::FLOW_OFF);
    port->setParity(QextSerialPort::PAR_NONE);
    port->setDataBits(QextSerialPort::DATA_8);
    port->setStopBits(QextSerialPort::STOP_2);

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(port->lineStatus() & QextSerialPort::LS_DSR))
            qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << port->portName();
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
    }
}

void PortListener::onReadyRead()
{
    QByteArray bytes;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
    qDebug() << "bytes read:" << bytes.size();
    qDebug() << "bytes:" << bytes;
}

void PortListener::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}

/****************************************************************************
** Copyright (c) 2000-2003 Wayne Roth
** Copyright (c) 2004-2007 Stefan Sander
** Copyright (c) 2007 Michal Policht
** Copyright (c) 2008 Brandon Fosdick
** Copyright (c) 2009-2010 Liam Staskawicz
** Copyright (c) 2011 Debao Zhang
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

#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include <QtCore/QIODevice>
#include "qextserialport_global.h"
#ifdef Q_OS_UNIX
#include <termios.h>
#endif

class QextSerialPortPrivate;
class QEXTSERIALPORT_EXPORT QextSerialPort: public QIODevice
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QextSerialPort)
    Q_ENUMS(BaudRateType)
    Q_ENUMS(ParityType)
    Q_ENUMS(DataBitsType)
    Q_ENUMS(StopBitsType)
    Q_ENUMS(FlowType)
    Q_PROPERTY(QString portName READ portName WRITE setPortName)
    Q_PROPERTY(BaudRateType baudRate READ baudRate WRITE setBaudRate)
    Q_PROPERTY(DataBitsType dataBits READ dataBits WRITE setDataBits)
    Q_PROPERTY(ParityType parity READ parity WRITE setParity)
    Q_PROPERTY(StopBitsType stopBits READ stopBits WRITE setStopBits)
    Q_PROPERTY(FlowType flowControl READ flowControl WRITE setFlowControl)

public:
    enum LineID
    {
        LS_CTS =  0x01,
        LS_DSR =  0x02,
        LS_DCD =  0x04,
        LS_RI  =  0x08,
        LS_RTS =  0x10,
        LS_DTR =  0x20,
        LS_ST  =  0x40,
        LS_SR  =  0x80
    };

    enum ErrorNumber
    {
        E_NO_ERROR                 =  0,
        E_INVALID_FD               =  1,
        E_NO_MEMORY                =  2,
        E_CAUGHT_NON_BLOCKED_SIGNAL=  3,
        E_PORT_TIMEOUT             =  4,
        E_INVALID_DEVICE           =  5,
        E_BREAK_CONDITION          =  6,
        E_FRAMING_ERROR            =  7,
        E_IO_ERROR                 =  8,
        E_BUFFER_OVERRUN           =  9,
        E_RECEIVE_OVERFLOW         = 10,
        E_RECEIVE_PARITY_ERROR     = 11,
        E_TRANSMIT_OVERFLOW        = 12,
        E_READ_FAILED              = 13,
        E_WRITE_FAILED             = 14,
        E_FILE_NOT_FOUND           = 15,
        E_PERMISSION_DENIED        = 16,
        E_AGAIN                    = 17
    };

    enum BaudRateType
    {
#if defined(Q_OS_UNIX) || defined(qdoc)
        BAUD50 = 50,
        BAUD75 = 75,
        BAUD134 = 134,
        BAUD150 = 150,
        BAUD200 = 200,
        BAUD1800 = 1800,
#if defined(B76800) || defined(qdoc)
        BAUD76800 = 76800,
#endif
#if (defined(B230400) && defined(B4000000)) || defined(qdoc)
        BAUD230400 = 230400,
        BAUD460800 = 460800,
        BAUD500000 = 500000,
        BAUD576000 = 576000,
        BAUD921600 = 921600,
        BAUD1000000 = 1000000,
        BAUD1152000 = 1152000,
        BAUD1500000 = 1500000,
        BAUD2000000 = 2000000,
        BAUD2500000 = 2500000,
        BAUD3000000 = 3000000,
        BAUD3500000 = 3500000,
        BAUD4000000 = 4000000,
#endif
#endif //Q_OS_UNIX
#if defined(Q_OS_WIN) || defined(qdoc)
        BAUD14400 = 14400,
        BAUD56000 = 56000,
        BAUD128000 = 128000,
        BAUD256000 = 256000,
#endif //Q_OS_WIN
        BAUD110 = 110,
        BAUD300 = 300,
        BAUD600 = 600,
        BAUD1200 = 1200,
        BAUD2400 = 2400,
        BAUD4800 = 4800,
        BAUD9600 = 9600,
        BAUD19200 = 19200,
        BAUD38400 = 38400,
        BAUD57600 = 57600,
        BAUD115200 = 115200
    };

    enum DataBitsType
    {
        DATA_5 = 5,
        DATA_6 = 6,
        DATA_7 = 7,
        DATA_8 = 8
    };

    enum ParityType
    {
        PAR_NONE,
        PAR_ODD,
        PAR_EVEN,
#if defined(Q_OS_WIN) || defined(qdoc)
        PAR_MARK,
#endif
        PAR_SPACE = 4
    };

    enum StopBitsType
    {
        STOP_1,
#if defined(Q_OS_WIN) || defined(qdoc)
        STOP_1_5,
#endif
        STOP_2 = 2
    };

    enum FlowType
    {
        FLOW_OFF,
        FLOW_HARDWARE,
        FLOW_XONXOFF
    };

    explicit QextSerialPort(QObject *parent = 0);
    explicit QextSerialPort(const QString & name, QObject * parent = 0);
    QextSerialPort(const QString & name, BaudRateType baud, QObject * parent = 0);
    QextSerialPort(const QString & name, BaudRateType baud, ParityType par, DataBitsType dataBits, StopBitsType stopBits, QObject *parent=0);

    ~QextSerialPort();

    QString portName() const;
    BaudRateType baudRate() const;
    DataBitsType dataBits() const;
    ParityType parity() const;
    StopBitsType stopBits() const;
    FlowType flowControl() const;

    bool open(OpenMode mode);
    bool isSequential() const;
    void close();
    void flush();
    qint64 bytesAvailable() const;
    bool canReadLine() const;
    QByteArray readAll();

    ulong lastError() const;

    ulong lineStatus();
    QString errorString();

public Q_SLOTS:
    void setPortName(const QString & name);
    void setBaudRate(BaudRateType baudRate);
    void setDataBits(DataBitsType dataBits);
    void setParity(ParityType parity);
    void setStopBits(StopBitsType stopBits);
    void setFlowControl(FlowType flow);
    void setTimeout(long millisec);

    void setDtr(bool set=true);
    void setRts(bool set=true);

Q_SIGNALS:
    void dsrChanged(bool status);

protected:
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);

private:
    Q_DISABLE_COPY(QextSerialPort)

#ifdef Q_OS_WIN
    Q_PRIVATE_SLOT(d_func(), void _q_onWinEvent(HANDLE))
#endif
    Q_PRIVATE_SLOT(d_func(), void _q_canRead())

    QextSerialPortPrivate * const d_ptr;
};

#endif

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

#include "qextserialport.h"
#include "qextserialport_p.h"
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>

void QextSerialPortPrivate::platformSpecificInit()
{
    fd = 0;
    readNotifier = 0;
}

/*!
    Standard destructor.
*/
void QextSerialPortPrivate::platformSpecificDestruct()
{
}

bool QextSerialPortPrivate::open_sys(QIODevice::OpenMode mode)
{
    Q_Q(QextSerialPort);
    //note: linux 2.6.21 seems to ignore O_NDELAY flag
    if ((fd = ::open(port.toAscii() ,O_RDWR | O_NOCTTY | O_NDELAY)) != -1) {
        q->setOpenMode(mode);
        ::tcgetattr(fd, &old_termios);    // Save the old termios
        Posix_CommConfig = old_termios; // Make a working copy
        ::cfmakeraw(&Posix_CommConfig);   // Enable raw access

        /*set up other port settings*/
        Posix_CommConfig.c_cflag |= CREAD|CLOCAL;
        Posix_CommConfig.c_lflag &= (~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
        Posix_CommConfig.c_iflag &= (~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
        Posix_CommConfig.c_oflag &= (~OPOST);
        Posix_CommConfig.c_cc[VMIN] = 0;
#ifdef _POSIX_VDISABLE
        // Is a disable character available on this system?
        // Some systems allow for per-device disable-characters, so get the
        // proper value for the configured device
        const long vdisable = ::fpathconf(fd, _PC_VDISABLE);
        Posix_CommConfig.c_cc[VINTR] = vdisable;
        Posix_CommConfig.c_cc[VQUIT] = vdisable;
        Posix_CommConfig.c_cc[VSTART] = vdisable;
        Posix_CommConfig.c_cc[VSTOP] = vdisable;
        Posix_CommConfig.c_cc[VSUSP] = vdisable;
#endif //_POSIX_VDISABLE
        settingsDirtyFlags = DFE_ALL;
        updatePortSettings();

        if (Settings.QueryMode == QextSerialPort::EventDriven) {
            readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read, q);
            q->connect(readNotifier, SIGNAL(activated(int)), q, SLOT(_q_canRead()));
        }
        return true;
    } else {
        lastErr = QextSerialPort::E_FILE_NOT_FOUND;
        return false;
    }
}

bool QextSerialPortPrivate::close_sys()
{
    // Force a flush and then restore the original termios
    flush_sys();
    // Using both TCSAFLUSH and TCSANOW here discards any pending input
    ::tcsetattr(fd, TCSAFLUSH | TCSANOW, &old_termios);   // Restore termios
    ::close(fd);
    if(readNotifier) {
        delete readNotifier;
        readNotifier = 0;
    }
    return true;
}

bool QextSerialPortPrivate::flush_sys()
{
    ::tcdrain(fd);
    return true;
}

qint64 QextSerialPortPrivate::bytesAvailable_sys() const
{
    int bytesQueued;
    if (::ioctl(fd, FIONREAD, &bytesQueued) == -1) {
        return (qint64)-1;
    }
    return bytesQueued;
}

/*!
    Translates a system-specific error code to a QextSerialPort error code.  Used internally.
*/
void QextSerialPortPrivate::translateError(ulong error)
{
    switch (error) {
        case EBADF:
        case ENOTTY:
            lastErr = QextSerialPort::E_INVALID_FD;
            break;
        case EINTR:
            lastErr = QextSerialPort::E_CAUGHT_NON_BLOCKED_SIGNAL;
            break;
        case ENOMEM:
            lastErr = QextSerialPort::E_NO_MEMORY;
            break;
    }
}

void QextSerialPortPrivate::setDtr_sys(bool set)
{
    int status;
    ::ioctl(fd, TIOCMGET, &status);
    if (set)
        status |= TIOCM_DTR;
    else
        status &= ~TIOCM_DTR;
    ::ioctl(fd, TIOCMSET, &status);
}

void QextSerialPortPrivate::setRts_sys(bool set)
{
    int status;
    ::ioctl(fd, TIOCMGET, &status);
    if (set)
        status |= TIOCM_RTS;
    else
        status &= ~TIOCM_RTS;
    ::ioctl(fd, TIOCMSET, &status);
}

unsigned long QextSerialPortPrivate::lineStatus_sys()
{
    unsigned long Status=0, Temp=0;
    ::ioctl(fd, TIOCMGET, &Temp);
    if (Temp & TIOCM_CTS) Status |= QextSerialPort::LS_CTS;
    if (Temp & TIOCM_DSR) Status |= QextSerialPort::LS_DSR;
    if (Temp & TIOCM_RI ) Status |= QextSerialPort::LS_RI;
    if (Temp & TIOCM_CD ) Status |= QextSerialPort::LS_DCD;
    if (Temp & TIOCM_DTR) Status |= QextSerialPort::LS_DTR;
    if (Temp & TIOCM_RTS) Status |= QextSerialPort::LS_RTS;
    if (Temp & TIOCM_ST ) Status |= QextSerialPort::LS_ST;
    if (Temp & TIOCM_SR ) Status |= QextSerialPort::LS_SR;
    return Status;
}

/*!
    Reads a block of data from the serial port.  This function will read at most maxSize bytes from
    the serial port and place them in the buffer pointed to by data.  Return value is the number of
    bytes actually read, or -1 on error.
    
    \warning before calling this function ensure that serial port associated with this class
    is currently open (use isOpen() function to check if port is open).
*/
qint64 QextSerialPortPrivate::readData_sys(char * data, qint64 maxSize)
{
    int retVal = ::read(fd, data, maxSize);
    if (retVal == -1)
        lastErr = QextSerialPort::E_READ_FAILED;

    return retVal;
}

/*!
    Writes a block of data to the serial port.  This function will write maxSize bytes
    from the buffer pointed to by data to the serial port.  Return value is the number
    of bytes actually written, or -1 on error.
    
    \warning before calling this function ensure that serial port associated with this class
    is currently open (use isOpen() function to check if port is open).
*/
qint64 QextSerialPortPrivate::writeData_sys(const char * data, qint64 maxSize)
{
    int retVal = ::write(fd, data, maxSize);
    if (retVal == -1)
       lastErr = QextSerialPort::E_WRITE_FAILED;

    return (qint64)retVal;
}

static void setBaudRate2Termios(termios *config, int baudRate)
{
#ifdef CBAUD
    config->c_cflag &= (~CBAUD);
    config->c_cflag |= baudRate;
#else
    ::cfsetispeed(config, baudRate);
    ::cfsetospeed(config, baudRate);
#endif
}

/*

*/
void QextSerialPortPrivate::updatePortSettings()
{
    if (!q_func()->isOpen() || !settingsDirtyFlags)
        return;

    if (settingsDirtyFlags & DFE_BaudRate) {
        switch (Settings.BaudRate) {
        case QextSerialPort::BAUD50:
            setBaudRate2Termios(&Posix_CommConfig, B50);
            break;
        case QextSerialPort::BAUD75:
            setBaudRate2Termios(&Posix_CommConfig, B75);
            break;
        case QextSerialPort::BAUD110:
            setBaudRate2Termios(&Posix_CommConfig, B110);
            break;
        case QextSerialPort::BAUD134:
            setBaudRate2Termios(&Posix_CommConfig, B134);
            break;
        case QextSerialPort::BAUD150:
            setBaudRate2Termios(&Posix_CommConfig, B150);
            break;
        case QextSerialPort::BAUD200:
            setBaudRate2Termios(&Posix_CommConfig, B200);
            break;
        case QextSerialPort::BAUD300:
            setBaudRate2Termios(&Posix_CommConfig, B300);
            break;
        case QextSerialPort::BAUD600:
            setBaudRate2Termios(&Posix_CommConfig, B600);
            break;
        case QextSerialPort::BAUD1200:
            setBaudRate2Termios(&Posix_CommConfig, B1200);
            break;
        case QextSerialPort::BAUD1800:
            setBaudRate2Termios(&Posix_CommConfig, B1800);
            break;
        case QextSerialPort::BAUD2400:
            setBaudRate2Termios(&Posix_CommConfig, B2400);
            break;
        case QextSerialPort::BAUD4800:
            setBaudRate2Termios(&Posix_CommConfig, B4800);
            break;
        case QextSerialPort::BAUD9600:
            setBaudRate2Termios(&Posix_CommConfig, B9600);
            break;
        case QextSerialPort::BAUD19200:
            setBaudRate2Termios(&Posix_CommConfig, B19200);
            break;
        case QextSerialPort::BAUD38400:
            setBaudRate2Termios(&Posix_CommConfig, B38400);
            break;
        case QextSerialPort::BAUD57600:
            setBaudRate2Termios(&Posix_CommConfig, B57600);
            break;
#ifdef B76800
        case QextSerialPort::BAUD76800:
            setBaudRate2Termios(&Posix_CommConfig, B76800);
            break;
#endif
        case QextSerialPort::BAUD115200:
            setBaudRate2Termios(&Posix_CommConfig, B115200);
            break;
        case QextSerialPort::BAUDCustom:
            setBaudRate2Termios(&Posix_CommConfig, Settings.CustomBaudRate);
            break;
        }
    }
    if (settingsDirtyFlags & DFE_Parity) {
        switch (Settings.Parity) {
        case QextSerialPort::PAR_SPACE:
            /*space parity not directly supported - add an extra data bit to simulate it*/
            settingsDirtyFlags |= DFE_DataBits;
            break;
        case QextSerialPort::PAR_NONE:
            Posix_CommConfig.c_cflag &= (~PARENB);
            break;
        case QextSerialPort::PAR_EVEN:
            Posix_CommConfig.c_cflag &= (~PARODD);
            Posix_CommConfig.c_cflag |= PARENB;
            break;
        case QextSerialPort::PAR_ODD:
            Posix_CommConfig.c_cflag |= (PARENB|PARODD);
            break;
        }
    }
    /*must after Parity settings*/
    if (settingsDirtyFlags & DFE_DataBits) {
        if (Settings.Parity != QextSerialPort::PAR_SPACE) {
            Posix_CommConfig.c_cflag &= (~CSIZE);
            switch(Settings.DataBits) {
            case QextSerialPort::DATA_5:
                Posix_CommConfig.c_cflag |= CS5;
                break;
            case QextSerialPort::DATA_6:
                Posix_CommConfig.c_cflag |= CS6;
                break;
            case QextSerialPort::DATA_7:
                Posix_CommConfig.c_cflag |= CS7;
                break;
            case QextSerialPort::DATA_8:
                Posix_CommConfig.c_cflag |= CS8;
                break;
            }
        } else {
            /*space parity not directly supported - add an extra data bit to simulate it*/
            Posix_CommConfig.c_cflag &= ~(PARENB|CSIZE);
            switch(Settings.DataBits) {
            case QextSerialPort::DATA_5:
                Posix_CommConfig.c_cflag |= CS6;
                break;
            case QextSerialPort::DATA_6:
                Posix_CommConfig.c_cflag |= CS7;
                break;
            case QextSerialPort::DATA_7:
                Posix_CommConfig.c_cflag |= CS8;
                break;
            case QextSerialPort::DATA_8:
                /*this will never happen, put here to Suppress an warning*/
                break;
            }
        }
    }
    if (settingsDirtyFlags & DFE_StopBits) {
        switch (Settings.StopBits) {
        /*one stop bit*/
        case QextSerialPort::STOP_1:
            Posix_CommConfig.c_cflag &= (~CSTOPB);
            break;
        /*two stop bits*/
        case QextSerialPort::STOP_2:
            Posix_CommConfig.c_cflag |= CSTOPB;
            break;
        }
    }
    if (settingsDirtyFlags & DFE_Flow) {
        switch(Settings.FlowControl) {
        /*no flow control*/
        case QextSerialPort::FLOW_OFF:
            Posix_CommConfig.c_cflag &= (~CRTSCTS);
            Posix_CommConfig.c_iflag &= (~(IXON|IXOFF|IXANY));
            break;
        /*software (XON/XOFF) flow control*/
        case QextSerialPort::FLOW_XONXOFF:
            Posix_CommConfig.c_cflag &= (~CRTSCTS);
            Posix_CommConfig.c_iflag |= (IXON|IXOFF|IXANY);
            break;
        case QextSerialPort::FLOW_HARDWARE:
            Posix_CommConfig.c_cflag |= CRTSCTS;
            Posix_CommConfig.c_iflag &= (~(IXON|IXOFF|IXANY));
            break;
        }
    }

    /*if any thing in Posix_CommConfig changed, flush*/
    if (settingsDirtyFlags & DFE_Settings_Mask)
        ::tcsetattr(fd, TCSAFLUSH, &Posix_CommConfig);

    if (settingsDirtyFlags & DFE_TimeOut) {
        int millisec = Settings.Timeout_Millisec;
        if (millisec == -1) {
            ::fcntl(fd, F_SETFL, O_NDELAY);
        }
        else {
            //O_SYNC should enable blocking ::write()
            //however this seems not working on Linux 2.6.21 (works on OpenBSD 4.2)
            ::fcntl(fd, F_SETFL, O_SYNC);
        }
        ::tcgetattr(fd, & Posix_CommConfig);
        Posix_CommConfig.c_cc[VTIME] = millisec/100;
        ::tcsetattr(fd, TCSAFLUSH, & Posix_CommConfig);
    }

    settingsDirtyFlags = 0;
}

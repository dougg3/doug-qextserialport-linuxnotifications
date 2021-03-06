/****************************************************************************
** Copyright (c) 2000-2003 Wayne Roth
** Copyright (c) 2004-2007 Stefan Sander
** Copyright (c) 2007 Michal Policht
** Copyright (c) 2008 Brandon Fosdick
** Copyright (c) 2009-2010 Liam Staskawicz
** Copyright (c) 2011 Debao Zhang
** Copyright (c) 2012 Doug Brown
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

#include "qextserialenumerator.h"
#include "qextserialenumerator_p.h"
#include <QtCore/QDebug>

void QextSerialEnumeratorPrivate::platformSpecificInit()
{
    udev = NULL;
    monitor = NULL;
    notifierFd = -1;
    notifier = NULL;

    udev = udev_new();
    if (!udev)
    {
        qCritical() << "Unable to initialize udev notifications";
        return;
    }
}

void QextSerialEnumeratorPrivate::platformSpecificDestruct()
{
    if (notifier)
    {
        notifier->setEnabled(false);
        delete notifier;
    }

    if (monitor)
    {
        udev_monitor_unref(monitor);
    }

    if (udev)
    {
        udev_unref(udev);
    }
}

static QextPortInfo portInfoFromDevice(struct udev_device *dev)
{
    QString vendor = QString(udev_device_get_property_value(dev, "ID_VENDOR_ID"));
    QString product = QString(udev_device_get_property_value(dev, "ID_MODEL_ID"));
    QextPortInfo pi;

    // Convert the strings to hex numbers if we have a vendor/device ID
    bool vendorOK = false;
    bool prodOK = false;
    pi.vendorID = vendor.toInt(&vendorOK, 16);
    pi.productID = product.toInt(&prodOK, 16);
    if (!vendorOK) pi.vendorID = 0;
    if (!prodOK) pi.productID = 0;

    pi.portName = udev_device_get_devnode(dev);
    pi.physName = pi.portName;
    pi.friendName = "";
    pi.enumName = "";

    return pi;
}

QList<QextPortInfo> QextSerialEnumeratorPrivate::getPorts_sys()
{
    QList<QextPortInfo> returnVal;
    struct udev *ud = udev_new();
    if (!ud)
    {
        qCritical() << "Unable to enumerate ports because udev is not initialized.";
        return returnVal;
    }

    struct udev_enumerate *enumerate = udev_enumerate_new(ud);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry *list = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;
    udev_list_entry_foreach(entry, list)
    {
        const char *path;
        struct udev_device *dev;

        // Have to grab the actual udev device here...
        path = udev_list_entry_get_name(entry);
        dev = udev_device_new_from_syspath(ud, path);

        returnVal.append(portInfoFromDevice(dev));

        // Done with this device
        udev_device_unref(dev);
    }
    // Done with the list and this udev
    udev_enumerate_unref(enumerate);
    udev_unref(ud);

    return returnVal;
}

bool QextSerialEnumeratorPrivate::setUpNotifications_sys(bool setup)
{
    Q_UNUSED(setup);
    Q_Q(QextSerialEnumerator);
    if (!udev)
    {
        qCritical() << "Unable to initialize notifications because udev is not initialized.";
        return false;
    }

    // Emit signals immediately for devices already connected (Windows version seems to behave
    // this way)
    QList<QextPortInfo> l = getPorts_sys();
    foreach (QextPortInfo i, l)
    {
        Q_EMIT q->deviceDiscovered(i);
    }

    // Look for tty devices from udev.
    monitor = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "tty", NULL);
    udev_monitor_enable_receiving(monitor);
    notifierFd = udev_monitor_get_fd(monitor);
    notifier = new QSocketNotifier(notifierFd, QSocketNotifier::Read);
    q->connect(notifier, SIGNAL(activated(int)), q, SLOT(_q_deviceEvent()));
    notifier->setEnabled(true);

    return true;
}

void QextSerialEnumeratorPrivate::_q_deviceEvent()
{
    Q_Q(QextSerialEnumerator);
    struct udev_device *dev = udev_monitor_receive_device(monitor);
    if (dev)
    {
        QextPortInfo pi = portInfoFromDevice(dev);

        QString action = QString(udev_device_get_action(dev));
        udev_device_unref(dev);

        if (action == "add")
        {
            Q_EMIT q->deviceDiscovered(pi);
        }
        else if (action == "remove")
        {
            Q_EMIT q->deviceRemoved(pi);
        }
    }
}

#include "dialog.h"
#include "ui_dialog.h"
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include "qextserialport.h"
#include "qextserialhelper.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //! [0]
#ifdef Q_OS_WIN
    ui->portBox->addItems(QStringList()<<"COM1"<<"COM2"<<"COM3"<<"COM4");
#else
    ui->portBox->addItems(QStringList()<<"/dev/ttyS0"<<"/dev/ttyS1"<<"/dev/ttyUSB0"<<"/dev/ttyUSB1");
#endif
    //make sure user can input their own port name!
    ui->portBox->setEditable(true);

    ui->baudRateBox->addItems(QextSerialHelper::getBaudRateList());
    ui->baudRateBox->setCurrentIndex(3);

    ui->parityBox->addItems(QextSerialHelper::getParityList());

    ui->dataBitsBox->addItems(QextSerialHelper::getDataBitsList());
    ui->dataBitsBox->setCurrentIndex(3);

    ui->stopBitsBox->addItems(QextSerialHelper::getStopBitsList());
    //! [0]

    ui->led->turnOff();

    //! [1]
    port = new QextSerialPort(ui->portBox->currentText());
    port->setBaudRate(QextSerialPort::BAUD9600);
    port->setDataBits(QextSerialPort::DATA_8);
    port->setParity(QextSerialPort::PAR_NONE);
    port->setStopBits(QextSerialPort::STOP_1);
    port->setFlowControl(QextSerialPort::FLOW_OFF);
    //! [1]

    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onPortSettingsChanged()));
    connect(ui->parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onPortSettingsChanged()));
    connect(ui->dataBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onPortSettingsChanged()));
    connect(ui->stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onPortSettingsChanged()));
    connect(ui->timeoutBox, SIGNAL(valueChanged(int)), SLOT(onPortSettingsChanged()));
    connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(ui->openCloseButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
    connect(ui->sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));

    setWindowTitle(tr("QextSerialPort Demo"));
}

Dialog::~Dialog()
{
    delete ui;
    delete port;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::onPortNameChanged(const QString & /*name*/)
{
    if (port->isOpen()) {
        port->close();
        ui->led->turnOff();
    }
}
//! [2]

void Dialog::onPortSettingsChanged()
{
    port->setBaudRate(QextSerialHelper::toBaudRateType(ui->baudRateBox->currentText()));
    port->setParity(QextSerialHelper::toParityType(ui->parityBox->currentText()));
    port->setDataBits(QextSerialHelper::toDataBitsType(ui->dataBitsBox->currentText()));
    port->setStopBits(QextSerialHelper::toStopBitsType(ui->stopBitsBox->currentText()));
    port->setTimeout(ui->timeoutBox->value());
}

//! [2]
//! [3]
void Dialog::onOpenCloseButtonClicked()
{
    if (!port->isOpen()) {
        port->setPortName(ui->portBox->currentText());
        port->open(QIODevice::ReadWrite);
    }
    else {
        port->close();
    }

    ui->led->turnOn(port->isOpen());
}

//! [3]
//! [4]
void Dialog::onSendButtonClicked()
{
    if (port->isOpen() && !ui->sendEdit->toPlainText().isEmpty())
        port->write(ui->sendEdit->toPlainText().toLatin1());
}

void Dialog::onReadyRead()
{
    if (port->bytesAvailable()) {
        ui->recvEdit->moveCursor(QTextCursor::End);
        ui->recvEdit->insertPlainText(QString::fromLatin1(port->readAll()));
    }
}

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}
class QextSerialPort;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void onPortNameChanged(const QString &);
    void onPortSettingsChanged();
    void onOpenCloseButtonClicked();
    void onSendButtonClicked();
    void onReadyRead();

private:
    Ui::Dialog *ui;
    QextSerialPort *port;
};

#endif // DIALOG_H

#ifndef SECURITYHUMITURE_H
#define SECURITYHUMITURE_H

#include <QWidget>
#include "SHSerialPort.h"
#include <QMessageBox>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <SHNetworkMessage.h>
#include <SHTcpSocket.h>
namespace Ui {
class SecurityHumiture;
}

class SecurityHumiture : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityHumiture(QWidget *parent = nullptr,QString type = "");
    ~SecurityHumiture();

    void init(QString type);
    void setDeviceName(QString name);

private slots:
    void updateHumitureData(QString temp,QString humi);
    void updateFireAlarm();
    void updateSmokeAlarm();

    void on_m_ckbWarning_stateChanged(int arg1);

private:
    Ui::SecurityHumiture *ui;
    SHTcpSocket *tcpClient;

    bool alarmToCommunity;
};

#endif // SECURITYHUMITURE_H

#ifndef SHSERIALPORT_H
#define SHSERIALPORT_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QBuffer>
#include <SHControls.h>
#include "HouseCtlConditon.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTimer>
namespace Ui {
class SHSerialPort;
}

class SHSerialPort : public QSerialPort
{
    Q_OBJECT
private:
    SHSerialPort();
    ~SHSerialPort();
    static SHSerialPort *instance;

public:
    void serialDataHandle(QByteArray  serial_buf);
    void msgProcessEvent(changeMsg msg);
    void eventAddrHandle(changeMsg msg);
    void eventDHT11Msg(changeMsg msg);
    void eventFireAlarmMsg(changeMsg msg);
    void eventSmokeAlarmMsg(changeMsg msg);

    static SHSerialPort* getInstance();

    bool static fireAlarmDlgExist;
    bool static smokeAlarmDlgExist;
    QTimer *m_timerFireAlarmDlg;
    QTimer *m_timerSmokeAlarmDlg;
private slots:
    void readData();
    void delayFireAlarmDlgRepeat();
    void delaySmokeAlarmDlgRepeat();

signals:
    void sendSerialMessage();
    void messageHumiture(QString temp,QString humi);
    void messageFireAlarm();
    void messageSmokeAlarm();
};
#endif // SHSERIALPORT_H

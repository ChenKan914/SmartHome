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

    bool delayfireAlarmDlg = false;
public:
    void serialDataHandle(QByteArray  serial_buf);
    void msgProcessEvent(changeMsg msg);
    void eventAddrHandle(changeMsg msg);
    void eventDHT11Msg(changeMsg msg);
    void eventFireAlarmMsg(changeMsg msg);

    static SHSerialPort* getInstance();

    bool static fireAlarmDlgExist;
    QTimer *m_timer;
private slots:
    void readData();
    void delayFireAlarmDlgRepeat();

signals:
    void sendSerialMessage();
    void messageHumiture(QString temp,QString humi);
    void messageFireAlarm();

};
#endif // SHSERIALPORT_H

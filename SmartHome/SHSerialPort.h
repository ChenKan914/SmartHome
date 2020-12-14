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
#include <SH_Controls.h>
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

public:
    explicit SHSerialPort(QWidget *parent = nullptr);
    ~SHSerialPort();


    void serialDataHandle(QByteArray  serial_buf);
    void msgProcessEvent(changeMsg msg);
    void eventAddrHandle(changeMsg msg);
    void eventDHT11Msg(changeMsg msg);


private slots:
    void readData();

signals:
    void sendSerialMessage();
    void messageHumiture(QString temp,QString humi);
};
#endif // SHSERIALPORT_H
